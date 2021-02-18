#include "../../../lib/catch.hpp"
#include "../../spa/src/ParserLib.h"
#include "../../spa/src/ParserPql.h"
#include <iostream>
#include <vector>

TEST_CASE("QueryMap declaration") {
  SECTION("Multi single groups") {
    std::string query = "while w1; stmt s1;";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        DECLARATION, PayLoad(SINGLE, WHILE, std::vector<std::string>{"w1"}));
    expectedQueryMap.addItem(
        DECLARATION,
        PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s1"}));

    QueryMap actualQueryMap = pqlParse(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("Single multiple group") {
    std::string query = "while w1, w2;";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        DECLARATION, PayLoad(SINGLE, WHILE, std::vector<std::string>{"w1"}));
    expectedQueryMap.addItem(
        DECLARATION, PayLoad(SINGLE, WHILE, std::vector<std::string>{"w2"}));

    QueryMap actualQueryMap = pqlParse(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("Multi multiple groups") {
    std::string query = "while w1, w2; stmt s1, s2;";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        DECLARATION, PayLoad(SINGLE, WHILE, std::vector<std::string>{"w1"}));
    expectedQueryMap.addItem(
        DECLARATION, PayLoad(SINGLE, WHILE, std::vector<std::string>{"w2"}));
    expectedQueryMap.addItem(
        DECLARATION,
        PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        DECLARATION,
        PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s2"}));

    QueryMap actualQueryMap = pqlParse(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }
}

TEST_CASE("QueryMap with no declaration, no such that, no pattern") {
  std::string query = "Select w";
  QueryMap expectedQueryMap;
  expectedQueryMap.addItem(
      SELECT, PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"w"}));

  QueryMap actualQueryMap = pqlParse(query);
  REQUIRE(expectedQueryMap == actualQueryMap);
}

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
    std::string query = "read re1, re2, re3; Select re1";
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
TEST_CASE("QueryMap with one such that") {
  SECTION("Parent") {
    std::string query = "stmt s1, s2; Select s1 such that Parent(s1, s2)";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        DECLARATION,
        PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        DECLARATION,
        PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s2"}));
    expectedQueryMap.addItem(
        SELECT, PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        SUCHTHAT, PayLoad(PAIR, PARENT, std::vector<std::string>{"s1", "s2"}));

    QueryMap actualQueryMap = pqlParse(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("Parent*") {
    std::string query = "stmt s1, s2; Select s1 such that Parent*(s1, s2)";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        DECLARATION,
        PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        DECLARATION,
        PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s2"}));
    expectedQueryMap.addItem(
        SELECT, PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        SUCHTHAT, PayLoad(PAIR, PARENTT, std::vector<std::string>{"s1", "s2"}));

    QueryMap actualQueryMap = pqlParse(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("Follows") {
    std::string query = "stmt s1, s2; Select s1 such that Follows(s1, s2)";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        DECLARATION,
        PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        DECLARATION,
        PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s2"}));
    expectedQueryMap.addItem(
        SELECT, PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        SUCHTHAT, PayLoad(PAIR, FOLLOWS, std::vector<std::string>{"s1", "s2"}));

    QueryMap actualQueryMap = pqlParse(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("Follows*") {
    std::string query = "stmt s1, s2; Select s1 such that Follows*(s1, s2)";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        DECLARATION,
        PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        DECLARATION,
        PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s2"}));
    expectedQueryMap.addItem(
        SELECT, PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        SUCHTHAT,
        PayLoad(PAIR, FOLLOWST, std::vector<std::string>{"s1", "s2"}));

    QueryMap actualQueryMap = pqlParse(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }
}
// TODO(zs)
TEST_CASE("QueryMap with one pattern") {}
// TODO(zs)
TEST_CASE("QueryMap with one such that and one pattern") {}
