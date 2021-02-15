#include "ParserPql.h"
#include "ParserLib.h"
#include <iostream>
#include <vector>

std::string ident(State &state) {
  int init = state.i;
  try {
    char r1 = charPredicate(state, &alphaPred, "letter");
    // :- letter
    std::string r2 = alphaNum(state);
    // :- (letter | digit)*
    r2.insert(r2.begin(), r1);
    return r2;
  } catch (ParseException &e) {
    state.excps.push_back(e);
    throw ParseException(init, state.i, "ident", "");
  }
}

PayLoad declaration(State &state, std::string design_entity, Single load_type) {
  State so(state);
  try {
    stringMatch(state, design_entity);
    whitespace(state);
    std::string synonym = ident(state);
    return PayLoad(SINGLE, load_type, synonym);
  } catch (ParseException &e) {
    state.excps.push_back(e);
    throw ParseException(so.i, state.i, "declaration", "");
  }
}

PayLoad stmt_decl(State &state) {
  return declaration(state, "stmt", STATEMENT);
}
PayLoad read_decl(State &state) { return declaration(state, "read", READ); }
PayLoad print_decl(State &state) { return declaration(state, "print", PRINT); }
PayLoad call_decl(State &state) { return declaration(state, "call", CALL); }
PayLoad while_decl(State &state) { return declaration(state, "while", WHILE); }
PayLoad if_decl(State &state) { return declaration(state, "if", IF); }
PayLoad assign_decl(State &state) {
  return declaration(state, "assign", ASSIGN);
}
PayLoad variable_decl(State &state) {
  return declaration(state, "variable", VARIABLE);
}
PayLoad constant_decl(State &state) {
  return declaration(state, "constant", CONSTANT);
}
PayLoad procedure_decl(State &state) {
  return declaration(state, "procedure", PROCEDURE);
}

std::vector<PayLoad> declaration_cl(State &state) {
  // declaration : design-entity synonym (‘,’ synonym)* ‘;’
  // design-entity : ‘stmt’ | ‘read’ | ‘print’ | ‘call’ | ‘while’ | ‘if’ |
  // ‘assign’ | ‘variable’ | ‘constant’ | ‘procedure’
  State so(state);
  std::vector<PayLoad> declarations;
  try {
    PayLoad decl_payload = stmt_decl(state);
    declarations.push_back(decl_payload);
    stringMatch(state, ";");
  } catch (ParseException &e) {
    try {
      PayLoad decl_payload = read_decl(state);
      declarations.push_back(decl_payload);
      stringMatch(state, ";");
    } catch (ParseException &e) {
      try {
        PayLoad decl_payload = print_decl(state);
        declarations.push_back(decl_payload);
        stringMatch(state, ";");
      } catch (ParseException &e) {
        try {
          PayLoad decl_payload = call_decl(state);
          declarations.push_back(decl_payload);
          stringMatch(state, ";");
        } catch (ParseException &e) {
          try {
            PayLoad decl_payload = while_decl(state);
            declarations.push_back(decl_payload);
            stringMatch(state, ";");
          } catch (ParseException &e) {
            try {
              PayLoad decl_payload = if_decl(state);
              declarations.push_back(decl_payload);
              stringMatch(state, ";");
            } catch (ParseException &e) {
              try {
                PayLoad decl_payload = assign_decl(state);
                declarations.push_back(decl_payload);
                stringMatch(state, ";");
              } catch (ParseException &e) {
                try {
                  PayLoad decl_payload = variable_decl(state);
                  declarations.push_back(decl_payload);
                  stringMatch(state, ";");
                } catch (ParseException &e) {
                  try {
                    PayLoad decl_payload = constant_decl(state);
                    declarations.push_back(decl_payload);
                    stringMatch(state, ";");
                  } catch (ParseException &e) {
                    try {
                      PayLoad decl_payload = procedure_decl(state);
                      declarations.push_back(decl_payload);
                      stringMatch(state, ";");
                    } catch (ParseException &e) {
                      throw e;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return declarations;
}

PayLoad select_cl(State &state) {
  State so(state);
  try {
    stringMatch(state, "Select");
    whitespace(state);
    std::string synonym = ident(state);
    return PayLoad(SINGLE, SYNONYM, synonym);
  } catch (ParseException &e) {
    throw ParseException(so.i, state.i, "select_cl", "");
  }
}

QueryMap pqlParse(std::string query) {
  std::cout << std::endl;
  std::cout << query << std::endl;
  QueryMap queryMap;

  // State state(&query);
  std::string str = "while w1;Select w1";
  State state(&str);

  try {
    std::vector<PayLoad> declarations = declaration_cl(state);
    PayLoad p2 = select_cl(state);

    std::cout << declarations[0].getValue() << std::endl;
    std::cout << p2.getValue() << std::endl;

  } catch (ParseException &e) {
    std::cout << e.what() << std::endl;
  }

  std::cout << std::endl;
  return queryMap;
}