#include "../src/core/file_manager.hpp"

#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <filesystem>
#include <random>
#include <system_error>

namespace fs = std::filesystem;

class TestFileManager : public FileManager {
public:
    void load() override {}
    void save() override {}

    fs::path expose_to_path(const std::string& path) const {
        return to_path(path);
    }
};

static std::string UniqueSuffix() {
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937_64 rng(static_cast<unsigned long>(now));
    return std::to_string(rng());
}

static fs::path MakeTempDir() {
    return fs::temp_directory_path() / "fast_calc_file_manager_tests" / UniqueSuffix();
}

struct TempDirGuard {
    explicit TempDirGuard(const fs::path& target) : path(target) {}
    ~TempDirGuard() {
        std::error_code ec;
        fs::remove_all(path, ec);
    }

    fs::path path;
};

TEST_CASE("create_dir creates nested directories and is idempotent", "[FileManager]") {
    TestFileManager fm;
    auto base_dir = MakeTempDir();
    TempDirGuard cleanup(base_dir);
    auto base_dir_str = base_dir.string();

    CHECK_FALSE(fm.exists(base_dir_str));
    REQUIRE(fm.create_dir(base_dir_str));
    REQUIRE(fm.exists(base_dir_str));
    CHECK(fm.create_dir(base_dir_str));
}

TEST_CASE("write_file persists data and read_file retrieves it", "[FileManager]") {
    TestFileManager fm;
    auto base_dir = MakeTempDir();
    TempDirGuard cleanup(base_dir);
    auto base_dir_str = base_dir.string();

    REQUIRE(fm.create_dir(base_dir_str));

    auto file_path = base_dir / "test.txt";
    auto file_str = file_path.string();

    CHECK(fm.read_file(file_str).empty());
    REQUIRE(fm.write_file(file_str, "hello world"));
    REQUIRE(fm.exists(file_str));
    CHECK(fm.read_file(file_str) == "hello world");

    auto nested_path = base_dir / "nested" / "dir" / "file.txt";
    auto nested_str = nested_path.string();

    REQUIRE(fm.write_file(nested_str, "nested data"));
    REQUIRE(fm.exists(nested_path.parent_path().string()));
    REQUIRE(fm.exists(nested_str));
    CHECK(fm.read_file(nested_str) == "nested data");
}

TEST_CASE("remove deletes files and directories recursively", "[FileManager]") {
    TestFileManager fm;
    auto base_dir = MakeTempDir();
    TempDirGuard cleanup(base_dir);

    auto nested_file = base_dir / "nested" / "dir" / "file.txt";
    auto nested_dir_str = (base_dir / "nested").string();

    REQUIRE(fm.write_file(nested_file.string(), "data"));
    REQUIRE(fm.exists(nested_file.string()));

    SECTION("Removing file clears it and leaves parent directory") {
        REQUIRE(fm.remove(nested_file.string()));
        CHECK_FALSE(fm.exists(nested_file.string()));
        CHECK(fm.exists(nested_dir_str));
    }

    SECTION("Removing directory clears all contents") {
        REQUIRE(fm.remove(nested_dir_str));
        CHECK_FALSE(fm.exists(nested_dir_str));
    }

    SECTION("Removing missing targets returns false") {
        REQUIRE(fm.remove(nested_dir_str));
        CHECK_FALSE(fm.remove(nested_dir_str));
    }
}

TEST_CASE("to_path normalizes relative inputs to absolute paths", "[FileManager]") {
    TestFileManager fm;

    auto absolute_input = fs::temp_directory_path();
    auto absolute_result = fm.expose_to_path(absolute_input.string());
    REQUIRE(absolute_result.is_absolute());
    CHECK(absolute_result == absolute_input);

    auto relative_result = fm.expose_to_path("relative_file.txt");
    REQUIRE(relative_result.is_absolute());
    CHECK(relative_result.filename() == fs::path("relative_file.txt"));
    CHECK(relative_result == fs::absolute("relative_file.txt"));
}
