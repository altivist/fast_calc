#pragma once
#include "file_manager.hpp"
#include <filesystem>
#include <string>
#include <vector>

class HelpManager : public FileManager
{
public:
    explicit HelpManager(const std::string &help_filename = "manual.txt");
    ~HelpManager() override = default;

    // Загружает содержимое help-файла
    void load() override;

    // Не используется, но требуется базовым интерфейсом
    void save() override {}

    // Получить построчное содержимое help-файла
    const std::vector<std::string> &get_lines() const;

private:
    std::filesystem::path help_path_;
    std::vector<std::string> lines_;

    // Определить путь к директории исполняемого файла
    std::filesystem::path executable_dir() const;
};
