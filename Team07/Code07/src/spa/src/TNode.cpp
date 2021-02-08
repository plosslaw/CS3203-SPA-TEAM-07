#include "TNode.h"

TNode::TNode()
{
  value = "";
  type = "";
}

TNode::TNode(std::string val, std::string typ)
{
  value = val;
  type = typ;
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
