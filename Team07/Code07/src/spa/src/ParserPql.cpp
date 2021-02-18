#include "ParserPql.h"
#include "ParserLib.h"
#include <iostream>
#include <vector>

std::string wildcard(State &state) {
  State so(state);
  try {
    stringMatch(state, "_");
    return "_";
  } catch (ParseException &e) {
    throw ParseException(so.i, state.i, "wildcard", "");
  }
}

// stmtRef: synonym | ‘_’ | INTEGER
std::string stmt_ref(State &state) {
  State so(state);
  try {
    return synonym(state);
  } catch (ParseException &e) {
    state.assign(so);
    try {
      return wildcard(state);
    } catch (ParseException &e) {
      state.assign(so);
      try {
        return integer(state);
      } catch (ParseException &e) {
        state.assign(so);
        throw ParseException(so.i, state.i, "stmt_ref", "");
      }
    }
  }
}

// TODO(zs):
// entRef: synonym | ‘_’ | ‘"’ IDENT ‘"’

PayLoad declaration(State &state, std::string design_entity, Single load_type) {
  State so(state);
  try {
    stringMatch(state, design_entity);
    whitespace(state);

    std::vector<std::string> values;
    values.push_back(synonym(state));
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
  // declaration : design-entity synonym (‘,’ synonym)* ‘;’
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
    values.push_back(synonym(state));
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

PayLoad stmt_and_stmt_ref(State &state, std::string relation_ref,
                          Pair load_type) {
  std::vector<std::string> values;
  State so(state);
  try {
    stringMatch(state, relation_ref);
    whitespace(state);

    stringMatch(state, "(");
    whitespace(state);

    values.push_back(stmt_ref(state));
    whitespace(state);

    stringMatch(state, ",");
    whitespace(state);

    values.push_back(stmt_ref(state));
    whitespace(state);

    stringMatch(state, ")");
    whitespace(state);

    return PayLoad(PAIR, load_type, values);
  } catch (ParseException &e) {
    state.assign(so);
    throw ParseException(so.i, state.i, "stmt_and_stmt_ref", "");
  }
}

PayLoad parent(State &state) {
  State so(state);
  try {
    return stmt_and_stmt_ref(state, "Parent", PARENT);
  } catch (ParseException &e) {
    throw ParseException(so.i, state.i, "parent", "");
  }
}

PayLoad parent_t(State &state) {
  State so(state);
  try {
    return stmt_and_stmt_ref(state, "Parent*", PARENTT);
  } catch (ParseException &e) {
    throw ParseException(so.i, state.i, "parent_t", "");
  }
}

PayLoad follows(State &state) {
  State so(state);
  try {
    return stmt_and_stmt_ref(state, "Follows", FOLLOWS);
  } catch (ParseException &e) {
    throw ParseException(so.i, state.i, "follows", "");
  }
}

PayLoad follows_t(State &state) {
  State so(state);
  try {
    return stmt_and_stmt_ref(state, "Follows*", FOLLOWST);
  } catch (ParseException &e) {
    throw ParseException(so.i, state.i, "follows_t", "");
  }
}

PayLoad suchthat(State &state) {
  // relRef : Follows | FollowsT | Parent | ParentT | UsesS | UsesP | ModifiesS
  // | ModifiesP
  State so(state);
  try {
    return parent(state);
  } catch (ParseException &e) {
    state.assign(so);
    try {
      return parent_t(state);
    } catch (ParseException &e) {
      state.assign(so);
      try {
        return follows(state);
      } catch (ParseException &e) {
        state.assign(so);
        try {
          return follows_t(state);
        } catch (ParseException &e) {
          state.assign(so);
          throw ParseException(so.i, state.i, "suchthat", "");
        }
      }
    }
  }
}

PayLoad suchthat_cl(State &state) {
  // suchthat-cl : ‘such that’ relRef
  std::vector<PayLoad> suchthats;
  State so(state);
  try {
    stringMatch(state, "such that");
    whitespace(state);

    PayLoad clause = suchthat(state);
    whitespace(state);

    so.assign(state);
    return clause;
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

    // pattern_cl

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

  // declaration*
  try {
    while (true) {
      std::vector<PayLoad> single_grp_declarations = declaration_cl(state);
      declarations.insert(declarations.end(), single_grp_declarations.begin(),
                          single_grp_declarations.end());
      so.assign(state);
    }
  } catch (ParseException &e) {
    state.assign(so);
  }

  // select-cl
  try {
    selects = select_cl(state);
    so.assign(state);
  } catch (ParseException &e) {
    state.assign(so);
  }

  // [suchthat-cl]
  try {
    suchthats.push_back(suchthat_cl(state));
    so.assign(state);
  } catch (ParseException &e) {
    state.assign(so);
  }

  // [pattern-cl]
  try {
    patterns = pattern_cl(state);
    so.assign(state);
  } catch (ParseException &e) {
    state.assign(so);
  }

  return QueryMap(declarations, selects, suchthats, patterns);
}
