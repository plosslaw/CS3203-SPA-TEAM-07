#include "ResponseRenderer.h"
#include <cctype>

using namespace std;

ResponseRenderer::ResponseRenderer() {}

vector<string> ResponseRenderer::render_results(vector<string> query_results) {
    if (query_results.size() == 0) {
        return query_results;
    }
    
    // check if first string is a number
    if (!StringUtil::is_number(query_results[0])) {
        // non-integer string
        sort(query_results.begin(), query_results.end());
        return query_results;
    }
    sort(query_results.begin(), query_results.end(), ResponseRenderer::compare_int_strings);
    return query_results;

}


bool ResponseRenderer::compare_int_strings(string s1, string s2) {
    if (s1.size() != s2.size()) {
        return s1.size() < s2.size();
    }
    if (s1.size() <= 20) {
        return stol(s1) < stol(s2);
    }
    int a, b;
    for (int i = 0; i < s1.size(); i++) {
        a = stoi(s1.substr(i,1));
        b = stoi(s2.substr(i,1));
        if (a != b) {
            return a < b; 
        }
    }
    return false;
}
