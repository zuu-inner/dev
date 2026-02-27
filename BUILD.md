# Build Guide

> Panduan lengkap untuk meng-compile `dev` dari source code.

---

## Prerequisites

| Tool | Versi Minimum | Download |
|------|---------------|----------|
| CMake | 3.20 | [cmake.org](https://cmake.org/download/) |
| C++ Compiler (C++23) | GCC 13+ / Clang 17+ / MSVC 19.35+ | — |
| Git | Terbaru | [git-scm.com](https://git-scm.com/) |

---

## Clone

```bash
git clone https://github.com/zuudevs/dev.git
cd dev
```

---

## Build Configurations

### Release Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Output binary: `build/bin/Dev` (atau `build/bin/Release/Dev.exe` di Windows MSVC).

### Debug Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
```

### Windows — Visual Studio Generator

```powershell
# Generate Visual Studio solution
cmake -B build -G "Visual Studio 17 2022"

# Build dari command line
cmake --build build --config Release

# Atau buka build/Dev.sln di Visual Studio
```

### Windows — Ninja Generator

```powershell
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

---

## Compiler Flags

`dev` menggunakan compiler flags yang ketat untuk memastikan kualitas kode:

### MSVC

| Flag | Deskripsi |
|------|-----------|
| `/W4` | Warning level 4 (hampir semua warning) |
| `/permissive-` | Strict standards conformance |
| `/utf-8` | UTF-8 source & execution charset |
| `/O2` | Optimisasi Release |

### GCC / Clang

| Flag | Deskripsi |
|------|-----------|
| `-Wall` | Semua warning umum |
| `-Wextra` | Warning tambahan |
| `-Wpedantic` | Strict ISO C++ compliance |
| `-Wconversion` | Warning implicit conversion |
| `-Wshadow` | Warning variable shadowing |
| `-O3` | Optimisasi Release |

---

## Output Directories

| Output | Lokasi |
|--------|--------|
| Binary (executable) | `build/bin/` |
| Static Libraries | `lib/` |
| Shared Libraries | `lib/` |

---

## Build dari Scratch (Clean Build)

```bash
# Hapus folder build lama
rm -rf build/    # Linux/macOS
rmdir /s build   # Windows CMD

# Lalu configure & build ulang
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

---

## CMake Variables

| Variable | Default | Deskripsi |
|----------|---------|-----------|
| `CMAKE_BUILD_TYPE` | — | `Debug`, `Release`, `RelWithDebInfo`, `MinSizeRel` |
| `CMAKE_CXX_STANDARD` | `23` | Standar C++ yang digunakan |
| `CMAKE_RUNTIME_OUTPUT_DIRECTORY` | `${CMAKE_BINARY_DIR}/bin` | Lokasi output binary |

---

## Troubleshooting

### Error: C++23 tidak didukung

```
error: 'std::print' is not a member of 'std'
```

**Solusi:** Upgrade compiler ke versi yang mendukung C++23. Lihat tabel prerequisites di atas.

### Error: CMake versi terlalu lama

```
CMake Error: CMake 3.20 or higher is required.
```

**Solusi:** Download CMake terbaru dari [cmake.org](https://cmake.org/download/).
