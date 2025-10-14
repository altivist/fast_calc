#pragma once
#include "file_manager.hpp"
#include "../common.hpp"

class ConfigManager : public FileManager {
public:
    explicit ConfigManager(const std::string& config_path);

    void load() override;
    void save() override;

    // Работа с локалью
    void set_locale(const std::string& locale);
    std::string get_locale() const;

    // Работа с цветами элементов
    void set_color(const std::string& element, const std::string& color);
    std::string get_color(const std::string& element) const;

    // Работа с горячими клавишами
    void set_key(const std::string& action, const std::string& key);
    std::string get_key(const std::string& action) const;

private:
    std::string config_file_path_;
    toml::table config_data_;
};
