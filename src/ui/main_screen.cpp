#include "main_screen.hpp"
#include <cmath>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>

MainScreen::MainScreen(function<double(const string&)> evaluator)
    : eval_fn(move(evaluator)) {}

void MainScreen::Run() {

    vector<string> tabs = {"Калькулятор", "Помощь", "О программе"};
    Component tab_toggle = Toggle(&tabs, &current_tab);

    Component input_box = Input(&input, "Введите выражение...");

    input_box |= CatchEvent([&](Event e) {
        if (e == Event::Return || (e.is_character() && e.character() == "=")) {
            if (!input.empty()) {
                try {
                    double res = eval_fn(input);
                    calc.add_result(input, res);
                } catch (...) {
                    calc.add_result(input, nan(""));
                }
                input.clear();
                current_tab = 0; // Возвращаемся в калькулятор
            }
            return true;
        }
        return false;
    });

    std::vector<std::string> help_text;
        for (int i = 1; i <= 20; ++i)
            help_text.push_back("Line " + std::to_string(i));

    TextScreen all_story(help_text);
    TextScreen help(help_text);
    HistoryScreen history(calc);


    Component container = Container::Vertical({
        tab_toggle,
        input_box
    });

    container |= CatchEvent([&](Event e) {
        if (e == Event::CtrlQ) {
            screen.Exit();
            return true;
        }
        return false;
    });

    Component renderer = Renderer(container, [&] {
        Component content;


        if (current_tab == 0)
            content = history.get_component();
        else if (current_tab == 1)
            content = help.get_component();
        else
            content = all_story.get_component();

        return vbox({
            text("FTXUI Калькулятор") | bold | center,
            separator(),
            tab_toggle->Render() | center,
            separator(),
            content->Render() | flex,
            separator(),
            hbox({text("> "), input_box->Render()}) | border
        }) | flex;
    });

    screen.Loop(renderer);
}
