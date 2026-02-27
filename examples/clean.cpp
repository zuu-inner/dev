/**
 * @file clean.cpp
 * @brief Plugin — auto-detect build system and clean build artifacts.
 *
 * Usage:  dev clean
 */

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <print>

namespace fs = std::filesystem;

static void remove_dir(const char* name)
{
    if (fs::exists(name) && fs::is_directory(name)) {
        auto count = fs::remove_all(name);
        std::println("  removed {}/  ({} items)", name, count);
    }
}

int main(int argc, char* argv[])
{
    if (argc > 1 && std::strcmp(argv[1], "--help") == 0) {
        std::println("clean — remove build artifacts");
        std::println("");
        std::println("usage: dev clean");
        std::println("");
        std::println("Auto-detects build system and removes known artifact");
        std::println("directories. Supported: CMake, Cargo, npm, Make, Go.");
        return 0;
    }

    bool found = false;

    // CMake
    if (fs::exists("CMakeLists.txt")) {
        found = true;
        std::println("clean: detected CMake project");
        remove_dir("build");
        remove_dir(".cache");
    }

    // Cargo / Rust
    if (fs::exists("Cargo.toml")) {
        found = true;
        std::println("clean: detected Cargo project");
        remove_dir("target");
    }

    // npm / Node.js
    if (fs::exists("package.json")) {
        found = true;
        std::println("clean: detected npm project");
        remove_dir("node_modules");
        remove_dir("dist");
        remove_dir(".next");
    }

    // Makefile
    if (fs::exists("Makefile")) {
        found = true;
        std::println("clean: detected Makefile project");
        std::println("  → make clean");
        std::system("make clean");
    }

    // Go
    if (fs::exists("go.mod")) {
        found = true;
        std::println("clean: detected Go project");
        std::println("  → go clean");
        std::system("go clean");
    }

    if (!found) {
        std::println(stderr, "clean: no supported build system detected");
        return 1;
    }

    std::println("");
    std::println("✓ Clean completed");
    return 0;
}
