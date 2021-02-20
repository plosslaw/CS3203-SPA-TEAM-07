#include "MockAST.h"
#include "../../spa/src/ActionsExecutor.h"
#include "../../spa/src/ActionsGenerator.h"
#include "../../spa/src/PKBBuilder.hpp"
#include "catch.hpp"
#include <iostream>
#include <vector>

using namespace std;

//using mockast

class Map_Query_unit {
    public:
        unordered_map<Single, 
        unordered_map<string, vector<string>>> map_storage_MOCKAST;
        unordered_map<string, Single> store_declaration_MOCKAST;
        QueryMap mapquery;
        void initialise_mock_ast_map_storage_storedeclaration() {
            vector<string> stmt_lst({"1","2","3","4","5","6","7","8","9","10"});
            vector<string> read_lst({"1"});
            vector<string> assignment_lst({"4","6","7","8","10"});
            vector<string> constant_lst({"1","2","42"});
            vector<string> if_lst({"5"});
            vector<string> print_lst({"2","3"});
            vector<string> procedure_lst({"main"});
            vector<string> variable_lst({"a","b","x","y","z","v","w"});
            vector<string> while_lst({"3","9"});
            vector<string> call_lst({});
            unordered_map<string, vector<string>> procedureStorage;
            procedureStorage["p1"] = procedure_lst;
            unordered_map<string, vector<string>> stmtStorage;
            stmtStorage["s1"] = stmt_lst;
            stmtStorage["s2"] = stmt_lst;
            stmtStorage["s3"] = stmt_lst;
            unordered_map<string, vector<string>> readStorage;
            readStorage["re1"] = read_lst;
            unordered_map<string, vector<string>> printStorage;
            printStorage["pn1"] = print_lst;
            unordered_map<string, vector<string>> assignStorage;
            assignStorage["a1"] = assignment_lst;
            assignStorage["a2"] = assignment_lst;
            unordered_map<string, vector<string>> whileStorage;
            whileStorage["w1"] = while_lst;
            unordered_map<string, vector<string>> ifStorage;
            ifStorage["ifs1"] = if_lst;
            unordered_map<string, vector<string>> constantStorage;
            constantStorage["c1"] = constant_lst;
            unordered_map<string, vector<string>> variableStorage;
            variableStorage["v1"] = variable_lst;
            variableStorage["v2"] = variable_lst;
            unordered_map<string, vector<string>> callStorage;
            callStorage["ca1"] = call_lst;
            map_storage_MOCKAST[Single::PROCEDURE] = procedureStorage;
            map_storage_MOCKAST[Single::STATEMENT] = stmtStorage;
            map_storage_MOCKAST[Single::READ] = readStorage;
            map_storage_MOCKAST[Single::PRINT] = printStorage;
            map_storage_MOCKAST[Single::ASSIGN] = assignStorage;
            map_storage_MOCKAST[Single::WHILE] = whileStorage;
            map_storage_MOCKAST[Single::IF] = ifStorage;
            map_storage_MOCKAST[Single::CONSTANT] = constantStorage;
            map_storage_MOCKAST[Single::VARIABLE] = variableStorage;

            store_declaration_MOCKAST["s1"] = Single::STATEMENT;
            store_declaration_MOCKAST["s2"] = Single::STATEMENT;
            store_declaration_MOCKAST["s3"] = Single::STATEMENT;
            store_declaration_MOCKAST["re1"] = Single::READ;
            store_declaration_MOCKAST["pn1"] = Single::PRINT;
            store_declaration_MOCKAST["ca1"] = Single::CALL;
            store_declaration_MOCKAST["w1"] = Single::WHILE;
            store_declaration_MOCKAST["ifs1"] = Single::IF;
            store_declaration_MOCKAST["a1"] = Single::ASSIGN;
            store_declaration_MOCKAST["a2"] = Single::ASSIGN;
            store_declaration_MOCKAST["c1"] = Single::CONSTANT;
            store_declaration_MOCKAST["v1"] = Single::VARIABLE;
            store_declaration_MOCKAST["v2"] = Single::VARIABLE;
            store_declaration_MOCKAST["p1"] = Single::PROCEDURE;
        }

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
        void add_pattern(PayLoad pattern) {
            mapquery.addItem(ClauseType::PATTERN, pattern);
        }

        Map_Query_unit() {
            QueryMap q;
            mapquery = q;    
        }
};

TEST_CASE("TEST PREPROCESSING OF MAP QUERY") {
    TNode ast_tree = getMockAST();
    PKBBuilder pkb_builder(ast_tree);
    PKBQueryController pkb_query_controller(pkb_builder.build());
    ActionsExecutor executor(pkb_query_controller);
    Map_Query_unit map_unit;
    map_unit.initialise_mock_ast_querymap();
    QueryMap mapQuery = map_unit.mapquery;
    ActionsGenerator generator(mapQuery,executor);
    // unordered_map<Single, 
    //     unordered_map<string, vector<string>>>
    //vector<string> mapStorage = generator.preprocess();
    SECTION("TEST individual storages") {
      //  REQUIRE(mapStorage == vector<string> {"A"});
        // REQUIRE(mapStorage[Single::STATEMENT]["s1"] == vector<string>{"11", "10", "9", "8", "7", "6", "2", "1", "3", "4", "5" });
        // REQUIRE(mapStorage[Single::READ]["re1"] == vector<string>{});
        // REQUIRE(mapStorage[Single::PRINT]["pn1"] == vector<string>{});
        // REQUIRE(mapStorage[Single::CALL]["c1"] == vector<string>{});
        // REQUIRE(mapStorage[Single::WHILE]["w1"] == vector<string>{});
        // REQUIRE(mapStorage[Single::IF]["ifs1"] == vector<string>{});
        // REQUIRE(mapStorage[Single::ASSIGN]["a1"] == vector<string>{"NONE"});
        // REQUIRE(mapStorage[Single::CONSTANT]["c1"] == vector<string>{"NONE"});
        // REQUIRE(mapStorage[Single::VARIABLE]["v1"] == vector<string>{"NONE"});
        // REQUIRE(mapStorage[Single::PROCEDURE]["p1"] == vector<string>{"NONE"});
    }

}

ActionsGenerator build_up_actions_generator(bool use_mock_ast, bool use_mock_storage) {
    TNode ast_tree = getMockAST();
    PKBBuilder pkb_builder(ast_tree);
    PKBQueryController pkb_query_controller(pkb_builder.build());
    ActionsExecutor executor(pkb_query_controller);
    Map_Query_unit map_unit;
    if(use_mock_ast) {
        map_unit.initialise_mock_ast_querymap();    
        QueryMap mapQuery = map_unit.mapquery;
        ActionsGenerator generator(mapQuery,executor);
        if (use_mock_storage) {
            map_unit.initialise_mock_ast_map_storage_storedeclaration();
            generator.set_map_storage_storeDeclaration(map_unit.map_storage_MOCKAST, map_unit.store_declaration_MOCKAST);
            return generator;
        } else {
            return generator;
        }
    } else {

    }
}

TEST_CASE("NO SELECT CLAUSE, SUCH THAT AND PATTERN") {
    // ActionsGenerator generator = build_up_actions_generator(true,true);
    
    // SECTION("TEST QUERY MAP WITH DECLARATION CLAUSE ONLY") {
    //     PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s"});
    //     Map_Query_unit map_unit;
    //     map_unit.add_Select_synonym(syn);
    //     map_unit.initialise_mock_ast_querymap();    
    //     QueryMap mapQuery = map_unit.mapquery;
    //     generator.set_Query_Map(mapQuery);
    //     generator.preprocess();
    //     vector<string> output = generator.TraverseQueryMap();
        
    //     REQUIRE(out == );

    // }

}

TEST_CASE("SELECT CLAUSE ONLY") {
    
    SECTION("Select stmt") {
        ActionsGenerator generator = build_up_actions_generator(true,true);
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
        Map_Query_unit map_unit;
        map_unit.add_Select_synonym(syn);
        map_unit.initialise_mock_ast_querymap();    
        QueryMap mapQuery = map_unit.mapquery;
        generator.set_Query_Map(mapQuery);
        vector<string> output = generator.TraverseQueryMap();     
        REQUIRE(output == vector<string>{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
    }

    SECTION("Select var") {
        ActionsGenerator generator = build_up_actions_generator(true,true);
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"v1"});
        Map_Query_unit map_unit;
        map_unit.add_Select_synonym(syn);
        map_unit.initialise_mock_ast_querymap();    
        QueryMap mapQuery = map_unit.mapquery;
        generator.set_Query_Map(mapQuery);
        vector<string> output = generator.TraverseQueryMap();     
        REQUIRE( output == vector<string>{"a", "b", "x", "y", "z", "v", "w"});
    }
    SECTION("Select constant") {
        ActionsGenerator generator = build_up_actions_generator(true,true);
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"c1"});
        Map_Query_unit map_unit;
        map_unit.add_Select_synonym(syn);
        map_unit.initialise_mock_ast_querymap();    
        QueryMap mapQuery = map_unit.mapquery;
        generator.set_Query_Map(mapQuery);
        vector<string> output = generator.TraverseQueryMap();     
        REQUIRE( output == vector<string>{ "1", "2", "42"});
    }
    SECTION("Select while") {
        ActionsGenerator generator = build_up_actions_generator(true,true);
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"w1"});
        Map_Query_unit map_unit;
        map_unit.add_Select_synonym(syn);
        map_unit.initialise_mock_ast_querymap();    
        QueryMap mapQuery = map_unit.mapquery;
        generator.set_Query_Map(mapQuery);
        vector<string> output = generator.TraverseQueryMap();     
        REQUIRE( output == vector<string>{"3", "9"});
    }
    SECTION("Select READ") {
        ActionsGenerator generator = build_up_actions_generator(true,true);
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"re1"});
        Map_Query_unit map_unit;
        map_unit.add_Select_synonym(syn);
        map_unit.initialise_mock_ast_querymap();    
        QueryMap mapQuery = map_unit.mapquery;
        generator.set_Query_Map(mapQuery);
        vector<string> output = generator.TraverseQueryMap();     
        REQUIRE( output == vector<string>{"1"});
    }
    SECTION("Select assigne") {
        ActionsGenerator generator = build_up_actions_generator(true,true);
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
        Map_Query_unit map_unit;
        map_unit.add_Select_synonym(syn);
        map_unit.initialise_mock_ast_querymap();    
        QueryMap mapQuery = map_unit.mapquery;
        generator.set_Query_Map(mapQuery);
        vector<string> output = generator.TraverseQueryMap();     
        REQUIRE( output == vector<string>{"4","6","7","8","10"});
    }
    SECTION("Select IF") {
        ActionsGenerator generator = build_up_actions_generator(true,true);
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"ifs1"});
        Map_Query_unit map_unit;
        map_unit.add_Select_synonym(syn);
        map_unit.initialise_mock_ast_querymap();    
        QueryMap mapQuery = map_unit.mapquery;
        generator.set_Query_Map(mapQuery);
        vector<string> output = generator.TraverseQueryMap();     
        REQUIRE( output == vector<string>{"5"});
    }
    SECTION("Select print") {
        ActionsGenerator generator = build_up_actions_generator(true,true);
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"pn1"});
        Map_Query_unit map_unit;
        map_unit.add_Select_synonym(syn);
        map_unit.initialise_mock_ast_querymap();    
        QueryMap mapQuery = map_unit.mapquery;
        generator.set_Query_Map(mapQuery);
        vector<string> output = generator.TraverseQueryMap();     
        REQUIRE( output == vector<string>{"2","3"});
    }
    SECTION("Select procedure") {
        ActionsGenerator generator = build_up_actions_generator(true,true);
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"p1"});
        Map_Query_unit map_unit;
        map_unit.add_Select_synonym(syn);
        map_unit.initialise_mock_ast_querymap();    
        QueryMap mapQuery = map_unit.mapquery;
        generator.set_Query_Map(mapQuery);
        vector<string> output = generator.TraverseQueryMap();     
        REQUIRE( output == vector<string>{"main"});
    }
    SECTION("Select call") {
        ActionsGenerator generator = build_up_actions_generator(true,true);
        PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"ca1"});
        Map_Query_unit map_unit;
        map_unit.add_Select_synonym(syn);
        map_unit.initialise_mock_ast_querymap();    
        QueryMap mapQuery = map_unit.mapquery;
        generator.set_Query_Map(mapQuery);
        vector<string> output = generator.TraverseQueryMap();     
        REQUIRE( output == vector<string>{});
    }
}

vector<string> such_that_using_mock_ast_mock_storage(PayLoad syn, PayLoad st) {
    ActionsGenerator generator = build_up_actions_generator(true,true);
    Map_Query_unit map_unit;
    map_unit.add_Select_synonym(syn);
    map_unit.add_Such_that(st);
    map_unit.initialise_mock_ast_querymap();    
    QueryMap mapQuery = map_unit.mapquery;
    generator.set_Query_Map(mapQuery);
    vector<string> output = generator.TraverseQueryMap();    
    return output;
}

//FOLLOWS

TEST_CASE("Select s1 follows(s1,1)" ) {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"s1","1"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"None"});
}
TEST_CASE("Select s2 follows(s1,1)" ) {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s2"});
    PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"s1","1"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"None"});
}
TEST_CASE("Select s1 follows(s1,2)" ) {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"s1","2"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"1"});
}
TEST_CASE("Select s2 follows(s1,2)" ) {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s2"});
    PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"s1","2"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
}
TEST_CASE("Select s1 follows(3,s1)" ) {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"3","s1"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"5"});
}
TEST_CASE("Select s1 follows(s1,4" ) {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"s1","4"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"None"});
}
TEST_CASE("Select s1 follows(4,s1" ) {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"4","s1"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"None"});
}
TEST_CASE("Select s1 follows(7,s1" ) {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"7","s1"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"8"});
}
TEST_CASE("Select s1 follows(s1,s2" ) {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"s1","s2"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"9", "8", "5", "7", "1", "2", "3"});
}
TEST_CASE("Select s2 follows(s1,s2)" ) {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s2"});
    PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"s1","s2"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"11", "9", "8", "3", "2", "5"});
}
TEST_CASE("Select s3 follows(s1,s2)" ) {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s3"});
    PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"s1","s2"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
}
TEST_CASE("Select s3 follows(1,3)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s3"});
    PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"1","3"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"None"});
}

TEST_CASE("Select s1 follows(s1,w1)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"s1","w1"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"2","8"});
}
TEST_CASE("Select s1 follows(s1,a1)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"s1","a1"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"7"});
}
TEST_CASE("Select a1 follows(a1,s1)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
    PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"a1","s1"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"None"});
}
TEST_CASE("Select a1 follows(w1,s1)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a"});
    PayLoad st(PAIR, Pair::FOLLOWS, std::vector<std::string>{"w1","s1"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"4","6","7","8","10"});
}
//Follows*
TEST_CASE("Select s1 followst(s1,s2)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::FOLLOWST, std::vector<std::string>{"s1","s2"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"2", "1", "3", "5", "8", "7"});
}
TEST_CASE("Select s3 followst(s1,s2)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s3"});
    PayLoad st(PAIR, Pair::FOLLOWST, std::vector<std::string>{"s1","s2"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
}
TEST_CASE("Select s1 followst(s1,3)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::FOLLOWST, std::vector<std::string>{"s1","3"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"2","1"});
}
TEST_CASE("Select s2 followst(s1,3)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s2"});
    PayLoad st(PAIR, Pair::FOLLOWST, std::vector<std::string>{"s1","3"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
}
TEST_CASE("Select s1 followst(3,s1)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::FOLLOWST, std::vector<std::string>{"3","s1"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"11","5"});
}
TEST_CASE("Select s2 followst(3,s1)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s2"});
    PayLoad st(PAIR, Pair::FOLLOWST, std::vector<std::string>{"3","s1"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
}
TEST_CASE("Select s1 followst(4,s1)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::FOLLOWST, std::vector<std::string>{"4","s1"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"None"});
}
TEST_CASE("Select s1 followst(s1,4)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::FOLLOWST, std::vector<std::string>{"s1","4"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"None"});
}

//USES
//still need to test different type of statements such as read, print etc.
TEST_CASE("Select s1 uses(5,\"y)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"5","y"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
}
TEST_CASE("Select s1 uses(5,\"w)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"5","w"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"None"});
}
TEST_CASE("Select s1 uses(5,\"v)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"5","v"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
}
TEST_CASE("Select s1 uses(s1,v1)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"s1","v1"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{ "10", "9", "5", "2", "11", "3", "4", "6"});
}
TEST_CASE("Select s2 uses(s1,v1)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s2"});
    PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"s1","v1"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
}
TEST_CASE("Select v1 uses(s1,v1)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"v1"});
    PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"s1","v1"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"x", "v", "y", "b", "z"});
}
TEST_CASE("Select s1 uses(s1,\"y)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"s1","y"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"6", "3", "5"});
}
TEST_CASE("Select s1 uses(s1,\"z)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::USES, std::vector<std::string>{"s1","z"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"5"});
}

//MODIFIES
//still need to test different type of statements such as read, print etc.
TEST_CASE("Select s1 modifies(s1,v1)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"s1","v1"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"6", "5", "10", "9", "1", "7", "3", "4", "8"});
}
TEST_CASE("Select s2 modifies(s1,v1)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s2"});
    PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"s1","v1"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
}
TEST_CASE("Select v1 modifies(s1,v1)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"v1"});
    PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"s1","v1"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"v", "y", "x", "w", "a"});
}
TEST_CASE("Select s1 modifies(5,\"y)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"5","y"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
}
TEST_CASE("Select s1 modifies(5,\"w)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"5","w"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
}
TEST_CASE("Select s1 modifies(s1,\"y)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"s1","y"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"8", "5"});
}
TEST_CASE("Select s1 modifies(s1,\"z)") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s1"});
    PayLoad st(PAIR, Pair::MODIFIES, std::vector<std::string>{"s1","z"});
    vector<string> output = such_that_using_mock_ast_mock_storage(syn, st);    
    REQUIRE(output == vector<string>{"None"});
}

vector<string> pattern_using_mock_ast_mock_storage(PayLoad syn, PayLoad pattern) {
    ActionsGenerator generator = build_up_actions_generator(true,true);
    Map_Query_unit map_unit;
    map_unit.add_Select_synonym(syn);
    map_unit.add_pattern(pattern);
    map_unit.initialise_mock_ast_querymap();    
    QueryMap mapQuery = map_unit.mapquery;
    generator.set_Query_Map(mapQuery);
    vector<string> output = generator.TraverseQueryMap();    
    return output;
}

// Pattern clause only
//need to test this
TEST_CASE("Select a1 pattern a1(v1,_") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
    PayLoad pattern(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","v1","_"});
    vector<string> output = pattern_using_mock_ast_mock_storage(syn, pattern);    
    //REQUIRE(output == vector<string>{"4", "6", "7", "8", "10"});
}
TEST_CASE("Select v1 pattern a1(v1,_") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"v1"});
    PayLoad pattern(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","v1","_"});
    vector<string> output = pattern_using_mock_ast_mock_storage(syn, pattern);    
    //REQUIRE(output == vector<string>{"4", "6", "7", "8", "10"});
}
TEST_CASE("Select a2 pattern a1(v1,_") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a2"});
    PayLoad pattern(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","v1","_"});
    vector<string> output = pattern_using_mock_ast_mock_storage(syn, pattern);    
    //REQUIRE(output == vector<string>{"4", "6", "7", "8", "10"});
}
TEST_CASE("Select a1 pattern a1(\"x\",_") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
    PayLoad pattern(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","x","_"});
    vector<string> output = pattern_using_mock_ast_mock_storage(syn, pattern);    
    //REQUIRE(output == vector<string>{"4","7"});
}
TEST_CASE("Select a1 pattern a1(v1,\"x - 1\"") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
    PayLoad pattern(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","x","x-1"});
    vector<string> output = pattern_using_mock_ast_mock_storage(syn, pattern);    
    //REQUIRE(output == vector<string>{"4"});
}
TEST_CASE("Select a1 pattern a1(\"y\",\"2\")") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
    PayLoad pattern(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","y","2"});
    vector<string> output = pattern_using_mock_ast_mock_storage(syn, pattern);    
    //REQUIRE(output == vector<string>{"8"});
}
TEST_CASE("Select a1 pattern a1(\"w\",\"y*\"") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
    PayLoad pattern(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","w","y*"});
    vector<string> output = pattern_using_mock_ast_mock_storage(syn, pattern);    
    //REQUIRE(output == vector<string>{"6"});
}

TEST_CASE("Select a1 pattern a1(\"w\",\"y *\"") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
    PayLoad pattern(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","w","y *"});
    vector<string> output = pattern_using_mock_ast_mock_storage(syn, pattern);    
    //REQUIRE(output == vector<string>{"6"});
}

TEST_CASE("Select a1 pattern a1(v1,\"-1\"") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
    PayLoad pattern(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","v1","-1"});
    vector<string> output = pattern_using_mock_ast_mock_storage(syn, pattern);    
    //REQUIRE(output == vector<string>{"6"});
}

TEST_CASE("Select a1 pattern a1(v1,_\"-1\"_") {
    PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"a1"});
    PayLoad pattern(TRIPLE, Triple::SYN_ASSIGN, std::vector<std::string>{"a1","v1","_"});
    vector<string> output = pattern_using_mock_ast_mock_storage(syn, pattern);    
    //REQUIRE(output == vector<string>{"6"});
}

TEST_CASE("SELECT CLAUSE SUCH THAT PATTERN CLAUSE") {

}
