#pragma once

#include "QueryMap.h"
#include "ActionsExecutor.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

class ActionsGenerator {
    private:
        //variables
        ActionsExecutor executor;
        QueryMap queryMap;

        // List of payloads from respective clauses - SELECT, DECLARATION, SUCHTHAT, PATTERN
        std::vector<PayLoad> select_list;
        std::vector<PayLoad> declaration_list;
        std::vector<PayLoad> such_that_list;
        std::vector<PayLoad> pattern_list;

        //Map declaration names to a list of its own entities type from PKB
        //example: stmt s1,s2..; stmtStorage = [(s1,[stmt_list]), s2,[stmt_list]...]
        std::unordered_map<std::string, std::vector<std::string>> procedure_storage;
        std::unordered_map<std::string, std::vector<std::string>> stmt_storage;
        std::unordered_map<std::string, std::vector<std::string>> read_storage;
        std::unordered_map<std::string, std::vector<std::string>> print_storage;
        std::unordered_map<std::string, std::vector<std::string>> assign_storage;
        std::unordered_map<std::string, std::vector<std::string>> while_storage;
        std::unordered_map<std::string, std::vector<std::string>> if_storage;
        std::unordered_map<std::string, std::vector<std::string>> constant_storage;
        std::unordered_map<std::string, std::vector<std::string>> variable_storage;
        std::unordered_map<std::string, std::vector<std::string>> call_storage;

        //store_declaration maps declaration name to Single (entitity type) for fast conversion, example: v1 => VARIABLE
        std::unordered_map<std::string, Single> store_declaration;

        //Maps Single to all types of storage such as procedure storage, stmt, read etc.
        std::unordered_map<Single, 
        std::unordered_map<std::string, std::vector<std::string>>> map_storage;
    
        //methods
        vector<string> zero_such_that_one_pattern(PayLoad pattern_pay_load, string select_value, Single select_type, pair<bool,bool> arg_pairs);
        vector<string> two_common_synonyms(PayLoad such_that_load, PayLoad pattern_load, string select_value, Single select_type);   
        vector<string> zero_common_synonyms(PayLoad such_that_load, PayLoad pattern_load, string select_value, Single select_type);   
        
        map<string,vector<string>> two_common_synonyms_pattern_val(PayLoad pattern_load, map<string,vector<string>> eval_map);
        
        //utilities 
        pair<bool,bool> check_if_args_are_variable(std::string first_arg, std::string second_arg);
        vector<pair<string,string>> crossproduct(vector<string> first_arg_lst, vector<string> second_arg_lst);
        stmt_type convert_single_to_stmt_type(Single s);
        
        bool is_element_inside_vectorA(string element, vector<string> vectorA);
        vector<string> inner_join_A(vector<string> lstA, vector<string> lstB);//check if elements in lstA is present in lstB
    public:
        std::vector<PayLoad> get_pattern_list();
        ActionsGenerator();
        ActionsGenerator(QueryMap mapQuery, ActionsExecutor executorActions);
        std::unordered_map<Single, 
            std::unordered_map<std::string, std::vector<std::string>>> preprocess();
        void set_map_storage_store_declaration(std::unordered_map<Single, 
            std::unordered_map<std::string, std::vector<std::string>>> storage_map,
            std::unordered_map<std::string, Single> store_declaration);
        void set_Query_Map(QueryMap mapQuery);
        std::vector<std::string> TraverseQueryMap();
};
