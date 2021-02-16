#pragma once

#include <string>
#include <vector>

#include "Action.h"
#include "PKB.h"
#include "PKBQueryController.hpp"
#include "QueryResult.h"


class ActionsExecutor {
public:
    enum arg_pos {
        FIRST_ARG,
        SECOND_ARG
    };

    ActionsExecutor();
    explicit ActionsExecutor(PKBQueryController pkb_query_controller);

    /*
     * Basic api
     */
    
    std::vector<constant> get_all_constants();
    std::vector<proc_ref> get_all_procedures();
    std::vector<statement> get_all_statements();
    std::vector<stmt_ref> get_all_statements_of_type(stmt_type type);
    std::vector<var_ref> get_all_variables();

    /*
     * Single clause api
     */
    
    // Such That Clauses
    std::vector<QueryResult> get_all_stmt_follows(stmt_type type, arg_pos pos, bool is_starred); // wildcard operation
    std::vector<QueryResult> get_all_stmt_follows(stmt_type type, arg_pos pos, stmt_ref other_stmt, bool is_starred);
    std::vector<QueryResult> get_all_stmt_follows(stmt_type type, arg_pos pos, stmt_type other_stmt_type, bool is_starred);
    
    std::vector<QueryResult> get_all_stmt_parent(stmt_type type, arg_pos pos, bool is_starred); // wildcard operation
    std::vector<QueryResult> get_all_stmt_parent(stmt_type type, arg_pos pos, stmt_ref other_stmt, bool is_starred);
    std::vector<QueryResult> get_all_stmt_parent(stmt_type type, arg_pos pos, stmt_type other_stmt_type, bool is_starred);

    std::vector<QueryResult> get_all_stmt_modifies(var_ref var);
    std::vector<QueryResult> get_all_stmt_uses(var_ref var);

    std::vector<QueryResult> get_all_procedure_modifies(var_ref var);
    std::vector<QueryResult> get_all_procedure_uses(var_ref var);

    std::vector<QueryResult> get_all_variable_modifies(proc_ref procedure);
    std::vector<QueryResult> get_all_variable_modifies(stmt_ref other_stmt);
    std::vector<QueryResult> get_all_variable_modifies(stmt_type other_stmt_type);

    std::vector<QueryResult> get_all_variable_uses(proc_ref procedure);
    std::vector<QueryResult> get_all_variable_uses(stmt_ref otherStmt);
    std::vector<QueryResult> get_all_variable_uses(stmt_type otherStmtType);

    // Pattern Clauses for Assign
    std::vector<QueryResult> get_all_stmt_pattern(pattern pattern); // default: assign statements

    std::vector<QueryResult> get_all_variables_pattern(arg_pos var_pos); // wildcard operation
    std::vector<QueryResult> get_all_variables_pattern(arg_pos var_pos, std::string pattern_string);

    // std::vector<std::string> executeActions(std::vector<Action> queryActions);

private:
    PKBQueryController pkb_query_controller;

};



