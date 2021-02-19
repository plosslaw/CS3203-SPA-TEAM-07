#include "ParserLib.h"
#include <cctype>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

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

std::string prettyPrintValidation(std::string *str, int pos, std::string msg) {
  std::string strv = *str;
  std::vector<char> linebuilder;
  bool atLine = false;
  int lineNum = 0;
  int colBegin = -1;
  for (int i = 0; i < strv.length(); i++) {
    char c = strv.at(i);
    if (i == pos) {
      atLine = true;
    }
    if (c == '\n') {
      if (atLine) {
        break;
      } else {
        lineNum++;
        colBegin = i;
        linebuilder.clear();
      }
    } else {
      linebuilder.push_back(c);
    }
  }
  std::string linestrvalue(linebuilder.begin(), linebuilder.end());
  int colNum = pos - colBegin;
  // calculate line and col from pos

  std::string outputstr("ValidationError");
  outputstr += " at line " + std::to_string(lineNum + 1) + " col " +
               std::to_string(colNum) + "\n\n";
  std::string preoutput = "  " + std::to_string(lineNum + 1) + "|  ";
  outputstr += preoutput + linestrvalue + "\n";
  std::string whitespaceprefix =
      std::string(preoutput.length() + colNum - 1, ' ');
  outputstr += whitespaceprefix + "^\n";
  outputstr += "  " + msg + "\n";
  return outputstr;
  // pretty print
}

std::string prettyPrintException(State &s, bool show_stack) {
  std::string *str = s.source;
  std::vector<ParseException> e = s.excps;
  int at = e[0].at;

  std::vector<int> fs;
  std::vector<std::string> es;
  for (int i = 0; i < e.size(); i++) {
    fs.push_back(e[i].from);
    es.push_back(e[i].errorMessage());
  }
  std::vector<int> fromLine(fs.size());
  std::vector<int> fromCol(fs.size());

  std::vector<int> linelengths;
  std::vector<std::string> linestr;
  std::vector<char> linebuilder;

  int curLineLength = 0;
  int prevLineLength = 0;
  int curLine = 0;
  int atLine = 0;
  int atCol = 0;

  std::string strv = *str;
  for (int i = 0; i < strv.length(); i++) {
    char c = strv.at(i);
    curLineLength++;
    if (c == '\n') {
      linelengths.push_back(curLineLength);
      std::string linestrvalue(linebuilder.begin(), linebuilder.end());
      linestr.push_back(linestrvalue);
      linebuilder.clear();
      curLine++;
      prevLineLength = curLineLength;
    } else {
      linebuilder.push_back(c);
    }
    if (i == at) {
      atLine = curLine;
      atCol = curLineLength - prevLineLength - 1;
    }
    for (int j = 0; j < fs.size(); j++) {
      if (i == fs[j]) {
        fromLine[j] = curLine;
        fromCol[j] = curLineLength - prevLineLength - 1;
      }
    }
  }
  if (at == strv.size()) {
    atLine = curLine;
    atCol = strv.size() - prevLineLength - 1;
  }
  for (int i = 0; i < fs.size(); i++) {
    if (fs[i] == strv.size()) {
      fromLine[i] = curLine;
      fromCol[i] = strv.size() - prevLineLength - 1;
    }
  }
  linelengths.push_back(curLineLength);
  std::string linestrvalue(linebuilder.begin(), linebuilder.end());
  linestr.push_back(linestrvalue);
  // calculate from line cols and line strings

  std::string outputstr("ParseError");
  outputstr += " at line " + std::to_string(atLine + 1) + " col " +
               std::to_string(atCol + 1) + "\n\n";
  std::string preoutput = "  " + std::to_string(atLine + 1) + "|  ";
  outputstr += preoutput + linestr[atLine] + "\n";
  std::string whitespaceprefix = std::string(preoutput.length() + atCol, ' ');
  outputstr += whitespaceprefix + "^\n";
  if (show_stack) {
    for (int i = 0; i < fromLine.size(); i++) {
      outputstr += "  " + es[i] + "\n";
      outputstr += "    from line " + std::to_string(fromLine[i] + 1) +
                   " col " + std::to_string(fromCol[i] + 1) + "\n";
    }
  }
  // generate pretty output
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
