#pragma once
#include "../common.hpp"
#include "calc_screen.hpp"
#include "text_screen.hpp"
#include "history_now_screen.hpp"
#include "../core/localization.hpp"



class MainScreen {
public:
    explicit MainScreen(function<double(const string&)> evaluator,
                        LocalizationManager localization = LocalizationManager("lang"));
    void Run();

private:
    ScreenInteractive screen = ScreenInteractive::Fullscreen();
    Calc calc;
    function<double(const string&)> eval_fn;
    LocalizationManager localization_;

    int current_tab = 0;
    string input;
};
