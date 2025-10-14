#pragma once
#include "../common.hpp"

class TextScreen {
public:
    explicit TextScreen(const std::vector<std::string>& lines);

    Component get_component();

protected:

    int scroll_position_ = 0;
    int max_visible_ = 10;
    std::vector<std::string> lines_;


    Element render_lines();
};
