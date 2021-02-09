#include "QNode.h"

QNode::QNode()
{
  tag = CLAUSE;
  value = "";
  QNodeType nodeType;
  nodeType.clauseType = SELECT_CL;
  type = nodeType;
}

QNode::QNode(Tag tagType, std::string val, QNodeType nodeType)
{
  tag = tagType;
  value = val;
  type = nodeType;
}

Tag QNode::getTag() { return tag; }

std::string QNode::getValue() { return value; }

QNodeType QNode::getType() { return type; }

std::vector<QNode> QNode::getChildren() { return children; }
