#pragma once
#include "../common.hpp"

class TextScreen {
public:
    explicit TextScreen(const vector<string>& lines);
    void set_default_string(const string& value) { defaultstring = value; }

    Component get_component();
    virtual bool handle_event(int i);
protected:

    virtual void extra_handler(int i) {}
    virtual void update_lines() {}
    int scroll_position_ = 0;
    int max_visible_ = 40;
    vector<string> lines_;

    string defaultstring = "No lines";

    Element render_lines();
};
