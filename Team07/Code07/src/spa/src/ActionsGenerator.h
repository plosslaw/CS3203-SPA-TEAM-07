#pragma once

#include "QueryMap.h"
#include "ActionsExecutor.h"
#include <string>
#include <vector>

using namespace std;

class ActionsGenerator {
    private:
        //variables
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
    
        //methods
        vector<string> one_such_that_zero_pattern(PayLoad such_that_pay_load, string select_value, Single select_type, pair<bool,bool> arg_pairs);
        vector<string> zero_such_that_one_pattern(PayLoad pattern_pay_load, string select_value, Single select_type, pair<bool,bool> arg_pairs);
                   
        //utilities 
        pair<bool,bool> check_if_args_are_variable(std::string first_arg, std::string second_arg);
        vector<pair<string,string>> crossproduct(vector<string> first_arg_lst, vector<string> second_arg_lst);

    public:
        ActionsGenerator();
        ActionsGenerator(QueryMap mapQuery, ActionsExecutor executerActions);
        std::vector<std::string> TraverseQueryMap();
};
