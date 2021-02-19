#pragma once
#include <string>
#include <unordered_map>
#include <vector>

enum class ClauseType { SELECT, DECLARATION, SUCHTHAT, PATTERN };

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
  INTEGER,
  WILDCARD,
  DOUBLE_QUOTE_IDENT,
  U_DQ_FACTOR, // underscore, double quote, factor 
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
  std::vector<bool> flag;

public:
  PayLoad(Tag loadTag, Single loadType, std::vector<std::string> loadVal);
  PayLoad(Tag loadTag, Pair loadType, std::vector<std::string> loadVal);
  PayLoad(Tag loadTag, Triple loadType, std::vector<std::string> loadVal);
  PayLoad(Tag loadTag, Single loadType, std::vector<std::string> loadVal, std::vector<bool> load_flags);
  PayLoad(Tag loadTag, Pair loadType, std::vector<std::string> loadVal, std::vector<bool> load_flags);
  PayLoad(Tag loadTag, Triple loadType, std::vector<std::string> loadVal, std::vector<bool> load_flags);

  Tag getTag();
  LoadType getType();
  std::vector<std::string> getValue();
  std::vector<bool> get_flag();
  friend bool operator==(const PayLoad &l, const PayLoad &r) {
    return (std::tie(l.tag, l.type.single, l.value, l.flag) ==
            std::tie(r.tag, r.type.single, r.value, r.flag)) ||
           (std::tie(l.tag, l.type.pair, l.value, l.flag) ==
            std::tie(r.tag, r.type.pair, r.value, r.flag)) ||
           (std::tie(l.tag, l.type.triple, l.value, l.flag) ==
            std::tie(r.tag, r.type.triple, r.value, r.flag));
  }
};

class QueryMap {
private:
  std::unordered_map<ClauseType, std::vector<PayLoad>> table;

public:
  QueryMap();
  QueryMap(std::vector<PayLoad> declaration_cl, std::vector<PayLoad> select_cl,
           std::vector<PayLoad> suchthat_cl, std::vector<PayLoad> pattern_cl);
  std::vector<PayLoad> getList(ClauseType key);
  void addItem(ClauseType key, PayLoad item);
  friend bool operator==(const QueryMap &l, const QueryMap &r) {
    return (l.table == r.table);
  }
};
