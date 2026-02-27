/**
 * @file hello.cpp
 * @brief Example plugin — prints a greeting.
 *
 * Usage:  dev hello [name]
 */

#include <cstring>
#include <print>

int main(int argc, char *argv[]) {
  if (argc > 1 && std::strcmp(argv[1], "--help") == 0) {
    std::println("hello — greet someone (or the world)");
    std::println("");
    std::println("usage: dev hello [name]");
    std::println("");
    std::println("examples:");
    std::println("  dev hello          → Hello, World!");
    std::println("  dev hello Rafi     → Hello, Rafi!");
    return 0;
  }

  const char *name = (argc > 1) ? argv[1] : "World";
  std::println("Hello, {}!", name);
  return 0;
}
