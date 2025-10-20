#pragma once
#include "../common.hpp"

class Calc {
public:
    void add_result(const string& expr, double result);
    const vector<string>& get_history() const;
    void add_result_exception(const string& except);

private:
    vector<string> history;
};
