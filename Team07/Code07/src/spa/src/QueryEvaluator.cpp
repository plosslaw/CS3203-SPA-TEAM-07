#include "QueryEvaluator.h"

using namespace std;

QueryEvaluator::QueryEvaluator(PKBQueryController pkbQueryController) {
    this->actionsExecutor = ActionsExecutor(pkbQueryController);
}

vector<string> QueryEvaluator::QERunQuery(TNode *PQL_AST_Root) {
    vector<Action> queryActions = QEGenerateActions(PQL_AST_Root);
    vector<string> queryResults = QEExecuteActions(queryActions);
    return QERenderResults(queryResults);
}

vector<Action> QueryEvaluator::QEGenerateActions(TNode* PQL_AST_Root) {
    //skeleton code
    vector<Action> actions;
    return actions;
}

vector<string> QueryEvaluator::QEExecuteActions (vector<Action> queryActions) {
    return this->actionsExecutor.executeActions(queryActions);
}

vector<string> QueryEvaluator::QERenderResults (vector<string> queryResults) {
    //skeleton code
    return queryResults;
}

