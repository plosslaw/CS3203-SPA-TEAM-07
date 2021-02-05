#include <string>
#define NULL_STMT_REF -1

enum stmt_type {
    ASSIGN, 
    IF, 
    CALL, 
    PRINT, 
    WHILE, 
    READ
};

// note that we use NULL_STMT_REF to represent a null statement reference
typedef int stmt_ref;
typedef int assign_ref;
typedef std::string var_ref;

typedef struct {
    var_ref lvalue;
    std::string rvalue;
} pattern;


