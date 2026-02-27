# Roadmap

> Rencana pengembangan `dev` — dari dispatcher sederhana menjadi *developer command hub* yang lengkap.

---

## Status: v0.1.0 (Current) — Foundation

Tahap pertama: membangun fondasi dispatcher dan plugin system.

### ✅ Selesai

- [x] Project scaffolding (CMake, folder structure)
- [x] Header definitions (version, metadata, command struct)
- [x] Basic CLI entry point (`main.cpp`)
- [x] Cross-platform build support (MSVC, GCC, Clang)

### 🔄 Dalam Pengerjaan

- [ ] Plugin discovery (scan folder `plugins/`)
- [ ] Process execution (spawn plugin + forward args)
- [ ] Exit code propagation
- [ ] Error handling (command not found, permission denied)
- [ ] `--help` dan `--version` flags bawaan

---

## v0.2.0 — Core Plugins

Membuat set awal plugin bawaan untuk workflow developer sehari-hari.

- [ ] `dev create` — Scaffold project baru dari template
- [ ] `dev open` — Buka project di editor/IDE
- [ ] `dev build` — Wrapper untuk build system (CMake, Cargo, etc.)
- [ ] `dev run` — Compile & jalankan project
- [ ] `dev clean` — Bersihkan build artifacts

---

## v0.3.0 — Configuration & Discovery

Menambahkan konfigurasi dan auto-discovery plugin.

- [ ] Config file (`~/.devrc` atau `dev.toml`)
- [ ] Custom plugin directories via config
- [ ] Plugin metadata (deskripsi, versi, author)
- [ ] `dev list` — Tampilkan semua command yang tersedia
- [ ] `dev help <command>` — Tampilkan help plugin tertentu

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
