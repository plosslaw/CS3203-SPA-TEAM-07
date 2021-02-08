#include <unordered_set>
#include "TNode.hpp"
#include "Types.hpp"

typedef struct statement {
    stmt_ref reference;
    stmt_type type;
    TNode ast;
    std::unordered_set<var_ref> uses;
    std::unordered_set<var_ref> modifies;

    stmt_ref parent;
    stmt_ref follows;

    std::unordered_set<stmt_ref> indirect_parent;
    std::unordered_set<stmt_ref> indirect_follows;

};

typedef struct variable {
    var_ref reference;
};

typedef struct constant {
    const_value value;
};

typedef struct procedure {
    proc_ref reference;

    std::unordered_set<proc_ref> children;
    std::unordered_set<var_ref> uses;
    std::unordered_set<var_ref> modifies;
};