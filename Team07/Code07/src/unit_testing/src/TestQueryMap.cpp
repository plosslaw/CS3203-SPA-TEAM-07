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

TEST_CASE("PQL with no such that and no pattern") {
  /*
  variable v;
  Select v
  */
  QueryMap query;
  query.addItem(DECLARATION, PayLoad(SINGLE, VARIABLE, "v"));
  query.addItem(SELECT, PayLoad(SINGLE, SYNONYM, "v"));

  REQUIRE(query.getList(DECLARATION).size() == 1);
  REQUIRE(query.getList(SELECT).size() == 1);
  REQUIRE(query.getList(SUCHTHAT).empty());
  REQUIRE(query.getList(PATTERN).empty());

  SECTION("Declaration clause") {
    REQUIRE(query.getList(DECLARATION)[0] == PayLoad(SINGLE, VARIABLE, "v"));

    REQUIRE_FALSE(query.getList(DECLARATION)[0] ==
                  PayLoad(PAIR, VARIABLE, "v"));
    REQUIRE_FALSE(query.getList(DECLARATION)[0] ==
                  PayLoad(SINGLE, VARIABLE, "s"));
    REQUIRE_FALSE(query.getList(DECLARATION)[0] ==
                  PayLoad(SINGLE, STATEMENT, "s"));
  }

  SECTION("Select clause") {
    REQUIRE(query.getList(SELECT)[0] == PayLoad(SINGLE, SYNONYM, "v"));

    REQUIRE_FALSE(query.getList(SELECT)[0] == PayLoad(PAIR, SYNONYM, "v"));
    REQUIRE_FALSE(query.getList(SELECT)[0] == PayLoad(SINGLE, VARIABLE, "v"));
    REQUIRE_FALSE(query.getList(SELECT)[0] == PayLoad(SINGLE, SYNONYM, "s"));
  }

  SECTION("Such that clause") {}

  SECTION("Pattern clause") {}
}

TEST_CASE("PQL with one such that") {
  /*
  stmt s;
  Select s such that Follows* (6, s)
  */
  QueryMap query;
  query.addItem(DECLARATION, PayLoad(SINGLE, STATEMENT, "s"));
  query.addItem(SELECT, PayLoad(SINGLE, SYNONYM, "s"));
  query.addItem(SUCHTHAT, PayLoad(PAIR, FOLLOWST, "6,s"));

  REQUIRE(query.getList(DECLARATION).size() == 1);
  REQUIRE(query.getList(SELECT).size() == 1);
  REQUIRE(query.getList(SUCHTHAT).size() == 1);
  REQUIRE(query.getList(PATTERN).empty());

  SECTION("Declaration clause") {
    REQUIRE(query.getList(DECLARATION)[0] == PayLoad(SINGLE, STATEMENT, "s"));
  }

  SECTION("Select clause") {
    REQUIRE(query.getList(SELECT)[0] == PayLoad(SINGLE, SYNONYM, "s"));
  }

  SECTION("Such that clause") {
    REQUIRE(query.getList(SUCHTHAT)[0] == PayLoad(PAIR, FOLLOWST, "6,s"));
  }

  SECTION("Pattern clause") {}
}

TEST_CASE("PQL with one pattern") {
  /*
  assign a;
  Select a pattern a (_,“count + 1”)
  */
  QueryMap query;
  query.addItem(DECLARATION, PayLoad(SINGLE, ASSIGN, "a"));
  query.addItem(SELECT, PayLoad(SINGLE, SYNONYM, "a"));
  query.addItem(PATTERN, PayLoad(PAIR, SYN_ASSIGN, "_,count + 1"));

  REQUIRE(query.getList(DECLARATION).size() == 1);
  REQUIRE(query.getList(SELECT).size() == 1);
  REQUIRE(query.getList(SUCHTHAT).empty());
  REQUIRE(query.getList(PATTERN).size() == 1);

  SECTION("Declaration clause") {
    REQUIRE(query.getList(DECLARATION)[0] == PayLoad(SINGLE, ASSIGN, "a"));
  }

  SECTION("Select clause") {
    REQUIRE(query.getList(SELECT)[0] == PayLoad(SINGLE, SYNONYM, "a"));
  }

  SECTION("Such that clause") {}

  SECTION("Pattern clause") {
    REQUIRE(query.getList(PATTERN)[0] ==
            PayLoad(PAIR, SYN_ASSIGN, "_,count + 1"));
  }
}

TEST_CASE("PQL with one such that and one pattern") {
  /*
  assign a; while w;
  Select w such that Parent* (w, a) pattern a (“count”, _)
  */

  QueryMap query;
  query.addItem(DECLARATION, PayLoad(SINGLE, ASSIGN, "a"));
  query.addItem(DECLARATION, PayLoad(SINGLE, WHILE, "w"));
  query.addItem(SELECT, PayLoad(SINGLE, SYNONYM, "w"));
  query.addItem(SUCHTHAT, PayLoad(PAIR, PARENTT, "w,a"));
  query.addItem(PATTERN, PayLoad(PAIR, SYN_ASSIGN, "count,_"));

  REQUIRE(query.getList(SELECT).size() == 1);
  REQUIRE(query.getList(DECLARATION).size() == 2);
  REQUIRE(query.getList(SUCHTHAT).size() == 1);
  REQUIRE(query.getList(PATTERN).size() == 1);

  SECTION("Declaration clause") {
    REQUIRE(query.getList(DECLARATION)[0] == PayLoad(SINGLE, ASSIGN, "a"));
    REQUIRE(query.getList(DECLARATION)[1] == PayLoad(SINGLE, WHILE, "w"));
  }

  SECTION("Select clause") {
    REQUIRE(query.getList(SELECT)[0] == PayLoad(SINGLE, SYNONYM, "w"));
  }

  SECTION("Such that clause") {
    REQUIRE(query.getList(SUCHTHAT)[0] == PayLoad(PAIR, PARENTT, "w,a"));
  }

  SECTION("Pattern clause") {
    REQUIRE(query.getList(PATTERN)[0] == PayLoad(PAIR, SYN_ASSIGN, "count,_"));
  }
}
