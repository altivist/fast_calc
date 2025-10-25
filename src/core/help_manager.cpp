#include "help_manager.hpp"
#include <fstream>
#include <stdexcept>

#ifdef _WIN32
    #include <windows.h>
#elif __APPLE__
    #include <mach-o/dyld.h>
    #include <limits.h>
#else
    #include <unistd.h>
    #include <limits.h>
#endif

using namespace std;


filesystem::path HelpManager::executable_dir() const
{
#ifdef _WIN32
    char buffer[MAX_PATH];
    DWORD len = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    if (len == 0 || len == MAX_PATH)
        throw runtime_error("Не удалось получить путь к исполняемому файлу (Windows)");
    return filesystem::path(buffer).parent_path();

#elif __APPLE__
    char buffer[PATH_MAX];
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) != 0)
        throw runtime_error("Не удалось получить путь к исполняемому файлу (macOS)");


    char resolved[PATH_MAX];
    if (realpath(buffer, resolved) == nullptr)
        throw runtime_error("Не удалось разрешить путь к исполняемому файлу (macOS)");

    return filesystem::path(resolved).parent_path();

#else
    char buffer[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len == -1)
        throw runtime_error("Не удалось определить путь к исполняемому файлу (Linux)");
    buffer[len] = '\0';
    return filesystem::path(buffer).parent_path();
#endif
}

HelpManager::HelpManager(const string &help_filename)
{
    help_path_ = executable_dir() / help_filename;
}

void HelpManager::load()
{
    lines_.clear();

    if (!exists(help_path_.string()))
        throw runtime_error("Файл справки не найден: " + help_path_.string());

    ifstream file(help_path_);
    if (!file.is_open())
        throw runtime_error("Не удалось открыть файл справки: " + help_path_.string());

    string line;
    while (getline(file, line))
        lines_.push_back(line);
    lines_.push_back("\n");
}

const vector<string> &HelpManager::get_lines() const
{
    return lines_;
}
