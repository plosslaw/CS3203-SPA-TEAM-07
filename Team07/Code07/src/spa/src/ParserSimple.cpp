#include <string>
#include <vector>
#include "ParserLib.h"
#include "ParserSimple.h"
#include "TNode.h"
#include "Types.hpp"

using namespace std;

/** name :- letter (letter | digit)* */
string name(State &s) {
  int init = s.i;
  try {
    char r1 = charPredicate(s, &alphaPred, "letter");
    // :- letter
    string r2 = alphaNum(s);
    // :- (letter | digit)*
    r2.insert(r2.begin(), r1);
    return r2;
  } catch (ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(init, s.i, "name", "");
  }
}

/** integer :- digit digit* */
string integer(State &s) {
  int init = s.i;
  try {
    char r1 = charPredicate(s, &digitPred, "digit");
    // :- digit
    string r2 = digit(s);
    // :- digit*
    r2.insert(r2.begin(), r1);
    return r2;
  } catch (ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(init, s.i, "integer", "");
  }
}

/** program :- procedure+ */
TNode program(State &s) {
  State so(s);
  TNode t("", PROGRAM);
	bool consumed = false;
	try {
		while(true) {
			t.addChild(procedure(s));
			so.assign(s);
			consumed = true;
		}
	} catch (ParseException &e){
		if(!consumed) {
			s.excps.push_back(e);
			throw ParseException(so.i, s.i, "program", "");
		}
		s.assign(so);
    return t;
	}
}

/** procedure :- 'procedure' name '{' stmtLst '}' */
TNode procedure(State &s) {
  State so(s);
  try {
    stringMatch(s, "procedure");
    whitespace(s);
    string proc_name = name(s);
    whitespace(s);
    stringMatch(s, "{");
    whitespace(s);
    TNode t1 = stmtLst(s);
    stringMatch(s, "}");
    whitespace(s);
    TNode t(proc_name, PROCEDURE);
    t.addChild(t1);
    return t;
  } catch(ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(so.i, s.i, "procedure", "");
  }
}

/** stmtLst :- stmt+ */
TNode stmtLst(State &s) {
  State so(s);
  TNode t("", STATEMENTLIST);
	bool consumed = false;
	try {
		while(true) {
			t.addChild(stmt(s));
      whitespace(s);
			so.assign(s);
			consumed = true;
		}
	} catch (ParseException &e){
		if(!consumed) {
			s.excps.push_back(e);
			throw ParseException(so.i, s.i, "stmtLst", "");
		}
		s.assign(so);
    return t;
	}
}

/** stmt :- read | print | call | while_stmt | if_stmt | assign */
TNode stmt(State &s) {
  State so(s);
  try {
    return read(s);
  } catch(ParseException &e) {
    s.assign(so);
    try {
      return print(s);
    } catch(ParseException &e) {
      s.assign(so);
      try {
        return call(s);
      } catch(ParseException &e) {
        s.assign(so);
        try {
          return while_stmt(s);
        } catch(ParseException &e) {
          s.assign(so);
          try {
            return if_stmt(s);
          } catch(ParseException &e) {
            s.assign(so);
            try {
              return assign(s);
            } catch(ParseException &e) {
              s.excps.push_back(e);
              throw ParseException(so.i, s.i, "stmt", "");
            }
          }
        }
      }
    }
  }
}

/** X :- <X> name ';' */
TNode unaryOp(State &s, string op, stmt_type typ) {
  int init = s.i;
  try {
    stringMatch(s, op);
    // :- 'read'
    whitespace(s);
    string n = name(s);
    // :- name
    whitespace(s);
    stringMatch(s, ";");
    // :- ';'
    whitespace(s);
    return TNode(s.advCurStmtNum(), n, typ);
  } catch(ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(init, s.i, op, "");
  }
}

/** read :- 'read' name ';' */
TNode read(State &s) {
  return unaryOp(s, "read", READ);
}

/** print :- 'print' name ';' */
TNode print(State &s) {
  return unaryOp(s, "print", PRINT);
}

/** call :- 'call' name ';' */
TNode call(State &s) {
  return unaryOp(s, "call", CALL);
}

/** while_stmt :- 'while' '(' cond_expr ')' '{' stmtLst '}' */
TNode while_stmt(State &s) {
  int init = s.i;
  int initNum = s.curStmtNum;
  TNode t(s.advCurStmtNum(), "", WHILE);
  try {
    stringMatch(s, "while");
    whitespace(s);
    stringMatch(s, "(");
    whitespace(s);
    TNode t1 = cond_expr(s);
    stringMatch(s, ")");
    whitespace(s);
    stringMatch(s, "{");
    whitespace(s);
    TNode t2 = stmtLst(s);
    stringMatch(s, "}");
    whitespace(s);
    t.addChild(t1);
    t.addChild(t2);
    return t;
  } catch(ParseException &e) {
    s.excps.push_back(e);
    s.curStmtNum = initNum;
    throw ParseException(init, s.i, "while_stmt", "");
  }
}

/** if_stmt :- 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}' */
TNode if_stmt(State &s) {
  int init = s.i;
  int initNum = s.curStmtNum;
  TNode t(s.advCurStmtNum(), "", IF);
  try {
    stringMatch(s, "if");
    whitespace(s);
    stringMatch(s, "(");
    whitespace(s);
    TNode t1 = cond_expr(s);
    stringMatch(s, ")");
    whitespace(s);
    stringMatch(s, "then");
    whitespace(s);
    stringMatch(s, "{");
    whitespace(s);
    TNode t2 = stmtLst(s);
    stringMatch(s, "}");
    whitespace(s);
    stringMatch(s, "else");
    whitespace(s);
    stringMatch(s, "{");
    whitespace(s);
    TNode t3 = stmtLst(s);
    stringMatch(s, "}");
    whitespace(s);
    t.addChild(t1);
    t.addChild(t2);
    t.addChild(t3);
    return t;
  } catch(ParseException &e) {
    s.excps.push_back(e);
    s.curStmtNum = initNum;
    throw ParseException(init, s.i, "if_stmt", "");
  }
}

TNode assign(State &s) {
  int init = s.i;
  int initNum = s.curStmtNum;
  try {
    TNode t1 = variable(s);
    whitespace(s);
    stringMatch(s, "=");
    whitespace(s);
    TNode t2 = expr(s);
    stringMatch(s, ";");
    TNode t(s.advCurStmtNum(), "", ASSIGN);
    t.addChild(t1);
    t.addChild(t2);
    return t;
  } catch(ParseException &e) {
    s.excps.push_back(e);
    s.curStmtNum = initNum;
    throw ParseException(init, s.i, "assign", "");
  }
}

/** cond_expr :- rel_expr | '!' '(' cond_expr ')' | '(' cond_expr ')' ('&&'|'||') '(' cond_expr ')' */
TNode cond_expr(State &s) {
  State so(s);
  try {
    return rel_expr(s);
  } catch(ParseException &e) {
    s.assign(so);
    try {
      stringMatch(s, "!");
      whitespace(s);
      stringMatch(s, "(");
      whitespace(s);
      TNode t1 = cond_expr(s);
      stringMatch(s, ")");
      whitespace(s);
      TNode t("!", EXPR);
      t.addChild(t1);
      return t;
    } catch(ParseException &e) {
      s.assign(so);
      try {
        s.assign(so);
        stringMatch(s, "(");
        whitespace(s);
        TNode t1 = cond_expr(s);
        stringMatch(s, ")");
        whitespace(s);
        State so1(s);
        string op;
        try {
          op = stringMatch(s, "&&");
        } catch(ParseException &e) {
          try {
            op = stringMatch(s, "||");
          } catch(ParseException &e) {
            s.excps.push_back(e);
            throw ParseException(so1.i, s.i, "cond_expr", "bin_op");
          }
        }
        whitespace(s);
        stringMatch(s, "(");
        whitespace(s);
        TNode t2 = cond_expr(s);
        stringMatch(s, ")");
        whitespace(s);
        TNode t(op, EXPR);
        t.addChild(t1);
        t.addChild(t2);
        return t;
      } catch(ParseException &e) {
        s.excps.push_back(e);
        throw ParseException(so.i, s.i, "cond_expr", "root");
      }
    }
  }
}

/** rel_expr :- expr ('>'|'>='|'<'|'<='|'=='|'!=') expr */
TNode rel_expr(State &s) {
  State so(s);
  TNode lchild;
  try {
    lchild = rel_factor(s);
    whitespace(s);
  } catch (ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(so.i, s.i, "rel_expr", "first_factor");
  }
  string op;
  so.assign(s);
  try {
    op = stringMatch(s, ">=");
  } catch(ParseException &e) {
    s.assign(so);
    try {
      op = stringMatch(s, ">");
    } catch(ParseException &e) {
      s.assign(so);
      try {
        op = stringMatch(s, "<=");
      } catch(ParseException &e) {
        s.assign(so);
        try {
          op = stringMatch(s, "<");
        } catch(ParseException &e) {
          s.assign(so);
          try {
            op = stringMatch(s, "==");
          } catch(ParseException &e) {
            s.assign(so);
            try {
              op = stringMatch(s, "!=");
            } catch(ParseException &e) {
              s.excps.push_back(e);
              throw ParseException(so.i, s.i, "rel_expr", "operator");
            }
          }
        }
      }
    }
  }
  so.assign(s);
  try {
    whitespace(s);
    TNode rchild = rel_factor(s);
    whitespace(s);
    TNode t(op, EXPR);
    t.addChild(lchild);
    t.addChild(rchild);
    return t;
  } catch(ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(so.i, s.i, "rel_expr", "second_factor");
  }
}

/** rel_factor :- expr */
TNode rel_factor(State &s) {
  State so(s);
  try {
    return expr(s);
  } catch (ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(so.i, s.i, "rel_factor", "");
  }
}

/** expr :- term expr_1 */
TNode expr(State &s) {
  int init = s.i;
  try {
    TNode c1 = term(s);
    // :- term
    whitespace(s);
    return expr_1(s, c1);
    // :- expr_1
  } catch(ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(init, s.i, "expr", "");
  }
}

/** expr_1 :- ('+'|'-') term expr_1 | e */
TNode expr_1(State &s, TNode &lchild) {
  State so(s);
  string op;
  try {
    op = stringMatch(s, "+");
  } catch(ParseException &e) {
    s.assign(so);
    try {
      op = stringMatch(s, "-");
    } catch(ParseException &e) {
      s.assign(so);
      return lchild;
      // :- e
    }
  }
  // :- ('+'|'-')
  try {
    whitespace(s);
    TNode rchild = term(s);
    // :- term
    TNode t(op, EXPR);
    t.addChild(lchild);
    t.addChild(rchild);
    return expr_1(s, t);
    // :- expr_1
  } catch(ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(so.i, s.i, "expr_1", "");
  }
}

/** term :- factor term_1 */
TNode term(State &s) {
  int init = s.i;
  try {
    TNode c1 = factor(s);
    // :- factor
    whitespace(s);
    return term_1(s, c1);
    // :- term_1
  } catch(ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(init, s.i, "term", "");
  }
}

/** term_1 :- ('*'|'/'|'%') factor term_1 | e */
TNode term_1(State &s, TNode &lchild) {
  State so(s);
  string op;
  try {
    op = stringMatch(s, "*");
  } catch(ParseException &e) {
    s.assign(so);
    try {
      op = stringMatch(s, "/");
    } catch(ParseException &e) {
      s.assign(so);
      try {
        op = stringMatch(s, "%");
      } catch(ParseException &e) {
        s.assign(so);
        return lchild;
        // :- e
      }
    }
  }
  // :- ('*'|'/'|'%')
  try {
    whitespace(s);
    TNode rchild = factor(s);
    // :- factor
    whitespace(s);
    TNode t(op, EXPR);
    t.addChild(lchild);
    t.addChild(rchild);
    return term_1(s, t);
    // :- term_1
  } catch(ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(so.i, s.i, "term_1", "");
  }
}

/** factor :- variable | constant | '(' expr ')' */
TNode factor(State &s) {
  State so(s);
  try {
    return variable(s);
  } catch (ParseException &e) {
    s.assign(so);
    try {
      return constant(s);
    } catch (ParseException &e) {
      s.assign(so);
      try {
        stringMatch(s, "(");
        whitespace(s);
        TNode t = expr(s);
        whitespace(s);
        stringMatch(s, ")");
        return t;
      } catch (ParseException &e) {
        s.excps.push_back(e);
        throw ParseException(so.i, s.i, "factor", "");
      }
    }
  }
}

/** variable :- name */
TNode variable(State &s) {
  int init = s.i;
  try {
    string v = name(s);
    return TNode(v, VARIABLE);
  } catch(ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(init, s.i, "variable", "");
  }
}

/** constant :- integer */
TNode constant(State &s) {
  int init = s.i;
  try {
    string v = integer(s);
    return TNode(v, CONSTANT);
  } catch(ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(init, s.i, "constant", "");
  }
}