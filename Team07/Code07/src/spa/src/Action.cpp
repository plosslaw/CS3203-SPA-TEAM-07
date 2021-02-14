#include "Action.h"

#include "PKBQueryController.hpp";
using namespace std;

Action::Action(search_type searchType, vector<string> argList, vector<arg_type> argTypeList)  {
        if (argList.size() != argTypeList.size()) {
            throw "Size of Argument List differs from the Size of Argument Type List";
        }
        this->searchType = searchType;
        for (int i = 0; i < argList.size(); i ++) {
            this->argList.push_back(argList[i]);
            this->argTypeList.push_back(argTypeList[i]);
        }
    }
