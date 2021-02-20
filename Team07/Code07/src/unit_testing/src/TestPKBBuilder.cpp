#include "catch.hpp"

#include "MockAST.h"
#include "PKBBuilder.hpp"

TNode root = getMockAST();

TEST_CASE("Test I: Procedure extraction") {
    PKBBuilder builder = PKBBuilder(root);
    PKB pkb = builder.build();

    SECTION("Statement extraction") {
        REQUIRE(pkb.statements.find(2) != pkb.statements.end());
        REQUIRE(pkb.statements[3].type == WHILE);

        REQUIRE(pkb.statements.find(0) == pkb.statements.end());
        REQUIRE(pkb.statements.find(-1) == pkb.statements.end());
        REQUIRE(pkb.statements.find(12) == pkb.statements.end());
    }

    SECTION("Procedure extraction") {
        REQUIRE(pkb.procedures.find("main") != pkb.procedures.end());
        REQUIRE(pkb.procedures.find("fn") == pkb.procedures.end());
    }

    SECTION("Variable extraction") {
        REQUIRE(pkb.variables.find("x") != pkb.variables.end());
        REQUIRE(pkb.variables.find("z") != pkb.variables.end());
        REQUIRE(pkb.variables.find("c") == pkb.variables.end());
        REQUIRE(pkb.variables.find("main") == pkb.variables.end());
    }
    
    SECTION("Constant extraction") {
        REQUIRE(pkb.constants.find(42) != pkb.constants.end());
        REQUIRE(pkb.constants.find(65) == pkb.constants.end());
    }
}

TEST_CASE("TEST II: Parent and Parent* relationship extraction") {
    PKBBuilder builder = PKBBuilder(root);
    PKB pkb = builder.build();

    REQUIRE(pkb.statements[6].parent == 5);
    REQUIRE(pkb.statements[11].parent == NULL_STMT_REF);

    REQUIRE(pkb.statements[10].indirect_parent.find(5) != pkb.statements[10].indirect_parent.end());
    REQUIRE(pkb.statements[4].indirect_parent.find(3) != pkb.statements[10].indirect_parent.end());
    REQUIRE(pkb.statements[4].indirect_parent.find(2) == pkb.statements[10].indirect_parent.end());
}

TEST_CASE("TEST III: Follows and Follows* relationship extraction") {
    PKBBuilder builder = PKBBuilder(root);
    PKB pkb = builder.build();
    
    REQUIRE(pkb.statements[7].follows == 8);
    REQUIRE(pkb.statements[6].follows == NULL_STMT_REF);
    REQUIRE(pkb.statements[11].follows == NULL_STMT_REF);

    REQUIRE(pkb.statements[6].indirect_follows.empty());
    REQUIRE(pkb.statements[1].indirect_follows.find(2) != pkb.statements[1].indirect_follows.end());
    REQUIRE(pkb.statements[1].indirect_follows.find(5) != pkb.statements[1].indirect_follows.end());
    REQUIRE(pkb.statements[11].indirect_follows.find(2) == pkb.statements[11].indirect_parent.end());
}

TEST_CASE("TEST IV: Uses relationship without function calls") {
    PKBBuilder builder = PKBBuilder(root);
    PKB pkb = builder.build();
    
    REQUIRE(pkb.statements[3].uses.find("y") != pkb.statements[3].uses.end());
    REQUIRE(pkb.statements[6].uses.find("w") == pkb.statements[6].uses.end());
    REQUIRE(pkb.statements[5].uses.find("v") != pkb.statements[5].uses.end());
    REQUIRE(pkb.statements[1].uses.find("a") == pkb.statements[1].uses.end());
    REQUIRE(pkb.statements[2].uses.find("b") != pkb.statements[2].uses.end());
    REQUIRE(pkb.statements[3].uses.find("z") == pkb.statements[3].uses.end());
}

TEST_CASE("TEST V: Uses relationship without function calls") {
    PKBBuilder builder = PKBBuilder(root);
    PKB pkb = builder.build();
    
    REQUIRE(pkb.statements[3].modifies.find("y") == pkb.statements[3].modifies.end());
    REQUIRE(pkb.statements[6].modifies.find("w") != pkb.statements[6].modifies.end());
    REQUIRE(pkb.statements[5].modifies.find("v") != pkb.statements[5].modifies.end());
    REQUIRE(pkb.statements[1].modifies.find("a") != pkb.statements[1].modifies.end());
    REQUIRE(pkb.statements[2].modifies.find("b") == pkb.statements[2].modifies.end());
    REQUIRE(pkb.statements[3].modifies.find("z") == pkb.statements[3].modifies.end());
}

TEST_CASE("Test VI: Assignment statement extraction") {
    PKBBuilder builder = PKBBuilder(root);
    PKB pkb = builder.build();

    REQUIRE(pkb.assignments[4].rightValue == "(x-1)");
    REQUIRE(pkb.assignments[6].rightValue == "(y*2)");
    REQUIRE(pkb.assignments.find(5) == pkb.assignments.end());
}