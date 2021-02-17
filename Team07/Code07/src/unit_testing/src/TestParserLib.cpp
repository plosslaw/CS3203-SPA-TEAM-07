
#include <string>
#include "../../spa/src/TNode.h"
#include "../../spa/src/ParserLib.h"
#include "../../../lib/catch.hpp"

TEST_CASE("predicates") {
  SECTION("alphanum") {
    REQUIRE(alphanumPred('a'));
    REQUIRE(alphanumPred('A'));
    REQUIRE(alphanumPred('1'));
    REQUIRE_FALSE(alphanumPred('.'));
  }

  SECTION("digit") {
    REQUIRE(digitPred('1'));
    REQUIRE_FALSE(digitPred('a'));
    REQUIRE_FALSE(digitPred('.'));
  }

  SECTION("alpha") {
    REQUIRE(alphaPred('a'));
    REQUIRE(alphaPred('A'));
    REQUIRE_FALSE(alphaPred('1'));
    REQUIRE_FALSE(alphaPred('.'));
  }

  SECTION("lower") {
    REQUIRE(lowerPred('a'));
    REQUIRE_FALSE(lowerPred('A'));
    REQUIRE_FALSE(lowerPred('.'));
  }

  SECTION("upper") {
    REQUIRE_FALSE(upperPred('a'));
    REQUIRE(upperPred('A'));
    REQUIRE_FALSE(upperPred('.'));
  }
  
  SECTION("whitespace") {
    REQUIRE(whitespacePred(' '));
    REQUIRE(whitespacePred('\n'));
    REQUIRE_FALSE(whitespacePred('.'));
  }
}

int checkStringPred(std::string str, bool (*pred)(char)) {
  std::string strv = str;
  State state(&str);
  stringPredicate(state, pred, "");
  return state.i;
}

TEST_CASE("parsers") {
  SECTION("alphanum") {
    REQUIRE(checkStringPred("a", &alphanumPred) == 1);
    REQUIRE(checkStringPred("A", &alphanumPred) == 1);
    REQUIRE(checkStringPred("1", &alphanumPred) == 1);
    REQUIRE(checkStringPred(".", &alphanumPred) == 0);
    REQUIRE(checkStringPred(" ", &alphanumPred) == 0);
  }

  SECTION("digit") {
    REQUIRE(checkStringPred("a", &digitPred) == 0);
    REQUIRE(checkStringPred("A", &digitPred) == 0);
    REQUIRE(checkStringPred("1", &digitPred) == 1);
    REQUIRE(checkStringPred(".", &digitPred) == 0);
    REQUIRE(checkStringPred(" ", &digitPred) == 0);
  }

  SECTION("alpha") {
    REQUIRE(checkStringPred("a", &alphaPred) == 1);
    REQUIRE(checkStringPred("A", &alphaPred) == 1);
    REQUIRE(checkStringPred("1", &alphaPred) == 0);
    REQUIRE(checkStringPred(".", &alphaPred) == 0);
    REQUIRE(checkStringPred(" ", &alphaPred) == 0);
  }

  SECTION("lower") {
    REQUIRE(checkStringPred("a", &lowerPred) == 1);
    REQUIRE(checkStringPred("A", &lowerPred) == 0);
    REQUIRE(checkStringPred("1", &lowerPred) == 0);
    REQUIRE(checkStringPred(".", &lowerPred) == 0);
    REQUIRE(checkStringPred(" ", &lowerPred) == 0);
  }

  SECTION("upper") {
    REQUIRE(checkStringPred("a", &upperPred) == 0);
    REQUIRE(checkStringPred("A", &upperPred) == 1);
    REQUIRE(checkStringPred("1", &upperPred) == 0);
    REQUIRE(checkStringPred(".", &upperPred) == 0);
    REQUIRE(checkStringPred(" ", &upperPred) == 0);
  }
  
  SECTION("whitespace") {
    REQUIRE(checkStringPred("a", &whitespacePred) == 0);
    REQUIRE(checkStringPred("A", &whitespacePred) == 0);
    REQUIRE(checkStringPred(".", &whitespacePred) == 0);
    REQUIRE(checkStringPred("\n", &whitespacePred) == 1);
    REQUIRE(checkStringPred(" ", &whitespacePred) == 1);
  }
}


