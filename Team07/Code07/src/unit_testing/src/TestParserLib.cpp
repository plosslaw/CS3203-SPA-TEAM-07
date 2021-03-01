
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

int checkStringPred(std::string str, bool (*pred)(char)) {
  std::string strv = str;
  State state(&str);
  string_predicate(state, pred, "");
  return state.i;
}

TEST_CASE("parsers") {
  SECTION("alphanum") {
    REQUIRE(checkStringPred("a", &alpha_num_pred) == 1);
    REQUIRE(checkStringPred("A", &alpha_num_pred) == 1);
    REQUIRE(checkStringPred("1", &alpha_num_pred) == 1);
    REQUIRE(checkStringPred(".", &alpha_num_pred) == 0);
    REQUIRE(checkStringPred(" ", &alpha_num_pred) == 0);
  }

  SECTION("digit") {
    REQUIRE(checkStringPred("a", &digit_pred) == 0);
    REQUIRE(checkStringPred("A", &digit_pred) == 0);
    REQUIRE(checkStringPred("1", &digit_pred) == 1);
    REQUIRE(checkStringPred(".", &digit_pred) == 0);
    REQUIRE(checkStringPred(" ", &digit_pred) == 0);
  }

  SECTION("alpha") {
    REQUIRE(checkStringPred("a", &alpha_pred) == 1);
    REQUIRE(checkStringPred("A", &alpha_pred) == 1);
    REQUIRE(checkStringPred("1", &alpha_pred) == 0);
    REQUIRE(checkStringPred(".", &alpha_pred) == 0);
    REQUIRE(checkStringPred(" ", &alpha_pred) == 0);
  }

  SECTION("lower") {
    REQUIRE(checkStringPred("a", &lower_pred) == 1);
    REQUIRE(checkStringPred("A", &lower_pred) == 0);
    REQUIRE(checkStringPred("1", &lower_pred) == 0);
    REQUIRE(checkStringPred(".", &lower_pred) == 0);
    REQUIRE(checkStringPred(" ", &lower_pred) == 0);
  }

  SECTION("upper") {
    REQUIRE(checkStringPred("a", &upper_pred) == 0);
    REQUIRE(checkStringPred("A", &upper_pred) == 1);
    REQUIRE(checkStringPred("1", &upper_pred) == 0);
    REQUIRE(checkStringPred(".", &upper_pred) == 0);
    REQUIRE(checkStringPred(" ", &upper_pred) == 0);
  }
  
  SECTION("whitespace") {
    REQUIRE(checkStringPred("a", &whitespace_pred) == 0);
    REQUIRE(checkStringPred("A", &whitespace_pred) == 0);
    REQUIRE(checkStringPred(".", &whitespace_pred) == 0);
    REQUIRE(checkStringPred("\n", &whitespace_pred) == 1);
    REQUIRE(checkStringPred(" ", &whitespace_pred) == 1);
  }
}


