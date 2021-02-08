#pragma once

#include <unordered_map>
#include "PKBEntities.hpp"

typedef struct {
	std::unordered_map<var_ref, variable> variables;
	std::unordered_map<stmt_ref, statement> statements;
	std::unordered_map<proc_ref, procedure> procedures;
	std::unordered_set<constant> constants;
	TNode ast_root;
} PKB;