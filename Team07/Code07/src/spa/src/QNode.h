#include <string>
#include <vector>

enum EntityType {
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
};

enum RelationType {
  FOLLOWS,
  FOLLOWST,
  PARENT,
  PARENTT,
  USES,
  MODIFIES,
};

enum ClauseType { DECLARATION, SELECT_CL, SUCHTHAT_CL, PATTERN_CL };

enum Tag {
  ENTITY,
  RELATION,
  CLAUSE,
};

union QNodeType {
  EntityType entityType;
  RelationType relationType;
  ClauseType clauseType;
};

class QNode {
  /* tag value:type */
private:
  Tag tag;
  std::string value;
  QNodeType type;

  std::vector<QNode> children;

public:
  QNode();
  QNode(Tag tagType, std::string val, QNodeType nodeType);

  void addChild(QNode child);
  Tag getTag();
  std::string getValue();
  QNodeType getType();
  std::vector<QNode> getChildren();
};
