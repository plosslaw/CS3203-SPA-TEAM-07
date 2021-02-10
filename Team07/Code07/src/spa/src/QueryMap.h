#include <string>
#include <unordered_map>
#include <vector>

enum ClauseType { SELECT, DECLARATION, SUCHTHAT, PATTERN };

class QueryMap {
private:
  std::unordered_map<ClauseType, std::vector<std::string>> table;

public:
  QueryMap();
  std::vector<std::string> getList(ClauseType key);
  void addItem(ClauseType key, std::string item);
};