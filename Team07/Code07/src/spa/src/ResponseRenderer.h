#pragma once

#include <string>
#include <vector>

class ResponseRenderer {
public:
    explicit ResponseRenderer();

    std::vector<std::string> render_results(std::vector<std::string>);

private:
    bool check_if_results_integer(std::vector<std::string>);
};