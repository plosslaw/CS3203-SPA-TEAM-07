#pragma once

#include "PKBEntities.hpp"
#include "TNode.h"
#include "PKB.h"

class PKBBuilder {
    private:
        PKB pkb;
        TNode ast_root;

    public:
        PKBBuilder(TNode root);
        PKB build();
};
