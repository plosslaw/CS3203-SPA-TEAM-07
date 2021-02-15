#include "StringUtil.h"
#include <algorithm>

std::string StringUtil::removeWhiteSpaces(std::string str) {
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}