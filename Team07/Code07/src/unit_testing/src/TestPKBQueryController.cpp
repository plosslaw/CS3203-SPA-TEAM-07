#include "catch.hpp"
#include "PKB.H"
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
    procedure one = {reference1, children1, uses1, modifies1};
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
    constants.insert(0);

    //initialise pkb
    TNode root;
    PKB pkb = {procedures, statements, assignments, variables, constants, root};

}
