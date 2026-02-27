# Roadmap

> Rencana pengembangan `dev` — dari dispatcher sederhana menjadi *developer command hub* yang lengkap.

---

## ✅ v0.1.0 (Current) — Foundation

Fondasi dispatcher dan plugin system — **selesai**.

- [x] Project scaffolding (CMake, folder structure)
- [x] Header definitions (version, metadata)
- [x] Clean include structure (`dev/version`, `error`, `process`, `dispatcher`)
- [x] Cross-platform build support (MSVC, GCC, Clang)
- [x] Plugin discovery (scan folder `plugins/`)
- [x] Cross-platform process spawning (`_spawnv` / `fork+execvp`)
- [x] Argument forwarding & exit code propagation
- [x] Error handling (command not found, permission denied)
- [x] `--help` dan `--version` flags bawaan
- [x] `dev list` — Tampilkan semua command yang tersedia
- [x] `dev help <command>` — Tampilkan help plugin tertentu
- [x] Example plugins (`hello`, `sysinfo`)
- [x] Dokumentasi lengkap (README, Architecture, API, QuickStart, Build, Roadmap, Changelog)

---

## ✅ v0.2.0 (Current) — Core Plugins

Plugin bawaan untuk workflow developer sehari-hari — **selesai**.

- [x] `dev create` — Scaffold project baru dari template (cpp, c, py)
- [x] `dev open` — Buka project di editor/IDE (auto-detect VS Code, dll.)
- [x] `dev build` — Auto-detect build system (CMake, Cargo, npm, Make, Go)
- [x] `dev run` — Compile & jalankan project
- [x] `dev clean` — Bersihkan build artifacts

---

## ✅ v0.3.0 (Current) — Configuration & Discovery

Konfigurasi, multi-path discovery, dan plugin metadata — **selesai**.

- [x] Config file (`dev.toml`) dengan parser INI/TOML-like
- [x] Custom plugin directories via config `[plugins] dirs`
- [x] Plugin metadata (`plugins.toml`) — deskripsi di `dev list`
- [x] Multiple plugin search paths (exe-relative + cwd + config-defined)
- [x] Alias support via config `[alias]` section (e.g., `b → build`)

---

## ✅ v0.4.0 (Current) — Developer Experience

Polish developer experience — **selesai**.

- [x] Shell completion generator (`dev completion bash|zsh|fish|pwsh`)
- [x] Colored output (ANSI terminal colors, auto-detect TTY)
- [x] Verbose/quiet mode (`--verbose`/`-V`, `--quiet`/`-q`)
- [x] Windows Virtual Terminal Processing support
- ~~Alias support~~ → sudah di v0.3.0
- ~~Plugin install/update~~ → dipindahkan ke v1.0.0

---

## ✅ v1.0.0 (Current) — Stable Release

Pertama rilis stabil — **selesai**.

- [x] Plugin template generator (`dev init-plugin <name>`)
- [x] GitHub Actions CI (Windows/Ubuntu/macOS)
- [x] Package distribution templates (Scoop, Homebrew)
- [x] Dokumentasi lengkap (README, API, Architecture)
- [x] Version bump ke 1.0.0

---

## Masa Depan (Post v1.0)

- [ ] Plugin marketplace / registry
- [ ] Dependency management antar plugin
- [ ] Hook system (pre/post command hooks)
- [ ] Parallel command execution
- [ ] Plugin SDK (shared library interface)
- [ ] Remote plugin execution
