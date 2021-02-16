#include "ActionsGenerator.h"
#include "StringUtil.h"
#include <sstream>
//#include "QueryEvaluator.h"

ActionsGenerator::ActionsGenerator() {

}

ActionsGenerator::ActionsGenerator(QueryMap mapQuery, ActionsExecutor executerActions) {
    executer = executerActions;
    queryMap = mapQuery;
}

std::vector<std::string> ActionsGenerator::TraverseQueryMap() {
    selectList= queryMap.getList(ClauseType::SELECT);
    declarationList = queryMap.getList(ClauseType::DECLARATION);
    suchThatList = queryMap.getList(ClauseType::SUCHTHAT);
    patternList = queryMap.getList(ClauseType::PATTERN);
    
    // //here
    // std::string dummyS = "s";
    // std::vector<std::string> dummyVS;
    // dummyVS.push_back("1");dummyVS.push_back("2"); dummyVS.push_back("3");
    // std::string dummyA = "a";
    // std::vector<std::string> dummyAS;
    // dummyAS.push_back("1");dummyAS.push_back("2"); dummyAS.push_back("3");
    // stmtStorage[dummyS] = dummyVS; 
    // assignStorage[dummyA] = dummyAS;
    // std::string dummyV = "v";
    // std::vector<std::string> dummyVar;
    // dummyVar.push_back("a");dummyVar.push_back("b"); dummyVar.push_back("c");
    // variableStorage[dummyV] = dummyVar;
    
    //DECLARATION
    for(auto i: declarationList) {
        if (i.getType().single != Single::SYNONYM) {
            std::vector<std::string> payloadValues = i.getValue();
            storeDeclaration[payloadValues[0]] = i.getType().single;            
        }
    }
    for(auto i: storeDeclaration) {
        Single s = i.second;
        if (s == Single::STATEMENT) {
            // put inside statementstorage
        } else if (s == Single::READ) {    
            
        } else if (s == Single::PRINT) {
        
        } else if (s == Single::CALL) {
        
        } else if (s == Single::WHILE) {
        
        } else if (s == Single::IF) {
        
        } else if (s == Single::ASSIGN) {
        
        } else {
            throw "Payload Single is not STATEMENT/READ/PRINT/CALL/WHILE/IF/ASSIGN.";
        }
    }
        /*
            case Single::PROCEDURE:
                break;
            
            case Single::CONSTANT:
          
                 break;
            case Single::VARIABLE:
          
                //variableStorage[i.first] = generate action => retrieve all variables from PKB
                break;
        */
    
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
    PayLoad selectPayLoad = selectList[0];
        //retrieve from pkb select synonym. example stmts s, Select s..., retrieve all stmts from pkb
    if (selectPayLoad.getType().single != Single::SYNONYM) {
        throw "SELECT payload is not a synonym. Synonym is required.";
    }
    std::string selectValue = (selectPayLoad.getValue())[0];
    Single selectSynonym = selectPayLoad.getType().single;
    
    //Default solution is the solution to SELECT without considering SUCH THAT and PATTERN.
    std::vector<std::string> defaultSolution = std::vector<string>{"1","2","3"};//here mapStorage[selectSynonym][selectValue];

    //Evaluated solution
    std::unordered_map<std::string, std::vector<std::string>> solutionSuchThat;
    std::unordered_map<std::string, std::vector<std::string>> solutionPattern;

    bool isSuchThatEmpty = suchThatList.empty();
    bool isPatternEmpty = patternList.empty();
    
    if (isSuchThatEmpty && isPatternEmpty) {
        //there is no such that and pattern clause
        //here return defaultSolution;
        return std::vector<string>{"1","2","3"};
    } else if (!isSuchThatEmpty && isPatternEmpty) {
        // there is only suchthat clause but no pattern clause
        PayLoad suchThatPayLoad = suchThatList[0];

        bool isSelectInSuchThat = false;
        std::string suchThatFirstArg = (suchThatPayLoad.getValue())[0];
        std::string suchThatSecondArg = (suchThatPayLoad.getValue())[1];

        if(!suchThatFirstArg.compare(selectValue) || !suchThatSecondArg.compare(selectValue)) {
            isSelectInSuchThat = true;
        }
        if (isSelectInSuchThat) {
            std::unordered_map<std::string, std::vector<std::string>> evaluatedSuchThat = evalSuchThatPre(suchThatFirstArg, suchThatSecondArg, selectValue, suchThatPayLoad);
            return evaluatedSuchThat[selectValue];
            //return std::vector<string>{"1","2","3"};
        } else {
            // return std::vector<string>{"1","2","3"};
            return defaultSolution;
        }
    } else if (isSuchThatEmpty && !isPatternEmpty) {  
        // there is pattern clause but no such that clause.
        PayLoad patternPayLoad = patternList[0];
        std::string patternFirstArg = (patternPayLoad.getValue())[0];
        std::string patternSecondArg = (patternPayLoad.getValue())[1];
        bool isSelectInPattern = false;
        if(!patternFirstArg.compare(selectValue) || !patternSecondArg.compare(selectValue)) {
        isSelectInPattern = true;
        }
        if (isSelectInPattern) {
            // call patternEval
            //return
        } else {
            return defaultSolution;
        }

    } else {
        // there is both pattern and empty clause
        PayLoad suchThatPayLoad = suchThatList[0];
        PayLoad patternPayLoad = patternList[0];

        std::string suchThatFirstArg = (suchThatPayLoad.getValue())[0];
        std::string suchThatSecondArg = (suchThatPayLoad.getValue())[1];

        std::string patternFirstArg = (patternPayLoad.getValue())[0];
        std::string patternSecondArg = (patternPayLoad.getValue())[1];

        bool isSelectInSuchThat = false;
        bool isSelectInPattern = false;

        if(!suchThatFirstArg.compare(selectValue) || !suchThatSecondArg.compare(selectValue)) {
            isSelectInSuchThat = true;
        }
        if(!patternFirstArg.compare(selectValue) || !patternSecondArg.compare(selectValue)) {
            isSelectInPattern = true;
        }
        if (!isSelectInSuchThat && !isSelectInPattern) {
            return defaultSolution;
        } 
        // to do...
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
    return permutated;
}

//return list
std::unordered_map<std::string, std::vector<std::string>> ActionsGenerator::evalSuchThatPre(std::string firstArg, std::string secondArg, 
std::string selectValue, PayLoad loadpay) {
    std::pair<bool,bool> pairArgs = checkIfArgsAreVariable(firstArg, secondArg);
    
    int findSelectVal;
    if(!selectValue.compare(firstArg)) {
        findSelectVal = 0;
    } else if (!selectValue.compare(secondArg)) {
        findSelectVal = 1;
    }

    std::unordered_map<std::string, std::vector<std::string>> evaluatedSuchThat;
    
    if(pairArgs.first && pairArgs.second) {
        std::vector<std::pair <std::string, std::string>> permutated = ActionsGenerator::bothAreVariables(firstArg, secondArg);
        for(auto j : permutated) {
            std::string actionFirstArg = j.first;
            std::string actionSecondArg = j.second;
            bool isInPKB = evalSuchThat(loadpay.getType().pair, actionFirstArg, actionSecondArg);
            if(isInPKB) {
                //if element is not in evaluatedSuchThat, add it.
                if(std::find(evaluatedSuchThat[firstArg].begin(), evaluatedSuchThat[firstArg].end(),actionFirstArg) == evaluatedSuchThat[firstArg].end()) {
                    evaluatedSuchThat[firstArg].push_back(actionFirstArg);
                }
                if(std::find(evaluatedSuchThat[secondArg].begin(), evaluatedSuchThat[secondArg].end(),actionSecondArg) == evaluatedSuchThat[secondArg].end()) {
                    evaluatedSuchThat[secondArg].push_back(actionSecondArg);
                }
            }
        }
    } else if(pairArgs.first && !pairArgs.second) {
        Single firstArgSynonym = storeDeclaration[firstArg];
        std::vector<std::string> firstArgLst = mapStorage[firstArgSynonym][firstArg];
        for(auto i : firstArgLst) {      
            bool isInPKB = evalSuchThat(loadpay.getType().pair, i, secondArg);
            
            if(isInPKB) {
                if(std::find(evaluatedSuchThat[firstArg].begin(), evaluatedSuchThat[firstArg].end(),i) == evaluatedSuchThat[firstArg].end()) {
                    evaluatedSuchThat[firstArg].push_back(i);
                }
            }
        }
    } else if (!pairArgs.first && pairArgs.second) {
        Single secondArgSynonym = storeDeclaration[secondArg];
        std::vector<std::string> secondArgLst = mapStorage[secondArgSynonym][secondArg];
        for(auto i : secondArgLst) {
            bool isInPKB = evalSuchThat(loadpay.getType().pair, firstArg, i);
            if(isInPKB) {
                if(std::find(evaluatedSuchThat[secondArg].begin(), evaluatedSuchThat[secondArg].end(),i) == evaluatedSuchThat[secondArg].end()) {
                    evaluatedSuchThat[secondArg].push_back(i);
                }
            }
        }
    } else {
        bool isInPKB = evalSuchThat(loadpay.getType().pair, firstArg, secondArg);
        if(isInPKB) {
                //evaluated.push_back(suchThatArgs[findSelectVal]);
            }
    }
    //testing evaluatedSuchThat["s"]= std::vector<std::string>{"1","2","3",firstArg,secondArg,to_string(pairArgs.first),to_string(pairArgs.second)};
    return evaluatedSuchThat;
}

bool ActionsGenerator::evalSuchThat(Pair suchThatType, std::string actionFirstArg, std::string actionSecondArg) {
    std::vector<std::string> argLst;
    argLst.push_back(actionFirstArg);
    argLst.push_back(actionSecondArg);
    switch (suchThatType)
    {
        {
            case Pair::FOLLOWS:
            
            return true;
            break;
        } {
        case Pair::FOLLOWST:
            
            return true;
            break;
        } {
        case Pair::PARENT:
            
            return true;
            break;
        } {
        case Pair::PARENTT:
            
            return true;
            break;
        } {
        case Pair::USES:
            
            return true;
            break;
        } {
        case Pair::MODIFIES:
            
            return true;
            break;
        }    
        default:
            throw "such that type is not FOLLOWS/FOLLOWST/PARENT/PARENTT/USES/MODIFIES.";
            break;
    }
}
/*
std::unordered_map<std::string, std::vector<std::string>> ActionsGenerator::evalPattern(PayLoad loadPay, 
std::unordered_map<std::string, std::vector<std::string>> solutionStorage) {
    std::vector<std::string> suchThatArgs = ActionsGenerator::extract(loadPay.getValue());
    std::string firstArg = suchThatArgs[0];
    std::string secondArg = suchThatArgs[1];
    std::unordered_map<std::string, std::vector<std::string>> evaluatedSolution;
    //if solutionStorage is empty, this implies there is no such-that clause 
    if(!solutionStorage.empty()) {
        if(solutionStorage.find(firstArg) == solutionStorage.end()) { // assign a is not found 
            std::vector<std::string> assignFromStorage = mapStorage[ASSIGN].begin()->second;
            for(auto i : assignFromStorage) {
                // generate action -pattern(i,secondArg);
                if (true) {
                    //add to evaluatedSolution[]
                }
            }
        } else {// found
            std::vector<std::string> solutionFromSuchThat = solutionStorage[firstArg];
            for(auto i : solutionFromSuchThat) {
                //generate action - pattern(i, secondArg)
                if(true) {
                    //add to evaluatedSolution[]
                }
            }
        }
    } else {
        std::vector<std::string> assignFromStorage = mapStorage[ASSIGN].begin()->second;
            for(auto i : assignFromStorage) {
                // generate action -pattern(i,secondArg);
                if (true) {
                    //add to evaluatedSolution[]
                }
            }
    }
    //generate action- Pattern (firstArg, SecondArg)
    //return list 
    return evaluatedSolution;   
}
*/
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

/*
std::vector<std::string> ActionsGenerator::extract(std::string payloadVal) {
    std::vector<std::string> output;
    std::stringstream splitter(payloadVal);
    std::string intermediate;

    while(getline(splitter, intermediate, ',')) {
        output.push_back(StringUtil::removeWhiteSpaces(intermediate));
    }
    return output;
}
*/
