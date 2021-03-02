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
  std::string error_message();
  const char *what() const throw() override;
};

class State {
public:
  /** current index in source to consume by parser */
  int i;
  /** source string to be parsed */
  std::string *source;
  /** current statement num index */
  int cur_stmt_num;
  /** exception stack */
  std::vector<ParseException> excps;
  State(std::string *str);
  State(State &s);
  std::string to_string();
  int adv_cur_stmt_num();
  void assign(State &s);
};

std::string pretty_print_validation(std::string msg);
std::string pretty_print_validation(ParserMapper &map, int pos, std::string msg);
std::string pretty_print_exception(ParserMapper &map, State &s, bool show_stack = true);

char char_match(State &s, char c);
std::string string_match(State &s, std::string str);
char char_predicate(State &s, bool (*pred)(char), std::string error_name);
std::string string_predicate(State &s, bool (*pred)(char),
                            std::string error_name);
bool whitespace_pred(char c);
std::string whitespace(State &s);
char whitespace_char(State &s);
bool upper_pred(char c);
std::string upper(State &s);
bool lower_pred(char c);
std::string lower(State &s);
bool alpha_pred(char c);
std::string alpha(State &s);
bool digit_pred(char c);
std::string digit(State &s);
bool alpha_num_pred(char c);
std::string alpha_num(State &s);

std::string name(State &s);
std::string ident(State &s);
std::string integer(State &s);
std::string synonym(State &s);
