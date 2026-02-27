/**
 * @file main.cpp
 * @brief Entry point for the dev CLI dispatcher.
 */

#include "dev.hpp"
#include <print>
#include <string_view>

static void print_usage(const char *argv0) {
  auto dir = dev::find_plugins_dir(argv0);
  auto plugins = dev::list_plugins(dir);

  std::println("dev v{} — lightweight CLI dispatcher", dev::version);
  std::println("");
  std::println("usage: dev <command> [args...]");
  std::println("");
  std::println("options:");
  std::println("  -h, --help       Show this help message");
  std::println("  -v, --version    Show version information");
  std::println("");
  std::println("built-in commands:");
  std::println("  list             List available plugin commands");
  std::println("  help <cmd>       Show help for a plugin command");
  std::println("");

  if (plugins.empty()) {
    std::println("plugins: (none found in {})", dir.string());
  } else {
    std::println("plugins:");
    for (const auto &name : plugins) {
      std::println("  {}", name);
    }
  }
}

static int cmd_list(const char *argv0) {
  auto dir = dev::find_plugins_dir(argv0);
  auto plugins = dev::list_plugins(dir);

  if (plugins.empty()) {
    std::println("No plugins found in {}", dir.string());
    std::println("");
    std::println("Place executable files in the plugins/ directory.");
    return 0;
  }

  std::println("Available commands:");
  std::println("");
  for (const auto &name : plugins) {
    std::println("  {}", name);
  }
  return 0;
}

static int cmd_help(int argc, char *argv[]) {
  if (argc < 3) {
    std::println(stderr, "usage: dev help <command>");
    return static_cast<int>(dev::Error::InvalidUsage);
  }

  std::string_view target = argv[2];
  auto dir = dev::find_plugins_dir(argv[0]);
  auto plugin = dev::resolve_plugin(target, dir);

  if (plugin.empty()) {
    std::println(stderr, "dev: command '{}' not found", target);
    return static_cast<int>(dev::Error::CommandNotFound);
  }

  // Synthesise: plugin --help
  const char *help_argv[] = {argv[0], argv[2], "--help", nullptr};
  return dev::spawn(plugin, 3, const_cast<char **>(help_argv));
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    print_usage(argv[0]);
    return 0;
  }

  std::string_view command = argv[1];

  // ── Built-in flags ──────────────────────────────────────
  if (command == "--version" || command == "-v") {
    std::println("dev v{}", dev::version);
    return 0;
  }

  if (command == "--help" || command == "-h") {
    print_usage(argv[0]);
    return 0;
  }

  // ── Built-in commands ───────────────────────────────────
  if (command == "list") {
    return cmd_list(argv[0]);
  }

  if (command == "help") {
    return cmd_help(argc, argv);
  }

  // ── Plugin dispatch ─────────────────────────────────────
  return dev::dispatch(argc, argv);
}