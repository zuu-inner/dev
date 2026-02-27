/**
 * @file main.cpp
 * @brief Entry point for the dev CLI dispatcher.
 */

#include "dev.hpp"
#include <print>
#include <string>
#include <string_view>
#include <unordered_map>

namespace s = dev::style;

// ── Globals ─────────────────────────────────────────────────

static dev::Config g_config;
static std::vector<dev::fs::path> g_plugin_dirs;
static std::unordered_map<std::string, std::string> g_aliases;
static dev::Config g_meta;
static bool g_verbose = false;
static bool g_quiet = false;

static void load_config(const char *argv0) {
  g_config = dev::Config::find(argv0);
  g_plugin_dirs = dev::find_all_plugin_dirs(argv0, g_config);
  g_aliases = g_config.get_section("alias");

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

  std::println("{}", s::bold_text("dev") + " " +
                         s::dim_text("v" + std::string(dev::version)) +
                         " — lightweight CLI dispatcher");
  std::println("");
  std::println("{}  dev <command> [args...]", s::yellow_text("usage:"));
  std::println("");
  std::println("{}", s::bold_text("options:"));
  std::println("  {}       Show this help message", s::cyan_text("-h, --help"));
  std::println("  {}    Show version information",
               s::cyan_text("-v, --version"));
  std::println("  {}     Suppress non-essential output",
               s::cyan_text("-q, --quiet"));
  std::println("  {}   Extra detail (config, search)",
               s::cyan_text("-V, --verbose"));
  std::println("");
  std::println("{}", s::bold_text("built-in commands:"));
  std::println("  {}             List available plugin commands",
               s::cyan_text("list"));
  std::println("  {}       Show help for a plugin command",
               s::cyan_text("help <cmd>"));
  std::println("  {} Generate shell completions", s::cyan_text("completion"));
  std::println("");

  if (plugins.empty()) {
    std::println("{}", s::dim_text("plugins: (none)"));
  } else {
    std::println("{}", s::bold_text("plugins:"));
    for (const auto &name : plugins) {
      auto desc = g_meta.get(name, "description");
      if (desc.empty()) {
        std::println("  {}", s::cyan_text(name));
      } else {
        std::println("  {:<22} {}", s::cyan_text(name), s::dim_text(desc));
      }
    }
  }

  if (!g_aliases.empty()) {
    std::println("");
    std::println("{}", s::bold_text("aliases:"));
    for (const auto &[alias, target] : g_aliases) {
      std::println("  {:<22} {} {}", s::cyan_text(alias), s::dim_text("→"),
                   target);
    }
  }

  if (g_verbose && !g_config.empty()) {
    std::println("");
    std::println("{} {}", s::dim_text("config:"), g_config.path().string());
    std::println("{}", s::dim_text("plugin dirs:"));
    for (const auto &d : g_plugin_dirs) {
      std::println("  {}", d.string());
    }
  }
}

static int cmd_list() {
  auto plugins = dev::list_plugins(g_plugin_dirs);

  if (plugins.empty()) {
    std::println("No plugins found.");
    if (!g_quiet) {
      std::println("");
      std::println("Place executable files in the plugins/ directory.");
    }
    return 0;
  }

  if (!g_quiet) {
    std::println("{}", s::bold_text("Available commands:"));
    std::println("");
  }

  for (const auto &name : plugins) {
    auto desc = g_meta.get(name, "description");
    if (desc.empty()) {
      std::println("  {}", s::cyan_text(name));
    } else {
      std::println("  {:<22} {}", s::cyan_text(name), desc);
    }
  }

  if (!g_aliases.empty() && !g_quiet) {
    std::println("");
    std::println("{}", s::bold_text("Aliases:"));
    std::println("");
    for (const auto &[alias, target] : g_aliases) {
      std::println("  {:<22} {} {}", s::cyan_text(alias), s::dim_text("→"),
                   target);
    }
  }

  return 0;
}

static int cmd_help(int argc, char *argv[]) {
  if (argc < 3) {
    std::println(stderr, "{} usage: dev help <command>", s::red_text("error:"));
    return static_cast<int>(dev::Error::InvalidUsage);
  }

  std::string_view target = argv[2];
  auto plugin = dev::resolve_plugin(target, g_plugin_dirs);

  if (plugin.empty()) {
    std::println(stderr, "{} command '{}' not found", s::red_text("dev:"),
                 target);
    return static_cast<int>(dev::Error::CommandNotFound);
  }

  const char *help_argv[] = {argv[0], argv[2], "--help", nullptr};
  return dev::spawn(plugin, 3, const_cast<char **>(help_argv));
}

// ── Entry point ─────────────────────────────────────────────

int main(int argc, char *argv[]) {
  s::init();
  load_config(argv[0]);

  // ── Pre-scan for global flags ───────────────────────────
  for (int i = 1; i < argc; ++i) {
    std::string_view a = argv[i];
    if (a == "--verbose" || a == "-V")
      g_verbose = true;
    if (a == "--quiet" || a == "-q")
      g_quiet = true;
  }

  if (argc < 2) {
    print_usage();
    return 0;
  }

  std::string command_str(argv[1]);

  // Skip if first arg is a global flag
  if (command_str == "--verbose" || command_str == "-V" ||
      command_str == "--quiet" || command_str == "-q") {
    if (argc < 3) {
      print_usage();
      return 0;
    }
    command_str = argv[2];
    // Shift argv for dispatch
    argv[1] = argv[2];
    for (int i = 3; i < argc; ++i)
      argv[i - 1] = argv[i];
    --argc;
  }

  // ── Resolve alias ───────────────────────────────────────
  if (auto it = g_aliases.find(command_str); it != g_aliases.end()) {
    if (g_verbose) {
      std::println("{} alias '{}' → '{}'", s::dim_text("dev:"), command_str,
                   it->second);
    }
    command_str = it->second;
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
  if (command == "list")
    return cmd_list();
  if (command == "help")
    return cmd_help(argc, argv);

  // ── Plugin dispatch ─────────────────────────────────────
  if (g_verbose) {
    auto plugin = dev::resolve_plugin(command, g_plugin_dirs);
    if (!plugin.empty()) {
      std::println("{} dispatching to {}", s::dim_text("dev:"),
                   plugin.string());
    }
  }

  return dev::dispatch(argc, argv, g_plugin_dirs);
}