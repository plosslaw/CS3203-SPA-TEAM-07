#include "catch.hpp"
#include "PKB.h"
#include "PKBQueryController.cpp"
#include "PKBEntities.hpp"
#include "Types.hpp"
#include <unordered_map>
#include <unordered_set>

TEST_CASE("One") {
    // SIMPLE code:
    // 0.  procedure gcd {
    // 1.     read a;
    // 2.     read b;
    // 3.     while (b != 0) {
    // 4.         temp = a % b;
    // 5.         a = b;
    // 6.         b = temp;
    // 7.     }
    // 8.     print a;
    // 9.  }
    std::unordered_map<proc_ref, procedure> procedures;
    std::unordered_map<stmt_ref, statement> statements;
    std::unordered_map<assign_ref, assignment> assignments;
	std::unordered_map<var_ref, variable> variables;
	std::unordered_set<const_value> constants;
    //line 1
    std::unordered_set<proc_ref> children1;
    std::unordered_set<var_ref> uses1;
    std::unordered_set<var_ref> modifies1;
    proc_ref reference1 = "gcd";
    TNode ast1;
    procedure one = {reference1, ast1, children1, uses1, modifies1};
    procedures.insert(std::make_pair(reference1, one));

    //line 2
    stmt_ref reference2 = 1;
    stmt_type type2 = READ;
    TNode ast2;
    std::unordered_set<var_ref> uses2;
    std::unordered_set<var_ref> modifies2;
    modifies2.insert("a");
    stmt_ref parent2 = NULL_STMT_REF;
    stmt_ref follows2 = 2;
    std::unordered_set<stmt_ref> indirect_parent2;
    std::unordered_set<stmt_ref> indirect_follows2;
    indirect_follows2.insert(2);
    indirect_follows2.insert(3);
    indirect_follows2.insert(8);
    statement two = {reference2, type2, ast2, uses2, modifies2, parent2, follows2, indirect_parent2, indirect_follows2};
    statements.insert(std::make_pair(reference2, two));

    //line 3
    stmt_ref reference3 = 2;
    stmt_type type3 = READ;
    TNode ast3;
    std::unordered_set<var_ref> uses3;
    std::unordered_set<var_ref> modifies3;
    modifies3.insert("b");
    stmt_ref parent3 = NULL_STMT_REF;
    stmt_ref follows3 = 3;
    std::unordered_set<stmt_ref> indirect_parent3;
    std::unordered_set<stmt_ref> indirect_follows3;
    indirect_follows3.insert(3);
    indirect_follows3.insert(8);
    statement three = {reference3, type3, ast3, uses3, modifies3, parent3, follows3, indirect_parent3, indirect_follows3};
    statements.insert(std::make_pair(reference3, three));

    //line 4
    stmt_ref reference4 = 3;
    stmt_type type4 = WHILE;
    TNode ast4;
    std::unordered_set<var_ref> uses4;
    uses4.insert("a");
    uses4.insert("b");
    std::unordered_set<var_ref> modifies4;
    stmt_ref parent4 = NULL_STMT_REF;
    stmt_ref follows4 = 8;
    std::unordered_set<stmt_ref> indirect_parent4;
    std::unordered_set<stmt_ref> indirect_follows4;
    indirect_follows4.insert(8);
    statement four = {reference4, type4, ast4, uses4, modifies4, parent4, follows4, indirect_parent4, indirect_follows4};
    statements.insert(std::make_pair(reference4, four));
    assignment assignment1 = {"a % b"};
    assign_ref assign4 = 4;
    assignments.insert(std::make_pair(assign4, assignment1));

    //line 5
    stmt_ref reference5 = 4;
    stmt_type type5 = ASSIGN;
    TNode ast5;
    std::unordered_set<var_ref> uses5;
    uses5.insert("a");
    uses5.insert("b");
    std::unordered_set<var_ref> modifies5;
    modifies5.insert("temp");
    stmt_ref parent5 = 3;
    stmt_ref follows5 = 5;
    std::unordered_set<stmt_ref> indirect_parent5;
    indirect_parent5.insert(3);
    std::unordered_set<stmt_ref> indirect_follows5;
    indirect_follows5.insert(5);
    indirect_follows5.insert(6);
    statement five = {reference5, type5, ast5, uses5, modifies5, parent5, follows5, indirect_parent5, indirect_follows5};
    statements.insert(std::make_pair(reference5, five));

    //line 6
    stmt_ref reference6 = 5;
    stmt_type type6 = ASSIGN;
    TNode ast6;
    std::unordered_set<var_ref> uses6;
    uses6.insert("b");
    std::unordered_set<var_ref> modifies6;
    modifies6.insert("a");
    stmt_ref parent6 = 3;
    stmt_ref follows6 = 6;
    std::unordered_set<stmt_ref> indirect_parent6;
    indirect_parent6.insert(3);
    std::unordered_set<stmt_ref> indirect_follows6;
    indirect_follows6.insert(6);
    statement six = {reference6, type6, ast6, uses6, modifies6, parent6, follows6, indirect_parent6, indirect_follows6};
    statements.insert(std::make_pair(reference6, six));

    //line 7
    stmt_ref reference7 = 6;
    stmt_type type7 = ASSIGN;
    TNode ast7;
    std::unordered_set<var_ref> uses7;
    uses7.insert("temp");
    std::unordered_set<var_ref> modifies7;
    modifies7.insert("b");
    stmt_ref parent7 = 3;
    stmt_ref follows7 = NULL_STMT_REF;
    std::unordered_set<stmt_ref> indirect_parent7;
    indirect_parent7.insert(3);
    std::unordered_set<stmt_ref> indirect_follows7;
    statement seven = {reference7, type7, ast7, uses7, modifies7, parent7, follows7, indirect_parent7, indirect_follows7};
    statements.insert(std::make_pair(reference7, seven));

    //line 8
    stmt_ref reference8 = 7;
    stmt_type type8 = PRINT;
    TNode ast8;
    std::unordered_set<var_ref> uses8;
    uses8.insert("a");
    std::unordered_set<var_ref> modifies8;
    stmt_ref parent8 = NULL_STMT_REF;
    stmt_ref follows8 = NULL_STMT_REF;
    std::unordered_set<stmt_ref> indirect_parent8;
    std::unordered_set<stmt_ref> indirect_follows8;
    statement eight = {reference8, type8, ast8, uses8, modifies8, parent8, follows8, indirect_parent8, indirect_follows8};
    statements.insert(std::make_pair(reference8, eight));

    // store variables
    variable a = {"a"};
    variable b = {"b"};
    variable temp = {"temp"};
    variables.insert(std::make_pair("a", a));
    variables.insert(std::make_pair("b", b));
    variables.insert(std::make_pair("temp", temp));

    // store constants
    constants.insert("0");

    //initialise pkb
    TNode root;
    PKB pkb = {procedures, statements, assignments, variables, constants, root};

    PKBQueryController pkbQueryController = PKBQueryController(pkb);

    SECTION("check_is_follows") {
        REQUIRE(pkbQueryController.isFollows(1, 2) == true);
        REQUIRE(pkbQueryController.isFollows(2, 3) == true);
        REQUIRE(pkbQueryController.isFollows(4, 5) == true);
        REQUIRE(pkbQueryController.isFollows(5, 6) == true);
        REQUIRE(pkbQueryController.isFollows(1, 3) == false);
        REQUIRE(pkbQueryController.isFollows(2, 1) == false);
    }

    SECTION("check_is_follows*") {
        REQUIRE(pkbQueryController.isFollowsStar(1, 3) == true);
        REQUIRE(pkbQueryController.isFollowsStar(1, 8) == true);
        REQUIRE(pkbQueryController.isFollowsStar(2, 8) == true);
        REQUIRE(pkbQueryController.isFollowsStar(2, 4) == false);
        REQUIRE(pkbQueryController.isFollowsStar(3, 6) == false);
        REQUIRE(pkbQueryController.isFollowsStar(6, 8) == false);
    }

    SECTION("Ccheck_is_parent") {
        REQUIRE(pkbQueryController.isParent(1, 2) == false);
        REQUIRE(pkbQueryController.isParent(1, 3) == false);
        REQUIRE(pkbQueryController.isParent(2, 1) == false);
        REQUIRE(pkbQueryController.isParent(3, 4) == true);
        REQUIRE(pkbQueryController.isParent(3, 5) == true);
        REQUIRE(pkbQueryController.isParent(3, 6) == true);
    }

    SECTION("check_statement_uses") {
        REQUIRE(pkbQueryController.statementUses(3, "b") == true);
        REQUIRE(pkbQueryController.statementUses(4, "b") == true);
        REQUIRE(pkbQueryController.statementUses(4, "a") == true);
        REQUIRE(pkbQueryController.statementUses(5, "b") == true);
        REQUIRE(pkbQueryController.statementUses(7, "a") == true);
        REQUIRE(pkbQueryController.statementUses(4, "temp") == false);
    }

    SECTION("check_satisfies_pattern") {
        assign_ref a = 4;
        pattern p1 = {"temp", "a % b"};
        pattern p2 = {"temp", "_a % b"};
        pattern p3 = {"temp", "a % b_"};
        pattern p4 = {"temp", "_a % b_"};
        pattern p5 = {"temp", "b"};
        pattern p6 = {"temp", "a"};
        pattern p7 = {"temp", "_a + c_"};
        pattern p8 = {"temp", "_temp_"};
        pattern p9 = {"temp", "_"};
        pattern p10 = {"not_modified_var", "a % b"};
        pattern p11 = {"v", "_a % b"};
        pattern p12 = {"v", "a % b_"};
        pattern p13 = {"v", "_a % b_"};
        pattern p14 = {"v", "a"};
        pattern p15 = {"v", "_a + c_"};
        pattern p16 = {"_", "_"};
        pattern p17 = {"_", "a % b"};
        pattern p18 = {"_", "_a + c_"};
        REQUIRE(pkbQueryController.satisfiesPattern(a, p1) == true);
        REQUIRE(pkbQueryController.satisfiesPattern(a, p2) == true);
        REQUIRE(pkbQueryController.satisfiesPattern(a, p3) == true);
        REQUIRE(pkbQueryController.satisfiesPattern(a, p4) == true);
        REQUIRE(pkbQueryController.satisfiesPattern(a, p5) == false);
        REQUIRE(pkbQueryController.satisfiesPattern(a, p6) == false);
        REQUIRE(pkbQueryController.satisfiesPattern(a, p7) == false);
        REQUIRE(pkbQueryController.satisfiesPattern(a, p8) == false);
        REQUIRE(pkbQueryController.satisfiesPattern(a, p9) == true);
        REQUIRE(pkbQueryController.satisfiesPattern(a, p10) == false);
        REQUIRE(pkbQueryController.satisfiesPattern(a, p11) == false);
        REQUIRE(pkbQueryController.satisfiesPattern(a, p12) == false);
        REQUIRE(pkbQueryController.satisfiesPattern(a, p13) == false);
        REQUIRE(pkbQueryController.satisfiesPattern(a, p14) == false);
        REQUIRE(pkbQueryController.satisfiesPattern(a, p15) == false);
        REQUIRE(pkbQueryController.satisfiesPattern(a, p16) == true);
        REQUIRE(pkbQueryController.satisfiesPattern(a, p17) == true);
        REQUIRE(pkbQueryController.satisfiesPattern(a, p18) == false);
    }
    // implement the rest in iteration/v1.3

    SECTION("check get_statements_of_type") {
        stmt_type p = PRINT;
        stmt_type r = READ;
        stmt_type w = WHILE;
        REQUIRE(pkbQueryController.getStatementsOfType(p).size() == 1);
        REQUIRE(pkbQueryController.getStatementsOfType(w).size() == 1);
        REQUIRE(pkbQueryController.getStatementsOfType(r).size() == 2);  
    }

}