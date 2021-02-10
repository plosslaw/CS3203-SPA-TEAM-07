#include <string>
#include <unordered_map>
#include <vector>

enum ClauseType { SELECT, DECLARATION, SUCHTHAT, PATTERN };

enum Single {
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
  SYNONYM,
};

enum Pair { FOLLOWS, FOLLOWST, PARENT, PARENTT, USES, MODIFIES, SYN_ASSIGN };

union LoadType {
  Single single;
  Pair pair;
};

enum Tag { SINGLE, PAIR };

class PayLoad {
private:
  Tag tag;
  LoadType type;
  std::string value;

public:
  PayLoad(Tag loadTag, Single loadType, std::string loadVal);
  PayLoad(Tag loadTag, Pair loadType, std::string loadVal);
  LoadType getType();
  std::string getValue();
};

class QueryMap {
private:
  std::unordered_map<ClauseType, std::vector<PayLoad>> table;

public:
  QueryMap();
  std::vector<PayLoad> getList(ClauseType key);
  void addItem(ClauseType key, PayLoad item);
};
