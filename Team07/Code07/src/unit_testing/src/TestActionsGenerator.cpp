#include "ActionsGenerator.h"
#include "catch.hpp"
#include <iostream>
#include <vector>

class ActionsExecutorTest: public ActionsExecutor {
    public: 
        int current_test;
        ActionsExecutorTest() {
            current_test = 0;
        }
        void set_current_test(int set_number) {
            current_test = set_number;
        }
        
        // Returns true if and only if Follows/*(s1,s2)
        bool is_follows(stmt_ref s1, stmt_ref s2, bool is_starred) {
            return true;
        }

        // Returns true if and only if Parent/*(s1,s2)
        bool is_parent(stmt_ref s1, stmt_ref s2, bool is_starred) {
            return true;
        }

        // Returns true if and only if Uses(s,v)
        bool statement_uses(stmt_ref s, var_ref v) {
            return true;
        }

        // Returns true if and only if Modifies(s,v)
        bool statement_modifies(stmt_ref s, var_ref v) {
            return true;
        }

        // Returns true if and only if assignment statement a matches pattern p
        bool satisfies_pattern(assign_ref a, pattern p) {
            return true;
        }
        // Returns list of all constants in the SIMPLE program
        std::vector<constant> get_all_constants() {
            if (current_test == 1) {
                return vector<constant>{1};
            }
        }

        // Returns list of all procedures in the SIMPLE program
        std::vector<proc_ref> get_all_procedures() {
            return vector<proc_ref>{"first_procedure"};
        };
        
        // Returns list of all statements in the SIMPLE program
        //std::vector<statement> get_all_statements();
        
        // Returns list of all statements of type t in the SIMPLE program
        std::vector<stmt_ref> get_all_statements_of_type(stmt_type t) {
                return vector<stmt_ref>{1,2,3};
            if (stmt_type::STATEMENT) {
                if (current_test == 1) {
                return vector<stmt_ref>{1,2,3};
                }
            }
            if (stmt_type::READ) {
                if (current_test == 1) {
                    return vector<stmt_ref>{};
                }
            }
            if (stmt_type::PRINT) {
                if (current_test == 1) {
                    return vector<stmt_ref>{};
                }
            }
            if (stmt_type::WHILE) {
                if (current_test == 1) {
                    return vector<stmt_ref>{};
                }
            }
            if (stmt_type::IF) {
                if (current_test == 1) {
                    return vector<stmt_ref>{};
                }
            }
            if (stmt_type::ASSIGN) {
                if (current_test == 1) {
                    return vector<stmt_ref>{};
                }
            } 
        }
        
        // Returns list of all variables in the SIMPLE program
        std::vector<var_ref> get_all_variables() {
            if (current_test == 1) {
                return vector<var_ref>{"x","y","z"};
            }
        }
};

class TestCases {
    public:
        ActionsGenerator generator;
        ActionsExecutorTest executor;
        QueryMap query;
        int current_test = 0;
        TestCases() {
        }
        void first_query() {
            executor.set_current_test(1);
            //stmt s
            //Select s such that FOLLOWST(s,3)
            
            PayLoad stmt(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"});
            PayLoad synonym(SINGLE, Single::SYNONYM, std::vector<std::string>{"s"});    
            PayLoad followsLoad(PAIR, FOLLOWST, std::vector<std::string>{"s","3"});
            
            query.addItem(DECLARATION, stmt);
            query.addItem(SELECT, synonym);
            query.addItem(SUCHTHAT, followsLoad);
            generator = ActionsGenerator(query, executor);
        }
        void second_query() {
            executor.set_current_test(2);
            //stmt s
            //Select s such that FOLLOWST(2,s)
            
            PayLoad stmt(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"});
            PayLoad synonym(SINGLE, Single::SYNONYM, std::vector<std::string>{"s"});    
            PayLoad followsLoad(PAIR, FOLLOWST, std::vector<std::string>{"2","s"});
            
            query.addItem(DECLARATION, stmt);
            query.addItem(SELECT, synonym);
            query.addItem(SUCHTHAT, followsLoad);
            generator = ActionsGenerator(query, executor);
        }
        void third_query() {
            executor.set_current_test(3);
            //stmt s, s1
            //Select s such that FOLLOWST(s,s1)
            
            PayLoad stmt(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"});
            PayLoad stmt2(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"});
            PayLoad synonym(SINGLE, Single::SYNONYM, std::vector<std::string>{"s"});    
            PayLoad followsLoad(PAIR, FOLLOWST, std::vector<std::string>{"s","s1"});
            
            query.addItem(DECLARATION, stmt);
            query.addItem(DECLARATION, stmt2);
            query.addItem(SELECT, synonym);
            query.addItem(SUCHTHAT, followsLoad);
            generator = ActionsGenerator(query, executor);
        }
        void fourth_query() {
            executor.set_current_test(4);
            //stmt s, s1
            //Select s such that FOLLOWST(1,3)
            
            PayLoad stmt(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"});
            PayLoad stmt2(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"});
            PayLoad synonym(SINGLE, Single::SYNONYM, std::vector<std::string>{"s"});    
            PayLoad followsLoad(PAIR, FOLLOWST, std::vector<std::string>{"1","3"});
            
            query.addItem(DECLARATION, stmt);
            query.addItem(DECLARATION, stmt2);
            query.addItem(SELECT, synonym);
            query.addItem(SUCHTHAT, followsLoad);
            generator = ActionsGenerator(query, executor);
        }
        
        void fifth_query() {
            executor.set_current_test(5);
            //stmt s, s1; variable v;
            //Select v such that FOLLOWST(s,3)
            
            PayLoad stmt(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"});
            PayLoad stmt2(SINGLE, Single::STATEMENT, std::vector<std::string>{"s1"});
            PayLoad stmt3(SINGLE, Single::VARIABLE, std::vector<std::string>{"v"});
            PayLoad synonym(SINGLE, Single::SYNONYM, std::vector<std::string>{"v"});    
            PayLoad followsLoad(PAIR, FOLLOWST, std::vector<std::string>{"s","3"});
            
            query.addItem(DECLARATION, stmt);
            query.addItem(DECLARATION, stmt2);
            query.addItem(DECLARATION, stmt3);
            query.addItem(SELECT, synonym);
            query.addItem(SUCHTHAT, followsLoad);
            generator = ActionsGenerator(query, executor);
        }
};

TEST_CASE("first_test") {
    TestCases firsttest;
    SECTION("first_test") {
    firsttest.first_query();
    vector<string> first_result = firsttest.generator.TraverseQueryMap();
    REQUIRE(first_result == vector<string>{"1"});
    }
    SECTION("second_test") {       
        firsttest.second_query();
        vector<string> second_result = firsttest.generator.TraverseQueryMap();
        REQUIRE(second_result == vector<string>{"1"});
    }
    SECTION("third_test") {       
        firsttest.third_query();
        vector<string> third_result = firsttest.generator.TraverseQueryMap();
        REQUIRE(third_result == vector<string>{"1"});
    }
    SECTION("fifth_test") {       
        firsttest.fifth_query();
        vector<string> fourth = firsttest.generator.TraverseQueryMap();
        REQUIRE(fourth == vector<string>{"1"});
    }
    SECTION("fourth_test") {       
        firsttest.fourth_query();
        vector<string> fourth = firsttest.generator.TraverseQueryMap();
        REQUIRE(fourth == vector<string>{"1"});
    }
}

// };
// TEST_CASE("Payload ") {
    
//     PayLoad stmt(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"});
//     PayLoad assign(SINGLE, Single::ASSIGN, std::vector<std::string>{"a"});
//     PayLoad var(SINGLE, Single::VARIABLE, std::vector<std::string>{"v"});


//     PayLoad synonym(SINGLE, Single::SYNONYM, std::vector<std::string>{"s"});
//     PayLoad synonym2(SINGLE, Single::SYNONYM, std::vector<std::string>{"v"});
//     PayLoad followsLoad(PAIR, FOLLOWST, std::vector<std::string>{"s","3"});
//     QueryMap mapQuery;
//     mapQuery.addItem(DECLARATION, stmt);
//     mapQuery.addItem(DECLARATION, assign);
//     mapQuery.addItem(SELECT, synonym);
//     mapQuery.addItem(SUCHTHAT, followsLoad);

//     SECTION("simple stmt s, select s such that followst(s,3)") {
//         // std::vector<std::string> result;
//         // result.push_back("1");result.push_back("2");result.push_back("3");
//         // ActionsGenerator actionsgenerator;
//         // std::vector<std::string>output = actionsgenerator.TraverseQueryMap(mapQuery);
//         // REQUIRE(output == result);
//     }
//     QueryMap mapQuery2;
//     mapQuery2.addItem(DECLARATION, stmt);
//     mapQuery2.addItem(DECLARATION, assign);
//     mapQuery2.addItem(DECLARATION, var);
//     mapQuery2.addItem(SELECT, synonym2);
//     mapQuery2.addItem(SUCHTHAT, followsLoad);

//     // SECTION("simple var v;assign a, select a such that USES(a,v") {
//     //     std::vector<std::string> result;
//     //     result.push_back("1");result.push_back("2");//result.push_back("3");
//     //     ActionsGenerator actionsgenerator;
//     //     std::vector<std::string>output = actionsgenerator.TraverseQueryMap(mapQuery2);
//     //     REQUIRE(output == result);
//     // }}
