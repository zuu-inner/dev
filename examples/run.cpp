/**
 * @file run.cpp
 * @brief Plugin — auto-detect build system and run the project.
 *
 * Usage:  dev run [args...]
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

/// Forward extra args (argv[1..]) as a single string.
static std::string extra_args(int argc, char* argv[])
{
    std::string args;
    for (int i = 1; i < argc; ++i) {
        if (std::string_view(argv[i]) == "--help")
            continue;
        if (!args.empty())
            args += ' ';
        args += argv[i];
    }
    return args;
}

static int run_cmake(const std::string& args)
{
    // Build first
    std::println("→ cmake --build build --config Release");
    if (int rc = std::system("cmake --build build --config Release"); rc != 0) {
        std::println(stderr, "run: build failed");
        return rc;
    }

    // Find the executable — look in common output locations
    for (auto dir : {"build/bin/Release",
                     "build/bin/Debug",
                     "build/bin",
                     "build/Release",
                     "build/Debug",
                     "build"}) {
        if (!fs::is_directory(dir))
            continue;
        for (const auto& entry : fs::directory_iterator(dir)) {
            if (!entry.is_regular_file())
                continue;
            auto ext = entry.path().extension().string();
#ifdef _WIN32
            if (ext != ".exe")
                continue;
#else
            if (!ext.empty())
                continue;
            // Check executable permission
            auto perms = entry.status().permissions();
            if ((perms & fs::perms::owner_exec) == fs::perms::none)
                continue;
#endif
            std::string cmd = "\"" + entry.path().string() + "\"";
            if (!args.empty()) {
                cmd += ' ';
                cmd += args;
            }
            std::println("→ {}", cmd);
            return std::system(cmd.c_str());
        }
    }

    std::println(stderr, "run: could not find built executable in build/");
    return 1;
}

static int run_cargo(const std::string& args)
{
    std::string cmd = "cargo run";
    if (!args.empty()) {
        cmd += " -- ";
        cmd += args;
    }
    std::println("→ {}", cmd);
    return std::system(cmd.c_str());
}

static int run_npm([[maybe_unused]] const std::string& args)
{
    std::println("→ npm start");
    return std::system("npm start");
}

static int run_make([[maybe_unused]] const std::string& args)
{
    std::println("→ make run");
    return std::system("make run");
}

static int run_go(const std::string& args)
{
    std::string cmd = "go run .";
    if (!args.empty()) {
        cmd += ' ';
        cmd += args;
    }
    std::println("→ {}", cmd);
    return std::system(cmd.c_str());
}

int main(int argc, char* argv[])
{
    if (argc > 1 && std::strcmp(argv[1], "--help") == 0) {
        std::println("run — auto-detect build system and run the project");
        std::println("");
        std::println("usage: dev run [args...]");
        std::println("");
        std::println("supported: CMake, Cargo, npm, Make, Go");
        return 0;
    }

    auto bs = detect();
    if (bs == BuildSystem::None) {
        std::println(stderr, "run: no supported build system detected");
        return 1;
    }

    std::println("run: detected {} project", name_of(bs));
    std::println("");

    auto args = extra_args(argc, argv);

    switch (bs) {
        case BuildSystem::CMake:
            return run_cmake(args);
        case BuildSystem::Cargo:
            return run_cargo(args);
        case BuildSystem::Npm:
            return run_npm(args);
        case BuildSystem::Make:
            return run_make(args);
        case BuildSystem::Go:
            return run_go(args);
        default:
            return 1;
    }
}
