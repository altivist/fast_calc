#include "text_screen.hpp"
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>


TextScreen::TextScreen(const vector<string>& lines)
    : lines_(lines) {}

    Element TextScreen::render_lines() {

        Elements visible;
        int end = std::min<int>(scroll_position_ + max_visible_, (int)lines_.size());
        for (int i = scroll_position_; i < end; ++i) {
            visible.push_back(text(lines_[i])); // без dim
        };

        if (lines_.empty()){
            visible.push_back(text(defaultstring) | dim);
        };


        return vbox(visible);
    }

Component TextScreen::get_component() {
    auto base = Renderer([this] {
        update_lines();             // обновляем строки перед рендером
        return render_lines() | border | flex;
    });

    return base;
};

bool TextScreen::handle_event(int i) {


    switch (i) {
    case 0: {if (scroll_position_ > 0) scroll_position_--; break;}
    case 1: {if (scroll_position_ + max_visible_ < (int)lines_.size()) scroll_position_++; break;}
    default:{ extra_handler(i); break;}
    }
    return true;

    }
