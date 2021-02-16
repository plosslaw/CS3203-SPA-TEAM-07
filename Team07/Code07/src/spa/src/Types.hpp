#pragma once

#include <string>
#define NULL_STMT_REF -1

// Enum for type of statements (e.g. read / print / assign) in the SIMPLE program
enum stmt_type {
  STATEMENT,
  READ,
  PRINT,
  CALL,
  WHILE,
  IF,
  ASSIGN,
  CONSTANT,
  VARIABLE,
  PROCEDURE,
  OPERATOR,
  STATEMENTLIST,
  EXPR,
  PROGRAM,
};

// Statement number of the referenced assign statement
typedef int assign_ref;

// Integer value of the constant
typedef int const_value;

// Statement number, note that we use NULL_STMT_REF to represent a null statement reference
typedef int stmt_ref;

// Name of the procedure
typedef std::string proc_ref;

// Name of the variable
typedef std::string var_ref;

// Representation of a pattern for the pattern clause of a query
typedef struct {
    var_ref lvalue;
    std::string rvalue;
} pattern;
