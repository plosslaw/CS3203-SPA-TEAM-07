#include "PKBEntities.hpp"
#include "PKB.hpp"

class PKBBuilder {
    private:
        PKB pkb;
        TNode ast_root;
    public:
        PKBBuilder(TNode root) {
            ast_root = root;
        }

        PKB build();
};