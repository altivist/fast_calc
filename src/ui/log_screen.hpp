#pragma once
#include "../common.hpp"
#include "text_screen.hpp"
#include "../core/history_manager.hpp"
#include "../core/localization.hpp"

class LogScreen: public TextScreen {
    private:
    HistoryManager& hismg;
    LocalizationManager* localization_;
    protected:
    void update_lines() override;

    public:
    LogScreen(HistoryManager& manager, LocalizationManager* localization);
    ~LogScreen() = default;

};
