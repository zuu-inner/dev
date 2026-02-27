/**
 * @file dispatcher.hpp
 * @brief Plugin discovery, listing, and dispatch.
 */

#pragma once

#include "dev/error.hpp"
#include "dev/process.hpp"

#include <algorithm>
#include <filesystem>
#include <print>
#include <string>
#include <string_view>
#include <vector>

namespace dev {

namespace fs = std::filesystem;

/// Locate the plugins directory.
/// Checks exe-relative first, then falls back to cwd-relative.
inline fs::path find_plugins_dir(const char *argv0) {
  // 1. Relative to the executable
  auto exe = fs::weakly_canonical(fs::path(argv0));
  auto dir = exe.parent_path() / "plugins";
  if (fs::is_directory(dir)) {
    return dir;
  }

  // 2. Fallback: relative to cwd
  dir = fs::current_path() / "plugins";
  if (fs::is_directory(dir)) {
    return dir;
  }

  return dir; // return cwd-relative even if not found yet
}

/// Resolve a command name to its plugin executable path.
/// Returns an empty path when the plugin does not exist.
inline fs::path resolve_plugin(std::string_view command, const fs::path &dir) {
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

/// Return a sorted list of available plugin names.
inline std::vector<std::string> list_plugins(const fs::path &dir) {
  std::vector<std::string> names;
  if (!fs::is_directory(dir)) {
    return names;
  }

  for (const auto &entry : fs::directory_iterator(dir)) {
    if (!entry.is_regular_file()) {
      continue;
    }
    names.push_back(entry.path().stem().string());
  }
  std::sort(names.begin(), names.end());
  return names;
}

/// Dispatch a command to its plugin, forwarding remaining arguments.
/// Returns the plugin's exit code, or Error::CommandNotFound (127).
inline int dispatch(int argc, char *argv[]) {
  if (argc < 2) {
    return static_cast<int>(Error::InvalidUsage);
  }

  std::string_view command = argv[1];
  fs::path dir = find_plugins_dir(argv[0]);
  fs::path plugin = resolve_plugin(command, dir);

  if (plugin.empty()) {
    std::println(stderr, "dev: command '{}' not found", command);
    std::println(stderr, "  looked in: {}", dir.string());
    return static_cast<int>(Error::CommandNotFound);
  }

  return spawn(plugin, argc, argv);
}

} // namespace dev
