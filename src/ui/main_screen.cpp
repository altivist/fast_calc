#include "main_screen.hpp"
#include <cmath>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>

MainScreen::MainScreen(function<double(const string&)> evaluator)
    : eval_fn(move(evaluator)) {}

void MainScreen::Run() {

    vector<string> tabs = {"Калькулятор", "Помощь", "О программе"};
    Component tab_toggle = Toggle(&tabs, &current_tab);
    HistoryScreen history(calc);
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
                history.handle_event(1);
                current_tab = 0; // Возвращаемся в калькулятор
            }
            return true;
        }
        return false;
    });

    std::vector<std::string> help_text;
        for (int i = 1; i <= 90; ++i)
            help_text.push_back("Line " + std::to_string(i));

    TextScreen all_story(help_text);
    TextScreen help(help_text);


    Component container = Container::Vertical({
        tab_toggle,
        input_box
    });
    // Подвесить на контейнер обработку нажатий, написать хендлер в который попадает чаптер и листается объект, написать в классе туда сюда штучки и
    // перевести классы объектов в хуйню какую-то
    int blatmodule = 0;
    container |= CatchEvent([&](Event e) {
        if (e == Event::CtrlQ) {
            screen.Exit();
            return true;
        }


        if (e == Event::ArrowUpCtrl){
            blatmodule = 0;
            switch (current_tab) {
                case 0: {
                    history.handle_event(blatmodule);
                    break;
                }
                case 1: {
                    help.handle_event(blatmodule);
                    break;
                }
                case 2:{
                    all_story.handle_event(blatmodule);
                    break;
                }
                default:{

                    break;
                }
            }

            return true;
        }

        if (e == Event::ArrowDownCtrl){

            blatmodule = 1;

            switch (current_tab) {
                case 0: {

                    history.handle_event(blatmodule);
                    break;
                }
                case 1: {

                    help.handle_event(blatmodule);
                    break;
                }
                case 2:{

                    all_story.handle_event(blatmodule);
                    break;
                }
                default:{

                    break;
                }
            }


            return true;
        }

        if (e == Event::ArrowLeftCtrl && current_tab == 0){

            history.handle_event(2);

            return true;
        }

        if (e == Event::ArrowRightCtrl && current_tab == 0){

            history.handle_event(3);

            return true;
        }

        if (e == Event::CtrlR && current_tab == 0){

            history.handle_event(4);

            return true;
        }

        return false;
    });

    Component tabs_content = Container::Tab({
        history.get_component(),
        help.get_component(),
        all_story.get_component()
    }, &current_tab);


    Component renderer = Renderer(container, [&] {

        return vbox({
            text("FTXUI Калькулятор") | bold | center,
            separator(),
            tab_toggle->Render() | center,
            separator(),
            tabs_content ->Render() | flex,
            separator(),
            hbox({text("> "), input_box->Render()}) | border
        }) | flex;
    });

    screen.Loop(renderer);
}
