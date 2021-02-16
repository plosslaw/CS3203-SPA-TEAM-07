#pragma once
#include <string>
#include <unordered_map>
#include <vector>

enum ClauseType { SELECT, DECLARATION, SUCHTHAT, PATTERN };

enum class Single {
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

enum Pair { FOLLOWS, FOLLOWST, PARENT, PARENTT, USES, MODIFIES };

enum Triple { SYN_ASSIGN };

union LoadType {
  Single single;
  Pair pair;
  Triple triple;
};

enum Tag { SINGLE, PAIR, TRIPLE };

class PayLoad {
private:
  Tag tag;
  LoadType type;
  std::vector<std::string> value;

public:
  PayLoad(Tag loadTag, Single loadType, std::vector<std::string> loadVal);
  PayLoad(Tag loadTag, Pair loadType, std::vector<std::string> loadVal);
  PayLoad(Tag loadTag, Triple loadType, std::vector<std::string> loadVal);

  Tag getTag();
  LoadType getType();
  std::vector<std::string> getValue();
  friend bool operator==(const PayLoad &l, const PayLoad &r) {
    return (std::tie(l.tag, l.type.single, l.value) ==
            std::tie(r.tag, r.type.single, r.value)) ||
           (std::tie(l.tag, l.type.pair, l.value) ==
            std::tie(r.tag, r.type.pair, r.value)) ||
           (std::tie(l.tag, l.type.triple, l.value) ==
            std::tie(r.tag, r.type.triple, r.value));
  }
};

class QueryMap {
private:
  std::unordered_map<ClauseType, std::vector<PayLoad>> table;

public:
  QueryMap();
  std::vector<PayLoad> getList(ClauseType key);
  void addItem(ClauseType key, PayLoad item);
};
