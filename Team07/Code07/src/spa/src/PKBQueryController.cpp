#include <vector>
#include "PKB.h"
#include "PKBQueryController.hpp"

PKBQueryController::PKBQueryController(PKB pkbToSet) {
    setPKB(pkbToSet);
}

void PKBQueryController::setPKB(PKB pkbToSet) {
    pkb = pkbToSet;
}

bool PKBQueryController::isFollows(stmt_ref s1, stmt_ref s2) {
    return true;
}

bool PKBQueryController::isFollowsStar(stmt_ref s1, stmt_ref s2) {
    return true;
}

bool PKBQueryController::isParent(stmt_ref s1, stmt_ref s2) {
    return true;
}

bool PKBQueryController::isParentStar(stmt_ref s1, stmt_ref s2) {
    return true;
}

bool PKBQueryController::statementUses(stmt_ref s, var_ref v) {
    return true;
}

bool PKBQueryController::statementModifies(stmt_ref s, var_ref v) {
    return true;
}

bool PKBQueryController::satisfiesPattern(assign_ref a, pattern p) {
    return true;
}

std::vector<const_value> PKBQueryController::getAllConstants() {
    std::vector<constant> vect;
    for (auto it : pkb.constants) {
        vect.push_back(it);
    }
    return vect;
}

std::vector<proc_ref> PKBQueryController::getAllProcedures() {
    std::vector<proc_ref> vect;
    for (auto it : pkb.procedures) {
        vect.push_back(it.first);
    }
    return vect;
}

std::vector<stmt_ref> PKBQueryController::getAllStatements() {
    std::vector<stmt_ref> vect;
    for (auto it : pkb.statements) {
        vect.push_back(it.first);
    }
    return vect;
}

std::vector<var_ref> PKBQueryController::getAllVariables() {
    std::vector<var_ref> vect;
    for (auto it : pkb.variables) {
        vect.push_back(it.first);
    }
    return vect;
}

std::vector<stmt_ref> PKBQueryController::getStatementsOfType(stmt_type t) {
    std::vector<stmt_ref> vect;
    return vect;
}
