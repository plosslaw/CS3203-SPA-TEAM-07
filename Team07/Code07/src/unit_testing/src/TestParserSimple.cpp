
#include <string>
#include "../../spa/src/TNode.h"
#include "../../spa/src/ParserSimple.h"
#include "../../spa/src/ParserLib.h"
#include "../../../lib/catch.hpp"
#include <iostream>

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
    REQUIRE_NOTHROW(checkTNodeParse("123", &constant));
    REQUIRE_THROWS(checkTNodeParse("x", &constant));
  }

  SECTION("variable") {
    REQUIRE_THROWS(checkTNodeParse("123", &variable));
    REQUIRE_NOTHROW(checkTNodeParse("x", &variable));
  }

  SECTION("factor") {
    REQUIRE_NOTHROW(checkTNodeParse("123", &factor));
    REQUIRE_NOTHROW(checkTNodeParse("(123)", &factor));
    REQUIRE_NOTHROW(checkTNodeParse("x", &factor));
    REQUIRE_NOTHROW(checkTNodeParse("(x)", &factor));
    REQUIRE_THROWS(checkTNodeParse("-123", &factor));
  }

  SECTION("term") {
    REQUIRE_NOTHROW(checkTNodeParse("1 * 1", &term));
    REQUIRE_NOTHROW(checkTNodeParse("1 / 1", &term));
    REQUIRE_NOTHROW(checkTNodeParse("1 % 1", &term));
    REQUIRE_THROWS(checkTNodeParse("1 * -1", &term));
  }

  SECTION("expr") {
    REQUIRE_NOTHROW(checkTNodeParse("1 + 1", &expr));
    REQUIRE_NOTHROW(checkTNodeParse("1 - 1", &expr));
    REQUIRE_THROWS(checkTNodeParse("1 - -1", &expr));
  }

  SECTION("rel_expr") {
    REQUIRE_NOTHROW(checkTNodeParse("1 + 1 > 2 + 2", &rel_expr));
    REQUIRE_NOTHROW(checkTNodeParse("1 - 1 < 3 / 2", &rel_expr));
    REQUIRE_NOTHROW(checkTNodeParse("1 == 1", &rel_expr));
    REQUIRE_THROWS(checkTNodeParse("(1 == 1)", &rel_expr));
    REQUIRE_THROWS(checkTNodeParse("1 === 1", &rel_expr));
  }

  SECTION("cond_expr") {
    std::string stub1 = checkTNodeParse("1 == 1", &cond_expr).toSexp();
    REQUIRE_NOTHROW(checkTNodeParse("! ( 1 == 1)", &cond_expr));
    REQUIRE_THROWS(checkTNodeParse("! 1 == 1", &cond_expr));
    REQUIRE_NOTHROW(checkTNodeParse("(1 == 1) && (2 == 2)", &cond_expr));
    REQUIRE(checkTNodeParse("1 == 1 && 2 == 2", &cond_expr).toSexp() == stub1);
    REQUIRE_FALSE(checkTNodeParse("(1 == 1) || (2 == 2)", &cond_expr).toSexp() == checkTNodeParse("1 == 1", &cond_expr).toSexp());
    REQUIRE(checkTNodeParse("1 == 1 || 2 == 2", &cond_expr).toSexp() == stub1);
    REQUIRE_THROWS(checkTNodeParse("(1 == 1) &|& (2 == 2)", &cond_expr));
    REQUIRE(checkTNodeParse("1 == 1 |&| 2 == 2", &cond_expr).toSexp() == stub1);
  }

  SECTION("assign") {
    REQUIRE_NOTHROW(checkTNodeParse("x = 1;", &assign));
    REQUIRE_THROWS(checkTNodeParse("x = 1", &assign));
    REQUIRE_NOTHROW(checkTNodeParse("x = 1 + 1 / 2 * 3;", &assign));
    REQUIRE_THROWS(checkTNodeParse("x = !(1 == 1);", &assign));
  }

  SECTION("if_stmt") {
    REQUIRE_NOTHROW(checkTNodeParse("if(x == 1) then { x = 2; } else { x = 3; }", &if_stmt));
    REQUIRE_THROWS(checkTNodeParse("if(x == 1 then { x = 2; } else { x = 3; }", &if_stmt));
    REQUIRE_THROWS(checkTNodeParse("if(x == 1) { x = 2; } else { x = 3; }", &if_stmt));
    REQUIRE_THROWS(checkTNodeParse("if(x == 1) then { x = 2; } else { }", &if_stmt));
    REQUIRE_THROWS(checkTNodeParse("if(x == 1) then {  } else { x = 2; }", &if_stmt));
    REQUIRE_THROWS(checkTNodeParse("if(x == 1) then {  } ", &if_stmt));
  }

  SECTION("while_stmt") {
    REQUIRE_NOTHROW(checkTNodeParse("while(x < 10) { x = x + 1; }", &while_stmt));
    REQUIRE_THROWS(checkTNodeParse("while(x < 10) then { x = x + 1; }", &while_stmt));
    REQUIRE_THROWS(checkTNodeParse("while(x < 10) { }", &while_stmt));
  }

  SECTION("call") {
    REQUIRE_NOTHROW(checkTNodeParse("call f;", &call));
    REQUIRE_THROWS(checkTNodeParse("call f", &call));
    REQUIRE_THROWS(checkTNodeParse("cal f;", &call));
    REQUIRE_THROWS(checkTNodeParse("callf;", &call));
    REQUIRE_THROWS(checkTNodeParse("call 123;", &call));
  }

  SECTION("print") {
    REQUIRE_NOTHROW(checkTNodeParse("print f;", &print));
    REQUIRE_THROWS(checkTNodeParse("print f", &print));
    REQUIRE_THROWS(checkTNodeParse("prin f;", &print));
    REQUIRE_THROWS(checkTNodeParse("printf;", &print));
    REQUIRE_THROWS(checkTNodeParse("print 123;", &print));
  }

  SECTION("read") {
    REQUIRE_NOTHROW(checkTNodeParse("read f;", &read));
    REQUIRE_THROWS(checkTNodeParse("read f", &read));
    REQUIRE_THROWS(checkTNodeParse("rea f;", &read));
    REQUIRE_THROWS(checkTNodeParse("readf;", &read));
    REQUIRE_THROWS(checkTNodeParse("read 123;", &read));
  }

  SECTION("stmt") {
    REQUIRE_NOTHROW(checkTNodeParse("read f;", &stmt));
    REQUIRE_NOTHROW(checkTNodeParse("print f;", &stmt));
    REQUIRE_NOTHROW(checkTNodeParse("call f;", &stmt));
    REQUIRE_NOTHROW(checkTNodeParse("while(x < 10) { x = x + 1; }", &stmt));
    REQUIRE_NOTHROW(checkTNodeParse("if(x == 1) then { x = 2; } else { x = 3; }", &stmt));
    REQUIRE_NOTHROW(checkTNodeParse("x = 1;", &stmt));
    REQUIRE_THROWS(checkTNodeParse("-1;", &stmt));
  }

  SECTION("stmtLst") {
    REQUIRE_NOTHROW(checkTNodeParse("read f; }", &stmtLst));
    REQUIRE_NOTHROW(checkTNodeParse("read f; read f; }", &stmtLst));
    REQUIRE_THROWS(checkTNodeParse("}", &stmtLst));
  }

  SECTION("procedure") {
    REQUIRE_NOTHROW(checkTNodeParse("procedure p { read f; }", &procedure));
    REQUIRE_THROWS(checkTNodeParse("procedure p read f; }", &procedure));
    REQUIRE_THROWS(checkTNodeParse("procedure { read f; }", &procedure));
  }

  SECTION("program") {
    REQUIRE_NOTHROW(checkTNodeParse("procedure main { x = 1; }", &program));
    REQUIRE_NOTHROW(checkTNodeParse("procedure main { x = 1; } procedure f { x = 2; }", &program));
    REQUIRE_THROWS(checkTNodeParse("procedure main { x = 1; } unexpected chars", &program));
  }
}