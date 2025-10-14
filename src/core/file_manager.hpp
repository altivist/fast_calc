#pragma once
#include <filesystem>
#include "../common.hpp"


class FileManager {
public:
    FileManager() = default;
    virtual ~FileManager() = default;

    // Проверка существования файла или папки
    bool exists(const std::string& path) const;

    // Создать папку (включая вложенные)
    bool create_dir(const std::string& path) const;

    // Удалить файл или папку
    bool remove(const std::string& path) const;

    // Считать весь файл в строку
    std::string read_file(const std::string& path) const;

    // Записать строку в файл (перезаписать)
    bool write_file(const std::string& path, const std::string& content) const;

    // Унифицированные методы для наследников, работающих с TOML
    virtual void load() = 0;
    virtual void save() = 0;

protected:
    std::filesystem::path to_path(const std::string& path) const;
};
