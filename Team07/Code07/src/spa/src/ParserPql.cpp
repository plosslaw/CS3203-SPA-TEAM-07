#include "ParserPql.h"
#include "ParserLib.h"
#include <iostream>
#include <vector>

std::string ident(State &state) {
  int init = state.i;
  try {
    char r1 = charPredicate(state, &alphaPred, "letter"); // :- letter
    std::string r2 = alphaNum(state); // :- (letter | digit)*
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

    std::vector<std::string> values;
    std::string synonym = ident(state);
    values.push_back(synonym);
    whitespace(state);

    so.assign(state);
    return PayLoad(SINGLE, load_type, values);
  } catch (ParseException &e) {
    State so1(state);
    state.assign(so);
    state.excps.push_back(e);
    throw ParseException(so.i, state.i, "declaration", "");
  }
}

std::vector<PayLoad> repeat_declaration(State &state, std::string design_entity,
                                        Single load_type) {
  State so(state);
  std::vector<PayLoad> declarations;

  try {
    declarations.push_back(declaration(state, design_entity, load_type));
    so.assign(state);
  } catch (ParseException &e) {
    throw e;
  }
  try {
    while (true) {
      declarations.push_back(declaration(state, ",", load_type));
      so.assign(state);
    }
  } catch (ParseException &e) {
    State so1(state);
    state.assign(so);
    try {
      stringMatch(state, ";");
      whitespace(state);
    } catch (ParseException &e) {
      state.assign(so1);
      throw e;
    }
  }
  return declarations;
}

std::vector<PayLoad> declaration_cl(State &state) {
  // declaration : design-entity synonym (‘,’ synonym)* ‘;’
  // design-entity : ‘stmt’ | ‘read’ | ‘print’ | ‘call’ | ‘while’ | ‘if’ |
  // ‘assign’ | ‘variable’ | ‘constant’ | ‘procedure’
  State so(state);
  std::vector<PayLoad> declarations;
  try {
    std::vector<PayLoad> while_decl = repeat_declaration(state, "while", WHILE);
    declarations.insert(declarations.end(), while_decl.begin(),
                        while_decl.end());
  } catch (ParseException &e) {
    state.assign(so);
    try {
      std::vector<PayLoad> stmt_decl =
          repeat_declaration(state, "stmt", STATEMENT);
      declarations.insert(declarations.end(), stmt_decl.begin(),
                          stmt_decl.end());
    } catch (ParseException &e) {
      state.assign(so);
      try {
        std::vector<PayLoad> read_decl =
            repeat_declaration(state, "read", READ);
        declarations.insert(declarations.end(), read_decl.begin(),
                            read_decl.end());
      } catch (ParseException &e) {
        state.assign(so);
        try {
          std::vector<PayLoad> print_decl =
              repeat_declaration(state, "print", PRINT);
          declarations.insert(declarations.end(), print_decl.begin(),
                              print_decl.end());
        } catch (ParseException &e) {
          state.assign(so);
          try {
            std::vector<PayLoad> call_decl =
                repeat_declaration(state, "call", CALL);
            declarations.insert(declarations.end(), call_decl.begin(),
                                call_decl.end());
          } catch (ParseException &e) {
            state.assign(so);
            try {
              std::vector<PayLoad> if_decl =
                  repeat_declaration(state, "if", IF);
              declarations.insert(declarations.end(), if_decl.begin(),
                                  if_decl.end());
            } catch (ParseException &e) {
              state.assign(so);
              try {
                std::vector<PayLoad> assign_decl =
                    repeat_declaration(state, "assign", ASSIGN);
                declarations.insert(declarations.end(), assign_decl.begin(),
                                    assign_decl.end());
              } catch (ParseException &e) {
                state.assign(so);
                try {
                  std::vector<PayLoad> variable_decl =
                      repeat_declaration(state, "variable", VARIABLE);
                  declarations.insert(declarations.end(), variable_decl.begin(),
                                      variable_decl.end());
                } catch (ParseException &e) {
                  state.assign(so);
                  try {
                    std::vector<PayLoad> constant_decl =
                        repeat_declaration(state, "constant", CONSTANT);
                    declarations.insert(declarations.end(),
                                        constant_decl.begin(),
                                        constant_decl.end());
                  } catch (ParseException &e) {
                    state.assign(so);
                    try {
                      std::vector<PayLoad> procedure_decl =
                          repeat_declaration(state, "procedure", PROCEDURE);
                      declarations.insert(declarations.end(),
                                          procedure_decl.begin(),
                                          procedure_decl.end());
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

PayLoad select(State &state, Single load_type) {
  State so(state);
  try {
    stringMatch(state, "Select");
    whitespace(state);

    std::vector<std::string> values;
    std::string synonym = ident(state);
    values.push_back(synonym);
    whitespace(state);

    so.assign(state);
    return PayLoad(SINGLE, SYNONYM, values);
  } catch (ParseException &e) {
    State so1(state);
    state.assign(so);
    state.excps.push_back(e);
    throw ParseException(so.i, state.i, "select", "");
  }
}

std::vector<PayLoad> select_cl(State &state) {
  std::vector<PayLoad> selects;
  State so(state);
  try {
    selects.push_back(select(state, SYNONYM));
    return selects;
  } catch (ParseException &e) {
    state.assign(so);
    throw ParseException(so.i, state.i, "select_cl", "");
  }
}
// TODO(zs)
std::vector<PayLoad> suchthat_cl(State &state) {
  // suchthat-cl : ‘such that’ relRef
  // relRef : Follows | FollowsT | Parent | ParentT | UsesS | UsesP | ModifiesS
  // | ModifiesP
  std::vector<PayLoad> suchthats;
  State so(state);
  try {
    stringMatch(state, "such that");
    whitespace(state);

    // TODO(zs):  suchthat_cl

    return suchthats;
  } catch (ParseException &e) {
    state.assign(so);
    throw ParseException(so.i, state.i, "suchthat_cl", "");
  }
}
// TODO(zs)
std::vector<PayLoad> pattern_cl(State &state) {
  std::vector<PayLoad> patterns;
  State so(state);
  try {
    stringMatch(state, "pattern");
    whitespace(state);

    // TODO(zs): pattern_cl

    return patterns;
  } catch (ParseException &e) {
    state.assign(so);
    throw ParseException(so.i, state.i, "pattern_cl", "");
  }
  return patterns;
}

QueryMap pqlParse(std::string query) {
  QueryMap queryMap;
  std::vector<PayLoad> declarations;
  std::vector<PayLoad> selects;
  std::vector<PayLoad> suchthats;
  std::vector<PayLoad> patterns;

  State state(&query);
  State so(state);
  try {
    std::vector<PayLoad> declarations = declaration_cl(state);
    std::vector<PayLoad> selects = select_cl(state);

    // [suchthat-cl]
    try {
      std::vector<PayLoad> suchthats = suchthat_cl(state);
    } catch (ParseException &e) {
      State so1(state);
      state.assign(so);

      // [pattern-cl]
      try {
        std::vector<PayLoad> patterns = pattern_cl(state);
      } catch (ParseException &e) {
        State so2(state);
        state.assign(so1);
        return QueryMap(declarations, selects, suchthats, patterns);
      }
    }
    return QueryMap(declarations, selects, suchthats, patterns);
  } catch (ParseException &e) {
    std::cout << e.what() << std::endl;
  }
  return QueryMap(declarations, selects, suchthats, patterns);
}
