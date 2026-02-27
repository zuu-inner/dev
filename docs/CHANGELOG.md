# Changelog

Semua perubahan penting pada proyek ini didokumentasikan di file ini.

Format berdasarkan [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
dan proyek ini mengikuti [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [Unreleased]

### Added
- Plugin discovery dan execution (dalam pengembangan)
- Exit code propagation
- Error handling untuk command not found

---

## [0.1.0] — 2026-02-27

### Added
- Initial project scaffolding
- CMake build system dengan dukungan C++23
- Cross-platform support (MSVC, GCC, Clang)
- Project metadata header (`dev.hpp`)
- Version macros (`config/project.hpp`)
- Command struct dan registry (`cli/command.hpp`)
- Error code enum (`core/error.hpp`)
- System utility class (`utils/system.hpp`)
- Basic CLI entry point (`src/main.cpp`)
- Dokumentasi: README, Architecture, API, QuickStart, Build Guide, Roadmap
- Folder structure: `plugins/`, `examples/`, `scripts/`, `tests/`

---

[Unreleased]: https://github.com/zuudevs/dev/compare/v0.1.0...HEAD
[0.1.0]: https://github.com/zuudevs/dev/releases/tag/v0.1.0
