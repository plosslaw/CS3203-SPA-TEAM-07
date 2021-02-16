#include "ActionsGenerator.h"
#include "catch.hpp"
#include <iostream>
#include <vector>

//stmt s
//Select s such that FOLLOWST(s,3)

TEST_CASE("Payload ") {
    
    PayLoad stmt(SINGLE, Single::STATEMENT, std::vector<std::string>{"s"});
    PayLoad assign(SINGLE, Single::ASSIGN, std::vector<std::string>{"a"});
    PayLoad var(SINGLE, Single::VARIABLE, std::vector<std::string>{"v"});


    PayLoad synonym(SINGLE, Single::SYNONYM, std::vector<std::string>{"s"});
    PayLoad synonym2(SINGLE, Single::SYNONYM, std::vector<std::string>{"v"});
    PayLoad followsLoad(PAIR, FOLLOWST, std::vector<std::string>{"s","3"});
    QueryMap mapQuery;
    mapQuery.addItem(DECLARATION, stmt);
    mapQuery.addItem(DECLARATION, assign);
    mapQuery.addItem(SELECT, synonym);
    mapQuery.addItem(SUCHTHAT, followsLoad);

    SECTION("simple stmt s, select s such that followst(s,3)") {
        std::vector<std::string> result;
        result.push_back("1");result.push_back("2");result.push_back("3");
        ActionsGenerator actionsgenerator;
        std::vector<std::string>output = actionsgenerator.TraverseQueryMap(mapQuery);
        REQUIRE(output == result);
    }
    QueryMap mapQuery2;
    mapQuery2.addItem(DECLARATION, stmt);
    mapQuery2.addItem(DECLARATION, assign);
    mapQuery2.addItem(DECLARATION, var);
    mapQuery2.addItem(SELECT, synonym2);
    mapQuery2.addItem(SUCHTHAT, followsLoad);

    // SECTION("simple var v;assign a, select a such that USES(a,v") {
    //     std::vector<std::string> result;
    //     result.push_back("1");result.push_back("2");//result.push_back("3");
    //     ActionsGenerator actionsgenerator;
    //     std::vector<std::string>output = actionsgenerator.TraverseQueryMap(mapQuery2);
    //     REQUIRE(output == result);
    // }

}