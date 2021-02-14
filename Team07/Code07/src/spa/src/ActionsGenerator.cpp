#include "ActionsGenerator.h";
#include "ActionsExecutor.h";
#include "QueryMap.h";
#include <"Action.h">;

void ActionsGenerator::TraverseQueryMap(QueryMap queryMap) {
    selectList= queryMap.getList(SELECT);
    declarationList = queryMap.getList(DECLARATION);
    suchThatList = queryMap.getList(SUCHTHAT);
    patternList = queryMap.getList(PATTERN);
    
    mapStorage[PROCEDURE] = procedureStorage;
    mapStorage[STATEMENT] = stmtStorage;
    mapStorage[READ] = readStorage;
    mapStorage[PRINT] = printStorage;
    mapStorage[ASSIGN] = assignStorage;
    mapStorage[WHILE] = whileStorage;
    mapStorage[IF] = ifStorage;
    mapStorage[CONSTANT] = constantStorage;

    //DECLARATION
    for(auto i: declarationList) {
        if (i.getType().single != SYNONYM) {
            storeDeclaration[i.getValue()] = i.getType().single;            
        }
    }

    for(auto i: storeDeclaration) {
        Single s = i.second;
        switch(s){
            case STATEMENT:
                //stmtStorage[i.first] = generate action => retrieve all statements from PKB
                break;
        /*    case READ:
                break;
            case PRINT:
                break;
            case CALL:
                break;
            case WHILE:
                break;
            case IF:
                break;
            case ASSIGN:
                break;
            case PROCEDURE:
                break;
        */
            case CONSTANT:
                //constantStorage[i.first] = generate action => retrieve all constants from PKB
                break;
            case VARIABLE:
                //variableStorage[i.first] = generate action => retrieve all variables from PKB
                break;
        }
    }


    //SELECT
    PayLoad selectPayLoad = selectList.front();
        //retrieve from pkb select synonym. example stmts s, Select s..., retrieve all stmts from pkb
    if (selectPayLoad.getType().single != SYNONYM) {
        throw "SELECT payload is not a synonym. Synonym is required.";
    }
    std::string selectValue = selectPayLoad.getValue();
    Single selectSynonym = storeDeclaration[selectValue];
    
    bool isSelectValueInSuchThatPattern = false;

    for(auto i : suchThatList) {
        std::string firstArg = i.getValue;
        std::string secondArg = i.getValue2;
        if(firstArg.compare(selectValue) || secondArg.compare(selectValue)) {
            isSelectValueInSuchThatPattern = true;
        }
    }
    for(auto i : patternList) {
        std::string firstArg = i.getValue;
        std::string secondArg = i.getValue2;
        if(firstArg.compare(selectValue) || secondArg.compare(selectValue)) {
            isSelectValueInSuchThatPattern = true;
        }
    }
    
    if (!isSelectValueInSuchThatPattern) {
        //SUCHTHAT
        // Follows,parent,uses
        // assume suchThatList has only 1 such-that clause as part of iteration 1 requirement.
        //need to validate constant string such as "1" and "_".
        for(auto i : suchThatList) {
            evalSuchThatPre(i);
            
        }

        //PATTERN
        for(auto i : patternList) {
            evalPattern(i);
        }
    } else {
        
    }
    

};


std::pair<bool,bool> ActionsGenerator::checkIfArgsAreVariable(std::string firstArg, std::string secondArg) {
    bool isFirstArgVariable = false;
    bool isSecondArgVariable = false;

    //check if string is in the storedeclaration.
    if(storeDeclaration.find(firstArg) != storeDeclaration.end()) {
        isFirstArgVariable = true;
    }
    if(storeDeclaration.find(secondArg) != storeDeclaration.end()) {
        isSecondArgVariable = true;
    }
    std::pair<bool, bool> pairArgs;
    pairArgs.first = isFirstArgVariable;
    pairArgs.second = isSecondArgVariable;
    return pairArgs;
} 

std::vector<std::pair <std::string, std::string>> ActionsGenerator::bothAreVariables(std::string firstArg, std::string secondArg) {
    Single firstArgSynonym = storeDeclaration[firstArg];
    Single secondArgSynonym = storeDeclaration[secondArg];
    std::vector<std::string> firstArgLst = mapStorage[firstArgSynonym][firstArg];
    std::vector<std::string> secondArgLst = mapStorage[secondArgSynonym][secondArg];
    std::vector<std::pair <std::string, std::string>> permutated = ActionsGenerator::permutateValues(firstArgLst, secondArgLst);
    return std::vector<std::pair <std::string, std::string>>;
}

void ActionsGenerator::evalSuchThatPre(PayLoad loadPay) {
    std::string firstArg = loadPay.getValue();
    std::string secondArg = loadPay.getValue2();
    std::pair<bool,bool> pairArgs = checkIfArgsAreVariable(firstArg, secondArg, storeDeclaration);
    if(pairArgs.first && pairArgs.second) {
        std::vector<std::pair <std::string, std::string>> permutated = ActionsGenerator::bothAreVariables(firstArg, secondArg);
        for(auto j : permutated) {
            std::string actionFirstArg = j.first;
            std::string actionSecondArg = j.second;
            evalSuchThat(loadPay.getType().pair, actionFirstArg, actionSecondArg);
        }
    } else if(pairArgs.first && !pairArgs.second) {
        Single firstArgSynonym = storeDeclaration[firstArg];
        std::vector<std::string> firstArgLst = mapStorage[firstArgSynonym][firstArg];
        for(auto i : firstArgLst) {      
            evalSuchThat(loadPay.getType().pair, i, secondArg);
            //Generate an action check FOLLOWS (SUCH_THAT_FOLLOWS, argLst, )
        }
    } else if (!pairArgs.first && pairArgs.second) {
        Single secondArgSynonym = storeDeclaration[secondArg];
        std::vector<std::string> secondArgLst = mapStorage[secondArgSynonym][secondArg];
        for(auto i : secondArgLst) {
            evalSuchThat(loadPay.getType().pair, firstArg, i);
            //Generate an action check FOLLOWS (SUCH_THAT_FOLLOWS, argLst, )
        }
    } else {
        evalSuchThat(loadPay.getType().pair, firstArg, secondArg);
        //CONSTANT, CONSTANT. Generate an action check FOLLOWS 
    }
}

void ActionsGenerator::evalSuchThat(Pair suchThatType, std::string actionFirstArg, std::string actionSecondArg) {
    std::vector<std::string> argLst;
    argLst.push_back(actionFirstArg);
    argLst.push_back(actionSecondArg);
    switch (suchThatType)
    {
    case FOLLOWS:
        
        break;
    case FOLLOWST:
        break;
    case PARENT:
        break;
    case PARENTT:
        break;
    case USES:

        break;
    case MODIFIES:
        break;
    default:
        break;
    }
}

void ActionsGenerator::evalPattern(PayLoad loadPay) {
    std::string firstArg = loadPay.getValue();
    std::string secondArg = loadPay.getValue2();
}
std::vector<std::pair <std::string, std::string>> ActionsGenerator::permutateValues(std::vector<std::string> firstArg, 
    std::vector<std::string> secondArg){
    std::vector<std::pair <std::string, std::string>> permutated;
    for(auto i : firstArg) {
        for(auto j : secondArg) {
            permutated.push_back(std::make_pair(i,j));
        }
    }
    return permutated;
};

