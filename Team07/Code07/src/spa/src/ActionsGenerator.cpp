#include "ActionsGenerator.h"
#include "StringUtil.h"
#include "SuchThatEval.h"
#include "Types.hpp"
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

ActionsGenerator::ActionsGenerator() {}

ActionsGenerator::ActionsGenerator(QueryMap mapQuery, ActionsExecutor executerActions) {
    executer = executerActions;
    queryMap = mapQuery;
}

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
    stmtStorage["s"] = vector<string>{"1","2","3"};
    stmtStorage["s1"] = vector<string>{"1","2","3"};
    assignStorage["a"] = vector<string>{"1","2","3"};
    constantStorage["const"] = vector<string>{"1"};
    variableStorage["v"] = vector<string>{"x","y","z"}; 

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
        if (is_select_val_in_suchthat.first || is_select_val_in_suchthat.second) {
            return one_such_that_zero_pattern(such_that_pay_load, select_value, select_type, is_select_val_in_suchthat);
        } else {
            // such that clause does not have select values inside. similar to kiv #1
            // kiv #1.a: return default clause first
            return default_solution;
        }
    } else if (is_such_that_empty && !is_pattern_empty) {
        // there is pattern clause only and no such that
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
        if (is_select_val_in_pattern.first || is_select_val_in_pattern.second) {
            //select a pattern a(v,_) or select v pattern a(v,_)
            return zero_such_that_one_pattern(pattern_pay_load, select_value, select_type, is_select_val_in_pattern);
        } else {
            //pattern clause does not have same values as select value, Select pn pattern a(v,_)
            //kiv #1.b - return default first
            return default_solution;
        }
    } //else {
    //     // there is both such that and pattern.
    //     //todo
    //     PayLoad such_that_pay_load = suchThatList.at(0);
    //     PayLoad pattern_pay_load = patternList.at(0);
    //     string such_that_first_arg = such_that_pay_load.getValue()[0];
    //     string such_that_second_arg = such_that_pay_load.getValue()[1];
    //     string pattern_first_arg = pattern_pay_load.getValue()[0];
    //     string pattern_second_arg = pattern_pay_load.getValue()[1];
    //     bool st_first_p_first = such_that_first_arg == pattern_first_arg;
    //     bool st_first_p_second = such_that_first_arg == pattern_second_arg; 
    //     bool st_second_p_first = such_that_second_arg == pattern_first_arg;
    //     bool st_second_p_second = such_that_second_arg == pattern_second_arg;
    //     if (st_first_p_first && st_second_p_second) {
    //         //two common synonyms - such that first, pattern first and such that second, pattern second

    //     }
    //     if(st_first_p_first ) {
    //         // one common synonym
            
    //     }
    //}
}

vector<string> ActionsGenerator::zero_such_that_one_pattern(PayLoad pattern_pay_load, string select_value, Single select_type, pair<bool,bool> arg_pairs) {
    string first_arg = pattern_pay_load.getValue()[0];
    string second_arg = pattern_pay_load.getValue()[1];
    string third_arg = pattern_pay_load.getValue()[2];
    if(arg_pairs.first && !arg_pairs.second) {
        //select a pattern a(v,_) first arg is same as select value
        pattern p; p.lvalue = "_"; p.rvalue = third_arg;
        vector<stmt_ref> all_stmts_p_pkb;//kiv #4 = executer.get_all_stmts_pattern(p); //stmt_ref is int
        vector<string> result;
        for(auto i : all_stmts_p_pkb) {
            result.push_back(to_string(i));
        }
        return result;
    } else if (!arg_pairs.first && arg_pairs.second) {
        //select v pattern a(v,_); second arg is same as select value
        //kiv #3: check get_all_variables_pattern_assign parameters again
        //return executer.get_all_variables_pattern_assign()
        return (mapStorage[select_type])[select_value];
    } else {
        // pattern clause does not have select values. select s pattern a(v,_)
        //kiv #1.b- return default first
        return (mapStorage[select_type])[select_value];
    }
}

vector<string> ActionsGenerator::one_such_that_zero_pattern(PayLoad such_that_pay_load, string select_value, Single select_type, pair<bool,bool> arg_pairs) {
    pair<bool, bool> bool_pairs_args = ActionsGenerator::check_if_args_are_variable(such_that_pay_load.getValue()[0], such_that_pay_load.getValue()[1]);
    Pair such_that_type = such_that_pay_load.getType().pair;
    string such_that_first_arg = such_that_pay_load.getValue()[0];
    string such_that_second_arg = such_that_pay_load.getValue()[1];

    if (bool_pairs_args.first && bool_pairs_args.second) {
        // this means both arguments are variables. might need to cross product
        vector<string> first_arg_lst = mapStorage[storeDeclaration[such_that_first_arg]][such_that_first_arg];
        vector<string> second_arg_lst = mapStorage[storeDeclaration[such_that_second_arg]][such_that_second_arg];
        //kiv #2
        vector<pair<string,string>> products = ActionsGenerator::crossproduct(first_arg_lst, second_arg_lst);
        map<string, vector<string>> output = SuchThatEval::such_that_eval_3(products, such_that_type, such_that_pay_load.getValue()[0], such_that_pay_load.getValue()[1],executer);
        return output[select_value];
    } else if (bool_pairs_args.first && !bool_pairs_args.second) {
        // means first is variable. second is not variable(constant string)
        if(arg_pairs.first) {
            // first argument is same declaration name as SELECT
            vector<string> first_arg_lst = (mapStorage[select_type])[select_value];
            return SuchThatEval::such_that_eval_1(first_arg_lst, such_that_second_arg, such_that_type, executer);
        } else {
            // first argument is not same declaration name as SELECT. need to evaluate if this such that returns true. if true return default soln.
            //kiv #1
            vector<string> default_solution = (mapStorage[select_type])[select_value];
            return default_solution;
        }

    } else if (!bool_pairs_args.first && bool_pairs_args.second) {
        // means first is not variable(constant string). second is variable.
        if(arg_pairs.second) {   
            //second argument is same declaration name as SELECT   
            vector<string> second_arg_lst = (mapStorage[select_type])[select_value];
            return SuchThatEval::such_that_eval_2(such_that_pay_load.getValue()[0], second_arg_lst, such_that_type, executer);
        } else {
        // second argument is not same declaration name as SELECT. need to evaluate if this such that returns true. if true return default soln.
        //kiv #1
        vector<string> default_solution = (mapStorage[select_type])[select_value];
        return default_solution;
        } 
    } else {
        // first and second args are not variables. (they are constants)
        //kiv #1
        vector<string> default_solution = (mapStorage[select_type])[select_value];
        return default_solution;
    }
}
   
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
