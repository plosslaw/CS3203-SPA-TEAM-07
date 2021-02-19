#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include "PKB.h"
#include "PKBQueryController.hpp"
#include "Types.hpp"


class ActionsExecutor {
public:
    /**
     * Constructors
     */

    ActionsExecutor();
    explicit ActionsExecutor(PKBQueryController pkb_query_controller);

    /**
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
    std::vector<const_value> get_all_constants();

    // Returns list of all procedures in the SIMPLE program
    std::vector<proc_ref> get_all_procedures();
    
    // Returns list of all statements in the SIMPLE program
    std::vector<stmt_ref> get_all_statements();
    
    // Returns list of all statements of type t in the SIMPLE program
    std::vector<stmt_ref> get_all_statements_of_type(stmt_type type);
    
    // Returns list of all variables in the SIMPLE program
    std::vector<var_ref> get_all_variables();

    /** 
     * Single Clause API
     */
    
    // Such That Clauses

    std::vector<stmt_ref> get_all_stmts_follows(stmt_type type, arg_pos pos, bool is_starred); // wildcard operation
    std::vector<stmt_ref> get_all_stmts_follows(stmt_type type, arg_pos pos, stmt_ref other_stmt, bool is_starred);
    std::vector<stmt_ref> get_all_stmts_follows(stmt_type type, arg_pos pos, stmt_type other_stmt_type, bool is_starred);
    
    std::vector<stmt_ref> get_all_stmts_parent(stmt_type type, arg_pos pos, bool is_starred); // wildcard operation
    std::vector<stmt_ref> get_all_stmts_parent(stmt_type type, arg_pos pos, stmt_ref other_stmt, bool is_starred);
    std::vector<stmt_ref> get_all_stmts_parent(stmt_type type, arg_pos pos, stmt_type other_stmt_type, bool is_starred);

    std::vector<stmt_ref> get_all_stmts_modifies(stmt_type type); // wildcard operation
    std::vector<stmt_ref> get_all_stmts_modifies(stmt_type type, var_ref var);
    std::vector<stmt_ref> get_all_stmts_uses(stmt_type type); // wildcard operation
    std::vector<stmt_ref> get_all_stmts_uses(stmt_type type, var_ref var);

    std::vector<proc_ref> get_all_procedures_modifies(); // wildcard operation
    std::vector<proc_ref> get_all_procedures_modifies(var_ref var);
    std::vector<proc_ref> get_all_procedures_uses(); // wildcard operation
    std::vector<proc_ref> get_all_procedures_uses(var_ref var);

    std::vector<var_ref> get_all_variables_modifies(); // wildcard operation
    std::vector<var_ref> get_all_variables_modifies(proc_ref procedure);
    std::vector<var_ref> get_all_variables_modifies(stmt_ref other_stmt);
    std::vector<var_ref> get_all_variables_modifies(stmt_type other_stmt_type);

    std::vector<var_ref> get_all_variables_uses(); // wildcard operation
    std::vector<var_ref> get_all_variables_uses(proc_ref procedure);
    std::vector<var_ref> get_all_variables_uses(stmt_ref other_stmt);
    std::vector<var_ref> get_all_variables_uses(stmt_type other_stmt_type);

    // Pattern Clauses for Assign

    std::vector<stmt_ref> get_all_stmts_pattern(pattern pattern); // default: assign statements

    // variable position assumed to be on left side
    std::vector<var_ref> get_all_variables_pattern_assign(); // wildcard operation
    std::vector<var_ref> get_all_variables_pattern_assign(std::string pattern_string);

private:
    PKBQueryController pkb_query_controller;
    std::vector<const_value> consts;
    std::vector<proc_ref> procs;
    std::vector<stmt_ref> stmts;
    std::vector<stmt_ref> stmts_assign;
    std::vector<stmt_ref> stmts_call;
    std::vector<stmt_ref> stmts_if;
    std::vector<stmt_ref> stmts_print;
    std::vector<stmt_ref> stmts_read;
    std::vector<stmt_ref> stmts_while;
    std::vector<var_ref> vars;

};

