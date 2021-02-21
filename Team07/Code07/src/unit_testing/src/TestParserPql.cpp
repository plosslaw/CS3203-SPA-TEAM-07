#include "../../../lib/catch.hpp"
#include "../../spa/src/ParserLib.h"
#include "../../spa/src/ParserPql.h"
#include <iostream>
#include <vector>

TEST_CASE("Parse single declaration clause, single synonym") {
  SECTION("Passes single declaration single entity, stmt") {
    std::string query = "stmt s;";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"})};

    State state(&query);
    std::vector<PayLoad> actual_clause = declaration_cl(state);
    REQUIRE(expected_clause == actual_clause);
  }

  SECTION("Passes single declaration single entity, read") {
    std::string query = "read re;";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::READ, std::vector<std::string>{"re"})};

    State state(&query);
    std::vector<PayLoad> actual_clause = declaration_cl(state);
    REQUIRE(expected_clause == actual_clause);
  }

  SECTION("Passes single declaration single entity, print") {
    std::string query = "print pn;";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::PRINT, std::vector<std::string>{"pn"})};

    State state(&query);
    std::vector<PayLoad> actual_clause = declaration_cl(state);
    REQUIRE(expected_clause == actual_clause);
  }

  SECTION("Passes single declaration single entity, call") {
    std::string query = "call c;";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::CALL, std::vector<std::string>{"c"})};

    State state(&query);
    std::vector<PayLoad> actual_clause = declaration_cl(state);
    REQUIRE(expected_clause == actual_clause);
  }

  SECTION("Passes single declaration single entity, while") {
    std::string query = "while w;";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"w"})};

    State state(&query);
    std::vector<PayLoad> actual_clause = declaration_cl(state);
    REQUIRE(expected_clause == actual_clause);
  }

  SECTION("Passes single declaration single entity, if") {
    std::string query = "if ifs;";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::IF, std::vector<std::string>{"ifs"})};

    State state(&query);
    std::vector<PayLoad> actual_clause = declaration_cl(state);
    REQUIRE(expected_clause == actual_clause);
  }

  SECTION("Passes single declaration single entity, assign") {
    std::string query = "assign a;";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::ASSIGN, std::vector<std::string>{"a"})};

    State state(&query);
    std::vector<PayLoad> actual_clause = declaration_cl(state);
    REQUIRE(expected_clause == actual_clause);
  }

  SECTION("Passes single declaration single entity, constant") {
    std::string query = "constant c;";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::CONSTANT, std::vector<std::string>{"c"})};

    State state(&query);
    std::vector<PayLoad> actual_clause = declaration_cl(state);
    REQUIRE(expected_clause == actual_clause);
  }

  SECTION("Passes single declaration single entity, variable") {
    std::string query = "variable v;";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::VARIABLE, std::vector<std::string>{"v"})};

    State state(&query);
    std::vector<PayLoad> actual_clause = declaration_cl(state);
    REQUIRE(expected_clause == actual_clause);
  }

  SECTION("Passes single declaration single entity, procedure") {
    std::string query = "procedure p;";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::PROCEDURE, std::vector<std::string>{"p"})};

    State state(&query);
    std::vector<PayLoad> actual_clause = declaration_cl(state);
    REQUIRE(expected_clause == actual_clause);
  }

  SECTION("Fails single declaration single entity no synonym") {
    std::string query = "stmt;";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"})};

    State state(&query);
    State so(&query);
    REQUIRE_THROWS(declaration_cl(state),
                   ParseException(so.i, state.i, "declaration_cl", ""));
  }

  SECTION("Fails single declaration single entity no semi colon") {
    std::string query = "stmt s";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"})};

    State state(&query);
    State so(&query);
    REQUIRE_THROWS(declaration_cl(state),
                   ParseException(so.i, state.i, "declaration_cl", ""));
  }
}

TEST_CASE("Parse single declaration clause, mutiple synonym") {
  SECTION("Passes single declaration single entity, stmt") {
    std::string query = "stmt s1, s2, s3;";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}),
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s2"}),
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s3"})};

    State state(&query);
    std::vector<PayLoad> actual_clause = declaration_cl(state);
    REQUIRE(expected_clause == actual_clause);
  }

  SECTION("Passes single declaration single entity, read") {
    std::string query = "read re1, re2, re3;";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::READ, std::vector<std::string>{"re1"}),
        PayLoad(SINGLE, Single::READ, std::vector<std::string>{"re2"}),
        PayLoad(SINGLE, Single::READ, std::vector<std::string>{"re3"})};

    State state(&query);
    std::vector<PayLoad> actual_clause = declaration_cl(state);
    REQUIRE(expected_clause == actual_clause);
  }

  SECTION("Passes single declaration single entity, print") {
    std::string query = "print pn1, pn2, pn3;";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::PRINT, std::vector<std::string>{"pn1"}),
        PayLoad(SINGLE, Single::PRINT, std::vector<std::string>{"pn2"}),
        PayLoad(SINGLE, Single::PRINT, std::vector<std::string>{"pn3"})};

    State state(&query);
    std::vector<PayLoad> actual_clause = declaration_cl(state);
    REQUIRE(expected_clause == actual_clause);
  }

  SECTION("Passes single declaration single entity, call") {
    std::string query = "call c1, c2, c3;";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::CALL, std::vector<std::string>{"c1"}),
        PayLoad(SINGLE, Single::CALL, std::vector<std::string>{"c2"}),
        PayLoad(SINGLE, Single::CALL, std::vector<std::string>{"c3"})};

    State state(&query);
    std::vector<PayLoad> actual_clause = declaration_cl(state);
    REQUIRE(expected_clause == actual_clause);
  }

  SECTION("Passes single declaration single entity, while") {
    std::string query = "while w1, w2, w3;";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"w1"}),
        PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"w2"}),
        PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"w3"})};

    State state(&query);
    std::vector<PayLoad> actual_clause = declaration_cl(state);
    REQUIRE(expected_clause == actual_clause);
  }

  SECTION("Passes single declaration single entity, if") {
    std::string query = "if ifs1, ifs2, ifs3;";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::IF, std::vector<std::string>{"ifs1"}),
        PayLoad(SINGLE, Single::IF, std::vector<std::string>{"ifs2"}),
        PayLoad(SINGLE, Single::IF, std::vector<std::string>{"ifs3"})};

    State state(&query);
    std::vector<PayLoad> actual_clause = declaration_cl(state);
    REQUIRE(expected_clause == actual_clause);
  }

  SECTION("Passes single declaration single entity, assign") {
    std::string query = "assign a1, a2, a3;";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::ASSIGN, std::vector<std::string>{"a1"}),
        PayLoad(SINGLE, Single::ASSIGN, std::vector<std::string>{"a2"}),
        PayLoad(SINGLE, Single::ASSIGN, std::vector<std::string>{"a3"})};

    State state(&query);
    std::vector<PayLoad> actual_clause = declaration_cl(state);
    REQUIRE(expected_clause == actual_clause);
  }

  SECTION("Passes single declaration single entity, constant") {
    std::string query = "constant c1, c2, c3;";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::CONSTANT, std::vector<std::string>{"c1"}),
        PayLoad(SINGLE, Single::CONSTANT, std::vector<std::string>{"c2"}),
        PayLoad(SINGLE, Single::CONSTANT, std::vector<std::string>{"c3"})};

    State state(&query);
    std::vector<PayLoad> actual_clause = declaration_cl(state);
    REQUIRE(expected_clause == actual_clause);
  }

  SECTION("Passes single declaration single entity, variable") {
    std::string query = "variable v1, v2, v3;";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::VARIABLE, std::vector<std::string>{"v1"}),
        PayLoad(SINGLE, Single::VARIABLE, std::vector<std::string>{"v2"}),
        PayLoad(SINGLE, Single::VARIABLE, std::vector<std::string>{"v3"})};

    State state(&query);
    std::vector<PayLoad> actual_clause = declaration_cl(state);
    REQUIRE(expected_clause == actual_clause);
  }

  SECTION("Passes single declaration single entity, procedure") {
    std::string query = "procedure p1, p2, p3;";
    std::vector<PayLoad> expected_clause{
        PayLoad(SINGLE, Single::PROCEDURE, std::vector<std::string>{"p1"}),
        PayLoad(SINGLE, Single::PROCEDURE, std::vector<std::string>{"p2"}),
        PayLoad(SINGLE, Single::PROCEDURE, std::vector<std::string>{"p3"})};

    State state(&query);
    std::vector<PayLoad> actual_clause = declaration_cl(state);
    REQUIRE(expected_clause == actual_clause);
  }

  SECTION("Fails single declaration single entity no synonym") {
    std::string query = "stmt , , ,;";

    State state(&query);
    State so(&query);
    REQUIRE_THROWS(declaration_cl(state),
                   ParseException(so.i, state.i, "declaration_cl", ""));
  }

  SECTION("Fails single declaration single entity no comma") {
    std::string query = "stmt s1 s2 s3;";

    State state(&query);
    State so(&query);
    REQUIRE_THROWS(declaration_cl(state),
                   ParseException(so.i, state.i, "declaration_cl", ""));
  }

  SECTION("Fails single declaration single entity no semi colon") {
    std::string query = "stmt s1, s2, s3";

    State state(&query);
    State so(&query);
    REQUIRE_THROWS(declaration_cl(state),
                   ParseException(so.i, state.i, "declaration_cl", ""));
  }
}

TEST_CASE("Parse select clause") {
  SECTION("Passes single synonym") {
    std::string query = "Select s";
    std::vector<PayLoad> expected_clause{PayLoad(SINGLE, Single::SYNONYM,
                                                 std::vector<std::string>{"s"},
                                                 std::vector<bool>{true})};

    State state(&query);
    std::vector<PayLoad> actual_clause = select_cl(state);
    REQUIRE(expected_clause == actual_clause);
  }

  SECTION("Fails no synonym") {
    std::string query = "Select";

    State state(&query);
    State so(&query);
    REQUIRE_THROWS(select_cl(state),
                   ParseException(so.i, state.i, "select_cl", ""));
  }

  SECTION("Fails invalid synonym") {
    std::string query = "Select ;";

    State state(&query);
    State so(&query);
    REQUIRE_THROWS(select_cl(state),
                   ParseException(so.i, state.i, "select_cl", ""));
  }
}

TEST_CASE("Parse such that Parent clause") {
  SECTION("synonym and synonym") {
    std::string query = "such that Parent(s1, s2)";
    PayLoad expected_clause(PAIR, PARENT, std::vector<std::string>{"s1", "s2"},
                            std::vector<bool>{true, true});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("synonym and wildcard") {
    std::string query = "such that Parent(s1, _)";
    PayLoad expected_clause(PAIR, PARENT, std::vector<std::string>{"s1", "_"},
                            std::vector<bool>{true, false});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }
  SECTION("synonym and integer") {
    std::string query = "such that Parent(s1, 1)";
    PayLoad expected_clause(PAIR, PARENT, std::vector<std::string>{"s1", "1"},
                            std::vector<bool>{true, false});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }
  SECTION("wildcard and synonym") {
    std::string query = "such that Parent(_, s1)";
    PayLoad expected_clause(PAIR, PARENT, std::vector<std::string>{"_", "s1"},
                            std::vector<bool>{false, true});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }
  SECTION("wildcard and wildcard") {
    std::string query = "such that Parent(_, _)";
    PayLoad expected_clause(PAIR, PARENT, std::vector<std::string>{"_", "_"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }
  SECTION("wildcard and integer") {
    std::string query = "such that Parent(_, 1)";
    PayLoad expected_clause(PAIR, PARENT, std::vector<std::string>{"_", "1"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }
  SECTION("integer and synonym") {
    std::string query = "such that Parent(1, s1)";
    PayLoad expected_clause(PAIR, PARENT, std::vector<std::string>{"1", "s1"},
                            std::vector<bool>{false, true});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }
  SECTION("integer and wildcard") {
    std::string query = "such that Parent(1, _)";
    PayLoad expected_clause(PAIR, PARENT, std::vector<std::string>{"1", "_"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }
  SECTION("integer and integer") {
    std::string query = "such that Parent(1, 2)";
    PayLoad expected_clause(PAIR, PARENT, std::vector<std::string>{"1", "2"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }
}

TEST_CASE("Parse such that Parent* clause") {
  SECTION("synonym and synonym") {
    std::string query = "such that Parent*(s1, s2)";
    PayLoad expected_clause(PAIR, PARENTT, std::vector<std::string>{"s1", "s2"},
                            std::vector<bool>{true, true});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("synonym and wildcard") {
    std::string query = "such that Parent*(s1, _)";
    PayLoad expected_clause(PAIR, PARENTT, std::vector<std::string>{"s1", "_"},
                            std::vector<bool>{true, false});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("synonym and integer") {
    std::string query = "such that Parent*(s1, 1)";
    PayLoad expected_clause(PAIR, PARENTT, std::vector<std::string>{"s1", "1"},
                            std::vector<bool>{true, false});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("wildcard and synonym") {
    std::string query = "such that Parent*(_, s1)";
    PayLoad expected_clause(PAIR, PARENTT, std::vector<std::string>{"_", "s1"},
                            std::vector<bool>{false, true});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("wildcard and wildcard") {
    std::string query = "such that Parent*(_, _)";
    PayLoad expected_clause(PAIR, PARENTT, std::vector<std::string>{"_", "_"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("wildcard and integer") {
    std::string query = "such that Parent*(_, 1)";
    PayLoad expected_clause(PAIR, PARENTT, std::vector<std::string>{"_", "1"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("integer and synonym") {
    std::string query = "such that Parent*(1, s1)";
    PayLoad expected_clause(PAIR, PARENTT, std::vector<std::string>{"1", "s1"},
                            std::vector<bool>{false, true});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("integer and wildcard") {
    std::string query = "such that Parent*(1, _)";
    PayLoad expected_clause(PAIR, PARENTT, std::vector<std::string>{"1", "_"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("integer and integer") {
    std::string query = "such that Parent*(1, 2)";
    PayLoad expected_clause(PAIR, PARENTT, std::vector<std::string>{"1", "2"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }
}

TEST_CASE("Parse such that Follows clause") {
  SECTION("synonym and synonym") {
    std::string query = "such that Follows(s1, s2)";
    PayLoad expected_clause(PAIR, FOLLOWS, std::vector<std::string>{"s1", "s2"},
                            std::vector<bool>{true, true});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("synonym and wildcard") {
    std::string query = "such that Follows(s1, _)";
    PayLoad expected_clause(PAIR, FOLLOWS, std::vector<std::string>{"s1", "_"},
                            std::vector<bool>{true, false});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("synonym and integer") {
    std::string query = "such that Follows(s1, 1)";
    PayLoad expected_clause(PAIR, FOLLOWS, std::vector<std::string>{"s1", "1"},
                            std::vector<bool>{true, false});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("wildcard and synonym") {
    std::string query = "such that Follows(_, s1)";
    PayLoad expected_clause(PAIR, FOLLOWS, std::vector<std::string>{"_", "s1"},
                            std::vector<bool>{false, true});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("wildcard and wildcard") {
    std::string query = "such that Follows(_, _)";
    PayLoad expected_clause(PAIR, FOLLOWS, std::vector<std::string>{"_", "_"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("wildcard and integer") {
    std::string query = "such that Follows(_, 1)";
    PayLoad expected_clause(PAIR, FOLLOWS, std::vector<std::string>{"_", "1"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("integer and synonym") {
    std::string query = "such that Follows(1, s1)";
    PayLoad expected_clause(PAIR, FOLLOWS, std::vector<std::string>{"1", "s1"},
                            std::vector<bool>{false, true});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("integer and wildcard") {
    std::string query = "such that Follows(1, _)";
    PayLoad expected_clause(PAIR, FOLLOWS, std::vector<std::string>{"1", "_"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("integer and integer") {
    std::string query = "such that Follows(1, 2)";
    PayLoad expected_clause(PAIR, FOLLOWS, std::vector<std::string>{"1", "2"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }
}

TEST_CASE("Parse such that Follows* clause") {
  SECTION("synonym and synonym") {
    std::string query = "such that Follows*(s1, s2)";
    PayLoad expected_clause(PAIR, FOLLOWST,
                            std::vector<std::string>{"s1", "s2"},
                            std::vector<bool>{true, true});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("synonym and wildcard") {
    std::string query = "such that Follows*(s1, _)";
    PayLoad expected_clause(PAIR, FOLLOWST, std::vector<std::string>{"s1", "_"},
                            std::vector<bool>{true, false});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("synonym and integer") {
    std::string query = "such that Follows*(s1, 1)";
    PayLoad expected_clause(PAIR, FOLLOWST, std::vector<std::string>{"s1", "1"},
                            std::vector<bool>{true, false});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("wildcard and synonym") {
    std::string query = "such that Follows*(_, s1)";
    PayLoad expected_clause(PAIR, FOLLOWST, std::vector<std::string>{"_", "s1"},
                            std::vector<bool>{false, true});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("wildcard and wildcard") {
    std::string query = "such that Follows*(_, _)";
    PayLoad expected_clause(PAIR, FOLLOWST, std::vector<std::string>{"_", "_"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("wildcard and integer") {
    std::string query = "such that Follows*(_, 1)";
    PayLoad expected_clause(PAIR, FOLLOWST, std::vector<std::string>{"_", "1"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("integer and synonym") {
    std::string query = "such that Follows*(1, s1)";
    PayLoad expected_clause(PAIR, FOLLOWST, std::vector<std::string>{"1", "s1"},
                            std::vector<bool>{false, true});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("integer and wildcard") {
    std::string query = "such that Follows*(1, _)";
    PayLoad expected_clause(PAIR, FOLLOWST, std::vector<std::string>{"1", "_"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("integer and integer") {
    std::string query = "such that Follows*(1, 2)";
    PayLoad expected_clause(PAIR, FOLLOWST, std::vector<std::string>{"1", "2"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }
}

TEST_CASE("Parse such that Modifies clause") {
  SECTION("synonym and synonym") {
    std::string query = "such that Modifies(s1, s2)";
    PayLoad expected_clause(PAIR, MODIFIES,
                            std::vector<std::string>{"s1", "s2"},
                            std::vector<bool>{true, true});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("synonym and wildcard") {
    std::string query = "such that Modifies(s1, _)";
    PayLoad expected_clause(PAIR, MODIFIES, std::vector<std::string>{"s1", "_"},
                            std::vector<bool>{true, false});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("synonym and \"ident\"") {
    std::string query = "such that Modifies(s1, \"x\")";
    PayLoad expected_clause(PAIR, MODIFIES, std::vector<std::string>{"s1", "x"},
                            std::vector<bool>{true, false});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("wildcard and synonym") {
    std::string query = "such that Modifies(_, s1)";
    PayLoad expected_clause(PAIR, MODIFIES, std::vector<std::string>{"_", "s1"},
                            std::vector<bool>{false, true});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("wildcard and wildcard") {
    std::string query = "such that Modifies(_, _)";
    PayLoad expected_clause(PAIR, MODIFIES, std::vector<std::string>{"_", "_"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("wildcard and \"ident\"") {
    std::string query = "such that Modifies(_, \"x1\")";
    PayLoad expected_clause(PAIR, MODIFIES,
                            std::vector<std::string>{"_", "x1"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("integer and synonym") {
    std::string query = "such that Modifies(1, s1)";
    PayLoad expected_clause(PAIR, MODIFIES, std::vector<std::string>{"1", "s1"},
                            std::vector<bool>{false, true});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("integer and wildcard") {
    std::string query = "such that Modifies(1, _)";
    PayLoad expected_clause(PAIR, MODIFIES, std::vector<std::string>{"1", "_"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("integer and \"ident\"") {
    std::string query = "such that Modifies(1, \"x\")";
    PayLoad expected_clause(PAIR, MODIFIES, std::vector<std::string>{"1", "x"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }
}

TEST_CASE("Parse such that Uses clause") {
  SECTION("synonym and synonym") {
    std::string query = "such that Uses(s1, s2)";
    PayLoad expected_clause(PAIR, USES, std::vector<std::string>{"s1", "s2"},
                            std::vector<bool>{true, true});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("synonym and wildcard") {
    std::string query = "such that Uses(s1, _)";
    PayLoad expected_clause(PAIR, USES, std::vector<std::string>{"s1", "_"},
                            std::vector<bool>{true, false});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("synonym and \"ident\"") {
    std::string query = "such that Uses(s1, \"x\")";
    PayLoad expected_clause(PAIR, USES, std::vector<std::string>{"s1", "x"},
                            std::vector<bool>{true, false});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("wildcard and synonym") {
    std::string query = "such that Uses(_, s1)";
    PayLoad expected_clause(PAIR, USES, std::vector<std::string>{"_", "s1"},
                            std::vector<bool>{false, true});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("wildcard and wildcard") {
    std::string query = "such that Uses(_, _)";
    PayLoad expected_clause(PAIR, USES, std::vector<std::string>{"_", "_"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("wildcard and \"ident\"") {
    std::string query = "such that Uses(_, \"x1\")";
    PayLoad expected_clause(PAIR, USES, std::vector<std::string>{"_", "x1"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("integer and synonym") {
    std::string query = "such that Uses(1, s1)";
    PayLoad expected_clause(PAIR, USES, std::vector<std::string>{"1", "s1"},
                            std::vector<bool>{false, true});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("integer and wildcard") {
    std::string query = "such that Uses(1, _)";
    PayLoad expected_clause(PAIR, USES, std::vector<std::string>{"1", "_"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("integer and \"ident\"") {
    std::string query = "such that Uses(1, \"x\")";
    PayLoad expected_clause(PAIR, USES, std::vector<std::string>{"1", "x"});

    State state(&query);
    PayLoad actual_clause = suchthat_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }
}

TEST_CASE("Parse pattern syn-assign clause") {
  SECTION("synonym and _factor_") {
    std::string query = "pattern a(s, _\"var\"_)";
    PayLoad expected_clause(TRIPLE, SYN_ASSIGN,
                            std::vector<std::string>{"a", "s", "_var_"},
                            std::vector<bool>{true, true, false});

    State state(&query);
    PayLoad actual_clause = pattern_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("synonym and _") {
    std::string query = "pattern a(s, _)";
    PayLoad expected_clause(TRIPLE, SYN_ASSIGN,
                            std::vector<std::string>{"a", "s", "_"},
                            std::vector<bool>{true, true, false});

    State state(&query);
    PayLoad actual_clause = pattern_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("_ and _factor_") {
    std::string query = "pattern a(_, _\"123\"_)";
    PayLoad expected_clause(TRIPLE, SYN_ASSIGN,
                            std::vector<std::string>{"a", "_", "_123_"},
                            std::vector<bool>{true, false, false});

    State state(&query);
    PayLoad actual_clause = pattern_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("_ and _") {
    std::string query = "pattern a(_, _)";
    PayLoad expected_clause(TRIPLE, SYN_ASSIGN,
                            std::vector<std::string>{"a", "_", "_"},
                            std::vector<bool>{true, false, false});

    State state(&query);
    PayLoad actual_clause = pattern_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("\"ident\" and _factor_") {
    std::string query = "pattern a(\"s\", _\"var\"_)";
    PayLoad expected_clause(TRIPLE, SYN_ASSIGN,
                            std::vector<std::string>{"a", "s", "_var_"},
                            std::vector<bool>{true, false, false});

    State state(&query);
    PayLoad actual_clause = pattern_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }

  SECTION("\"ident\" and _") {
    std::string query = "pattern a(\"s\", _)";
    PayLoad expected_clause(TRIPLE, SYN_ASSIGN,
                            std::vector<std::string>{"a", "s", "_"},
                            std::vector<bool>{true, false, false});

    State state(&query);
    PayLoad actual_clause = pattern_cl(state);
    REQUIRE(actual_clause == expected_clause);
  }
}

TEST_CASE("Parse PQL with declaration and select clause") {
  SECTION("no entity declaration and single synonym") {
    std::string query = "Select s";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(ClauseType::SELECT,
                             PayLoad(SINGLE, Single::SYNONYM,
                                     std::vector<std::string>{"s"},
                                     std::vector<bool>{true}));

    State state(&query);
    QueryMap actualQueryMap = pql_query(state);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("single entity declaration and synonym") {
    std::string query = "stmt s; Select s";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"}));
    expectedQueryMap.addItem(ClauseType::SELECT,
                             PayLoad(SINGLE, Single::SYNONYM,
                                     std::vector<std::string>{"s"},
                                     std::vector<bool>{true}));

    State state(&query);
    QueryMap actualQueryMap = pql_query(state);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }

  SECTION("multiple entity declaration and single synonym") {
    std::string query = "stmt s; while w; Select s";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"w"}));
    expectedQueryMap.addItem(ClauseType::SELECT,
                             PayLoad(SINGLE, Single::SYNONYM,
                                     std::vector<std::string>{"s"},
                                     std::vector<bool>{true}));

    State state(&query);
    QueryMap actualQueryMap = pql_query(state);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }
}

// TODO(zs)
TEST_CASE("Parse PQL with declaration and such that clause") {
  SECTION("") {}
}

// TODO(zs)
TEST_CASE("Parse PQL with declaration and pattern clause") {
  SECTION("") {}
}

TEST_CASE("Parse PQL with declaration, select and such that clause") {
  SECTION("multiple entity declaration, synonym and such that") {
    std::string query = "stmt s; while w; Select s such that Parent(w, s)";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"w"}));
    expectedQueryMap.addItem(ClauseType::SELECT,
                             PayLoad(SINGLE, Single::SYNONYM,
                                     std::vector<std::string>{"s"},
                                     std::vector<bool>{true}));
    expectedQueryMap.addItem(ClauseType::SUCHTHAT,
                             PayLoad(PAIR, PARENT,
                                     std::vector<std::string>{"w", "s"},
                                     std::vector<bool>{true, true}));

    State state(&query);
    QueryMap actualQueryMap = pql_query(state);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }
}

TEST_CASE("Parse PQL with declaration, select and pattern clause") {
  SECTION("single entity declaration, synonym and pattern") {
    std::string query = "assign a; stmt s; Select s pattern a(s, _)";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::ASSIGN, std::vector<std::string>{"a"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"}));
    expectedQueryMap.addItem(ClauseType::SELECT,
                             PayLoad(SINGLE, Single::SYNONYM,
                                     std::vector<std::string>{"s"},
                                     std::vector<bool>{true}));
    expectedQueryMap.addItem(ClauseType::PATTERN,
                             PayLoad(TRIPLE, SYN_ASSIGN,
                                     std::vector<std::string>{"a", "s", "_"},
                                     std::vector<bool>{true, true, false}));

    State state(&query);
    QueryMap actualQueryMap = pql_query(state);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }
}

TEST_CASE("Parse PQL with declaration, select, such that and pattern clause") {
  SECTION("multiple entity declaration, synonym, such that & pattern") {
    std::string query =
        "assign a; stmt s; Select a such that Parent(s, _) pattern a(s, _)";
    QueryMap expectedQueryMap;
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::ASSIGN, std::vector<std::string>{"a"}));
    expectedQueryMap.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"}));
    expectedQueryMap.addItem(ClauseType::SELECT,
                             PayLoad(SINGLE, Single::SYNONYM,
                                     std::vector<std::string>{"a"},
                                     std::vector<bool>{true}));
    expectedQueryMap.addItem(ClauseType::SUCHTHAT,
                             PayLoad(PAIR, PARENT,
                                     std::vector<std::string>{"s", "_"},
                                     std::vector<bool>{true, false}));
    expectedQueryMap.addItem(ClauseType::PATTERN,
                             PayLoad(TRIPLE, SYN_ASSIGN,
                                     std::vector<std::string>{"a", "s", "_"},
                                     std::vector<bool>{true, true, false}));

    State state(&query);
    QueryMap actualQueryMap = pql_query(state);
    REQUIRE(expectedQueryMap == actualQueryMap);
  }
}

TEST_CASE("Validate declaration clause") {
  SECTION("Passes unique synonyms") {
    // stmt s1 s2; while w1, w2
    QueryMap query_map;
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}));
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s2"}));
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"w1"}));
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"w2"}));

    REQUIRE(is_declaration_clause_valid(query_map));
  }

  SECTION("Fails duplicate synonyms in single entity declaration") {
    // stmt s1 s1;
    QueryMap query_map;
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}));
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}));

    REQUIRE_FALSE(is_declaration_clause_valid(query_map));
  }

  SECTION("Fails duplicate synonyms in multiple entity declaration") {
    // stmt s1; while s1
    QueryMap query_map;
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}));
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::WHILE, std::vector<std::string>{"s1"}));

    REQUIRE_FALSE(is_declaration_clause_valid(query_map));
  }
}

TEST_CASE("Validate select clause") {
  SECTION("Passes synonym declared") {
    // stmt s; Select s
    QueryMap query_map;
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"}));
    query_map.addItem(ClauseType::SELECT, PayLoad(SINGLE, Single::SYNONYM,
                                                  std::vector<std::string>{"s"},
                                                  std::vector<bool>{true}));

    REQUIRE(is_select_clause_valid(query_map));
  }

  SECTION("Fails synonym undeclared") {
    // stmt w; Select s
    QueryMap query_map;
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"w"}));
    query_map.addItem(ClauseType::SELECT, PayLoad(SINGLE, Single::SYNONYM,
                                                  std::vector<std::string>{"s"},
                                                  std::vector<bool>{true}));

    REQUIRE_FALSE(is_select_clause_valid(query_map));
  }

  SECTION("Fails no synonym selected") {
    // stmt w; Select
    QueryMap query_map;
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"w"}));

    REQUIRE_FALSE(is_select_clause_valid(query_map));
  }
}

TEST_CASE("Validate such that clause") {
  SECTION("Passes left synonym declared") {
    // stmt s; Select s such that Parent(s, _)
    QueryMap query_map;
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"}));
    query_map.addItem(ClauseType::SELECT, PayLoad(SINGLE, Single::SYNONYM,
                                                  std::vector<std::string>{"s"},
                                                  std::vector<bool>{true}));
    query_map.addItem(ClauseType::SUCHTHAT,
                      PayLoad(PAIR, PARENT, std::vector<std::string>{"s", "_"},
                              std::vector<bool>{true, false}));

    REQUIRE(is_suchthat_clause_valid(query_map));
  }

  SECTION("Passes right synonym declared") {
    // stmt s; Select s such that Parent(_, s)
    QueryMap query_map;
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"}));
    query_map.addItem(ClauseType::SELECT, PayLoad(SINGLE, Single::SYNONYM,
                                                  std::vector<std::string>{"s"},
                                                  std::vector<bool>{true}));
    query_map.addItem(ClauseType::SUCHTHAT,
                      PayLoad(PAIR, PARENT, std::vector<std::string>{"_", "s"},
                              std::vector<bool>{false, true}));

    REQUIRE(is_suchthat_clause_valid(query_map));
  }

  SECTION("Passes both synonyms declared") {
    // stmt s1, s2; Select s1 such that Parent(_, s)
    QueryMap query_map;
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}));
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s2"}));
    query_map.addItem(ClauseType::SELECT,
                      PayLoad(SINGLE, Single::SYNONYM,
                              std::vector<std::string>{"s1"},
                              std::vector<bool>{true}));
    query_map.addItem(ClauseType::SUCHTHAT,
                      PayLoad(PAIR, PARENT,
                              std::vector<std::string>{"s1", "s2"},
                              std::vector<bool>{true, true}));

    REQUIRE(is_suchthat_clause_valid(query_map));
  }

  SECTION("Fails synonym undeclared") {
    // stmt s1; Select s1 such that Parent(s1, s2)
    QueryMap query_map;
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}));
    query_map.addItem(ClauseType::SELECT,
                      PayLoad(SINGLE, Single::SYNONYM,
                              std::vector<std::string>{"s1"},
                              std::vector<bool>{true}));
    query_map.addItem(ClauseType::SUCHTHAT,
                      PayLoad(PAIR, PARENT,
                              std::vector<std::string>{"s1", "s2"},
                              std::vector<bool>{true, true}));

    REQUIRE_FALSE(is_suchthat_clause_valid(query_map));
  }

  SECTION("Fails synonyms undeclared") {
    // stmt s; Select s such that Parent(s1, s2)
    QueryMap query_map;
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"}));
    query_map.addItem(ClauseType::SELECT, PayLoad(SINGLE, Single::SYNONYM,
                                                  std::vector<std::string>{"s"},
                                                  std::vector<bool>{true}));
    query_map.addItem(ClauseType::SUCHTHAT,
                      PayLoad(PAIR, PARENT,
                              std::vector<std::string>{"s1", "s2"},
                              std::vector<bool>{true, true}));

    REQUIRE_FALSE(is_suchthat_clause_valid(query_map));
  }
}

TEST_CASE("Validate pattern clause") {
  SECTION("Passes synonyms declared") {
    // assign a; stmt s; Select s pattern a(s, _)
    QueryMap query_map;
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::ASSIGN, std::vector<std::string>{"a"}));
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"}));
    query_map.addItem(ClauseType::SELECT, PayLoad(SINGLE, Single::SYNONYM,
                                                  std::vector<std::string>{"s"},
                                                  std::vector<bool>{true}));
    query_map.addItem(ClauseType::PATTERN,
                      PayLoad(TRIPLE, SYN_ASSIGN,
                              std::vector<std::string>{"a", "s", "_"},
                              std::vector<bool>{true, true, false}));

    REQUIRE(is_pattern_clause_valid(query_map));
  }
  
  SECTION("Passes syn-assign is type assign") {
    // assign a; stmt s; Select s pattern a(_, _)
    QueryMap query_map;
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::ASSIGN, std::vector<std::string>{"a"}));
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"}));
    query_map.addItem(ClauseType::SELECT, PayLoad(SINGLE, Single::SYNONYM,
                                                  std::vector<std::string>{"s"},
                                                  std::vector<bool>{true}));
    query_map.addItem(ClauseType::PATTERN,
                      PayLoad(TRIPLE, SYN_ASSIGN,
                              std::vector<std::string>{"a", "_", "_"},
                              std::vector<bool>{true, false, false}));

    REQUIRE(is_pattern_clause_valid(query_map));
  }

  SECTION("Fails synonym undeclared") {
    // assign a; stmt s1; Select s pattern a(s, _)
    QueryMap query_map;
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::ASSIGN, std::vector<std::string>{"a"}));
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"}));
    query_map.addItem(ClauseType::SELECT, PayLoad(SINGLE, Single::SYNONYM,
                                                  std::vector<std::string>{"s"},
                                                  std::vector<bool>{true}));
    query_map.addItem(ClauseType::PATTERN,
                      PayLoad(TRIPLE, SYN_ASSIGN,
                              std::vector<std::string>{"a", "s", "_"},
                              std::vector<bool>{true, true, false}));

    REQUIRE_FALSE(is_pattern_clause_valid(query_map));
  }
  SECTION("Fails syn-assign is not type assign") {
    // assign s; stmt a; Select s pattern a(s, _)
    QueryMap query_map;
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::ASSIGN, std::vector<std::string>{"s"}));
    query_map.addItem(
        ClauseType::DECLARATION,
        PayLoad(SINGLE, Single::STATEMENT, std::vector<std::string>{"a"}));
    query_map.addItem(ClauseType::SELECT, PayLoad(SINGLE, Single::SYNONYM,
                                                  std::vector<std::string>{"s"},
                                                  std::vector<bool>{true}));
    query_map.addItem(ClauseType::PATTERN,
                      PayLoad(TRIPLE, SYN_ASSIGN,
                              std::vector<std::string>{"a", "s", "_"},
                              std::vector<bool>{true, true, false}));

    REQUIRE_FALSE(is_pattern_clause_valid(query_map));
  }
}

TEST_CASE("Validate empty clause") {
  SECTION("Fails no query parsed") {
    QueryMap input_query_map;
    QueryMap expected_query_map;

    REQUIRE_THROWS(pql_validate(input_query_map),
                   throw prettyPrintValidation("Empty query."));
  }
}
