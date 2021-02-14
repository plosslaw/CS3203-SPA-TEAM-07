#include "ActionsGenerator.h";
#include "ActionsExecutor.h";
#include "QueryMap.h";
#include "StringUtil.h";
#include <sstream>;
#include <"Action.h">;


std::vector<std::string> ActionsGenerator::TraverseQueryMap(QueryMap queryMap) {
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
    
    //Default solution
    std::vector<std::string> defaultSolution = mapStorage[selectSynonym][selectValue];

    //Evaluated solution
    std::vector<std::string> solutionSuchThat;
    std::vector<std::string> solutionPattern;

    bool isSelectValueInSuchThatPattern = false;

    for(auto i : suchThatList) {
        std::vector<std::string> suchThatArgs = ActionsGenerator::extract(i.getValue());
        std::string firstArg = suchThatArgs[0];
        std::string secondArg = suchThatArgs[1];
        if(!firstArg.compare(selectValue) || !secondArg.compare(selectValue)) {
            isSelectValueInSuchThatPattern = true;
        }
    }
    for(auto i : patternList) {
        std::vector<std::string> suchThatArgs = ActionsGenerator::extract(i.getValue());
        std::string firstArg = suchThatArgs[0];
        std::string secondArg = suchThatArgs[1];
        if(!firstArg.compare(selectValue) || !secondArg.compare(selectValue)) {
            isSelectValueInSuchThatPattern = true;
        }
    }

    
    if (!isSelectValueInSuchThatPattern) {
        //SUCHTHAT
        // Follows,parent,uses
        for(auto i : suchThatList) {
            solutionSuchThat = evalSuchThatPre(i, selectValue);
        }

        //PATTERN
        for(auto i : patternList) {
            solutionPattern = evalPattern(i);
        }

        //do a crossproduct?
        std::vector<std::string> evaluatedSolution;
        return evaluatedSolution;
    } else {
        //return the default solution
        return defaultSolution;
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

//return list
std::vector<std::string> ActionsGenerator::evalSuchThatPre(PayLoad loadPay, std::string selectValue) {
    std::vector<std::string> suchThatArgs = ActionsGenerator::extract(loadPay.getValue());
    std::string firstArg = suchThatArgs[0];
    std::string secondArg = suchThatArgs[1];
    std::pair<bool,bool> pairArgs = checkIfArgsAreVariable(firstArg, secondArg, storeDeclaration);
    
    int findSelectVal;
    if(!selectValue.compare(firstArg)) {
        findSelectVal = 0;
    } else if (!selectValue.compare(secondArg)) {
        findSelectVal = 1;
    }

    std::unordered_map<std::string, std::vector<std::string>> evaluatedStorage;
    std::vector<std::string> evaluatedSolution;

    if(pairArgs.first && pairArgs.second) {
        std::vector<std::pair <std::string, std::string>> permutated = ActionsGenerator::bothAreVariables(firstArg, secondArg);
        for(auto j : permutated) {
            std::string actionFirstArg = j.first;
            std::string actionSecondArg = j.second;
            bool isInPKB = evalSuchThat(loadPay.getType().pair, actionFirstArg, actionSecondArg);
            if(isInPKB) {
                //evaluatedSolution.push_back(suchThatArgs[findSelectVal]);
                evaluatedStorage[firstArg].push_back(suchThatArgs[firstArg]);
                evaluatedStorage[secondArg].push_back(suchThatArgs[secondArg]);
            }
        }
    } else if(pairArgs.first && !pairArgs.second) {
        Single firstArgSynonym = storeDeclaration[firstArg];
        std::vector<std::string> firstArgLst = mapStorage[firstArgSynonym][firstArg];
        for(auto i : firstArgLst) {      
            bool isInPKB = evalSuchThat(loadPay.getType().pair, i, secondArg);
            if(isInPKB) {
                //evaluatedSolution.push_back(suchThatArgs[findSelectVal]);
                evaluatedStorage[firstArg].push_back(suchThatArgs[firstArg]);
            }
        }
    } else if (!pairArgs.first && pairArgs.second) {
        Single secondArgSynonym = storeDeclaration[secondArg];
        std::vector<std::string> secondArgLst = mapStorage[secondArgSynonym][secondArg];
        for(auto i : secondArgLst) {
            bool isInPKB = evalSuchThat(loadPay.getType().pair, firstArg, i);
            if(isInPKB) {
                //evaluatedSolution.push_back(suchThatArgs[findSelectVal]);
                evaluatedStorage[secondArg].push_back(suchThatArgs[secondArg]);
            }
        }
    } else {
        bool isInPKB = evalSuchThat(loadPay.getType().pair, firstArg, secondArg);
        if(isInPKB) {
                evaluatedSolution.push_back(suchThatArgs[findSelectVal]);
            }
    }
    //return evaluatedSolution;
    return evaluatedStorage[suchThatArgs[findSelectVal]];
}

bool ActionsGenerator::evalSuchThat(Pair suchThatType, std::string actionFirstArg, std::string actionSecondArg) {
    std::vector<std::string> argLst;
    argLst.push_back(actionFirstArg);
    argLst.push_back(actionSecondArg);
    switch (suchThatType)
    {
    case FOLLOWS:
        
        return true;
        break;
    case FOLLOWST:

        return true;
        break;
    case PARENT:
        return true;
        break;
    case PARENTT:
        return true;
        break;
    case USES:
        return true;
        break;
    case MODIFIES:
        return true;
        break;
    default:
        break;
    }
}

std::vector<std::string> ActionsGenerator::evalPattern(PayLoad loadPay) {
    std::vector<std::string> suchThatArgs = ActionsGenerator::extract(loadPay.getValue());
    std::string firstArg = suchThatArgs[0];
    std::string secondArg = suchThatArgs[1];

    //generate action- Pattern (firstArg, SecondArg)

    //return list    
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

std::vector<std::string> ActionsGenerator::extract(std::string payloadVal) {
    std::vector<std::string> output;
    std::stringstream splitter(payloadVal);
    std::string intermediate;

    while(getline(splitter, intermediate, ',')) {
        output.push_back(StringUtil::removeWhiteSpaces(intermediate));
    }
    return output;
}
