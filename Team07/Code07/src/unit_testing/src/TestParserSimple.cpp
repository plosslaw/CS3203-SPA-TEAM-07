
#include <string>
#include "../../spa/src/TNode.h"
#include "../../spa/src/ParserSimple.h"
#include "../../spa/src/ParserLib.h"
#include "../../../lib/catch.hpp"

std::string checkStringParse(std::string str, std::string (*parser)(State&)) {
  std::string strv = str;
  State state(&str);
  return parser(state);
}

TEST_CASE("string parsers") {
  SECTION("name") {
    REQUIRE(checkStringParse("x", &name) == "x");
    REQUIRE(checkStringParse("x1", &name) == "x1");
    REQUIRE_THROWS(checkStringParse("1x", &name));
    REQUIRE_THROWS(checkStringParse("_x", &name));
  }

  SECTION("integer") {
    REQUIRE(checkStringParse("123", &integer) == "123");
    REQUIRE_THROWS(checkStringParse("x123", &integer));
    REQUIRE_THROWS(checkStringParse(".321", &integer));
    REQUIRE(checkStringParse("0.123", &integer) == "0");
  }
}

TNode checkTNodeParse(std::string str, TNode (*parser)(State&)) {
  State state(&str);
  return parser(state);
}

TEST_CASE("TNode parsers") {
  SECTION("constant") {
    REQUIRE(checkTNodeParse("123", &constant).eq(
      TNode("123", CONSTANT, 0)
    ));
    REQUIRE_THROWS(checkTNodeParse("x", &constant));
  }

  SECTION("variable") {
    REQUIRE_THROWS(checkTNodeParse("123", &variable));
    REQUIRE(checkTNodeParse("x", &variable).eq(
      TNode("x", VARIABLE, 0)
    ));
  }

  SECTION("factor") {
    REQUIRE(checkTNodeParse("123", &factor).eq(
      TNode("123", CONSTANT, 0)
    ));
    REQUIRE(checkTNodeParse("(123)", &factor).eq(
      TNode("123", CONSTANT, 1)
    ));
    REQUIRE(checkTNodeParse("x", &factor).eq(
      TNode("x", VARIABLE, 0)
    ));
    REQUIRE(checkTNodeParse("(x)", &factor).eq(
      TNode("x", VARIABLE, 1)
    ));
    REQUIRE_THROWS(checkTNodeParse("-123", &factor));
  }

  SECTION("term") {
    REQUIRE(checkTNodeParse("1 * 1", &term).eq(
      TNode("*", EXPR, 2).addChild(
        TNode("1", CONSTANT, 0)).addChild(
        TNode("1", CONSTANT, 4))
    ));
    REQUIRE(checkTNodeParse("1 / 1", &term).eq(
      TNode("/", EXPR, 2).addChild(
        TNode("1", CONSTANT, 0)).addChild(
        TNode("1", CONSTANT, 4))
    ));
    REQUIRE(checkTNodeParse("1 % 1", &term).eq(
      TNode("%", EXPR, 2).addChild(
        TNode("1", CONSTANT, 0)).addChild(
        TNode("1", CONSTANT, 4))
    ));
    REQUIRE_THROWS(checkTNodeParse("1 * -1", &term));
  }

  SECTION("expr") {
    REQUIRE(checkTNodeParse("1 + 1", &expr).eq(
      TNode("+", EXPR, 2).addChild(
        TNode("1", CONSTANT, 0)).addChild(
        TNode("1", CONSTANT, 4))
    ));
    REQUIRE(checkTNodeParse("1 - 1", &expr).eq(
      TNode("-", EXPR, 2).addChild(
        TNode("1", CONSTANT, 0)).addChild(
        TNode("1", CONSTANT, 4))
    ));
    REQUIRE_THROWS(checkTNodeParse("1 - -1", &expr));
  }

  SECTION("rel_expr") {
    REQUIRE(checkTNodeParse("1 + 1 > 2 + 2", &rel_expr).eq(
      TNode(">", EXPR, 0).addChild(
        TNode("+", EXPR, 2).addChild(
          TNode("1", CONSTANT, 0)).addChild(
          TNode("1", CONSTANT, 4))).addChild(
        TNode("+", EXPR, 10).addChild(
          TNode("2", CONSTANT, 8)).addChild(
          TNode("2", CONSTANT, 12)))
    ));
    REQUIRE(checkTNodeParse("1 - 1 < 3 / 2", &rel_expr).eq(
      TNode("<", EXPR, 0).addChild(
        TNode("-", EXPR, 2).addChild(
          TNode("1", CONSTANT, 0)).addChild(
          TNode("1", CONSTANT, 4))).addChild(
        TNode("/", EXPR, 10).addChild(
          TNode("3", CONSTANT, 8)).addChild(
          TNode("2", CONSTANT, 12)))
    ));
    REQUIRE(checkTNodeParse("1 == 1", &rel_expr).eq(
      TNode("==", EXPR, 0).addChild(
        TNode("1", CONSTANT, 0)).addChild(
        TNode("1", CONSTANT, 5))
    ));
    REQUIRE_THROWS(checkTNodeParse("(1 == 1)", &rel_expr));
    REQUIRE_THROWS(checkTNodeParse("1 === 1", &rel_expr));
  }

  SECTION("cond_expr") {
    REQUIRE(checkTNodeParse("! ( 1 == 1)", &cond_expr).eq(
      TNode("!", EXPR, 0).addChild(
        TNode("==", EXPR, 4).addChild(
          TNode("1", CONSTANT,4)).addChild(
          TNode("1", CONSTANT, 9)))
    ));
    REQUIRE_THROWS(checkTNodeParse("! 1 == 1", &cond_expr));
    REQUIRE(checkTNodeParse("(1 == 1) && (2 == 2)", &cond_expr).eq(
      TNode("&&", EXPR, 0).addChild(
        TNode("==", EXPR, 1).addChild(
          TNode("1", CONSTANT, 1)).addChild(
          TNode("1", CONSTANT, 6))).addChild(
        TNode("==", EXPR, 13).addChild(
          TNode("2", CONSTANT, 13)).addChild(
          TNode("2", CONSTANT, 18)))
    ));
    REQUIRE(checkTNodeParse("1 == 1 && 2 == 2", &cond_expr).eq(
      TNode("==", EXPR, 0).addChild(
        TNode("1", CONSTANT, 0)).addChild(
        TNode("1", CONSTANT, 5))
    ));
    REQUIRE(checkTNodeParse("(1 == 1) || (2 == 2)", &cond_expr).eq(
      TNode("||", EXPR, 0).addChild(
        TNode("==", EXPR, 1).addChild(
          TNode("1", CONSTANT, 1)).addChild(
          TNode("1", CONSTANT, 6))).addChild(
        TNode("==", EXPR, 13).addChild(
          TNode("2", CONSTANT, 13)).addChild(
          TNode("2", CONSTANT, 18)))
    ));
    REQUIRE(checkTNodeParse("1 == 1 || 2 == 2", &cond_expr).eq(
      TNode("==", EXPR, 0).addChild(
        TNode("1", CONSTANT, 0)).addChild(
        TNode("1", CONSTANT, 5))
    ));
    REQUIRE_THROWS(checkTNodeParse("(1 == 1) &|& (2 == 2)", &cond_expr));
    REQUIRE(checkTNodeParse("1 == 1 |&| 2 == 2", &cond_expr).eq(
      TNode("==", EXPR, 0).addChild(
        TNode("1", CONSTANT, 0)).addChild(
        TNode("1", CONSTANT, 5))
    ));
  }

  SECTION("assign") {
    REQUIRE(checkTNodeParse("x = 1;", &assign).eq(
      TNode(1, "", ASSIGN, 0).addChild(
        TNode("x", VARIABLE, 0)).addChild(
        TNode("1", CONSTANT, 4))
    ));
    REQUIRE_THROWS(checkTNodeParse("x = 1", &assign));
    REQUIRE(checkTNodeParse("x = 1 + 1 / 2 * 3;", &assign).eq(
      TNode(1, "", ASSIGN, 0).addChild(
        TNode("x", VARIABLE, 0)).addChild(
        TNode("+", EXPR, 6).addChild(
          TNode("1", CONSTANT, 4)).addChild(
          TNode("*", EXPR, 14).addChild(
            TNode("/", EXPR, 10).addChild(
              TNode("1", CONSTANT, 8)).addChild(
              TNode("2", CONSTANT, 12))).addChild(
            TNode("3", CONSTANT, 16))))
    ));
    REQUIRE_THROWS(checkTNodeParse("x = !(1 == 1);", &assign));
  }

  SECTION("if_stmt") {
    REQUIRE(checkTNodeParse("if(x == 1) then { x = 2; } else { x = 3; }", &if_stmt).eq(
      TNode(1, "", IF, 0).addChild(
        TNode("==", EXPR, 3).addChild(
          TNode("x", VARIABLE, 3)).addChild(
          TNode("1", CONSTANT, 8))).addChild(
        TNode("", STATEMENTLIST, 18).addChild(
          TNode(2, "", ASSIGN, 18).addChild(
            TNode("x", VARIABLE, 18)).addChild(
            TNode("2", CONSTANT, 22)))).addChild(
        TNode("", STATEMENTLIST, 34).addChild(
          TNode(3, "", ASSIGN, 34).addChild(
            TNode("x", VARIABLE, 34)).addChild(
            TNode("3", CONSTANT, 38))))
    ));
    REQUIRE_THROWS(checkTNodeParse("if(x == 1 then { x = 2; } else { x = 3; }", &if_stmt));
    REQUIRE_THROWS(checkTNodeParse("if(x == 1) { x = 2; } else { x = 3; }", &if_stmt));
    REQUIRE_THROWS(checkTNodeParse("if(x == 1) then { x = 2; } else { }", &if_stmt));
    REQUIRE_THROWS(checkTNodeParse("if(x == 1) then {  } else { x = 2; }", &if_stmt));
    REQUIRE_THROWS(checkTNodeParse("if(x == 1) then {  } ", &if_stmt));
  }

  SECTION("while_stmt") {
    REQUIRE(checkTNodeParse("while(x < 10) { x = x + 1; }", &while_stmt).eq(
      TNode(1, "", WHILE, 0).addChild(
        TNode("<", EXPR, 6).addChild(
          TNode("x", VARIABLE, 6)).addChild(
          TNode("10", CONSTANT, 10))).addChild(
        TNode("", STATEMENTLIST, 16).addChild(
          TNode(2, "", ASSIGN, 16).addChild(
            TNode("x", VARIABLE, 16)).addChild(
            TNode("+", EXPR, 22).addChild(
              TNode("x", VARIABLE, 20)).addChild(
              TNode("1", CONSTANT, 24)))))
    ));
    REQUIRE_THROWS(checkTNodeParse("while(x < 10) then { x = x + 1; }", &while_stmt));
    REQUIRE_THROWS(checkTNodeParse("while(x < 10) { }", &while_stmt));
  }

  SECTION("call") {
    REQUIRE(checkTNodeParse("call f;", &call).eq(
      TNode(1, "f", CALL, 0)
    ));
    REQUIRE_THROWS(checkTNodeParse("call f", &call));
    REQUIRE_THROWS(checkTNodeParse("cal f;", &call));
    REQUIRE_THROWS(checkTNodeParse("callf;", &call));
    REQUIRE_THROWS(checkTNodeParse("call 123;", &call));
  }

  SECTION("print") {
    REQUIRE(checkTNodeParse("print f;", &print).eq(
      TNode(1, "f", PRINT, 0)
    ));
    REQUIRE_THROWS(checkTNodeParse("print f", &print));
    REQUIRE_THROWS(checkTNodeParse("prin f;", &print));
    REQUIRE_THROWS(checkTNodeParse("printf;", &print));
    REQUIRE_THROWS(checkTNodeParse("print 123;", &print));
  }

  SECTION("read") {
    REQUIRE(checkTNodeParse("read f;", &read).eq(
      TNode(1, "f", READ, 0)
    ));
    REQUIRE_THROWS(checkTNodeParse("read f", &read));
    REQUIRE_THROWS(checkTNodeParse("rea f;", &read));
    REQUIRE_THROWS(checkTNodeParse("readf;", &read));
    REQUIRE_THROWS(checkTNodeParse("read 123;", &read));
  }

  SECTION("stmt") {
    REQUIRE(checkTNodeParse("read f;", &stmt).eq(
      TNode(1, "f", READ, 0)
    ));
    REQUIRE(checkTNodeParse("print f;", &stmt).eq(
      TNode(1, "f", PRINT, 0)
    ));
    REQUIRE(checkTNodeParse("call f;", &stmt).eq(
      TNode(1, "f", CALL, 0)
    ));
    REQUIRE(checkTNodeParse("while(x < 10) { x = x + 1; }", &stmt).eq(
      TNode(1, "", WHILE, 0).addChild(
        TNode("<", EXPR, 6).addChild(
          TNode("x", VARIABLE, 6)).addChild(
          TNode("10", CONSTANT, 10))).addChild(
        TNode("", STATEMENTLIST, 16).addChild(
          TNode(2, "", ASSIGN, 16).addChild(
            TNode("x", VARIABLE, 16)).addChild(
            TNode("+", EXPR, 22).addChild(
              TNode("x", VARIABLE, 20)).addChild(
              TNode("1", CONSTANT, 24)))))
    ));
    REQUIRE(checkTNodeParse("if(x == 1) then { x = 2; } else { x = 3; }", &stmt).eq(
      TNode(1, "", IF, 0).addChild(
        TNode("==", EXPR, 3).addChild(
          TNode("x", VARIABLE, 3)).addChild(
          TNode("1", CONSTANT, 8))).addChild(
        TNode("", STATEMENTLIST, 18).addChild(
          TNode(2, "", ASSIGN, 18).addChild(
            TNode("x", VARIABLE, 18)).addChild(
            TNode("2", CONSTANT, 22)))).addChild(
        TNode("", STATEMENTLIST, 34).addChild(
          TNode(3, "", ASSIGN, 34).addChild(
            TNode("x", VARIABLE, 34)).addChild(
            TNode("3", CONSTANT, 38))))
    ));
    REQUIRE(checkTNodeParse("x = 1;", &stmt).eq(
      TNode(1, "", ASSIGN, 0).addChild(
        TNode("x", VARIABLE, 0)).addChild(
        TNode("1", CONSTANT, 4))
    ));
    REQUIRE_THROWS(checkTNodeParse("-1;", &stmt));
  }

  SECTION("stmtLst") {
    REQUIRE(checkTNodeParse("read f; }", &stmtLst).eq(
      TNode("", STATEMENTLIST, 0).addChild(
        TNode(1, "f", READ, 0))
    ));
    REQUIRE(checkTNodeParse("read f; read f; }", &stmtLst).eq(
      TNode("", STATEMENTLIST, 0).addChild(
        TNode(1, "f", READ, 0)).addChild(
        TNode(2, "f", READ, 8))
    ));
    REQUIRE_THROWS(checkTNodeParse("}", &stmtLst));
  }

  SECTION("procedure") {
    REQUIRE(checkTNodeParse("procedure p { read f; }", &procedure).eq(
      TNode("p", PROCEDURE, 0).addChild(
        TNode("", STATEMENTLIST, 14).addChild(
          TNode(1, "f", READ, 14)))
    ));
    REQUIRE_THROWS(checkTNodeParse("procedure p read f; }", &procedure));
    REQUIRE_THROWS(checkTNodeParse("procedure { read f; }", &procedure));
  }

  SECTION("program") {
    REQUIRE(checkTNodeParse("procedure main { x = 1; }", &program).eq(
      TNode("", PROGRAM, 0).addChild(
        TNode("main", PROCEDURE, 0).addChild(
          TNode("", STATEMENTLIST, 17).addChild(
            TNode(1, "", ASSIGN, 17).addChild(
              TNode("x", VARIABLE, 17)).addChild(
              TNode("1", CONSTANT, 21)))))
    ));
    REQUIRE(checkTNodeParse("procedure main { x = 1; } procedure f { x = 2; }", &program).eq(
      TNode("", PROGRAM, 0).addChild(
        TNode("main", PROCEDURE, 0).addChild(
          TNode("", STATEMENTLIST, 17).addChild(
            TNode(1, "", ASSIGN, 17).addChild(
              TNode("x", VARIABLE, 17)).addChild(
              TNode("1", CONSTANT, 21))))).addChild(
        TNode("f", PROCEDURE, 26).addChild(
          TNode("", STATEMENTLIST, 40).addChild(
            TNode(2, "", ASSIGN, 40).addChild(
              TNode("x", VARIABLE, 40)).addChild(
              TNode("2", CONSTANT, 44)))))
    ));
    REQUIRE_THROWS(checkTNodeParse("procedure main { x = 1; } unexpected chars", &program));
  }
}