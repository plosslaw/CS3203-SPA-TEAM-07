#include "SuchThatPatternEval.h"
#include "SuchThatEval.h"
#include "PatternEval.h"
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

//method
vector<string> SuchThatPatternEval::such_that_pattern_eval(PayLoad such_that_pay_load, PayLoad pattern_pay_load,string select_value, Single select_type) {
    vector<string> such_that_values = such_that_pay_load.getValue();
    vector<string> pattern_values = pattern_pay_load.getValue();
    int common_link = 0; // 0 common link represents such that and pattern has no similar variable names
    for(int such_that_value = 0; such_that_value < such_that_values.size(); such_that_value++) {
        for(int pattern_value = 0; pattern_value < pattern_values.size()-1; pattern_value++) {
            if(such_that_values[such_that_value] == pattern_values[pattern_value]) {
                common_link++;
            }
        }
    }
    vector<string> result;
    if(common_link == 0) {
        result = SuchThatPatternEval::zero_common_synonym(such_that_pay_load, pattern_pay_load,select_value,select_type);
    } else if (common_link == 1) {
        result = SuchThatPatternEval::one_common_synonym(such_that_pay_load, pattern_pay_load, select_value, select_type);
    } else {
        result = SuchThatPatternEval::more_than_one_common_synonym(such_that_pay_load, pattern_pay_load, select_value, select_type);
    }
    return result;
}

vector<string> SuchThatPatternEval::zero_common_synonym(PayLoad such_that_pay_load, PayLoad pattern_pay_load,string select_value, Single select_type) {
    string such_that_first_arg = such_that_pay_load.getValue()[0];
    string such_that_second_arg = such_that_pay_load.getValue()[1];
    string pattern_first_arg = pattern_pay_load.getValue()[0];
    string pattern_second_arg = pattern_pay_load.getValue()[1];
    
    SuchThatEval such_that_eval(storeDeclaration, mapStorage,executor);
    PatternEval pattern_eval(storeDeclaration, mapStorage,executor);
    pair<bool, bool> is_select_val_in_suchthat(false, false);  
        if (such_that_first_arg == select_value) {
            is_select_val_in_suchthat.first = true;
        }
        if (such_that_second_arg == select_value) {
            is_select_val_in_suchthat.second = true;
        }
    vector<string> such_that_lst = such_that_eval.one_such_that_zero_pattern(such_that_pay_load, select_value, select_type, is_select_val_in_suchthat);
    pair<bool, bool> is_select_val_in_pattern(false, false);  
        if (pattern_first_arg == select_value) {
            is_select_val_in_pattern.first = true;
        }
        if (pattern_second_arg == select_value) {
            is_select_val_in_pattern.second = true;
        }
    vector<string> pattern_lst =  pattern_eval.zero_such_that_one_pattern(pattern_pay_load, select_value, select_type, is_select_val_in_pattern);
    if(is_select_val_in_suchthat.first || is_select_val_in_suchthat.second) {
        return such_that_lst;
    } else if (is_select_val_in_pattern.first || is_select_val_in_pattern.second) {
        return pattern_lst;
    } else {
        if(such_that_lst.empty() || pattern_lst.empty()) {
            return vector<string>();
        } else {
            return (mapStorage[select_type])[select_value];
        }
    }
}

//select a such that uses(a,v) pattern a(v2,_);
vector<string> SuchThatPatternEval::one_common_synonym(PayLoad such_that_pay_load, PayLoad pattern_pay_load,string select_value, Single select_type) {
    string such_that_first_arg = such_that_pay_load.getValue()[0];
    Single such_that_first_type = storeDeclaration[such_that_first_arg];

    string such_that_second_arg = such_that_pay_load.getValue()[1];
    Single such_that_second_type = storeDeclaration[such_that_second_arg];

    string pattern_first_arg = pattern_pay_load.getValue()[0];
    string pattern_second_arg = pattern_pay_load.getValue()[1];
    string pattern_third_arg = pattern_pay_load.getValue()[2];

    Pair such_that_type = such_that_pay_load.getType().pair;
    Pair pattern_type = pattern_pay_load.getType().pair;

    SuchThatEval such_that_eval(storeDeclaration, mapStorage,executor);
    PatternEval pattern_eval(storeDeclaration, mapStorage,executor);

    bool is_first_arg_common_such_that = such_that_first_arg == pattern_first_arg || such_that_first_arg == pattern_second_arg;
    bool is_first_arg_common_pattern = pattern_first_arg == such_that_first_arg|| pattern_first_arg == such_that_second_arg;
    
    // check if select value appears in such that or pattern
    pair<bool, bool> such_that_or_pattern(false, false);
    if(select_value == such_that_first_arg || select_value == such_that_second_arg) {
        such_that_or_pattern.first = true;
    }
    if (select_value == pattern_first_arg || select_value == pattern_second_arg) {
        such_that_or_pattern.second = true;
    }

    Single common_type;
    if(is_first_arg_common_pattern) {
        common_type = Single::ASSIGN;
    } else {
        common_type = Single::VARIABLE;
    }
    
    if(common_type == Single::VARIABLE) {
        //evaluate such that
        pair<bool, bool> arg_pairs;
        arg_pairs.first = false;
        arg_pairs.second = true;
        vector<string> such_that_result = such_that_eval.one_such_that_zero_pattern(such_that_pay_load, such_that_second_arg, Single::VARIABLE,arg_pairs);
        //evaluate pattern
        vector<string> pattern_result = pattern_eval.zero_such_that_one_pattern(pattern_pay_load, such_that_second_arg, Single::VARIABLE,arg_pairs);
        vector<string> inner_join_lst = SuchThatPatternEval::inner_join(such_that_result, pattern_result);
        if(inner_join_lst.empty()) {
            return vector<string>{};
        }
        if(select_type == Single::VARIABLE) {
            return inner_join_lst;
        }
        //select_type is not variable
        // check if select value appears in such that or pattern
        if (such_that_or_pattern.first && !such_that_or_pattern.second) {
            // select value appears in such that      
            if (such_that_first_type == Single::PROCEDURE) {
                vector<proc_ref> result;
                for(auto item : inner_join_lst) {
                    if (such_that_type == Pair::USES) {
                        vector<proc_ref> result2 = executor.get_all_procedures_uses(item);
                        for(auto item2:result2) {
                            bool is_in_result = SuchThatPatternEval::is_element_inside_vectorA(item2, result);
                            if (!is_in_result) {
                                result.push_back(item2);
                            }
                        }
                    } else {    
                        vector<proc_ref> result2 = executor.get_all_procedures_modifies(item);
                        for(auto item2:result2) {
                            bool is_in_result = SuchThatPatternEval::is_element_inside_vectorA(item2, result);
                            if (!is_in_result) {
                                result.push_back(item2);
                            }
                        }
                    }    
                }
                return result;
            } else {
                vector<stmt_ref> result;
                for(auto item : inner_join_lst) {
                    if (such_that_type == Pair::USES) {
                        stmt_type st_first_stmt_type = SuchThatPatternEval::convert_single_to_stmt_type(such_that_first_type);
                        vector<stmt_ref> result2 = executor.get_all_stmts_uses(st_first_stmt_type, item);
                        for(auto item2:result2) {
                            bool is_in_result = SuchThatPatternEval::is_element_inside_vectorA_int(item2, result);
                            if (!is_in_result) {
                                result.push_back(item2);
                            }
                        }
                    } else {    
                        stmt_type st_first_stmt_type = SuchThatPatternEval::convert_single_to_stmt_type(such_that_first_type);
                        vector<stmt_ref> result2 = executor.get_all_stmts_modifies(st_first_stmt_type, item);
                        for(auto item2:result2) {
                            bool is_in_result = SuchThatPatternEval::is_element_inside_vectorA_int(item2, result);
                            if (!is_in_result) {
                                result.push_back(item2);
                            }
                        }
                    }    
                }
                return SuchThatPatternEval::convert_lst_string_to_int(result);
            } 
        } else if (!such_that_or_pattern.first && such_that_or_pattern.second) {
            // select value appears in pattern
            vector<stmt_ref> result;
            for(auto i : inner_join_lst) {
                pattern p; p.lvalue = i; p.rvalue = pattern_third_arg;
                vector<stmt_ref> assign_lst = executor.get_all_stmts_pattern(p);
                for (auto j : assign_lst) {
                    bool is_present = SuchThatPatternEval::is_element_inside_vectorA_int(j, result);
                    if (!is_present) {
                        result.push_back(j);
                    }
                }
            }
            return SuchThatPatternEval::convert_lst_string_to_int(result);
        } else {
            // select value appears neither in such that or pattern
            return vector<string>{};       
        }
    } 
    else {
        //common type = ASSIGNMENT
        
        if(such_that_type == Pair::FOLLOWS || such_that_type == Pair::FOLLOWST || such_that_type == Pair::PARENT || such_that_type == Pair::PARENTT) {
            // determine if assignment is in first or second arg of such that clause.

            bool is_assign_first_arg_st = is_first_arg_common_such_that;
            pair<bool, bool> arg_pairs(false,false);
            vector<string> such_that_result;
            if(is_assign_first_arg_st) {
                arg_pairs.first = true;
                arg_pairs.second = false;
                such_that_result = such_that_eval.one_such_that_zero_pattern(such_that_pay_load, such_that_first_arg, Single::ASSIGN,arg_pairs);
            } else {
                arg_pairs.first = false;
                arg_pairs.second = true;
                such_that_result = such_that_eval.one_such_that_zero_pattern(such_that_pay_load, such_that_second_arg, Single::ASSIGN,arg_pairs);
            }
                
            arg_pairs.first = true;
            arg_pairs.second = false;
            vector<string> pattern_result = pattern_eval.zero_such_that_one_pattern(pattern_pay_load, pattern_first_arg, Single::ASSIGN,arg_pairs);

            vector<string> inner_join_lst = SuchThatPatternEval::inner_join(such_that_result, pattern_result);

            if(inner_join_lst.empty()) {
                return vector<string>{};
            }
            if(select_type == Single::ASSIGN) {
                return inner_join_lst;
            }
            //select type is not assign.
            if (such_that_or_pattern.first && !such_that_or_pattern.second) {
                //select value appears in such that (follows/* and parent/*)
                bool IS_STAR = such_that_type == Pair::FOLLOWST ||  such_that_type == Pair::PARENTT;
                if (such_that_type == Pair::FOLLOWS || such_that_type == Pair::FOLLOWST) {
                    vector<stmt_ref> temp;
                    for(auto i : inner_join_lst) {
                        vector<stmt_ref> result;
                        if (is_assign_first_arg_st) {
                            stmt_type second_arg_stmt_type = SuchThatPatternEval::convert_single_to_stmt_type(such_that_second_type);
                            result = executor.get_all_stmts_follows_ref(second_arg_stmt_type,arg_pos::SECOND_ARG, stoi(i), IS_STAR);
                        } else {
                            
                            stmt_type first_arg_stmt_type = SuchThatPatternEval::convert_single_to_stmt_type(such_that_first_type);
                            result = executor.get_all_stmts_follows_ref(first_arg_stmt_type,arg_pos::FIRST_ARG, stoi(i), IS_STAR);           
                        }
                        for(auto j : result) {
                            bool is_present = SuchThatPatternEval::is_element_inside_vectorA_int(j, temp);
                            if (!is_present) {
                                temp.push_back(j);
                            }
                        }
                    }
                    return SuchThatPatternEval::convert_lst_string_to_int(temp);
                } else {
                    // PARENT or PARENTT
                    vector<stmt_ref> temp;
                    for(auto i : inner_join_lst) {
                        vector<stmt_ref> result;
                        if (is_assign_first_arg_st) {
                            stmt_type such_arg_stmt_type = SuchThatPatternEval::convert_single_to_stmt_type(such_that_second_type);
                            result = executor.get_all_stmts_parent_ref(such_arg_stmt_type,arg_pos::SECOND_ARG, stoi(i), IS_STAR);
                        } else {
                            stmt_type first_arg_stmt_type = SuchThatPatternEval::convert_single_to_stmt_type(such_that_first_type);
                            result = executor.get_all_stmts_parent_ref(first_arg_stmt_type,arg_pos::FIRST_ARG, stoi(i), IS_STAR);           
                        }
                        for(auto j : result) {
                            bool is_present = SuchThatPatternEval::is_element_inside_vectorA_int(j, temp);
                            if (!is_present) {
                                temp.push_back(j);
                            }
                        }
                    }
                    return SuchThatPatternEval::convert_lst_string_to_int(temp);
                }
            } else if (!such_that_or_pattern.first && such_that_or_pattern.second) {
                // select values appear in pattern clause
                //vector<var_ref> get_variable_pattern_assign(stmt_ref assign_stmt,string pattern_string);
                vector<var_ref> result;
                for(auto i : inner_join_lst) {
                    vector<var_ref> temp = executor.get_variable_pattern_assign(stoi(i), pattern_third_arg);
                    for(auto j : temp) {
                        bool is_present = SuchThatPatternEval::is_element_inside_vectorA(j, result);
                        if(!is_present) {
                            result.push_back(j);
                        }
                    }
                }
                return result;
            } else {
                //select appear neither in such that or pattern
               
                return inner_join_lst;                
            }
        } else {
            //such that uses and modifies
            pair<bool, bool> arg_pairs;
            arg_pairs.first = true;
            arg_pairs.second = false;
            vector<string> such_that_result = such_that_eval.one_such_that_zero_pattern(such_that_pay_load, such_that_first_arg, Single::ASSIGN,arg_pairs);
            //evaluate pattern
            vector<string> pattern_result = pattern_eval.zero_such_that_one_pattern(pattern_pay_load, such_that_first_arg, Single::ASSIGN,arg_pairs);
            vector<string> inner_join_lst = SuchThatPatternEval::inner_join(such_that_result, pattern_result);
            //inner_join_lst is list of assignments statements.
            if(inner_join_lst.empty()) {
            return vector<string>{};
            }
            if(select_type == Single::ASSIGN) {
                return inner_join_lst;
            }
            // select type is not assign
            if (such_that_or_pattern.first && !such_that_or_pattern.second) {
                //select value appears in such that (uses and modifies)
                // by default select value has to be variable in such that clause
                vector<var_ref> result;
                for(auto item : inner_join_lst) {
                    vector<var_ref> result2;
                    if (such_that_type == Pair::USES) {
                        result2 =  executor.get_all_variables_uses_ref(stoi(item));
                    } else {
                        result2 =  executor.get_all_variables_modifies_ref(stoi(item));
                    }
                    for (auto item2 : result2) {
                        bool is_present = SuchThatPatternEval::is_element_inside_vectorA(item2, result);
                        if(!is_present) {
                            result.push_back(item2);
                        }
                    }
                }
                return result;
            } else if (!such_that_or_pattern.first && such_that_or_pattern.second) {
                // select value appears in pattern
                vector<var_ref> result;
                for(auto i : inner_join_lst) {
                    vector<var_ref> temp = executor.get_variable_pattern_assign(stoi(i), pattern_third_arg);
                    for(auto j : temp) {
                        bool is_present = SuchThatPatternEval::is_element_inside_vectorA(j, result);
                        if(!is_present) {
                            result.push_back(j);
                        }
                    }
                }
                return result;
            } else {
                // select value appears neither in such that or pattern
                return inner_join_lst;
            }
        }
    }        

}
        
vector<string> SuchThatPatternEval::more_than_one_common_synonym(PayLoad such_that_pay_load, PayLoad pattern_pay_load,string select_value, Single select_type) {
    //brute force
    string such_that_first_arg = such_that_pay_load.getValue()[0];
    string such_that_second_arg = such_that_pay_load.getValue()[1];
    vector<string> first_arg_lst = mapStorage[storeDeclaration[such_that_first_arg]][such_that_first_arg];
    vector<string> second_arg_lst = mapStorage[storeDeclaration[such_that_second_arg]][such_that_second_arg];
    vector<pair<string,string>> arg_lst = SuchThatPatternEval::crossproduct(first_arg_lst, second_arg_lst);
    Pair such_that_type = such_that_pay_load.getType().pair;

    unordered_set<string> result;
    unordered_set<string> result2;
    for(int i = 0; i < arg_lst.size(); i++) {
        string first_arg = arg_lst[i].first;
        string second_arg = arg_lst[i].second;
        if (such_that_type == Pair::FOLLOWS) {
            bool query = executor.is_follows(stoi(first_arg), stoi(second_arg), false);
                if (query) {
                    //if element is not added, add now
                    result.insert(first_arg);
                    result2.insert(second_arg);
                }
        } else if (such_that_type == Pair::FOLLOWST) {
            bool query = executor.is_follows(stoi(first_arg), stoi(second_arg), true);
                if (query) {
                    result.insert(first_arg);
                    result2.insert(second_arg);
                }
        } else if (such_that_type == Pair::PARENT) {
            bool query = executor.is_parent(stoi(first_arg), stoi(second_arg), false);
                if (query) {
                    result.insert(first_arg);
                    result2.insert(second_arg);
                }
                break;
        } else if (such_that_type == Pair::PARENTT) {
            bool query = executor.is_parent(stoi(first_arg), stoi(second_arg), true);
                if (query) {
                    result.insert(first_arg);
                    result2.insert(second_arg);
                }
        } else if (such_that_type == Pair::MODIFIES) {
            bool query = executor.statement_modifies(stoi(first_arg), second_arg);
                if (query) {
                    result.insert(first_arg);
                    result2.insert(second_arg);
                }
        } else if (such_that_type == Pair::USES) {
                    bool query = executor.statement_uses(stoi(first_arg), second_arg);
                if (query) {
                    result.insert(first_arg);
                    result2.insert(second_arg);
                }
        }
    }
    vector<string> v1(result.begin(), result.end());
    vector<string> v2(result2.begin(), result2.end());
    map<string,vector<string>> output; output[such_that_first_arg] = v1; output[such_that_second_arg] = v2;

    //pattern
    string pattern_first_arg = pattern_pay_load.getValue()[0];
    string pattern_second_arg = pattern_pay_load.getValue()[1];
    string pattern_third_arg = pattern_pay_load.getValue()[2]; //subexpression
    //retrieve lists from output
    vector<string> pattern_first_lst = output[pattern_first_arg]; //assign
    vector<string> pattern_second_lst = output[pattern_second_arg]; //variable
    vector<pair<string,string>> combined_pattern_lst = SuchThatPatternEval::crossproduct(pattern_first_lst, pattern_second_lst);
    unordered_set<string> result3;
    unordered_set<string> result4;
    for(int i = 0;i<combined_pattern_lst.size();i++) {
        string first_arg = combined_pattern_lst[i].first;
        string second_arg = combined_pattern_lst[i].second;
        pattern p; p.lvalue = second_arg; p.rvalue = pattern_third_arg;
        bool query = executor.satisfies_pattern(stoi(first_arg), p);
        if (query) {
            result3.insert(first_arg);
            result4.insert(second_arg);
        }
    }
    vector<string> v3(result3.begin(), result3.end());
    vector<string> v4(result4.begin(), result4.end());
    map<string,vector<string>> output2; output2[pattern_first_arg] = v3; output2[pattern_second_arg] = v4;
    return output2[select_value];
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

bool SuchThatPatternEval::is_element_inside_vectorA_int(int element, vector<int> vectorA) {
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

vector<string> SuchThatPatternEval::inner_join(vector<string> lstA, vector<string> lstB) {
    vector<string> output;
    for(int i = 0; i < lstA.size(); i++) {
        for(int j = 0; j < lstB.size(); j++) {
            if(lstA[i] == lstB[j]) {
                output.push_back(lstA[i]);
            }
        }
    }
    return output;
}

vector<pair<string,string>> SuchThatPatternEval::crossproduct(vector<string> first_arg_lst, vector<string> second_arg_lst) {
    vector<pair <string, string>> products;
    for(auto i : first_arg_lst) {
        for(auto j : second_arg_lst) {
            products.push_back(make_pair(i,j));
        }
    }
    return products;
}

vector<string> SuchThatPatternEval::convert_lst_string_to_int(vector<int> lstA) {
    vector<string> output;
    for(auto i : lstA) {
        output.push_back(to_string(i));
    }
    return output;
}

bool SuchThatPatternEval::is_pattern_variable_is_constant(std::string pattern_variable_value) {
    return(pattern_variable_value.at(0) == '"' && pattern_variable_value.at(pattern_variable_value.size()-1) == '"');
}