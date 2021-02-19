#include "../../spa/src/ActionsExecutor.h"
#include "../../spa/src/PKBBuilder.hpp"
#include "MockAST.h"

#include "../../../lib/catch.hpp"

TEST_CASE("single_such_that_clause") {
    TNode ast_tree = getMockAST();
    PKBBuilder pkb_builder(ast_tree);
    PKBQueryController pkb_query_controller(pkb_builder.build());

    ActionsExecutor executor(pkb_query_controller);

    SECTION("follows") {
        REQUIRE(true);
    }
}

TEST_CASE("single_pattern_clause") {
    TNode ast_tree = getMockAST();
    PKBBuilder pkb_builder(ast_tree);
    PKBQueryController pkb_query_controller(pkb_builder.build());

    ActionsExecutor executor(pkb_query_controller);
}