#include "Action.h"

Action::Action(search_type searchType, std::string firstArg, std::string secondArg,
    arg_type firstArgType, arg_type secondArgType) {
        this->searchType = searchType;
        this->firstArg = firstArg;
        this->secondArg = secondArg;
        this->firstArgType = firstArgType;
        this->secondArgType = secondArgType;
    }
