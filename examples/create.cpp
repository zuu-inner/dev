/**
 * @file create.cpp
 * @brief Plugin — scaffold a new project from a template.
 *
 * Usage:  dev create <name> [--template cpp|c|py]
 */

#include <cstring>
#include <filesystem>
#include <fstream>
#include <print>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

// ── Helpers ──────────────────────────────────────────────────

static void write_file(const fs::path &path, std::string_view content) {
  std::ofstream ofs(path);
  ofs << content;
}

// ── Templates ────────────────────────────────────────────────

static void scaffold_cpp(const fs::path &root, std::string_view name) {
  fs::create_directories(root / "src");
  fs::create_directories(root / "include");

  write_file(root / "src" / "main.cpp", "#include <print>\n\n"
                                        "int main() {\n"
                                        "\tstd::println(\"Hello from " +
                                            std::string(name) +
                                            "!\");\n"
                                            "\treturn 0;\n"
                                            "}\n");

  write_file(
      root / "CMakeLists.txt",
      "cmake_minimum_required(VERSION 3.20)\n\n"
      "project(\n"
      "\t" +
          std::string(name) +
          "\n"
          "\tVERSION 0.1.0\n"
          "\tLANGUAGES CXX\n"
          ")\n\n"
          "set(CMAKE_CXX_STANDARD 23)\n"
          "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n\n"
          "add_executable(${PROJECT_NAME}\n"
          "\tsrc/main.cpp\n"
          ")\n\n"
          "target_include_directories(${PROJECT_NAME} PRIVATE include)\n");

  write_file(root / ".gitignore", "/build/\n"
                                  "/.cache/\n"
                                  "*.exe\n");

  write_file(root / "README.md",
             "# " + std::string(name) +
                 "\n\n"
                 "## Build\n\n"
                 "```bash\n"
                 "cmake -B build -DCMAKE_BUILD_TYPE=Release\n"
                 "cmake --build build --config Release\n"
                 "```\n");
}

static void scaffold_c(const fs::path &root, std::string_view name) {
  fs::create_directories(root / "src");

  write_file(root / "src" / "main.c", "#include <stdio.h>\n\n"
                                      "int main(void) {\n"
                                      "\tprintf(\"Hello from " +
                                          std::string(name) +
                                          "!\\n\");\n"
                                          "\treturn 0;\n"
                                          "}\n");

  write_file(root / "Makefile", "CC      = gcc\n"
                                "CFLAGS  = -Wall -Wextra -std=c17\n"
                                "TARGET  = " +
                                    std::string(name) +
                                    "\n"
                                    "SRC     = src/main.c\n\n"
                                    "all: $(TARGET)\n\n"
                                    "$(TARGET): $(SRC)\n"
                                    "\t$(CC) $(CFLAGS) -o $@ $^\n\n"
                                    "clean:\n"
                                    "\trm -f $(TARGET)\n\n"
                                    ".PHONY: all clean\n");

  write_file(root / ".gitignore", std::string(name) + "\n"
                                                      "*.o\n"
                                                      "*.exe\n");

  write_file(root / "README.md", "# " + std::string(name) +
                                     "\n\n"
                                     "## Build\n\n"
                                     "```bash\nmake\n```\n");
}

static void scaffold_py(const fs::path &root, std::string_view name) {
  fs::create_directories(root / "src");

  write_file(root / "src" / "main.py", "\"\"\"" + std::string(name) +
                                           " — entry point.\"\"\"\n\n\n"
                                           "def main() -> None:\n"
                                           "    print(\"Hello from " +
                                           std::string(name) +
                                           "!\")\n\n\n"
                                           "if __name__ == \"__main__\":\n"
                                           "    main()\n");

  write_file(root / "pyproject.toml", "[project]\n"
                                      "name = \"" +
                                          std::string(name) +
                                          "\"\n"
                                          "version = \"0.1.0\"\n"
                                          "requires-python = \">= 3.10\"\n\n"
                                          "[project.scripts]\n" +
                                          std::string(name) +
                                          " = \"src.main:main\"\n");

  write_file(root / ".gitignore", "__pycache__/\n"
                                  "*.pyc\n"
                                  ".venv/\n"
                                  "dist/\n");

  write_file(root / "README.md", "# " + std::string(name) +
                                     "\n\n"
                                     "## Run\n\n"
                                     "```bash\npython src/main.py\n```\n");
}

// ── Main ─────────────────────────────────────────────────────

int main(int argc, char *argv[]) {
  if (argc < 2 || std::strcmp(argv[1], "--help") == 0) {
    std::println("create — scaffold a new project");
    std::println("");
    std::println("usage: dev create <name> [--template cpp|c|py]");
    std::println("");
    std::println("templates:");
    std::println("  cpp   C++23 project with CMakeLists.txt (default)");
    std::println("  c     C17 project with Makefile");
    std::println("  py    Python project with pyproject.toml");
    return (argc < 2) ? 2 : 0;
  }

  std::string_view name = argv[1];
  std::string tmpl = "cpp";

  for (int i = 2; i < argc - 1; ++i) {
    if (std::string_view(argv[i]) == "--template" ||
        std::string_view(argv[i]) == "-t") {
      tmpl = argv[i + 1];
    }
  }

  fs::path root = fs::current_path() / std::string(name);

  if (fs::exists(root)) {
    std::println(stderr, "create: '{}' already exists", name);
    return 1;
  }

  fs::create_directories(root);

  if (tmpl == "cpp") {
    scaffold_cpp(root, name);
  } else if (tmpl == "c") {
    scaffold_c(root, name);
  } else if (tmpl == "py") {
    scaffold_py(root, name);
  } else {
    std::println(stderr, "create: unknown template '{}'", tmpl);
    std::println(stderr, "  available: cpp, c, py");
    return 1;
  }

  std::println("✓ Created '{}' project: {}", tmpl, root.string());
  return 0;
}
