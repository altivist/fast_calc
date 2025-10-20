#pragma once
#include "../common.hpp"
#include "text_screen.hpp"
#include "../core/history_manager.hpp"


class LogScreen: public TextScreen {
    private:
    HistoryManager hismg = HistoryManager("заглушка, поменять, когда необходимо");
    vector<string> history_;


    public:
    LogScreen(): TextScreen({}){

        hismg.load();


    }

};
