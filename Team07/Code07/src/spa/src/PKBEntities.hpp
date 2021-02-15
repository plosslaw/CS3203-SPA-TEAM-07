#pragma once

#include <unordered_set>
#include "TNode.h"
#include "Types.hpp"

typedef struct {
    // Reference to this statement (statement number)
    stmt_ref reference;

    // Type of the statement (e.g. read / print / assign)
    stmt_type type;

    // Node in the AST tree where the statement is defined at
    TNode ast;

    // Set of variables used by the statement
    std::unordered_set<var_ref> uses;

    // Set of variables modified by the statement
    std::unordered_set<var_ref> modifies;

    // Reference to the statement that directly nests this statement, or NULL_STMT_REF if the statement has no parent
    stmt_ref parent;

    // Reference to the statement that follows after this statement, or NULL_STMT_REF if there is no such statement
    stmt_ref follows;

    // Set of statements s' such that Parent*(s', this statement)
    std::unordered_set<stmt_ref> indirect_parent;

    // Set of statements s' such that Follows*(this statement, s')
    std::unordered_set<stmt_ref> indirect_follows;

} statement;

typedef struct {
    // struct inheritance
    struct statement;
    // right side of equation, left hand side always one variable.
    // variable assessible with parent class attribute "modifies"
    std::string rightValue;
} assignment;

typedef struct {
    // Reference to this variable (variable name)
    var_ref reference;
} variable;

typedef const_value constant;

typedef struct {
    // Reference to this procedure (procedure name)
    proc_ref reference;

    // List of procedures called by this procedure
    std::unordered_set<proc_ref> children;

    // List of variables used by this procedure (not populated for this iteration)
    std::unordered_set<var_ref> uses;

    // List of variables modified by this procedure (not populated for this iteration)
    std::unordered_set<var_ref> modifies;
} procedure;
