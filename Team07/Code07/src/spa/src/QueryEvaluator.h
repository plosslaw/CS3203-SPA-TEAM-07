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
    std::vector<std::string> QERunQuery(QueryMap query_map);

private:
    std::vector<std::string> QEGenerateActions(QueryMap query_map);
    std::vector<std::string> QERenderResults(std::vector<std::string> queryResults);

    ActionsExecutor actions_executor;

};





