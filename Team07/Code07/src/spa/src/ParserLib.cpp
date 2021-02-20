#include "ParserLib.h"
#include <cctype>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>
#include <cctype>
#include "ParserLib.h"
#include "ParserIndexMapper.h"

// class definitions ----------------------------------------------------------

State::State(std::string *str) {
  i = 0;
  source = str;
  curStmtNum = 1;
  excps.clear();
}
State::State(State &s) {
  i = s.i;
  source = s.source;
  curStmtNum = s.curStmtNum;
  excps = s.excps;
}
int State::advCurStmtNum() { return curStmtNum++; }
std::string State::toString() { return "{ i: " + std::to_string(i) + " }"; }
void State::assign(State &s) {
  i = s.i;
  source = s.source;
  curStmtNum = s.curStmtNum;
  excps = s.excps;
}

ParseException::ParseException(int f, int a, std::string s, std::string ar) {
  from = f;
  at = a;
  subparser = s;
  args = ar;
}

std::string ParseException::errorMessage() {
  return subparser + "(" + args + ") : ";
}
const char *ParseException::what() const throw() {
  static std::string str("ParseException : " + subparser);
  return str.c_str();
}

std::string prettyPrintValidation(ParserMapper &map, int pos, std::string msg) {
	std::string outputstr("ValidationError");
	outputstr += map.get_pos_print(pos) + "\n";
	outputstr += map.get_line_caret(pos, 2);
	outputstr += "  " + msg + "\n";
	return outputstr;
}

std::string prettyPrintException(ParserMapper &map, State &s, bool show_stack) {
	std::vector<ParseException> e = s.excps;
	int pos = e[0].at;

	std::string outputstr("ParseError");
	outputstr += map.get_pos_print(pos) + "\n";
	outputstr += map.get_line_caret(pos, 2);
	if(show_stack) {
		std::vector<int> fs;
		std::vector<std::string> es;
		for(int i = 0; i < e.size(); i++) {
			outputstr += "  " + e[i].errorMessage() + "\n";
			outputstr += map.get_pos_print(e[i].from, 4);
		}
  }
  return outputstr;
}

// combinators ----------------------------------------------------------------

std::string stringMatch(State &s, std::string str) {
  int init = s.i;
  try {
    for (int j = 0; j < str.size(); j++) {
      if ((*s.source).at(s.i + j) != str.at(j)) {
        s.i += j;
        throw ParseException(init, s.i, "stringMatch", str);
      }
    }
    s.i += str.size();
    return str;
  } catch (std::out_of_range &e) {
    s.i = (*s.source).size();
    throw ParseException(init, s.i, "stringMatch", str);
  }
}

char charPredicate(State &s, bool (*pred)(char), std::string errorName) {
  try {
    char c = (*s.source).at(s.i);
    if (pred(c)) {
      s.i += 1;
    } else {
      throw ParseException(s.i, s.i, "charPredicate", errorName);
    }
    return c;
  } catch (std::out_of_range &e) {
    s.i = (*s.source).size();
    throw ParseException(s.i, s.i, "charPredicate", errorName);
  }
}

std::string stringPredicate(State &s, bool (*pred)(char),
                            std::string errorName) {
  std::vector<char> cs;
  State so(s);
  while (true) {
    try {
      cs.push_back(charPredicate(s, pred, errorName));
      so.assign(s);
    } catch (ParseException &e) {
      s.assign(so);
      break;
    }
  }
  std::string str(cs.begin(), cs.end());
  return str;
}

std::string whitespace(State &s) {
  return stringPredicate(s, &whitespacePred, "whitespace");
}

std::string upper(State &s) { return stringPredicate(s, &upperPred, "upper"); }

std::string lower(State &s) { return stringPredicate(s, &lowerPred, "lower"); }

std::string alpha(State &s) { return stringPredicate(s, &alphaPred, "alpha"); }

std::string digit(State &s) { return stringPredicate(s, &digitPred, "digit"); }

std::string alphaNum(State &s) {
  return stringPredicate(s, &alphanumPred, "alphaNum");
}

std::string double_quotes(State &s) {
  return stringPredicate(s, &double_quotes_pred, "double_quotes");
}

std::string wildcard(State &s) {
  return stringPredicate(s, &wildcard_pred, "wildcard");
}

// predicates -----------------------------------------------------------------

bool whitespacePred(char c) { return std::isspace(c); }

bool upperPred(char c) { return std::isupper(c); }

bool lowerPred(char c) { return std::islower(c); }

bool alphaPred(char c) { return std::isalpha(c); }

bool digitPred(char c) { return std::isdigit(c); }

bool alphanumPred(char c) { return std::isalnum(c); }

bool double_quotes_pred(char c) { return c == '\"'; }

bool wildcard_pred(char c) { return c == '_'; }

// Low level abstractions
// -----------------------------------------------------------------
// NAME: LETTER (LETTER | DIGIT)*
std::string name(State &s) {
  int init = s.i;
  try {
    char r1 = charPredicate(s, &alphaPred, "letter");
    // :- letter
    std::string r2 = alphaNum(s);
    // :- (letter | digit)*
    r2.insert(r2.begin(), r1);
    return r2;
  } catch (ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(init, s.i, "name", "");
  }
}

// IDENT: LETTER (LETTER | DIGIT)*
std::string ident(State &state) {
  int init = state.i;
  try {
    char r1 = charPredicate(state, &alphaPred, "letter"); // letter
    std::string r2 = alphaNum(state);                     // (letter | digit)*
    r2.insert(r2.begin(), r1);
    return r2;
  } catch (ParseException &e) {
    state.excps.push_back(e);
    throw ParseException(init, state.i, "ident", "");
  }
}

/** integer :- digit digit* */
std::string integer(State &s) {
  int init = s.i;
  try {
    char r1 = charPredicate(s, &digitPred, "digit");
    // :- digit
    std::string r2 = digit(s);
    // :- digit*
    r2.insert(r2.begin(), r1);
    return r2;
  } catch (ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(init, s.i, "integer", "");
  }
}

// synonym: IDENT
std::string synonym(State &state) { return ident(state); }
