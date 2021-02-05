#include <string>

enum stmt_type {
    ASSIGN, 
    IF, 
    CALL, 
    PRINT, 
    WHILE, 
    READ
};

typedef int stmt_ref;
typedef std::string var_ref;

typedef struct {
    var_ref lvalue;
    std::string rvalue;
} pattern;


