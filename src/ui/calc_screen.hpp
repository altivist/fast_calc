#pragma once
#include "../common.hpp"
#include "../core/history_manager.hpp"


class Calc {
public:
    Calc(HistoryManager& manager) : manager_(manager){};
    void add_result(const string& expr, double result);
    const vector<string>& get_history() const;
    void add_result_exception(const string& except);
    HistoryManager& get_manager();

private:
    vector<string> history;
    HistoryManager& manager_;
};
