#pragma once

#include "StringUtil.h"

#include <algorithm>
#include <string>
#include <unordered_set>
#include <vector>

class ResponseRenderer {
public:
    explicit ResponseRenderer();

    std::vector<std::string> render_results(std::vector<std::string> query_results);

private:
    static bool compare_int_strings(std::string s1, std::string s2);
};