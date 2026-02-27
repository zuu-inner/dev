<p align="center">
  <h1 align="center"><code>dev</code></h1>
  <p align="center">
    A lightweight CLI dispatcher — satu entry-point untuk semua perintah developer.
    <br />
    <a href="docs/QUICKSTART.md"><strong>Quick Start »</strong></a>
    ·
    <a href="docs/ARCHITECTURE.md">Architecture</a>
    ·
    <a href="docs/API.md">Plugin API</a>
    ·
    <a href="docs/ROADMAP.md">Roadmap</a>
  </p>
</p>

---

## Apa itu `dev`?

**`dev`** adalah *command-hub* untuk developer — satu binary kecil yang **mendelegasikan** perintah ke plugin/command terpisah. Cukup ketik:

```bash
dev <command> [args...]
```

`dev` akan mencari executable plugin bernama `<command>` di folder `plugins/`, lalu menjalankannya dengan meneruskan seluruh argumen dan exit code.

**Filosofi:** *Build once, extend forever.* Tambah fitur baru cukup dengan meletakkan executable baru di folder plugin — tanpa perlu rebuild `dev` itu sendiri.

---

## Fitur

| Fitur | Deskripsi |
|-------|-----------|
| **Zero-rebuild extension** | Tambah command baru tanpa kompilasi ulang dispatcher |
| **Plugin berbasis executable** | Tiap plugin adalah program mandiri (bahasa apapun) |
| **9 plugins bawaan** | `create`, `open`, `build`, `run`, `clean`, `completion`, `init-plugin`, `hello`, `sysinfo` |
| **Colored output** | ANSI terminal colors dengan auto-detect TTY |
| **Config file** | `dev.toml` untuk alias, custom plugin dirs, dll. |
| **Shell completion** | Bash, Zsh, Fish, PowerShell |
| **Cross-platform** | Windows (MSVC), Linux (GCC), macOS (Clang) |
| **Modern C++23** | `<print>`, `<filesystem>`, header-only library |

---

## Available Commands

```bash
dev create <name> [--template cpp|c|py]   # Scaffold project baru
dev open [path]                           # Buka di editor (VS Code, dll.)
dev build [--release]                     # Auto-detect build system & build
dev run [args...]                         # Auto-detect & run project
dev clean                                 # Hapus build artifacts
dev completion <bash|zsh|fish|pwsh>       # Generate shell completions
dev init-plugin <name>                    # Scaffold plugin baru
dev list                                  # Daftar semua commands
dev help <cmd>                            # Help untuk command tertentu
```

**Flags:**
```bash
dev --version / -v       # Show version
dev --help / -h          # Show help
dev --verbose / -V       # Extra detail
dev --quiet / -q         # Suppress banners
```

**Aliases** (via `dev.toml`):
```bash
dev b    → dev build
dev r    → dev run
dev c    → dev create
dev o    → dev open
```

---

## Architecture

```
User Shell
│
▼
┌─────────────── dev (dispatcher) ──────────────┐
│  1. Load config (dev.toml)                     │
│  2. Resolve alias                              │
│  3. Resolve plugin in: exe/ → cwd/ → config/  │
│  4. Spawn plugin + forward argv                │
│  5. Propagate exit code                        │
└────────────────────┬──────────────────────────┘
          ┌──────────┼──────────┐
          ▼          ▼          ▼
     ┌────────┐ ┌────────┐ ┌────────┐
     │ create │ │ build  │ │  run   │  ← plugin executables
     └────────┘ └────────┘ └────────┘
```

> Lihat [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) untuk detail.

---

## Struktur Direktori

```
dev/
├── src/main.cpp                 # Dispatcher entry point
├── include/
│   ├── dev.hpp                  # Umbrella header
│   └── dev/
│       ├── version.hpp.in       # CMake template → version.hpp
│       ├── error.hpp            # Error codes
│       ├── style.hpp            # ANSI color utilities
│       ├── config.hpp           # Config file parser
│       ├── process.hpp          # Cross-platform process spawning
│       └── dispatcher.hpp       # Plugin discovery & dispatch
├── examples/                    # Plugin source files
│   ├── hello.cpp, sysinfo.cpp   # Example plugins
│   ├── create.cpp, open.cpp     # Core plugins
│   ├── build.cpp, run.cpp       #
│   ├── clean.cpp                #
│   ├── completion.cpp           # Shell completions
│   └── init-plugin.cpp          # Plugin scaffolding
├── plugins/                     # Built plugin executables
├── docs/                        # Documentation
├── dist/                        # Package manifests
│   ├── scoop/dev.json           # Scoop (Windows)
│   └── homebrew/dev.rb          # Homebrew (macOS)
├── .github/workflows/ci.yml     # CI pipeline
├── dev.toml                     # Config file
├── plugins.toml                 # Plugin metadata
└── CMakeLists.txt               # Build system
```

---

## Quick Start

### Prerequisites

- **CMake** ≥ 3.20
- **Compiler** dengan dukungan C++23 (GCC 13+, Clang 17+, MSVC 19.35+)

### Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

### Shell Completion (opsional)

```bash
# Bash
eval "$(dev completion bash)"

# Zsh
eval "$(dev completion zsh)"

# Fish
dev completion fish > ~/.config/fish/completions/dev.fish

# PowerShell
dev completion pwsh | Invoke-Expression
```

> Untuk panduan lengkap lihat [BUILD.md](BUILD.md) dan [docs/QUICKSTART.md](docs/QUICKSTART.md).

---

## Membuat Plugin

### Cara cepat:

```bash
dev init-plugin my-tool
cd my-tool
cmake -B build && cmake --build build --config Release
cp build/my-tool.exe ../plugins/
```

### Atau manual — buat executable dengan `--help` support:

```cpp
// my-tool.cpp → compile jadi "my-tool" atau "my-tool.exe"
#include <cstring>
#include <print>

int main(int argc, char* argv[]) {
    if (argc > 1 && std::strcmp(argv[1], "--help") == 0) {
        std::println("my-tool — does something cool");
        return 0;
    }
    std::println("Hello from my-tool!");
    return 0;
}
```

Letakkan executable di folder `plugins/`, langsung bisa digunakan.

> Lihat [docs/API.md](docs/API.md) untuk spesifikasi lengkap plugin API.

---

## Konfigurasi

Buat file `dev.toml` di root project:

```toml
editor = "code"
default_template = "cpp"

[plugins]
dirs = ["~/.dev/plugins"]

[alias]
b = "build"
r = "run"
c = "create"
o = "open"
```

---

## Dokumentasi

| Dokumen | Deskripsi |
|---------|-----------|
| [BUILD.md](BUILD.md) | Panduan build & konfigurasi |
| [docs/QUICKSTART.md](docs/QUICKSTART.md) | Panduan memulai cepat |
| [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) | Arsitektur & design decisions |
| [docs/API.md](docs/API.md) | Plugin API reference |
| [docs/CHANGELOG.md](docs/CHANGELOG.md) | Riwayat perubahan |
| [docs/ROADMAP.md](docs/ROADMAP.md) | Roadmap pengembangan |
| [CONTRIBUTING.md](CONTRIBUTING.md) | Panduan kontribusi |
| [SECURITY.md](SECURITY.md) | Security policy |

---

## Kontribusi

Kontribusi sangat diterima! Silakan baca [CONTRIBUTING.md](CONTRIBUTING.md) untuk panduan lengkap.

---

## Lisensi

Proyek ini dilisensikan di bawah **MIT License** — lihat file [LICENSE](LICENSE) untuk detail.

---

## Kontributor

| Username | Email |
|----------|-------|
| [zuudevs](https://github.com/zuudevs) | zuudevs@gmail.com |
| [zuu-inner](https://github.com/zuu-inner) | rafizuhayr001@gmail.com |

---

<p align="center">
  <sub>Built with ❤️ by <a href="https://github.com/zuudevs">zuudevs</a></sub>
</p>
