/**
 * @file open.cpp
 * @brief Plugin — open a directory in the preferred editor/IDE.
 *
 * Usage:  dev open [path]
 */

#include <array>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <print>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

#ifdef _WIN32
static constexpr std::array editors = {"code", "code-insiders", "subl", "notepad++"};
static constexpr std::string_view fallback = "explorer";
#elif defined(__APPLE__)
static constexpr std::array editors = {"code", "code-insiders", "subl", "atom"};
static constexpr std::string_view fallback = "open";
#else
static constexpr std::array editors = {"code", "code-insiders", "subl", "atom", "vim", "nano"};
static constexpr std::string_view fallback = "xdg-open";
#endif

/// Try to launch an editor. Returns true on success.
static bool try_editor(std::string_view editor, const fs::path& target)
{
    std::string cmd = std::string(editor) + " \"" + target.string() + "\"";

#ifdef _WIN32
    // On Windows, `where` checks if a command exists.
    std::string check = "where " + std::string(editor) + " >nul 2>nul";
#else
    std::string check = "which " + std::string(editor) + " >/dev/null 2>&1";
#endif

    if (std::system(check.c_str()) != 0) {
        return false;
    }

    std::system(cmd.c_str());
    return true;
}

int main(int argc, char* argv[])
{
    if (argc > 1 && std::strcmp(argv[1], "--help") == 0) {
        std::println("open — open a directory in your editor");
        std::println("");
        std::println("usage: dev open [path]");
        std::println("");
        std::println("Opens the given path (default: cwd) in the first");
        std::println("available editor: code, code-insiders, subl, ...");
        return 0;
    }

    fs::path target = (argc > 1) ? fs::path(argv[1]) : fs::current_path();

    if (!fs::exists(target)) {
        std::println(stderr, "open: path '{}' does not exist", target.string());
        return 1;
    }

    // Try each known editor
    for (auto editor : editors) {
        if (try_editor(editor, target)) {
            std::println("✓ Opened in {}", editor);
            return 0;
        }
    }

    // Fallback to OS file manager
    std::string cmd = std::string(fallback) + " \"" + target.string() + "\"";
    std::system(cmd.c_str());
    std::println("✓ Opened with {}", fallback);
    return 0;
}
