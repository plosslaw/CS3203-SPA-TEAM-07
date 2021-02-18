#include "SuchThatEval.h" 

vector<string> SuchThatEval::such_that_eval_1(vector<string> first_arg_lst, string second_arg, Pair such_that_type, ActionsExecutor executor) {
    unordered_set<string> result;
    for(int i = 0; i < first_arg_lst.size(); i++) {
        if (such_that_type == Pair::FOLLOWS) {
            bool query = executor.is_follows(stoi(first_arg_lst[i]), stoi(second_arg), false);
            if (query) {
                //if element is not added, add now
                result.insert(first_arg_lst[i]);
            }
        } else if (such_that_type == Pair::FOLLOWST) {
            bool query = executor.is_follows(stoi(first_arg_lst[i]), stoi(second_arg), true);
            if (query) {               
                result.insert(first_arg_lst[i]);
            }
        } else if (such_that_type == Pair::PARENT) {
            bool query = executor.is_parent(stoi(first_arg_lst[i]), stoi(second_arg), false);
            if (query) {
                result.insert(first_arg_lst[i]);
            }
        } else if (such_that_type == Pair::PARENTT) {
            bool query = executor.is_parent(stoi(first_arg_lst[i]), stoi(second_arg), true);
            if (query) {
                result.insert(first_arg_lst[i]);
            }
        } else if (such_that_type == Pair::MODIFIES) {
            bool query = executor.statement_modifies(stoi(first_arg_lst[i]), second_arg);
            if (query) {
                result.insert(first_arg_lst[i]);
            }
        } else if (such_that_type == Pair::USES) {
            bool query = executor.statement_uses(stoi(first_arg_lst[i]), second_arg);
            if (query) {
                result.insert(first_arg_lst[i]);
            }
        }
    }
    vector<string> output(result.begin(), result.end());
    return output;
}

vector<string> SuchThatEval::such_that_eval_2(string first_arg, vector<string> second_arg_lst, Pair such_that_type, ActionsExecutor executor) {
    unordered_set<string> result;
    for(int i = 0; i < second_arg_lst.size(); i++) {
        if (such_that_type == Pair::FOLLOWS) {
            bool query = executor.is_follows(stoi(first_arg), stoi(second_arg_lst[i]), false);
                if (query) {
                    //if element is not added, add now
                    result.insert(second_arg_lst[i]);
                }
        } else if (such_that_type == Pair::FOLLOWST) {
            bool query = executor.is_follows(stoi(first_arg), stoi(second_arg_lst[i]), true);
                if (query) {
                    result.insert(second_arg_lst[i]);
                }
    } else if (such_that_type == Pair::PARENT) {
            bool query = executor.is_parent(stoi(first_arg), stoi(second_arg_lst[i]), false);
                if (query) {
                    result.insert(second_arg_lst[i]);
                }
                break;
    } else if (such_that_type == Pair::PARENTT) {
           bool query = executor.is_parent(stoi(first_arg), stoi(second_arg_lst[i]), true);
                if (query) {
                    result.insert(second_arg_lst[i]);
                }
    } else if (such_that_type == Pair::MODIFIES) {
            //kiv modifies(STRING::procedure, var_name) create one modifies executor for procedure parameter.
            bool query = executor.statement_modifies(stoi(first_arg), second_arg_lst[i]);
            if (query) {
                    result.insert(second_arg_lst[i]);
                }
    } else if (such_that_type == Pair::USES) {
             //kiv Uses(STRING::procedure, var_name) create one modifies executor for procedure parameter.        
                bool query = executor.statement_uses(stoi(first_arg), second_arg_lst[i]);
                if (query) {
                    result.insert(second_arg_lst[i]);
        }
                 
        }
    }
    vector<string> output(result.begin(), result.end());
    return output;
}

map<string,vector<string>> SuchThatEval::such_that_eval_3(vector<pair<string,string>> arg_lst, Pair such_that_type, string first_arg_name, string second_arg_name, ActionsExecutor executor) {
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
    map<string,vector<string>> output; output[first_arg_name] = v1; output[second_arg_name] = v2;
    return output;
}      
// vector<string> SuchThatEval::such_that_eval_4(Pair such_that_type, ActionsExecutor executor) {

// }
