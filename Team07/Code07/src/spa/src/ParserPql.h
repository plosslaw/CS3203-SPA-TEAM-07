#include "ParserLib.h"
#include "QueryMap.h"
#include <string.h>

QueryMap pql_query(State &state);
std::vector<PayLoad> declaration_cl(State &state);
std::vector<PayLoad> select_cl(State &state);
PayLoad suchthat_cl(State &state);
PayLoad pattern_cl(State &state);

QueryMap pql_validate(QueryMap query);
bool is_declaration_clause_valid(QueryMap query);
bool is_select_clause_valid(QueryMap query);
bool is_suchthat_clause_valid(QueryMap query);
bool is_pattern_clause_valid(QueryMap query);

/*
// low level abstractions
std::string var_name(State &state);
std::string constant(State &state);
std::string factor(State &state);

// std::string stmt_ref(State &state);
std::string ent_ref(State &state);
std::string expr_spec(State &state);
std::vector<std::string> stmt_and_stmt_ref(State &state);
std::vector<std::string> stmt_and_ent_ref(State &state);
std::vector<std::string> ent_and_expr_spec(State &state);

// declaration clause
std::vector<PayLoad> declaration(State &state, std::string design_entity,
                                 Single load_type);

// select clause
PayLoad select(State &state, Single load_type);

// such that clause
PayLoad rel_ref(State &state, std::string design_relation, Pair load_type);
PayLoad parent(State &state);
PayLoad parent_t(State &state);
PayLoad follows(State &state);
PayLoad follows_t(State &state);
PayLoad modifies(State &state);
PayLoad uses(State &state);
PayLoad suchthat(State &state);

// pattern clause
PayLoad syn_assign(State &state);
PayLoad pattern(State &state);
*/
