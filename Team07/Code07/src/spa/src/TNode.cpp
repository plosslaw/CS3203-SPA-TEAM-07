#include "TNode.h"
#include "Types.hpp"
#include <string>

TNode::TNode() {
  statementNum = NULL_STMT_REF;
  value = "";
  type = STATEMENT;
}

TNode::TNode(std::string val, stmt_type typ) {
  statementNum = NULL_STMT_REF;
  value = val;
  type = typ;
}

TNode::TNode(int num, std::string val, stmt_type typ) {
  statementNum = num;
  value = val;
  type = typ;
}

void TNode::addChild(TNode child) { children.push_back(child); }

int TNode::getStatementNum() { return statementNum; }

std::string TNode::getValue() { return value; }

stmt_type TNode::getType() { return type; }

std::vector<TNode> TNode::getChildren() { return children; }

std::string typeToString(stmt_type typ) {
  switch(typ) {
    case STATEMENT: return "STATEMENT";
    case READ: return "READ";
    case PRINT: return "PRINT";
    case CALL: return "CALL";
    case WHILE: return "WHILE";
    case IF: return "IF";
    case ASSIGN: return "ASSIGN";
    case CONSTANT: return "CONSTANT";
    case VARIABLE: return "VARIABLE";
    case PROCEDURE: return "PROCEDURE";
    case OPERATOR: return "OPERATOR";
    case STATEMENTLIST: return "STATEMENTLIST";
    case EXPR: return "EXPR";
    case PROGRAM: return "PROGRAM";
  }
  return "";
}

std::string TNode::toSexp(int sep) {
  std::string sepstr(sep, ' ');
  std::string str = children.size() == 0 ? "" : "( ";
  str += statementNum == NULL_STMT_REF ? "" : "[" + std::to_string(statementNum) + "]";
  str += value + ":" + typeToString(type);
  bool multiline = false;
  for (int i = 0; i < children.size(); i++) {
    multiline = multiline || (children[i].children.size() > 0);
  }
  for (int i = 0; i < children.size(); i++) {
    str += multiline ? "\n" + sepstr + "  " : " ";
    str += (children[i].toSexp(sep + 2));
  }
  str += children.size() == 0 ? "" : multiline ? ("\n" + sepstr + ")") : " )";
  return str;
}

