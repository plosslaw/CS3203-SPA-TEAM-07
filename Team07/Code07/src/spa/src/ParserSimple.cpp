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

/** read :- 'read' name ';' */
TNode read(State &s) {
  int init = s.i;
  try {
    stringMatch(s, "read");
    // :- 'read'
    whitespace(s);
    string n = name(s);
    // :- name
    whitespace(s);
    stringMatch(s, ";");
    // :- ';'
    whitespace(s);
    return TNode(s.advCurStmtNum(), n, READ);
  } catch(ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(init, s.i, "read", "");
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