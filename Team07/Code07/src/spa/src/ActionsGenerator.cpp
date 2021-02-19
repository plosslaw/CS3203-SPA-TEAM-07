#include "ActionsGenerator.h"
#include "StringUtil.h"
#include "SuchThatEval.h"
#include "PatternEval.h"
#include "Types.hpp"
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;

//constructor
ActionsGenerator::ActionsGenerator() {}

ActionsGenerator::ActionsGenerator(QueryMap mapQuery, ActionsExecutor executerActions) {
    executer = executerActions;
    queryMap = mapQuery;
}

//methods
vector<string> ActionsGenerator::TraverseQueryMap() {
    
    // preprocessing - retrieval of queryMap clauses
    declarationList = queryMap.getList(ClauseType::DECLARATION);
    selectList = queryMap.getList(ClauseType::SELECT);
    suchThatList = queryMap.getList(ClauseType::SUCHTHAT);
    patternList = queryMap.getList(ClauseType::PATTERN);
    

    //DECLARATION
    for(auto i: declarationList) {
        if (i.getType().single != Single::SYNONYM) {
            vector<string> payloadValues = i.getValue();
            storeDeclaration[payloadValues[0]] = i.getType().single;            
        }
    }
    
    for(auto i: storeDeclaration) {
        string declaration_name = i.first;
        Single s = i.second;

        if (s == Single::STATEMENT) {
            vector<stmt_ref> statement_lst = executer.get_all_statements_of_type(stmt_type::STATEMENT);
            vector<string> statement_lst_string;
            for(auto stmt : statement_lst) {
                statement_lst_string.push_back(to_string(stmt));
            }
            stmtStorage[declaration_name] = statement_lst_string;
        } else if (s == Single::READ) {    
            vector<stmt_ref> read_lst = executer.get_all_statements_of_type(stmt_type::READ);
            vector<string> read_lst_string;
            for(auto rd : read_lst) {
                read_lst_string.push_back(to_string(rd));
            }
            readStorage[declaration_name] = read_lst_string;             
        } else if (s == Single::PRINT) {
            vector<stmt_ref> print_lst = executer.get_all_statements_of_type(stmt_type::PRINT);
            vector<string> print_lst_string;
            for(auto pr : print_lst) {
                print_lst_string.push_back(to_string(pr));
            }
            stmtStorage[declaration_name] = print_lst_string;
        } else if (s == Single::WHILE) {
            vector<stmt_ref> while_lst = executer.get_all_statements_of_type(stmt_type::WHILE);
            vector<string> while_lst_string;
            for(auto while_ : while_lst) {
                while_lst_string.push_back(to_string(while_));
            }
            whileStorage[declaration_name] = while_lst_string;
        } else if (s == Single::IF) {
            vector<stmt_ref> if_lst = executer.get_all_statements_of_type(stmt_type::IF);
            vector<string> if_lst_string;
            for(auto ifs : if_lst) {
                if_lst_string.push_back(to_string(ifs));
            }
            ifStorage[declaration_name] = if_lst_string;
        } else if (s == Single::ASSIGN) {
            vector<stmt_ref> assign_lst = executer.get_all_statements_of_type(stmt_type::ASSIGN);
            vector<string> assign_lst_string;
            for(auto ass : assign_lst) {
                assign_lst_string.push_back(to_string(ass));
            }
            assignStorage[declaration_name] = assign_lst_string;  
        } else if (s == Single::CONSTANT) {
            vector<constant> constant_lst = executer.get_all_constants();
            vector<string> constant_lst_string;
            for(auto cnst : constant_lst) {
                constant_lst_string.push_back(to_string(cnst));
            }
            constantStorage[declaration_name] = constant_lst_string;   
        } else if (s == Single::VARIABLE) {
            vector<var_ref> variable_lst= executer.get_all_variables();
            variableStorage[declaration_name] = variable_lst;
        } else {
            throw "Payload Single is not STATEMENT/READ/PRINT/CALL/WHILE/IF/ASSIGN.";
        }
    } 

    //map all types of storage such as stmt, read etc into mapStorage
    mapStorage[Single::PROCEDURE] = procedureStorage;
    mapStorage[Single::STATEMENT] = stmtStorage;
    mapStorage[Single::READ] = readStorage;
    mapStorage[Single::PRINT] = printStorage;
    mapStorage[Single::ASSIGN] = assignStorage;
    mapStorage[Single::WHILE] = whileStorage;
    mapStorage[Single::IF] = ifStorage;
    mapStorage[Single::CONSTANT] = constantStorage;
    mapStorage[Single::VARIABLE] = variableStorage;
    
    //SELECT
    PayLoad select_payload = selectList.at(0);
    if (select_payload.getType().single != Single::SYNONYM) {
        throw "SELECT payload is not a synonym. Synonym type is required.";
    }
    string select_value = (select_payload.getValue())[0];
    Single select_type = storeDeclaration[select_value];
    vector<string> default_solution = (mapStorage[select_type])[select_value];
    vector<string> solution_such_that;
    vector<string> solution_pattern;

    bool is_such_that_empty = suchThatList.empty();
    bool is_pattern_empty = patternList.empty();
    if(is_such_that_empty && is_such_that_empty) {
        // There is no such that and pattern clause.
        return default_solution;
    }
    if (!is_such_that_empty && is_pattern_empty) {
        // there is only such that clause and no pattern clause.
        PayLoad such_that_pay_load = suchThatList.at(0);

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
        SuchThatEval such_that_eval(storeDeclaration, mapStorage, executer);

        vector<string> return_result = such_that_eval.one_such_that_zero_pattern(such_that_pay_load, select_value, select_type, is_select_val_in_suchthat);
        if (is_select_val_in_suchthat.first || is_select_val_in_suchthat.second) {
            return return_result;    
        } else {
            if (return_result.empty()) {
                return vector<string> {"None"};
            } else {
                return default_solution;
            }
        }
    } else if (is_such_that_empty && !is_pattern_empty) {
        // there is only pattern clause and no such that
        PayLoad pattern_pay_load = patternList.at(0);
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
        PatternEval pattern_eval(storeDeclaration, mapStorage, executer);

        vector<string> return_result = pattern_eval.zero_such_that_one_pattern(pattern_pay_load, select_value, select_type, is_select_val_in_pattern);
        if (is_select_val_in_pattern.first || is_select_val_in_pattern.second) {
            //select a pattern a(v,_) or select v pattern a(v,_)
            return return_result;
        } else {
            if (return_result.empty()) {
                return vector<string> {"None"};
            } else {
                return default_solution;
            }
        }
    } else {
        // there is both such that and pattern.
        //todo
        PayLoad such_that_pay_load = suchThatList.at(0);
        PayLoad pattern_pay_load = patternList.at(0);
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
        if (!is_select_val_in_suchthat.first && !is_select_val_in_suchthat.second && !is_select_val_in_pattern.first && !is_select_val_in_pattern.second) {
            //select value is not present in both such that clause and pattern clause
            return default_solution;
        }
        bool st_first_p_first = such_that_first_arg == pattern_first_arg;
        bool st_first_p_second = such_that_first_arg == pattern_second_arg; 
        bool st_second_p_first = such_that_second_arg == pattern_first_arg;
        bool st_second_p_second = such_that_second_arg == pattern_second_arg;
        if (st_first_p_first && st_second_p_second || st_first_p_second && st_second_p_first) {
            //two common synonyms - such that first, pattern first and such that second, pattern second
            return ActionsGenerator::two_common_synonyms(such_that_pay_load, pattern_pay_load, select_value, select_type);
        }
        if (!st_first_p_first && !st_first_p_first && !st_second_p_first &&!st_second_p_second) {
            // no common synonym
             return ActionsGenerator::zero_common_synonyms(such_that_pay_load, pattern_pay_load, select_value, select_type);
        }
        else {

        }
    }
}

// vector<string> ActionsGenerator::zero_common_synonyms(PayLoad such_that_load, PayLoad pattern_load, string select_value, Single select_type) {
//     string such_that_first_arg = such_that_load.getValue()[0];
//     string such_that_second_arg = such_that_load.getValue()[1];
//     //
// }
// vector<string> ActionsGenerator::two_common_synonyms(PayLoad such_that_load, PayLoad pattern_load, string select_value, Single select_type) {
//     string such_that_first_arg = such_that_load.getValue()[0];
//     string such_that_second_arg = such_that_load.getValue()[1];
    
//     //evaluate such that clause first then pass the evaluated such that result to pattern.
//     vector<string> first_arg_lst = mapStorage[storeDeclaration[such_that_first_arg]][such_that_first_arg];
//     vector<string> second_arg_lst = mapStorage[storeDeclaration[such_that_second_arg]][such_that_second_arg];
//     vector<pair<string,string>> products = ActionsGenerator::crossproduct(first_arg_lst, second_arg_lst);
//     map<string, vector<string>> output = SuchThatEval::such_that_eval_3(products, such_that_load.getType().pair, such_that_first_arg, such_that_second_arg,executer);
    
//     //evaluate pattern clause by passing results from suchthat clause to it
//     map<string, vector<string>> output2 = ActionsGenerator::two_common_synonyms_pattern_val(pattern_load, output);
//     return output2[select_value];
// }

// map<string,vector<string>> ActionsGenerator::two_common_synonyms_pattern_val(PayLoad pattern_load, map<string,vector<string>> eval_map) {
//     string pattern_first_arg = pattern_load.getValue()[0];
//     string pattern_second_arg = pattern_load.getValue()[1];
//     string pattern_third_arg = pattern_load.getValue()[2];
//     vector<string> first_arg_lst = eval_map[pattern_first_arg]; 
//     vector<string> second_arg_lst = eval_map[pattern_second_arg];
//     vector<pair<string,string>> products = ActionsGenerator::crossproduct(first_arg_lst, second_arg_lst);
//     return PatternEval::pattern_eval_two_variables(products, pattern_first_arg, pattern_second_arg, pattern_third_arg, executer);
// }    
   
// utilities

pair<bool,bool> ActionsGenerator::check_if_args_are_variable(std::string first_arg, std::string second_arg) {
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