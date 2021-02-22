#include "SuchThatEval.h"
#include "StringUtil.h"
#include "ActionsExecutor.h"
#include "ActionsGenerator.h"
#include "algorithm"
#include <iostream>
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

    if (such_that_first_arg == such_that_second_arg && such_that_first_arg != "_") {
        return vector<string>();
    }

    pair<bool, bool> bool_pairs_args = SuchThatEval::check_if_args_are_variable(such_that_first_arg, such_that_second_arg); //which of the two param is/are variables?
    if(SuchThatEval::is_pattern_variable_is_constant(such_that_first_arg)) {
            such_that_first_arg.erase(such_that_first_arg.begin());
            such_that_first_arg.pop_back();
            bool_pairs_args.first =false;
    }
    if(SuchThatEval::is_pattern_variable_is_constant(such_that_second_arg)) {
            such_that_second_arg.erase(such_that_second_arg.begin());
            such_that_second_arg.pop_back();
            bool_pairs_args.second =false;
    }
    Pair such_that_type = such_that_pay_load.getType().pair;
    //stmt_type select_stmt_type = SuchThatEval::convert_single_to_stmt_type(select_type);

    if (such_that_type == Pair::FOLLOWS || such_that_type == Pair::FOLLOWST) {
        bool IS_FOLLOWST = such_that_type == Pair::FOLLOWST;
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
                            result = executor.get_all_stmts_follows(stmt_type::STATEMENT, arg_pos::FIRST_ARG, IS_FOLLOWST);   
                        } else {
                            result = executor.get_all_stmts_follows(stmt_type::STATEMENT, arg_pos::SECOND_ARG, IS_FOLLOWST);                        
                        }
                        return SuchThatEval::convert_lst_string_to_int(result);
                    } else {
                        vector<stmt_ref> result = executor.get_all_stmts_follows(stmt_type::STATEMENT, arg_pos::FIRST_ARG, IS_FOLLOWST);   
                        return SuchThatEval::convert_lst_string_to_int(result);
                    } 
                } else {
                    // Follows(s1,s1) for example
                    return vector<string>{};
                }                
            } else {
                //either both not statements or one of them is statement only (both are variables) e.g. Follows(w,s1), Follows(w,re)
                stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                stmt_type second_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(second_arg_type);
                if(arg_pairs.first || arg_pairs.second) {
                    vector<stmt_ref> result;
                    if (arg_pairs.first) {
                        result = executor.get_all_stmts_follows_type(first_arg_stmt_type, arg_pos::FIRST_ARG, second_arg_stmt_type, IS_FOLLOWST);
                    } else {
                        result = executor.get_all_stmts_follows_type(second_arg_stmt_type, arg_pos::SECOND_ARG, first_arg_stmt_type, IS_FOLLOWST);
                    }
                    return SuchThatEval::convert_lst_string_to_int(result);
                } else {
                    vector<stmt_ref> result = executor.get_all_stmts_follows_type(first_arg_stmt_type, arg_pos::FIRST_ARG, second_arg_stmt_type, IS_FOLLOWST);
                    return SuchThatEval::convert_lst_string_to_int(result);
                }
            }    
        } else if (!bool_pairs_args.first && !bool_pairs_args.second) {
            //both are constant
            bool is_first_arg_wild = such_that_first_arg == "_";
            bool is_second_arg_wild =  such_that_second_arg == "_";
            if (is_first_arg_wild && is_second_arg_wild) {
                //both wild
                vector<stmt_ref> result1 = executor.get_all_stmts_follows(stmt_type::STATEMENT,arg_pos::FIRST_ARG, IS_FOLLOWST);
                vector<stmt_ref> result2 = executor.get_all_stmts_follows(stmt_type::STATEMENT,arg_pos::SECOND_ARG, IS_FOLLOWST);
                vector<string> result1_converted = SuchThatEval::convert_lst_string_to_int(result1);
                vector<string> result2_converted = SuchThatEval::convert_lst_string_to_int(result2);
                for(auto i : result2_converted) {
                    if(!SuchThatEval::is_element_inside_vectorA(i,result1_converted)) {
                        result1_converted.push_back(i);
                    }
                }
                return result1_converted;
            } else if(is_first_arg_wild && !is_second_arg_wild) {
                //second arg is wild only
                vector<string> result;
                vector<stmt_ref> stmt_lst = executor.get_all_statements();
                for(auto i : stmt_lst) {
                    bool query = executor.is_follows(i,stoi(such_that_second_arg),IS_FOLLOWST);
                    if(query) {
                        if(!SuchThatEval::is_element_inside_vectorA(to_string(i),result)) {
                            result.push_back(to_string(i));
                        }
                    }
                }
                return result;
            } else if(!is_first_arg_wild && is_second_arg_wild) {
                //second arg is wild only
                vector<string> result;
                vector<stmt_ref> stmt_lst = executor.get_all_statements();
                for(auto i : stmt_lst) {
                    bool query = executor.is_follows(stoi(such_that_first_arg), i, IS_FOLLOWST);
                    if(query) {
                        if(!SuchThatEval::is_element_inside_vectorA(to_string(i),result)) {
                            result.push_back(to_string(i));
                        }
                    }
                }
                return result;
            } else {
                bool query = executor.is_follows(stoi(such_that_first_arg), stoi(such_that_second_arg), IS_FOLLOWST);
                if (query) {
                    vector<string> default_solution = (mapStorage[select_type])[select_value];
                    return default_solution;
                } else {
                    return vector<string>{};
                }
            }
        } else {
            //one of them is constant
            bool is_second_arg_wild =  such_that_second_arg == "_";
            if (bool_pairs_args.first && !bool_pairs_args.second) {
                //first parameter is variable and second parameter is a constant.
                if(is_second_arg_wild) {
                    Single first_arg_type  = storeDeclaration[such_that_first_arg];
                    stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                    vector<stmt_ref> result = executor.get_all_stmts_follows(first_arg_stmt_type, arg_pos::FIRST_ARG, IS_FOLLOWST);
                    return SuchThatEval::convert_lst_string_to_int(result); 
                } 
                else {
                    Single first_arg_type  = storeDeclaration[such_that_first_arg];
                    stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                    vector<stmt_ref> result = executor.get_all_stmts_follows_ref(first_arg_stmt_type, arg_pos::FIRST_ARG, stoi(such_that_second_arg), IS_FOLLOWST);
                    return SuchThatEval::convert_lst_string_to_int(result); 
                }         
            } else {
                //first parameter is a constnat and second parameter is a variable.
                bool is_first_arg_wild =  such_that_first_arg == "_";
                if(is_first_arg_wild) {
                    Single second_arg_type  = storeDeclaration[such_that_second_arg];
                    stmt_type second_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(second_arg_type);
                    vector<stmt_ref> result = executor.get_all_stmts_follows(second_arg_stmt_type, arg_pos::SECOND_ARG, IS_FOLLOWST);          
                    return SuchThatEval::convert_lst_string_to_int(result);
                } else {
                    Single second_arg_type  = storeDeclaration[such_that_second_arg];
                    stmt_type second_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(second_arg_type);
                    vector<stmt_ref> result = executor.get_all_stmts_follows_ref(second_arg_stmt_type, arg_pos::SECOND_ARG, stoi(such_that_first_arg), IS_FOLLOWST);          
                    return SuchThatEval::convert_lst_string_to_int(result);
                }
            }
        }
    } else if (such_that_type == Pair::PARENT || such_that_type == Pair::PARENTT) {
        bool IS_PARENTT = such_that_type == Pair::PARENTT;
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
                            result = executor.get_all_stmts_parent(stmt_type::STATEMENT, arg_pos::FIRST_ARG, IS_PARENTT);   
                        } else {
                            result = executor.get_all_stmts_parent(stmt_type::STATEMENT, arg_pos::SECOND_ARG, IS_PARENTT);                        
                        }
                        return SuchThatEval::convert_lst_string_to_int(result);
                    } else {
                        vector<stmt_ref> result = executor.get_all_stmts_parent(stmt_type::STATEMENT, arg_pos::FIRST_ARG, IS_PARENTT);   
                        return SuchThatEval::convert_lst_string_to_int(result);
                    } 
                } else {
                    // Parent(s1,s1) for example
                    return vector<string>{};
                }                
            } else {
                //either both not statements or one of them is statement only (both are variables) e.g. Parent(w,s1), Parent(w,re)
                stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                stmt_type second_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(second_arg_type);
                if(arg_pairs.first || arg_pairs.second) {
                    vector<stmt_ref> result;
                    if (arg_pairs.first) {
                        result = executor.get_all_stmts_parent_type(first_arg_stmt_type, arg_pos::FIRST_ARG, second_arg_stmt_type, IS_PARENTT);
                    } else {
                        result = executor.get_all_stmts_parent_type(second_arg_stmt_type, arg_pos::SECOND_ARG, first_arg_stmt_type, IS_PARENTT);
                    }
                    return SuchThatEval::convert_lst_string_to_int(result);
                } else {
                    vector<stmt_ref> result = executor.get_all_stmts_parent_type(first_arg_stmt_type, arg_pos::FIRST_ARG, second_arg_stmt_type, IS_PARENTT);
                    return SuchThatEval::convert_lst_string_to_int(result);
                }
            }    
        } else if (!bool_pairs_args.first && !bool_pairs_args.second) {
            //both are constant
            bool is_first_arg_wild = such_that_first_arg == "_";
            bool is_second_arg_wild =  such_that_second_arg == "_";
            if (is_first_arg_wild && is_second_arg_wild) {
                //both wild
                vector<stmt_ref> result1 = executor.get_all_stmts_parent(stmt_type::STATEMENT,arg_pos::FIRST_ARG, IS_PARENTT);
                vector<stmt_ref> result2 = executor.get_all_stmts_parent(stmt_type::STATEMENT,arg_pos::SECOND_ARG, IS_PARENTT);
                vector<string> result1_converted = SuchThatEval::convert_lst_string_to_int(result1);
                vector<string> result2_converted = SuchThatEval::convert_lst_string_to_int(result2);
                for(auto i : result2_converted) {
                    if(!SuchThatEval::is_element_inside_vectorA(i,result1_converted)) {
                        result1_converted.push_back(i);
                    }
                }
                return result1_converted;
            } else if(is_first_arg_wild && !is_second_arg_wild) {
                //second arg is wild only
                vector<string> result;
                vector<stmt_ref> stmt_lst = executor.get_all_statements();
                for(auto i : stmt_lst) {
                    bool query = executor.is_parent(i,stoi(such_that_second_arg),IS_PARENTT);
                    if(query) {
                        if(!SuchThatEval::is_element_inside_vectorA(to_string(i),result)) {
                            result.push_back(to_string(i));
                        }
                    }
                }
                return result;
            } else if(!is_first_arg_wild && is_second_arg_wild) {
                //second arg is wild only
                vector<string> result;
                vector<stmt_ref> stmt_lst = executor.get_all_statements();
                for(auto i : stmt_lst) {
                    bool query = executor.is_parent(stoi(such_that_first_arg), i, IS_PARENTT);
                    if(query) {
                        if(!SuchThatEval::is_element_inside_vectorA(to_string(i),result)) {
                            result.push_back(to_string(i));
                        }
                    }
                }
                return result;
            } else {
                bool query = executor.is_parent(stoi(such_that_first_arg), stoi(such_that_second_arg), IS_PARENTT);
                if (query) {
                    vector<string> default_solution = (mapStorage[select_type])[select_value];
                    return default_solution;
                } else {
                    return vector<string>{};
                }
            }
        } else {
            //one of them is constant
            bool is_second_arg_wild =  such_that_second_arg == "_";
            if (bool_pairs_args.first && !bool_pairs_args.second) {
                //first parameter is variable and second parameter is a constant.
                if(is_second_arg_wild) {
                    Single first_arg_type  = storeDeclaration[such_that_first_arg];
                    stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                    vector<stmt_ref> result = executor.get_all_stmts_parent(first_arg_stmt_type, arg_pos::FIRST_ARG, IS_PARENTT);
                    return SuchThatEval::convert_lst_string_to_int(result); 
                } 
                else {
                    Single first_arg_type  = storeDeclaration[such_that_first_arg];
                    stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                    vector<stmt_ref> result = executor.get_all_stmts_parent_ref(first_arg_stmt_type, arg_pos::FIRST_ARG, stoi(such_that_second_arg), IS_PARENTT);
                    return SuchThatEval::convert_lst_string_to_int(result); 
                }         
            } else {
                //first parameter is a constnat and second parameter is a variable.
                bool is_first_arg_wild =  such_that_first_arg == "_";
                if(is_first_arg_wild) {
                    Single second_arg_type  = storeDeclaration[such_that_second_arg];
                    stmt_type second_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(second_arg_type);
                    vector<stmt_ref> result = executor.get_all_stmts_parent(second_arg_stmt_type, arg_pos::SECOND_ARG, IS_PARENTT);          
                    return SuchThatEval::convert_lst_string_to_int(result);
                } else {
                    Single second_arg_type  = storeDeclaration[such_that_second_arg];
                    stmt_type second_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(second_arg_type);
                    vector<stmt_ref> result = executor.get_all_stmts_parent_ref(second_arg_stmt_type, arg_pos::SECOND_ARG, stoi(such_that_first_arg), IS_PARENTT);          
                    return SuchThatEval::convert_lst_string_to_int(result);
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
                    return result;         
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
                    return result;   
                } else {
                    //Select type is statements or constants. statements could be statement, read, assign etc
                    //by default if select type is constant then it will check if query is empty or not...
                    vector<stmt_ref> result = executor.get_all_stmts_modifies(stmt_type::PROCEDURE);
                    return SuchThatEval::convert_lst_string_to_int(result);
                }    
            } else {
                // first_arg is statement. could be statement, read, while, if
                stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                if (select_type == Single::VARIABLE) {
                    vector<var_ref> result = executor.get_all_variables_modifies_type(first_arg_stmt_type);
                    return result;
                } else {
                    // select type is other than variable.
                    stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                    vector<stmt_ref> result = executor.get_all_stmts_modifies(first_arg_stmt_type);
                    return SuchThatEval::convert_lst_string_to_int(result);
                }
            }
        } else if (!bool_pairs_args.first && !bool_pairs_args.second) {
            //both are constants
            if (such_that_second_arg == "_") {
                if(StringUtil::is_number(such_that_first_arg)) {
                    //this means first arg is a statement
                    vector<var_ref> result = executor.get_all_variables_modifies_ref(stoi(such_that_first_arg));
                    return result;
                } else {
                    vector<var_ref> result = executor.get_all_variables_modifies(such_that_first_arg);
                    return result;                    
                }
            } 
            else {
                bool query = executor.statement_modifies(stoi(such_that_first_arg), such_that_second_arg);
                if (query) {
                    vector<string> default_solution = (mapStorage[select_type])[select_value];
                    return default_solution;
                } else {
                    return vector<string>{};
                }
            }
        } else {
            //one of them is a constant
            if (bool_pairs_args.first && !bool_pairs_args.second) {
                // first parameter is a variable and second parameter is a constant.
                Single first_arg_type  = storeDeclaration[such_that_first_arg];

                if(first_arg_type == Single::PROCEDURE) {
                    if(such_that_second_arg == "_") {
                        vector<proc_ref> result = executor.get_all_procedures_modifies();
                        return result;
                    } else {
                        vector<proc_ref> result = executor.get_all_procedures_modifies(such_that_second_arg);
                        return result;
                    }
                } else {
                    //first arg is statements not procedure.
                    stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                    if(such_that_second_arg == "_") {
                        vector<stmt_ref> result = executor.get_all_stmts_modifies(first_arg_stmt_type);
                        return SuchThatEval::convert_lst_string_to_int(result);
                    } else {
                        vector<stmt_ref> result = executor.get_all_stmts_modifies(first_arg_stmt_type, such_that_second_arg);
                        return SuchThatEval::convert_lst_string_to_int(result);
                    }
                }      
            } else {
                // first parameter is a constant and second parameter is a variable.
                // second parameter has to be variable type
                if(such_that_first_arg == "_") {
                    vector<var_ref> result = executor.get_all_variables_modifies(such_that_first_arg);
                    return result;
                }
                else if (StringUtil::is_number(such_that_first_arg)) {
                    //statement first arg type
                    vector<var_ref> result = executor.get_all_variables_modifies_ref(stoi(such_that_first_arg));
                    return result;
                } else {
                    //procedure first arg type
                    vector<var_ref> result = executor.get_all_variables_modifies(such_that_first_arg);
                    return result;
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
                    return result;         
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
                    return result;   
                } else {
                    //Select type is statements or constants. statements could be statement, read, assign etc
                    //by default if select type is constant then it will check if query is empty or not...
                    vector<stmt_ref> result = executor.get_all_stmts_uses(stmt_type::PROCEDURE);
                    return SuchThatEval::convert_lst_string_to_int(result);
                }    
            } else {
                // first_arg is statement. could be statement, read, while, if
                stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                if (select_type == Single::VARIABLE) {
                    vector<var_ref> result = executor.get_all_variables_uses_type(first_arg_stmt_type);
                    return result;
                } else {
                    // select type is other than variable.
                    stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                    vector<stmt_ref> result = executor.get_all_stmts_uses(first_arg_stmt_type);
                    return SuchThatEval::convert_lst_string_to_int(result);
                }
            }
        } else if (!bool_pairs_args.first && !bool_pairs_args.second) {
            //both are constants
            if (such_that_second_arg == "_") {
                if(StringUtil::is_number(such_that_first_arg)) {
                    //this means first arg is a statement
                    vector<var_ref> result = executor.get_all_variables_uses_ref(stoi(such_that_first_arg));
                    return result;
                } else {
                    vector<var_ref> result = executor.get_all_variables_uses(such_that_first_arg);
                    return result;                    
                }
            } 
            else {
                bool query = executor.statement_uses(stoi(such_that_first_arg), such_that_second_arg);
                if (query) {
                    vector<string> default_solution = (mapStorage[select_type])[select_value];
                    return default_solution;
                } else {
                    return vector<string>{};
                }
            }
        } else {
            //one of them is a constant
            if (bool_pairs_args.first && !bool_pairs_args.second) {
                // first parameter is a variable and second parameter is a constant.
                Single first_arg_type  = storeDeclaration[such_that_first_arg];

                if(first_arg_type == Single::PROCEDURE) {
                    if(such_that_second_arg == "_") {
                        vector<proc_ref> result = executor.get_all_procedures_uses();
                        return result;
                    } else {
                        vector<proc_ref> result = executor.get_all_procedures_uses(such_that_second_arg);
                        return result;
                    }
                } else {
                    //first arg is statements not procedure.
                    stmt_type first_arg_stmt_type = SuchThatEval::convert_single_to_stmt_type(first_arg_type);
                    if(such_that_second_arg == "_") {
                        vector<stmt_ref> result = executor.get_all_stmts_uses(first_arg_stmt_type);
                        return SuchThatEval::convert_lst_string_to_int(result);
                    } else {
                        vector<stmt_ref> result = executor.get_all_stmts_uses(first_arg_stmt_type, such_that_second_arg);
                        return SuchThatEval::convert_lst_string_to_int(result);
                    }
                }      
            } else {
                // first parameter is a constant and second parameter is a variable.
                // second parameter has to be variable type
                if(such_that_first_arg == "_") {
                    vector<var_ref> result = executor.get_all_variables_uses(such_that_second_arg);
                    return result;
                }
                else if (StringUtil::is_number(such_that_first_arg)) {
                    //statement first arg type
                    vector<var_ref> result = executor.get_all_variables_uses_ref(stoi(such_that_first_arg));
                    return result;
                } else {
                    //procedure first arg type
                    vector<var_ref> result = executor.get_all_variables_uses(such_that_first_arg);
                    return result;
                }
            }
        }    
    } else {
        return vector<string> {};
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

vector<string> SuchThatEval::convert_lst_string_to_int(vector<int> lstA) {
    vector<string> output;
    for(auto i : lstA) {
        output.push_back(to_string(i));
    }
    return output;
}

bool SuchThatEval::is_pattern_variable_is_constant(std::string pattern_variable_value) {
    return(pattern_variable_value.at(0) == '"' && pattern_variable_value.at(pattern_variable_value.size()-1) == '"');
}