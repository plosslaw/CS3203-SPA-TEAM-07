#pragma once
#include <exception>
#include <string>
#include <vector>
#include <utility>

class ParserMapper {
  private:
    std::string *source;
    std::vector<std::string> lines;
    std::vector<int> line_start;
  public:
    ParserMapper(std::string *str);
    int get_line(int i);
    int get_col(int line, int i);
    std::string get_str(int line);
    std::string get_caret(int col);
    std::string get_line_caret(int i, int sep = 0);
    std::string get_pos_print(int i, int sep = 0);
};