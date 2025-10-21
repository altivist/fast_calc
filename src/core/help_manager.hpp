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


    void load() override;
    void save() override {}


    const std::vector<std::string> &get_lines() const;

private:
    std::filesystem::path help_path_;
    std::vector<std::string> lines_;


    std::filesystem::path executable_dir() const;
};
