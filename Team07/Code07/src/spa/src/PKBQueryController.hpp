#include <vector>
#include "PKB.hpp"
#include "Types.hpp"
#include "PKBEntities.hpp"

class PKBQueryController {
    private:
        PKB pkb;
        PKBQueryController() {}
    public:
        bool isFollows(stmt_ref s1, stmt_ref s2);
        bool isFollowsStar(stmt_ref s1, stmt_ref s2);
        bool isParent(stmt_ref s1, stmt_ref s2);
        bool isParentStar(stmt_ref s1, stmt_ref s2);
        bool statementUses(stmt_ref s, var_ref v);
        bool statementModifies(stmt_ref s, var_ref v);
        bool satisfiesPattern(assign_ref a, pattern p);
        // pattern uses, pattern modifies
        std::vector<constant> getAllConstants();
        std::vector<proc_ref> getAllProcedures();
        std::vector<var_ref> getAllVariables();
        std::vector<statement> getAllStatements();
        std::vector<stmt_ref> getStatementsOfType();
};