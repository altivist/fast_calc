#include "history_manager.hpp"

#include <cstdlib>
#include <filesystem>
#include <sstream>

namespace
{
    namespace fs = std::filesystem;

    fs::path ResolveConfigRoot()
    {
#if defined(_WIN32)
        if (const char *appdata = std::getenv("APPDATA"))
        {
            return fs::path(appdata);
        }
        if (const char *local = std::getenv("LOCALAPPDATA"))
        {
            return fs::path(local);
        }
#elif defined(__APPLE__)
        if (const char *home = std::getenv("HOME"))
        {
            return fs::path(home) / "Library" / "Application Support";
        }
#else
        if (const char *xdg = std::getenv("XDG_CONFIG_HOME"))
        {
            return fs::path(xdg);
        }
        if (const char *home = std::getenv("HOME"))
        {
            return fs::path(home) / ".config";
        }
#endif
        return fs::current_path();
    }

    bool LooksLikeFile(const fs::path &value)
    {
        return value.has_extension() ||
               (!value.empty() && value.filename().string().find('.') != std::string::npos);
    }

    fs::path ResolveHistoryPath(const string &history_file)
    {
        const fs::path provided(history_file);

        if (provided.empty())
        {
            return ResolveConfigRoot() / "history.log";
        }

        if (provided.is_absolute())
        {
            if (LooksLikeFile(provided))
            {
                return provided;
            }
            return provided / "history.log";
        }

        fs::path base = ResolveConfigRoot();
        if (LooksLikeFile(provided))
        {
            return base / provided;
        }

        return base / provided / "history.log";
    }
} // namespace

HistoryManager::HistoryManager(const string &history_file)
{
    namespace fs = std::filesystem;

    fs::path target = ResolveHistoryPath(history_file);

    if (!target.has_extension())
    {
        target += ".log";
    }

    history_file_path_ = fs::absolute(target).string();
}

void HistoryManager::load()
{
    history_.clear();

    if (!exists(history_file_path_))
    {
        auto parent = std::filesystem::path(history_file_path_).parent_path();
        if (!parent.empty())
        {
            create_dir(parent.string());
        }
        return;
    }

    const auto content = read_file(history_file_path_);
    if (content.empty())
    {
        return;
    }

    std::istringstream stream(content);
    string line;
    while (std::getline(stream, line))
    {
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        if (!line.empty())
        {
            history_.push_back(line);
        }
    }
}

void HistoryManager::save()
{
    std::ostringstream stream;
    for (size_t i = 0; i < history_.size(); ++i)
    {
        if (i != 0)
        {
            stream << '\n';
        }
        stream << history_[i];
    }

    write_file(history_file_path_, stream.str());
}

void HistoryManager::add_entry(const string &entry)
{
    if (entry.empty())
    {
        return;
    }
    history_.push_back(entry);
}

const vector<string> &HistoryManager::get_history() const
{
    return history_;
}
