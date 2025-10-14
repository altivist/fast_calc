#pragma once
#include "file_manager.hpp"
#include "../common.hpp"


class LocalizationManager : public FileManager {
public:
    explicit LocalizationManager(const std::string& locale_dir);

    void load() override;  // Загружает текущую локаль
    void save() override;  // Можно сохранять кастомные тексты, если нужно

    bool load_locale(const std::string& locale_code);
    std::string get_text(const std::string& key, const std::string& default_text = "") const;
    std::vector<std::string> available_locales() const;

private:
    std::string locale_directory_;
    std::string current_locale_;
    std::unordered_map<std::string, std::string> texts_;
};
