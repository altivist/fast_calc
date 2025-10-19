#include "localization.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <sstream>

using namespace std::string_literals;

namespace {

using StringMap = std::unordered_map<std::string, std::string>;

std::string Lowercase(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}

void FlattenNode(const toml::node& node, const std::string& prefix, StringMap& out);

void FlattenArray(const toml::array& array, const std::string& prefix, StringMap& out) {
    for (std::size_t i = 0; i < array.size(); ++i) {
        const toml::node* element = array.get(i);
        if (!element) {
            continue;
        }
        std::string key = prefix.empty() ? std::to_string(i) : prefix + "." + std::to_string(i);
        FlattenNode(*element, key, out);
    }
}

void FlattenNode(const toml::node& node, const std::string& prefix, StringMap& out) {
    if (const auto* table = node.as_table()) {
        for (const auto& [child_key, child_node] : *table) {
            const std::string key = prefix.empty()
                                        ? child_key.str()
                                        : prefix + "." + child_key.str();
            FlattenNode(child_node, key, out);
        }
        return;
    }

    if (const auto* array = node.as_array()) {
        FlattenArray(*array, prefix, out);
        return;
    }

    if (const auto string_value = node.value<std::string>()) {
        out[prefix] = *string_value;
    } else if (const auto integer_value = node.value<int64_t>()) {
        out[prefix] = std::to_string(*integer_value);
    } else if (const auto float_value = node.value<double>()) {
        out[prefix] = std::to_string(*float_value);
    } else if (const auto bool_value = node.value<bool>()) {
        out[prefix] = *bool_value ? "true"s : "false"s;
    }
}

bool ParseTomlFile(const std::filesystem::path& path, StringMap& out) {
    try {
        const toml::table table = toml::parse_file(path.string());
        FlattenNode(table, "", out);
        return true;
    } catch (const toml::parse_error&) {
        return false;
    }
}

StringMap ParseLocalePath(const std::filesystem::path& locale_path) {
    StringMap collected;
    if (!std::filesystem::exists(locale_path)) {
        return collected;
    }

    if (std::filesystem::is_directory(locale_path)) {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(locale_path)) {
            if (!entry.is_regular_file()) {
                continue;
            }

            const auto extension = Lowercase(entry.path().extension().string());
            if (extension == ".toml") {
                ParseTomlFile(entry.path(), collected);
            }
        }
    } else if (std::filesystem::is_regular_file(locale_path)) {
        ParseTomlFile(locale_path, collected);
    }

    return collected;
}

void InsertFlatKey(toml::table& table, const std::string& flat_key, const std::string& value) {
    std::stringstream stream(flat_key);
    std::string segment;
    toml::table* current = &table;

    while (std::getline(stream, segment, '.')) {
        if (stream.peek() == EOF) {
            current->insert_or_assign(segment, value);
            break;
        }

        toml::table* next = current->get_as<toml::table>(segment);
        if (!next) {
            auto [it, _] = current->insert_or_assign(segment, toml::table{});
            next = it->second.as_table();
        }
        current = next;
    }
}

}  // namespace

LocalizationManager::LocalizationManager(const string& locale_dir)
    : locale_directory_(to_path(locale_dir).string()) {
    create_dir(locale_directory_);
}

void LocalizationManager::load() {
    if (current_locale_.empty()) {
        const auto locales = available_locales();
        if (!locales.empty()) {
            load_locale(locales.front());
        } else {
            texts_.clear();
        }
        return;
    }

    if (!load_locale(current_locale_)) {
        texts_.clear();
    }
}

void LocalizationManager::save() {
    if (current_locale_.empty() || texts_.empty()) {
        return;
    }

    std::filesystem::path override_path =
        std::filesystem::path(locale_directory_) / current_locale_ / "overrides.toml";
    const auto parent = override_path.parent_path();
    if (!parent.empty()) {
        create_dir(parent.string());
    }

    toml::table table;
    for (const auto& [key, value] : texts_) {
        InsertFlatKey(table, key, value);
    }

    std::stringstream serialized;
    serialized << table;
    write_file(override_path.string(), serialized.str());
}

bool LocalizationManager::load_locale(const string& locale_code) {
    const auto normalized_code = locale_code;

    std::filesystem::path locale_path = std::filesystem::path(locale_directory_) / normalized_code;
    if (!std::filesystem::exists(locale_path)) {
        locale_path = std::filesystem::path(locale_directory_) / (normalized_code + ".toml");
        if (!std::filesystem::exists(locale_path)) {
            return false;
        }
    }

    StringMap collected = ParseLocalePath(locale_path);
    if (collected.empty()) {
        return false;
    }

    current_locale_ = normalized_code;
    texts_ = std::move(collected);
    return true;
}

string LocalizationManager::get_text(const string& key, const string& default_text) const {
    const auto it = texts_.find(key);
    if (it != texts_.end()) {
        return it->second;
    }
    return default_text;
}

vector<string> LocalizationManager::available_locales() const {
    vector<string> locales;
    const std::filesystem::path dir(locale_directory_);
    if (!std::filesystem::exists(dir)) {
        return locales;
    }

    std::error_code ec;
    for (const auto& entry : std::filesystem::directory_iterator(dir, ec)) {
        if (ec) {
            break;
        }

        if (entry.is_directory()) {
            locales.emplace_back(entry.path().filename().string());
        } else if (entry.is_regular_file()) {
            const auto extension = Lowercase(entry.path().extension().string());
            if (extension == ".toml") {
                locales.emplace_back(entry.path().stem().string());
            }
        }
    }

    std::sort(locales.begin(), locales.end());
    locales.erase(std::unique(locales.begin(), locales.end()), locales.end());
    return locales;
}
