/**
 * @file main.cpp
 * @brief Entry point for the dev CLI dispatcher.
 */

#include "dev.hpp"
#include <print>
#include <string_view>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::println("dev v{} — lightweight CLI dispatcher", dev::version);
    std::println("");
    std::println("usage: dev <command> [args...]");
    std::println("");
    std::println("Run 'dev --help' for more information.");
    return 0;
  }

  std::string_view command = argv[1];

  if (command == "--version" || command == "-v") {
    std::println("dev v{}", dev::version);
    return 0;
  }

  if (command == "--help" || command == "-h") {
    std::println("dev v{} — lightweight CLI dispatcher", dev::version);
    std::println("");
    std::println("usage: dev <command> [args...]");
    std::println("");
    std::println("options:");
    std::println("  -h, --help       Show this help message");
    std::println("  -v, --version    Show version information");
    std::println("");
    std::println(
        "Commands are plugin executables in the 'plugins/' directory.");
    std::println("See docs/API.md for the plugin authoring guide.");
    return 0;
  }

  return dev::dispatch(command, argc, argv);
}