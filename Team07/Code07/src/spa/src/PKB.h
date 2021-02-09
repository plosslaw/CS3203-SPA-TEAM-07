#pragma once

#include <unordered_map>
#include "PKBEntities.hpp"

typedef struct {
	// PKB Table for procedures
	std::unordered_map<proc_ref, procedure> procedures;

	// PKB Table for statements
	std::unordered_map<stmt_ref, statement> statements;

	// PKB Table for variables
	std::unordered_map<var_ref, variable> variables;

	// List of constant for select constant queries
	std::unordered_set<constant> constants;

	// Root node of the program's AST tree
	TNode ast_root;
} PKB;
