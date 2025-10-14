#pragma once
#include "../common.hpp"
#include "calc_screen.hpp"

class HistoryScreen {
public:
    HistoryScreen(const Calc& calc);
    Component get_component();

private:
    const Calc& calc_ref;
};
