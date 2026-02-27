/**
 * @file dispatcher.hpp
 * @brief Plugin discovery and process execution.
 */

#pragma once

#include "dev/error.hpp"

#include <cstdlib>
#include <filesystem>
#include <print>
#include <string>
#include <string_view>

namespace dev {

namespace fs = std::filesystem;

/// Return the plugins directory (next to the running binary).
inline fs::path plugins_dir() { return fs::current_path() / "plugins"; }

/// Resolve a command name to its plugin executable path.
/// Returns an empty path if the plugin does not exist.
inline fs::path resolve_plugin(std::string_view command) {
  fs::path dir = plugins_dir();

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

/// Dispatch a command to its plugin, forwarding all remaining arguments.
/// Returns the plugin's exit code, or Error::CommandNotFound (127).
inline int dispatch(std::string_view command, int argc, char *argv[]) {
  fs::path plugin = resolve_plugin(command);

  if (plugin.empty()) {
    std::println(stderr, "dev: command '{}' not found", command);
    std::println(stderr, "  looked in: {}", plugins_dir().string());
    return static_cast<int>(Error::CommandNotFound);
  }

  // Build the shell command: "<plugin>" arg2 arg3 ...
  std::string cmd = '\"' + plugin.string() + '\"';
  for (int i = 2; i < argc; ++i) {
    cmd += ' ';
    cmd += argv[i];
  }

  int rc = std::system(cmd.c_str());

#ifndef _WIN32
  // On POSIX, std::system returns a waitpid-style status.
  if (WIFEXITED(rc)) {
    rc = WEXITSTATUS(rc);
  }
#endif

  return rc;
}

} // namespace dev
