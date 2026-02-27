# Changelog

Semua perubahan penting pada proyek ini didokumentasikan di file ini.

Format berdasarkan [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
dan proyek ini mengikuti [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [Unreleased]

---

## [0.3.0] — 2026-02-27

### Added
- Config file system (`dev.toml`) with INI/TOML-like parser (`dev/config.hpp`)
- Multi-path plugin discovery: exe-relative + cwd + config-defined directories
- Plugin metadata via `plugins.toml` — descriptions shown in `dev list`
- Alias support via config `[alias]` section (e.g., `b → build`)
- Config search: local `dev.toml` → global `%APPDATA%/dev/config.toml`

---

## [0.2.0] — 2026-02-27

### Added
- Core plugin: `dev create` — scaffold project baru (template: cpp, c, py)
- Core plugin: `dev open` — buka directory di editor (auto-detect VS Code, dll.)
- Core plugin: `dev build` — auto-detect build system & build
- Core plugin: `dev run` — auto-detect & run project
- Core plugin: `dev clean` — auto-detect & clean build artifacts
- CMake `add_plugin()` helper function dengan `DEV_BUILD_EXAMPLES` option
- `dev_` prefix untuk CMake target names (hindari reserved names)

---

## [0.1.0] — 2026-02-27

### Added
- Dispatcher entry point (`src/main.cpp`) dengan `--help`, `--version`, `list`, `help <cmd>`
- Cross-platform process spawning (`dev/process.hpp`): `_spawnv` (Windows) / `fork+execvp` (POSIX)
- Plugin discovery (`dev/dispatcher.hpp`): exe-relative dan cwd-relative lookup
- Error code enum (`dev/error.hpp`)
- CMake `configure_file` untuk auto-generated `version.hpp`
- Cross-platform build (MSVC, GCC, Clang) dengan C++23
- Example plugins: `hello`, `sysinfo`
- Dokumentasi: README, Architecture, API, QuickStart, Build Guide, Roadmap, Changelog

---

[Unreleased]: https://github.com/zuudevs/dev/compare/v0.2.0...HEAD
[0.2.0]: https://github.com/zuudevs/dev/compare/v0.1.0...v0.2.0
[0.1.0]: https://github.com/zuudevs/dev/releases/tag/v0.1.0
