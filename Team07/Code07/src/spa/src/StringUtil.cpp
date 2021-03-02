#include "StringUtil.h"
#include <string> 
#include <algorithm>

string StringUtil::remove_white_spaces(string str) {
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

bool StringUtil::is_number(string s) {
    return s.find_first_not_of("0123456789") == string::npos;
}