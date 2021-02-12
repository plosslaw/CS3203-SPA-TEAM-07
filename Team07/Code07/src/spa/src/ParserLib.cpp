#include <limits>
#include <string>
#include <vector>
#include <cctype>
#include "ParserLib.h"
#include <iostream>

using namespace std;
// class definitions ----------------------------------------------------------

State::State(string *str) {
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

ParseException::ParseException(int f, int a, string s, string ar, string *source) {
	from = f;
	at = a;
	subparser = s;
	args = ar;
	childException = nullptr;
	str = source;
}

ParseException::ParseException(int f, int a, string s, string ar, string *source, ParseException *c) {
	from = f;
	at = a;
	subparser = s;
	args = ar;
	childException = c;
	str = source;
}

string ParseException::errorMessage() {
	return subparser + "(" + args + ") : ";
}
const char* ParseException::what() const throw()
{
	static string str("ParseException : " + subparser);
	return str.c_str();
}

string ParseException::prettyPrint() {
	vector<int> fs;
	vector<string> es;
	froms(fs, es);
	vector<int> fromLine(fs.size());
	vector<int> fromCol(fs.size());

	vector<int> linelengths;
	vector<string> linestr;
	vector<char> linebuilder;
	
	int curLineLength = 0;
	int curLine = 0;
	int atLine = 0;
	int atCol = 0;

	string strv = *str;
	for(int i = 0 ; i < strv.length(); i++) {
		char c = strv.at(i);
		curLineLength++;
		if(c == '\n') {
			linelengths.push_back(curLineLength);
			string linestrvalue(linebuilder.begin(), linebuilder.end());
			linestr.push_back(linestrvalue);
			linebuilder.clear();
			curLine++;
		} else {
			linebuilder.push_back(c);
		}
		if(i == at) {
			atLine = curLine;
		}
		for(int j = 0; j < fs.size(); j++) {
			if(i == fs[j]) {
				fromLine[j] = curLine;
			}
		}
	}
	linelengths.push_back(curLineLength);
	string linestrvalue(linebuilder.begin(), linebuilder.end());
	linestr.push_back(linestrvalue);
	
	atCol = linelengths.at(atLine) - at;
	for(int j = 0; j < fs.size(); j++) {
		int prevline = fromLine[j] - 1;
		if(prevline == -1) {
			fromCol[j] = fs[j];
		} else {
			fromCol[j] = linelengths[prevline] - fs[j];
		}
	}
	// calculate from line cols and line strings

	string outputstr = "";
	outputstr += "ParseException\n\n";
	outputstr += "  " + to_string(atLine + 1) + "|  " + linestr[atLine] + "\n\n";
	for(int i = 0; i < fromLine.size(); i++) {
		outputstr += "  " + es[i] + "\n";
		outputstr += "    from line " + to_string(fromLine[i] + 1) + " col " + to_string(fromCol[i] + 1) + "\n";
	}
	// generate pretty output
  return outputstr;
}

// combinators ----------------------------------------------------------------

string stringMatch(State &s, string str) {
	for (int j = 0; j < str.size(); j++) {
		if ((*s.source).at(s.i + j) != str.at(j)) {
			throw ParseException(s.i, s.i+j, "stringParse", str, s.source);
		}
	}
	s.i += str.size();
	return str;
}

char charPredicate(State &s, bool (*pred)(char), string errorName) {
	char c = (*s.source).at(s.i);
	if (pred(c)) {
		s.i += 1;
	} else {
		throw ParseException(s.i, s.i, "charPredicate", errorName, s.source);
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