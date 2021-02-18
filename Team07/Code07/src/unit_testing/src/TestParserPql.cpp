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

TEST_CASE("one such that Parent") {
  SECTION("synonym and synonym") {
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

  SECTION("synonym and wildcard") {
    std::string query = "stmt s1; Select s1 such that Parent(s1, _)";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        DECLARATION,
        PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        SELECT, PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        SUCHTHAT, PayLoad(PAIR, PARENT, std::vector<std::string>{"s1", "_"}));

    QueryMap actualQueryMap = pqlParse(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("synonym and integer") {
    std::string query = "stmt s1; Select s1 such that Parent(s1, 1)";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        DECLARATION,
        PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        SELECT, PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        SUCHTHAT, PayLoad(PAIR, PARENT, std::vector<std::string>{"s1", "1"}));

    QueryMap actualQueryMap = pqlParse(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  // TODO(zs)
  SECTION("wildcard and synonym") {}

  // TODO(zs)
  SECTION("wildcard and wildcard") {}

  // TODO(zs)
  SECTION("wildcard and integer") {}

  SECTION("integer and synonym") {
    std::string query = "stmt s1; Select s1 such that Parent(1, s1)";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        DECLARATION,
        PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        SELECT, PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        SUCHTHAT, PayLoad(PAIR, PARENT, std::vector<std::string>{"1", "s1"}));

    QueryMap actualQueryMap = pqlParse(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  // TODO(zs)
  SECTION("integer and wildcard") {}

  // TODO(zs)
  SECTION("integer and integer") {}
}

TEST_CASE("one such that Parent*") {

  SECTION("synonym and synonym") {
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

  // TODO(zs)
  SECTION("synonym and wildcard") {}

  // TODO(zs)
  SECTION("synonym and integer") {}

  // TODO(zs)
  SECTION("wildcard and synonym") {}

  // TODO(zs)
  SECTION("wildcard and wildcard") {}

  // TODO(zs)
  SECTION("wildcard and integer") {}

  // TODO(zs)
  SECTION("integer and synonym") {}

  // TODO(zs)
  SECTION("integer and wildcard") {}

  // TODO(zs)
  SECTION("integer and integer") {}
}

TEST_CASE("one such that Follows") {

  SECTION("synonym and synonym") {
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

  // TODO(zs)
  SECTION("synonym and wildcard") {}

  // TODO(zs)
  SECTION("synonym and integer") {}

  // TODO(zs)
  SECTION("wildcard and synonym") {}

  // TODO(zs)
  SECTION("wildcard and wildcard") {}

  // TODO(zs)
  SECTION("wildcard and integer") {}

  // TODO(zs)
  SECTION("integer and synonym") {}

  // TODO(zs)
  SECTION("integer and wildcard") {}

  // TODO(zs)
  SECTION("integer and integer") {}
}

TEST_CASE("one such that Follows*") {

  SECTION("synonym and synonym") {
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

  // TODO(zs)
  SECTION("synonym and wildcard") {}

  // TODO(zs)
  SECTION("synonym and integer") {}

  // TODO(zs)
  SECTION("wildcard and synonym") {}

  // TODO(zs)
  SECTION("wildcard and wildcard") {}

  // TODO(zs)
  SECTION("wildcard and integer") {}

  // TODO(zs)
  SECTION("integer and synonym") {}

  // TODO(zs)
  SECTION("integer and wildcard") {}

  // TODO(zs)
  SECTION("integer and integer") {}
}

TEST_CASE("one such that Modifies") {

  SECTION("synonym and synonym") {
    std::string query = "stmt s1, s2; Select s1 such that Modifies(s1, s2)";
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
        PayLoad(PAIR, MODIFIES, std::vector<std::string>{"s1", "s2"}));

    QueryMap actualQueryMap = pqlParse(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  // TODO(zs)
  SECTION("synonym and wildcard") {}

  SECTION("synonym and \"idnet\"") {
    std::string query = "stmt s1; Select s1 such that Modifies(s1, \"x\")";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        DECLARATION,
        PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        SELECT, PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        SUCHTHAT,
        PayLoad(PAIR, MODIFIES, std::vector<std::string>{"s1", "\"x\""}));

    QueryMap actualQueryMap = pqlParse(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  // TODO(zs)
  SECTION("wildcard and synonym") {}

  // TODO(zs)
  SECTION("wildcard and wildcard") {}

  // TODO(zs)
  SECTION("wildcard and \"idnet\"") {}

  // TODO(zs)
  SECTION("\"idnet\" and synonym") {}

  // TODO(zs)
  SECTION("\"idnet\" and wildcard") {}

  // TODO(zs)
  SECTION("\"idnet\" and \"idnet\"") {}
}

TEST_CASE("one such that Uses") {

  SECTION("synonym and synonym") {
    std::string query = "stmt s1, s2; Select s1 such that Uses(s1, s2)";
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
        SUCHTHAT, PayLoad(PAIR, USES, std::vector<std::string>{"s1", "s2"}));

    QueryMap actualQueryMap = pqlParse(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  // TODO(zs)
  SECTION("synonym and wildcard") {}

  // TODO(zs)
  SECTION("synonym and integer") {}

  // TODO(zs)
  SECTION("wildcard and synonym") {}

  // TODO(zs)
  SECTION("wildcard and wildcard") {}

  // TODO(zs)
  SECTION("wildcard and integer") {}

  // TODO(zs)
  SECTION("integer and synonym") {}

  // TODO(zs)
  SECTION("integer and wildcard") {}

  // TODO(zs)
  SECTION("integer and integer") {}
}

// TODO(zs)
TEST_CASE("one pattern syn_assign, ent_ref and expr_spec ") {
  SECTION("synonym and _factor_") {
    std::string query = "stmt s; assign a; Select a pattern a(s, _var_)";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        DECLARATION, PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s"}));
    expectedQueryMap.addItem(
        DECLARATION, PayLoad(SINGLE, ASSIGN, std::vector<std::string>{"a"}));
    expectedQueryMap.addItem(
        SELECT, PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"a"}));
    expectedQueryMap.addItem(
        PATTERN, PayLoad(TRIPLE, SYN_ASSIGN,
                         std::vector<std::string>{"a", "s", "_var_"}));

    QueryMap actualQueryMap = pqlParse(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("synonym and _") {
    std::string query = "stmt s; assign a; Select a pattern a(s, _)";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        DECLARATION, PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s"}));
    expectedQueryMap.addItem(
        DECLARATION, PayLoad(SINGLE, ASSIGN, std::vector<std::string>{"a"}));
    expectedQueryMap.addItem(
        SELECT, PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"a"}));
    expectedQueryMap.addItem(
        PATTERN,
        PayLoad(TRIPLE, SYN_ASSIGN, std::vector<std::string>{"a", "s", "_"}));

    QueryMap actualQueryMap = pqlParse(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  // TODO(zs)
  SECTION("_ and _factor_") {}

  // TODO(zs)
  SECTION("_ and _") {}

  // TODO(zs)
  SECTION("ident and _factor_") {}

  // TODO(zs)
  SECTION("ident and _") {}
}

// TODO(zs)
TEST_CASE("QueryMap with one such that and one pattern") {}
