#include "ParserPql.h"
#include "catch.hpp"
#include <iostream>
#include <vector>

TEST_CASE("QueryMap with no such that and no pattern") {
  SECTION("while") {
    std::string query = "while w; Select w";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        DECLARATION, PayLoad(SINGLE, WHILE, std::vector<std::string>{"w"}));
    expectedQueryMap.addItem(
        SELECT, PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"w"}));

    QueryMap actualQueryMap = pqlParse(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("stmt") {
    std::string query = "stmt s; Select s";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        DECLARATION, PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s"}));
    expectedQueryMap.addItem(
        SELECT, PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"s"}));

    QueryMap actualQueryMap = pqlParse(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("read") {
    std::string query = "read re; Select re";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        DECLARATION, PayLoad(SINGLE, READ, std::vector<std::string>{"re"}));
    expectedQueryMap.addItem(
        SELECT, PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"re"}));

    QueryMap actualQueryMap = pqlParse(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }
}

TEST_CASE("QueryMap repeated declaration with no such that and no pattern") {
  SECTION("while") {
    std::string query = "while w1, w2, w3; Select w1";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        DECLARATION, PayLoad(SINGLE, WHILE, std::vector<std::string>{"w1"}));
    expectedQueryMap.addItem(
        DECLARATION, PayLoad(SINGLE, WHILE, std::vector<std::string>{"w2"}));
    expectedQueryMap.addItem(
        DECLARATION, PayLoad(SINGLE, WHILE, std::vector<std::string>{"w3"}));
    expectedQueryMap.addItem(
        SELECT, PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"w1"}));

    QueryMap actualQueryMap = pqlParse(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("stmt") {
    std::string query = "stmt s1, s2, s3; Select s1";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        DECLARATION,
        PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        DECLARATION,
        PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s2"}));
    expectedQueryMap.addItem(
        DECLARATION,
        PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s3"}));
    expectedQueryMap.addItem(
        SELECT, PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"s1"}));

    QueryMap actualQueryMap = pqlParse(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("read") {
    std::string query = "read re1, re2,re3; Select re1";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        DECLARATION, PayLoad(SINGLE, READ, std::vector<std::string>{"re1"}));
    expectedQueryMap.addItem(
        DECLARATION, PayLoad(SINGLE, READ, std::vector<std::string>{"re2"}));
    expectedQueryMap.addItem(
        DECLARATION, PayLoad(SINGLE, READ, std::vector<std::string>{"re3"}));
    expectedQueryMap.addItem(
        SELECT, PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"re1"}));

    QueryMap actualQueryMap = pqlParse(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }
}

// TODO(zs)
TEST_CASE("QueryMap with one such that") {}
// TODO(zs)
TEST_CASE("QueryMap with one pattern") {}
// TODO(zs)
TEST_CASE("QueryMap with one such that and one pattern") {}
