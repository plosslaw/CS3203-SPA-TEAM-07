#include "ParserLib.h"
#include "QueryMap.h"
#include <string.h>

PayLoad declaration(State &state, std::string design_entity, Single load_type);
std::vector<PayLoad> repeat_declaration(State &state, std::string design_entity,
                                        Single load_type);
std::vector<PayLoad> declaration_cl(State &state);
std::vector<PayLoad> select_cl(State &state);
PayLoad suchthat_cl(State &state);
std::vector<PayLoad> pattern_cl(State &state);
QueryMap pqlParse(std::string query);
