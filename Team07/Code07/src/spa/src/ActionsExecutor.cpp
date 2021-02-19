#include "ActionsExecutor.h"

using namespace std;

ActionsExecutor::ActionsExecutor() {}

ActionsExecutor::ActionsExecutor(PKBQueryController pkb_query_controller) {
    this->pkb_query_controller = pkb_query_controller;
}

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

bool ActionsExecutor::statement_modifies(stmt_ref s, var_ref v) {
    return this->pkb_query_controller.statementModifies(s, v);
}

bool ActionsExecutor::statement_uses(stmt_ref s, var_ref v) {
    return this->pkb_query_controller.statementUses(s, v);
}

bool ActionsExecutor::satisfies_pattern(assign_ref a, pattern p) {
    return this->pkb_query_controller.satisfiesPattern(a, p);
}

vector<const_value> ActionsExecutor::get_all_constants() {
    return this->pkb_query_controller.getAllConstants();
}

vector<proc_ref> ActionsExecutor::get_all_procedures() {
    return this->pkb_query_controller.getAllProcedures();
}

vector<stmt_ref> ActionsExecutor::get_all_statements() {
    return this->pkb_query_controller.getAllStatements();
}

vector<stmt_ref> ActionsExecutor::get_all_statements_of_type(stmt_type type) {
    return this->pkb_query_controller.getStatementsOfType(type);
}

vector<var_ref> ActionsExecutor::get_all_variables() {
    return this->pkb_query_controller.getAllVariables();
}




