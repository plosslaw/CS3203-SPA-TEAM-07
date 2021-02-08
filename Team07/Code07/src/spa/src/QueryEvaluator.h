#pragma once

#include <string>
#include <vector>

#include "Action.h"
#include "ActionsExecutor.h"
#include "ActionsGenerator.h"
#include "PKB.h"
#include "ResponseRenderer.h"
#include "TNode.h"

class QueryEvaluator {
public:
    static std::vector<std::string> QERunQuery(TNode* PQL_AST_Root);

private:
    static std::vector<Action> QEGenerateActions(TNode* PQL_AST_Root);
    static vector<string> QEExecuteActions (std::vector<Action> queryActions);
    static std::vector<std::string> QERenderResults(vector<string> queryResults);

};





