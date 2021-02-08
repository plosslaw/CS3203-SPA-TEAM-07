#pragma once

#include <string>

class Action {
    enum search_type {
        PATTERN,
        SUCH_THAT
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
    Action(search_type searchType, std::string firstArg, std::string secondArg,
        arg_type firstArgType, arg_type secondArgType);

private:
    std::string firstArg;
    std::string secondArg;

};
