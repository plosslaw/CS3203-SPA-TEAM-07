#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Action.h"
#include "PKB.h"
#include "PKBQueryController.hpp"
#include "ResponseRenderer.h"

class ActionsExecutor {
public:
    ActionsExecutor();
    explicit ActionsExecutor(PKBQueryController pkbQueryController);
    std::vector<std::string> executeActions(std::vector<Action> queryActions);

private:
    void declareSynonyms();
    void createDependencyGraph();
    std::vector<std::string> getAllThatSatisfies();

    PKBQueryController pkbQueryController;

};



