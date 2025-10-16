#pragma once
#include "../common.hpp"
#include "calc_screen.hpp"
#include "text_screen.hpp"

class HistoryScreen: public TextScreen {
public:
    HistoryScreen(const Calc& calc);
    const vector<string> make_string();
    bool handle_event_for_his(int i);
protected:
    void update_lines() override;
    void extra_handler(int i) override;
private:
    int offset_ = 0;
    int max_line_ = 0;
    int visability_max_ = 30;
    const Calc& calc_ref;
};
