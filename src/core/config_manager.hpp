#pragma once
#include "file_manager.hpp"
#include "../common.hpp"

class ConfigManager : public FileManager {
public:
    explicit ConfigManager(const string& config_path);

    void load() override;
    void save() override;

    void set_locale(const string& locale);
    string get_locale() const;

    void set_color(const string& element, const string& color);
    string get_color(const string& element) const;

    void set_key(const string& action, const string& key);
    string get_key(const string& action) const;

private:
    string config_file_path_;
    toml::table config_data_;
};
