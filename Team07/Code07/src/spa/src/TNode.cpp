#include "TNode.h"

TNode::TNode() {
  statementNum = NULL_STMT_REF;
  value = "";
  type = STATEMENT;
}

TNode::TNode(std::string val, StatementType typ) {
  statementNum = NULL_STMT_REF;
  value = val;
  type = STATEMENT;
}

TNode::TNode(int num, std::string val, StatementType typ) {
  statementNum = num;
  value = val;
  type = typ;
}

void TNode::addChild(TNode child) { children.push_back(child); }

int TNode::getStatementNum() { return statementNum; }

std::string TNode::getValue() { return value; }

StatementType TNode::getType() { return type; }

std::vector<TNode> TNode::getChildren() { return children; }
