/**
 * @file dispatcher.hpp
 * @brief Plugin discovery, listing, and dispatch — with multi-path support.
 */

#pragma once

#include "dev/config.hpp"
#include "dev/error.hpp"
#include "dev/process.hpp"
#include "dev/style.hpp"

#include <algorithm>
#include <filesystem>
#include <print>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace dev {

namespace fs = std::filesystem;

/// Collect all plugin search directories (exe-relative + cwd + config).
inline std::vector<fs::path> find_all_plugin_dirs(const char* argv0, const Config& cfg = {})
{

    std::vector<fs::path> dirs;

    // 1. Exe-relative
    auto exe = fs::weakly_canonical(fs::path(argv0));
    auto exe_dir = exe.parent_path() / "plugins";
    if (fs::is_directory(exe_dir)) {
        dirs.push_back(exe_dir);
    }

    // 2. Cwd-relative
    auto cwd_dir = fs::current_path() / "plugins";
    if (fs::is_directory(cwd_dir) && cwd_dir != exe_dir) {
        dirs.push_back(cwd_dir);
    }

    // 3. Config-defined
    for (const auto& p : cfg.get_list("plugins", "dirs")) {
        fs::path dir(p);
        // Expand ~ on POSIX
#ifndef _WIN32
        if (p.starts_with("~/")) {
            const char* home = std::getenv("HOME");
            if (home)
                dir = fs::path(home) / p.substr(2);
        }
#endif
        if (fs::is_directory(dir)) {
            dirs.push_back(dir);
        }
    }

    // Fallback: if nothing found, include cwd/plugins anyway
    if (dirs.empty()) {
        dirs.push_back(fs::current_path() / "plugins");
    }

    return dirs;
}

/// Backwards-compatible single-dir helper.
inline fs::path find_plugins_dir(const char* argv0)
{
    auto dirs = find_all_plugin_dirs(argv0);
    return dirs.empty() ? (fs::current_path() / "plugins") : dirs.front();
}

/// Resolve a command name to its plugin executable path.
/// Searches a single directory.
inline fs::path resolve_plugin(std::string_view command, const fs::path& dir)
{
#ifdef _WIN32
    fs::path path = dir / (std::string(command) + ".exe");
#else
    fs::path path = dir / std::string(command);
#endif
    if (fs::exists(path) && fs::is_regular_file(path)) {
        return path;
    }
    return {};
}

/// Resolve a command across multiple directories.
inline fs::path resolve_plugin(std::string_view command, const std::vector<fs::path>& dirs)
{
    for (const auto& dir : dirs) {
        auto p = resolve_plugin(command, dir);
        if (!p.empty())
            return p;
    }
    return {};
}

/// Return a sorted, deduplicated list of plugin names from one dir.
inline std::vector<std::string> list_plugins(const fs::path& dir)
{
    std::vector<std::string> names;
    if (!fs::is_directory(dir))
        return names;

    for (const auto& entry : fs::directory_iterator(dir)) {
        if (!entry.is_regular_file())
            continue;
        names.push_back(entry.path().stem().string());
    }
    std::sort(names.begin(), names.end());
    return names;
}

/// Return a sorted, deduplicated list from multiple directories.
inline std::vector<std::string> list_plugins(const std::vector<fs::path>& dirs)
{
    std::set<std::string> seen;
    for (const auto& dir : dirs) {
        for (const auto& name : list_plugins(dir)) {
            seen.insert(name);
        }
    }
    return {seen.begin(), seen.end()};
}

/// Dispatch a command to its plugin, searching across all dirs.
inline int dispatch(int argc, char* argv[], const std::vector<fs::path>& dirs)
{
    if (argc < 2) {
        return static_cast<int>(Error::InvalidUsage);
    }

    std::string_view command = argv[1];
    fs::path plugin = resolve_plugin(command, dirs);

    if (plugin.empty()) {
        std::println(stderr, "dev: command '{}' not found", command);
        std::println(stderr, "  searched in:");
        for (const auto& d : dirs) {
            std::println(stderr, "    {}", d.string());
        }
        return static_cast<int>(Error::CommandNotFound);
    }

    return spawn(plugin, argc, argv);
}

/// Legacy overload — single implicit dir.
inline int dispatch(int argc, char* argv[])
{
    auto dirs = find_all_plugin_dirs(argv[0]);
    return dispatch(argc, argv, dirs);
}

} // namespace dev
