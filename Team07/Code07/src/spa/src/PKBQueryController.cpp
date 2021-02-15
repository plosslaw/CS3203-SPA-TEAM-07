#include <vector>
#include "PKBEntities.hpp"
#include "PKB.h"
#include "PKBQueryController.hpp"

PKBQueryController::PKBQueryController(PKB pkbToSet) {
    setPKB(pkbToSet);
}

void PKBQueryController::setPKB(PKB pkbToSet) {
    pkb = pkbToSet;
}

bool PKBQueryController::isFollows(stmt_ref s1, stmt_ref s2) {
    statement statement1 = pkb.statements.at(s1);
    return statement1.follows = s2;
}

bool PKBQueryController::isFollowsStar(stmt_ref s1, stmt_ref s2) {
    statement statement1 = pkb.statements.at(s1);
    return statement1.indirect_follows.find(s2) != statement1.indirect_follows.end();
}

bool PKBQueryController::isParent(stmt_ref s1, stmt_ref s2) {
    statement statement2 = pkb.statements.at(s2);
    return statement2.parent == s1;
}

bool PKBQueryController::isParentStar(stmt_ref s1, stmt_ref s2) {
    statement statement2 = pkb.statements.at(s2);
    return statement2.indirect_parent.find(s1) != statement2.indirect_parent.end();
}

bool PKBQueryController::statementUses(stmt_ref s, var_ref v) {
    statement stmt = pkb.statements.at(s);
    return stmt.uses.find(v) != stmt.uses.end();
}

bool PKBQueryController::statementModifies(stmt_ref s, var_ref v) {
    statement stmt = pkb.statements.at(s);
    return stmt.modifies.find(v) != stmt.modifies.end();
}

bool PKBQueryController::satisfiesPattern(assign_ref a, pattern p) {
    assignment asmt = pkb.assignments.at(a);
    std::string text = asmt.rightValue;
    std::string substr = p.rvalue;
    if (substr == "_") return true;
    // KMP algorithm
    int m = text.length();
    int n = substr.length();
    if (n == 0) return true;
    if (m < n) return false;
    int next[n + 1];
    for (int i = 0; i <= n; i++) {
        next[i] = 0;
    }
    for (int i = 1; i < n; i++) {
        int j = next[i + 1];
        while ( j > 0 && substr[j] != substr[i]) {
            j = next[j];
        }
        if (j > 0 || substr[j] == substr[j]) {
            next[i + 1] = j + 1;
        }
    }
    for (int i = 0, j = 0; i < m; i++) {
        if (text[i] == substr[j]) {
            if (++j == n) return true;
        } else if (j > 0) {
            j = next[j];
            i--;
        }
    }
    return false;
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
