#include "QNode.h"

QNode::QNode() {
  tag = CLAUSE;
  value = "";
  type.clauseType = SELECT_CL;
}

QNode::QNode(Tag tagType, std::string val, QNodeType nodeType) {
  tag = tagType;
  value = val;
  type = nodeType;
}

QNode::QNode(Tag tagType, std::string val, EntityType nodeType) {
  tag = tagType;
  value = val;
  type.entityType = nodeType;
}

QNode::QNode(Tag tagType, std::string val, RelationType nodeType) {
  tag = tagType;
  value = val;
  type.relationType = nodeType;
}

QNode::QNode(Tag tagType, std::string val, ClauseType nodeType) {
  tag = tagType;
  value = val;
  type.clauseType = nodeType;
}

void QNode::addChild(QNode child) { children.push_back(child); }

Tag QNode::getTag() { return tag; }

std::string QNode::getValue() { return value; }

QNodeType QNode::getType() { return type; }

std::vector<QNode> QNode::getChildren() { return children; }
