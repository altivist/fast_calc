#pragma once
#include "file_manager.hpp"
#include "../common.hpp"

class HistoryManager : public FileManager
{
public:
    explicit HistoryManager(const string &history_file = "history");

    void load() override;
    void save() override;

    void add_entry(const string &entry);
    const vector<string> &get_history() const;

private:
    string history_file_path_;
    vector<string> history_;
};
