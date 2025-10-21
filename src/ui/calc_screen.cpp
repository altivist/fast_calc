#include "calc_screen.hpp"

void Calc::add_result(const string& expr, double result) {
    ostringstream ss;
    ss << expr << " = " << result;
    manager_.add_entry(ss.str());
    history.push_back(ss.str());
}

void Calc::add_result_exception(const string& expr) {
    ostringstream ss;
    ss << expr;
    history.push_back(ss.str());
}

HistoryManager& Calc::get_manager() {
    return manager_;
}


const vector<string>& Calc::get_history() const {
    return history;
}
