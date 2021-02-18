#include "QueryMap.h"
#include "catch.hpp"
#include <iostream>
#include <vector>

TEST_CASE("PayLoad with single variable") {
  PayLoad stmt(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"});
  PayLoad re(SINGLE, Single::READ, std::vector<std::string>{"re"});
  PayLoad pn(SINGLE, Single::PRINT, std::vector<std::string>{"pn"});
  PayLoad call(SINGLE, Single::CALL, std::vector<std::string>{"c"});
  PayLoad w(SINGLE, Single::WHILE, std::vector<std::string>{"w"});
  PayLoad ifs(SINGLE, Single::IF, std::vector<std::string>{"ifs"});
  PayLoad a(SINGLE, Single::ASSIGN, std::vector<std::string>{"a"});
  PayLoad c(SINGLE, Single::CONSTANT, std::vector<std::string>{"c"});
  PayLoad var(SINGLE, Single::VARIABLE, std::vector<std::string>{"v"});
  PayLoad proc(SINGLE, Single::PROCEDURE, std::vector<std::string>{"p"});
  PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s"});

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
    REQUIRE(stmt.getType().single == Single::STATEMENT);
    REQUIRE(re.getType().single == Single::READ);
    REQUIRE(pn.getType().single == Single::PRINT);
    REQUIRE(call.getType().single == Single::CALL);
    REQUIRE(w.getType().single == Single::WHILE);
    REQUIRE(ifs.getType().single == Single::IF);
    REQUIRE(a.getType().single == Single::ASSIGN);
    REQUIRE(c.getType().single == Single::CONSTANT);
    REQUIRE(var.getType().single == Single::VARIABLE);
    REQUIRE(proc.getType().single == Single::PROCEDURE);
    REQUIRE(syn.getType().single == Single::SYNONYM);
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
  query.addItem(ClauseType::DECLARATION, PayLoad(SINGLE, Single::VARIABLE,
                                     std::vector<std::string>{"v"}));
  query.addItem(
      ClauseType::SELECT, PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"v"}));

  REQUIRE(query.getList(ClauseType::DECLARATION).size() == 1);
  REQUIRE(query.getList(ClauseType::SELECT).size() == 1);
  REQUIRE(query.getList(ClauseType::SUCHTHAT).empty());
  REQUIRE(query.getList(ClauseType::PATTERN).empty());

  SECTION("Declaration clause") {
    REQUIRE(query.getList(ClauseType::DECLARATION)[0] ==
            PayLoad(SINGLE, Single::VARIABLE, std::vector<std::string>{"v"}));

    REQUIRE_FALSE(
        query.getList(ClauseType::DECLARATION)[0] ==
        PayLoad(PAIR, Single::VARIABLE, std::vector<std::string>{"v"}));
    REQUIRE_FALSE(
        query.getList(ClauseType::DECLARATION)[0] ==
        PayLoad(SINGLE, Single::VARIABLE, std::vector<std::string>{"s"}));
    REQUIRE_FALSE(
        query.getList(ClauseType::DECLARATION)[0] ==
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"}));
  }

  SECTION("Select clause") {
    REQUIRE(query.getList(ClauseType::SELECT)[0] ==
            PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"v"}));

    REQUIRE_FALSE(
        query.getList(ClauseType::SELECT)[0] ==
        PayLoad(PAIR, Single::SYNONYM, std::vector<std::string>{"v"}));
    REQUIRE_FALSE(
        query.getList(ClauseType::SELECT)[0] ==
        PayLoad(SINGLE, Single::VARIABLE, std::vector<std::string>{"v"}));
    REQUIRE_FALSE(
        query.getList(ClauseType::SELECT)[0] ==
        PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"s"}));
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
  query.addItem(
      ClauseType::DECLARATION,
      PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"}));
  query.addItem(ClauseType::SELECT, PayLoad(SINGLE, Single::SYNONYM,
                                            std::vector<std::string>{"s"}));
  query.addItem(ClauseType::SUCHTHAT,
                PayLoad(PAIR, FOLLOWST, std::vector<std::string>{"6", "s"}));

  REQUIRE(query.getList(ClauseType::DECLARATION).size() == 1);
  REQUIRE(query.getList(ClauseType::SELECT).size() == 1);
  REQUIRE(query.getList(ClauseType::SUCHTHAT).size() == 1);
  REQUIRE(query.getList(ClauseType::PATTERN).empty());

  SECTION("Declaration clause") {
    REQUIRE(query.getList(ClauseType::DECLARATION)[0] ==
            PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"}));
  }

  SECTION("Select clause") {
    REQUIRE(query.getList(ClauseType::SELECT)[0] ==
            PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"s"}));
  }

  SECTION("Such that clause") {
    REQUIRE(query.getList(ClauseType::SUCHTHAT)[0] ==
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
  query.addItem(ClauseType::DECLARATION,
                PayLoad(SINGLE, Single::ASSIGN, std::vector<std::string>{"a"}));
  query.addItem(ClauseType::SELECT, PayLoad(SINGLE, Single::SYNONYM,
                                            std::vector<std::string>{"a"}));
  query.addItem(ClauseType::PATTERN,
                PayLoad(TRIPLE, SYN_ASSIGN,
                        std::vector<std::string>{"a", "_", "count + 1"}));

  REQUIRE(query.getList(ClauseType::DECLARATION).size() == 1);
  REQUIRE(query.getList(ClauseType::SELECT).size() == 1);
  REQUIRE(query.getList(ClauseType::SUCHTHAT).empty());
  REQUIRE(query.getList(ClauseType::PATTERN).size() == 1);

  SECTION("Declaration clause") {
    REQUIRE(query.getList(ClauseType::DECLARATION)[0] ==
            PayLoad(SINGLE, Single::ASSIGN, std::vector<std::string>{"a"}));
  }

  SECTION("Select clause") {
    REQUIRE(query.getList(ClauseType::SELECT)[0] ==
            PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"a"}));
  }

  SECTION("Such that clause") {}

  SECTION("Pattern clause") {
    REQUIRE(query.getList(ClauseType::PATTERN)[0] ==
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
  query.addItem(ClauseType::DECLARATION,
                PayLoad(SINGLE, Single::ASSIGN, std::vector<std::string>{"a"}));
  query.addItem(ClauseType::DECLARATION,
                PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"w"}));
  query.addItem(ClauseType::SELECT, PayLoad(SINGLE, Single::SYNONYM,
                                            std::vector<std::string>{"w"}));
  query.addItem(ClauseType::SUCHTHAT,
                PayLoad(PAIR, PARENTT, std::vector<std::string>{"w", "a"}));
  query.addItem(
      ClauseType::PATTERN,
      PayLoad(TRIPLE, SYN_ASSIGN, std::vector<std::string>{"a", "count", "_"}));

  REQUIRE(query.getList(ClauseType::SELECT).size() == 1);
  REQUIRE(query.getList(ClauseType::DECLARATION).size() == 2);
  REQUIRE(query.getList(ClauseType::SUCHTHAT).size() == 1);
  REQUIRE(query.getList(ClauseType::PATTERN).size() == 1);

  SECTION("Declaration clause") {
    REQUIRE(query.getList(ClauseType::DECLARATION)[0] ==
            PayLoad(SINGLE, Single::ASSIGN, std::vector<std::string>{"a"}));
    REQUIRE(query.getList(ClauseType::DECLARATION)[1] ==
            PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"w"}));
  }

  SECTION("Select clause") {
    REQUIRE(query.getList(ClauseType::SELECT)[0] ==
            PayLoad(SINGLE, Single::SYNONYM, std::vector<std::string>{"w"}));
  }

  SECTION("Such that clause") {
    REQUIRE(query.getList(ClauseType::SUCHTHAT)[0] ==
            PayLoad(PAIR, PARENTT, std::vector<std::string>{"w", "a"}));
  }

  SECTION("Pattern clause") {
    REQUIRE(query.getList(ClauseType::PATTERN)[0] ==
            PayLoad(TRIPLE, SYN_ASSIGN,
                    std::vector<std::string>{"a", "count", "_"}));
  }
}
