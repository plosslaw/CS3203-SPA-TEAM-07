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
  cur_stmt_num = 1;
  excps.clear();
}
State::State(State &s) {
  i = s.i;
  source = s.source;
  cur_stmt_num = s.cur_stmt_num;
  excps = s.excps;
}
int State::adv_cur_stmt_num() { return cur_stmt_num++; }
std::string State::to_string() { return "{ i: " + std::to_string(i) + " }"; }
void State::assign(State &s) {
  i = s.i;
  source = s.source;
  cur_stmt_num = s.cur_stmt_num;
  excps = s.excps;
}

ParseException::ParseException(int f, int a, std::string s, std::string ar) {
  from = f;
  at = a;
  subparser = s;
  args = ar;
}

std::string ParseException::error_message() {
  return subparser + "(" + args + ") : ";
}
const char *ParseException::what() const throw() {
  static std::string str("ParseException : " + subparser);
  return str.c_str();
}

std::string pretty_print_validation(std::string msg) {
	return "ValidationError " + msg + "\n";
}

std::string pretty_print_validation(ParserMapper &map, int pos, std::string msg) {
	std::string outputstr("ValidationError ");
	outputstr += map.get_pos_print(pos) + "\n";
	outputstr += map.get_line_caret(pos, 2);
	outputstr += "  " + msg + "\n";
	return outputstr;
}

std::string pretty_print_exception(ParserMapper &map, State &s, bool show_stack) {
	std::vector<ParseException> e = s.excps;
	int pos = e[0].at;

	std::string outputstr("ParseError ");
	outputstr += map.get_pos_print(pos) + "\n";
	outputstr += map.get_line_caret(pos, 2);
	if(show_stack) {
		std::vector<int> fs;
		std::vector<std::string> es;
		for(int i = 0; i < e.size(); i++) {
			outputstr += "  " + e[i].error_message() + "\n";
			outputstr += map.get_pos_print(e[i].from, 4);
		}
  }
  return outputstr;
}

// combinators ----------------------------------------------------------------

std::string string_match(State &s, std::string str) {
  int init = s.i;
  try {
    for (int j = 0; j < str.size(); j++) {
      if ((*s.source).at(s.i + j) != str.at(j)) {
        s.i += j;
        throw ParseException(init, s.i, "string_match", str);
      }
    }
    s.i += str.size();
    return str;
  } catch (std::out_of_range &e) {
    s.i = (*s.source).size();
    throw ParseException(init, s.i, "string_match", str);
  }
}

char char_predicate(State &s, bool (*pred)(char), std::string error_name) {
  try {
    char c = (*s.source).at(s.i);
    if (pred(c)) {
      s.i += 1;
    } else {
      throw ParseException(s.i, s.i, "char_predicate", error_name);
    }
    return c;
  } catch (std::out_of_range &e) {
    s.i = (*s.source).size();
    throw ParseException(s.i, s.i, "char_predicate", error_name);
  }
}

std::string string_predicate(State &s, bool (*pred)(char),
                            std::string error_name) {
  std::vector<char> cs;
  State so(s);
  while (true) {
    try {
      cs.push_back(char_predicate(s, pred, error_name));
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
  return string_predicate(s, &whitespace_pred, "whitespace");
}

std::string upper(State &s) { return string_predicate(s, &upper_pred, "upper"); }

std::string lower(State &s) { return string_predicate(s, &lower_pred, "lower"); }

std::string alpha(State &s) { return string_predicate(s, &alpha_pred, "alpha"); }

std::string digit(State &s) { return string_predicate(s, &digit_pred, "digit"); }

std::string alpha_num(State &s) {
  return string_predicate(s, &alpha_num_pred, "alpha_num");
}

std::string double_quotes(State &s) {
  return string_predicate(s, &double_quotes_pred, "double_quotes");
}

std::string wildcard(State &s) {
  return string_predicate(s, &wildcard_pred, "wildcard");
}

// predicates -----------------------------------------------------------------

bool whitespace_pred(char c) { return std::isspace(c); }

bool upper_pred(char c) { return std::isupper(c); }

bool lower_pred(char c) { return std::islower(c); }

bool alpha_pred(char c) { return std::isalpha(c); }

bool digit_pred(char c) { return std::isdigit(c); }

bool alpha_num_pred(char c) { return std::isalnum(c); }

bool double_quotes_pred(char c) { return c == '\"'; }

bool wildcard_pred(char c) { return c == '_'; }

// Low level abstractions
// -----------------------------------------------------------------
// NAME: LETTER (LETTER | DIGIT)*
std::string name(State &s) {
  int init = s.i;
  try {
    char r1 = char_predicate(s, &alpha_pred, "letter");
    // :- letter
    std::string r2 = alpha_num(s);
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
    char r1 = char_predicate(state, &alpha_pred, "letter"); // letter
    std::string r2 = alpha_num(state);                     // (letter | digit)*
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
    char r1 = char_predicate(s, &digit_pred, "digit");
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
