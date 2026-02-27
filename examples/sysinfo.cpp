/**
 * @file sysinfo.cpp
 * @brief Example plugin — prints basic system information.
 *
 * Usage:  dev sysinfo
 */

#include <filesystem>
#include <print>

int main(int argc, char *argv[]) {
  if (argc > 1 && std::string_view(argv[1]) == "--help") {
    std::println("sysinfo — display basic system information");
    std::println("");
    std::println("usage: dev sysinfo");
    return 0;
  }

  std::println("── System Information ──────────────────");
  std::println("");

  // OS
#if defined(_WIN32)
  std::println("  OS:        Windows");
#elif defined(__APPLE__)
  std::println("  OS:        macOS");
#elif defined(__linux__)
  std::println("  OS:        Linux");
#else
  std::println("  OS:        Unknown");
#endif

  // Compiler
#if defined(_MSC_VER)
  std::println("  Compiler:  MSVC {}", _MSC_VER);
#elif defined(__clang__)
  std::println("  Compiler:  Clang {}.{}.{}", __clang_major__, __clang_minor__,
               __clang_patchlevel__);
#elif defined(__GNUC__)
  std::println("  Compiler:  GCC {}.{}.{}", __GNUC__, __GNUC_MINOR__,
               __GNUC_PATCHLEVEL__);
#endif

  // C++ standard
  std::println("  C++:       {}", __cplusplus);

  // Working directory
  std::println("  CWD:       {}", std::filesystem::current_path().string());

  std::println("");
  return 0;
}
