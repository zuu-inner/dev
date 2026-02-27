# Quick Start

> Panduan memulai `dev` dari nol — build, jalankan, dan buat plugin pertama dalam 5 menit.

---

## Prerequisites

| Tool | Versi Minimum | Cek Versi |
|------|---------------|-----------|
| CMake | 3.20 | `cmake --version` |
| C++ Compiler | C++23 support | `g++ --version` / `cl` |
| Git | Terbaru | `git --version` |

### Compiler yang Didukung

| Compiler | Versi Minimum |
|----------|---------------|
| GCC | 13+ |
| Clang | 17+ |
| MSVC | 19.35+ (VS 2022 17.5+) |

---

## 1. Clone Repository

```bash
git clone https://github.com/zuudevs/dev.git
cd dev
```

---

## 2. Build

### Linux / macOS

```bash
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build

# Binary ada di build/bin/dev
```

### Windows (Visual Studio)

```powershell
# Configure
cmake -B build

# Build (Release)
cmake --build build --config Release

# Binary ada di build\bin\Release\Dev.exe
```

### Windows (MinGW)

```bash
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

---

## 3. Verifikasi Instalasi

```bash
# Jalankan tanpa argumen — tampilkan help
./build/bin/dev

# Output:
# usage: dev <command> [options]
```

---

## 4. (Opsional) Install ke PATH

### Linux / macOS

```bash
# Copy binary ke /usr/local/bin
sudo cp build/bin/dev /usr/local/bin/dev

# Copy folder plugins
sudo mkdir -p /usr/local/lib/dev/plugins
sudo cp -r plugins/* /usr/local/lib/dev/plugins/

# Sekarang bisa digunakan dari mana saja
dev --help
```

### Windows

```powershell
# Tambahkan folder build\bin\ ke PATH
# Atau copy Dev.exe ke folder yang sudah ada di PATH
```

---

## 5. Buat Plugin Pertama

Mari buat plugin sederhana bernama `hello`:

### Step 1: Tulis kode plugin

```c
/* Simpan sebagai: hello.c */
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc > 1) {
        printf("Hello, %s! Welcome to dev.\n", argv[1]);
    } else {
        printf("Hello, World! Welcome to dev.\n");
    }
    return 0;
}
```

### Step 2: Compile

```bash
# Linux / macOS
gcc -o plugins/hello hello.c

# Windows (MSVC)
cl /Fe:plugins\hello.exe hello.c
```

### Step 3: Jalankan via `dev`

```bash
dev hello
# Output: Hello, World! Welcome to dev.

dev hello Rafi
# Output: Hello, Rafi! Welcome to dev.
```

🎉 **Selesai!** Anda telah membuat dan menjalankan plugin pertama.

---

## Langkah Selanjutnya

- 📖 Baca [Plugin API Reference](API.md) untuk spesifikasi lengkap
- 🏗️ Pelajari [Architecture](ARCHITECTURE.md) untuk memahami cara kerja internal
- 🗺️ Lihat [Roadmap](ROADMAP.md) untuk fitur yang akan datang
- 🤝 Baca [Contributing](../CONTRIBUTING.md) untuk berkontribusi
