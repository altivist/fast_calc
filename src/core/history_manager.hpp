#pragma once
#include "file_manager.hpp"
#include "../common.hpp"


class HistoryManager : public FileManager {
public:
    explicit HistoryManager(const std::string& history_file);

    void load() override;
    void save() override;

    void add_entry(const std::string& entry);
    const std::vector<std::string>& get_history() const;

private:
    std::string history_file_path_;
    std::vector<std::string> history_;
};
