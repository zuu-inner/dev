# Plugin API Reference

> Dokumentasi lengkap tentang cara membuat, menginstall, dan mengonfigurasi plugin untuk `dev`.

---

## Daftar Isi

- [Overview](#overview)
- [Kontrak Plugin](#kontrak-plugin)
- [Argument Passing](#argument-passing)
- [Exit Codes](#exit-codes)
- [Lokasi Plugin](#lokasi-plugin)
- [Contoh Plugin](#contoh-plugin)
- [Best Practices](#best-practices)
- [Troubleshooting](#troubleshooting)

---

## Overview

Plugin `dev` adalah **executable mandiri** yang dipanggil oleh dispatcher `dev`. Tidak ada library yang perlu di-link, tidak ada interface yang perlu di-implement — cukup buat executable dengan nama yang sesuai.

```
dev <command> [args...]
     │          │
     │          └── Diteruskan ke plugin sebagai argv[1..n]
     │
     └── Nama plugin executable yang dicari di plugins/
```

---

## Kontrak Plugin

Setiap plugin **HARUS** memenuhi spesifikasi berikut:

### Wajib

| Requirement | Deskripsi |
|-------------|-----------|
| **Executable** | File harus bisa dieksekusi oleh OS |
| **Nama file** | Nama file (tanpa ekstensi di Linux/macOS) menjadi nama command |
| **Exit code** | Return `0` untuk sukses, non-zero untuk error |

### Opsional (Rekomendasi)

| Requirement | Deskripsi |
|-------------|-----------|
| **`--help` flag** | Menampilkan bantuan penggunaan |
| **`--version` flag** | Menampilkan versi plugin |
| **Stderr untuk error** | Gunakan stderr untuk pesan error, stdout untuk output normal |

---

## Argument Passing

`dev` meneruskan argumen **apa adanya** ke plugin. Mapping-nya:

```
$ dev create my-app --template cpp --verbose

Plugin menerima:
  argv[0] = "plugins/create"     (atau path penuh)
  argv[1] = "my-app"
  argv[2] = "--template"
  argv[3] = "cpp"
  argv[4] = "--verbose"
  argc    = 5
```

> **Catatan:** `argv[0]` adalah path ke plugin itu sendiri, bukan string `"dev"`.

---

## Exit Codes

Plugin berkomunikasi status via exit code:

| Kode | Arti |
|------|------|
| `0` | Sukses |
| `1` | Error umum |
| `2` | Penggunaan salah (invalid arguments) |
| `126` | Command found tapi tidak bisa dieksekusi |
| `127` | Command not found (digunakan internal oleh `dev`) |

`dev` akan **meneruskan exit code plugin** langsung ke shell pemanggil:

```bash
dev build
echo $?   # Exit code dari plugin "build", bukan dari "dev"
```

---

## Lokasi Plugin

Plugin harus ditempatkan di folder `plugins/` relatif terhadap binary `dev`:

```
dev                 ← dispatcher binary
plugins/
├── create          ← plugin "create"
├── open            ← plugin "open"
├── build           ← plugin "build"
└── deploy          ← plugin "deploy"
```

### Penamaan per Platform

| Platform | Nama File | Contoh |
|----------|-----------|--------|
| Linux | `<nama>` (tanpa ekstensi) | `plugins/create` |
| macOS | `<nama>` (tanpa ekstensi) | `plugins/create` |
| Windows | `<nama>.exe` | `plugins\create.exe` |

---

## Contoh Plugin

### C — Plugin Minimal

```c
/* plugins/hello.c */
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        printf("Usage: dev hello [name]\n");
        printf("  Prints a hello message.\n");
        return 0;
    }

    const char* name = (argc > 1) ? argv[1] : "World";
    printf("Hello, %s!\n", name);
    return 0;
}
```

```bash
# Compile
gcc -o plugins/hello plugins/hello.c

# Gunakan
dev hello Rafi
# Output: Hello, Rafi!
```

### C++ — Plugin dengan Argument Parsing

```cpp
// plugins/init.cpp
#include <iostream>
#include <string>
#include <string_view>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: dev init <project-name> [--lang <language>]\n";
        return 2;
    }

    std::string_view project_name = argv[1];
    std::string lang = "cpp";

    for (int i = 2; i < argc - 1; ++i) {
        if (std::string_view(argv[i]) == "--lang") {
            lang = argv[i + 1];
        }
    }

    std::cout << "Initializing project: " << project_name 
              << " (language: " << lang << ")\n";

    // ... logic pembuatan project
    return 0;
}
```

### Python — Script Plugin

```python
#!/usr/bin/env python3
"""plugins/lint — Run linter pada project."""

import sys
import subprocess

def main():
    if "--help" in sys.argv:
        print("Usage: dev lint [--fix] [path...]")
        print("  Run linter pada source files.")
        return 0

    fix_mode = "--fix" in sys.argv
    paths = [a for a in sys.argv[1:] if not a.startswith("--")]

    if not paths:
        paths = ["src/", "include/"]

    cmd = ["clang-tidy"]
    if fix_mode:
        cmd.append("--fix")
    cmd.extend(paths)

    return subprocess.run(cmd).returncode

if __name__ == "__main__":
    sys.exit(main())
```

> **Catatan:** Untuk Linux/macOS, pastikan file script memiliki shebang line dan permission executable (`chmod +x`).

### Bash — Shell Script Plugin

```bash
#!/bin/bash
# plugins/clean — Bersihkan build artifacts

set -e

if [[ "$1" == "--help" ]]; then
    echo "Usage: dev clean [--all]"
    echo "  Hapus folder build dan artifacts."
    exit 0
fi

echo "Cleaning build directory..."
rm -rf build/

if [[ "$1" == "--all" ]]; then
    echo "Cleaning all artifacts..."
    rm -rf bin/ lib/
fi

echo "Done."
```

---

## Best Practices

### ✅ Do

- **Implementasi `--help`** — Semua plugin sebaiknya menampilkan usage info
- **Gunakan exit code yang benar** — `0` sukses, `1` error, `2` usage error
- **Tulis ke stderr untuk error** — `stderr` untuk error/warning, `stdout` untuk output
- **Buat plugin stateless** — Hindari menyimpan state global antar pemanggilan
- **Dokumentasikan plugin** — Tambahkan deskripsi singkat di README atau file terpisah

### ❌ Don't

- **Jangan hardcode path** — Gunakan path relatif atau environment variables
- **Jangan assume working directory** — Working directory bisa dari mana saja
- **Jangan block tanpa timeout** — Hindari operasi yang bisa hang tanpa batas waktu
- **Jangan print ke stdout untuk debug** — Gunakan stderr atau flag `--verbose`

---

## Troubleshooting

### Plugin tidak ditemukan

```
dev: error: command 'foo' not found
```

**Solusi:**
1. Pastikan executable ada di folder `plugins/`
2. Pastikan nama file sesuai (tanpa ekstensi di Linux/macOS, `.exe` di Windows)
3. Pastikan file memiliki permission executable: `chmod +x plugins/foo`

### Permission denied

```
dev: error: permission denied: 'plugins/foo'
```

**Solusi:**
```bash
chmod +x plugins/foo
```

### Plugin crash

Jika plugin crash, `dev` akan meneruskan exit code dari OS (biasanya 139 untuk segfault). Debug plugin secara independen:

```bash
# Jalankan plugin langsung tanpa dev
./plugins/foo arg1 arg2
```
