#include "ActionsGenerator.h"
#include "StringUtil.h"
#include "SuchThatEval.h"
#include "PatternEval.h"
#include "SuchThatPatternEval.h"
#include "Types.hpp"
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;

//constructor
ActionsGenerator::ActionsGenerator() {}

ActionsGenerator::ActionsGenerator(QueryMap mapQuery, ActionsExecutor executorActions) {
    executor = executorActions;
    queryMap = mapQuery;
    preprocess();
}

// //methods
void ActionsGenerator::set_map_storage_store_declaration(unordered_map<Single, 
        unordered_map<string, vector<string>>> storage_map, 
        unordered_map<string, Single> store_declaration) {
            map_storage = storage_map;
            procedure_storage = storage_map[Single::PROCEDURE];
            stmt_storage = storage_map[Single::STATEMENT];
            read_storage = storage_map[Single::READ];
            print_storage = storage_map[Single::PRINT];
            assign_storage = storage_map[Single::ASSIGN];
            while_storage = storage_map[Single::WHILE];
            if_storage = storage_map[Single::IF];
            constant_storage = storage_map[Single::CONSTANT];
            variable_storage = storage_map[Single::VARIABLE];
            call_storage = storage_map[Single::CALL];
            this->store_declaration = store_declaration;
}

void ActionsGenerator::set_Query_Map(QueryMap mapQuery) {
    this->queryMap = mapQuery;
    declaration_list = mapQuery.getList(ClauseType::DECLARATION);
    select_list = mapQuery.getList(ClauseType::SELECT);
    such_that_list = mapQuery.getList(ClauseType::SUCHTHAT);
    pattern_list = mapQuery.getList(ClauseType::PATTERN);
    
}

std::vector<PayLoad> ActionsGenerator::get_pattern_list() {
    return pattern_list;
}
std::unordered_map<Single, 
        std::unordered_map<std::string, std::vector<std::string>>> ActionsGenerator::preprocess() {
    // preprocessing - retrieval of queryMap clauses
    declaration_list = queryMap.getList(ClauseType::DECLARATION);
    select_list = queryMap.getList(ClauseType::SELECT);
    such_that_list = queryMap.getList(ClauseType::SUCHTHAT);
    pattern_list = queryMap.getList(ClauseType::PATTERN);
    

    //DECLARATION
    for(auto i: declaration_list) {
        if (i.getType().single != Single::SYNONYM) {
            vector<string> payloadValues = i.getValue();
            store_declaration[payloadValues[0]] = i.getType().single;            
        }
    }
    
    for(auto i: this->store_declaration) {
        string declaration_name = i.first;
        Single s = i.second;
        if (s == Single::STATEMENT) {
            vector<stmt_ref> statement_lst = executor.get_all_statements_of_type(stmt_type::STATEMENT);
            vector<string> statement_lst_string;
            for(auto stmt : statement_lst) {
                statement_lst_string.push_back(to_string(stmt));
            }
            stmt_storage[declaration_name] = statement_lst_string;
        } else if (s == Single::READ) {    
            vector<stmt_ref> read_lst = executor.get_all_statements_of_type(stmt_type::READ);
            vector<string> read_lst_string;
            for(auto rd : read_lst) {
                read_lst_string.push_back(to_string(rd));
            }
            read_storage[declaration_name] = read_lst_string;           
        } else if (s == Single::PRINT) {
            vector<stmt_ref> print_lst = executor.get_all_statements_of_type(stmt_type::PRINT);
            vector<string> print_lst_string;
            for(auto pr : print_lst) {
                print_lst_string.push_back(to_string(pr));
            }
            print_storage[declaration_name] = print_lst_string;
        } else if (s == Single::WHILE) {
            vector<stmt_ref> while_lst = executor.get_all_statements_of_type(stmt_type::WHILE);
            vector<string> while_lst_string;
            for(auto while_ : while_lst) {
                while_lst_string.push_back(to_string(while_));
            }
            while_storage[declaration_name] = while_lst_string;
        } else if (s == Single::IF) {
            vector<stmt_ref> if_lst = executor.get_all_statements_of_type(stmt_type::IF);
            vector<string> if_lst_string;
            for(auto ifs : if_lst) {
                if_lst_string.push_back(to_string(ifs));
            }
            if_storage[declaration_name] = if_lst_string;
        } else if (s == Single::ASSIGN) {
            vector<stmt_ref> assign_lst = executor.get_all_statements_of_type(stmt_type::ASSIGN);
            vector<string> assign_lst_string;
            for(auto ass : assign_lst) {
                assign_lst_string.push_back(to_string(ass));
            }
            assign_storage[declaration_name] = assign_lst_string;  
        } else if (s == Single::CONSTANT) {
            vector<constant> constant_lst = executor.get_all_constants();
            vector<string> constant_lst_string;
            for(auto cnst : constant_lst) {
                constant_lst_string.push_back(cnst);
            }
            constant_storage[declaration_name] = constant_lst_string;   
        } else if (s == Single::VARIABLE) {
            vector<var_ref> variable_lst= executor.get_all_variables();
            variable_storage[declaration_name] = variable_lst;
        } else if (s == Single::PROCEDURE) {
            vector<proc_ref> procedure_lst = executor.get_all_procedures();
            procedure_storage[declaration_name] = procedure_lst;  
        } else if (s == Single::CALL) {
            vector<stmt_ref> call_lst = executor.get_all_statements_of_type(stmt_type::CALL);
            vector<string> call_lst_string;
            for(auto cll : call_lst) {
                call_lst_string.push_back(to_string(cll));
            }
            call_storage[declaration_name] = call_lst_string;  
        } else {
            throw "Payload Single is not STATEMENT/READ/PRINT/CALL/WHILE/IF/ASSIGN.";
        }
    } 

    //map all types of storage such as stmt, read etc into map_storage
    map_storage[Single::PROCEDURE] = procedure_storage;
    map_storage[Single::STATEMENT] = stmt_storage;
    map_storage[Single::READ] = read_storage;
    map_storage[Single::PRINT] = print_storage;
    map_storage[Single::ASSIGN] = assign_storage;
    map_storage[Single::WHILE] = while_storage;
    map_storage[Single::IF] = if_storage;
    map_storage[Single::CONSTANT] = constant_storage;
    map_storage[Single::VARIABLE] = variable_storage;
    return map_storage; 
}
vector<string> ActionsGenerator::TraverseQueryMap() {

    //SELECT
    if(select_list.empty()) {
        // return vector<string>{"Invalid"};
        throw "Missing SELECT payload.";
    }
    PayLoad select_payload = select_list.at(0);
    
    if (select_payload.getType().single != Single::SYNONYM) {
        throw "SELECT payload is not a synonym. Synonym type is required.";
    }
    string select_value = (select_payload.getValue())[0];
    Single select_type = store_declaration[select_value];
    
    vector<string> default_solution = (map_storage[select_type])[select_value];
    vector<string> solution_such_that;
    vector<string> solution_pattern;

    bool is_such_that_empty = such_that_list.empty();
    bool is_pattern_empty = pattern_list.empty();
    if(is_such_that_empty && is_pattern_empty) {
        // There is no such that and pattern clause.
        return default_solution;
    }
    if (!is_such_that_empty && is_pattern_empty) {
        // there is only such that clause and no pattern clause.
        PayLoad such_that_pay_load = such_that_list.at(0);

        // check if select value is in such that clauses(first and second)
        string such_that_first_arg = such_that_pay_load.getValue()[0];
        string such_that_second_arg = such_that_pay_load.getValue()[1];
        pair<bool, bool> is_select_val_in_suchthat(false, false);  
        if (such_that_first_arg == select_value) {
            is_select_val_in_suchthat.first = true;
        }
        if (such_that_second_arg == select_value) {
            is_select_val_in_suchthat.second = true;
        }
        SuchThatEval such_that_eval(store_declaration, map_storage, executor);

        vector<string> return_result = such_that_eval.one_such_that_zero_pattern(such_that_pay_load, select_value, select_type, is_select_val_in_suchthat);
        if (return_result.empty()) {
                return vector<string> {};
        } else {
            if (is_select_val_in_suchthat.first || is_select_val_in_suchthat.second) {
                return return_result;    
            } else {
                return default_solution;
            }
        }
    } else if (is_such_that_empty && !is_pattern_empty) {
        // there is only pattern clause and no such that
        PayLoad pattern_pay_load = pattern_list.at(0);
        string pattern_first_arg = pattern_pay_load.getValue()[0];
        string pattern_second_arg = pattern_pay_load.getValue()[1];
        string pattern_third_arg = pattern_pay_load.getValue()[2]; //sub expression
        pair<bool, bool> is_select_val_in_pattern(false, false);  
        if (pattern_first_arg == select_value) {
            is_select_val_in_pattern.first = true;
        }
        if (pattern_second_arg == select_value) {
            is_select_val_in_pattern.second = true;
        }
        PatternEval pattern_eval(store_declaration, map_storage, executor);

        vector<string> return_result = pattern_eval.zero_such_that_one_pattern(pattern_pay_load, select_value, select_type, is_select_val_in_pattern);
        
        if (return_result.empty()) {
                return vector<string> {};
        } else {
            if (is_select_val_in_pattern.first || is_select_val_in_pattern.second) {
                return return_result;    
            } else {
                return default_solution;
            }
        }
    } else {
        // there is both such that and pattern.
        PayLoad such_that_pay_load = such_that_list.at(0);
        PayLoad pattern_pay_load = pattern_list.at(0);
        SuchThatPatternEval such_that_pattern(store_declaration, map_storage, executor);
        
        vector<string> return_result = such_that_pattern.such_that_pattern_eval(such_that_pay_load, pattern_pay_load,select_value,select_type);
        
        string such_that_first_arg = such_that_pay_load.getValue()[0];
        string such_that_second_arg = such_that_pay_load.getValue()[1];
        string pattern_first_arg = pattern_pay_load.getValue()[0];
        string pattern_second_arg = pattern_pay_load.getValue()[1];
        pair<bool, bool> is_select_val_in_suchthat(false, false);  
        if (such_that_first_arg == select_value) {
            is_select_val_in_suchthat.first = true;
        }
        if (such_that_second_arg == select_value) {
            is_select_val_in_suchthat.second = true;
        }
        pair<bool, bool> is_select_val_in_pattern(false, false);  
        if (pattern_first_arg == select_value) {
            is_select_val_in_pattern.first = true;
        }
        if (pattern_second_arg == select_value) {
            is_select_val_in_pattern.second = true;
        }

        if (return_result.empty()) {
                return vector<string> {};
        } else {
            if (is_select_val_in_suchthat.first || is_select_val_in_suchthat.second || is_select_val_in_pattern.first || is_select_val_in_pattern.second) {
                return return_result;    
            } else {
                return default_solution;
            }
        }    
    }
    return default_solution;
}


// utilities

pair<bool,bool> ActionsGenerator::check_if_args_are_variable(std::string first_arg, std::string second_arg) {
    bool is_first_arg_variable = false;
    bool is_second_arg_variable = false;

    //check if string is in the store_declaration.
    if(store_declaration.find(first_arg) != store_declaration.end()) {
        is_first_arg_variable = true;
    }
    if(store_declaration.find(second_arg) != store_declaration.end()) {
        is_second_arg_variable = true;
    }
    std::pair<bool, bool> pairArgs;
    pairArgs.first = is_first_arg_variable;
    pairArgs.second = is_second_arg_variable;
    return pairArgs;
} 

vector<pair<string,string>> ActionsGenerator::crossproduct(vector<string> first_arg_lst, vector<string> second_arg_lst){
    vector<pair <string, string>> products;
    for(auto i : first_arg_lst) {
        for(auto j : second_arg_lst) {
            products.push_back(make_pair(i,j));
        }
    }
    return products;
};

vector<string> ActionsGenerator::inner_join_A(vector<string> lstA, vector<string> lstB) {
    // add all elements of lst A that is present in lstB.
    vector<string> result;
    for(auto item : lstA) {
        if(std::find(lstB.begin(), lstB.end(), item) != lstB.end()) {
            result.push_back(item);
        }
    }
    return result;
}


bool ActionsGenerator::is_element_inside_vectorA(string element, vector<string> vectorA) {
    return std::find(vectorA.begin(), vectorA.end(), element) != vectorA.end();
}
stmt_type ActionsGenerator::convert_single_to_stmt_type(Single s) {
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