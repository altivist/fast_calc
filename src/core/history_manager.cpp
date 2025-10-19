#include "history_manager.hpp"

#include <sstream>

HistoryManager::HistoryManager(const string& history_file)
    : history_file_path_(to_path(history_file).string()) {}

void HistoryManager::load() {
    history_.clear();

    if (!exists(history_file_path_)) {
        auto parent = std::filesystem::path(history_file_path_).parent_path();
        if (!parent.empty()) {
            create_dir(parent.string());
        }
        return;
    }

    const auto content = read_file(history_file_path_);
    if (content.empty()) {
        return;
    }

    std::istringstream stream(content);
    string line;
    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (!line.empty()) {
            history_.push_back(line);
        }
    }
}

void HistoryManager::save() {
    std::ostringstream stream;
    for (size_t i = 0; i < history_.size(); ++i) {
        if (i != 0) {
            stream << '\n';
        }
        stream << history_[i];
    }

    write_file(history_file_path_, stream.str());
}

void HistoryManager::add_entry(const string& entry) {
    if (entry.empty()) {
        return;
    }
    history_.push_back(entry);
}

const vector<string>& HistoryManager::get_history() const {
    return history_;
}
