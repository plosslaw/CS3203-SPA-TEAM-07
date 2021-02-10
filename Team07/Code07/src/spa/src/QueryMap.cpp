#include "QueryMap.h"

PayLoad::PayLoad(Tag loadTag, Single loadType, std::string loadVal) {
  tag = loadTag;
  type.single = loadType;
  value = loadVal;
}
PayLoad::PayLoad(Tag loadTag, Pair loadType, std::string loadVal) {
  tag = loadTag;
  type.pair = loadType;
  value = loadVal;
}

LoadType PayLoad::getType() { return type; }
std::string PayLoad::getValue() { return value; }

QueryMap::QueryMap() {
  table[SELECT] = std::vector<PayLoad>();
  table[DECLARATION] = std::vector<PayLoad>();
  table[SUCHTHAT] = std::vector<PayLoad>();
  table[PATTERN] = std::vector<PayLoad>();
}

std::vector<PayLoad> QueryMap::getList(ClauseType key) { return table[key]; }

void QueryMap::addItem(ClauseType key, PayLoad item) {
  table[key].push_back(item);
}
