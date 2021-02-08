#include "PKBBuilder.hpp"

PKBBuilder::PKBBuilder(TNode root) {
    // Note that the rest of the fields should initialize themselves as empty by default
    PKBBuilder::pkb.ast_root = root;
}

// Traverse through the tree rooted at node pkb.ast_root to generate and return the PKB 
PKB PKBBuilder::build() {
    /*
     implementation of the builder goes here
    */

    return pkb;
}