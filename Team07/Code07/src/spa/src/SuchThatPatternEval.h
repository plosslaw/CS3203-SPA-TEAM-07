#pragma once

#include "QueryMap.h"
#include "ActionsExecutor.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

class SuchThatPatternEval {
    private:
        //variables
        unordered_map<string, Single> storeDeclaration;
        unordered_map<Single, 
            unordered_map<string, vector<string>>> mapStorage;
        ActionsExecutor executor;
        
        //methods
        vector<string> zero_common_synonym(PayLoad such_that_pay_load, PayLoad pattern_pay_load,string select_value, Single select_type);
        vector<string> one_common_synonym(PayLoad such_that_pay_load, PayLoad pattern_pay_load,string select_value, Single select_type);
        vector<string> more_than_one_common_synonym(PayLoad such_that_pay_load, PayLoad pattern_pay_load,string select_value, Single select_type);

        //utilities
        pair<bool,bool> check_if_args_are_variable(std::string first_arg, std::string second_arg);
        bool is_element_inside_vectorA(string element, vector<string> vectorA);
        bool is_element_inside_vectorA_int(int element, vector<int> vectorA);
        stmt_type convert_single_to_stmt_type(Single s);
        vector<string> inner_join(vector<string> lstA, vector<string> lstB);
        vector<pair<string,string>> crossproduct(vector<string> first_arg_lst, vector<string> second_arg_lst);
        vector<string> convert_lst_string_to_int(vector<int> lstA);
        bool is_pattern_variable_is_constant(std::string pattern_variable_value);
        
    public:
        SuchThatPatternEval(unordered_map<string, Single> declaration_store, unordered_map<Single, 
            unordered_map<string, vector<string>>> map_storage, ActionsExecutor executor_);

        vector<string> such_that_pattern_eval(PayLoad such_that_pay_load, PayLoad pattern_load,string select_value, Single select_type);
};
