/**
 * @file config.hpp
 * @brief Lightweight INI/TOML-like configuration file parser.
 *
 * Format:
 *   # comment
 *   key = value
 *   key = "quoted value"
 *   key = ["a", "b", "c"]
 *   [section]
 *   key = value
 */

#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace dev {

namespace fs = std::filesystem;

class Config
{
public:
    using Map = std::unordered_map<std::string, std::string>;
    using List = std::unordered_map<std::string, std::vector<std::string>>;

    /// Get a scalar value.  Returns fallback if not found.
    [[nodiscard]] std::string
    get(const std::string& section, const std::string& key, const std::string& fallback = {}) const
    {
        auto full = section.empty() ? key : section + "." + key;
        auto it = values_.find(full);
        return (it != values_.end()) ? it->second : fallback;
    }

    /// Get an array value.  Returns empty vector if not found.
    [[nodiscard]] std::vector<std::string> get_list(const std::string& section,
                                                    const std::string& key) const
    {
        auto full = section.empty() ? key : section + "." + key;
        auto it = lists_.find(full);
        return (it != lists_.end()) ? it->second : std::vector<std::string>{};
    }

    /// Get all key-value pairs inside a section.
    [[nodiscard]] std::unordered_map<std::string, std::string>
    get_section(const std::string& section) const
    {
        std::unordered_map<std::string, std::string> result;
        std::string prefix = section + ".";
        for (const auto& [k, v] : values_) {
            if (k.starts_with(prefix)) {
                result[k.substr(prefix.size())] = v;
            }
        }
        return result;
    }

    /// Whether any configuration was loaded.
    [[nodiscard]] bool empty() const
    {
        return values_.empty() && lists_.empty();
    }

    /// Path of the loaded config file (empty if none).
    [[nodiscard]] const fs::path& path() const
    {
        return path_;
    }

    // ── Factory ─────────────────────────────────────────────

    /// Parse a config file.  Returns an empty Config on failure.
    static Config load(const fs::path& filepath)
    {
        Config cfg;
        std::ifstream ifs(filepath);
        if (!ifs.is_open())
            return cfg;

        cfg.path_ = filepath;
        std::string section;
        std::string line;

        while (std::getline(ifs, line)) {
            // Strip \r (Windows line endings)
            if (!line.empty() && line.back() == '\r')
                line.pop_back();

            // Trim leading whitespace
            auto start = line.find_first_not_of(" \t");
            if (start == std::string::npos)
                continue;
            line = line.substr(start);

            // Skip comments and empty lines
            if (line.empty() || line[0] == '#')
                continue;

            // Section header: [name]
            if (line.front() == '[' && line.back() == ']') {
                section = line.substr(1, line.size() - 2);
                continue;
            }

            // Key = value
            auto eq = line.find('=');
            if (eq == std::string::npos)
                continue;

            auto key = trim(line.substr(0, eq));
            auto val = trim(line.substr(eq + 1));
            auto full_key = section.empty() ? key : section + "." + key;

            // Array: ["a", "b", "c"]
            if (val.starts_with("[")) {
                cfg.lists_[full_key] = parse_array(val);
            } else {
                cfg.values_[full_key] = unquote(val);
            }
        }
        return cfg;
    }

    /// Search for config file in standard locations.
    /// Order: ./dev.toml → global config dir.
    static Config find(const char* argv0 = nullptr)
    {
        // 1. Project-local
        if (fs::exists("dev.toml")) {
            return load("dev.toml");
        }

        // 1b. Exe-relative
        if (argv0) {
            auto exe_dir = fs::weakly_canonical(fs::path(argv0)).parent_path();
            auto p = exe_dir / "dev.toml";
            if (fs::exists(p))
                return load(p);
        }

        // 2. Global config
        auto global = global_config_path();
        if (fs::exists(global)) {
            return load(global);
        }

        return {}; // No config found
    }

private:
    Map values_;
    List lists_;
    fs::path path_;

    static std::string trim(std::string_view s)
    {
        auto a = s.find_first_not_of(" \t\"");
        auto b = s.find_last_not_of(" \t\"");
        if (a == std::string_view::npos)
            return {};
        return std::string(s.substr(a, b - a + 1));
    }

    static std::string unquote(std::string_view s)
    {
        auto a = s.find_first_not_of(" \t");
        auto b = s.find_last_not_of(" \t");
        if (a == std::string_view::npos)
            return {};
        s = s.substr(a, b - a + 1);
        if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
            s = s.substr(1, s.size() - 2);
        }
        return std::string(s);
    }

    static std::vector<std::string> parse_array(std::string_view s)
    {
        std::vector<std::string> result;
        // Strip [ ]
        auto a = s.find('[');
        auto b = s.rfind(']');
        if (a == std::string_view::npos || b == std::string_view::npos)
            return result;
        s = s.substr(a + 1, b - a - 1);

        // Split by comma, unquote each element
        std::string token;
        for (char c : s) {
            if (c == ',') {
                auto t = unquote(token);
                if (!t.empty())
                    result.push_back(std::move(t));
                token.clear();
            } else {
                token += c;
            }
        }
        auto t = unquote(token);
        if (!t.empty())
            result.push_back(std::move(t));
        return result;
    }

    static fs::path global_config_path()
    {
#ifdef _WIN32
        const char* appdata = std::getenv("APPDATA");
        if (appdata)
            return fs::path(appdata) / "dev" / "config.toml";
        return {};
#else
        const char* home = std::getenv("HOME");
        if (home)
            return fs::path(home) / ".config" / "dev" / "config.toml";
        return {};
#endif
    }
};

} // namespace dev
