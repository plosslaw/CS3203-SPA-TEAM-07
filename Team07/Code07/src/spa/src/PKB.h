#pragma once

#include <unordered_map>
#include <unordered_set>
#include "PKBEntities.hpp"

typedef struct {
	// PKB Table for procedures
	std::unordered_map<proc_ref, procedure> procedures;

	// PKB Table for statements
	std::unordered_map<stmt_ref, statement> statements;

    // when parsing AST, insert into this table if the statement is an assignment.
    // assign_ref is a superclass of stmt_ref
    // an assignment should be included in statements
    std::unordered_map<assign_ref, assignment> assignments;

	// PKB Table for variables
	std::unordered_map<var_ref, variable> variables;

	// List of constant for select constant queries
	std::unordered_set<const_value> constants;

	// Root node of the program's AST tree
	TNode ast_root;
} PKB;
