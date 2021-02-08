#include <string>
#include <vector>

class TNode
{
  /* value:type */
private:
  std::string value; // proc/var names, const, then, else, stmt number
  std::string type;  // var, const, call, assign, if, while, operators, stmtLst

  std::vector<TNode> children;

public:
  TNode();
  TNode(std::string val, std::string type);

  std::string getValue();
  std::string getType();
  std::vector<TNode> getChildren();
};
