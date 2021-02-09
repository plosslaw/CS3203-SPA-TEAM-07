#include <string>
#include <vector>
#define NULL_STMT_REF -1

enum statementType {
  STATEMENT,
  READ,
  PRINT,
  CALL,
  WHILE,
  IF,
  ASSIGN,
  CONSTANT,
  VARIABLE,
  PROCEDURE,
  OPERATOR,
  STATEMENTLIST,
};

class TNode {
  /* value:type */
private:
  int statementNum;
  std::string value; // proc/var names, const, then, else
  statementType type;

  std::vector<TNode> children;

public:
  TNode();
  TNode(std::string val, statementType typ);
  TNode(int num, std::string val, statementType typ);

  void addChild(TNode child);
  int getStatementNum();
  std::string getValue();
  statementType getType();
  std::vector<TNode> getChildren();
};
