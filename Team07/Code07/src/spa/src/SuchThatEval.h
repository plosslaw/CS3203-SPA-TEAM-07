#pragma once

#include "QueryMap.h"
#include "ActionsExecutor.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

class SuchThatEval {
    private:

    public:
        // evaluate a such that clauses without pattern clause
        //vector<string> one_such_that_zero_pattern(PayLoad pay_load, string select_value, Single select_type, pair<bool,bool> arg_pairs);
    
        //evaluate such that clause given left arg is variable(same declaration name as select value) and right arg is a constant(not a variable)
        static vector<string> such_that_eval_1(vector<string> first_arg_lst, string second_arg, Pair such_that_type, ActionsExecutor executor);
        
        //evaluate such that clause given left arg is a constant (not a variable) and right arg is a variable(same declaration name as select value)
        static vector<string> such_that_eval_2(string first_arg, vector<string> second_arg_lst, Pair such_that_type, ActionsExecutor executor);
        
        //evaluate such that clause given left and right are variables and at least one of them is same val as select value
        static map<string,vector<string>> such_that_eval_3(vector<pair<string,string>> arg_lst, Pair such_that_type, string first_arg_name, string second_arg_name, ActionsExecutor executor);
        
        //evaluate such that clause given left is a variable but not same declaration name as select value. and second var is a constant
        static vector<string> such_that_eval_4(Pair such_that_type, ActionsExecutor executor);
        

};
