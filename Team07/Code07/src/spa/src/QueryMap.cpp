#include "QueryMap.h"

PayLoad::PayLoad(Tag loadTag, Single loadType, std::vector<std::string> loadVal) {
  tag = loadTag;
  type.single = loadType;
  value = loadVal;
  flag = std::vector<bool>{false};
}

PayLoad::PayLoad(Tag loadTag, Pair loadType, std::vector<std::string> loadVal) {
  tag = loadTag;
  type.pair = loadType;
  value = loadVal;
  flag = std::vector<bool>{false, false};
}

PayLoad::PayLoad(Tag loadTag, Triple loadType, std::vector<std::string> loadVal) {
  tag = loadTag;
  type.triple = loadType;
  value = loadVal;
  flag = std::vector<bool>{false, false, false};
}

PayLoad::PayLoad(Tag loadTag, Single loadType, std::vector<std::string> loadVal, std::vector<bool> load_flags) {
  tag = loadTag;
  type.single = loadType;
  value = loadVal;
  flag = load_flags;
}

PayLoad::PayLoad(Tag loadTag, Pair loadType, std::vector<std::string> loadVal, std::vector<bool> load_flags) {
  tag = loadTag;
  type.pair = loadType;
  value = loadVal;
  flag = load_flags;
}

PayLoad::PayLoad(Tag loadTag, Triple loadType, std::vector<std::string> loadVal, std::vector<bool> load_flags) {
  tag = loadTag;
  type.triple = loadType;
  value = loadVal;
  flag = load_flags;
}

Tag PayLoad::getTag() { return tag; }

LoadType PayLoad::getType() { return type; }

std::vector<std::string> PayLoad::getValue() { return value; }

QueryMap::QueryMap() {
  table[ClauseType::SELECT] = std::vector<PayLoad>();
  table[ClauseType::DECLARATION] = std::vector<PayLoad>();
  table[ClauseType::SUCHTHAT] = std::vector<PayLoad>();
  table[ClauseType::PATTERN] = std::vector<PayLoad>();
}

QueryMap::QueryMap(std::vector<PayLoad> declaration_cl,
                   std::vector<PayLoad> select_cl,
                   std::vector<PayLoad> suchthat_cl,
                   std::vector<PayLoad> pattern_cl) {
  table[ClauseType::DECLARATION] = declaration_cl;
  table[ClauseType::SELECT] = select_cl;
  table[ClauseType::SUCHTHAT] = suchthat_cl;
  table[ClauseType::PATTERN] = pattern_cl;
}

std::vector<PayLoad> QueryMap::getList(ClauseType key) { return table[key]; }

void QueryMap::addItem(ClauseType key, PayLoad item) {
  table[key].push_back(item);
}
