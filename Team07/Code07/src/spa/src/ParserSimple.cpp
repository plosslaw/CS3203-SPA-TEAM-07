#include <string>
#include <vector>
#include <unordered_set>
#include "ParserLib.h"
#include "ParserSimple.h"
#include "ParserLib.h"
#include "TNode.h"
#include "Types.hpp"
#include <string>
#include <vector>

/** program :- procedure+ */
TNode program(State &s) {
  State so(s);
  TNode t("", PROGRAM, 0);
	bool consumed = false;
	try {
		while(true) {
			t.addChild(procedure(s));
			so.assign(s);
			consumed = true;
		}
	} catch (ParseException &e){
		if(!consumed) {
			throw e;
		}
    if (so.i != (*s.source).size()) {
      throw e;
    }
    return t;
  }
}

/** procedure :- 'procedure' name '{' stmtLst '}' */
TNode procedure(State &s) {
  State so(s);
  bool stmtLstFail = false;
  std::string proc_name;
  try {
    string_match(s, "procedure");
    whitespace(s);
    proc_name = name(s);
    whitespace(s);
    char_match(s, '{');
    whitespace(s);
    stmtLstFail = true;
    TNode t1 = stmtLst(s);
    stmtLstFail = false;
    TNode t(proc_name, PROCEDURE, so.i);
    t.addChild(t1);
    return t;
  } catch (ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(so.i, s.i, "procedure", stmtLstFail ? proc_name : "");
  }
}

/** stmtLst :- stmt+ '}' */
TNode stmtLst(State &s) {
  State so(s);
  TNode t("", STATEMENTLIST, so.i);
  bool consumed = false;
  try {
    while (true) {
      t.addChild(stmt(s));
      whitespace(s);
      so.assign(s);
      consumed = true;
    }
  } catch (ParseException &e) {
    if (!consumed) {
      throw e;
    }
    State so1(s);
    s.assign(so);
    try {
      char_match(s, '}');
      whitespace(s);
      return t;
    } catch (ParseException &eUnused) {
      s.assign(so1);
      throw e;
    }
  }
}

/** stmt :- read | print | call | while_stmt | if_stmt | assign */
TNode stmt(State &s) {
  State so(s);
  bool isPartial = false;
  ParseException partial(0,0,"","");
  try {
    return read(s);
  } catch (ParseException &e) {
    if (e.args.compare("partial") == 0) {
      isPartial = true;
      partial = e;
    }
    s.assign(so);
    try {
      return print(s);
    } catch (ParseException &e) {
      if (e.args.compare("partial") == 0) {
        isPartial = true;
        partial = e;
      }
      s.assign(so);
      try {
        return call(s);
      } catch (ParseException &e) {
        if (e.args.compare("partial") == 0) {
          isPartial = true;
          partial = e;
        }
        s.assign(so);
        try {
          return while_stmt(s);
        } catch(ParseException &e) {
          if (e.args.compare("body") == 0) {
            throw e;
          }
          s.assign(so);
          try {
            return if_stmt(s);
          } catch(ParseException &e) {
            if (e.args.compare("body") == 0) {
              throw e;
            }
            s.assign(so);
            try {
              return assign(s);
            } catch (ParseException &e) {
              throw isPartial ? partial : e;
            }
          }
        }
      }
    }
  }
}

/** X :- <X> name ';' */
TNode unary_op(State &s, std::string op, stmt_type typ, bool nest_variable = false) {
  int init = s.i;
  bool partial = false;
  try {
    string_match(s, op);
    partial = true;
    // :- '<op>'
    whitespace_char(s);
    // compulsory whitespace
    whitespace(s);
    int var_i = s.i;
    std::string n = name(s);
    // :- name
    whitespace(s);
    char_match(s, ';');
    // :- ';'
    whitespace(s);
    if (nest_variable) {
      TNode stmtNode = TNode(s.adv_cur_stmt_num(), "", typ, init);
      TNode varNode = TNode(n, VARIABLE, var_i);
      stmtNode.addChild(varNode);
      return stmtNode;
    } else {
      return TNode(s.adv_cur_stmt_num(), n, typ, init);
    }
  } catch (ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(init, s.i, op, partial ? "partial" : "");
  }
}

/** read :- 'read' name ';' */
TNode read(State &s) { return unary_op(s, "read", READ, true); }

/** print :- 'print' name ';' */
TNode print(State &s) { return unary_op(s, "print", PRINT, true); }

/** call :- 'call' name ';' */
TNode call(State &s) { return unary_op(s, "call", CALL); }

/** while_stmt :- 'while' '(' cond_expr ')' '{' stmtLst '}' */
TNode while_stmt(State &s) {
  int init = s.i;
  int initNum = s.cur_stmt_num;
  TNode t(s.adv_cur_stmt_num(), "", WHILE, init);
  bool stmtLstFail = false;
  try {
    string_match(s, "while");
    whitespace(s);
    char_match(s, '(');
    whitespace(s);
    TNode t1 = cond_expr(s);
    char_match(s, ')');
    whitespace(s);
    char_match(s, '{');
    whitespace(s);
    stmtLstFail = true;
    TNode t2 = stmtLst(s);
    stmtLstFail = false;
    t.addChild(t1);
    t.addChild(t2);
    return t;
  } catch (ParseException &e) {
    s.excps.push_back(e);
    s.cur_stmt_num = initNum;
    throw ParseException(init, s.i, "while_stmt", stmtLstFail ? "body" : "");
  }
}

/** if_stmt :- 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst
 * '}' */
TNode if_stmt(State &s) {
  int init = s.i;
  int initNum = s.cur_stmt_num;
  TNode t(s.adv_cur_stmt_num(), "", IF, init);
  bool stmtLstFail = false;
  try {
    string_match(s, "if");
    whitespace(s);
    char_match(s, '(');
    whitespace(s);
    TNode t1 = cond_expr(s);
    char_match(s, ')');
    whitespace(s);
    string_match(s, "then");
    whitespace(s);
    char_match(s, '{');
    whitespace(s);
    stmtLstFail = true;
    TNode t2 = stmtLst(s);
    stmtLstFail = false;
    string_match(s, "else");
    whitespace(s);
    char_match(s, '{');
    whitespace(s);
    stmtLstFail = true;
    TNode t3 = stmtLst(s);
    stmtLstFail = false;
    t.addChild(t1);
    t.addChild(t2);
    t.addChild(t3);
    return t;
  } catch (ParseException &e) {
    s.excps.push_back(e);
    s.cur_stmt_num = initNum;
    throw ParseException(init, s.i, "if_stmt", stmtLstFail ? "body" : "");
  }
}

TNode assign(State &s) {
  int init = s.i;
  int initNum = s.cur_stmt_num;
  try {
    TNode t1 = variable(s);
    whitespace(s);
    char_match(s, '=');
    whitespace(s);
    TNode t2 = expr(s);
    char_match(s, ';');
    TNode t(s.adv_cur_stmt_num(), "", ASSIGN, init);
    t.addChild(t1);
    t.addChild(t2);
    return t;
  } catch (ParseException &e) {
    s.excps.push_back(e);
    s.cur_stmt_num = initNum;
    throw ParseException(init, s.i, "assign", "");
  }
}

/** cond_expr :- rel_expr | '!' '(' cond_expr ')' | '(' cond_expr ')'
 * ('&&'|'||') '(' cond_expr ')' */
TNode cond_expr(State &s) {
  State so(s);
  try {
    return rel_expr(s);
  } catch (ParseException &e) {
    s.assign(so);
    try {
      char_match(s, '!');
      whitespace(s);
      char_match(s, '(');
      whitespace(s);
      TNode t1 = cond_expr(s);
      char_match(s, ')');
      whitespace(s);
      TNode t("!", EXPR, so.i);
      t.addChild(t1);
      return t;
    } catch (ParseException &e) {
      s.assign(so);
      try {
        s.assign(so);
        char_match(s, '(');
        whitespace(s);
        TNode t1 = cond_expr(s);
        char_match(s, ')');
        whitespace(s);
        State so1(s);
        std::string op;
        try {
          op = string_match(s, "&&");
        } catch (ParseException &e) {
          try {
            op = string_match(s, "||");
          } catch (ParseException &e) {
            s.excps.push_back(e);
            throw ParseException(so1.i, s.i, "cond_expr", "bin_op");
          }
        }
        whitespace(s);
        char_match(s, '(');
        whitespace(s);
        TNode t2 = cond_expr(s);
        char_match(s, ')');
        whitespace(s);
        TNode t(op, EXPR, so.i);
        t.addChild(t1);
        t.addChild(t2);
        return t;
      } catch (ParseException &e) {
        s.excps.push_back(e);
        throw ParseException(so.i, s.i, "cond_expr", "root");
      }
    }
  }
}

/** rel_expr :- expr ('>'|'>='|'<'|'<='|'=='|'!=') expr */
TNode rel_expr(State &s) {
  State so(s);
  int init = so.i;
  TNode lchild;
  try {
    lchild = rel_factor(s);
    whitespace(s);
  } catch (ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(so.i, s.i, "rel_expr", "first_factor");
  }
  std::string op;
  so.assign(s);
  try {
    op = string_match(s, ">=");
  } catch (ParseException &e) {
    s.assign(so);
    try {
      op = char_match(s, '>');
    } catch (ParseException &e) {
      s.assign(so);
      try {
        op = string_match(s, "<=");
      } catch (ParseException &e) {
        s.assign(so);
        try {
          op = char_match(s, '<');
        } catch (ParseException &e) {
          s.assign(so);
          try {
            op = string_match(s, "==");
          } catch (ParseException &e) {
            s.assign(so);
            try {
              op = string_match(s, "!=");
            } catch (ParseException &e) {
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
    TNode t(op, EXPR, init);
    t.addChild(lchild);
    t.addChild(rchild);
    return t;
  } catch (ParseException &e) {
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
  } catch (ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(init, s.i, "expr", "");
  }
}

/** expr_1 :- ('+'|'-') term expr_1 | e */
TNode expr_1(State &s, TNode &lchild) {
  State so(s);
  std::string op;
  try {
    op = char_match(s, '+');
  } catch (ParseException &e) {
    s.assign(so);
    try {
      op = char_match(s, '-');
    } catch (ParseException &e) {
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
    TNode t(op, EXPR, so.i);
    t.addChild(lchild);
    t.addChild(rchild);
    return expr_1(s, t);
    // :- expr_1
  } catch (ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(so.i, s.i, "expr", op);
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
  } catch (ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(init, s.i, "term", "");
  }
}

/** term_1 :- ('*'|'/'|'%') factor term_1 | e */
TNode term_1(State &s, TNode &lchild) {
  State so(s);
  std::string op;
  try {
    op = char_match(s, '*');
  } catch (ParseException &e) {
    s.assign(so);
    try {
      op = char_match(s, '/');
    } catch (ParseException &e) {
      s.assign(so);
      try {
        op = char_match(s, '%');
      } catch (ParseException &e) {
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
    TNode t(op, EXPR, so.i);
    t.addChild(lchild);
    t.addChild(rchild);
    return term_1(s, t);
    // :- term_1
  } catch (ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(so.i, s.i, "term", op);
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
        char_match(s, '(');
        whitespace(s);
        TNode t = expr(s);
        whitespace(s);
        char_match(s, ')');
        return t;
      } catch (ParseException &e) {
        throw ParseException(so.i, s.i, "factor", "");
      }
    }
  }
}

/** variable :- name */
TNode variable(State &s) {
  int init = s.i;
  try {
    std::string v = name(s);
    return TNode(v, VARIABLE, init);
  } catch (ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(init, s.i, "variable", "");
  }
}

/** constant :- integer */
TNode constant(State &s) {
  int init = s.i;
  try {
    std::string v = integer(s);
    return TNode(v, CONSTANT, init);
  } catch (ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(init, s.i, "constant", "");
  }
}


void validate_unique_procedure_names(TNode &root, std::unordered_set<std::string> &procs) {
  switch(root.getType()) {
    case PROGRAM:
      for(int i = 0; i < root.getChildren().size(); i++) {
        validate_unique_procedure_names(root.getChildren()[i], procs);
      }
      break;
    case PROCEDURE:
      std::string procName = root.getValue();
      if(procs.find(procName) != procs.end()) {
        throw root.getPos();
      } else {
        procs.insert(procName);
      }
      break;
  }
}

void validate_call_procedure_exists(TNode &root, std::unordered_set<std::string> &procs) {
  if(root.getType() == PROGRAM || root.getType() == STATEMENTLIST) {
    for(int i = 0; i < root.getChildren().size(); i++) {
      validate_call_procedure_exists(root.getChildren()[i], procs);
    }
  } else if(root.getType() == PROCEDURE) {
    validate_call_procedure_exists(root.getChildren()[0], procs);
  } else if(root.getType() == CALL) {
    if(procs.find(root.getValue()) == procs.end()) {
      throw root.getPos();
    }
  } else if(root.getType() == WHILE) {
    validate_call_procedure_exists(root.getChildren()[1], procs);
  } else if(root.getType() == IF) {
    validate_call_procedure_exists(root.getChildren()[1], procs);
    validate_call_procedure_exists(root.getChildren()[2], procs);
  }
}