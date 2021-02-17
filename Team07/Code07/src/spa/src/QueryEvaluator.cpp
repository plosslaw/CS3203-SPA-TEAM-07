#include "QueryEvaluator.h"

using namespace std;

QueryEvaluator::QueryEvaluator(PKBQueryController pkb_query_controller) {
    this->actionsExecutor = ActionsExecutor(pkb_query_controller);
}

vector<string> QueryEvaluator::QERunQuery(QueryMap query_map) {
    vector<string> queryResults = QEGenerateActions(query_map);
    return QERenderResults(queryResults);
}

vector<string> QueryEvaluator::QEGenerateActions(QueryMap query_map) {
    //skeleton code
    vector<string> results;
    return results;
}

vector<string> QueryEvaluator::QERenderResults (vector<string> queryResults) {
    //skeleton code
    return queryResults;
}

