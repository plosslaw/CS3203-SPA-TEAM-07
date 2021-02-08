#pragma once

#include <string>
#include <vector>

#include "Action.h"
#include "ActionsExecutor.h"
#include "ActionsGenerator.h"
#include "PKB.h"
#include "PKBQueryController.h"
#include "ResponseRenderer.h"
#include "TNode.h"

class QueryEvaluator {
public:
    explicit QueryEvaluator(PKBQueryController pkbQueryController);
    std::vector<std::string> QERunQuery(TNode* PQL_AST_Root);

private:
    std::vector<Action> QEGenerateActions(TNode* PQL_AST_Root);
    vector<string> QEExecuteActions (std::vector<Action> queryActions);
    std::vector<std::string> QERenderResults(vector<string> queryResults);

};





