# Arsitektur `dev`

> Dokumen ini menjelaskan arsitektur internal `dev` CLI dispatcher, design decisions, dan bagaimana tiap komponen saling berhubungan.

---

## Daftar Isi

- [Filosofi Desain](#filosofi-desain)
- [Overview Arsitektur](#overview-arsitektur)
- [Alur Dispatch](#alur-dispatch)
- [Komponen Internal](#komponen-internal)
- [Plugin System](#plugin-system)
- [Error Handling](#error-handling)
- [Cross-Platform Support](#cross-platform-support)

---

## Filosofi Desain

`dev` dibangun dengan prinsip-prinsip berikut:

1. **Single Responsibility** — `dev` hanya satu hal: *dispatch*. Seluruh logika bisnis ada di plugin.
2. **Zero Coupling** — Plugin tidak perlu link ke library `dev`. Plugin adalah executable mandiri.
3. **Convention over Configuration** — Plugin dikenali dari nama file-nya, bukan dari file konfigurasi.
4. **Fail Fast** — Jika command tidak ditemukan, beri pesan error yang jelas dan keluar.
5. **Transparent Forwarding** — Argumen dan exit code diteruskan tanpa modifikasi.

---

## Overview Arsitektur

```
                    ┌──────────────────────────────┐
                    │          User Shell           │
                    │    $ dev build --release      │
                    └──────────────┬───────────────┘
                                   │
                                   ▼
┌──────────────────────────────────────────────────────────────┐
│                     dev (dispatcher binary)                   │
│                                                               │
│  ┌───────────────┐  ┌────────────────┐  ┌──────────────────┐ │
│  │   CLI Parser  │→ │ Plugin Resolver │→ │ Process Executor │ │
│  │  (cli/)       │  │  (core/)        │  │  (utils/)        │ │
│  └───────────────┘  └────────────────┘  └──────────────────┘ │
│                                                               │
│  ┌───────────────┐  ┌────────────────┐                       │
│  │ Config/Version│  │ Error Handler  │                       │
│  │ (config/)     │  │ (core/)        │                       │
│  └───────────────┘  └────────────────┘                       │
└──────────────────────────────┬───────────────────────────────┘
                               │
              ┌────────────────┼────────────────┐
              ▼                ▼                ▼
        ┌──────────┐   ┌──────────┐    ┌──────────┐
        │  create  │   │   open   │    │  build   │
        │ (plugin) │   │ (plugin) │    │ (plugin) │
        └──────────┘   └──────────┘    └──────────┘
                    plugins/ directory
```

---

## Alur Dispatch

Berikut alur lengkap saat user menjalankan `dev <command> [args...]`:

```
1. main() menerima argc, argv
       │
       ▼
2. Validasi: apakah ada argumen?
   ├── Tidak  → Tampilkan usage/help → exit(0)
   └── Ya     → Lanjut
       │
       ▼
3. Ambil command name dari argv[1]
       │
       ▼
4. Resolve plugin path:
   plugins/<command>        (Linux/macOS)
   plugins/<command>.exe    (Windows)
       │
       ▼
5. Cek apakah plugin executable ada?
   ├── Tidak  → Error: "command not found" → exit(1)
   └── Ya     → Lanjut
       │
       ▼
6. Execute plugin:
   - Forward argv[2..n] sebagai argumen
   - Tunggu proses selesai
       │
       ▼
7. Propagate exit code plugin → exit(code)
```

---

## Komponen Internal

### `include/dev.hpp` — Project Metadata

Header utama yang mendefinisikan metadata proyek:

```cpp
namespace dev {
    constexpr std::string_view version;     // Versi dari CMake
    constexpr std::string_view author;      // Nama author
    constexpr std::string_view email;       // Email author
    constexpr std::string_view repository;  // URL repository
}
```

### `include/config/project.hpp` — Version Macros

Macro versi untuk conditional compilation:

```cpp
#define DEV_VERSION_MAJOR 0
#define DEV_VERSION_MINOR 1
#define DEV_VERSION_PATCH 0
#define DEV_VERSION ((MAJOR << 16) | (MINOR << 8) | PATCH)
// Contoh: v0.1.0 → 0x000100
```

Digunakan untuk feature-gating, contoh:

```cpp
#if DEV_VERSION >= 0x00000100
// Fitur yang tersedia sejak v0.1.0
#endif
```

### `include/cli/command.hpp` — Command Registry

Mendefinisikan struktur `Command` dan global registry:

```cpp
namespace dev::cli {
    struct Command {
        std::string_view name;          // Nama command (e.g., "create")
        void(*callback)(int, char**);   // Fungsi callback (untuk built-in commands)
    };

    static std::vector<Command> g_commands;  // Registry global
}
```

> **Catatan:** Registry ini digunakan untuk *built-in commands* (bukan plugin external). Plugin external di-resolve langsung dari filesystem.

### `include/core/error.hpp` — Error Codes

Enum error codes untuk penanganan kesalahan:

```cpp
namespace dev::core {
    enum class Error {
        // Error codes akan ditambahkan sesuai kebutuhan
    };
}
```

### `include/core/registry.hpp` — Plugin Registry

Modul untuk registrasi dan lookup plugin. Saat ini dalam pengembangan.

### `include/utils/system.hpp` — System Utilities

Utility class untuk operasi filesystem dan system calls:

```cpp
namespace dev::utils {
    class System {
        static void create_file(std::string_view path, std::string_view content) noexcept;
    };
}
```

### `src/main.cpp` — Entry Point

Entry point utama yang menangani parsing argumen dan dispatch:

```cpp
int main(int argc, char* argv[]) {
    // 1. Cek argumen
    // 2. Dispatch ke plugin atau tampilkan help
    // 3. Return exit code
}
```

---

## Plugin System

### Konvensi Penamaan

| Platform | Nama Plugin | Path |
|----------|-------------|------|
| Linux/macOS | `create` | `plugins/create` |
| Windows | `create.exe` | `plugins/create.exe` |

### Kontrak Plugin

Setiap plugin harus memenuhi kontrak berikut:

1. **Executable** — Plugin harus berupa file executable
2. **Nama = Command** — Nama file (tanpa ekstensi) = nama command
3. **Exit Code** — Return `0` untuk sukses, non-zero untuk error
4. **Argumen** — Menerima argumen via `argc`/`argv` standar
5. **Lokasi** — Ditempatkan di folder `plugins/`

### Bahasa yang Didukung

Plugin bisa ditulis dalam bahasa apapun yang menghasilkan executable:

- **C/C++** — Compile langsung
- **Rust** — Compile via `cargo build`
- **Go** — Compile via `go build`
- **Python** — Dengan shebang `#!/usr/bin/env python3` (Linux/macOS)
- **Shell Script** — Dengan shebang `#!/bin/bash` (Linux/macOS)
- **Dan lainnya...**

---

## Error Handling

| Kondisi | Behavior |
|---------|----------|
| Tidak ada argumen (`dev`) | Tampilkan help/usage |
| Command tidak ditemukan | Error message + exit(1) |
| Plugin gagal dieksekusi | Error message + exit(1) |
| Plugin return non-zero | Forward exit code ke shell |

---

## Cross-Platform Support

`dev` dirancang untuk berjalan di multiple platform:

| Platform | Compiler | Status |
|----------|----------|--------|
| Windows | MSVC 19.35+ | ✅ Didukung |
| Linux | GCC 13+ | ✅ Didukung |
| macOS | Clang 17+ | ✅ Didukung |

### Platform-Specific Notes

- **Windows:** Plugin harus memiliki ekstensi `.exe`
- **Linux/macOS:** Plugin harus memiliki permission `chmod +x`
- **CMake** menangani perbedaan compiler flags secara otomatis
