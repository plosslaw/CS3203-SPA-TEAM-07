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
            unordered_map<string, vector<string>> readStorage;
            readStorage["re1"] = read_lst;
            unordered_map<string, vector<string>> printStorage;
            printStorage["pn1"] = print_lst;
            unordered_map<string, vector<string>> assignStorage;
            assignStorage["a1"] = assignment_lst;
            unordered_map<string, vector<string>> whileStorage;
            whileStorage["w1"] = while_lst;
            unordered_map<string, vector<string>> ifStorage;
            ifStorage["ifs1"] = if_lst;
            unordered_map<string, vector<string>> constantStorage;
            constantStorage["c1"] = constant_lst;
            unordered_map<string, vector<string>> variableStorage;
            variableStorage["v1"] = variable_lst;
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
            store_declaration_MOCKAST["re1"] = Single::READ;
            store_declaration_MOCKAST["pn1"] = Single::PRINT;
            store_declaration_MOCKAST["ca1"] = Single::CALL;
            store_declaration_MOCKAST["w1"] = Single::WHILE;
            store_declaration_MOCKAST["ifs1"] = Single::IF;
            store_declaration_MOCKAST["a1"] = Single::ASSIGN;
            store_declaration_MOCKAST["c1"] = Single::CONSTANT;
            store_declaration_MOCKAST["v1"] = Single::VARIABLE;
            store_declaration_MOCKAST["p1"] = Single::PROCEDURE;
        }

        void initialise_mock_ast_querymap() {
            //vector<PayLoad> declaration_list{}
            PayLoad stmt(SINGLE, Single::STATEMENT, vector<string>{"s1"});
            PayLoad stmt2(SINGLE, Single::STATEMENT, vector<string>{"s2"});
            PayLoad re(SINGLE, Single::READ, vector<string>{"re1"});
            PayLoad pn(SINGLE, Single::PRINT, vector<string>{"pn1"});
            PayLoad call(SINGLE, Single::CALL, vector<string>{"ca1"});
            PayLoad w(SINGLE, Single::WHILE, vector<string>{"w1"});
            PayLoad ifs(SINGLE, Single::IF, vector<string>{"ifs1"});
            PayLoad a(SINGLE, Single::ASSIGN, vector<string>{"a1"});
            PayLoad c(SINGLE, Single::CONSTANT, vector<string>{"c1"});
            PayLoad var(SINGLE, Single::VARIABLE, vector<string>{"v1"});
            PayLoad proc(SINGLE, Single::PROCEDURE, vector<string>{"p1"});
            vector<PayLoad> declaration_lst{stmt, re, pn, call,w,ifs,a,c,var,proc};
            mapquery.addItem(ClauseType::DECLARATION, stmt);
            mapquery.addItem(ClauseType::DECLARATION, stmt2);
            mapquery.addItem(ClauseType::DECLARATION, re);
            mapquery.addItem(ClauseType::DECLARATION, pn);
            mapquery.addItem(ClauseType::DECLARATION, call);
            mapquery.addItem(ClauseType::DECLARATION, ifs);
            mapquery.addItem(ClauseType::DECLARATION, w);
            mapquery.addItem(ClauseType::DECLARATION, a);
            mapquery.addItem(ClauseType::DECLARATION, c);
            mapquery.addItem(ClauseType::DECLARATION, var);
            mapquery.addItem(ClauseType::DECLARATION, proc);
        }

        void add_Select_synonym(PayLoad syn) {
            mapquery.addItem(ClauseType::SELECT, syn);
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

TEST_CASE("SELECT CLAUSE SUCH THAT CLAUSE" ) {
    // SECTION("Select stmt") {
    //     ActionsGenerator generator = build_up_actions_generator(true,true);
    //     PayLoad syn(SINGLE, Single::SYNONYM, std::vector<std::string>{"s2"});
    //     Map_Query_unit map_unit;
    //     map_unit.add_Select_synonym(syn);
    //     map_unit.initialise_mock_ast_querymap();    
    //     QueryMap mapQuery = map_unit.mapquery;
    //     generator.set_Query_Map(mapQuery);
    //     vector<string> output = generator.TraverseQueryMap();     
    //     REQUIRE(output == vector<string>{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
    // }
    SECTION("Select stmt follows(s1,s2") {

    }
}

TEST_CASE("SELECT CLAUSE PATTERN CLAUSE") {

}

TEST_CASE("SELECT CLAUSE SUCH THAT PATTERN CLAUSE") {

}

// class TestCases {
//     public:
//         ActionsGenerator generator;
//         ActionsExecutor executor;
//         QueryMap query;
//         int current_test = 0;
//         TestCases() {
//         }
//         void first_query() {
//             executor.set_current_test(1);
//             //stmt s
//             //Select s such that FOLLOWST(s,3)
            
//             PayLoad stmt(SINGLE, Single::STATEMENT, vector<string>{"s"});
//             PayLoad synonym(SINGLE, Single::SYNONYM, vector<string>{"s"});    
//             PayLoad followsLoad(PAIR, FOLLOWST, vector<string>{"s","3"});
            
//             query.addItem(DECLARATION, stmt);
//             query.addItem(SELECT, synonym);
//             query.addItem(SUCHTHAT, followsLoad);
//             generator = ActionsGenerator(query, executor);
//         }
//         void second_query() {
//             executor.set_current_test(2);
//             //stmt s
//             //Select s such that FOLLOWST(2,s)
            
//             PayLoad stmt(SINGLE, Single::STATEMENT, vector<string>{"s"});
//             PayLoad synonym(SINGLE, Single::SYNONYM, vector<string>{"s"});    
//             PayLoad followsLoad(PAIR, FOLLOWST, vector<string>{"2","s"});
            
//             query.addItem(DECLARATION, stmt);
//             query.addItem(SELECT, synonym);
//             query.addItem(SUCHTHAT, followsLoad);
//             generator = ActionsGenerator(query, executor);
//         }
//         void third_query() {
//             executor.set_current_test(3);
//             //stmt s, s1
//             //Select s such that FOLLOWST(s,s1)
            
//             PayLoad stmt(SINGLE, Single::STATEMENT, vector<string>{"s"});
//             PayLoad stmt2(SINGLE, Single::STATEMENT, vector<string>{"s1"});
//             PayLoad synonym(SINGLE, Single::SYNONYM, vector<string>{"s"});    
//             PayLoad followsLoad(PAIR, FOLLOWST, vector<string>{"s","s1"});
            
//             query.addItem(DECLARATION, stmt);
//             query.addItem(DECLARATION, stmt2);
//             query.addItem(SELECT, synonym);
//             query.addItem(SUCHTHAT, followsLoad);
//             generator = ActionsGenerator(query, executor);
//         }
//         void fourth_query() {
//             executor.set_current_test(4);
//             //stmt s, s1
//             //Select s such that FOLLOWST(1,3)
            
//             PayLoad stmt(SINGLE, Single::STATEMENT, vector<string>{"s"});
//             PayLoad stmt2(SINGLE, Single::STATEMENT, vector<string>{"s1"});
//             PayLoad synonym(SINGLE, Single::SYNONYM, vector<string>{"s"});    
//             PayLoad followsLoad(PAIR, FOLLOWST, vector<string>{"1","3"});
            
//             query.addItem(DECLARATION, stmt);
//             query.addItem(DECLARATION, stmt2);
//             query.addItem(SELECT, synonym);
//             query.addItem(SUCHTHAT, followsLoad);
//             generator = ActionsGenerator(query, executor);
//         }
        
//         void fifth_query() {
//             executor.set_current_test(5);
//             //stmt s, s1; variable v;
//             //Select v such that FOLLOWST(s,3)
            
//             PayLoad stmt(SINGLE, Single::STATEMENT, vector<string>{"s"});
//             PayLoad stmt2(SINGLE, Single::STATEMENT, vector<string>{"s1"});
//             PayLoad stmt3(SINGLE, Single::VARIABLE, vector<string>{"v"});
//             PayLoad synonym(SINGLE, Single::SYNONYM, vector<string>{"v"});    
//             PayLoad followsLoad(PAIR, FOLLOWST, vector<string>{"s","3"});
            
//             query.addItem(DECLARATION, stmt);
//             query.addItem(DECLARATION, stmt2);
//             query.addItem(DECLARATION, stmt3);
//             query.addItem(SELECT, synonym);
//             query.addItem(SUCHTHAT, followsLoad);
//             generator = ActionsGenerator(query, executor);
//         }
// };

// TEST_CASE("first_test") {
//     TestCases firsttest;
//     SECTION("first_test") {
//     firsttest.first_query();
//     vector<string> first_result = firsttest.generator.TraverseQueryMap();
//     REQUIRE(first_result == vector<string>{"1"});
//     }
//     SECTION("second_test") {       
//         firsttest.second_query();
//         vector<string> second_result = firsttest.generator.TraverseQueryMap();
//         REQUIRE(second_result == vector<string>{"1"});
//     }
//     SECTION("third_test") {       
//         firsttest.third_query();
//         vector<string> third_result = firsttest.generator.TraverseQueryMap();
//         REQUIRE(third_result == vector<string>{"1"});
//     }
//     SECTION("fifth_test") {       
//         firsttest.fifth_query();
//         vector<string> fourth = firsttest.generator.TraverseQueryMap();
//         REQUIRE(fourth == vector<string>{"1"});
//     }
//     SECTION("fourth_test") {       
//         firsttest.fourth_query();
//         vector<string> fourth = firsttest.generator.TraverseQueryMap();
//         REQUIRE(fourth == vector<string>{"1"});
//     }
// }

// // };
// // TEST_CASE("Payload ") {
    
// //     PayLoad stmt(SINGLE, Single::STATEMENT, vector<string>{"s"});
// //     PayLoad assign(SINGLE, Single::ASSIGN, vector<string>{"a"});
// //     PayLoad var(SINGLE, Single::VARIABLE, vector<string>{"v"});


// //     PayLoad synonym(SINGLE, Single::SYNONYM, vector<string>{"s"});
// //     PayLoad synonym2(SINGLE, Single::SYNONYM, vector<string>{"v"});
// //     PayLoad followsLoad(PAIR, FOLLOWST, vector<string>{"s","3"});
// //     QueryMap mapQuery;
// //     mapQuery.addItem(DECLARATION, stmt);
// //     mapQuery.addItem(DECLARATION, assign);
// //     mapQuery.addItem(SELECT, synonym);
// //     mapQuery.addItem(SUCHTHAT, followsLoad);

// //     SECTION("simple stmt s, select s such that followst(s,3)") {
// //         // vector<string> result;
// //         // result.push_back("1");result.push_back("2");result.push_back("3");
// //         // ActionsGenerator actionsgenerator;
// //         // vector<string>output = actionsgenerator.TraverseQueryMap(mapQuery);
// //         // REQUIRE(output == result);
// //     }
// //     QueryMap mapQuery2;
// //     mapQuery2.addItem(DECLARATION, stmt);
// //     mapQuery2.addItem(DECLARATION, assign);
// //     mapQuery2.addItem(DECLARATION, var);
// //     mapQuery2.addItem(SELECT, synonym2);
// //     mapQuery2.addItem(SUCHTHAT, followsLoad);

// //     // SECTION("simple var v;assign a, select a such that USES(a,v") {
// //     //     vector<string> result;
// //     //     result.push_back("1");result.push_back("2");//result.push_back("3");
// //     //     ActionsGenerator actionsgenerator;
// //     //     vector<string>output = actionsgenerator.TraverseQueryMap(mapQuery2);
// //     //     REQUIRE(output == result);
// //     // }}
