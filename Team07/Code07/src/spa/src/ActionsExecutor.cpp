#include "ActionsExecutor.h"

#include<iostream>

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

    for (stmt_ref assign_stmt : this->stmts_assign) {
        this->unique_set_assign.insert(assign_stmt);
    }
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

    for (stmt_ref ele : this->stmts_assign) {
        cout<<ele<<" ";
    }
    cout<<'\n';

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
    vector<stmt_ref> stmts;
    vector<stmt_ref> results;

    unordered_set<stmt_ref> unique_set;

    switch (type) {
    
    case stmt_type::ASSIGN:
        stmts = this->stmts_assign;
        break;

    case stmt_type::CALL:
        stmts = this->stmts_call;
        break;

    case stmt_type::IF:
        stmts = this->stmts_if;
        break;

    case stmt_type::PRINT:
        stmts = this->stmts_print;
        break;

    case stmt_type::READ:
        stmts = this->stmts_read;
        break;

    case stmt_type::WHILE:
        stmts = this->stmts_while;
        break;

    case stmt_type::STATEMENT:
        stmts = this->stmts;
        break;

    default:
        throw "Invalid statement type passed"; 
    }

    // TODO(plosslaw): refactor to reduce nesting
    if (pos == arg_pos::FIRST_ARG && is_starred) {
        for (stmt_ref stmt_wildcard : this->stmts) {
            for (stmt_ref stmt : stmts) {
                if (pkb_query_controller.isFollowsStar(stmt, stmt_wildcard)) {
                    if (unique_set.find(stmt) == unique_set.end()) {
                        unique_set.insert(stmt);
                        results.push_back(stmt);
                    }
                }
            }
        }
    } else if (pos == arg_pos::FIRST_ARG && !is_starred) {
        for (stmt_ref stmt_wildcard : this->stmts) {
            for (stmt_ref stmt : stmts) {
                if (pkb_query_controller.isFollows(stmt, stmt_wildcard)) {
                    if (unique_set.find(stmt) == unique_set.end()) {
                        unique_set.insert(stmt);
                        results.push_back(stmt);
                    }
                }
            }
        }
    } else if (pos == arg_pos::SECOND_ARG && is_starred) {
        for (stmt_ref stmt_wildcard : this->stmts) {
            for (stmt_ref stmt : stmts) {
                if (pkb_query_controller.isFollowsStar(stmt_wildcard, stmt)) {
                    if (unique_set.find(stmt) == unique_set.end()) {
                        unique_set.insert(stmt);
                        results.push_back(stmt);
                    }
                }
            }
        }
    } else if (pos == arg_pos::SECOND_ARG && !is_starred) {
        for (stmt_ref stmt_wildcard : this->stmts) {
            for (stmt_ref stmt : stmts) {
                if (pkb_query_controller.isFollows(stmt_wildcard, stmt)) {
                    if (unique_set.find(stmt) == unique_set.end()) {
                        unique_set.insert(stmt);
                        results.push_back(stmt);
                    }
                }
            }
        }
    } else {
        throw "Unexpected path";
    }

    return results;
}

std::vector<stmt_ref> ActionsExecutor::get_all_stmts_follows(stmt_type type, arg_pos pos, stmt_ref other_stmt, bool is_starred) {
    vector<stmt_ref> stmts;
    vector<stmt_ref> results;

    unordered_set<stmt_ref> unique_set;

    switch (type) {
    
    case stmt_type::ASSIGN:
        stmts = this->stmts_assign;
        break;

    case stmt_type::CALL:
        stmts = this->stmts_call;
        break;

    case stmt_type::IF:
        stmts = this->stmts_if;
        break;

    case stmt_type::PRINT:
        stmts = this->stmts_print;
        break;

    case stmt_type::READ:
        stmts = this->stmts_read;
        break;

    case stmt_type::WHILE:
        stmts = this->stmts_while;
        break;

    case stmt_type::STATEMENT:
        stmts = this->stmts;
        break;

    default:
        throw "Invalid statement type passed"; 
    }

    // TODO(plosslaw): refactor to reduce nesting
    if (pos == arg_pos::FIRST_ARG && is_starred) {
        for (stmt_ref stmt : stmts) {
            if (pkb_query_controller.isFollowsStar(stmt, other_stmt)) {
                if (unique_set.find(stmt) == unique_set.end()) {
                    unique_set.insert(stmt);
                    results.push_back(stmt);
                }
            }
        }
    } else if (pos == arg_pos::FIRST_ARG && !is_starred) {
        for (stmt_ref stmt : stmts) {
            if (pkb_query_controller.isFollows(stmt, other_stmt)) {
                if (unique_set.find(stmt) == unique_set.end()) {
                    unique_set.insert(stmt);
                    results.push_back(stmt);
                }
            }
        }
    } else if (pos == arg_pos::SECOND_ARG && is_starred) {
        for (stmt_ref stmt : stmts) {
            if (pkb_query_controller.isFollowsStar(other_stmt, stmt)) {
                if (unique_set.find(stmt) == unique_set.end()) {
                    unique_set.insert(stmt);
                    results.push_back(stmt);
                }
            }
        }
    } else if (pos == arg_pos::SECOND_ARG && !is_starred) {
        for (stmt_ref stmt : stmts) {
            if (pkb_query_controller.isFollows(other_stmt, stmt)) {
                if (unique_set.find(stmt) == unique_set.end()) {
                    unique_set.insert(stmt);
                    results.push_back(stmt);
                }
            }
        }
    } else {
        throw "Unexpected path";
    }

    return results;
}

std::vector<stmt_ref> ActionsExecutor::get_all_stmts_follows(stmt_type type, arg_pos pos, stmt_type other_stmt_type, bool is_starred) {
    vector<stmt_ref> stmts;
    vector<stmt_ref> stmts_wildcard = this->get_all_statements_of_type(other_stmt_type);
    vector<stmt_ref> results;

    unordered_set<stmt_ref> unique_set;

    switch (type) {
    
    case stmt_type::ASSIGN:
        stmts = this->stmts_assign;
        break;

    case stmt_type::CALL:
        stmts = this->stmts_call;
        break;

    case stmt_type::IF:
        stmts = this->stmts_if;
        break;

    case stmt_type::PRINT:
        stmts = this->stmts_print;
        break;

    case stmt_type::READ:
        stmts = this->stmts_read;
        break;

    case stmt_type::WHILE:
        stmts = this->stmts_while;
        break;

    case stmt_type::STATEMENT:
        stmts = this->stmts;
        break;

    default:
        throw "Invalid statement type passed"; 
    }

    // TODO(plosslaw): refactor to reduce nesting
    if (pos == arg_pos::FIRST_ARG && is_starred) {
        for (stmt_ref stmt_wildcard : stmts_wildcard) {
            for (stmt_ref stmt : stmts) {
                if (pkb_query_controller.isFollowsStar(stmt, stmt_wildcard)) {
                    if (unique_set.find(stmt) == unique_set.end()) {
                        unique_set.insert(stmt);
                        results.push_back(stmt);
                    }
                }
            }
        }
    } else if (pos == arg_pos::FIRST_ARG && !is_starred) {
        for (stmt_ref stmt_wildcard : stmts_wildcard) {
            for (stmt_ref stmt : stmts) {
                if (pkb_query_controller.isFollows(stmt, stmt_wildcard)) {
                    if (unique_set.find(stmt) == unique_set.end()) {
                        unique_set.insert(stmt);
                        results.push_back(stmt);
                    }
                }
            }
        }
    } else if (pos == arg_pos::SECOND_ARG && is_starred) {
        for (stmt_ref stmt_wildcard : stmts_wildcard) {
            for (stmt_ref stmt : stmts) {
                if (pkb_query_controller.isFollowsStar(stmt_wildcard, stmt)) {
                    if (unique_set.find(stmt) == unique_set.end()) {
                        unique_set.insert(stmt);
                        results.push_back(stmt);
                    }
                }
            }
        }
    } else if (pos == arg_pos::SECOND_ARG && !is_starred) {
        for (stmt_ref stmt_wildcard : stmts_wildcard) {
            for (stmt_ref stmt : stmts) {
                if (pkb_query_controller.isFollows(stmt_wildcard, stmt)) {
                    if (unique_set.find(stmt) == unique_set.end()) {
                        unique_set.insert(stmt);
                        results.push_back(stmt);
                    }
                }
            }
        }
    } else {
        throw "Unexpected path";
    }

    return results;
}

// Wildcard operation
std::vector<stmt_ref> ActionsExecutor::get_all_stmts_parent(stmt_type type, arg_pos pos, bool is_starred) {
    vector<stmt_ref> stmts;
    vector<stmt_ref> results;

    unordered_set<stmt_ref> unique_set;

    switch (type) {
    
    case stmt_type::ASSIGN:
        stmts = this->stmts_assign;
        break;

    case stmt_type::CALL:
        stmts = this->stmts_call;
        break;

    case stmt_type::IF:
        stmts = this->stmts_if;
        break;

    case stmt_type::PRINT:
        stmts = this->stmts_print;
        break;

    case stmt_type::READ:
        stmts = this->stmts_read;
        break;

    case stmt_type::WHILE:
        stmts = this->stmts_while;
        break;

    case stmt_type::STATEMENT:
        stmts = this->stmts;
        break;

    default:
        throw "Invalid statement type passed"; 
    }

    // TODO(plosslaw): refactor to reduce nesting
    if (pos == arg_pos::FIRST_ARG && is_starred) {
        for (stmt_ref stmt_wildcard : this->stmts) {
            for (stmt_ref stmt : stmts) {
                if (pkb_query_controller.isParentStar(stmt, stmt_wildcard)) {
                    if (unique_set.find(stmt) == unique_set.end()) {
                        unique_set.insert(stmt);
                        results.push_back(stmt);
                    }
                }
            }
        }
    } else if (pos == arg_pos::FIRST_ARG && !is_starred) {
        for (stmt_ref stmt_wildcard : this->stmts) {
            for (stmt_ref stmt : stmts) {
                if (pkb_query_controller.isParent(stmt, stmt_wildcard)) {
                    if (unique_set.find(stmt) == unique_set.end()) {
                        unique_set.insert(stmt);
                        results.push_back(stmt);
                    }
                }
            }
        }
    } else if (pos == arg_pos::SECOND_ARG && is_starred) {
        for (stmt_ref stmt_wildcard : this->stmts) {
            for (stmt_ref stmt : stmts) {
                if (pkb_query_controller.isParentStar(stmt_wildcard, stmt)) {
                    if (unique_set.find(stmt) == unique_set.end()) {
                        unique_set.insert(stmt);
                        results.push_back(stmt);
                    }
                }
            }
        }
    } else if (pos == arg_pos::SECOND_ARG && !is_starred) {
        for (stmt_ref stmt_wildcard : this->stmts) {
            for (stmt_ref stmt : stmts) {
                if (pkb_query_controller.isParent(stmt_wildcard, stmt)) {
                    if (unique_set.find(stmt) == unique_set.end()) {
                        unique_set.insert(stmt);
                        results.push_back(stmt);
                    }
                }
            }
        }
    } else {
        throw "Unexpected path";
    }

    return results;
}

std::vector<stmt_ref> ActionsExecutor::get_all_stmts_parent(stmt_type type, arg_pos pos, stmt_ref other_stmt, bool is_starred) {
    vector<stmt_ref> stmts;
    vector<stmt_ref> results;

    unordered_set<stmt_ref> unique_set;

    switch (type) {
    
    case stmt_type::ASSIGN:
        stmts = this->stmts_assign;
        break;

    case stmt_type::CALL:
        stmts = this->stmts_call;
        break;

    case stmt_type::IF:
        stmts = this->stmts_if;
        break;

    case stmt_type::PRINT:
        stmts = this->stmts_print;
        break;

    case stmt_type::READ:
        stmts = this->stmts_read;
        break;

    case stmt_type::WHILE:
        stmts = this->stmts_while;
        break;

    case stmt_type::STATEMENT:
        stmts = this->stmts;
        break;

    default:
        throw "Invalid statement type passed"; 
    }

    // TODO(plosslaw): refactor to reduce nesting
    if (pos == arg_pos::FIRST_ARG && is_starred) {
        for (stmt_ref stmt : stmts) {
            if (pkb_query_controller.isParentStar(stmt, other_stmt)) {
                if (unique_set.find(stmt) == unique_set.end()) {
                    unique_set.insert(stmt);
                    results.push_back(stmt);
                }
            }
        }
    } else if (pos == arg_pos::FIRST_ARG && !is_starred) {
        for (stmt_ref stmt : stmts) {
            if (pkb_query_controller.isParent(stmt, other_stmt)) {
                if (unique_set.find(stmt) == unique_set.end()) {
                    unique_set.insert(stmt);
                    results.push_back(stmt);
                }
            }
        }
    } else if (pos == arg_pos::SECOND_ARG && is_starred) {
        for (stmt_ref stmt : stmts) {
            if (pkb_query_controller.isParentStar(other_stmt, stmt)) {
                if (unique_set.find(stmt) == unique_set.end()) {
                    unique_set.insert(stmt);
                    results.push_back(stmt);
                }
            }
        }
    } else if (pos == arg_pos::SECOND_ARG && !is_starred) {
        for (stmt_ref stmt : stmts) {
            if (pkb_query_controller.isParent(other_stmt, stmt)) {
                if (unique_set.find(stmt) == unique_set.end()) {
                    unique_set.insert(stmt);
                    results.push_back(stmt);
                }
            }
        }
    } else {
        throw "Unexpected path";
    }

    return results;
}

std::vector<stmt_ref> ActionsExecutor::get_all_stmts_parent(stmt_type type, arg_pos pos, stmt_type other_stmt_type, bool is_starred) {
    vector<stmt_ref> stmts;
    vector<stmt_ref> stmts_wildcard = this->get_all_statements_of_type(other_stmt_type);
    vector<stmt_ref> results;

    unordered_set<stmt_ref> unique_set;

    switch (type) {
    
    case stmt_type::ASSIGN:
        stmts = this->stmts_assign;
        break;

    case stmt_type::CALL:
        stmts = this->stmts_call;
        break;

    case stmt_type::IF:
        stmts = this->stmts_if;
        break;

    case stmt_type::PRINT:
        stmts = this->stmts_print;
        break;

    case stmt_type::READ:
        stmts = this->stmts_read;
        break;

    case stmt_type::WHILE:
        stmts = this->stmts_while;
        break;

    case stmt_type::STATEMENT:
        stmts = this->stmts;
        break;

    default:
        throw "Invalid statement type passed"; 
    }

    // TODO(plosslaw): refactor to reduce nesting
    if (pos == arg_pos::FIRST_ARG && is_starred) {
        for (stmt_ref stmt_wildcard : stmts_wildcard) {
            for (stmt_ref stmt : stmts) {
                if (pkb_query_controller.isParentStar(stmt, stmt_wildcard)) {
                    if (unique_set.find(stmt) == unique_set.end()) {
                        unique_set.insert(stmt);
                        results.push_back(stmt);
                    }
                }
            }
        }
    } else if (pos == arg_pos::FIRST_ARG && !is_starred) {
        for (stmt_ref stmt_wildcard : stmts_wildcard) {
            for (stmt_ref stmt : stmts) {
                if (pkb_query_controller.isParent(stmt, stmt_wildcard)) {
                    if (unique_set.find(stmt) == unique_set.end()) {
                        unique_set.insert(stmt);
                        results.push_back(stmt);
                    }
                }
            }
        }
    } else if (pos == arg_pos::SECOND_ARG && is_starred) {
        for (stmt_ref stmt_wildcard : stmts_wildcard) {
            for (stmt_ref stmt : stmts) {
                if (pkb_query_controller.isParentStar(stmt_wildcard, stmt)) {
                    if (unique_set.find(stmt) == unique_set.end()) {
                        unique_set.insert(stmt);
                        results.push_back(stmt);
                    }
                }
            }
        }
    } else if (pos == arg_pos::SECOND_ARG && !is_starred) {
        for (stmt_ref stmt_wildcard : stmts_wildcard) {
            for (stmt_ref stmt : stmts) {
                if (pkb_query_controller.isParent(stmt_wildcard, stmt)) {
                    if (unique_set.find(stmt) == unique_set.end()) {
                        unique_set.insert(stmt);
                        results.push_back(stmt);
                    }
                }
            }
        }
    } else {
        throw "Unexpected path";
    }

    return results;
}

// Wildcard operation
std::vector<stmt_ref> ActionsExecutor::get_all_stmts_modifies(stmt_type type) {
    vector<stmt_ref> stmts;
    vector<stmt_ref> results;

    unordered_set<stmt_ref> unique_set;

    switch (type) {
    
    case stmt_type::ASSIGN:
        stmts = this->stmts_assign;
        break;

    case stmt_type::CALL:
        stmts = this->stmts_call;
        break;

    case stmt_type::IF:
        stmts = this->stmts_if;
        break;

    case stmt_type::PRINT:
        stmts = this->stmts_print;
        break;

    case stmt_type::READ:
        stmts = this->stmts_read;
        break;

    case stmt_type::WHILE:
        stmts = this->stmts_while;
        break;

    case stmt_type::STATEMENT:
        stmts = this->stmts;
        break;

    default:
        throw "Invalid statement type passed"; 
    }

    // TODO(plosslaw): refactor to reduce nesting
    for (var_ref var_wildcard : this->vars) {
        for (stmt_ref stmt : stmts) {
            if (pkb_query_controller.statementModifies(stmt, var_wildcard)) {
                if (unique_set.find(stmt) == unique_set.end()) {
                    unique_set.insert(stmt);
                    results.push_back(stmt);
                }
            }
        }
    }

    return results;
}

std::vector<stmt_ref> ActionsExecutor::get_all_stmts_modifies(stmt_type type, var_ref var) {
    vector<stmt_ref> stmts;
    vector<stmt_ref> results;

    unordered_set<stmt_ref> unique_set;

    switch (type) {
    
    case stmt_type::ASSIGN:
        stmts = this->stmts_assign;
        break;

    case stmt_type::CALL:
        stmts = this->stmts_call;
        break;

    case stmt_type::IF:
        stmts = this->stmts_if;
        break;

    case stmt_type::PRINT:
        stmts = this->stmts_print;
        break;

    case stmt_type::READ:
        stmts = this->stmts_read;
        break;

    case stmt_type::WHILE:
        stmts = this->stmts_while;
        break;

    case stmt_type::STATEMENT:
        stmts = this->stmts;
        break;

    default:
        throw "Invalid statement type passed"; 
    }

    // TODO(plosslaw): refactor to reduce nesting
    for (stmt_ref stmt : stmts) {
        if (pkb_query_controller.statementModifies(stmt, var)) {
            if (unique_set.find(stmt) == unique_set.end()) {
                unique_set.insert(stmt);
                results.push_back(stmt);
            }
        }
    }

    return results;
}

// Wildcard operation
std::vector<stmt_ref> ActionsExecutor::get_all_stmts_uses(stmt_type type) {
    vector<stmt_ref> stmts;
    vector<stmt_ref> results;

    unordered_set<stmt_ref> unique_set;

    switch (type) {
    
    case stmt_type::ASSIGN:
        stmts = this->stmts_assign;
        break;

    case stmt_type::CALL:
        stmts = this->stmts_call;
        break;

    case stmt_type::IF:
        stmts = this->stmts_if;
        break;

    case stmt_type::PRINT:
        stmts = this->stmts_print;
        break;

    case stmt_type::READ:
        stmts = this->stmts_read;
        break;

    case stmt_type::WHILE:
        stmts = this->stmts_while;
        break;

    case stmt_type::STATEMENT:
        stmts = this->stmts;
        break;

    default:
        throw "Invalid statement type passed"; 
    }

    // TODO(plosslaw): refactor to reduce nesting
    for (var_ref var_wildcard : this->vars) {
        for (stmt_ref stmt : stmts) {
            if (pkb_query_controller.statementUses(stmt, var_wildcard)) {
                if (unique_set.find(stmt) == unique_set.end()) {
                    unique_set.insert(stmt);
                    results.push_back(stmt);
                }
            }
        }
    }

    return results;
}

std::vector<stmt_ref> ActionsExecutor::get_all_stmts_uses(stmt_type type, var_ref var) {
    vector<stmt_ref> stmts;
    vector<stmt_ref> results;

    unordered_set<stmt_ref> unique_set;

    switch (type) {
    
    case stmt_type::ASSIGN:
        stmts = this->stmts_assign;
        break;

    case stmt_type::CALL:
        stmts = this->stmts_call;
        break;

    case stmt_type::IF:
        stmts = this->stmts_if;
        break;

    case stmt_type::PRINT:
        stmts = this->stmts_print;
        break;

    case stmt_type::READ:
        stmts = this->stmts_read;
        break;

    case stmt_type::WHILE:
        stmts = this->stmts_while;
        break;

    case stmt_type::STATEMENT:
        stmts = this->stmts;
        break;

    default:
        throw "Invalid statement type passed"; 
    }

    // TODO(plosslaw): refactor to reduce nesting
    for (stmt_ref stmt : stmts) {
        if (pkb_query_controller.statementUses(stmt, var)) {
            if (unique_set.find(stmt) == unique_set.end()) {
                unique_set.insert(stmt);
                results.push_back(stmt);
            }
        }
    }

    return results;
}

// Wildcard operation
std::vector<proc_ref> ActionsExecutor::get_all_procedures_modifies() {
    vector<proc_ref> results;

    unordered_set<proc_ref> unique_set;

    // TODO(plosslaw): refactor to reduce nesting
    for (var_ref var_wildcard : this->vars) {
        for (proc_ref proc : this->procs) {
            if (pkb_query_controller.procedureModifies(proc, var_wildcard)) {
                if (unique_set.find(proc) == unique_set.end()) {
                    unique_set.insert(proc);
                    results.push_back(proc);
                }
            }
        }
    }

    return results;
}

std::vector<proc_ref> ActionsExecutor::get_all_procedures_modifies(var_ref var) {
    vector<proc_ref> results;

    unordered_set<proc_ref> unique_set;

    // TODO(plosslaw): refactor to reduce nesting
    for (proc_ref proc : this->procs) {
        if (pkb_query_controller.procedureModifies(proc, var)) {
            if (unique_set.find(proc) == unique_set.end()) {
                unique_set.insert(proc);
                results.push_back(proc);
            }
        }
    }

    return results;
}

// Wildcard operation
std::vector<proc_ref> ActionsExecutor::get_all_procedures_uses() {
    vector<proc_ref> results;

    unordered_set<proc_ref> unique_set;

    // TODO(plosslaw): refactor to reduce nesting
    for (var_ref var_wildcard : this->vars) {
        for (proc_ref proc : this->procs) {
            if (pkb_query_controller.procedureUses(proc, var_wildcard)) {
                if (unique_set.find(proc) == unique_set.end()) {
                    unique_set.insert(proc);
                    results.push_back(proc);
                }
            }
        }
    }

    return results;
}

std::vector<proc_ref> ActionsExecutor::get_all_procedures_uses(var_ref var) {
    vector<proc_ref> results;

    unordered_set<proc_ref> unique_set;

    // TODO(plosslaw): refactor to reduce nesting
    for (proc_ref proc : this->procs) {
        if (pkb_query_controller.procedureUses(proc, var)) {
            if (unique_set.find(proc) == unique_set.end()) {
                unique_set.insert(proc);
                results.push_back(proc);
            }
        }
    }

    return results;
}

// Wildcard operation
std::vector<var_ref> ActionsExecutor::get_all_variables_modifies() {
    vector<var_ref> results;

    unordered_set<var_ref> unique_set;

    // TODO(plosslaw): refactor to reduce nesting
    for (proc_ref proc_wildcard : this->procs) {
        for (var_ref var : this->vars) {
            if (pkb_query_controller.procedureModifies(proc_wildcard, var)) {
                if (unique_set.find(var) == unique_set.end()) {
                    unique_set.insert(var);
                    results.push_back(var);
                }
            }
        }
    }

    // TODO(plosslaw): refactor to reduce nesting
    for (stmt_ref stmt_wildcard : this->stmts) {
        for (var_ref var : this->vars) {
            if (pkb_query_controller.statementModifies(stmt_wildcard, var)) {
                if (unique_set.find(var) == unique_set.end()) {
                    unique_set.insert(var);
                    results.push_back(var);
                }
            }
        }
    }

    return results;
}

std::vector<var_ref> ActionsExecutor::get_all_variables_modifies(proc_ref procedure) {
    vector<var_ref> results;

    unordered_set<var_ref> unique_set;

    // TODO(plosslaw): refactor to reduce nesting
    for (var_ref var : this->vars) {
        if (pkb_query_controller.procedureModifies(procedure, var)) {
            if (unique_set.find(var) == unique_set.end()) {
                unique_set.insert(var);
                results.push_back(var);
            }
        }
    }

    return results;
}

std::vector<var_ref> ActionsExecutor::get_all_variables_modifies(stmt_ref other_stmt) {
    vector<var_ref> results;

    unordered_set<var_ref> unique_set;

    // TODO(plosslaw): refactor to reduce nesting
    for (var_ref var : this->vars) {
        if (pkb_query_controller.statementModifies(other_stmt, var)) {
            if (unique_set.find(var) == unique_set.end()) {
                unique_set.insert(var);
                results.push_back(var);
            }
        }
    }

    return results;
}

std::vector<var_ref> ActionsExecutor::get_all_variables_modifies(stmt_type other_stmt_type) {
    vector<var_ref> results;

    unordered_set<var_ref> unique_set;

    // TODO(plosslaw): refactor to reduce nesting
    if (other_stmt_type == stmt_type::PROCEDURE) {
        for (proc_ref proc_wildcard : this->procs) {
            for (var_ref var : this->vars) {
                if (pkb_query_controller.procedureModifies(proc_wildcard, var)) {
                    if (unique_set.find(var) == unique_set.end()) {
                        unique_set.insert(var);
                        results.push_back(var);
                    }
                }
            }
        }
        
        return results;
    }

    vector<stmt_ref> stmts_wildcard;
    
    switch (other_stmt_type) {
    
    case stmt_type::ASSIGN:
        stmts_wildcard = this->stmts_assign;
        break;

    case stmt_type::CALL:
        stmts_wildcard = this->stmts_call;
        break;
    
    case stmt_type::IF:
        stmts_wildcard = this->stmts_if;
        break;

    case stmt_type::PRINT:
        stmts_wildcard = this->stmts_print;
        break;

    case stmt_type::READ:
        stmts_wildcard = this->stmts_read;
        break;

    case stmt_type::WHILE:
        stmts_wildcard = this->stmts_while;
        break;

    case stmt_type::STATEMENT:
        stmts_wildcard = this->stmts;
        break;

    default:
        throw "Invalid statement type passed"; 
    }

    // TODO(plosslaw): refactor to reduce nesting
    for (stmt_ref stmt_wildcard : stmts_wildcard) {
        for (var_ref var : this->vars) {
            if (pkb_query_controller.statementModifies(stmt_wildcard, var)) {
                if (unique_set.find(var) == unique_set.end()) {
                    unique_set.insert(var);
                    results.push_back(var);
                }
            }
        }
    }

    return results;
}

// Wildcard operation
std::vector<var_ref> ActionsExecutor::get_all_variables_uses() {
    vector<var_ref> results;

    unordered_set<var_ref> unique_set;

    // TODO(plosslaw): refactor to reduce nesting
    for (proc_ref proc_wildcard : this->procs) {
        for (var_ref var : this->vars) {
            if (pkb_query_controller.procedureUses(proc_wildcard, var)) {
                if (unique_set.find(var) == unique_set.end()) {
                    unique_set.insert(var);
                    results.push_back(var);
                }
            }
        }
    }

    // TODO(plosslaw): refactor to reduce nesting
    for (stmt_ref stmt_wildcard : this->stmts) {
        for (var_ref var : this->vars) {
            if (pkb_query_controller.statementUses(stmt_wildcard, var)) {
                if (unique_set.find(var) == unique_set.end()) {
                    unique_set.insert(var);
                    results.push_back(var);
                }
            }
        }
    }

    return results;
}

std::vector<var_ref> ActionsExecutor::get_all_variables_uses(proc_ref procedure) {
    vector<var_ref> results;

    unordered_set<var_ref> unique_set;

    // TODO(plosslaw): refactor to reduce nesting
    for (var_ref var : this->vars) {
        if (pkb_query_controller.procedureUses(procedure, var)) {
            if (unique_set.find(var) == unique_set.end()) {
                unique_set.insert(var);
                results.push_back(var);
            }
        }
    }

    return results;
}

std::vector<var_ref> ActionsExecutor::get_all_variables_uses(stmt_ref other_stmt) {
    vector<var_ref> results;

    unordered_set<var_ref> unique_set;

    // TODO(plosslaw): refactor to reduce nesting
    for (var_ref var : this->vars) {
        if (pkb_query_controller.statementUses(other_stmt, var)) {
            if (unique_set.find(var) == unique_set.end()) {
                unique_set.insert(var);
                results.push_back(var);
            }
        }
    }

    return results;
}

std::vector<var_ref> ActionsExecutor::get_all_variables_uses(stmt_type other_stmt_type) {
    vector<var_ref> results;

    unordered_set<var_ref> unique_set;

    // TODO(plosslaw): refactor to reduce nesting
    if (other_stmt_type == stmt_type::PROCEDURE) {
        for (proc_ref proc_wildcard : this->procs) {
            for (var_ref var : this->vars) {
                if (pkb_query_controller.procedureUses(proc_wildcard, var)) {
                    if (unique_set.find(var) == unique_set.end()) {
                        unique_set.insert(var);
                        results.push_back(var);
                    }
                }
            }
        }
        
        return results;
    }

    vector<stmt_ref> stmts_wildcard;
    
    switch (other_stmt_type) {
    
    case stmt_type::ASSIGN:
        stmts_wildcard = this->stmts_assign;
        break;

    case stmt_type::CALL:
        stmts_wildcard = this->stmts_call;
        break;
    
    case stmt_type::IF:
        stmts_wildcard = this->stmts_if;
        break;

    case stmt_type::PRINT:
        stmts_wildcard = this->stmts_print;
        break;

    case stmt_type::READ:
        stmts_wildcard = this->stmts_read;
        break;

    case stmt_type::WHILE:
        stmts_wildcard = this->stmts_while;
        break;

    case stmt_type::STATEMENT:
        stmts_wildcard = this->stmts;
        break;

    default:
        throw "Invalid statement type passed"; 
    }

    // TODO(plosslaw): refactor to reduce nesting
    for (stmt_ref stmt_wildcard : stmts_wildcard) {
        for (var_ref var : this->vars) {
            if (pkb_query_controller.statementUses(stmt_wildcard, var)) {
                if (unique_set.find(var) == unique_set.end()) {
                    unique_set.insert(var);
                    results.push_back(var);
                }
            }
        }
    }

    return results;
}

// Pattern Clauses for Assign

// Default: Assign statements
std::vector<stmt_ref> ActionsExecutor::get_all_stmts_pattern(pattern pattern) {
    vector<stmt_ref> results;

    unordered_set<stmt_ref> unique_set;

    // TODO(plosslaw): refactor to reduce nesting
    for (stmt_ref stmt : this->stmts_assign) {
        if (pkb_query_controller.satisfiesPattern(stmt, pattern)) {
            if (unique_set.find(stmt) == unique_set.end()) {
                unique_set.insert(stmt);
                results.push_back(stmt);
            }
        }
    }
    
    return results;
} 

// Variable position assumed to be on left side
// Wildcard operation
std::vector<var_ref> ActionsExecutor::get_all_variables_pattern_assign() {
    vector<var_ref> results;

    unordered_set<var_ref> unique_set;
    
    pattern trial_pattern;
    trial_pattern.rvalue = "_";

    // TODO(plosslaw): refactor to reduce nesting
    for (stmt_ref stmt_wildcard : this->stmts_assign) {
        for (var_ref var : this->vars) {
            trial_pattern.lvalue = var;
            if (pkb_query_controller.satisfiesPattern(stmt_wildcard, trial_pattern)) {
                if (unique_set.find(var) == unique_set.end()) {
                    unique_set.insert(var);
                    results.push_back(var);
                }
            }
        }
    }
    
    return results;
} 

std::vector<var_ref> ActionsExecutor::get_all_variables_pattern_assign(std::string pattern_string) {
    vector<var_ref> results;

    unordered_set<var_ref> unique_set;

    pattern trial_pattern;
    trial_pattern.rvalue = pattern_string;

    // TODO(plosslaw): refactor to reduce nesting
    for (stmt_ref stmt_wildcard : this->stmts_assign) {
        for (var_ref var : this->vars) {
            trial_pattern.lvalue = var;
            if (pkb_query_controller.satisfiesPattern(stmt_wildcard, trial_pattern)) {
                if (unique_set.find(var) == unique_set.end()) {
                    unique_set.insert(var);
                    results.push_back(var);
                }
            }
        }
    }
    
    return results;
}

std::vector<var_ref> ActionsExecutor::get_variable_pattern_assign(stmt_ref assign_stmt, std::string pattern_string) {
    vector<var_ref> results;

    if (this->unique_set_assign.find(assign_stmt) == this->unique_set_assign.end()) {
        // assign_stmt passed is not found in list of assignments
        return results;
    }

    pattern trial_pattern;
    trial_pattern.rvalue = pattern_string;
    for (var_ref var : this->vars) {
        trial_pattern.lvalue = var;
        if (pkb_query_controller.satisfiesPattern(assign_stmt, trial_pattern)) {
            results.push_back(var);
            break;
        }
    }
    return results;
}



