#include "calc_screen.hpp"

void Calc::add_result(const string& expr, double result) {
    ostringstream ss;
    ss << expr << " = " << result;
    history.push_back(ss.str());
}

const vector<string>& Calc::get_history() const {
    return history;
}
