#include "SuchThatPatternEval.h"
#include "StringUtil.h"
#include "ActionsExecutor.h"
#include "ActionsGenerator.h"

//constructor
SuchThatPatternEval::SuchThatPatternEval(unordered_map<string, Single> declaration_store, unordered_map<Single, 
            unordered_map<string, vector<string>>> map_storage, ActionsExecutor executor_) {
                storeDeclaration = declaration_store;
                mapStorage = map_storage;
                executor = executor_; 
}

//utilities
pair<bool,bool> SuchThatPatternEval::check_if_args_are_variable(std::string first_arg, std::string second_arg) {
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

bool SuchThatPatternEval::is_element_inside_vectorA(string element, vector<string> vectorA) {
    return std::find(vectorA.begin(), vectorA.end(), element) != vectorA.end();
}

stmt_type SuchThatPatternEval::convert_single_to_stmt_type(Single s) {
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

vector<string> SuchThatPatternEval::is_result_empty_yes_none_no_default_soln(bool is_result_empty, Single select_type, string select_value) {
    if (is_result_empty) {
        return vector<string>{"None"};              
    } else {
        vector<string> default_solution = (mapStorage[select_type])[select_value];
        return default_solution;                      
    }
}