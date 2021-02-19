#include "../../../lib/catch.hpp"
#include "../../spa/src/ParserLib.h"
#include "../../spa/src/ParserPql.h"
#include <iostream>
#include <vector>

TEST_CASE("QueryMap ClauseType::declaration") {
  SECTION("Multi single groups") {
    std::string query = "while w1; stmt s1;";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"w1"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}));

    QueryMap actualQueryMap = pql_query(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("Single multiple group") {
    std::string query = "while w1, w2;";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"w1"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"w2"}));

    QueryMap actualQueryMap = pql_query(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("Multi multiple groups") {
    std::string query = "while w1, w2; stmt s1, s2;";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"w1"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"w2"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s2"}));

    QueryMap actualQueryMap = pql_query(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }
}

TEST_CASE(
    "QueryMap with no ClauseType::declaration, no such that, no pattern") {
  std::string query = "Select w";
  QueryMap expectedQueryMap;
  expectedQueryMap.addItem(
      ClauseType::SELECT,
      PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"w"}));

  QueryMap actualQueryMap = pql_query(query);
  REQUIRE(expectedQueryMap == actualQueryMap);
}

TEST_CASE("QueryMap with no such that and no pattern") {
  SECTION("while") {
    std::string query = "while w; Select w";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"w"}));
    expectedQueryMap.addItem(
        ClauseType::SELECT,
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"w"}));

    QueryMap actualQueryMap = pql_query(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("stmt") {
    std::string query = "stmt s; Select s";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"}));
    expectedQueryMap.addItem(
        ClauseType::SELECT,
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"s"}));

    QueryMap actualQueryMap = pql_query(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("read") {
    std::string query = "read re; Select re";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::READ, std::vector<std::string>{"re"}));
    expectedQueryMap.addItem(
        ClauseType::SELECT,
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"re"}));

    QueryMap actualQueryMap = pql_query(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }
}

TEST_CASE("QueryMap repeated declaration with no such that and no pattern") {
  SECTION("while") {
    std::string query = "while w1, w2, w3; Select w1";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"w1"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"w2"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"w3"}));
    expectedQueryMap.addItem(
        ClauseType::SELECT,
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"w1"}));

    QueryMap actualQueryMap = pql_query(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("stmt") {
    std::string query = "stmt s1, s2, s3; Select s1";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s2"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s3"}));
    expectedQueryMap.addItem(
        ClauseType::SELECT,
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"}));

    QueryMap actualQueryMap = pql_query(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("read") {
    std::string query = "read re1, re2, re3; Select re1";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::READ, std::vector<std::string>{"re1"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::READ, std::vector<std::string>{"re2"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::READ, std::vector<std::string>{"re3"}));
    expectedQueryMap.addItem(
        ClauseType::SELECT,
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"re1"}));

    QueryMap actualQueryMap = pql_query(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }
}

TEST_CASE("one such that Parent") {
  SECTION("synonym and synonym") {
    std::string query = "stmt s1, s2; Select s1 such that Parent(s1, s2)";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s2"}));
    expectedQueryMap.addItem(
        ClauseType::SELECT,
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::SUCHTHAT,
        PayLoad(PAIR, PARENT, std::vector<std::string>{"s1", "s2"}));

    QueryMap actualQueryMap = pql_query(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("synonym and wildcard") {
    std::string query = "stmt s1; Select s1 such that Parent(s1, _)";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::SELECT,
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::SUCHTHAT,
        PayLoad(PAIR, PARENT, std::vector<std::string>{"s1", "_"}));

    QueryMap actualQueryMap = pql_query(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("synonym and integer") {
    std::string query = "stmt s1; Select s1 such that Parent(s1, 1)";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::SELECT,
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::SUCHTHAT,
        PayLoad(PAIR, PARENT, std::vector<std::string>{"s1", "1"}));

    QueryMap actualQueryMap = pql_query(query);
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
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::SELECT,
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::SUCHTHAT,
        PayLoad(PAIR, PARENT, std::vector<std::string>{"1", "s1"}));

    QueryMap actualQueryMap = pql_query(query);
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
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s2"}));
    expectedQueryMap.addItem(
        ClauseType::SELECT,
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::SUCHTHAT,
        PayLoad(PAIR, PARENTT, std::vector<std::string>{"s1", "s2"}));

    QueryMap actualQueryMap = pql_query(query);
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
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s2"}));
    expectedQueryMap.addItem(
        ClauseType::SELECT,
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::SUCHTHAT,
        PayLoad(PAIR, FOLLOWS, std::vector<std::string>{"s1", "s2"}));

    QueryMap actualQueryMap = pql_query(query);
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
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s2"}));
    expectedQueryMap.addItem(
        ClauseType::SELECT,
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::SUCHTHAT,
        PayLoad(PAIR, FOLLOWST, std::vector<std::string>{"s1", "s2"}));

    QueryMap actualQueryMap = pql_query(query);
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
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s2"}));
    expectedQueryMap.addItem(
        ClauseType::SELECT,
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::SUCHTHAT,
        PayLoad(PAIR, MODIFIES, std::vector<std::string>{"s1", "s2"}));

    QueryMap actualQueryMap = pql_query(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  // TODO(zs)
  SECTION("synonym and wildcard") {}

  SECTION("synonym and \"ident\"") {
    std::string query = "stmt s1; Select s1 such that Modifies(s1, \"x\")";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::SELECT,
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::SUCHTHAT,
        PayLoad(PAIR, MODIFIES, std::vector<std::string>{"s1", "\"x\""}));

    QueryMap actualQueryMap = pql_query(query);
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
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s2"}));
    expectedQueryMap.addItem(
        ClauseType::SELECT,
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::SUCHTHAT,
        PayLoad(PAIR, USES, std::vector<std::string>{"s1", "s2"}));

    QueryMap actualQueryMap = pql_query(query);
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
    std::string query = "stmt s; assign a; Select a pattern a(s, _\"var\"_)";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::ASSIGN, std::vector<std::string>{"a"}));
    expectedQueryMap.addItem(
        ClauseType::SELECT,
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"a"}));
    expectedQueryMap.addItem(
        ClauseType::PATTERN,
        PayLoad(TRIPLE, SYN_ASSIGN,
                std::vector<std::string>{"a", "s", "_\"var\"_"}));

    QueryMap actualQueryMap = pql_query(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("synonym and _") {
    std::string query = "stmt s; assign a; Select a pattern a(s, _)";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::ASSIGN, std::vector<std::string>{"a"}));
    expectedQueryMap.addItem(
        ClauseType::SELECT,
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"a"}));
    expectedQueryMap.addItem(
        ClauseType::PATTERN,
        PayLoad(TRIPLE, SYN_ASSIGN, std::vector<std::string>{"a", "s", "_"}));

    QueryMap actualQueryMap = pql_query(query);
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
TEST_CASE("one such that and one pattern") {
  SECTION("parent and pattern") {
    std::string query = "stmt s1, s2; assign a; Select a such that Parent(s1, "
                        "s2) pattern a(s1, _)";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s2"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::ASSIGN, std::vector<std::string>{"a"}));
    expectedQueryMap.addItem(
        ClauseType::SELECT,
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"a"}));
    expectedQueryMap.addItem(
        ClauseType::SUCHTHAT,
        PayLoad(PAIR, PARENT, std::vector<std::string>{"s1", "s2"}));
    expectedQueryMap.addItem(
        ClauseType::PATTERN,
        PayLoad(TRIPLE, SYN_ASSIGN, std::vector<std::string>{"a", "s1", "_"}));

    QueryMap actualQueryMap = pql_query(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("modifies and pattern") {
    std::string query = "stmt s; assign a; Select a such that Modifies(s, "
                        "\"var\") pattern a(s, _)";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::ASSIGN, std::vector<std::string>{"a"}));
    expectedQueryMap.addItem(
        ClauseType::SELECT,
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"a"}));
    expectedQueryMap.addItem(
        ClauseType::SUCHTHAT,
        PayLoad(PAIR, MODIFIES, std::vector<std::string>{"s", "\"var\""}));
    expectedQueryMap.addItem(
        ClauseType::PATTERN,
        PayLoad(TRIPLE, SYN_ASSIGN, std::vector<std::string>{"a", "s", "_"}));

    QueryMap actualQueryMap = pql_query(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("modifies and pattern") {
    std::string query =
        "while w1, w2; stmt s; assign a; Select a such that Modifies(s, "
        "\"var\") pattern a(s, _\"123\"_)";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"w1"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"w2"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::ASSIGN, std::vector<std::string>{"a"}));
    expectedQueryMap.addItem(
        ClauseType::SELECT,
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"a"}));
    expectedQueryMap.addItem(
        ClauseType::SUCHTHAT,
        PayLoad(PAIR, MODIFIES, std::vector<std::string>{"s", "\"var\""}));
    expectedQueryMap.addItem(
        ClauseType::PATTERN,
        PayLoad(TRIPLE, SYN_ASSIGN,
                std::vector<std::string>{"a", "s", "_\"123\"_"}));

    QueryMap actualQueryMap = pql_query(query);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }
}
