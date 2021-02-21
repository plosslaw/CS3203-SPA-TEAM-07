#pragma once
#include<exception>
#include<string>
#include<vector>
#include"ParserIndexMapper.h"

class ParseException : public std::exception {
public:
  /** state.i when entering parser */
  int from;
  /** state.i value when exception occured */
  int at;
  /** parser name */
  std::string subparser;
  /** arguments to parser configuration */
  std::string args;
  ParseException(int f, int a, std::string s, std::string ar);
  std::string errorMessage();
  const char *what() const throw() override;
};

class State {
public:
  /** current index in source to consume by parser */
  int i;
  /** source string to be parsed */
  std::string *source;
  /** current statement num index */
  int curStmtNum;
  /** exception stack */
  std::vector<ParseException> excps;
  State(std::string *str);
  State(State &s);
  std::string toString();
  int advCurStmtNum();
  void assign(State &s);
};

std::string prettyPrintValidation(std::string msg);
std::string prettyPrintValidation(ParserMapper &map, int pos, std::string msg);
std::string prettyPrintException(ParserMapper &map, State &s, bool show_stack = true);

std::string stringMatch(State &s, std::string str);
char charPredicate(State &s, bool (*pred)(char), std::string errorName);
std::string stringPredicate(State &s, bool (*pred)(char),
                            std::string errorName);
bool whitespacePred(char c);
std::string whitespace(State &s);
bool upperPred(char c);
std::string upper(State &s);
bool lowerPred(char c);
std::string lower(State &s);
bool alphaPred(char c);
std::string alpha(State &s);
bool digitPred(char c);
std::string digit(State &s);
bool alphanumPred(char c);
std::string alphaNum(State &s);
bool double_quotes_pred(char c);
std::string double_quotes(State &s);
bool wildcard_pred(char c);
std::string wildcard(State &s);

std::string name(State &s);
std::string ident(State &s);
std::string integer(State &s);
std::string synonym(State &s);
