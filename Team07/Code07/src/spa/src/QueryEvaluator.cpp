#include "QueryEvaluator.h"

using namespace std;

QueryEvaluator::QueryEvaluator() {
    PKBQueryController emptyPKBQueryController = PKBQueryController();
    this->actions_executor = ActionsExecutor(PKBQueryController());
}

QueryEvaluator::QueryEvaluator(PKBQueryController pkb_query_controller) {
    this->actions_executor = ActionsExecutor(pkb_query_controller);
}

vector<string> QueryEvaluator::QERunQuery(QueryMap query_map) {
    vector<string> queryResults = QEGenerateActions(query_map);
    return QERenderResults(queryResults);
}

vector<string> QueryEvaluator::QEGenerateActions(QueryMap query_map) {
    ActionsGenerator actions_generator = ActionsGenerator(query_map, this->actions_executor);
    return actions_generator.TraverseQueryMap();
}

vector<string> QueryEvaluator::QERenderResults (vector<string> queryResults) {
    //skeleton code
    return queryResults;
}

