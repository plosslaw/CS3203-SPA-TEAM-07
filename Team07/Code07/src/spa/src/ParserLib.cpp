#include <string>
#include <vector>
#include <cctype>
#include "ParserLib.h"

using namespace std;
// class definitions ----------------------------------------------------------

State::State(string str) {
	i = 0;
	source = str;
}
State::State(State &s) {
	i = s.i;
	source = s.source;
}
string State::toString() {
	return "{ i: " + to_string(i) + " }";
}

void ParseException::init(int f, int a, string s, string ar, ParseException *c) {
	from = f;
	at = a;
	subparser = s;
	args = ar;
	childException = c;
	errmsg = subparser + "(" + args + ") : from " + to_string(from) + " at " + to_string(at);
}

ParseException::ParseException(int f, int a, string s, string ar) {
	init(f,a,s,ar,nullptr);
}

ParseException::ParseException(int f, int a, string s, string ar, ParseException *c) {
	init(f,a,s,ar,c);
}
string ParseException::prettyPrint() {
  // for each f and a
  // identify line number
  // identify column number
  
  // print whole lines of line from f and a
  // underline from (line,column) f to a

  // concat what(), recursively for each childException
  return "";
}

// combinators ----------------------------------------------------------------

string stringMatch(State &s, string str) {
	int i = s.i;
	for (int j = 0; j < str.size(); j++) {
		if (s.source.at(i + j) != str.at(j)) {
			throw ParseException(i, i+j, "stringParse", str);
		}
	}
	s.i += str.size();
	return str;
}

char charPredicate(State &s, bool (*pred)(char), string errorName) {
	char c = s.source.at(s.i);
	if (pred(c)) {
		s.i += 1;
	} else {
		throw ParseException(s.i, s.i, "charPredicate", errorName);
	}
	return c;
}

string stringPredicate(State &s, bool (*pred)(char), string errorName) {
	vector<char> cs;
	while(true) {
		try {
			cs.push_back(charPredicate(s, pred, errorName));
		} catch(ParseException e) {
			break;
		}
	}
	string str(cs.begin(), cs.end());
	return str;
}

string whitespace(State &s) {
	return stringPredicate(s, &whitespacePred, "whitespace");
}

string upper(State &s) {
	return stringPredicate(s, &upperPred, "whitespace");
}

string lower(State &s) {
	return stringPredicate(s, &lowerPred, "whitespace");
}

string alpha(State &s) {
	return stringPredicate(s, &alphaPred, "whitespace");
}

string digit(State &s) {
	return stringPredicate(s, &digitPred, "whitespace");
}

string alphanum(State &s) {
	return stringPredicate(s, &alphanumPred, "whitespace");
}

// predicates -----------------------------------------------------------------

bool whitespacePred(char c) {
	return std::isspace(c);	
}

bool upperPred(char c) {
	return std::isupper(c);
}

bool lowerPred(char c) {
	return std::islower(c);
}

bool alphaPred(char c) {
	return std::isalpha(c);
}

bool digitPred(char c) {
	return std::isdigit(c);
}

bool alphanumPred(char c) {
	return std::isalnum(c);
}