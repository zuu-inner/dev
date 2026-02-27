/**
 * @file main.cpp
 * @brief Entry point for the dev CLI dispatcher.
 */

#include "dev.hpp"
#include <print>
#include <string>
#include <string_view>
#include <unordered_map>

// ── Globals (loaded once) ───────────────────────────────────

static dev::Config g_config;
static std::vector<dev::fs::path> g_plugin_dirs;
static std::unordered_map<std::string, std::string> g_aliases;
static dev::Config g_meta; // plugins.toml

static void load_config(const char *argv0) {
  g_config = dev::Config::find(argv0);
  g_plugin_dirs = dev::find_all_plugin_dirs(argv0, g_config);
  g_aliases = g_config.get_section("alias");

  // Load plugin metadata (plugins.toml next to exe or cwd)
  if (dev::fs::exists("plugins.toml")) {
    g_meta = dev::Config::load("plugins.toml");
  } else {
    auto exe = dev::fs::weakly_canonical(dev::fs::path(argv0));
    auto p = exe.parent_path() / "plugins.toml";
    if (dev::fs::exists(p))
      g_meta = dev::Config::load(p);
  }
}

// ── Commands ────────────────────────────────────────────────

static void print_usage() {
  auto plugins = dev::list_plugins(g_plugin_dirs);

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
    std::println("plugins: (none)");
  } else {
    std::println("plugins:");
    for (const auto &name : plugins) {
      auto desc = g_meta.get(name, "description");
      if (desc.empty()) {
        std::println("  {}", name);
      } else {
        std::println("  {:<14} {}", name, desc);
      }
    }
  }

  if (!g_aliases.empty()) {
    std::println("");
    std::println("aliases:");
    for (const auto &[alias, target] : g_aliases) {
      std::println("  {:<14} → {}", alias, target);
    }
  }

  if (!g_config.empty()) {
    std::println("");
    std::println("config: {}", g_config.path().string());
  }
}

static int cmd_list() {
  auto plugins = dev::list_plugins(g_plugin_dirs);

  if (plugins.empty()) {
    std::println("No plugins found.");
    std::println("");
    std::println("Place executable files in the plugins/ directory.");
    return 0;
  }

  std::println("Available commands:");
  std::println("");
  for (const auto &name : plugins) {
    auto desc = g_meta.get(name, "description");
    if (desc.empty()) {
      std::println("  {}", name);
    } else {
      std::println("  {:<14} {}", name, desc);
    }
  }

  if (!g_aliases.empty()) {
    std::println("");
    std::println("Aliases:");
    std::println("");
    for (const auto &[alias, target] : g_aliases) {
      std::println("  {:<14} → {}", alias, target);
    }
  }

  return 0;
}

static int cmd_help(int argc, char *argv[]) {
  if (argc < 3) {
    std::println(stderr, "usage: dev help <command>");
    return static_cast<int>(dev::Error::InvalidUsage);
  }

  std::string_view target = argv[2];
  auto plugin = dev::resolve_plugin(target, g_plugin_dirs);

  if (plugin.empty()) {
    std::println(stderr, "dev: command '{}' not found", target);
    return static_cast<int>(dev::Error::CommandNotFound);
  }

  const char *help_argv[] = {argv[0], argv[2], "--help", nullptr};
  return dev::spawn(plugin, 3, const_cast<char **>(help_argv));
}

// ── Entry point ─────────────────────────────────────────────

int main(int argc, char *argv[]) {
  load_config(argv[0]);

  if (argc < 2) {
    print_usage();
    return 0;
  }

  std::string command_str(argv[1]);

  // ── Resolve alias ───────────────────────────────────────
  if (auto it = g_aliases.find(command_str); it != g_aliases.end()) {
    command_str = it->second;
    // Replace argv[1] with resolved command for forwarding
    argv[1] = command_str.data();
  }

  std::string_view command = command_str;

  // ── Built-in flags ──────────────────────────────────────
  if (command == "--version" || command == "-v") {
    std::println("dev v{}", dev::version);
    return 0;
  }

  if (command == "--help" || command == "-h") {
    print_usage();
    return 0;
  }

  // ── Built-in commands ───────────────────────────────────
  if (command == "list") {
    return cmd_list();
  }

  if (command == "help") {
    return cmd_help(argc, argv);
  }

  // ── Plugin dispatch ─────────────────────────────────────
  return dev::dispatch(argc, argv, g_plugin_dirs);
}