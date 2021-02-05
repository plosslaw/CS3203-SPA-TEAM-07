#include <unordered_set>
#include <optional>
#include "Types.hpp"
#include "TNode.hpp"

typedef struct statement{
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

typedef struct variable{
    var_ref reference;
};