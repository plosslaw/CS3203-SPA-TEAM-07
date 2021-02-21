#include "QueryEvaluator.h"

using namespace std;

QueryEvaluator::QueryEvaluator() {
    PKBQueryController empty_pkb_query_controller = PKBQueryController();
    this->actions_executor = ActionsExecutor(PKBQueryController());
}

QueryEvaluator::QueryEvaluator(PKBQueryController pkb_query_controller) {
    this->actions_executor = ActionsExecutor(pkb_query_controller);
}

vector<string> QueryEvaluator::run_query(QueryMap query_map) {
    vector<string> query_results = this->generate_actions(query_map);
    return this->render_results(query_results);
}

vector<string> QueryEvaluator::generate_actions(QueryMap query_map) {
    ActionsGenerator actions_generator = ActionsGenerator(query_map, this->actions_executor);
    return actions_generator.TraverseQueryMap();
}

vector<string> QueryEvaluator::render_results (vector<string> query_results) {
    return this->response_renderer.render_results(query_results);
}

