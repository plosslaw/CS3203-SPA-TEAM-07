#pragma once

#include "QueryMap.h"
#include "ActionsExecutor.h"

using namespace std;

class ActionsGenerator {

    private:
        ActionsExecutor executer;
        QueryMap queryMap;

        // List of payloads from respective clauses - SELECT, DECLARATION, SUCHTHAT, PATTERN
        std::vector<PayLoad> selectList;
        std::vector<PayLoad> declarationList;
        std::vector<PayLoad> suchThatList;
        std::vector<PayLoad> patternList;

        //Map declaration names to a list of its own entities type from PKB
        //example: stmt s1,s2..; stmtStorage = [(s1,[stmt_list]), s2,[stmt_list]...]
        std::unordered_map<std::string, std::vector<std::string>> procedureStorage;
        std::unordered_map<std::string, std::vector<std::string>> stmtStorage;
        std::unordered_map<std::string, std::vector<std::string>> readStorage;
        std::unordered_map<std::string, std::vector<std::string>> printStorage;
        std::unordered_map<std::string, std::vector<std::string>> assignStorage;
        std::unordered_map<std::string, std::vector<std::string>> whileStorage;
        std::unordered_map<std::string, std::vector<std::string>> ifStorage;
        std::unordered_map<std::string, std::vector<std::string>> constantStorage;
        std::unordered_map<std::string, std::vector<std::string>> variableStorage;


        //storeDeclaration maps declaration name to entitity type for fast conversion, example: v1 => VARIABLE
        std::unordered_map<std::string, Single> storeDeclaration;

        //Maps Single to all types of storage such as procedure storage, stmt, read etc.
        std::unordered_map<Single, 
        std::unordered_map<std::string, std::vector<std::string>>> mapStorage;
    

        // helper methods
        std::vector<std::pair <std::string,string>> permutateValues(std::vector<std::string> firstArg, std::vector<std::string> secondArg);
        std::vector<std::pair <std::string, std::string>> bothAreVariables(std::string firstArg, std::string secondArg);
        std::pair<bool,bool> checkIfArgsAreVariable(std::string firstArg, std::string secondArg);
        
        // evaluation methods
        std::unordered_map<std::string, std::vector<std::string>> evalSuchThatPre(std::string firstArg, std::string secondArg, 
            std::string selectValue, PayLoad loadpay);
        bool evalSuchThat(Pair suchThatType, std::string actionFirstArg, std::string actionSecondArg);

        std::vector<std::string> evalPattern(PayLoad loadPay, 
            std::unordered_map<std::string, std::vector<std::string>> solutionStorage);
        //std::vector<std::string> ActionsGenerator::extract(std::string payloadVal);

    public:
        std::vector<std::string> TraverseQueryMap();
        ActionsGenerator(QueryMap mapQuery, ActionsExecutor executerActions);
        ActionsGenerator();
};

