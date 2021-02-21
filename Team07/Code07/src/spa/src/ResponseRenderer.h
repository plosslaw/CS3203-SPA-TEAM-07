#pragma once

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
    static bool check_is_number(std::string s);
    std::unordered_set<int> digits_set;
};