#include "QueryMap.h"
#include "catch.hpp"
#include <iostream>
#include <vector>

TEST_CASE("PayLoad with single variable") {
  PayLoad stmt(SINGLE, STATEMENT, std::vector<std::string>{"s"});
  PayLoad re(SINGLE, READ, std::vector<std::string>{"re"});
  PayLoad pn(SINGLE, PRINT, std::vector<std::string>{"pn"});
  PayLoad call(SINGLE, CALL, std::vector<std::string>{"c"});
  PayLoad w(SINGLE, WHILE, std::vector<std::string>{"w"});
  PayLoad ifs(SINGLE, IF, std::vector<std::string>{"ifs"});
  PayLoad a(SINGLE, ASSIGN, std::vector<std::string>{"a"});
  PayLoad c(SINGLE, CONSTANT, std::vector<std::string>{"c"});
  PayLoad var(SINGLE, VARIABLE, std::vector<std::string>{"v"});
  PayLoad proc(SINGLE, PROCEDURE, std::vector<std::string>{"p"});
  PayLoad syn(SINGLE, SYNONYM, std::vector<std::string>{"s"});

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
    REQUIRE(stmt.getValue() == std::vector<std::string>{"s"});
    REQUIRE(re.getValue() == std::vector<std::string>{"re"});
    REQUIRE(pn.getValue() == std::vector<std::string>{"pn"});
    REQUIRE(call.getValue() == std::vector<std::string>{"c"});
    REQUIRE(w.getValue() == std::vector<std::string>{"w"});
    REQUIRE(ifs.getValue() == std::vector<std::string>{"ifs"});
    REQUIRE(a.getValue() == std::vector<std::string>{"a"});
    REQUIRE(c.getValue() == std::vector<std::string>{"c"});
    REQUIRE(var.getValue() == std::vector<std::string>{"v"});
    REQUIRE(proc.getValue() == std::vector<std::string>{"p"});
    REQUIRE(syn.getValue() == std::vector<std::string>{"s"});
  }
}

TEST_CASE("PayLoad with pair variable") {
  PayLoad follows(PAIR, FOLLOWS, std::vector<std::string>{"s1", "s2"});
  PayLoad followst(PAIR, FOLLOWST, std::vector<std::string>{"s1", "s2"});
  PayLoad parent(PAIR, PARENT, std::vector<std::string>{"s1", "s2"});
  PayLoad parentt(PAIR, PARENTT, std::vector<std::string>{"s1", "s2"});
  PayLoad uses(PAIR, USES, std::vector<std::string>{"s1", "v"});
  PayLoad modifies(PAIR, MODIFIES, std::vector<std::string>{"s1", "v"});

  SECTION("PayLoad tag") {
    REQUIRE(follows.getTag() == PAIR);
    REQUIRE(followst.getTag() == PAIR);
    REQUIRE(parent.getTag() == PAIR);
    REQUIRE(parentt.getTag() == PAIR);
    REQUIRE(uses.getTag() == PAIR);
    REQUIRE(modifies.getTag() == PAIR);
  }

  SECTION("PayLoad type") {
    REQUIRE(follows.getType().pair == FOLLOWS);
    REQUIRE(followst.getType().pair == FOLLOWST);
    REQUIRE(parent.getType().pair == PARENT);
    REQUIRE(parentt.getType().pair == PARENTT);
    REQUIRE(uses.getType().pair == USES);
    REQUIRE(modifies.getType().pair == MODIFIES);
  }

  SECTION("PayLoad value") {
    REQUIRE(follows.getValue() == std::vector<std::string>{"s1", "s2"});
    REQUIRE(followst.getValue() == std::vector<std::string>{"s1", "s2"});
    REQUIRE(parent.getValue() == std::vector<std::string>{"s1", "s2"});
    REQUIRE(parentt.getValue() == std::vector<std::string>{"s1", "s2"});
    REQUIRE(uses.getValue() == std::vector<std::string>{"s1", "v"});
    REQUIRE(modifies.getValue() == std::vector<std::string>{"s1", "v"});
  }
}

TEST_CASE("PayLoad with triple variable") {
  PayLoad synonym_assign(TRIPLE, SYN_ASSIGN,
                         std::vector<std::string>{"a", "_", "_"});

  SECTION("PayLoad tag") { REQUIRE(synonym_assign.getTag() == TRIPLE); }

  SECTION("PayLoad type") {
    REQUIRE(synonym_assign.getType().triple == SYN_ASSIGN);
  }

  SECTION("PayLoad value") {
    REQUIRE(synonym_assign.getValue() ==
            std::vector<std::string>{"a", "_", "_"});
  }
}

TEST_CASE("PQL with no such that and no pattern") {
  /*
  variable v;
  Select v
  */
  QueryMap query;
  query.addItem(DECLARATION,
                PayLoad(SINGLE, VARIABLE, std::vector<std::string>{"v"}));
  query.addItem(SELECT,
                PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"v"}));

  REQUIRE(query.getList(DECLARATION).size() == 1);
  REQUIRE(query.getList(SELECT).size() == 1);
  REQUIRE(query.getList(SUCHTHAT).empty());
  REQUIRE(query.getList(PATTERN).empty());

  SECTION("Declaration clause") {
    REQUIRE(query.getList(DECLARATION)[0] ==
            PayLoad(SINGLE, VARIABLE, std::vector<std::string>{"v"}));

    REQUIRE_FALSE(query.getList(DECLARATION)[0] ==
                  PayLoad(PAIR, VARIABLE, std::vector<std::string>{"v"}));
    REQUIRE_FALSE(query.getList(DECLARATION)[0] ==
                  PayLoad(SINGLE, VARIABLE, std::vector<std::string>{"s"}));
    REQUIRE_FALSE(query.getList(DECLARATION)[0] ==
                  PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s"}));
  }

  SECTION("Select clause") {
    REQUIRE(query.getList(SELECT)[0] ==
            PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"v"}));

    REQUIRE_FALSE(query.getList(SELECT)[0] ==
                  PayLoad(PAIR, SYNONYM, std::vector<std::string>{"v"}));
    REQUIRE_FALSE(query.getList(SELECT)[0] ==
                  PayLoad(SINGLE, VARIABLE, std::vector<std::string>{"v"}));
    REQUIRE_FALSE(query.getList(SELECT)[0] ==
                  PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"s"}));
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
  query.addItem(DECLARATION,
                PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s"}));
  query.addItem(SELECT,
                PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"s"}));
  query.addItem(SUCHTHAT,
                PayLoad(PAIR, FOLLOWST, std::vector<std::string>{"6", "s"}));

  REQUIRE(query.getList(DECLARATION).size() == 1);
  REQUIRE(query.getList(SELECT).size() == 1);
  REQUIRE(query.getList(SUCHTHAT).size() == 1);
  REQUIRE(query.getList(PATTERN).empty());

  SECTION("Declaration clause") {
    REQUIRE(query.getList(DECLARATION)[0] ==
            PayLoad(SINGLE, STATEMENT, std::vector<std::string>{"s"}));
  }

  SECTION("Select clause") {
    REQUIRE(query.getList(SELECT)[0] ==
            PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"s"}));
  }

  SECTION("Such that clause") {
    REQUIRE(query.getList(SUCHTHAT)[0] ==
            PayLoad(PAIR, FOLLOWST, std::vector<std::string>{"6", "s"}));
  }

  SECTION("Pattern clause") {}
}

TEST_CASE("PQL with one pattern") {
  /*
  assign a;
  Select a pattern a (_,“count + 1”)
  */
  QueryMap query;
  query.addItem(DECLARATION,
                PayLoad(SINGLE, ASSIGN, std::vector<std::string>{"a"}));
  query.addItem(SELECT,
                PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"a"}));
  query.addItem(PATTERN,
                PayLoad(TRIPLE, SYN_ASSIGN,
                        std::vector<std::string>{"a", "_", "count + 1"}));

  REQUIRE(query.getList(DECLARATION).size() == 1);
  REQUIRE(query.getList(SELECT).size() == 1);
  REQUIRE(query.getList(SUCHTHAT).empty());
  REQUIRE(query.getList(PATTERN).size() == 1);

  SECTION("Declaration clause") {
    REQUIRE(query.getList(DECLARATION)[0] ==
            PayLoad(SINGLE, ASSIGN, std::vector<std::string>{"a"}));
  }

  SECTION("Select clause") {
    REQUIRE(query.getList(SELECT)[0] ==
            PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"a"}));
  }

  SECTION("Such that clause") {}

  SECTION("Pattern clause") {
    REQUIRE(query.getList(PATTERN)[0] ==
            PayLoad(TRIPLE, SYN_ASSIGN,
                    std::vector<std::string>{"a", "_", "count + 1"}));
  }
}

TEST_CASE("PQL with one such that and one pattern") {
  /*
  assign a; while w;
  Select w such that Parent* (w, a) pattern a (“count”, _)
  */

  QueryMap query;
  query.addItem(DECLARATION,
                PayLoad(SINGLE, ASSIGN, std::vector<std::string>{"a"}));
  query.addItem(DECLARATION,
                PayLoad(SINGLE, WHILE, std::vector<std::string>{"w"}));
  query.addItem(SELECT,
                PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"w"}));
  query.addItem(SUCHTHAT,
                PayLoad(PAIR, PARENTT, std::vector<std::string>{"w", "a"}));
  query.addItem(PATTERN, PayLoad(TRIPLE, SYN_ASSIGN,
                                 std::vector<std::string>{"a", "count", "_"}));

  REQUIRE(query.getList(SELECT).size() == 1);
  REQUIRE(query.getList(DECLARATION).size() == 2);
  REQUIRE(query.getList(SUCHTHAT).size() == 1);
  REQUIRE(query.getList(PATTERN).size() == 1);

  SECTION("Declaration clause") {
    REQUIRE(query.getList(DECLARATION)[0] ==
            PayLoad(SINGLE, ASSIGN, std::vector<std::string>{"a"}));
    REQUIRE(query.getList(DECLARATION)[1] ==
            PayLoad(SINGLE, WHILE, std::vector<std::string>{"w"}));
  }

  SECTION("Select clause") {
    REQUIRE(query.getList(SELECT)[0] ==
            PayLoad(SINGLE, SYNONYM, std::vector<std::string>{"w"}));
  }

  SECTION("Such that clause") {
    REQUIRE(query.getList(SUCHTHAT)[0] ==
            PayLoad(PAIR, PARENTT, std::vector<std::string>{"w", "a"}));
  }

  SECTION("Pattern clause") {
    REQUIRE(query.getList(PATTERN)[0] ==
            PayLoad(TRIPLE, SYN_ASSIGN,
                    std::vector<std::string>{"a", "count", "_"}));
  }
}
