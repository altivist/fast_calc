#pragma once
#include <filesystem>
#include "../common.hpp"

class FileManager
{
public:
    FileManager() = default;
    virtual ~FileManager() = default;

    // Проверка существования файла или папки
    bool exists(const string &path) const;

    // Создать папку (включая вложенные)
    bool create_dir(const string &path) const;

    // Удалить файл или папку
    bool remove(const string &path) const;

    // Считать весь файл в строку
    string read_file(const string &path) const;

    // Записать строку в файл (перезаписать)
    bool write_file(const string &path, const string &content) const;

    // Унифицированные методы для наследников, работающих с TOML
    virtual void load() = 0;
    virtual void save() = 0;

protected:
    std::filesystem::path to_path(const string &path) const;
};
