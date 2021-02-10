#include "QueryMap.h"

QueryMap::QueryMap() {
  table[SELECT] = std::vector<std::string>();
  table[DECLARATION] = std::vector<std::string>();
  table[SUCHTHAT] = std::vector<std::string>();
  table[PATTERN] = std::vector<std::string>();
}
std::vector<std::string> QueryMap::getList(ClauseType key) {
  return table[key];
}

void QueryMap::addItem(ClauseType key, std::string item) {
  table[key].push_back(item);
}
