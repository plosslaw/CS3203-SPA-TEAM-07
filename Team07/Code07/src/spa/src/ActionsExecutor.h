#pragma once


#include <string>
#include <vector>

#include "PKB.h"
#include "PKBQueryController.hpp"


class ActionsExecutor {
public:
    enum arg_pos {
        FIRST_ARG,
        SECOND_ARG
    };

    ActionsExecutor();
    explicit ActionsExecutor(PKBQueryController pkb_query_controller);

    /*
     * Basic API
     */

    // Returns true if and only if Follows/*(s1,s2)
    bool is_follows(stmt_ref s1, stmt_ref s2, bool is_starred);

    // Returns true if and only if Parent/*(s1,s2)
    bool is_parent(stmt_ref s1, stmt_ref s2, bool is_starred);

    // Returns true if and only if Uses(s,v)
    bool statement_uses(stmt_ref s, var_ref v);

    // Returns true if and only if Modifies(s,v)
    bool statement_modifies(stmt_ref s, var_ref v);

    // Returns true if and only if assignment statement a matches pattern p
    bool satisfies_pattern(assign_ref a, pattern p);
    
    // Returns list of all constants in the SIMPLE program
    std::vector<constant> get_all_constants();

    // Returns list of all procedures in the SIMPLE program
    std::vector<proc_ref> get_all_procedures();
    
    // Returns list of all statements in the SIMPLE program
    std::vector<statement> get_all_statements();
    
    // Returns list of all statements of type t in the SIMPLE program
    std::vector<stmt_ref> get_all_statements_of_type(stmt_type t);
    
    // Returns list of all variables in the SIMPLE program
    std::vector<var_ref> get_all_variables();

    /*
     * Single Clause API
     */
    
    // Such That Clauses

    std::vector<stmt_ref> get_all_stmts_follows(stmt_type type, arg_pos pos, bool is_starred); // wildcard operation
    std::vector<stmt_ref> get_all_stmts_follows(stmt_type type, arg_pos pos, stmt_ref other_stmt, bool is_starred);
    std::vector<stmt_ref> get_all_stmts_follows(stmt_type type, arg_pos pos, stmt_type other_stmt_type, bool is_starred);
    
    std::vector<stmt_ref> get_all_stmts_parent(stmt_type type, arg_pos pos, bool is_starred); // wildcard operation
    std::vector<stmt_ref> get_all_stmts_parent(stmt_type type, arg_pos pos, stmt_ref other_stmt, bool is_starred);
    std::vector<stmt_ref> get_all_stmts_parent(stmt_type type, arg_pos pos, stmt_type other_stmt_type, bool is_starred);

    std::vector<stmt_ref> get_all_stmts_modifies(var_ref var);
    std::vector<stmt_ref> get_all_stmts_uses(var_ref var);

    std::vector<proc_ref> get_all_procedures_modifies(var_ref var);
    std::vector<proc_ref> get_all_procedures_uses(var_ref var);

    std::vector<var_ref> get_all_variables_modifies(proc_ref procedure);
    std::vector<var_ref> get_all_variables_modifies(stmt_ref other_stmt);
    std::vector<var_ref> get_all_variables_modifies(stmt_type other_stmt_type);

    std::vector<var_ref> get_all_variables_uses(proc_ref procedure);
    std::vector<var_ref> get_all_variables_uses(stmt_ref otherStmt);
    std::vector<var_ref> get_all_variables_uses(stmt_type otherStmtType);

    // Pattern Clauses for Assign

    std::vector<stmt_ref> get_all_stmts_pattern(pattern pattern); // default: assign statements

    std::vector<var_ref> get_all_variables_pattern_assign(arg_pos var_pos); // wildcard operation
    std::vector<var_ref> get_all_variables_pattern_assign(arg_pos var_pos, std::string pattern_string);

    // std::vector<std::string> executeActions(std::vector<Action> queryActions);

private:
    PKBQueryController pkb_query_controller;

};


