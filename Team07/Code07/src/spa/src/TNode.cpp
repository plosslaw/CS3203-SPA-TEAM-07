#include "TNode.h"
#include "Types.hpp"
#include <string>

TNode::TNode() {
  statementNum = NULL_STMT_REF;
  value = "";
  type = STATEMENT;
  pos = 0;
}

TNode::TNode(std::string val, stmt_type typ) {
  statementNum = NULL_STMT_REF;
  value = val;
  type = typ;
  pos = 0;
}

TNode::TNode(int num, std::string val, stmt_type typ) {
  statementNum = num;
  value = val;
  type = typ;
  pos = 0;
}

TNode::TNode(std::string val, stmt_type typ, int p) {
  statementNum = NULL_STMT_REF;
  value = val;
  type = typ;
  pos = p;
}

TNode::TNode(int num, std::string val, stmt_type typ, int p) {
  statementNum = num;
  value = val;
  type = typ;
  pos = p;
}

TNode& TNode::addChild(TNode child) { children.push_back(child); return *this; }

int TNode::getStatementNum() { return statementNum; }

int TNode::getPos() { return pos; }

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

std::string TNode::toStub(int sep) {
  std::string sepstr(sep, ' ');
  std::string str(sepstr + "TNode(");
  if(statementNum != NULL_STMT_REF) str += std::to_string(statementNum) + ", ";
  str += "\"" + value + "\", " + typeToString(type) +", " + std::to_string(pos) +")";
  for(int i = 0; i < children.size(); i++) {
    str += ".addChild(\n";
    str += children[i].toStub(sep + 2);
    str += ")";
  }
  return str;
}

bool TNode::eq(TNode rhs) {
  if (
    statementNum == rhs.statementNum &&
    value == rhs.value &&
    type == rhs.type &&
    pos == rhs.pos &&
    children.size() == rhs.children.size()) {
      for(int i = 0; i < children.size(); i++) {
        bool res = children[i].eq(rhs.children[i]);
        if(!res) return false;
      }
      return true;
    }
    return false;
}
