#pragma once

#include <vector>
#include "PKB.h"

class PKBQueryController {
    private:
        PKB pkb;
    public:
        PKBQueryController();

        explicit PKBQueryController(PKB pkbToSet);

        void setPKB(PKB pkbToSet);
        // Returns true if and only if Follows(s1,s2)
        bool isFollows(stmt_ref s1, stmt_ref s2);

        // Returns true if and only if Follows*(s1,s2)
        bool isFollowsStar(stmt_ref s1, stmt_ref s2);

        // Returns true if and only if Parent(s1,s2)
        bool isParent(stmt_ref s1, stmt_ref s2);

        // Returns true if and only if Parent(s1,s2)
        bool isParentStar(stmt_ref s1, stmt_ref s2);

        // Returns true if and only if Uses(s,v)
        bool statementUses(stmt_ref s, var_ref v);

        // Returns true if and only if Modifies(s,v)
        bool statementModifies(stmt_ref s, var_ref v);

        // Returns true if and only if assignment statement a matches pattern p
        bool satisfiesPattern(assign_ref a, pattern p);

        // Returns list of all constants in the SIMPLE program
        std::vector<const_value> getAllConstants();

        // Returns list of all procedures in the SIMPLE program
        std::vector<proc_ref> getAllProcedures();

        // Returns list of all statements in the SIMPLE program
        std::vector<stmt_ref> getAllStatements();

        // Returns list of all variables in the SIMPLE program
        std::vector<var_ref> getAllVariables();

        // Returns list of all statements of type t in the SIMPLE program
        std::vector<stmt_ref> getStatementsOfType(stmt_type t);
};
