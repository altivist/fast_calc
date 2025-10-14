#include "history_now_screen.hpp"

HistoryScreen::HistoryScreen(const Calc& calc)
    : calc_ref(calc) {}

Component HistoryScreen::get_component() {
    return Renderer([&] {
        Elements items;
        for (auto& line : calc_ref.get_history())
            items.push_back(text("> " + line));
        if (items.empty())
            items.push_back(text("История пуста.") | dim);

        return vbox(items) | border | flex;
    });
}
