#pragma once
#include "../common.hpp"
#include "text_screen.hpp"
#include "../core/history_manager.hpp"


class LogScreen: public TextScreen {
    private:
    HistoryManager hismg = HistoryManager();
    vector<string> history_;


    public:
    LogScreen(): TextScreen({}){

        hismg.load();


    }

};
