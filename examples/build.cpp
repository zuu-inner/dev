/**
 * @file build.cpp
 * @brief Plugin — auto-detect build system and build the project.
 *
 * Usage:  dev build [--release]
 */

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <print>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

enum class BuildSystem
{
    None,
    CMake,
    Cargo,
    Npm,
    Make,
    Go
};

static BuildSystem detect()
{
    if (fs::exists("CMakeLists.txt"))
        return BuildSystem::CMake;
    if (fs::exists("Cargo.toml"))
        return BuildSystem::Cargo;
    if (fs::exists("package.json"))
        return BuildSystem::Npm;
    if (fs::exists("Makefile"))
        return BuildSystem::Make;
    if (fs::exists("go.mod"))
        return BuildSystem::Go;
    return BuildSystem::None;
}

static const char* name_of(BuildSystem bs)
{
    switch (bs) {
        case BuildSystem::CMake:
            return "CMake";
        case BuildSystem::Cargo:
            return "Cargo";
        case BuildSystem::Npm:
            return "npm";
        case BuildSystem::Make:
            return "Make";
        case BuildSystem::Go:
            return "Go";
        default:
            return "???";
    }
}

static int build_cmake(bool release)
{
    auto type = release ? "Release" : "Debug";
    std::string configure = std::string("cmake -B build -DCMAKE_BUILD_TYPE=") + type;
    std::string build = std::string("cmake --build build --config ") + type;

    std::println("→ {}", configure);
    if (int rc = std::system(configure.c_str()); rc != 0)
        return rc;

    std::println("→ {}", build);
    return std::system(build.c_str());
}

static int build_cargo(bool release)
{
    std::string cmd = release ? "cargo build --release" : "cargo build";
    std::println("→ {}", cmd);
    return std::system(cmd.c_str());
}

static int build_npm([[maybe_unused]] bool release)
{
    std::println("→ npm run build");
    return std::system("npm run build");
}

static int build_make([[maybe_unused]] bool release)
{
    std::println("→ make");
    return std::system("make");
}

static int build_go([[maybe_unused]] bool release)
{
    std::println("→ go build ./...");
    return std::system("go build ./...");
}

int main(int argc, char* argv[])
{
    if (argc > 1 && std::strcmp(argv[1], "--help") == 0) {
        std::println("build — auto-detect build system and build");
        std::println("");
        std::println("usage: dev build [--release]");
        std::println("");
        std::println("supported: CMake, Cargo, npm, Make, Go");
        return 0;
    }

    bool release = false;
    for (int i = 1; i < argc; ++i) {
        if (std::string_view(argv[i]) == "--release" || std::string_view(argv[i]) == "-r") {
            release = true;
        }
    }

    auto bs = detect();
    if (bs == BuildSystem::None) {
        std::println(stderr, "build: no supported build system detected");
        std::println(stderr,
                     "  looked for: CMakeLists.txt, Cargo.toml, "
                     "package.json, Makefile, go.mod");
        return 1;
    }

    std::println("build: detected {} project", name_of(bs));
    std::println("");

    int rc = 0;
    switch (bs) {
        case BuildSystem::CMake:
            rc = build_cmake(release);
            break;
        case BuildSystem::Cargo:
            rc = build_cargo(release);
            break;
        case BuildSystem::Npm:
            rc = build_npm(release);
            break;
        case BuildSystem::Make:
            rc = build_make(release);
            break;
        case BuildSystem::Go:
            rc = build_go(release);
            break;
        default:
            break;
    }

    if (rc == 0) {
        std::println("");
        std::println("✓ Build succeeded");
    }
    return rc;
}
