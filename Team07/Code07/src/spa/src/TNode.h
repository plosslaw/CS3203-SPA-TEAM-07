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
  int pos; // position in source file

public:
  TNode();
  TNode(std::string val, stmt_type typ);
  TNode(int num, std::string val, stmt_type typ);
  TNode(std::string val, stmt_type typ, int p);
  TNode(int num, std::string val, stmt_type typ, int p);

  void addChild(TNode child);
  int getStatementNum();
  int getPos();
  std::string getValue();
  stmt_type getType();
  std::vector<TNode> getChildren();
  std::string toSexp(int sep = 0);
};