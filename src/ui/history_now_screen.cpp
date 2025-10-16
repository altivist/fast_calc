#include "history_now_screen.hpp"

HistoryScreen::HistoryScreen(const Calc& calc)
    : TextScreen({}), calc_ref(calc) {
    lines_ = make_string();
    defaultstring = "История пуста.";
}

void HistoryScreen::update_lines() {
    lines_ = make_string();
}
const vector<string> HistoryScreen::make_string() {

        vector<string> items;
        for (const auto& line : calc_ref.get_history()) {
            if (max_line_ < line.size())
                max_line_ = line.size();
        }

        for (const auto& line : calc_ref.get_history()) {
            std::string text = line;
            if (text.size() < max_line_)
                text += std::string(max_line_ - text.size(), ' ');

            if ((size_t)offset_ < text.size())
                text = text.substr(offset_);
            else
                text.clear();

            items.push_back("> " + text);
        }

        return items;
}


void HistoryScreen::extra_handler(int i){


    switch (i) {
        case 2: { if (offset_ < max_line_ - visability_max_) offset_++; break;}
        case 3: {if (offset_ > 0) offset_--; break;}
        case 4: {offset_ = 0; break;}
        default:{ break;}
    }

}
