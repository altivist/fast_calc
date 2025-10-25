#include "log_screen.hpp"
#include "text_screen.hpp"


LogScreen::LogScreen(HistoryManager& manager, LocalizationManager* localization): TextScreen({}),  hismg(manager), localization_(localization){
    hismg.load();
    if (localization_) {
        defaultstring = localization_->get_text("history.empty", "History is empty.");
    } else {
        defaultstring = "History is empty.";
    }
};

void LogScreen::update_lines(){
    lines_ = hismg.get_history();
};
