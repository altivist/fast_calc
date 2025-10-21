#pragma once
#include "../common.hpp"
#include "text_screen.hpp"
#include "../core/history_manager.hpp"


class LogScreen: public TextScreen {
    private:
    HistoryManager& hismg;
    protected:
    void update_lines() override;

    public:
    LogScreen(HistoryManager& manager);
    ~LogScreen() = default;

};
