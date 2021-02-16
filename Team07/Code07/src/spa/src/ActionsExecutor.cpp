#include "ActionsExecutor.h"

using namespace std;

ActionsExecutor::ActionsExecutor() {}

ActionsExecutor::ActionsExecutor(PKBQueryController pkb_query_controller) {
    this->pkb_query_controller = pkb_query_controller;
}

vector<constant> ActionsExecutor::get_all_constants() {
    return this->pkb_query_controller.getAllConstants();
}

vector<proc_ref> ActionsExecutor::get_all_procedures() {
    return this->pkb_query_controller.getAllProcedures();
}

vector<statement> ActionsExecutor::get_all_statements() {
    return this->pkb_query_controller.getAllStatements();
}

vector<stmt_ref> ActionsExecutor::get_all_statements_of_type(stmt_type type) {
    return this->pkb_query_controller.getStatementsOfType(type);
}

vector<var_ref> ActionsExecutor::get_all_variables() {
    return this->pkb_query_controller.getAllVariables();
}




