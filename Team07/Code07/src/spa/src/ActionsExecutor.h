#pragma once

#include <string>
#include <vector>

#include "Action.h"
#include "PKB.h"
#include "PKBQueryController.hpp"
#include "QueryResult.h"


class ActionsExecutor {
public:
    enum stmt_pos {
        FIRST_ARG,
        SECOND_ARG
    };

    ActionsExecutor();
    explicit ActionsExecutor(PKBQueryController pkbQueryController);

    // Basic api
    std::vector<QueryResult> getAllConstants();
    std::vector<QueryResult> getAllEntitiesOfType(stmt_type entityType);
    std::vector<QueryResult> getAllProcedures();
    std::vector<QueryResult> getAllStatements();
    std::vector<QueryResult> getAllVariables();

    // Single clause api
    std::vector<QueryResult> getAllStmtFollows(stmt_type stmtType, stmt_pos stmtPos);
    std::vector<QueryResult> getAllStmtFollows(stmt_type stmtType, stmt_pos stmtPos, stmt_ref otherStmt);
    std::vector<QueryResult> getAllStmtFollows(stmt_type stmtType, stmt_pos stmtPos, stmt_type otherStmtType);
    
    std::vector<QueryResult> getAllStmtParent(stmt_type stmtType);
    std::vector<QueryResult> getAllStmtParent(stmt_type stmtType, stmt_pos stmtPos, stmt_ref otherStmt);
    std::vector<QueryResult> getAllStmtParent(stmt_type stmtType, stmt_pos stmtPos, stmt_type otherStmtType);

    std::vector<QueryResult> getAllStmtModifies(var_ref v);
    std::vector<QueryResult> getAllStmtUses(var_ref v);

    std::vector<QueryResult> getAllVariableModifies(stmt_ref otherStmt);
    std::vector<QueryResult> getAllVariableModifies(stmt_type otherStmtType);

    std::vector<QueryResult> getAllVariableUses(stmt_ref otherStmt);
    std::vector<QueryResult> getAllVariableUses(stmt_type otherStmtType);

    std::vector<QueryResult> getAllProcedureModifies(var_ref v);
    std::vector<QueryResult> getAllProcedureUses(var_ref v);

    // std::vector<std::string> executeActions(std::vector<Action> queryActions);

private:
    PKBQueryController pkbQueryController;

};



