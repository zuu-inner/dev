# Quick Start

> Panduan memulai `dev` v1.0.0 — build, jalankan, dan buat plugin pertama.

---

## Prerequisites

| Tool | Versi Minimum |
|------|---------------|
| CMake | 3.20 |
| C++ Compiler (C++23) | GCC 13+ / Clang 17+ / MSVC 19.35+ |

---

## 1. Clone & Build

```bash
git clone https://github.com/zuudevs/dev.git
cd dev

cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Binary tersedia di `build/bin/Release/Dev.exe` (Windows) atau `build/bin/Dev` (Linux/macOS).

---

## 2. Verifikasi

```bash
dev --version
# dev v1.0.0

dev list
# Available commands:
#   build          Auto-detect build system and build
#   clean          Remove build artifacts
#   create         Scaffold a new project from a template
#   ...
```

---

## 3. Coba Commands

```bash
# Scaffold project C++ baru
dev create my-app

# Scaffold project Python
dev create my-py-app --template py

# Build project (auto-detect CMake/Cargo/npm/Make/Go)
cd my-app
dev build

# Buka di VS Code
dev open .

# Clean build artifacts
dev clean
```

---

## 4. Setup Aliases (opsional)

Buat file `dev.toml` di project root:

```toml
[alias]
b = "build"
r = "run"
c = "create"
o = "open"
```

Sekarang `dev b --release` = `dev build --release`.

---

## 5. Shell Completion (opsional)

```bash
# Bash:  eval "$(dev completion bash)"
# Zsh:   eval "$(dev completion zsh)"
# Fish:  dev completion fish > ~/.config/fish/completions/dev.fish
# PowerShell:
dev completion pwsh | Invoke-Expression
```

---

## 6. Buat Plugin Pertama

```bash
dev init-plugin my-tool
cd my-tool

# Edit my-tool.cpp sesuai kebutuhan
cmake -B build && cmake --build build --config Release

# Install ke plugins/
cp build/Release/my-tool.exe ../plugins/   # Windows
cp build/my-tool ../plugins/               # Linux/macOS
```

```bash
dev my-tool
# Hello from my-tool!
```

**Selesai!**

---

## Langkah Selanjutnya

- [Plugin API Reference](API.md) — spesifikasi lengkap
- [Architecture](ARCHITECTURE.md) — cara kerja internal
- [Roadmap](ROADMAP.md) — rencana pengembangan
- [Contributing](../CONTRIBUTING.md) — panduan kontribusi
