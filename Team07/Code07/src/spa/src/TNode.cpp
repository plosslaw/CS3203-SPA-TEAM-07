#include "TNode.h"

TNode::TNode()
{
  statementNum = NULL_STMT_REF;
  value = "";
  type = STATEMENT;
}

TNode::TNode(std::string val, statementType typ)
{
  statementNum = NULL_STMT_REF;
  value = val;
  type = STATEMENT;
}

TNode::TNode(int num, std::string val, statementType typ)
{
  statementNum = num;
  value = val;
  type = typ;
}

int TNode::getStatementNum()
{
  return statementNum;
}

std::string TNode::getValue()
{
  return value;
}

statementType TNode::getType()
{
  return type;
}

std::vector<TNode> TNode::getChildren()
{
  return children;
}
