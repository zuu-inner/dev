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

## ✨ Fitur Utama

| Fitur | Deskripsi |
|-------|-----------|
| **Zero-rebuild extension** | Tambah command baru tanpa kompilasi ulang dispatcher |
| **Plugin berbasis executable** | Tiap plugin adalah program mandiri (bahasa apapun) |
| **Argument forwarding** | Seluruh `[args...]` diteruskan apa adanya ke plugin |
| **Exit code propagation** | Exit code plugin diteruskan kembali ke shell |
| **Cross-platform** | Mendukung Windows (MSVC) dan Linux/macOS (GCC/Clang) |
| **Modern C++23** | Dibangun dengan standar C++ terbaru |

---

## 🏗️ Arsitektur

```
┌─────────────────────────────────────────────────┐
│                    User Shell                    │
│              $ dev create my-app                 │
└──────────────────────┬──────────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────────┐
│              dev  (dispatcher)                   │
│                                                  │
│  1. Parse argv[1] → command name ("create")      │
│  2. Cari plugin di plugins/ folder               │
│  3. Exec plugin + forward argv[2..n]             │
│  4. Propagate exit code                          │
└──────────────────────┬──────────────────────────┘
                       │
          ┌────────────┼────────────┐
          ▼            ▼            ▼
     ┌─────────┐ ┌─────────┐ ┌─────────┐
     │ create  │ │  open   │ │  build  │  ← plugin executables
     └─────────┘ └─────────┘ └─────────┘
```

> Lihat [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) untuk penjelasan arsitektur lengkap.

---

## 📁 Struktur Direktori

```
dev/
├── src/
│   └── main.cpp              # Entry point dispatcher
├── include/
│   ├── dev.hpp                # Project metadata (version, author)
│   ├── cli/
│   │   └── command.hpp        # Command struct & registry
│   ├── config/
│   │   └── project.hpp        # Version macros
│   ├── core/
│   │   ├── error.hpp          # Error codes
│   │   └── registry.hpp       # Plugin registry
│   └── utils/
│       └── system.hpp         # Filesystem utilities
├── plugins/                   # Drop plugin executables di sini
├── bin/                       # Build output
├── lib/                       # Library output
├── scripts/                   # Helper scripts
├── tests/                     # Unit & integration tests
├── examples/                  # Contoh plugin
├── docs/                      # Dokumentasi lengkap
│   ├── ARCHITECTURE.md
│   ├── API.md
│   ├── QUICKSTART.md
│   ├── CHANGELOG.md
│   └── ROADMAP.md
├── CMakeLists.txt             # Build system
├── BUILD.md                   # Panduan build
├── CONTRIBUTING.md            # Panduan kontribusi
├── CODE_OF_CONDUCT.md         # Code of conduct
├── SECURITY.md                # Security policy
└── LICENSE                    # Lisensi
```

---

## 🚀 Quick Start

### Prerequisites

- **CMake** ≥ 3.20
- **Compiler** dengan dukungan C++23 (GCC 13+, Clang 17+, MSVC 19.35+)

### Build

```bash
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# Binary tersedia di build/bin/
```

### Jalankan

```bash
# Tampilkan help
dev

# Jalankan plugin "create" dengan argumen
dev create my-project --template cpp
```

> Untuk panduan lengkap lihat [BUILD.md](BUILD.md) dan [docs/QUICKSTART.md](docs/QUICKSTART.md).

---

## 🔌 Membuat Plugin

Membuat plugin untuk `dev` sangat sederhana — cukup buat executable dengan nama command yang diinginkan.

**Contoh plugin `hello` (C):**

```c
// plugins/hello.c → compile jadi "hello" atau "hello.exe"
#include <stdio.h>

int main(int argc, char* argv[]) {
    printf("Hello from dev plugin!\n");
    for (int i = 1; i < argc; i++) {
        printf("  arg[%d] = %s\n", i, argv[i]);
    }
    return 0;
}
```

**Contoh plugin `greet` (Python script — dengan shebang):**

```python
#!/usr/bin/env python3
# plugins/greet
import sys

print(f"Greetings! Args: {sys.argv[1:]}")
```

Letakkan executable di folder `plugins/`, dan langsung bisa digunakan:

```bash
dev hello world
# Output: Hello from dev plugin!
#           arg[1] = world
```

> Lihat [docs/API.md](docs/API.md) untuk spesifikasi lengkap plugin API.

---

## 📖 Dokumentasi

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

## 🤝 Kontribusi

Kontribusi sangat diterima! Silakan baca [CONTRIBUTING.md](CONTRIBUTING.md) untuk panduan lengkap.

---

## 📄 Lisensi

Proyek ini dilisensikan di bawah **MIT License** — lihat file [LICENSE](LICENSE) untuk detail.

---

## 👥 Kontributor

| Username | Email |
|----------|-------|
| [zuudevs](https://github.com/zuudevs) | zuudevs@gmail.com |
| [zuu-inner](https://github.com/zuu-inner) | rafizuhayr001@gmail.com |

---

<p align="center">
  <sub>Built with ❤️ by <a href="https://github.com/zuudevs">zuudevs</a></sub>
</p>
