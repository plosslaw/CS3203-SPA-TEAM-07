#pragma once

#include <string>
#include <vector>

class Action {
    enum search_type {
        PATTERN,
        SUCH_THAT_FOLLOWS,
        SUCH_THAT_FOLLOWST,
        SUCH_THAT_PARENT,
        SUCH_THAT_PARENTT,
        SUCH_THAT_USES,
        SUCH_THAT_MODIFIES,
        DECLARATION
    };

/**
 * Refactor to use Types.h ???
 */
    enum arg_type {
        ASSIGN,
        IF,
        CALL,
        PRINT,
        WHILE,
        READ,
        STATEMENT,
        STRING,
        STATEMENT_NUM,
        WILDCARD
    };

public: 
    Action(search_type searchType, std::vector<std::string> argList,
        std::vector<arg_type> argTypeList);

private:
    search_type searchType;
    std::vector<std::string> argList;
    std::vector<arg_type> argTypeList;

};
