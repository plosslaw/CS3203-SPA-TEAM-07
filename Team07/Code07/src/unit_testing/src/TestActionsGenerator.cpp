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
        QueryMap mapquery;
        void initialise_mock_ast_querymap() {
            //vector<PayLoad> declaration_list{}
            PayLoad stmt(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"});
            PayLoad re(SINGLE, Single::READ, std::vector<std::string>{"re1"});
            PayLoad pn(SINGLE, Single::PRINT, std::vector<std::string>{"pn1"});
            PayLoad call(SINGLE, Single::CALL, std::vector<std::string>{"c1"});
            PayLoad w(SINGLE, Single::WHILE, std::vector<std::string>{"w1"});
            PayLoad ifs(SINGLE, Single::IF, std::vector<std::string>{"ifs1"});
            PayLoad a(SINGLE, Single::ASSIGN, std::vector<std::string>{"a1"});
            PayLoad c(SINGLE, Single::CONSTANT, std::vector<std::string>{"c1"});
            PayLoad var(SINGLE, Single::VARIABLE, std::vector<std::string>{"v1"});
            PayLoad proc(SINGLE, Single::PROCEDURE, std::vector<std::string>{"p1"});
            vector<PayLoad> declaration_lst{stmt, re, pn, call,w,ifs,a,c,var,proc};
            mapquery.addItem(ClauseType::DECLARATION, stmt);
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

        Map_Query_unit() {
            QueryMap q;
            mapquery = q;    
        }
};


TEST_CASE("NO SELECT CLAUSE, SUCH THAT AND PATTERN") {
    TNode ast_tree = getMockAST();
    PKBBuilder pkb_builder(ast_tree);
    PKBQueryController pkb_query_controller(pkb_builder.build());
    ActionsExecutor executor(pkb_query_controller);
    Map_Query_unit map_unit;
    map_unit.initialise_mock_ast_querymap();
    QueryMap mapQuery = map_unit.mapquery;
    ActionsGenerator generator(mapQuery,executor);
    vector<string> output = generator.TraverseQueryMap();
    SECTION("TEST QUERY MAP WITH DECLARATION CLAUSE ONLY") {
         REQUIRE(output == vector<string> {"Invalid"});
    }

}

TEST_CASE("SELECT CLAUSE ONLY") {

}

TEST_CASE("SELECT CLAUSE SUCH THAT CLAUSE" ) {

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
            
//             PayLoad stmt(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"});
//             PayLoad synonym(SINGLE, Single::SYNONYM, std::vector<std::string>{"s"});    
//             PayLoad followsLoad(PAIR, FOLLOWST, std::vector<std::string>{"s","3"});
            
//             query.addItem(DECLARATION, stmt);
//             query.addItem(SELECT, synonym);
//             query.addItem(SUCHTHAT, followsLoad);
//             generator = ActionsGenerator(query, executor);
//         }
//         void second_query() {
//             executor.set_current_test(2);
//             //stmt s
//             //Select s such that FOLLOWST(2,s)
            
//             PayLoad stmt(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"});
//             PayLoad synonym(SINGLE, Single::SYNONYM, std::vector<std::string>{"s"});    
//             PayLoad followsLoad(PAIR, FOLLOWST, std::vector<std::string>{"2","s"});
            
//             query.addItem(DECLARATION, stmt);
//             query.addItem(SELECT, synonym);
//             query.addItem(SUCHTHAT, followsLoad);
//             generator = ActionsGenerator(query, executor);
//         }
//         void third_query() {
//             executor.set_current_test(3);
//             //stmt s, s1
//             //Select s such that FOLLOWST(s,s1)
            
//             PayLoad stmt(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"});
//             PayLoad stmt2(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"});
//             PayLoad synonym(SINGLE, Single::SYNONYM, std::vector<std::string>{"s"});    
//             PayLoad followsLoad(PAIR, FOLLOWST, std::vector<std::string>{"s","s1"});
            
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
            
//             PayLoad stmt(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"});
//             PayLoad stmt2(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"});
//             PayLoad synonym(SINGLE, Single::SYNONYM, std::vector<std::string>{"s"});    
//             PayLoad followsLoad(PAIR, FOLLOWST, std::vector<std::string>{"1","3"});
            
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
            
//             PayLoad stmt(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"});
//             PayLoad stmt2(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"});
//             PayLoad stmt3(SINGLE, Single::VARIABLE, std::vector<std::string>{"v"});
//             PayLoad synonym(SINGLE, Single::SYNONYM, std::vector<std::string>{"v"});    
//             PayLoad followsLoad(PAIR, FOLLOWST, std::vector<std::string>{"s","3"});
            
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
    
// //     PayLoad stmt(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"});
// //     PayLoad assign(SINGLE, Single::ASSIGN, std::vector<std::string>{"a"});
// //     PayLoad var(SINGLE, Single::VARIABLE, std::vector<std::string>{"v"});


// //     PayLoad synonym(SINGLE, Single::SYNONYM, std::vector<std::string>{"s"});
// //     PayLoad synonym2(SINGLE, Single::SYNONYM, std::vector<std::string>{"v"});
// //     PayLoad followsLoad(PAIR, FOLLOWST, std::vector<std::string>{"s","3"});
// //     QueryMap mapQuery;
// //     mapQuery.addItem(DECLARATION, stmt);
// //     mapQuery.addItem(DECLARATION, assign);
// //     mapQuery.addItem(SELECT, synonym);
// //     mapQuery.addItem(SUCHTHAT, followsLoad);

// //     SECTION("simple stmt s, select s such that followst(s,3)") {
// //         // std::vector<std::string> result;
// //         // result.push_back("1");result.push_back("2");result.push_back("3");
// //         // ActionsGenerator actionsgenerator;
// //         // std::vector<std::string>output = actionsgenerator.TraverseQueryMap(mapQuery);
// //         // REQUIRE(output == result);
// //     }
// //     QueryMap mapQuery2;
// //     mapQuery2.addItem(DECLARATION, stmt);
// //     mapQuery2.addItem(DECLARATION, assign);
// //     mapQuery2.addItem(DECLARATION, var);
// //     mapQuery2.addItem(SELECT, synonym2);
// //     mapQuery2.addItem(SUCHTHAT, followsLoad);

// //     // SECTION("simple var v;assign a, select a such that USES(a,v") {
// //     //     std::vector<std::string> result;
// //     //     result.push_back("1");result.push_back("2");//result.push_back("3");
// //     //     ActionsGenerator actionsgenerator;
// //     //     std::vector<std::string>output = actionsgenerator.TraverseQueryMap(mapQuery2);
// //     //     REQUIRE(output == result);
// //     // }}
