#include "log_screen.hpp"
#include "text_screen.hpp"


LogScreen::LogScreen(HistoryManager& manager): TextScreen({}),  hismg(manager){
    hismg.load();

};

void LogScreen::update_lines(){
    lines_ = hismg.get_history();
};
