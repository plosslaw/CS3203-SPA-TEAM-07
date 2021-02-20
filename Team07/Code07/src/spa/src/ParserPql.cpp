#include "ParserPql.h"
#include "ParserLib.h"
#include "QueryMap.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

// -------------------- Low Level Abstractions -------------------- //

// var_name: NAME
std::string var_name(State &state) {
  State so(state);
  try {
    return name(state);
  } catch (ParseException &e) {
    state.excps.push_back(e);
    throw ParseException(so.i, state.i, "var_name", "");
  }
}

// const_value: INTEGER
std::string constant(State &state) {
  State so(state);
  try {
    return integer(state);
  } catch (ParseException &e) {
    state.excps.push_back(e);
    throw ParseException(so.i, state.i, "constant", "");
  }
}

// factor: var_name | const_value
std::string factor(State &state) {
  State so(state);
  try {
    return var_name(state);
  } catch (ParseException &e) {
    state.assign(so);
    try {
      return constant(state);
    } catch (ParseException &e) {
      state.excps.push_back(e);
      throw ParseException(so.i, state.i, "factor", "");
    }
  }
}

// stmtRef: synonym | ‘_’ | INTEGER
PayLoad stmt_ref(State &state) {
  State so(state);
  try {
    return PayLoad(SINGLE, Single::SYNONYM,
                   std::vector<std::string>{synonym(state)},
                   std::vector<bool>{true});
  } catch (ParseException &e) {
    state.assign(so);
    try {
      return PayLoad(SINGLE, Single::INTEGER,
                     std::vector<std::string>{integer(state)});
    } catch (ParseException &e) {
      state.assign(so);
      try {
        return PayLoad(SINGLE, Single::WILDCARD,
                       std::vector<std::string>{wildcard(state)});
      } catch (ParseException &e) {
        state.excps.push_back(e);
        throw ParseException(so.i, state.i, "stmt_ref", "");
      }
    }
  }
}

// entRef: synonym | ‘_’ | ‘"’ IDENT ‘"’
PayLoad ent_ref(State &state) {
  State so(state);
  try {
    return PayLoad(SINGLE, Single::SYNONYM,
                   std::vector<std::string>{synonym(state)},
                   std::vector<bool>{true});
  } catch (ParseException &e) {
    state.assign(so);
    try {
      std::string dbl_quotes_1 = double_quotes(state);
      whitespace(state);

      std::string val = ident(state);
      whitespace(state);

      std::string dbl_quotes_2 = double_quotes(state);
      whitespace(state);

      return PayLoad(
          SINGLE, Single::DOUBLE_QUOTE_IDENT,
          std::vector<std::string>{val});
    } catch (ParseException &e) {
      state.assign(so);
      try {
        return PayLoad(SINGLE, Single::WILDCARD,
                       std::vector<std::string>{wildcard(state)});
      } catch (ParseException &e) {
        state.excps.push_back(e);
        throw ParseException(so.i, state.i, "ent_ref", "");
      }
    }
  }
}

// expression-spec: ‘_’ ‘"’ factor ‘"’ ‘_’ | ‘_’
PayLoad expr_spec(State &state) {
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

    return PayLoad(SINGLE, Single::U_DQ_FACTOR,
                   std::vector<std::string>{any_val_1 + value + any_val_2});
  } catch (ParseException &e) {
    state.assign(so);
    try {
      return PayLoad(SINGLE, Single::WILDCARD,
                     std::vector<std::string>{wildcard(state)});
    } catch (ParseException &e) {
      state.excps.push_back(e);
      throw ParseException(so.i, state.i, "expr_spec", "");
    }
  }
}

// TODO(zs): To refactor allowing stmt and stmt, stmt and ent, ent and ent, ent
// and expr ‘(’ stmtRef ‘,’ stmtRef ‘)’
std::vector<PayLoad> stmt_and_stmt_ref(State &state) {
  std::vector<PayLoad> refs;
  State so(state);
  try {
    stringMatch(state, "(");
    whitespace(state);

    refs.push_back(stmt_ref(state));
    whitespace(state);

    stringMatch(state, ",");
    whitespace(state);

    refs.push_back(stmt_ref(state));
    whitespace(state);

    whitespace(state);

    stringMatch(state, ")");
    whitespace(state);

    return refs;
  } catch (ParseException &e) {
    state.excps.push_back(e);
    throw ParseException(so.i, state.i, "stmt_and_stmt_ref", "");
  }
}

// TODO(zs): To refactor allowing stmt and stmt, stmt and ent, ent and ent, ent
// and expr ‘(’ stmtRef ‘,’ entRef ‘)’
std::vector<PayLoad> stmt_and_ent_ref(State &state) {
  std::vector<PayLoad> refs;
  State so(state);
  try {
    stringMatch(state, "(");
    whitespace(state);

    refs.push_back(stmt_ref(state));
    whitespace(state);

    stringMatch(state, ",");
    whitespace(state);

    refs.push_back(ent_ref(state));
    whitespace(state);

    stringMatch(state, ")");
    whitespace(state);

    return refs;
  } catch (ParseException &e) {
    state.excps.push_back(e);
    throw ParseException(so.i, state.i, "stmt_and_ent_ref", "");
  }
}

// TODO(zs): To refactor allowing stmt and stmt, stmt and ent, ent and ent, ent
// and expr ‘(‘entRef ’,’ expression-spec ’)’
std::vector<PayLoad> ent_and_expr_spec(State &state) {
  std::vector<PayLoad> refs;
  State so(state);
  try {
    stringMatch(state, "(");
    whitespace(state);

    refs.push_back(ent_ref(state));
    whitespace(state);

    stringMatch(state, ",");
    whitespace(state);

    refs.push_back(expr_spec(state));
    whitespace(state);

    stringMatch(state, ")");
    whitespace(state);

    return refs;
  } catch (ParseException &e) {
    state.excps.push_back(e);
    throw ParseException(so.i, state.i, "ent_and_expr_spec", "");
  }
}

// -------------------- Declaration Clause -------------------- //

// declaration : design-entity synonym (‘,’ synonym)* ‘;’
std::vector<PayLoad> declaration(State &state, std::string design_entity,
                                 Single load_type) {
  std::vector<PayLoad> declarations;
  State so(state);
  try {
    stringMatch(state, design_entity);
    whitespace(state);

    std::vector<std::string> values;
    values.push_back(synonym(state));
    whitespace(state);

    declarations.push_back(PayLoad(SINGLE, load_type, values));
    so.assign(state);

    while (true) {
      stringMatch(state, ",");
      whitespace(state);

      std::vector<std::string> values;
      values.push_back(synonym(state));
      whitespace(state);

      declarations.push_back(PayLoad(SINGLE, load_type, values));
      so.assign(state);
    }
  } catch (ParseException &e) {
    state.assign(so);
    try {
      stringMatch(state, ";");
      whitespace(state);
      return declarations;
    } catch (ParseException &e) {
      state.excps.push_back(e);
      throw ParseException(so.i, state.i, "declaration", "");
    }
  }
}

// design-entity : ‘stmt’ | ‘read’ | ‘print’ | ‘call’ | ‘while’ | ‘if’ |
// ‘assign’ | ‘variable’ | ‘constant’ | ‘procedure’
std::vector<PayLoad> declaration_cl(State &state) {
  State so(state);
  std::vector<PayLoad> declarations;
  try {
    std::vector<PayLoad> while_decl =
        declaration(state, "while", Single::WHILE);
    declarations.insert(declarations.end(), while_decl.begin(),
                        while_decl.end());
  } catch (ParseException &e) {
    state.assign(so);
    try {
      std::vector<PayLoad> stmt_decl =
          declaration(state, "stmt", Single::STATEMENT);
      declarations.insert(declarations.end(), stmt_decl.begin(),
                          stmt_decl.end());
    } catch (ParseException &e) {
      state.assign(so);
      try {
        std::vector<PayLoad> read_decl =
            declaration(state, "read", Single::READ);
        declarations.insert(declarations.end(), read_decl.begin(),
                            read_decl.end());
      } catch (ParseException &e) {
        state.assign(so);
        try {
          std::vector<PayLoad> print_decl =
              declaration(state, "print", Single::PRINT);
          declarations.insert(declarations.end(), print_decl.begin(),
                              print_decl.end());
        } catch (ParseException &e) {
          state.assign(so);
          try {
            std::vector<PayLoad> call_decl =
                declaration(state, "call", Single::CALL);
            declarations.insert(declarations.end(), call_decl.begin(),
                                call_decl.end());
          } catch (ParseException &e) {
            state.assign(so);
            try {
              std::vector<PayLoad> if_decl =
                  declaration(state, "if", Single::IF);
              declarations.insert(declarations.end(), if_decl.begin(),
                                  if_decl.end());
            } catch (ParseException &e) {
              state.assign(so);
              try {
                std::vector<PayLoad> assign_decl =
                    declaration(state, "assign", Single::ASSIGN);
                declarations.insert(declarations.end(), assign_decl.begin(),
                                    assign_decl.end());
              } catch (ParseException &e) {
                state.assign(so);
                try {
                  std::vector<PayLoad> variable_decl =
                      declaration(state, "variable", Single::VARIABLE);
                  declarations.insert(declarations.end(), variable_decl.begin(),
                                      variable_decl.end());
                } catch (ParseException &e) {
                  state.assign(so);
                  try {
                    std::vector<PayLoad> constant_decl =
                        declaration(state, "constant", Single::CONSTANT);
                    declarations.insert(declarations.end(),
                                        constant_decl.begin(),
                                        constant_decl.end());
                  } catch (ParseException &e) {
                    state.assign(so);
                    try {
                      std::vector<PayLoad> procedure_decl =
                          declaration(state, "procedure", Single::PROCEDURE);
                      declarations.insert(declarations.end(),
                                          procedure_decl.begin(),
                                          procedure_decl.end());
                    } catch (ParseException &e) {
                      state.excps.push_back(e);
                      throw ParseException(so.i, state.i, "declaration_cl", "");
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

// -------------------- Select Clause -------------------- //

// ‘Select’ synonym
PayLoad select(State &state, Single load_type) {
  State so(state);
  try {
    stringMatch(state, "Select");
    whitespace(state);

    std::vector<std::string> values;
    values.push_back(synonym(state));
    whitespace(state);

    return PayLoad(SINGLE, load_type, values, std::vector<bool>{true});
  } catch (ParseException &e) {
    state.excps.push_back(e);
    throw ParseException(so.i, state.i, "select", "");
  }
}

// select-cl: ... ‘Select’ synonym ...
std::vector<PayLoad> select_cl(State &state) {
  std::vector<PayLoad> selects;
  State so(state);
  try {
    selects.push_back(select(state, Single::SYNONYM));
    return selects;
  } catch (ParseException &e) {
    state.excps.push_back(e);
    throw ParseException(so.i, state.i, "select_cl", "");
  }
}

// -------------------- Such That Clause -------------------- //
std::vector<std::string> payloads_to_values(std::vector<PayLoad> payloads) {
  std::vector<std::string> values;
  for (auto it = payloads.begin(); it != payloads.end(); ++it) {
    std::string val = (*it).getValue()[0];
    values.push_back(val);
  }
  return values;
}

std::vector<bool> payloads_to_flags(std::vector<PayLoad> payloads) {
  std::vector<bool> flags;
  for (auto it = payloads.begin(); it != payloads.end(); ++it) {
    LoadType type = (*it).getType();
    if (type.single == Single::SYNONYM) {
      flags.push_back(true);
    } else {
      flags.push_back(false);
    }
  }
  return flags;
}

// relRef : Follows | FollowsT | Parent | ParentT | UsesS | UsesP | ModifiesS
// | ModifiesP
PayLoad rel_ref(State &state, std::string design_relation, Pair load_type) {
  std::vector<std::string> values;
  std::vector<bool> flags;

  State so(state);
  try {
    stringMatch(state, design_relation);
    whitespace(state);

    if (load_type == PARENT || load_type == PARENTT || load_type == FOLLOWS ||
        load_type == FOLLOWST) {
      std::vector<PayLoad> pair_1 = stmt_and_stmt_ref(state);
      values = payloads_to_values(pair_1);
      flags = payloads_to_flags(pair_1);
    }
    if (load_type == MODIFIES || load_type == USES) {
      std::vector<PayLoad> pair_2 = stmt_and_ent_ref(state);
      values = payloads_to_values(pair_2);
      flags = payloads_to_flags(pair_2);
    }

    return PayLoad(PAIR, load_type, values, flags);
  } catch (ParseException &e) {
    state.excps.push_back(e);
    throw ParseException(so.i, state.i, "stmt_and_stmt_ref", "");
  }
}

PayLoad parent(State &state) {
  State so(state);
  try {
    return rel_ref(state, "Parent", PARENT);
  } catch (ParseException &e) {
    state.excps.push_back(e);
    throw ParseException(so.i, state.i, "parent", "");
  }
}

PayLoad parent_t(State &state) {
  State so(state);
  try {
    return rel_ref(state, "Parent*", PARENTT);
  } catch (ParseException &e) {
    state.excps.push_back(e);
    throw ParseException(so.i, state.i, "parent_t", "");
  }
}

PayLoad follows(State &state) {
  State so(state);
  try {
    return rel_ref(state, "Follows", FOLLOWS);
  } catch (ParseException &e) {
    state.excps.push_back(e);
    throw ParseException(so.i, state.i, "follows", "");
  }
}

PayLoad follows_t(State &state) {
  State so(state);
  try {
    return rel_ref(state, "Follows*", FOLLOWST);
  } catch (ParseException &e) {
    state.excps.push_back(e);
    throw ParseException(so.i, state.i, "follows_t", "");
  }
}

PayLoad modifies(State &state) {
  State so(state);
  try {
    return rel_ref(state, "Modifies", MODIFIES);
  } catch (ParseException &e) {
    state.excps.push_back(e);
    throw ParseException(so.i, state.i, "modifies", "");
  }
}

PayLoad uses(State &state) {
  State so(state);
  try {
    return rel_ref(state, "Uses", USES);
  } catch (ParseException &e) {
    state.excps.push_back(e);
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
              state.excps.push_back(e);
              throw ParseException(so.i, state.i, "suchthat", "");
            }
          }
        }
      }
    }
  }
}

// suchthat-cl : ‘such that’ relRef
PayLoad suchthat_cl(State &state) {
  std::vector<PayLoad> suchthats;
  State so(state);
  try {
    stringMatch(state, "such that");
    whitespace(state);

    PayLoad clause = suchthat(state);
    whitespace(state);

    return clause;
  } catch (ParseException &e) {
    state.excps.push_back(e);
    throw ParseException(so.i, state.i, "suchthat_cl", "");
  }
}

// -------------------- Pattern Clause -------------------- //

PayLoad syn_assign(State &state) {
  std::vector<PayLoad> args;

  State so(state);
  try {
    args.push_back(PayLoad(SINGLE, Single::SYNONYM,
                           std::vector<std::string>{synonym(state)},
                           std::vector<bool>{true}));
    whitespace(state);

    std::vector<PayLoad> pair = ent_and_expr_spec(state);
    args.insert(args.end(), pair.begin(), pair.end());
    std::vector<std::string> values = payloads_to_values(args);
    std::vector<bool> flags = payloads_to_flags(args);
    whitespace(state);

    return PayLoad(TRIPLE, SYN_ASSIGN, values, flags);
  } catch (ParseException &e) {
    state.excps.push_back(e);
    throw ParseException(so.i, state.i, "syn_assign", "");
  }
}

PayLoad pattern(State &state) {
  State so(state);
  try {
    return syn_assign(state);
  } catch (ParseException &e) {
    state.excps.push_back(e);
    throw ParseException(so.i, state.i, "pattern", "");
  }
}

// pattern-cl: ‘pattern’ syn-assign ‘(‘entRef ’,’ expression-spec ’)’
PayLoad pattern_cl(State &state) {
  std::vector<PayLoad> patterns;
  State so(state);
  try {
    stringMatch(state, "pattern");
    whitespace(state);

    PayLoad clause = pattern(state);
    whitespace(state);

    return clause;
  } catch (ParseException &e) {
    state.excps.push_back(e);
    throw ParseException(so.i, state.i, "pattern_cl", "");
  }
}

QueryMap pql_query(State &state) {
  QueryMap queryMap;
  std::vector<PayLoad> declarations;
  std::vector<PayLoad> selects;
  std::vector<PayLoad> suchthats;
  std::vector<PayLoad> patterns;

  State so(state);
  // declaration*
  try {
    whitespace(state);
    while (true) {
      std::vector<PayLoad> single_ent_declarations = declaration_cl(state);
      declarations.insert(declarations.end(), single_ent_declarations.begin(),
                          single_ent_declarations.end());
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

  if (state.i != (*state.source).size()) {
    throw ParseException(so.i, state.i, "pql_query", "");
  }

  return QueryMap(declarations, selects, suchthats, patterns);
}

// TODO(zs): Optimise comparison
// Returns true if payload is in vector.
bool is_payload_in(std::vector<PayLoad> payloads, PayLoad target) {
  for (auto it_dcl = payloads.begin(); it_dcl != payloads.end(); ++it_dcl) {
    PayLoad current = (*it_dcl);
    if (current == target) {
      return true;
    }
  }
  return false;
}

// TODO(zs): Optimise comparison
// Returns true if string is in first item of payload's vector
bool is_value_in_declarations(std::vector<PayLoad> payloads,
                              std::string target) {
  for (auto it = payloads.begin(); it != payloads.end(); ++it) {
    std::string current = (*it).getValue()[0];
    if (current == target) {
      return true;
    }
  }
  return false;
}

bool is_synonym_unique(std::vector<PayLoad> declarations) {
  std::vector<std::string> synonyms;
  for (auto it = declarations.begin(); it != declarations.end(); ++it) {
    synonyms.push_back((*it).getValue()[0]);
  }
  sort(synonyms.begin(), synonyms.end());
  auto it = std::unique(synonyms.begin(), synonyms.end());
  return (it == synonyms.end());
}

bool is_synonym_declared(std::vector<PayLoad> declarations, PayLoad target) {
  std::vector<std::string> values = target.getValue();
  std::vector<bool> flags = target.get_flag();

  int idx = 0;
  for (auto it = flags.begin(); it != flags.end(); ++it) {
    bool is_synonym = (*it);
    if (is_synonym) {
      if (!is_value_in_declarations(declarations, values[idx])) {
        return false;
      }
    }
    idx++;
  }
  return true;
}

// Returns true if synonyms are unique
bool is_declaration_clause_valid(QueryMap table) {
  std::vector<PayLoad> declarations = table.getList(ClauseType::DECLARATION);
  return is_synonym_unique(declarations);
}

// Returns true if 1 synonym is selected and
// synonym declared
bool is_select_clause_valid(QueryMap table) {
  std::vector<PayLoad> selections = table.getList(ClauseType::SELECT);
  if (selections.size() == 0) {
    return false;
  }

  std::vector<PayLoad> declarations = table.getList(ClauseType::DECLARATION);
  PayLoad target_synonym = selections[0];
  return is_synonym_declared(declarations, target_synonym);
}

// synonyms declared
// wildcard does not exist as first argument of Uses/ Modifies
bool is_suchthat_clause_valid(QueryMap table) {
  std::vector<PayLoad> declarations = table.getList(ClauseType::DECLARATION);
  std::vector<PayLoad> suchthats = table.getList(ClauseType::SUCHTHAT);

  // optional suchthat-cl absent
  if (suchthats.size() == 0) {
    return true;
  }

  for (auto it = suchthats.begin(); it != suchthats.end(); ++it) {
    PayLoad target = (*it);
    if (!is_synonym_declared(declarations, target)) {
      return false;
    }
  }
  return true;
}

// synonyms declared
// syn_assign synonym is of assign design entity
bool is_pattern_clause_valid(QueryMap table) {
  std::vector<PayLoad> declarations = table.getList(ClauseType::DECLARATION);
  std::vector<PayLoad> patterns = table.getList(ClauseType::PATTERN);

  // optional suchthat-cl absent
  if (patterns.size() == 0) {
    return true;
  }

  for (auto it = patterns.begin(); it != patterns.end(); ++it) {
    PayLoad current_clause = (*it);
    std::string current_synonym = current_clause.getValue()[0];
    if (!is_synonym_declared(declarations, current_clause)) {
      return false;
    }

    if (current_clause.getType().triple == SYN_ASSIGN) {
      PayLoad target(SINGLE, Single::ASSIGN,
                     std::vector<std::string>{current_synonym});
      if (!is_payload_in(declarations, target)) {
        return false;
      }
    }
  }

  return true;
}

QueryMap pql_validate(QueryMap query) {
  if (query.is_empty()) {
    throw prettyPrintValidation("Empty query.");
  }

  if (!is_declaration_clause_valid(query)) {
    throw prettyPrintValidation("Invalid declaration clause.");
  }
  if (!is_select_clause_valid(query)) {
    throw prettyPrintValidation("Invalid select clause.");
  }
  if (!is_suchthat_clause_valid(query)) {
    throw prettyPrintValidation("Invalid such that clause.");
  }
  if (!is_pattern_clause_valid(query)) {
    throw prettyPrintValidation("Invalid pattern clause.");
  }

  return query;
}
