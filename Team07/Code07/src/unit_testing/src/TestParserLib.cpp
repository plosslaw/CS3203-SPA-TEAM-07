
#include <string>
#include "../../spa/src/TNode.h"
#include "../../spa/src/ParserLib.h"
#include "../../../lib/catch.hpp"

TEST_CASE("predicates") {
  SECTION("alphanum") {
    REQUIRE(alpha_num_pred('a'));
    REQUIRE(alpha_num_pred('A'));
    REQUIRE(alpha_num_pred('1'));
    REQUIRE_FALSE(alpha_num_pred('.'));
  }

  SECTION("digit") {
    REQUIRE(digit_pred('1'));
    REQUIRE_FALSE(digit_pred('a'));
    REQUIRE_FALSE(digit_pred('.'));
  }

  SECTION("alpha") {
    REQUIRE(alpha_pred('a'));
    REQUIRE(alpha_pred('A'));
    REQUIRE_FALSE(alpha_pred('1'));
    REQUIRE_FALSE(alpha_pred('.'));
  }

  SECTION("lower") {
    REQUIRE(lower_pred('a'));
    REQUIRE_FALSE(lower_pred('A'));
    REQUIRE_FALSE(lower_pred('.'));
  }

  SECTION("upper") {
    REQUIRE_FALSE(upper_pred('a'));
    REQUIRE(upper_pred('A'));
    REQUIRE_FALSE(upper_pred('.'));
  }
  
  SECTION("whitespace") {
    REQUIRE(whitespace_pred(' '));
    REQUIRE(whitespace_pred('\n'));
    REQUIRE_FALSE(whitespace_pred('.'));
  }
}

int check_string_pred(std::string str, bool (*pred)(char)) {
  std::string strv = str;
  State state(&str);
  string_predicate(state, pred, "");
  return state.i;
}

TEST_CASE("parsers") {
  SECTION("alphanum") {
    REQUIRE(check_string_pred("a", &alpha_num_pred) == 1);
    REQUIRE(check_string_pred("A", &alpha_num_pred) == 1);
    REQUIRE(check_string_pred("1", &alpha_num_pred) == 1);
    REQUIRE(check_string_pred(".", &alpha_num_pred) == 0);
    REQUIRE(check_string_pred(" ", &alpha_num_pred) == 0);
  }

  SECTION("digit") {
    REQUIRE(check_string_pred("a", &digit_pred) == 0);
    REQUIRE(check_string_pred("A", &digit_pred) == 0);
    REQUIRE(check_string_pred("1", &digit_pred) == 1);
    REQUIRE(check_string_pred(".", &digit_pred) == 0);
    REQUIRE(check_string_pred(" ", &digit_pred) == 0);
  }

  SECTION("alpha") {
    REQUIRE(check_string_pred("a", &alpha_pred) == 1);
    REQUIRE(check_string_pred("A", &alpha_pred) == 1);
    REQUIRE(check_string_pred("1", &alpha_pred) == 0);
    REQUIRE(check_string_pred(".", &alpha_pred) == 0);
    REQUIRE(check_string_pred(" ", &alpha_pred) == 0);
  }

  SECTION("lower") {
    REQUIRE(check_string_pred("a", &lower_pred) == 1);
    REQUIRE(check_string_pred("A", &lower_pred) == 0);
    REQUIRE(check_string_pred("1", &lower_pred) == 0);
    REQUIRE(check_string_pred(".", &lower_pred) == 0);
    REQUIRE(check_string_pred(" ", &lower_pred) == 0);
  }

  SECTION("upper") {
    REQUIRE(check_string_pred("a", &upper_pred) == 0);
    REQUIRE(check_string_pred("A", &upper_pred) == 1);
    REQUIRE(check_string_pred("1", &upper_pred) == 0);
    REQUIRE(check_string_pred(".", &upper_pred) == 0);
    REQUIRE(check_string_pred(" ", &upper_pred) == 0);
  }
  
  SECTION("whitespace") {
    REQUIRE(check_string_pred("a", &whitespace_pred) == 0);
    REQUIRE(check_string_pred("A", &whitespace_pred) == 0);
    REQUIRE(check_string_pred(".", &whitespace_pred) == 0);
    REQUIRE(check_string_pred("\n", &whitespace_pred) == 1);
    REQUIRE(check_string_pred(" ", &whitespace_pred) == 1);
  }
}


