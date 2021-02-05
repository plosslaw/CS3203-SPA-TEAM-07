#include "Types.hpp"
#include "TNode.hpp"
#include <unordered_set>
#include <optional>

typedef struct {
    stmt_ref reference;
    stmt_type type;
    TNode ast;
    std::unordered_set<var_ref> uses;
    std::unordered_set<var_ref> modifies;

    std::optional<stmt_ref> parent;
    std::optional<stmt_ref> follows;

    std::unordered_set<stmt_ref> indirect_parent;
    std::unordered_set<stmt_ref> indirect_follows;

} statement;

typedef struct {
    var_ref reference;
} variable;