#include <PatternEval.h>
#include "StringUtil.h"
#include "ActionsExecutor.h"
#include "ActionsGenerator.h"

//constructor
PatternEval::PatternEval(unordered_map<string, Single> declaration_store, unordered_map<Single, 
            unordered_map<string, vector<string>>> map_storage, ActionsExecutor executor_) {
                storeDeclaration = declaration_store;
                mapStorage = map_storage;
                executor = executor_; 
}

//methods
vector<string> PatternEval::zero_such_that_one_pattern(PayLoad pattern_pay_load, string select_value, Single select_type, pair<bool,bool> arg_pairs) {
    string first_arg = pattern_pay_load.getValue()[0];
    string second_arg = pattern_pay_load.getValue()[1];
    string third_arg = pattern_pay_load.getValue()[2]; //sub expression
    pair<bool, bool> bool_pairs_args = PatternEval::check_if_args_are_variable(first_arg, second_arg);
    //first arg has to be variable by convention.
    
    pattern p; p.rvalue = third_arg;
    if (bool_pairs_args.second) {
        // pattern a(v,_) where _ refers to any string term. first arg has to be variable by convention.
        p.lvalue = "_";
    } else {
        // pattern a("something", _) where _ refers to any string term
        p.lvalue = second_arg;
    }  
    if (select_type == Single::ASSIGN) {
        vector<stmt_ref> result = executor.get_all_stmts_pattern(p);
        if (arg_pairs.first) {
            vector<string> result2;
            for(auto item : result) {
                result2.push_back(to_string(item));
            }
            return result2;
        } else {
            // select a1 pattern a(v,_);
            return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
        }
    } else if (select_type == Single::VARIABLE) {
        if (third_arg == "_") {
            vector<var_ref> result  = executor.get_all_variables_pattern_assign();
            if(arg_pairs.second) {
                return result;
            } else {
                return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
            }
        } else {
            vector<var_ref> result  = executor.get_all_variables_pattern_assign(third_arg);
            if(arg_pairs.second) {
                return result;
            } else {
                return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
            }
        }
    } else {
        vector<stmt_ref> result = executor.get_all_stmts_pattern(p);
        return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
    }
}


// utilities
pair<bool,bool> PatternEval::check_if_args_are_variable(std::string first_arg, std::string second_arg) {
    bool is_first_arg_variable = false;
    bool is_second_arg_variable = false;

    //check if string is in the storedeclaration.
    if(storeDeclaration.find(first_arg) != storeDeclaration.end()) {
        is_first_arg_variable = true;
    }
    if(storeDeclaration.find(second_arg) != storeDeclaration.end()) {
        is_second_arg_variable = true;
    }
    std::pair<bool, bool> pairArgs;
    pairArgs.first = is_first_arg_variable;
    pairArgs.second = is_second_arg_variable;
    return pairArgs;
}

bool PatternEval::is_element_inside_vectorA(string element, vector<string> vectorA) {
    return std::find(vectorA.begin(), vectorA.end(), element) != vectorA.end();
}

stmt_type PatternEval::convert_single_to_stmt_type(Single s) {
    if (s == Single::STATEMENT) {
        return stmt_type::STATEMENT;
    } else if (s == Single::READ) {
        return stmt_type::READ;
    } else if (s == Single::PRINT) {
        return stmt_type::PRINT;
    } else if (s == Single::CALL) {
        return stmt_type::CALL;
    } else if (s == Single::WHILE) {
        return stmt_type::WHILE;
    } else if (s == Single::IF) {
        return stmt_type::IF;
    } else if (s == Single::ASSIGN) {
        return stmt_type::ASSIGN;
    } else if (s == Single::CONSTANT) {
        return stmt_type::CONSTANT;
    } else if (s == Single::VARIABLE) {
        return stmt_type::VARIABLE;
    } else if (s == Single::PROCEDURE) {
        return stmt_type::PROCEDURE;
    } else {
        return stmt_type::STATEMENT;
    }
}

vector<string> PatternEval::is_result_empty_yes_none_no_default_soln(bool is_result_empty, Single select_type, string select_value) {
    if (is_result_empty) {
        return vector<string>{"None"};              
    } else {
        vector<string> default_solution = (mapStorage[select_type])[select_value];
        return default_solution;                      
    }
}
