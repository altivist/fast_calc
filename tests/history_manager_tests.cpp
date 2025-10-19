#include "../src/core/history_manager.hpp"

#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <random>
#include <system_error>

namespace fs = std::filesystem;

static std::string UniqueSuffix() {
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937_64 rng(static_cast<unsigned long>(now));
    return std::to_string(rng());
}

static fs::path TempHistoryFilePath() {
    return fs::temp_directory_path() / "fast_calc_history_manager_tests" / UniqueSuffix() / "history.log";
}

struct TempFileGuard {
    explicit TempFileGuard(const fs::path& target) : path(target) {}
    ~TempFileGuard() {
        std::error_code ec;
        fs::remove_all(path.parent_path(), ec);
    }

    fs::path path;
};

static std::vector<std::string> ReadLines(const fs::path& file) {
    std::vector<std::string> lines;
    std::ifstream input(file);
    std::string line;
    while (std::getline(input, line)) {
        lines.push_back(line);
    }
    return lines;
}

TEST_CASE("HistoryManager loads entries from existing file", "[HistoryManager]") {
    auto filepath = TempHistoryFilePath();
    TempFileGuard guard(filepath);
    auto parent = filepath.parent_path();
    std::error_code ec;
    fs::create_directories(parent, ec);

    {
        std::ofstream output(filepath);
        output << "first\nsecond\n\nthird\r\n";
    }

    HistoryManager manager(filepath.string());
    manager.load();
    auto history = manager.get_history();

    REQUIRE(history.size() == 3);
    CHECK(history[0] == "first");
    CHECK(history[1] == "second");
    CHECK(history[2] == "third");
}

TEST_CASE("HistoryManager save persists current history entries", "[HistoryManager]") {
    auto filepath = TempHistoryFilePath();
    TempFileGuard guard(filepath);

    HistoryManager manager(filepath.string());
    manager.add_entry("one");
    manager.add_entry("two");
    manager.add_entry("");
    manager.save();

    auto lines = ReadLines(filepath);
    REQUIRE(lines.size() == 2);
    CHECK(lines[0] == "one");
    CHECK(lines[1] == "two");
}

TEST_CASE("HistoryManager add_entry appends entries while ignoring empty strings", "[HistoryManager]") {
    auto filepath = TempHistoryFilePath();
    TempFileGuard guard(filepath);

    HistoryManager manager(filepath.string());
    manager.add_entry("");
    manager.add_entry("calc 1");
    manager.add_entry("");
    manager.add_entry("calc 2");
    auto history = manager.get_history();

    REQUIRE(history.size() == 2);
    CHECK(history[0] == "calc 1");
    CHECK(history[1] == "calc 2");
}

TEST_CASE("HistoryManager load creates parent directories when file is missing", "[HistoryManager]") {
    auto filepath = TempHistoryFilePath();
    TempFileGuard guard(filepath);
    auto parent = filepath.parent_path();

    HistoryManager manager(filepath.string());
    manager.load();
    auto history = manager.get_history();

    CHECK(history.empty());
    CHECK(fs::exists(parent));
}
