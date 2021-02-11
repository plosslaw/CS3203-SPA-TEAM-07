#include "QueryMap.h"
#include "catch.hpp"
#include <iostream>
#include <vector>

TEST_CASE("PayLoad with single variable") {
  PayLoad stmt(SINGLE, STATEMENT, "s");
  PayLoad re(SINGLE, READ, "re");
  PayLoad pn(SINGLE, PRINT, "pn");
  PayLoad call(SINGLE, CALL, "c");
  PayLoad w(SINGLE, WHILE, "w");
  PayLoad ifs(SINGLE, IF, "ifs");
  PayLoad a(SINGLE, ASSIGN, "a");
  PayLoad c(SINGLE, CONSTANT, "c");
  PayLoad var(SINGLE, VARIABLE, "v");
  PayLoad proc(SINGLE, PROCEDURE, "p");
  PayLoad syn(SINGLE, SYNONYM, "s");

  SECTION("PayLoad tag") {
    REQUIRE(stmt.getTag() == SINGLE);
    REQUIRE(re.getTag() == SINGLE);
    REQUIRE(pn.getTag() == SINGLE);
    REQUIRE(call.getTag() == SINGLE);
    REQUIRE(w.getTag() == SINGLE);
    REQUIRE(ifs.getTag() == SINGLE);
    REQUIRE(a.getTag() == SINGLE);
    REQUIRE(c.getTag() == SINGLE);
    REQUIRE(var.getTag() == SINGLE);
    REQUIRE(proc.getTag() == SINGLE);
    REQUIRE(syn.getTag() == SINGLE);
  }

  SECTION("PayLoad type") {
    REQUIRE(stmt.getType().single == STATEMENT);
    REQUIRE(re.getType().single == READ);
    REQUIRE(pn.getType().single == PRINT);
    REQUIRE(call.getType().single == CALL);
    REQUIRE(w.getType().single == WHILE);
    REQUIRE(ifs.getType().single == IF);
    REQUIRE(a.getType().single == ASSIGN);
    REQUIRE(c.getType().single == CONSTANT);
    REQUIRE(var.getType().single == VARIABLE);
    REQUIRE(proc.getType().single == PROCEDURE);
    REQUIRE(syn.getType().single == SYNONYM);
  }

  SECTION("PayLoad value") {
    REQUIRE(stmt.getValue() == "s");
    REQUIRE(re.getValue() == "re");
    REQUIRE(pn.getValue() == "pn");
    REQUIRE(call.getValue() == "c");
    REQUIRE(w.getValue() == "w");
    REQUIRE(ifs.getValue() == "ifs");
    REQUIRE(a.getValue() == "a");
    REQUIRE(c.getValue() == "c");
    REQUIRE(var.getValue() == "v");
    REQUIRE(proc.getValue() == "p");
    REQUIRE(syn.getValue() == "s");
  }
}

TEST_CASE("PayLoad with pair variable") {
  PayLoad follows(PAIR, FOLLOWS, "s1,s2");
  PayLoad followst(PAIR, FOLLOWST, "s1,s2");
  PayLoad parent(PAIR, PARENT, "s1,s2");
  PayLoad parentt(PAIR, PARENTT, "s1,s2");
  PayLoad uses(PAIR, USES, "s1,v");
  PayLoad modifies(PAIR, MODIFIES, "s1,v");
  PayLoad synonym_assign(PAIR, SYN_ASSIGN, "_,_");

  SECTION("PayLoad tag") {
    REQUIRE(follows.getTag() == PAIR);
    REQUIRE(followst.getTag() == PAIR);
    REQUIRE(parent.getTag() == PAIR);
    REQUIRE(parentt.getTag() == PAIR);
    REQUIRE(uses.getTag() == PAIR);
    REQUIRE(modifies.getTag() == PAIR);
    REQUIRE(synonym_assign.getTag() == PAIR);
  }

  SECTION("PayLoad type") {
    REQUIRE(follows.getType().pair == FOLLOWS);
    REQUIRE(followst.getType().pair == FOLLOWST);
    REQUIRE(parent.getType().pair == PARENT);
    REQUIRE(parentt.getType().pair == PARENTT);
    REQUIRE(uses.getType().pair == USES);
    REQUIRE(modifies.getType().pair == MODIFIES);
    REQUIRE(synonym_assign.getType().pair == SYN_ASSIGN);
  }

  SECTION("PayLoad value") {
    REQUIRE(follows.getValue() == "s1,s2");
    REQUIRE(followst.getValue() == "s1,s2");
    REQUIRE(parent.getValue() == "s1,s2");
    REQUIRE(parentt.getValue() == "s1,s2");
    REQUIRE(uses.getValue() == "s1,v");
    REQUIRE(modifies.getValue() == "s1,v");
    REQUIRE(synonym_assign.getValue() == "_,_");
  }
}
