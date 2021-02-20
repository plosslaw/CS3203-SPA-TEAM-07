#include <vector>
#include "PKBEntities.hpp"
#include "PKB.h"
#include "PKBQueryController.hpp"

PKBQueryController::PKBQueryController() {}

PKBQueryController::PKBQueryController(PKB pkbToSet) {
    setPKB(pkbToSet);
}

void PKBQueryController::setPKB(PKB pkbToSet) {
    pkb = pkbToSet;
}

bool PKBQueryController::isFollows(stmt_ref s1, stmt_ref s2) {
    if (pkb.statements.find(s1) == pkb.statements.end()) return false;
    statement statement1 = pkb.statements.at(s1);
    return statement1.follows == s2;
}

bool PKBQueryController::isFollowsStar(stmt_ref s1, stmt_ref s2) {
    if (pkb.statements.find(s1) == pkb.statements.end()) return false;
    statement statement1 = pkb.statements.at(s1);
    return statement1.indirect_follows.find(s2) != statement1.indirect_follows.end();
}

bool PKBQueryController::isParent(stmt_ref s1, stmt_ref s2) {
    if (pkb.statements.find(s2) == pkb.statements.end()) return false;
    statement statement2 = pkb.statements.at(s2);
    return statement2.parent == s1;
}

bool PKBQueryController::isParentStar(stmt_ref s1, stmt_ref s2) {
    if (pkb.statements.find(s2) == pkb.statements.end()) return false;
    statement statement2 = pkb.statements.at(s2);
    return statement2.indirect_parent.find(s1) != statement2.indirect_parent.end();
}

bool PKBQueryController::statementUses(stmt_ref s, var_ref v) {
    if (pkb.statements.find(s) == pkb.statements.end()) return false;
    statement stmt = pkb.statements.at(s);
    return stmt.uses.find(v) != stmt.uses.end();
}

bool PKBQueryController::statementModifies(stmt_ref s, var_ref v) {
    if (pkb.statements.find(s) == pkb.statements.end()) return false;
    statement stmt = pkb.statements.at(s);
    return stmt.modifies.find(v) != stmt.modifies.end();
}

bool PKBQueryController::procedureUses(proc_ref p, var_ref v) {
    return false;
}

bool PKBQueryController::procedureModifies(proc_ref p, var_ref v) {
    return false;
}


bool PKBQueryController::satisfiesPattern(assign_ref a, pattern p) {
    if (pkb.assignments.find(a) == pkb.assignments.end()) return false;
    std::string pattern_string = p.rvalue;
    int initial_pattern_length = pattern_string.length();
    std::string substring;
    // 0 wildcardleft
    // 1 wildcardright
    // 2 wildcardboth
    // -1 no wildcard
    int position_wildcard = -1; 
    if (pattern_string[0] == '_' && pattern_string[initial_pattern_length - 1] == '_') {
        substring = pattern_string.substr(1, initial_pattern_length - 2);
        position_wildcard = 2;
    } else if (pattern_string[0] == '_') {
        substring = pattern_string.substr(1, initial_pattern_length - 1);
        position_wildcard = 0;
    } else if (pattern_string[initial_pattern_length - 1] == '_') {
        substring = pattern_string.substr(0, initial_pattern_length - 2);
        position_wildcard = 1;
    } else {
        substring = pattern_string;
    }
    assignment asmt = pkb.assignments.at(a);
    std::string text = asmt.rightValue;
    int m = text.length();
    int n = substring.length();
    if (substring == "_") return true;  
    int pointer_idx[n];
    int j = 0;
    if (position_wildcard == -1) return text == substring; // no wildcard, must match exact
    for (int i = 2; i <= n; i++) {
        while (j > 0 && substring[i - 1] != substring[j]) {
            j = pointer_idx[j];
        } 
        if (substring[i - 1] == substring[j]) {
            j++;
        }
        pointer_idx[i] = j;
    }
    j = 0;
    for (int i = 1; i <= m; i++) {
        while (j > 0 && text[i - 1] != substring[j]) {
            j = pointer_idx[j];
        }
        if (text[i - 1] == substring[j]) {
            j++;
        }
        if (j == n) {
            //j = pointer_idx[j];
            if (position_wildcard == 0) {
                // wildcard at left, the pattern must be at rightmost
                // j is the ending of the pattern,   
                // check if the pattern start has some strings before it.    
                return i - n >= 0;
            } else if (position_wildcard == 1) {
                // wildcard at right, pattern at leftmost
                // j is the ending of the pattern, 
                // check if the pattern end has some strings after it.
                return j <= m;
            } else {
                return true;
            }
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
    for (auto it : pkb.statements) {
        statement s = it.second;
        if (s.type == t) vect.push_back(it.first);
    }
    return vect;
}
