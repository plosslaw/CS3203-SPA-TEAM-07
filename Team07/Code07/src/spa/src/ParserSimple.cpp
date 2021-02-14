#include <string>
#include <vector>
#include "ParserLib.h"

using namespace std;

/** name :- letter (letter | digit)* */
string name(State &s) {
  int init = s.i;
  try {
    char r1 = charPredicate(s, &alphaPred, "letter");
    // :- letter
    string r2 = alphaNum(s);
    // :- (letter | digit)*
    return to_string(r1) + r2;
  } catch (ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(init, s.i, "name", "");
  }
}

/** integer :- digit digit* */
int integer(State &s) {
  int init = s.i;
  try {
    char r1 = charPredicate(s, &digitPred, "digit");
    // :- digit
    string r2 = digit(s);
    // :- digit*
    return stoi(to_string(r1) + r2);
  } catch (ParseException &e) {
    s.excps.push_back(e);
    throw ParseException(init, s.i, "integer", "");
  }
}