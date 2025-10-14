#pragma once
#include "../common.hpp"

class Calc {
public:
    void add_result(const string& expr, double result);
    const vector<string>& get_history() const;

private:
    vector<string> history;
};
