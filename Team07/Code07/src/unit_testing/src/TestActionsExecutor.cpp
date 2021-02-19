#include "../../spa/src/ActionsExecutor.h"
#include "../../spa/src/PKBBuilder.hpp"
#include "MockAST.h"

#include "../../../lib/catch.hpp"
#include <unordered_set>

using namespace std;

// helper functions
bool verify_const_vector(vector<const_value> test_vector, vector<const_value> ans_vector){
    if (test_vector.size() != ans_vector.size()) {
        return false;
    }
    unordered_set<const_value> unique_set;
    for (const_value ele : ans_vector) {
        unique_set.insert(ele);
    }

    for (const_value ele : test_vector) {
        if (unique_set.find(ele) == unique_set.end()) {
            return false;
        }
    }

    return true;
}

bool verify_proc_vector(vector<proc_ref> test_vector, vector<proc_ref> ans_vector){
    if (test_vector.size() != ans_vector.size()) {
        return false;
    }
    unordered_set<proc_ref> unique_set;
    for (proc_ref ele : ans_vector) {
        unique_set.insert(ele);
    }

    for (proc_ref ele : test_vector) {
        if (unique_set.find(ele) == unique_set.end()) {
            return false;
        }
    }

    return true;
}

bool verify_stmt_vector(vector<stmt_ref> test_vector, vector<stmt_ref> ans_vector){
    if (test_vector.size() != ans_vector.size()) {
        return false;
    }
    unordered_set<stmt_ref> unique_set;
    for (stmt_ref ele : ans_vector) {
        unique_set.insert(ele);
    }

    for (stmt_ref ele : test_vector) {
        if (unique_set.find(ele) == unique_set.end()) {
            return false;
        }
    }

    return true;
}

bool verify_var_vector(vector<var_ref> test_vector, vector<var_ref> ans_vector){
    if (test_vector.size() != ans_vector.size()) {
        return false;
    }
    unordered_set<var_ref> unique_set;
    for (var_ref ele : ans_vector) {
        unique_set.insert(ele);
    }

    for (var_ref ele : test_vector) {
        if (unique_set.find(ele) == unique_set.end()) {
            return false;
        }
    }

    return true;
}

//TODO(plosslaw): add test cases for boolean ActionsExecutor API

TEST_CASE("get_all_entities") {
    TNode ast_tree = getMockAST();
    PKBBuilder pkb_builder(ast_tree);
    PKBQueryController pkb_query_controller(pkb_builder.build());

    ActionsExecutor executor(pkb_query_controller);

    // answer vectors - stmts
    vector<stmt_ref> ans_vector_stmts {1,2,3,4,5,6,7,8,9,10,11};
    vector<stmt_ref> ans_vector_stmts_assign {4,6,7,8,10};
    vector<stmt_ref> ans_vector_stmts_call {};
    vector<stmt_ref> ans_vector_stmts_if {5};
    vector<stmt_ref> ans_vector_stmts_print {2,11};
    vector<stmt_ref> ans_vector_stmts_read {1};
    vector<stmt_ref> ans_vector_stmts_while {3,9};

    // answer vectors - consts
    vector<const_value> ans_vector_consts {1,2,42};

    // answer vectors - procedures
    vector<proc_ref> ans_vector_procs {"main"};

    // answer vectors - vars
    vector<var_ref> ans_vector_vars {"a", "b", "v", "w", "x", "y", "z"};


    SECTION("get_all_stmts") {
        REQUIRE(verify_stmt_vector(executor.get_all_statements(), ans_vector_stmts));
        REQUIRE(verify_stmt_vector(executor.get_all_statements_of_type(stmt_type::ASSIGN), ans_vector_stmts_assign));
        REQUIRE(verify_stmt_vector(executor.get_all_statements_of_type(stmt_type::CALL), ans_vector_stmts_call));
        REQUIRE(verify_stmt_vector(executor.get_all_statements_of_type(stmt_type::IF), ans_vector_stmts_if));
        REQUIRE(verify_stmt_vector(executor.get_all_statements_of_type(stmt_type::PRINT), ans_vector_stmts_print));
        REQUIRE(verify_stmt_vector(executor.get_all_statements_of_type(stmt_type::READ), ans_vector_stmts_read));
        REQUIRE(verify_stmt_vector(executor.get_all_statements_of_type(stmt_type::WHILE), ans_vector_stmts_while));
    }

    SECTION("get_all_consts") {
        REQUIRE(verify_const_vector(executor.get_all_constants(), ans_vector_consts));
    }

    SECTION("get_all_procs") {
        REQUIRE(verify_proc_vector(executor.get_all_procedures(), ans_vector_procs));
    }

    SECTION("get_all_vars") {
        REQUIRE(verify_var_vector(executor.get_all_variables(), ans_vector_vars));
    }
}


TEST_CASE("single_such_that_clause") {
    TNode ast_tree = getMockAST();
    PKBBuilder pkb_builder(ast_tree);
    PKBQueryController pkb_query_controller(pkb_builder.build());

    ActionsExecutor executor(pkb_query_controller);

    // TODO(plosslaw): add more test cases
    // answer vectors - follows
    vector<stmt_ref> ans_vector_follows_stmt_first_starred {1,2,3,5,7,8};
    vector<stmt_ref> ans_vector_follows_assign_second_non_starred {8};
    vector<stmt_ref> ans_vector_follows_stmt_second_3_starred {5,11};
    vector<stmt_ref> ans_vector_follows_while_first_stmt_non_starred {3};

    // answer vectors - parent
    vector<stmt_ref> ans_vector_parent_stmt_first_starred {3,5,9};
    vector<stmt_ref> ans_vector_parent_if_first_non_starred {5};
    vector<stmt_ref> ans_vector_parent_stmt_second_9_starred {10};
    vector<stmt_ref> ans_vector_parent_while_first_stmt_non_starred {3,9};

    // answer vectors - modifies
    vector<stmt_ref> ans_vector_modifies_stmt {1,4,6,7,8,10};
    vector<stmt_ref> ans_vector_modifies_assign {4,6,7,8,10};
    vector<stmt_ref> ans_vector_modifies_while {3,9};
    vector<stmt_ref> ans_vector_modifies_stmt_x {3,7};
    vector<proc_ref> ans_vector_proc_modifies {"main"};
    vector<proc_ref> ans_vector_proc_modifies_x {"main"};
    vector<proc_ref> ans_vector_proc_modifies_j {};
    vector<var_ref> ans_vector_var_modifies {"a", "v", "w", "x", "y"};
    vector<var_ref> ans_vector_var_modifies_proc_main {"a", "v", "w", "x", "y"};
    vector<var_ref> ans_vector_var_modifies_stmt_5 {"v", "w", "x", "y"};
    vector<var_ref> ans_vector_var_modifies_while {"v", "x"};


    // answer vectors - uses
    vector<stmt_ref> ans_vector_uses_stmt {2,3,4,5,6,9,10,11};
    vector<stmt_ref> ans_vector_uses_assign_y {6};
    vector<stmt_ref> ans_vector_uses_while {3,9};
    vector<stmt_ref> ans_vector_uses_stmt_x {3,4,5,11};
    vector<proc_ref> ans_vector_proc_uses {"main"};
    vector<proc_ref> ans_vector_proc_uses_x {"main"};
    vector<proc_ref> ans_vector_proc_uses_j {};
    vector<var_ref> ans_vector_var_uses {"b", "v", "x", "y", "z"};
    vector<var_ref> ans_vector_var_uses_proc_main {"b", "v", "x", "y", "z"};
    vector<var_ref> ans_vector_var_uses_stmt_5 {"v", "x", "y", "z"};
    vector<var_ref> ans_vector_var_uses_while {"v", "x", "y"};


    SECTION("follows") {
        REQUIRE(verify_stmt_vector(
            executor.get_all_stmts_follows(stmt_type::STATEMENT, arg_pos::FIRST_ARG, true),
            ans_vector_follows_stmt_first_starred));

        REQUIRE(verify_stmt_vector(
            executor.get_all_stmts_follows(stmt_type::ASSIGN, arg_pos::SECOND_ARG, false),
            ans_vector_follows_assign_second_non_starred));

        REQUIRE(verify_stmt_vector(
            executor.get_all_stmts_follows(stmt_type::STATEMENT, arg_pos::SECOND_ARG, 3, true),
            ans_vector_follows_stmt_second_3_starred));

        REQUIRE(verify_stmt_vector(
            executor.get_all_stmts_follows(stmt_type::WHILE, arg_pos::FIRST_ARG, stmt_type::STATEMENT, false),
            ans_vector_follows_while_first_stmt_non_starred));
    }

    SECTION("parent") {
        REQUIRE(verify_stmt_vector(
            executor.get_all_stmts_parent(stmt_type::STATEMENT, arg_pos::FIRST_ARG, true),
            ans_vector_parent_stmt_first_starred));

        REQUIRE(verify_stmt_vector(
            executor.get_all_stmts_parent(stmt_type::IF, arg_pos::FIRST_ARG, false),
            ans_vector_parent_if_first_non_starred));

        REQUIRE(verify_stmt_vector(
            executor.get_all_stmts_parent(stmt_type::STATEMENT, arg_pos::SECOND_ARG, 9, true),
            ans_vector_parent_stmt_second_9_starred));

        REQUIRE(verify_stmt_vector(
            executor.get_all_stmts_parent(stmt_type::WHILE, arg_pos::FIRST_ARG, stmt_type::STATEMENT, false),
            ans_vector_parent_while_first_stmt_non_starred));
    }

    SECTION("modifies") {
        REQUIRE(verify_stmt_vector(
            executor.get_all_stmts_modifies(stmt_type::STATEMENT),
            ans_vector_modifies_stmt));

        REQUIRE(verify_stmt_vector(
            executor.get_all_stmts_modifies(stmt_type::ASSIGN),
            ans_vector_modifies_assign));

        REQUIRE(verify_stmt_vector(
            executor.get_all_stmts_modifies(stmt_type::WHILE),
            ans_vector_modifies_while));

        REQUIRE(verify_stmt_vector(
            executor.get_all_stmts_modifies(stmt_type::STATEMENT, "x"),
            ans_vector_modifies_stmt_x));

        REQUIRE(verify_proc_vector(
            executor.get_all_procedures_modifies(),
            ans_vector_proc_modifies));

        REQUIRE(verify_proc_vector(
            executor.get_all_procedures_modifies("x"),
            ans_vector_proc_modifies_x));

        REQUIRE(verify_proc_vector(
            executor.get_all_procedures_modifies("j"),
            ans_vector_proc_modifies_j));
        
        REQUIRE(verify_var_vector(
            executor.get_all_variables_modifies(),
            ans_vector_var_modifies));
        
        REQUIRE(verify_var_vector(
            executor.get_all_variables_modifies("main"),
            ans_vector_var_modifies_proc_main));
        
        REQUIRE(verify_var_vector(
            executor.get_all_variables_modifies(5),
            ans_vector_var_modifies_stmt_5));
        
        REQUIRE(verify_var_vector(
            executor.get_all_variables_modifies(stmt_type::WHILE),
            ans_vector_var_modifies_while));

    }

    SECTION("uses") {
        REQUIRE(verify_stmt_vector(
            executor.get_all_stmts_uses(stmt_type::STATEMENT),
            ans_vector_uses_stmt));

        REQUIRE(verify_stmt_vector(
            executor.get_all_stmts_uses(stmt_type::ASSIGN, "y"),
            ans_vector_uses_assign_y));

        REQUIRE(verify_stmt_vector(
            executor.get_all_stmts_uses(stmt_type::WHILE),
            ans_vector_uses_while));

        REQUIRE(verify_stmt_vector(
            executor.get_all_stmts_uses(stmt_type::STATEMENT, "x"),
            ans_vector_uses_stmt_x));
        
        REQUIRE(verify_proc_vector(
            executor.get_all_procedures_uses(),
            ans_vector_proc_uses));

        REQUIRE(verify_proc_vector(
            executor.get_all_procedures_uses("x"),
            ans_vector_proc_uses_x));

        REQUIRE(verify_proc_vector(
            executor.get_all_procedures_uses("j"),
            ans_vector_proc_uses_j));
        
        REQUIRE(verify_var_vector(
            executor.get_all_variables_uses(),
            ans_vector_var_uses));
        
        REQUIRE(verify_var_vector(
            executor.get_all_variables_uses("main"),
            ans_vector_var_uses_proc_main));
        
        REQUIRE(verify_var_vector(
            executor.get_all_variables_uses(5),
            ans_vector_var_uses_stmt_5));
        
        REQUIRE(verify_var_vector(
            executor.get_all_variables_uses(stmt_type::WHILE),
            ans_vector_var_uses_while));

    }

}

TEST_CASE("single_pattern_clause") {
    TNode ast_tree = getMockAST();
    PKBBuilder pkb_builder(ast_tree);
    PKBQueryController pkb_query_controller(pkb_builder.build());

    ActionsExecutor executor(pkb_query_controller);

    // TODO(plosslaw): add more test cases
    // answer vectors - assignments
    vector<stmt_ref> ans_vector_assign_w__y_ {6};
    vector<stmt_ref> ans_vector_assign_x__ {4,7};
    vector<stmt_ref> ans_vector_assign___v_minus_1 {10};
    vector<stmt_ref> ans_vector_assign____y_ {6};

    // answer vectors - variables
    vector<var_ref> ans_vector_var_y_times_2 {"w"};
    vector<var_ref> ans_vector_var__x_ {"x"};
    vector<var_ref> ans_vector_var__ {"v", "w", "x", "y"};
    vector<var_ref> ans_vector_var_v {};

    SECTION("assignments") {
        pattern trial_pattern;
        trial_pattern.lvalue = "w";
        trial_pattern.rvalue = "_y_";

        REQUIRE(verify_stmt_vector(
            executor.get_all_stmts_pattern(trial_pattern),
            ans_vector_assign_w__y_));

        trial_pattern.lvalue = "x";
        trial_pattern.rvalue = "_";

        REQUIRE(verify_stmt_vector(
            executor.get_all_stmts_pattern(trial_pattern),
            ans_vector_assign_x__));

        trial_pattern.lvalue = "_";
        trial_pattern.rvalue = "v - 1";

        REQUIRE(verify_stmt_vector(
            executor.get_all_stmts_pattern(trial_pattern),
            ans_vector_assign___v_minus_1));

        trial_pattern.lvalue = "_";
        trial_pattern.rvalue = "_y_";

        REQUIRE(verify_stmt_vector(
            executor.get_all_stmts_pattern(trial_pattern),
            ans_vector_assign____y_));
    }

    SECTION("variables") {
        REQUIRE(verify_var_vector(
            executor.get_all_variables_pattern_assign("y * 2"),
            ans_vector_var_y_times_2));

        REQUIRE(verify_var_vector(
            executor.get_all_variables_pattern_assign("_x_"),
            ans_vector_var__x_));

        REQUIRE(verify_var_vector(
            executor.get_all_variables_pattern_assign(),
            ans_vector_var__));
        
        REQUIRE(verify_var_vector(
            executor.get_all_variables_pattern_assign("v"),
            ans_vector_var_v));
    }
}