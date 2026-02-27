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

## v0.3.0 — Configuration & Discovery

Menambahkan konfigurasi dan auto-discovery plugin.

- [ ] Config file (`~/.devrc` atau `dev.toml`)
- [ ] Custom plugin directories via config
- [ ] Plugin metadata (deskripsi, versi, author)
- [ ] Multiple plugin search paths (exe-relative + user-defined)

---

## v0.4.0 — Developer Experience

Polish developer experience.

- [ ] Tab completion (Bash, Zsh, Fish, PowerShell)
- [ ] Colored output (terminal colors)
- [ ] Plugin install/update dari registry
- [ ] Alias support (`dev b` → `dev build`)
- [ ] Verbose/quiet mode (`--verbose`, `--quiet`)

---

## v1.0.0 — Stable Release

- [ ] Semua fitur core stabil dan teruji
- [ ] Dokumentasi lengkap
- [ ] CI/CD pipeline
- [ ] Package distribution (Homebrew, Scoop, APT)
- [ ] Plugin template generator

---

## Masa Depan (Post v1.0)

- [ ] Plugin marketplace / registry
- [ ] Dependency management antar plugin
- [ ] Hook system (pre/post command hooks)
- [ ] Parallel command execution
- [ ] Plugin SDK (shared library interface)
- [ ] Remote plugin execution
