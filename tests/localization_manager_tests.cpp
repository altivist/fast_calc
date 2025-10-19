#include "../src/core/localization.hpp"

#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <random>
#include <vector>

namespace fs = std::filesystem;

static std::string UniqueSuffix()
{
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937_64 rng(static_cast<unsigned long>(now));
    return std::to_string(rng());
}

static fs::path MakeTempDir()
{
    return fs::temp_directory_path() / "fast_calc_localization_tests" / UniqueSuffix();
}

struct TempDirGuard
{
    explicit TempDirGuard(const fs::path &target) : path(target) {}
    ~TempDirGuard()
    {
        std::error_code ec;
        fs::remove_all(path, ec);
    }

    fs::path path;
};

static void WriteFile(const fs::path &path, const std::string &content)
{
    if (const auto parent = path.parent_path(); !parent.empty())
    {
        fs::create_directories(parent);
    }
    std::ofstream file(path);
    file << content;
}

TEST_CASE("LocalizationManager loads TOML locale files and flattens structure", "[LocalizationManager]")
{
    const auto base_dir = MakeTempDir();
    TempDirGuard cleanup(base_dir);

    WriteFile(base_dir / "en.toml",
              R"([general]
greeting = "Hello"

[menu]
items = ["File", "Edit"]
)");

    LocalizationManager manager(base_dir.string());
    REQUIRE(manager.load_locale("en"));
    CHECK(manager.current_locale() == "en");
    CHECK(manager.get_text("general.greeting") == "Hello");
    CHECK(manager.get_text("menu.items.0") == "File");
    CHECK(manager.get_text("menu.items.1") == "Edit");
    CHECK(manager.get_text("missing.key", "Default") == "Default");
}

TEST_CASE("LocalizationManager discovers locales in files and directories", "[LocalizationManager]")
{
    const auto base_dir = MakeTempDir();
    TempDirGuard cleanup(base_dir);

    WriteFile(base_dir / "en.toml",
              R"([texts]
welcome = "Welcome"
)");
    WriteFile(base_dir / "ru" / "common.toml",
              R"([texts]
welcome = "Privet"
)");

    LocalizationManager manager(base_dir.string());

    auto locales = manager.available_locales();
    std::vector<std::string> expected{"en", "ru"};
    CHECK(locales == expected);

    REQUIRE(manager.load_locale("ru"));
    CHECK(manager.current_locale() == "ru");
    CHECK(manager.get_text("texts.welcome") == "Privet");
}
