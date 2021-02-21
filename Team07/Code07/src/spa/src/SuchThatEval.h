#pragma once

#include "QueryMap.h"
#include "ActionsExecutor.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

class SuchThatEval {
    private:
        //variables
        unordered_map<string, Single> storeDeclaration;
        unordered_map<Single, 
            unordered_map<string, vector<string>>> mapStorage;
        ActionsExecutor executor;
        //utilities
        pair<bool,bool> check_if_args_are_variable(std::string first_arg, std::string second_arg);
        bool is_element_inside_vectorA(string element, vector<string> vectorA);
        vector<string> convert_lst_string_to_int(vector<int> lstA);
        stmt_type convert_single_to_stmt_type(Single s);
        bool is_pattern_variable_is_constant(std::string pattern_variable_value);
    
    public:
        SuchThatEval(unordered_map<string, Single> declaration_store, unordered_map<Single, 
            unordered_map<string, vector<string>>> map_storage, ActionsExecutor executor_);
        
        // evaluate a such that clauses without pattern clause. 
        vector<string> one_such_that_zero_pattern(PayLoad such_that_pay_load, string select_value, Single select_type, pair<bool,bool> arg_pairs);
        
};
