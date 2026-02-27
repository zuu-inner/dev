# Build Guide

> Panduan lengkap untuk meng-compile `dev` v1.0.0 dari source code.

---

## Prerequisites

| Tool | Versi Minimum | Download |
|------|---------------|----------|
| CMake | 3.20 | [cmake.org](https://cmake.org/download/) |
| C++ Compiler (C++23) | GCC 13+ / Clang 17+ / MSVC 19.35+ | — |

---

## Build

### Release (recommended)

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

### Debug

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
```

### Windows — Visual Studio

```powershell
cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
# Atau buka build/Dev.sln di Visual Studio
```

### Windows — Ninja

```powershell
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

---

## Output

| Output | Lokasi |
|--------|--------|
| Dispatcher | `build/bin/Release/Dev.exe` (Windows) / `build/bin/Dev` (Linux) |
| Plugins | `plugins/` (9 plugins bawaan) |
| Generated headers | `build/include/dev/version.hpp` |

---

## CMake Options

| Variable | Default | Deskripsi |
|----------|---------|-----------|
| `CMAKE_BUILD_TYPE` | — | `Debug`, `Release`, `RelWithDebInfo` |
| `DEV_BUILD_EXAMPLES` | `ON` | Build example + core plugins ke `plugins/` |

Disable plugin build:

```bash
cmake -B build -DDEV_BUILD_EXAMPLES=OFF
```

---

## Compiler Flags

### MSVC

| Flag | Deskripsi |
|------|-----------|
| `/W4` | Warning level 4 |
| `/permissive-` | Strict standards conformance |
| `/utf-8` | UTF-8 source & execution charset |

### GCC / Clang

| Flag | Deskripsi |
|------|-----------|
| `-Wall -Wextra` | Semua warning |
| `-Wpedantic` | Strict ISO C++ |
| `-Wconversion` | Implicit conversion warning |
| `-Wshadow` | Variable shadowing warning |

---

## Clean Build

```bash
# Hapus build lama
rm -rf build/              # Linux/macOS
Remove-Item -Recurse build # PowerShell

# Build ulang
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

---

## Verify Build

```bash
./build/bin/Release/Dev.exe --version    # → dev v1.0.0
./build/bin/Release/Dev.exe list         # → 9 plugins
./build/bin/Release/Dev.exe hello World  # → Hello, World!
```

---

## Troubleshooting

### C++23 tidak didukung

```
error: 'std::print' is not a member of 'std'
```

**Solusi:** Upgrade ke GCC 13+, Clang 17+, atau MSVC 19.35+.

### CMake versi terlalu lama

```
CMake Error: CMake 3.20 or higher is required.
```

**Solusi:** Download CMake terbaru dari [cmake.org](https://cmake.org/download/).

### Target 'clean' reserved

CMake targets `build` dan `clean` di-prefix dengan `dev_` secara internal.
Output binary tetap menggunakan nama asli via `OUTPUT_NAME`.
