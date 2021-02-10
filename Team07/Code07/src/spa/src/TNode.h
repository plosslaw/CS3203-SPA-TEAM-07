#pragma once

#include <string>
#include <vector>
#include "Types.hpp"

class TNode {
  /* value:type */
private:
  int statementNum;
  std::string value; // proc/var names, const, then, else
  stmt_type type;
  std::vector<TNode> children;

public:
  TNode();
  TNode(std::string val, stmt_type typ);
  TNode(int num, std::string val, stmt_type typ);

  void addChild(TNode child);
  int getStatementNum();
  std::string getValue();
  stmt_type getType();
  std::vector<TNode> getChildren();
};
