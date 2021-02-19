#pragma once

#include "QueryMap.h"
#include "ActionsExecutor.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

class PatternEval {
    private:
        //variables
        unordered_map<string, Single> storeDeclaration;
        unordered_map<Single, 
            unordered_map<string, vector<string>>> mapStorage;
        ActionsExecutor executor;
        //utilities
        pair<bool,bool> check_if_args_are_variable(std::string first_arg, std::string second_arg);
        bool is_element_inside_vectorA(string element, vector<string> vectorA);
        stmt_type convert_single_to_stmt_type(Single s);
        vector<string> is_result_empty_yes_none_no_default_soln(bool is_result_empty, Single select_type, string select_value);
    
    public:
        PatternEval(unordered_map<string, Single> declaration_store, unordered_map<Single, 
            unordered_map<string, vector<string>>> map_storage, ActionsExecutor executor_);
        
        // evaluate a pattern clauses without such clause. 
        vector<string> zero_such_that_one_pattern(PayLoad pattern_pay_load, string select_value, Single select_type, pair<bool,bool> arg_pairs);
         
};
