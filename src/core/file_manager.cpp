#include "file_manager.hpp"

namespace fs = std::filesystem;

bool FileManager::exists(const string& path) const {
    try {
        return fs::exists(to_path(path));
    } catch (const fs::filesystem_error&) {
        return false;
    }
}

bool FileManager::create_dir(const string& path) const {
    try {
        auto target = to_path(path);
        if (fs::exists(target)) {
            return fs::is_directory(target);
        }
        return fs::create_directories(target);
    } catch (const fs::filesystem_error&) {
        return false;
    }
}

bool FileManager::remove(const string& path) const {
    try {
        auto target = to_path(path);
        if (!fs::exists(target)) {
            return false;
        }
        return fs::remove_all(target) > 0;
    } catch (const fs::filesystem_error&) {
        return false;
    }
}

string FileManager::read_file(const string& path) const {
    auto target = to_path(path);
    std::ifstream file(target, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        return {};
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool FileManager::write_file(const string& path, const string& content) const {
    try {
        auto target = to_path(path);
        auto parent = target.parent_path();
        if (!parent.empty() && !fs::exists(parent)) {
            fs::create_directories(parent);
        }

        std::ofstream file(target, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!file.is_open()) {
            return false;
        }

        file << content;
        return static_cast<bool>(file);
    } catch (const fs::filesystem_error&) {
        return false;
    }
}

fs::path FileManager::to_path(const string& path) const {
    fs::path converted(path);
    if (converted.is_absolute()) {
        return converted;
    }
    return fs::absolute(converted);
}
