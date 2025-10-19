#include "../core/config_manager.hpp"
#include "../common.hpp"


using namespace ftxui;

class SettingScreen {
public:
    // Конструктор, получает указатель на ConfigManager
    explicit SettingScreen(ConfigManager* cfg);

    // Возвращает FTXUI компонент для рендеринга экрана
    Component get_component();

private:
    ConfigManager* config_;

    // Варианты выбора
    std::vector<std::string> colors = {"black", "white"}; //добавить цветов и тд тп
    std::vector<std::string> locales = {"en", "ru"};

    int current_color_index_ = 0;
    int current_locale_index_ = 0;

    // Компоненты для выбора
    Component color_toggle_;
    Component locale_toggle_;

    // Инициализация интерактивных элементов
    void init_components();
};
