#include "ActionsExecutor.h"

using namespace std;

/**
 * Constructors
 */

ActionsExecutor::ActionsExecutor() {}

ActionsExecutor::ActionsExecutor(PKBQueryController pkb_query_controller) {
    this->pkb_query_controller = pkb_query_controller;
    this->consts = pkb_query_controller.getAllConstants();
    this->procs = pkb_query_controller.getAllProcedures();
    this->stmts = pkb_query_controller.getAllStatements();
    this->stmts_assign = pkb_query_controller.getStatementsOfType(stmt_type::ASSIGN);
    this->stmts_call = pkb_query_controller.getStatementsOfType(stmt_type::CALL);
    this->stmts_if = pkb_query_controller.getStatementsOfType(stmt_type::IF);
    this->stmts_print = pkb_query_controller.getStatementsOfType(stmt_type::PRINT);
    this->stmts_read = pkb_query_controller.getStatementsOfType(stmt_type::READ);
    this->stmts_while = pkb_query_controller.getStatementsOfType(stmt_type::WHILE);
    this->vars = pkb_query_controller.getAllVariables();
}

/**
 * Basic API
 */

bool ActionsExecutor::is_follows(stmt_ref s1, stmt_ref s2, bool is_starred) {
    if (!is_starred) {
        return this->pkb_query_controller.isFollows(s1, s2);
    }
    return this->pkb_query_controller.isFollowsStar(s1, s2);
}

bool ActionsExecutor::is_parent(stmt_ref s1, stmt_ref s2, bool is_starred) {
    if (!is_starred) {
        return this->pkb_query_controller.isParent(s1, s2);
    }
    return this->pkb_query_controller.isParentStar(s1, s2);
}

bool ActionsExecutor::statement_uses(stmt_ref s, var_ref v) {
    return this->pkb_query_controller.statementUses(s, v);
}

bool ActionsExecutor::statement_modifies(stmt_ref s, var_ref v) {
    return this->pkb_query_controller.statementModifies(s, v);
}

bool ActionsExecutor::satisfies_pattern(assign_ref a, pattern p) {
    return this->pkb_query_controller.satisfiesPattern(a, p);
}

vector<const_value> ActionsExecutor::get_all_constants() {
    return this->consts;
}

vector<proc_ref> ActionsExecutor::get_all_procedures() {
    return this->procs;
}

vector<stmt_ref> ActionsExecutor::get_all_statements() {
    return this->stmts;
}

vector<stmt_ref> ActionsExecutor::get_all_statements_of_type(stmt_type type) {
    switch (type) {
    
    case stmt_type::ASSIGN:
        return this->stmts_assign;

    case stmt_type::CALL:
        return this->stmts_call;
    
    case stmt_type::IF:
        return this->stmts_if;

    case stmt_type::PRINT:
        return this->stmts_print;

    case stmt_type::READ:
        return this->stmts_read;

    case stmt_type::WHILE:
        return this->stmts_while;

    case stmt_type::STATEMENT:
        return this->stmts;

    default:
        throw "Invalid statement type passed"; 
    }
}

vector<var_ref> ActionsExecutor::get_all_variables() {
    return this->vars;
}

/**
 * Single Clause API
 */

// Such That Clauses

// Wildcard operation
std::vector<stmt_ref> ActionsExecutor::get_all_stmts_follows(stmt_type type, arg_pos pos, bool is_starred) {
    vector<stmt_ref> results;
    return results;
}

std::vector<stmt_ref> ActionsExecutor::get_all_stmts_follows(stmt_type type, arg_pos pos, stmt_ref other_stmt, bool is_starred) {
    vector<stmt_ref> results;
    return results;
}

std::vector<stmt_ref> ActionsExecutor::get_all_stmts_follows(stmt_type type, arg_pos pos, stmt_type other_stmt_type, bool is_starred) {
    vector<stmt_ref> results;
    return results;
}

// Wildcard operation
std::vector<stmt_ref> ActionsExecutor::get_all_stmts_parent(stmt_type type, arg_pos pos, bool is_starred) {
    vector<stmt_ref> results;
    return results;
}

std::vector<stmt_ref> ActionsExecutor::get_all_stmts_parent(stmt_type type, arg_pos pos, stmt_ref other_stmt, bool is_starred) {
    vector<stmt_ref> results;
    return results;
}

std::vector<stmt_ref> ActionsExecutor::get_all_stmts_parent(stmt_type type, arg_pos pos, stmt_type other_stmt_type, bool is_starred) {
    vector<stmt_ref> results;
    return results;
}

// Wildcard operation
std::vector<stmt_ref> ActionsExecutor::get_all_stmts_modifies(stmt_type type) {
    vector<stmt_ref> results;
    return results;
}

std::vector<stmt_ref> ActionsExecutor::get_all_stmts_modifies(stmt_type type, var_ref var) {
    vector<stmt_ref> results;
    return results;
}

// Wildcard operation
std::vector<stmt_ref> ActionsExecutor::get_all_stmts_uses(stmt_type type) {
    vector<stmt_ref> results;
    return results;
}

std::vector<stmt_ref> ActionsExecutor::get_all_stmts_uses(stmt_type type, var_ref var) {
    vector<stmt_ref> results;
    return results;
}

// Wildcard operation
std::vector<proc_ref> ActionsExecutor::get_all_procedures_modifies() {
    vector<proc_ref> results;
    return results;
}

std::vector<proc_ref> ActionsExecutor::get_all_procedures_modifies(var_ref var) {
    vector<proc_ref> results;
    return results;
}

// Wildcard operation
std::vector<proc_ref> ActionsExecutor::get_all_procedures_uses() {
    vector<proc_ref> results;
    return results;
}

std::vector<proc_ref> ActionsExecutor::get_all_procedures_uses(var_ref var) {
    vector<proc_ref> results;
    return results;
}

// Wildcard operation
std::vector<var_ref> ActionsExecutor::get_all_variables_modifies() {
    vector<var_ref> results;
    return results;
}

std::vector<var_ref> ActionsExecutor::get_all_variables_modifies(proc_ref procedure) {
    vector<var_ref> results;
    return results;
}

std::vector<var_ref> ActionsExecutor::get_all_variables_modifies(stmt_ref other_stmt) {
    vector<var_ref> results;
    return results;
}

std::vector<var_ref> ActionsExecutor::get_all_variables_modifies(stmt_type other_stmt_type) {
    vector<var_ref> results;
    return results;
}

// Wildcard operation
std::vector<var_ref> ActionsExecutor::get_all_variables_uses() {
    vector<var_ref> results;
    return results;
}

std::vector<var_ref> ActionsExecutor::get_all_variables_uses(proc_ref procedure) {
    vector<var_ref> results;
    return results;
}

std::vector<var_ref> ActionsExecutor::get_all_variables_uses(stmt_ref otherStmt) {
    vector<var_ref> results;
    return results;
}

std::vector<var_ref> ActionsExecutor::get_all_variables_uses(stmt_type otherStmtType) {
    vector<var_ref> results;
    return results;
}

// Pattern Clauses for Assign

// Default: Assign statements
std::vector<stmt_ref> ActionsExecutor::get_all_stmts_pattern(pattern pattern) {
    vector<stmt_ref> results;
    return results;
} 

// Variable position assumed to be on left side
// Wildcard operation
std::vector<var_ref> ActionsExecutor::get_all_variables_pattern_assign() {
    vector<var_ref> results;
    return results;
} 

std::vector<var_ref> ActionsExecutor::get_all_variables_pattern_assign(std::string pattern_string) {
    vector<var_ref> results;
    return results;
}



