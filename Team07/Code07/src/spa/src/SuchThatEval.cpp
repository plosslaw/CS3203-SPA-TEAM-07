#include "SuchThatEval.h"
#include "StringUtil.h"
#include "ActionsExecutor.h"
#include "ActionsGenerator.h"

//constructor
SuchThatEval::SuchThatEval(unordered_map<string, Single> declaration_store, unordered_map<Single, 
            unordered_map<string, vector<string>>> map_storage, ActionsExecutor executor_) {
                storeDeclaration = declaration_store;
                mapStorage = map_storage;
                executor = executor_;    
}

//methods
vector<string> SuchThatEval::one_such_that_zero_pattern(PayLoad such_that_pay_load, string select_value, Single select_type, pair<bool,bool> arg_pairs) {
    string such_that_first_arg = such_that_pay_load.getValue()[0];
    string such_that_second_arg = such_that_pay_load.getValue()[1];
    pair<bool, bool> bool_pairs_args = SuchThatEval::check_if_args_are_variable(such_that_first_arg, such_that_second_arg); //which of the two param is/are variables?
    Pair such_that_type = such_that_pay_load.getType().pair;
    //stmt_type select_stmt_type = SuchThatEval::convert_single_to_stmt_type(select_type);

    if (such_that_type == Pair::FOLLOWS || such_that_type == Pair::FOLLOWST) {
        bool IS_FOLLOWS = such_that_type == Pair::FOLLOWS;
        if(bool_pairs_args.first && bool_pairs_args.second) {
            // both are variables. both are types of statements (statement, read, while etc...)
            Single first_arg_type  = storeDeclaration[such_that_first_arg];
            Single second_arg_type  = storeDeclaration[such_that_second_arg];
            if (first_arg_type == Single::STATEMENT && second_arg_type == Single::STATEMENT) {
                // both are statements
                if (such_that_first_arg != such_that_second_arg) {
                    if(arg_pairs.first || arg_pairs.second) {
                        vector<stmt_ref> result;
                        if (arg_pairs.first) {
                            result = executor.get_all_stmts_follows(stmt_type::STATEMENT, arg_pos::FIRST_ARG, IS_FOLLOWS);   
                        } else {
                            result = executor.get_all_stmts_follows(stmt_type::STATEMENT, arg_pos::SECOND_ARG, IS_FOLLOWS);                        
                        }
                        vector<string> result_converted;
                        for(auto item : result) {
                            result_converted.push_back(to_string(item));
                        }
                        return result_converted;
                    } else {
                        vector<stmt_ref> result = executor.get_all_stmts_follows(stmt_type::STATEMENT, arg_pos::FIRST_ARG, IS_FOLLOWS);   
                        return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                    } 
                } else {
                    // Follows(s1,s1) for example
                    return vector<string>{"None"};
                }                
            } else {
                //either both not statements or one of them is statement only (both are variables) e.g. Follows(w,s1), Follows(w,re)
                stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                stmt_type second_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(second_arg_type);
                if(arg_pairs.first || arg_pairs.second) {
                    vector<stmt_ref> result;
                    if (arg_pairs.first) {
                        result = executor.get_all_stmts_follows(first_arg_stmt_type, arg_pos::FIRST_ARG, second_arg_stmt_type, IS_FOLLOWS);
                    } else {
                        result = executor.get_all_stmts_follows(first_arg_stmt_type, arg_pos::SECOND_ARG, second_arg_stmt_type, IS_FOLLOWS);
                    }
                    vector<string> result_converted;
                    for (auto item : result) {
                        result_converted.push_back(to_string(item));
                    }
                    return result_converted;
                } else {
                    vector<stmt_ref> result = executor.get_all_stmts_follows(first_arg_stmt_type, arg_pos::FIRST_ARG, second_arg_stmt_type, IS_FOLLOWS);
                    return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                }
            }    
        } else if (!bool_pairs_args.first && !bool_pairs_args.second) {
            //both are constant
            bool query = executor.is_follows(stoi(such_that_first_arg), stoi(such_that_second_arg), IS_FOLLOWS);
            if (query) {
                vector<string> default_solution = (mapStorage[select_type])[select_value];
                return default_solution;
            } else {
                return vector<string>{"None"};
            }
        } else {
            //one of them is constant
            if (bool_pairs_args.first && !bool_pairs_args.second) {
                //first parameter is variable and second parameter is a constant.
                Single first_arg_type  = storeDeclaration[such_that_first_arg];
                stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                vector<stmt_ref> result = executor.get_all_stmts_follows(first_arg_stmt_type, arg_pos::FIRST_ARG, stoi(such_that_second_arg), IS_FOLLOWS);
                if (arg_pairs.first) {
                    vector<string> result_converted;
                    for(auto item : result) {
                        result_converted.push_back(to_string(item));
                    }
                    return result_converted;
                } else {
                    return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                }          
            } else {
                //first parameter is a constnat and second parameter is a variable.
                Single second_arg_type  = storeDeclaration[such_that_second_arg];
                stmt_type second_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(second_arg_type);
                vector<stmt_ref> result = executor.get_all_stmts_follows(second_arg_stmt_type, arg_pos::SECOND_ARG, stoi(such_that_first_arg), IS_FOLLOWS);  
                //kiv
                if (arg_pairs.second) {
                    vector<string> result_converted;
                    for(auto item : result) {
                        result_converted.push_back(to_string(item));
                    }
                    return result_converted;
                } else {
                    return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                }
            }
        }
    } else if (such_that_type == Pair::PARENT || such_that_type == Pair::PARENTT) {
        bool IS_PARENT = such_that_type == Pair::PARENT;
        if(bool_pairs_args.first && bool_pairs_args.second) {
            // both are variables. both are types of statements (statement, read, while etc...)
            Single first_arg_type  = storeDeclaration[such_that_first_arg];
            Single second_arg_type  = storeDeclaration[such_that_second_arg];
            if (first_arg_type == Single::STATEMENT && second_arg_type == Single::STATEMENT) {
                // both are statements
                if (such_that_first_arg != such_that_second_arg) {
                    if(arg_pairs.first || arg_pairs.second) {
                        vector<stmt_ref> result;
                        if (arg_pairs.first) {
                            result = executor.get_all_stmts_parent(stmt_type::STATEMENT, arg_pos::FIRST_ARG, IS_PARENT);   
                        } else {
                            result = executor.get_all_stmts_parent(stmt_type::STATEMENT, arg_pos::SECOND_ARG, IS_PARENT);                        
                        }
                        vector<string> result_converted;
                        for(auto item : result) {
                            result_converted.push_back(to_string(item));
                        }
                        return result_converted;
                    } else {
                        vector<stmt_ref> result = executor.get_all_stmts_parent(stmt_type::STATEMENT, arg_pos::FIRST_ARG, IS_PARENT);   
                        return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                    } 
                } else {
                    // parent(s1,s1) for example
                    return vector<string>{"None"};
                }                
            } else {
                //either both not statements or one of them is statement only (both are variables) e.g. Parent(w,s1), Parent(w,re)
                stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                stmt_type second_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(second_arg_type);
                if(arg_pairs.first || arg_pairs.second) {
                    vector<stmt_ref> result;
                    if (arg_pairs.first) {
                        result = executor.get_all_stmts_parent(first_arg_stmt_type, arg_pos::FIRST_ARG, second_arg_stmt_type, IS_PARENT);
                    } else {
                        result = executor.get_all_stmts_parent(first_arg_stmt_type, arg_pos::SECOND_ARG, second_arg_stmt_type, IS_PARENT);
                    }
                    vector<string> result_converted;
                    for (auto item : result) {
                        result_converted.push_back(to_string(item));
                    }
                    return result_converted;
                } else {
                    vector<stmt_ref> result = executor.get_all_stmts_parent(first_arg_stmt_type, arg_pos::FIRST_ARG, second_arg_stmt_type, IS_PARENT);
                    return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                }
            }    
        } else if (!bool_pairs_args.first && !bool_pairs_args.second) {
            //both are constant
            bool query = executor.is_parent(stoi(such_that_first_arg), stoi(such_that_second_arg), IS_PARENT);
            if (query) {
                vector<string> default_solution = (mapStorage[select_type])[select_value];
                return default_solution;
            } else {
                return vector<string>{"None"};
            }
        } else {
            //one of them is constant
            if (bool_pairs_args.first && !bool_pairs_args.second) {
                //first parameter is variable and second parameter is a constant.
                Single first_arg_type  = storeDeclaration[such_that_first_arg];
                stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                vector<stmt_ref> result = executor.get_all_stmts_parent(first_arg_stmt_type, arg_pos::FIRST_ARG, stoi(such_that_second_arg), IS_PARENT);
                if (arg_pairs.first) {
                    vector<string> result_converted;
                    for(auto item : result) {
                        result_converted.push_back(to_string(item));
                    }
                    return result_converted;
                } else {
                    return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                }          
            } else {
                //first parameter is a constnat and second parameter is a variable.
                Single second_arg_type  = storeDeclaration[such_that_second_arg];
                stmt_type second_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(second_arg_type);
                vector<stmt_ref> result = executor.get_all_stmts_parent(second_arg_stmt_type, arg_pos::SECOND_ARG, stoi(such_that_first_arg), IS_PARENT);  
                if (arg_pairs.second) {
                    vector<string> result_converted;
                    for(auto item : result) {
                        result_converted.push_back(to_string(item));
                    }
                    return result_converted;
                } else {
                    return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                }
            }
        }
    } else if (such_that_type == Pair::MODIFIES) {
        if(bool_pairs_args.first && bool_pairs_args.second) {
            // both are variables
            Single first_arg_type  = storeDeclaration[such_that_first_arg];
            Single second_arg_type  = storeDeclaration[such_that_second_arg];

            if (first_arg_type == Single::PROCEDURE) {
                vector<string> proc_lst = mapStorage[Single::PROCEDURE][such_that_first_arg];
                if (select_type == Single::PROCEDURE) {
                    vector<proc_ref> result = executor.get_all_procedures_modifies();
                    if(arg_pairs.first) {
                        return result;
                    } else {
                        return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                    }           
                } else if (select_type == Single::VARIABLE) {
                    vector<string> result;
                    for(auto item : proc_lst) {
                        vector<var_ref> result2 = executor.get_all_variables_modifies(item);
                        for(auto item2 : result2) {
                            if (!SuchThatEval::is_element_inside_vectorA(item2, result)) {
                                result.push_back(item2);
                            }
                        }
                    }
                    if(arg_pairs.second) {
                        return result;
                    } else {
                        return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                    }    
                } else {
                    //Select type is statements or constants. statements could be statement, read, assign etc
                    //by default if select type is constant then it will check if query is empty or not...
                    vector<string> result;
                    vector<stmt_ref> result2 = executor.get_all_stmts_modifies(stmt_type::PROCEDURE);
                    for (auto item : result2) {
                        result.push_back(to_string(item));
                    }
                    if(arg_pairs.first) {
                        return result;
                    } else {
                        return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                    } 
                }    
            } else {
                // first_arg is statement. could be statement, read, while, if
                stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                if (select_type == Single::VARIABLE) {
                    vector<var_ref> result = executor.get_all_variables_modifies(first_arg_stmt_type);
                    if (arg_pairs.second) {
                        return result;
                    } else {
                        return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                    }
                } else {
                    // select type is other than variable.
                    vector<string> result;
                    stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                    vector<stmt_ref> result2 = executor.get_all_stmts_modifies(first_arg_stmt_type);
                    for (auto item : result2) {
                        result.push_back(to_string(item));
                    }
                    if(arg_pairs.first) {
                        return result;
                    } else {
                        return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                    } 
                }
            }
        } else if (!bool_pairs_args.first && !bool_pairs_args.second) {
            //both are constants
            bool query = executor.statement_modifies(stoi(such_that_first_arg), such_that_second_arg);
            if (query) {
                vector<string> default_solution = (mapStorage[select_type])[select_value];
                return default_solution;
            } else {
                return vector<string>{"None"};
            }
        } else {
            //one of them is a constant
            if (bool_pairs_args.first && !bool_pairs_args.second) {
                // first parameter is a variable and second parameter is a constant.
                Single first_arg_type  = storeDeclaration[such_that_first_arg];

                if(first_arg_type == Single::PROCEDURE) {
                    vector<proc_ref> result = executor.get_all_procedures_modifies(such_that_second_arg);
                    if (arg_pairs.first) {
                        return result;
                    } else {
                        return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                    }
                } else {
                    stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                    vector<string> result;
                    vector<stmt_ref> result2 = executor.get_all_stmts_modifies(first_arg_stmt_type, such_that_second_arg);
                    for(auto item: result2) {
                        result.push_back(to_string(item));
                    }
                    if (arg_pairs.first) {
                        return result;
                    }
                    return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                }      
            } else {
                // first parameter is a constant and second parameter is a variable.
                // second parameter has to be variable type
                if (StringUtil::is_number(such_that_first_arg)) {
                    //statement first arg type
                    vector<var_ref> result = executor.get_all_variables_modifies(stoi(such_that_first_arg));
                    if(arg_pairs.second) {
                        return result;
                    } else {
                        return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                    }
                } else {
                    //procedure first arg type
                    vector<var_ref> result = executor.get_all_variables_modifies(such_that_first_arg);
                    if(arg_pairs.second) {
                        return result;
                    } else {
                        return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                    }
                }
            }
        }
    } else if (such_that_type == Pair::USES) {
        if(bool_pairs_args.first && bool_pairs_args.second) {
            // both are variables
            Single first_arg_type  = storeDeclaration[such_that_first_arg];
            Single second_arg_type  = storeDeclaration[such_that_second_arg];

            if (first_arg_type == Single::PROCEDURE) {
                vector<string> proc_lst = mapStorage[Single::PROCEDURE][such_that_first_arg];
                if (select_type == Single::PROCEDURE) {
                    vector<proc_ref> result = executor.get_all_procedures_uses();
                    if(arg_pairs.first) {
                        return result;
                    } else {
                        return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                    }           
                } else if (select_type == Single::VARIABLE) {
                    vector<string> result;
                    for(auto item : proc_lst) {
                        vector<var_ref> result2 = executor.get_all_variables_uses(item);
                        for(auto item2 : result2) {
                            if (!SuchThatEval::is_element_inside_vectorA(item2, result)) {
                                result.push_back(item2);
                            }
                        }
                    }
                    if(arg_pairs.second) {
                        return result;
                    } else {
                        return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                    }    
                } else {
                    //Select type is statements or constants. statements could be statement, read, assign etc
                    //by default if select type is constant then it will check if query is empty or not...
                    vector<string> result;
                    vector<stmt_ref> result2 = executor.get_all_stmts_uses(stmt_type::PROCEDURE);
                    for (auto item : result2) {
                        result.push_back(to_string(item));
                    }
                    if(arg_pairs.first) {
                        return result;
                    } else {
                        return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                    } 
                }    
            } else {
                // first_arg is statement. could be statement, read, while, if
                stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                if (select_type == Single::VARIABLE) {
                    vector<var_ref> result = executor.get_all_variables_uses(first_arg_stmt_type);
                    if (arg_pairs.second) {
                        return result;
                    } else {
                        return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                    }
                } else {
                    // select type is other than variable.
                    vector<string> result;
                    stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                    vector<stmt_ref> result2 = executor.get_all_stmts_uses(first_arg_stmt_type);
                    for (auto item : result2) {
                        result.push_back(to_string(item));
                    }
                    if(arg_pairs.first) {
                        return result;
                    } else {
                        return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                    } 
                }
            }
        } else if (!bool_pairs_args.first && !bool_pairs_args.second) {
            //both are constants
            bool query = executor.statement_uses(stoi(such_that_first_arg), such_that_second_arg);
            if (query) {
                vector<string> default_solution = (mapStorage[select_type])[select_value];
                return default_solution;
            } else {
                return vector<string>{"None"};
            }
        } else {
            //one of them is a constant
            if (bool_pairs_args.first && !bool_pairs_args.second) {
                // first parameter is a variable and second parameter is a constant.
                Single first_arg_type  = storeDeclaration[such_that_first_arg];

                if(first_arg_type == Single::PROCEDURE) {
                    vector<proc_ref> result = executor.get_all_procedures_uses(such_that_second_arg);
                    if (arg_pairs.first) {
                        return result;
                    } else {
                        return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                    }
                } else {
                    stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                    vector<string> result;
                    vector<stmt_ref> result2 = executor.get_all_stmts_uses(first_arg_stmt_type, such_that_second_arg);
                    for(auto item: result2) {
                        result.push_back(to_string(item));
                    }
                    if (arg_pairs.first) {
                        return result;
                    }
                    return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                }      
            } else {
                // first parameter is a constant and second parameter is a variable.
                // second parameter has to be variable type
                if (StringUtil::is_number(such_that_first_arg)) {
                    //statement first arg type
                    vector<var_ref> result = executor.get_all_variables_uses(stoi(such_that_first_arg));
                    if(arg_pairs.second) {
                        return result;
                    } else {
                        return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                    }
                } else {
                    //procedure first arg type
                    vector<var_ref> result = executor.get_all_variables_uses(such_that_first_arg);
                    if(arg_pairs.second) {
                        return result;
                    } else {
                        return is_result_empty_yes_none_no_default_soln(result.empty(), select_type, select_value);
                    }
                }
            }
        }     
    } else {
        return vector<string> {"None"};
    }
}       


// utilities
pair<bool,bool> SuchThatEval::check_if_args_are_variable(std::string first_arg, std::string second_arg) {
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

bool SuchThatEval::is_element_inside_vectorA(string element, vector<string> vectorA) {
    return std::find(vectorA.begin(), vectorA.end(), element) != vectorA.end();
}

stmt_type SuchThatEval::convert_single_to_stmt_type(Single s) {
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

vector<string> SuchThatEval::is_result_empty_yes_none_no_default_soln(bool is_result_empty, Single select_type, string select_value) {
    if (is_result_empty) {
        return vector<string>{"None"};              
    } else {
        vector<string> default_solution = (mapStorage[select_type])[select_value];
        return default_solution;                      
    }
}
