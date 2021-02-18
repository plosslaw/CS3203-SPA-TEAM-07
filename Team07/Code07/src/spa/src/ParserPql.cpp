#include "ParserPql.h"
#include "ParserLib.h"
#include <functional>
#include <iostream>
#include <vector>

// std::string wildcard(State &state) {
//   State so(state);
//   try {
//     return stringMatch(state, "_");
//   } catch (ParseException &e) {
//     throw ParseException(so.i, state.i, "wildcard", "");
//   }
// }

// var_name: NAME
std::string var_name(State &state) {
  State so(state);
  try {
    return name(state);
  } catch (ParseException &e) {
    throw ParseException(so.i, state.i, "var_name", "");
  }
}

// const_value: INTEGER
std::string constant(State &state) {
  State so(state);
  try {
    return integer(state);
  } catch (ParseException &e) {
    throw ParseException(so.i, state.i, "constant", "");
  }
}

std::string factor(State &state) {
  State so(state);
  try {
    return var_name(state);
  } catch (ParseException &e) {
    state.assign(so);
    try {
      return constant(state);
    } catch (ParseException &e) {
      state.assign(so);
      state.excps.push_back(e);
      throw ParseException(so.i, state.i, "factor", "");
    }
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
      return integer(state);
    } catch (ParseException &e) {
      state.assign(so);
      try {
        return wildcard(state);
      } catch (ParseException &e) {
        state.assign(so);
        throw ParseException(so.i, state.i, "stmt_ref", "");
      }
    }
  }
}

// entRef: synonym | ‘_’ | ‘"’ IDENT ‘"’
std::string ent_ref(State &state) {
  State so(state);
  try {
    return synonym(state);
  } catch (ParseException &e) {
    state.assign(so);
    try {
      std::string dbl_quotes_1 = double_quotes(state);
      whitespace(state);

      std::string val = ident(state);
      whitespace(state);

      std::string dbl_quotes_2 = double_quotes(state);
      whitespace(state);

      return dbl_quotes_1 + val + dbl_quotes_2;
    } catch (ParseException &e) {
      state.assign(so);

      try {
        return wildcard(state);
      } catch (ParseException &e) {
        state.assign(so);
        throw ParseException(so.i, state.i, "ent_ref", "");
      }
    }
  }
}

// expression-spec: ‘_’ ‘"’ factor ‘"’ ‘_’ | ‘_’
std::string expr_spec(State &state) {
  State so(state);
  try {
    std::string any_val_1 = wildcard(state);
    whitespace(state);

    std::string dbl_quotes_1 = double_quotes(state);
    whitespace(state);

    std::string value = factor(state);
    whitespace(state);

    std::string dbl_quotes_2 = double_quotes(state);
    whitespace(state);

    std::string any_val_2 = wildcard(state);
    whitespace(state);

    return any_val_1 + dbl_quotes_1 + value + dbl_quotes_2 + any_val_2;
  } catch (ParseException &e) {
    state.assign(so);
    try {
      return wildcard(state);
    } catch (ParseException &e) {
      state.assign(so);
      throw ParseException(so.i, state.i, "expr_spec", "");
    }
  }
}

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
    std::vector<PayLoad> while_decl =
        repeat_declaration(state, "while", Single::WHILE);
    declarations.insert(declarations.end(), while_decl.begin(),
                        while_decl.end());
  } catch (ParseException &e) {
    state.assign(so);
    try {
      std::vector<PayLoad> stmt_decl =
          repeat_declaration(state, "stmt", Single::STATEMENT);
      declarations.insert(declarations.end(), stmt_decl.begin(),
                          stmt_decl.end());
    } catch (ParseException &e) {
      state.assign(so);
      try {
        std::vector<PayLoad> read_decl =
            repeat_declaration(state, "read", Single::READ);
        declarations.insert(declarations.end(), read_decl.begin(),
                            read_decl.end());
      } catch (ParseException &e) {
        state.assign(so);
        try {
          std::vector<PayLoad> print_decl =
              repeat_declaration(state, "print", Single::PRINT);
          declarations.insert(declarations.end(), print_decl.begin(),
                              print_decl.end());
        } catch (ParseException &e) {
          state.assign(so);
          try {
            std::vector<PayLoad> call_decl =
                repeat_declaration(state, "call", Single::CALL);
            declarations.insert(declarations.end(), call_decl.begin(),
                                call_decl.end());
          } catch (ParseException &e) {
            state.assign(so);
            try {
              std::vector<PayLoad> if_decl =
                  repeat_declaration(state, "if", Single::IF);
              declarations.insert(declarations.end(), if_decl.begin(),
                                  if_decl.end());
            } catch (ParseException &e) {
              state.assign(so);
              try {
                std::vector<PayLoad> assign_decl =
                    repeat_declaration(state, "assign", Single::ASSIGN);
                declarations.insert(declarations.end(), assign_decl.begin(),
                                    assign_decl.end());
              } catch (ParseException &e) {
                state.assign(so);
                try {
                  std::vector<PayLoad> variable_decl =
                      repeat_declaration(state, "variable", Single::VARIABLE);
                  declarations.insert(declarations.end(), variable_decl.begin(),
                                      variable_decl.end());
                } catch (ParseException &e) {
                  state.assign(so);
                  try {
                    std::vector<PayLoad> constant_decl =
                        repeat_declaration(state, "constant", Single::CONSTANT);
                    declarations.insert(declarations.end(),
                                        constant_decl.begin(),
                                        constant_decl.end());
                  } catch (ParseException &e) {
                    state.assign(so);
                    try {
                      std::vector<PayLoad> procedure_decl = repeat_declaration(
                          state, "procedure", Single::PROCEDURE);
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
    return PayLoad(SINGLE, load_type, values);
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
    selects.push_back(select(state, Single::SYNONYM));
    return selects;
  } catch (ParseException &e) {
    state.assign(so);
    throw ParseException(so.i, state.i, "select_cl", "");
  }
}

// TODO(zs): To refactor allowing stmt and stmt, stmt and ent, ent and ent
std::vector<std::string> stmt_and_stmt_ref(State &state) {
  std::vector<std::string> values;
  State so(state);
  try {
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

    return values;
  } catch (ParseException &e) {
    state.assign(so);
    throw ParseException(so.i, state.i, "stmt_and_stmt_ref", "");
  }
  return values;
}

// TODO(zs): To refactor allowing stmt and stmt, stmt and ent, ent and ent
std::vector<std::string> stmt_and_ent_ref(State &state) {
  std::vector<std::string> values;
  State so(state);
  try {
    stringMatch(state, "(");
    whitespace(state);

    values.push_back(stmt_ref(state));
    whitespace(state);

    stringMatch(state, ",");
    whitespace(state);

    values.push_back(ent_ref(state));
    whitespace(state);

    stringMatch(state, ")");
    whitespace(state);

    return values;
  } catch (ParseException &e) {
    state.assign(so);
    throw ParseException(so.i, state.i, "stmt_and_ent_ref", "");
  }
  return values;
}

// TODO(zs): To refactor allowing stmt and stmt, stmt and ent, ent and ent,
// ent and expr
std::vector<std::string> ent_and_expr_spec(State &state) {
  std::vector<std::string> values;
  State so(state);
  try {
    stringMatch(state, "(");
    whitespace(state);

    values.push_back(ent_ref(state));
    whitespace(state);

    stringMatch(state, ",");
    whitespace(state);

    values.push_back(expr_spec(state));
    whitespace(state);

    stringMatch(state, ")");
    whitespace(state);

    return values;
  } catch (ParseException &e) {
    state.assign(so);
    throw ParseException(so.i, state.i, "ent_and_expr_spec", "");
  }
  return values;
}

PayLoad rel_ref(State &state, std::string design_relation, Pair load_type) {
  // relRef : Follows | FollowsT | Parent | ParentT | UsesS | UsesP | ModifiesS
  // | ModifiesP
  std::vector<std::string> values;
  State so(state);
  try {
    stringMatch(state, design_relation);
    whitespace(state);

    if (load_type == PARENT || load_type == PARENTT || load_type == FOLLOWS ||
        load_type == FOLLOWST) {
      values = stmt_and_stmt_ref(state);
    }
    if (load_type == MODIFIES || load_type == USES) {
      values = stmt_and_ent_ref(state);
    }

    return PayLoad(PAIR, load_type, values);
  } catch (ParseException &e) {
    state.assign(so);
    throw ParseException(so.i, state.i, "stmt_and_stmt_ref", "");
  }
}

PayLoad parent(State &state) {
  State so(state);
  try {
    return rel_ref(state, "Parent", PARENT);
  } catch (ParseException &e) {
    state.assign(so);
    throw ParseException(so.i, state.i, "parent", "");
  }
}

PayLoad parent_t(State &state) {
  State so(state);
  try {
    return rel_ref(state, "Parent*", PARENTT);
  } catch (ParseException &e) {
    state.assign(so);
    throw ParseException(so.i, state.i, "parent_t", "");
  }
}

PayLoad follows(State &state) {
  State so(state);
  try {
    return rel_ref(state, "Follows", FOLLOWS);
  } catch (ParseException &e) {
    state.assign(so);
    throw ParseException(so.i, state.i, "follows", "");
  }
}

PayLoad follows_t(State &state) {
  State so(state);
  try {
    return rel_ref(state, "Follows*", FOLLOWST);
  } catch (ParseException &e) {
    state.assign(so);
    throw ParseException(so.i, state.i, "follows_t", "");
  }
}

PayLoad modifies(State &state) {
  State so(state);
  try {
    return rel_ref(state, "Modifies", MODIFIES);
  } catch (ParseException &e) {
    state.assign(so);
    throw ParseException(so.i, state.i, "modifies", "");
  }
}

PayLoad uses(State &state) {
  State so(state);
  try {
    return rel_ref(state, "Uses", USES);
  } catch (ParseException &e) {
    state.assign(so);
    throw ParseException(so.i, state.i, "uses", "");
  }
}

PayLoad suchthat(State &state) {
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
          try {
            return modifies(state);
          } catch (ParseException &e) {
            state.assign(so);
            try {
              return uses(state);
            } catch (ParseException &e) {
              state.assign(so);
              throw ParseException(so.i, state.i, "suchthat", "");
            }
          }
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

PayLoad syn_assign(State &state) {
  std::vector<std::string> values;
  State so(state);
  try {
    std::string val = synonym(state);
    values.push_back(val);
    whitespace(state);

    std::vector<std::string> vals = ent_and_expr_spec(state);
    values.insert(values.end(), vals.begin(), vals.end());
    whitespace(state);

    return PayLoad(TRIPLE, SYN_ASSIGN, values);
  } catch (ParseException &e) {
    state.assign(so);
    throw ParseException(so.i, state.i, "syn_assign", "");
  }
}

PayLoad pattern(State &state) {
  State so(state);
  try {
    return syn_assign(state);
  } catch (ParseException &e) {
    throw ParseException(so.i, state.i, "pattern", "");
  }
}

PayLoad pattern_cl(State &state) {
  // pattern-cl: ‘pattern’ syn-assign ‘(‘entRef ’,’ expression-spec ’)’
  std::vector<PayLoad> patterns;
  State so(state);
  try {
    stringMatch(state, "pattern");
    whitespace(state);

    PayLoad clause = pattern(state);
    whitespace(state);

    so.assign(state);
    return clause;
  } catch (ParseException &e) {
    state.assign(so);
    throw ParseException(so.i, state.i, "pattern_cl", "");
  }
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
    patterns.push_back(pattern_cl(state));
    so.assign(state);
  } catch (ParseException &e) {
    state.assign(so);
  }

  return QueryMap(declarations, selects, suchthats, patterns);
}
