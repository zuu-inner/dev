/**
 * @file init-plugin.cpp
 * @brief Plugin — scaffold a new dev plugin project.
 *
 * Usage:  dev init-plugin <name>
 */

#include <cstring>
#include <filesystem>
#include <fstream>
#include <print>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

static void write_file(const fs::path& path, std::string_view content)
{
    std::ofstream ofs(path);
    ofs << content;
}

int main(int argc, char* argv[])
{
    if (argc < 2 || std::strcmp(argv[1], "--help") == 0) {
        std::println("init-plugin — scaffold a new dev plugin");
        std::println("");
        std::println("usage: dev init-plugin <name>");
        std::println("");
        std::println("creates:");
        std::println("  <name>/");
        std::println("  ├── <name>.cpp          main source with --help boilerplate");
        std::println("  ├── CMakeLists.txt      standalone build file");
        std::println("  └── README.md           plugin documentation");
        return (argc < 2) ? 2 : 0;
    }

    std::string name(argv[1]);
    fs::path root = fs::current_path() / name;

    if (fs::exists(root)) {
        std::println(stderr, "init-plugin: '{}' already exists", name);
        return 1;
    }

    fs::create_directories(root);

    // ── Source file ─────────────────────────────────────────
    write_file(root / (name + ".cpp"),
               "/**\n"
               " * @file " +
                   name +
                   ".cpp\n"
                   " * @brief Plugin — " +
                   name +
                   ".\n"
                   " *\n"
                   " * Usage:  dev " +
                   name +
                   " [args...]\n"
                   " */\n"
                   "\n"
                   "#include <cstring>\n"
                   "#include <print>\n"
                   "#include <string_view>\n"
                   "\n"
                   "int main(int argc, char* argv[]) {\n"
                   "\tif (argc > 1 && std::strcmp(argv[1], \"--help\") == 0) {\n"
                   "\t\tstd::println(\"" +
                   name +
                   " — <description>\");\n"
                   "\t\tstd::println(\"\");\n"
                   "\t\tstd::println(\"usage: dev " +
                   name +
                   " [args...]\");\n"
                   "\t\treturn 0;\n"
                   "\t}\n"
                   "\n"
                   "\tstd::println(\"Hello from " +
                   name +
                   "!\");\n"
                   "\treturn 0;\n"
                   "}\n");

    // ── CMakeLists.txt ──────────────────────────────────────
    write_file(root / "CMakeLists.txt",
               "cmake_minimum_required(VERSION 3.20)\n"
               "\n"
               "project(\n"
               "\t" +
                   name +
                   "\n"
                   "\tVERSION 0.1.0\n"
                   "\tLANGUAGES CXX\n"
                   ")\n"
                   "\n"
                   "set(CMAKE_CXX_STANDARD 23)\n"
                   "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n"
                   "\n"
                   "add_executable(${PROJECT_NAME} " +
                   name +
                   ".cpp)\n"
                   "\n"
                   "# Install to dev plugins directory:\n"
                   "#   cmake --install build --prefix <path-to-dev>/plugins\n"
                   "install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)\n");

    // ── README.md ───────────────────────────────────────────
    write_file(root / "README.md",
               "# " + name +
                   "\n"
                   "\n"
                   "A plugin for [dev](https://github.com/zuudevs/dev).\n"
                   "\n"
                   "## Build\n"
                   "\n"
                   "```bash\n"
                   "cmake -B build -DCMAKE_BUILD_TYPE=Release\n"
                   "cmake --build build --config Release\n"
                   "```\n"
                   "\n"
                   "## Install\n"
                   "\n"
                   "Copy the built executable to your `dev` plugins directory:\n"
                   "\n"
                   "```bash\n"
                   "cp build/" +
                   name +
                   " /path/to/dev/plugins/\n"
                   "```\n"
                   "\n"
                   "## Usage\n"
                   "\n"
                   "```bash\n"
                   "dev " +
                   name +
                   " [args...]\n"
                   "```\n");

    std::println("✓ Created plugin scaffold: {}/", name);
    std::println("");
    std::println("  cd {} && cmake -B build && cmake --build build", name);
    std::println("  cp build/{}.exe ../plugins/", name);
    return 0;
}
