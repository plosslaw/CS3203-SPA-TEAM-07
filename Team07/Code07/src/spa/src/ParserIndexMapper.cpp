#include <vector>
#include <string>
#include <utility>
#include "ParserIndexMapper.h"

ParserMapper::ParserMapper(std::string *str) {
  source = str;
  line_start.push_back(0);
  std::vector<char> linebuilder;
  for(int i = 0; i < (*str).size(); i++) {
    char c = (*str).at(i);
    if(c == '\n') {
      line_start.push_back(i + 1);
      lines.push_back(linebuilder.size() > 0 ? std::string(linebuilder.begin(), linebuilder.end()) : "");
      linebuilder.clear();
    } else {
      linebuilder.push_back(c);
    }
  }
  lines.push_back(linebuilder.size() > 0 ? std::string(linebuilder.begin(), linebuilder.end()) : "");
}

// binary search to find line
int ParserMapper::get_line(int i) {
  int r = line_start.size();
  int l = 0;
  while (r >= l) {
    int mid = l + (r - l) / 2;
    if (mid >= line_start.size()) return line_start.size() - 1;
    if (line_start[mid] == i) return mid;
    if (line_start[mid] > i) {
      r = mid - 1;
    } else {
      l = mid + 1;
    }
  }
  return r >= 0 && line_start[r] < i ? r : l;
}

int ParserMapper::get_col(int line, int i) {
  return i - line_start[line];
}

std::string ParserMapper::get_str(int line) {
  return lines[line];
}

std::string ParserMapper::get_caret(int col) {
  return std::string(col, ' ') + "^";
}

std::string ParserMapper::get_line_caret(int i, int sep) {
  int line = get_line(i);
  int col = get_col(line, i);
  std::string linenum(std::to_string(line + 1) + "| ");
  std::string sepstr(sep, ' ');
  return sepstr + linenum + get_str(line) + "\n" + sepstr + std::string(linenum.size(), ' ') + get_caret(col) + "\n";
}

std::string ParserMapper::get_pos_print(int i, int sep) {
  int line = get_line(i);
  int col = get_col(line, i);
  return std::string(sep, ' ') + "at line " + std::to_string(line + 1) + " col " + std::to_string(col + 1) + "\n";
}