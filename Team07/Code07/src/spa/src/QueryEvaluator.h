#pragma once

#include <string>
#include <vector>

#include "ActionsExecutor.h"
#include "ActionsGenerator.h"
#include "PKB.h"
#include "PKBQueryController.hpp"
#include "QueryMap.h"
#include "ResponseRenderer.h"

class QueryEvaluator {
public:
    explicit QueryEvaluator();
    explicit QueryEvaluator(PKBQueryController pkb_query_controller);
    std::vector<std::string> run_query(QueryMap query_map);

private:
    std::vector<std::string> generate_actions(QueryMap query_map);
    std::vector<std::string> render_results(std::vector<std::string> query_results);

    ActionsExecutor actions_executor;

};





