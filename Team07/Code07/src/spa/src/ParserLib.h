#pragma once
#include<exception>
#include<string>
using namespace std;

class State {
	public:
    /** current index in source to consume by parser */
		int i;
    /** source string to be parsed */
    string source;
		State(string str);
		State(State &s);
		string toString();
};

class ParseException: public exception {
	public:
    /** state.i when entering parser */
		int from;
    /** state.i value when exception occured */
		int at;
    /** parser name */
		std::string subparser;
    /** arguments to parser configuration */
		std::string args;
    /** compiled error message by ParseException */
		std::string errmsg;
    ParseException *childException;
		ParseException(int f, int a, string s, string ar);
    ParseException(int f, int a, string s, string ar, ParseException *c);
    void init(int f, int a, string s, string ar, ParseException *c);
    string prettyPrint();
	virtual const char* what() const throw()
	{
	  return errmsg.c_str();
	}
};


string stringMatch(State &s, string str);
char charPredicate(State &s, bool (*pred)(char), string errorName);
string stringPredicate(State &s, bool (*pred)(char), string errorName);
bool whitespacePred(char c);
string whitespace(State &s);
bool upperPred(char c);
string upper(State &s);
bool lowerPred(char c);
string lower(State &s);
bool alphaPred(char c);
string alpha(State &s);
bool digitPred(char c);
string digit(State &s);
bool alphanumPred(char c);
string alphaNum(State &s);