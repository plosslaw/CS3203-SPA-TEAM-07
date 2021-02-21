#include "MockAST.h"
#include "../../spa/src/ActionsExecutor.h"
#include "../../spa/src/ActionsGenerator.h"
#include "../../spa/src/PKBBuilder.hpp"
#include "catch.hpp"
#include <iostream>
#include <vector>

using namespace std;

//using mockast

bool verify_stmts(vector<string> test_vector, vector<string> ans_vector){
    if (test_vector.size() != ans_vector.size()) {
        return false;
    }
    unordered_set<string> unique_set;
    for (string ele : ans_vector) {
        unique_set.insert(ele);
    }

    for (string ele : test_vector) {
        if (unique_set.find(ele) == unique_set.end()) {
            return false;
        }
    }

    return true;
}


class Map_Query_unit {
    public:
        QueryMap mapquery;

        void initialise_mock_ast_querymap() {
            //vector<PayLoad> declaration_list{}
            PayLoad stmt(SINGLE, Single::STATEMENT, vector<string>{"s1"});
            PayLoad stmt2(SINGLE, Single::STATEMENT, vector<string>{"s2"});
            PayLoad stmt3(SINGLE, Single::STATEMENT, vector<string>{"s3"});
            PayLoad re(SINGLE, Single::READ, vector<string>{"re1"});
            PayLoad pn(SINGLE, Single::PRINT, vector<string>{"pn1"});
            PayLoad call(SINGLE, Single::CALL, vector<string>{"ca1"});
            PayLoad w(SINGLE, Single::WHILE, vector<string>{"w1"});
            PayLoad ifs(SINGLE, Single::IF, vector<string>{"ifs1"});
            PayLoad a(SINGLE, Single::ASSIGN, vector<string>{"a1"});
            PayLoad a2(SINGLE, Single::ASSIGN, vector<string>{"a2"});
            PayLoad c(SINGLE, Single::CONSTANT, vector<string>{"c1"});
            PayLoad var(SINGLE, Single::VARIABLE, vector<string>{"v1"});
            PayLoad var2(SINGLE, Single::VARIABLE, vector<string>{"v2"});
            PayLoad proc(SINGLE, Single::PROCEDURE, vector<string>{"p1"});
            vector<PayLoad> declaration_lst{stmt, re, pn, call,w,ifs,a,c,var,proc};
            mapquery.addItem(ClauseType::DECLARATION, stmt);
            mapquery.addItem(ClauseType::DECLARATION, stmt2);
            mapquery.addItem(ClauseType::DECLARATION, stmt3);
            mapquery.addItem(ClauseType::DECLARATION, re);
            mapquery.addItem(ClauseType::DECLARATION, pn);
            mapquery.addItem(ClauseType::DECLARATION, call);
            mapquery.addItem(ClauseType::DECLARATION, ifs);
            mapquery.addItem(ClauseType::DECLARATION, w);
            mapquery.addItem(ClauseType::DECLARATION, a);
            mapquery.addItem(ClauseType::DECLARATION, a2);
            mapquery.addItem(ClauseType::DECLARATION, c);
            mapquery.addItem(ClauseType::DECLARATION, var);
            mapquery.addItem(ClauseType::DECLARATION, var2);
            mapquery.addItem(ClauseType::DECLARATION, proc);
        }

        void add_Select_synonym(PayLoad syn) {
            mapquery.addItem(ClauseType::SELECT, syn);
        }
        void add_Such_that(PayLoad st) {
            mapquery.addItem(ClauseType::SUCHTHAT, st);
        }
        
        void add_pattern(PayLoad pattern1) {
            mapquery.addItem(ClauseType::PATTERN, pattern1);
        }

        Map_Query_unit() {
            QueryMap q;
            mapquery = q;    
        }
};

vector<string> test_select_only(PayLoad select_payload) {
    TNode ast_tree = getMockAST();
    PKBBuilder pkb_builder(ast_tree);
    PKBQueryController pkb_query_controller(pkb_builder.build());
    ActionsExecutor executor(pkb_query_controller);
    Map_Query_unit map_unit;
    map_unit.initialise_mock_ast_querymap();    
    map_unit.add_Select_synonym(select_payload);   
    QueryMap mapQuery = map_unit.mapquery;
    ActionsGenerator generator(mapQuery,executor);
    vector<string> output = generator.TraverseQueryMap();
    return output;
}

vector<string> test_select_such_that_only(PayLoad select_payload, PayLoad such_that_payload) {
    TNode ast_tree = getMockAST();
    PKBBuilder pkb_builder(ast_tree);
    PKBQueryController pkb_query_controller(pkb_builder.build());
    ActionsExecutor executor(pkb_query_controller);
    Map_Query_unit map_unit;
    map_unit.initialise_mock_ast_querymap();
    map_unit.add_Select_synonym(select_payload);    
    map_unit.add_Such_that(such_that_payload);   
    QueryMap mapQuery = map_unit.mapquery;
    ActionsGenerator generator(mapQuery,executor);
    vector<string> output = generator.TraverseQueryMap();
    return output;
}
vector<string> test_select_pattern_only(PayLoad select_payload, PayLoad pattern_payload) {
    TNode ast_tree = getMockAST();
    PKBBuilder pkb_builder(ast_tree);
    PKBQueryController pkb_query_controller(pkb_builder.build());
    ActionsExecutor executor(pkb_query_controller);
    Map_Query_unit map_unit;
    map_unit.initialise_mock_ast_querymap();
    map_unit.add_Select_synonym(select_payload); 
    map_unit.add_pattern(pattern_payload);     
    QueryMap mapQuery = map_unit.mapquery;
    ActionsGenerator generator(mapQuery,executor);
    vector<string> output = generator.TraverseQueryMap();
    return output;
}

vector<string> test_select_such_that_pattern_only(PayLoad select_payload, PayLoad such_that_payload,PayLoad pattern_payload) {
    TNode ast_tree = getMockAST();
    PKBBuilder pkb_builder(ast_tree);
    PKBQueryController pkb_query_controller(pkb_builder.build());
    ActionsExecutor executor(pkb_query_controller);
    Map_Query_unit map_unit;
    map_unit.initialise_mock_ast_querymap();
    map_unit.add_Select_synonym(select_payload); 
    map_unit.add_Such_that(such_that_payload); 
    map_unit.add_pattern(pattern_payload);     
    QueryMap mapQuery = map_unit.mapquery;
    ActionsGenerator generator(mapQuery,executor);
    vector<string> output = generator.TraverseQueryMap();
    return output;
}

TEST_CASE("TEST PREPROCESSING OF MAP QUERY") {
    vector<string> stmt_lst{"1","2","3","4","5","6","7","8","9","10","11"};
    vector<string> read_lst{"1"};
    vector<string> assignment_lst{"4","6","7","8","10"};
    vector<string> constant_lst{"1","2","42"};
    vector<string> if_lst{"5"};
    vector<string> print_lst{"2","11"};
    vector<string> procedure_lst{"main"};
    vector<string> variable_lst{"a","b","x","y","z","v","w"};
    vector<string> while_lst{"3","9"};
    vector<string> call_lst({});
    
    TNode ast_tree = getMockAST();
    PKBBuilder pkb_builder(ast_tree);
    PKBQueryController pkb_query_controller(pkb_builder.build());
    ActionsExecutor executor(pkb_query_controller);
    Map_Query_unit map_unit;
    map_unit.initialise_mock_ast_querymap();
    QueryMap mapQuery = map_unit.mapquery;
    ActionsGenerator generator(mapQuery,executor);
    unordered_map<Single, 
    unordered_map<string, vector<string>>>mapStorage = generator.preprocess();
    //vector<string> mapStorage = generator.preprocess();
    SECTION("TEST individual storages") {
        REQUIRE(verify_stmts(mapStorage[Single::STATEMENT]["s1"], stmt_lst));
        REQUIRE(verify_stmts(mapStorage[Single::READ]["re1"],read_lst));
        REQUIRE(verify_stmts(mapStorage[Single::PRINT]["pn1"], print_lst));
        REQUIRE(verify_stmts(mapStorage[Single::CALL]["ca1"], call_lst));
        REQUIRE(verify_stmts(mapStorage[Single::WHILE]["w1"], while_lst));
        REQUIRE(verify_stmts(mapStorage[Single::IF]["ifs1"], if_lst));
        REQUIRE(verify_stmts(mapStorage[Single::ASSIGN]["a1"], assignment_lst));
        REQUIRE(verify_stmts(mapStorage[Single::CONSTANT]["c1"], constant_lst));
        REQUIRE(verify_stmts(mapStorage[Single::VARIABLE]["v1"], variable_lst));
        REQUIRE(verify_stmts(mapStorage[Single::PROCEDURE]["p1"], procedure_lst));
    }
}

TEST_CASE("SELECT ONLY") {
    vector<string> stmt_lst{"1","2","3","4","5","6","7","8","9","10","11"};
    vector<string> read_lst{"1"};
    vector<string> assignment_lst{"4","6","7","8","10"};
    vector<string> constant_lst{"1","2","42"};
    vector<string> if_lst{"5"};
    vector<string> print_lst{"2","11"};
    vector<string> procedure_lst{"main"};
    vector<string> variable_lst{"a","b","x","y","z","v","w"};
    vector<string> while_lst{"3","9"};
    vector<string> call_lst({});

    SECTION("select stmts") {
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        vector<string> result = test_select_only(syn);
        REQUIRE(verify_stmts(result, stmt_lst));
    }
    SECTION("select read") {
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"re1"});
        vector<string> result = test_select_only(syn);
        REQUIRE(verify_stmts(result, read_lst));
    }
    SECTION("select print") {
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"pn1"});
        vector<string> result = test_select_only(syn);
        REQUIRE(verify_stmts(result, print_lst));
    }
    SECTION("select call") {
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"ca1"});
        vector<string> result = test_select_only(syn);
        REQUIRE(verify_stmts(result, call_lst));
    }
    SECTION("select while") {
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"w1"});
        vector<string> result = test_select_only(syn);
        REQUIRE(verify_stmts(result, while_lst));
    }
    SECTION("select if") {
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"ifs1"});
        vector<string> result = test_select_only(syn);
        REQUIRE(verify_stmts(result, if_lst));
    }
    SECTION("select assignment") {
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        vector<string> result = test_select_only(syn);
        REQUIRE(verify_stmts(result, assignment_lst));
    }
    SECTION("select constant") {
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"c1"});
        vector<string> result = test_select_only(syn);
        REQUIRE(verify_stmts(result, constant_lst));
    }
    SECTION("select variable") {
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"v1"});
        vector<string> result = test_select_only(syn);
        REQUIRE(verify_stmts(result, variable_lst));
    }
    SECTION("select procedure") {
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"p1"});
        vector<string> result = test_select_only(syn);
        REQUIRE(verify_stmts(result, procedure_lst));
    }
}

TEST_CASE("SELECT WITH SUCH THAT CLAUSE: FOLLOWS") {
    vector<string> stmt_lst{"1","2","3","4","5","6","7","8","9","10","11"};
    vector<string> read_lst{"1"};
    vector<string> assignment_lst{"4","6","7","8","10"};
    vector<string> constant_lst{"1","2","42"};
    vector<string> if_lst{"5"};
    vector<string> print_lst{"2","11"};
    vector<string> procedure_lst{"main"};
    vector<string> variable_lst{"a","b","x","y","z","v","w"};
    vector<string> while_lst{"3","9"};
    vector<string> call_lst({});
    
    SECTION("Select s1 follows(s1,s2") {
        vector<string> correct_ans{"8", "5", "7", "1", "2", "3"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"s1","s2"});
        vector<string> output = test_select_such_that_only(syn, st);
    }
    SECTION("Select s2 follows(s1,s2") {
        vector<string> correct_ans{"2","3","5","8","9","11"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s2"});
        PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"s1","s2"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 follows(s1,s2") {
        vector<string> correct_ans = assignment_lst;
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"s1","s2"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 follows(a1,s1") {
        vector<string> correct_ans{"7","8"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"a1","s1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 follows(s1,a1") { //kiv
        vector<string> correct_ans{"8"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"s1","a1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 follows(s1,w1") {
        vector<string> correct_ans{"2","8"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"s1","w1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select w1 follows(w1,ifs1") {
        vector<string> correct_ans{"3"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"w1"});
        PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"w1","ifs1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select ifs1 follows(ifs1,pn1") {//kiv
        vector<string> correct_ans{"5"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"ifs1"});
        PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"ifs1","pn1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 follows(s1,3") {
        vector<string> correct_ans{"2"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"s1","3"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 follows(s1,4") {
        vector<string> correct_ans{"None"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"s1","4"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 follows(4,s1") {
        vector<string> correct_ans{"None"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"4","s1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 follows(7,s1") {
        vector<string> correct_ans{"8"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"7","s1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 follows(7,a1") {
        vector<string> correct_ans{"8"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"7","a1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select w1 follows(2,w1") {
        vector<string> correct_ans{"3"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"w1"});
        PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"2","w1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 follows(2,3") {
        vector<string> correct_ans = stmt_lst;
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"2","3"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
}

TEST_CASE("SELECT WITH SUCH THAT CLAUSE: FOLLOWS*") {
    vector<string> stmt_lst{"1","2","3","4","5","6","7","8","9","10","11"};
    vector<string> read_lst{"1"};
    vector<string> assignment_lst{"4","6","7","8","10"};
    vector<string> constant_lst{"1","2","42"};
    vector<string> if_lst{"5"};
    vector<string> print_lst{"2","11"};
    vector<string> procedure_lst{"main"};
    vector<string> variable_lst{"a","b","x","y","z","v","w"};
    vector<string> while_lst{"3","9"};
    vector<string> call_lst({});
    
    SECTION("Select s1 follows*(s1,s2") {
        vector<string> correct_ans{"1","2","3","5","7","8"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::FOLLOWST, std::vector<std::string>{"s1","s2"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s2 follows*(s1,s2") {
        vector<string> correct_ans{"2","3","5","8","9","11"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s2"});
        PayLoad st(PAIR, Pair::FOLLOWST, std::vector<std::string>{"s1","s2"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select v1 follows*(s1,s2") {
        vector<string> correct_ans = variable_lst;
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"v1"});
        PayLoad st(PAIR, Pair::FOLLOWST, std::vector<std::string>{"s1","s2"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s2 follows*(w1,s2") {
        //kiv
        vector<string> correct_ans{"5","11"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s2"});
        PayLoad st(PAIR, Pair::FOLLOWST, std::vector<std::string>{"w1","s2"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s2 follows*(ifs1,s2") {
        //kiv
        vector<string> correct_ans{"11"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s2"});
        PayLoad st(PAIR, Pair::FOLLOWST, std::vector<std::string>{"ifs1","s2"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 follows*(s1,5") {
        vector<string> correct_ans{"1","2","3"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::FOLLOWST, std::vector<std::string>{"s1","5"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s2 follows*(5,s2") {
        vector<string> correct_ans{"11"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s2"});
        PayLoad st(PAIR, Pair::FOLLOWST, std::vector<std::string>{"5","s2"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 follows*(a1,s2") {
        vector<string> correct_ans{"7","8"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad st(PAIR, Pair::FOLLOWST, std::vector<std::string>{"a1","s2"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 follows*(1,2") {
        vector<string> correct_ans = assignment_lst;
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad st(PAIR, Pair::FOLLOWST, std::vector<std::string>{"1","2"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 follows*(1,4") {
        vector<string> correct_ans{"None"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad st(PAIR, Pair::FOLLOWST, std::vector<std::string>{"1","4"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }    
}

TEST_CASE("SELECT WITH SUCH THAT CLAUSE: Parent") {
    vector<string> stmt_lst{"1","2","3","4","5","6","7","8","9","10","11"};
    vector<string> read_lst{"1"};
    vector<string> assignment_lst{"4","6","7","8","10"};
    vector<string> constant_lst{"1","2","42"};
    vector<string> if_lst{"5"};
    vector<string> print_lst{"2","11"};
    vector<string> procedure_lst{"main"};
    vector<string> variable_lst{"a","b","x","y","z","v","w"};
    vector<string> while_lst{"3","9"};
    vector<string> call_lst({});
    
    SECTION("Select s1 parent(s1,s2)") {
        vector<string> correct_ans{"9", "5", "3"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::PARENT, std::vector<std::string>{"s1","s2"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s2 parent(s1,s2)") {
        vector<string> correct_ans{"10", "4", "8", "7", "6", "9"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s2"});
        PayLoad st(PAIR, Pair::PARENT, std::vector<std::string>{"s1","s2"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s3 parent(s1,s2)") {
        vector<string> correct_ans = stmt_lst;
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s3"});
        PayLoad st(PAIR, Pair::PARENT, std::vector<std::string>{"s1","s2"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 parent(3,s1)") {
        vector<string> correct_ans{"4"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::PARENT, std::vector<std::string>{"3","s1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 parent(s1,10)") {
        vector<string> correct_ans{"9"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::PARENT, std::vector<std::string>{"s1","10"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 parent(s1,7)") {
        vector<string> correct_ans{"5"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::PARENT, std::vector<std::string>{"s1","7"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 parent(s1,9)") {
        vector<string> correct_ans{"5"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::PARENT, std::vector<std::string>{"s1","9"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 parent(5,9)") {
        vector<string> correct_ans = stmt_lst;
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::PARENT, std::vector<std::string>{"5","9"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 parent(5,6)") {
        vector<string> correct_ans = stmt_lst;
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::PARENT, std::vector<std::string>{"5","6"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 parent(1,3)") {
        vector<string> correct_ans{"None"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::PARENT, std::vector<std::string>{"1","3"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select v1 parent(5,7)") {
        vector<string> correct_ans = variable_lst;
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"v1"});
        PayLoad st(PAIR, Pair::PARENT, std::vector<std::string>{"5","7"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 parent(1,6)") {
        vector<string> correct_ans{"None"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::PARENT, std::vector<std::string>{"1","6"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select w1 parent(w1,s1)") {
        vector<string> correct_ans{"3","9"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"w1"});
        PayLoad st(PAIR, Pair::PARENT, std::vector<std::string>{"w1","s1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select ifs1 parent(ifs1,s1)") {
        vector<string> correct_ans{"5"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"ifs1"});
        PayLoad st(PAIR, Pair::PARENT, std::vector<std::string>{"ifs1","s1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 parent(ifs1,s1)") {
        //kiv
        vector<string> correct_ans{"6","7","8","9"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::PARENT, std::vector<std::string>{"ifs1","s1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
}

TEST_CASE("SELECT WITH SUCH THAT CLAUSE: Parent*") {
    vector<string> stmt_lst{"1","2","3","4","5","6","7","8","9","10","11"};
    vector<string> read_lst{"1"};
    vector<string> assignment_lst{"4","6","7","8","10"};
    vector<string> constant_lst{"1","2","42"};
    vector<string> if_lst{"5"};
    vector<string> print_lst{"2","11"};
    vector<string> procedure_lst{"main"};
    vector<string> variable_lst{"a","b","x","y","z","v","w"};
    vector<string> while_lst{"3","9"};
    vector<string> call_lst({});
    
    SECTION("Select s1 parent*(s1,s2)") {
        vector<string> correct_ans{"9", "5", "3"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::PARENTT, std::vector<std::string>{"s1","s2"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s2 parent*(s1,s2)") {
        vector<string> correct_ans{"4","6","7","8","9","10"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s2"});
        PayLoad st(PAIR, Pair::PARENTT, std::vector<std::string>{"s1","s2"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s3 parent*(s1,s2)") {
        vector<string> correct_ans = stmt_lst;
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s3"});
        PayLoad st(PAIR, Pair::PARENTT, std::vector<std::string>{"s1","s2"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 parent*(a1,s1)") {
        vector<string> correct_ans{"None"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad st(PAIR, Pair::PARENTT, std::vector<std::string>{"a1","s1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 parent*(s1,10)") {
        vector<string> correct_ans{"5","9"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::PARENTT, std::vector<std::string>{"s1","10"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 parent*(s1,7)") {
        vector<string> correct_ans{"5"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::PARENTT, std::vector<std::string>{"s1","7"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 parent*(7,s1)") {
        vector<string> correct_ans{"None"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::PARENTT, std::vector<std::string>{"7","s1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 parent*(s1,6)") {
        vector<string> correct_ans{"5"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::PARENTT, std::vector<std::string>{"s1","6"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select v1 parent*(s1,6)") {
        vector<string> correct_ans = variable_lst;
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"v1"});
        PayLoad st(PAIR, Pair::PARENTT, std::vector<std::string>{"s1","6"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 parent*(s1,10)") {
        vector<string> correct_ans{"5","9"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::PARENTT, std::vector<std::string>{"s1","10"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 parent*(5,9)") {
        vector<string> correct_ans = stmt_lst;
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::PARENTT, std::vector<std::string>{"5","9"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
}

TEST_CASE("SELECT WITH SUCH THAT CLAUSE: USES") {
    vector<string> stmt_lst{"1","2","3","4","5","6","7","8","9","10","11"};
    vector<string> read_lst{"1"};
    vector<string> assignment_lst{"4","6","7","8","10"};
    vector<string> constant_lst{"1","2","42"};
    vector<string> if_lst{"5"};
    vector<string> print_lst{"2","11"};
    vector<string> procedure_lst{"main"};
    vector<string> variable_lst{"a","b","x","y","z","v","w"};
    vector<string> while_lst{"3","9"};
    vector<string> call_lst({});
    
    SECTION("Select a1 uses(a1,v1") {
        vector<string> correct_ans{"4","6","10"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"a1","v1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select v1 uses(a1,v1") {
        vector<string> correct_ans{"x","y","v"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"v1"});
        PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"a1","v1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select pn1 uses(pn1,v1") {
        vector<string> correct_ans{"2","11"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"pn1"});
        PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"pn1","v1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select v1 uses(pn1,v1") {
        vector<string> correct_ans{"b","x"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"v1"});
        PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"pn1","v1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 uses(s1,v1") {
        vector<string> correct_ans{"4","6","10","2","11","3","5","9"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"s1","v1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select v1 uses(s1,v1") {
        vector<string> correct_ans{"x","y","v","b","z"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"v1"});
        PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"s1","v1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select w1 uses(w1,v1") {
        vector<string> correct_ans{"3","9"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"w1"});
        PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"s1","v1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 uses(a1,\"x\"") {
        vector<string> correct_ans{"4"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"a1","x"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select w1 uses(a1,\"y\"") {
        vector<string> correct_ans = while_lst;
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"w1"});
        PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"a1","y"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select w1 uses(a1,\"z\"") {
        vector<string> correct_ans{"None"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"w1"});
        PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"a1","z"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select ifs1 uses(ifs1,\"z\"") {
        vector<string> correct_ans{"5"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"ifs1"});
        PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"ifs1","z"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select w1 uses(w1,\"v\"") {
        vector<string> correct_ans{"9"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"w1"});
        PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"w1","v"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 uses(4,\"x\"") {
        vector<string> correct_ans = stmt_lst;
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"4","x"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    // SECTION("Select v1 uses(p1,\'b\'") {
    //     vector<string> correct_ans = variable_lst;
    //     PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"v1"});
    //     PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"p1","b"});
    //     vector<string> output = test_select_such_that_only(syn, st);
    //     REQUIRE(output == vector<string>{});
    //     REQUIRE(verify_stmts(output, correct_ans));
    // }
    SECTION("Select v1 uses(2,\'b\'") {
        vector<string> correct_ans = variable_lst;
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"v1"});
        PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"2","b"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select p1 uses(10,\'v\'") {
        vector<string> correct_ans = procedure_lst;
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"p1"});
        PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"10","v"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }        
    SECTION("Select s1 uses(s1,\"z") {
        vector<string> correct_ans{"5"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"s1","z"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }     
}

TEST_CASE("SELECT WITH SUCH THAT CLAUSE: MODIFIES") {
    vector<string> stmt_lst{"1","2","3","4","5","6","7","8","9","10","11"};
    vector<string> read_lst{"1"};
    vector<string> assignment_lst{"4","6","7","8","10"};
    vector<string> constant_lst{"1","2","42"};
    vector<string> if_lst{"5"};
    vector<string> print_lst{"2","11"};
    vector<string> procedure_lst{"main"};
    vector<string> variable_lst{"a","b","x","y","z","v","w"};
    vector<string> while_lst{"3","9"};
    vector<string> call_lst({});
    
    SECTION("Select a1 modifies(a1,v1") {
        vector<string> correct_ans{"4","6","10","7","8"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"a1","v1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select v1 modifies(a1,v1") {
        vector<string> correct_ans{"x","y","v","w"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"v1"});
        PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"a1","v1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 modifies(a1,v1") {
        vector<string> correct_ans = stmt_lst;
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"a1","v1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 modifies(s1,v1") {
        vector<string> correct_ans{"4","6","7","8","10","1","3","5","9"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"s1","v1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select v1 modifies(s1,v1") {
        vector<string> correct_ans{"v","y","x","w","a"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"v1"});
        PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"s1","v1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select re1 modifies(re1,v1") {
        vector<string> correct_ans{"1"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"re1"});
        PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"re1","v1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select v1 modifies(re1,v1") {
        vector<string> correct_ans{"a"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"v1"});
        PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"re1","v1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    // SECTION("Select p1 modifies(p1,v1") {
    //     vector<string> correct_ans{"main"};
    //     PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"p1"});
    //     PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"p1","v1"});
    //     vector<string> output = test_select_such_that_only(syn, st);
    //     REQUIRE(verify_stmts(output, correct_ans));
    // }
    SECTION("Select w1 modifies(w1,v1") {
        vector<string> correct_ans{"3","9"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"w1"});
        PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"w1","v1"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 modifies(5,\"y") {
        vector<string> correct_ans = stmt_lst;
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"5","y"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select s1 modifies(5,\"w") {
        vector<string> correct_ans = stmt_lst;
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"5","w"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }     
    SECTION("Select s1 modifies(s1,\"y") {
        vector<string> correct_ans{"5","8"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"s1","y"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }      
    SECTION("Select s1 modifies(s1,\"v") {
        vector<string> correct_ans{"5","9","10"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"s1","v"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }     
    SECTION("Select s1 modifies(s1,\"z") {
        vector<string> correct_ans{"None"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"s1","z"});
        vector<string> output = test_select_such_that_only(syn, st);
        REQUIRE(verify_stmts(output, correct_ans));
    }   
}

TEST_CASE("SELECT WITH SUCH THAT CLAUSE: Pattern") {
    vector<string> stmt_lst{"1","2","3","4","5","6","7","8","9","10","11"};
    vector<string> read_lst{"1"};
    vector<string> assignment_lst{"4","6","7","8","10"};
    vector<string> constant_lst{"1","2","42"};
    vector<string> if_lst{"5"};
    vector<string> print_lst{"2","11"};
    vector<string> procedure_lst{"main"};
    vector<string> variable_lst{"a","b","x","y","z","v","w"};
    vector<string> while_lst{"3","9"};
    vector<string> call_lst({});
    
    SECTION("Select a1 pattern a1(v1,_)") {
        vector<string> correct_ans{"4","6","10","7","8"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","v1","_"});
        vector<string> output = test_select_pattern_only(syn, pt);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select v1 pattern a1(v1,_)") {
        vector<string> correct_ans{"w", "v","x","y"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"v1"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","v1","_"});
        vector<string> output = test_select_pattern_only(syn, pt);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select w1 pattern a1(v1,_)") {
        vector<string> correct_ans{"3","9"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"w1"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","v1","_"});
        vector<string> output = test_select_pattern_only(syn, pt);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 pattern a1(v1,_*2)") {
        vector<string> correct_ans{"6"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","v1","_*2"});
        vector<string> output = test_select_pattern_only(syn, pt);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 pattern a1(_,_)") {
        vector<string> correct_ans{"4","6","10","7","8"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","_","_"});
        vector<string> output = test_select_pattern_only(syn, pt);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 pattern a1(_,\"-1\")") {
        vector<string> correct_ans{"4","10"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","_","_-1"});
        vector<string> output = test_select_pattern_only(syn, pt);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 pattern a1(_,\"_\"*\"_") {
        vector<string> correct_ans{"6"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","_","_*_"});
        vector<string> output = test_select_pattern_only(syn, pt);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 pattern a1(\"x\",_") {
        vector<string> correct_ans{"4","7"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","x","_"});
        vector<string> output = test_select_pattern_only(syn, pt);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 pattern a1(\"w\",_)") {
        vector<string> correct_ans{"8"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","y","_"});
        vector<string> output = test_select_pattern_only(syn, pt);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 pattern a1(_,_* 2)") {
        vector<string> correct_ans{"6"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","_","_* 2"});
        vector<string> output = test_select_pattern_only(syn, pt);
        
        //REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 pattern a1(_,_*2)") {
        vector<string> correct_ans{"6"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","v1","_*2"});
        vector<string> output = test_select_pattern_only(syn, pt);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 pattern a1(_,_*2)") {
        vector<string> correct_ans{"6"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","w","y*2"});
        vector<string> output = test_select_pattern_only(syn, pt);
        //REQUIRE(verify_stmts(output, correct_ans));
    }
}


TEST_CASE("SELECT VALUE WITH BOTH SUCH THAT AND PATTERN") {
//test_select_such_that_pattern_only
    SECTION("Select a1 uses(a1,v1) pattern a1(v1,_)") {
        vector<string> correct_ans{"4","10"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"a1", "v1"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","v1","_"});
        vector<string> output = test_select_such_that_pattern_only(syn,st,pt);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 uses(a1,v1) pattern a2(v1,_)") {
        vector<string> correct_ans{"4","6","10"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"a1", "v1"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a2","v1","_"});
        vector<string> output = test_select_such_that_pattern_only(syn,st,pt);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a2 uses(a1,v1) pattern a2(v2,_)") {
        vector<string> correct_ans{"4","6","7","8","10"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a2"});
        PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"a1", "v1"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a2","v2","_"});
        vector<string> output = test_select_such_that_pattern_only(syn,st,pt);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 follows*(a1,a2) pattern a1(v1,_)") {
        vector<string> correct_ans{"7"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad st(PAIR, Pair::FOLLOWST, std::vector<std::string>{"a1", "a2"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","v1","_"});
        vector<string> output = test_select_such_that_pattern_only(syn,st,pt);

        REQUIRE(verify_stmts(output, correct_ans));
    }   
    SECTION("Select v1 follows*(a1,a2) pattern a1(v1,_)") {
        vector<string> correct_ans{"x"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"v1"});
        PayLoad st(PAIR, Pair::FOLLOWST, std::vector<std::string>{"a1", "a2"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","v1","_"});
        vector<string> output = test_select_such_that_pattern_only(syn,st,pt);
        REQUIRE(verify_stmts(output, correct_ans));
    }
       
    SECTION("Select a1 parent*(a1,a2) pattern a1(v1,_)") {
        vector<string> correct_ans{"None"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad st(PAIR, Pair::PARENTT, std::vector<std::string>{"a1", "a2"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","v1","_"});
        vector<string> output = test_select_such_that_pattern_only(syn,st,pt);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 modifies(a1,v1) pattern a1(v1,_)") {
        vector<string> correct_ans{ "8", "6", "4", "7", "10"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"a1", "v1"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","v1","_"});
        vector<string> output = test_select_such_that_pattern_only(syn,st,pt);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select v1 modifies(a1,v1) pattern a1(v1,_)") {
        vector<string> correct_ans{ "x", "w", "y", "v"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"v1"});
        PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"a1", "v1"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","v1","_"});
        vector<string> output = test_select_such_that_pattern_only(syn,st,pt);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 modifies(a2,v1) pattern a1(v1,_)") {
        vector<string> correct_ans{ "8", "6", "4", "7", "10"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"a2", "v1"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","v1","_"});
        vector<string> output = test_select_such_that_pattern_only(syn,st,pt);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select a1 uses(w1,v1) pattern a1(v1,_)") { 
        vector<string> correct_ans{ "10", "7", "4", "8"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"w1", "v1"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","v1","_"});
        vector<string> output = test_select_such_that_pattern_only(syn,st,pt);
        REQUIRE(verify_stmts(output, correct_ans));
    }
    SECTION("Select v1 uses(w1,v1) pattern a1(v1,_)") {
        vector<string> correct_ans{ "v","x","y"};
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"v1"});
        PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"w1", "v1"});
        PayLoad pt(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","v1","_"});
        vector<string> output = test_select_such_that_pattern_only(syn,st,pt);
        REQUIRE(verify_stmts(output, correct_ans));
    }

}