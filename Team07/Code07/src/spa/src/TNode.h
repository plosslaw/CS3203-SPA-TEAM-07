#include <string>
#include <vector>
#define NULL_STMT_REF -1

class TNode
{
  /* value:type */
private:
  int stmtNum;
  std::string value; // proc/var names, const, then, else
  std::string type;  // var, const, call, assign, if, while, operators, stmtLst

  std::vector<TNode> children;

public:
  TNode();
  TNode(std::string val, std::string typ);
  TNode(int num, std::string val, std::string typ);

  int getStmtNum();
  std::string getValue();
  std::string getType();
  std::vector<TNode> getChildren();
};
