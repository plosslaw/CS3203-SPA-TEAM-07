#include "TNode.h"

TNode::TNode()
{
  stmtNum = NULL_STMT_REF;
  value = "";
  type = "";
}

TNode::TNode(std::string val, std::string typ)
{
  stmtNum = NULL_STMT_REF;
  value = val;
  type = typ;
}

TNode::TNode(int num, std::string val, std::string typ)
{
  stmtNum = num;
  value = val;
  type = typ;
}

int TNode::getStmtNum()
{
  return stmtNum;
}

std::string TNode::getValue()
{
  return value;
}

std::string TNode::getType()
{
  return type;
}

std::vector<TNode> TNode::getChildren()
{
  return children;
}
