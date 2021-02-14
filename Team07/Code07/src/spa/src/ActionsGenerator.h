#pragma once

#include "Action.h"

class ActionsGenerator {

    private:
        std::vector<PayLoad> selectList;
        std::vector<PayLoad> declarationList;
        std::vector<PayLoad> suchThatList;
        std::vector<PayLoad> patternList;

        //Storage to hold list of all entities declared from PKB
        //example: stmtStorage = [(s1,[s1_list]), s2,[s2_list]...]
        std::unordered_map<std::string, std::vector<std::string>> procedureStorage;
        std::unordered_map<std::string, std::vector<std::string>> stmtStorage;
        std::unordered_map<std::string, std::vector<std::string>> readStorage;
        std::unordered_map<std::string, std::vector<std::string>> printStorage;
        std::unordered_map<std::string, std::vector<std::string>> assignStorage;
        std::unordered_map<std::string, std::vector<std::string>> whileStorage;
        std::unordered_map<std::string, std::vector<std::string>> ifStorage;
        std::unordered_map<std::string, std::vector<std::string>> constantStorage;


        //storeDeclaration maps declaration for fast conversion, example: v1 => VARIABLE
        std::unordered_map<std::string, Single> storeDeclaration;

        //Maps Single to all types of storage such as procedure storage, stmt, read etc.
        std::unordered_map<Single, 
        std::unordered_map<std::string, std::vector<std::string>>> mapStorage;
    

        // methods
        std::vector<std::pair <std::string, std::string>> ActionsGenerator::permutateValues(std::vector<std::string> firstArg, std::vector<std::string> secondArg);
        std::vector<std::pair <std::string, std::string>> ActionsGenerator::bothAreVariables(std::string firstArg, std::string secondArg);
        std::vector<std::pair <std::string, std::string>> ActionsGenerator::VarAndNotVar(std::string firstArg, std::string secondArg);
        std::vector<std::pair <std::string, std::string>> ActionsGenerator::NotVarAndVar(std::string firstArg, std::string secondArg);
        std::pair<bool,bool> ActionsGenerator::checkIfArgsAreVariable(std::string firstArg, std::string secondArg);
        void ActionsGenerator::evalSuchThatPre(PayLoad loadPay);
        void ActionsGenerator::evalPattern(PayLoad loadPay);
        void ActionsGenerator::evalSuchThat(Pair suchThatType, std::string actionFirstArg, std::string actionSecondArg);
    public:
        void ActionsGenerator::TraverseQueryMap(QueryMap queryMap);
        
};