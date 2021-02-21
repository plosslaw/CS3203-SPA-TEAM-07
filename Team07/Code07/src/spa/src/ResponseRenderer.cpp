#include "ResponseRenderer.h"
#include <cctype>

using namespace std;

ResponseRenderer::ResponseRenderer() {
    this->digits_set.clear();
    for (int i = 0; i < 10; i++) {
        this->digits_set.insert(i);
    }
}

vector<string> ResponseRenderer::render_results(vector<string> query_results) {
    if (query_results.size() == 0) {
        return query_results;
    }
    
    // check if first string is a number
    if (!ResponseRenderer::check_is_number(query_results[0])) {
        // non-integer string
        sort(query_results.begin(), query_results.end());
        return query_results;
    }
    sort(query_results.begin(), query_results.end(), ResponseRenderer::compare_int_strings);
    return query_results;

}


bool ResponseRenderer::compare_int_strings(string s1, string s2) {
    return stol(s1) < stol(s2);
}

bool ResponseRenderer::check_is_number(string s) {
    if (s.empty() || (!isdigit(s[0]))) {
        return false;
    }

    const int BASE_TEN = 10;
    char *error;
    strtol(s.c_str(), &error, BASE_TEN);
    
    if (*error == 0) {
        return true;
    }
    return false;
}