# Plugin API Reference

> Cara membuat, menginstall, dan mengonfigurasi plugin untuk `dev` v1.0.0.

---

## Overview

Plugin `dev` adalah **executable mandiri** — tidak ada library yang perlu di-link. Cukup buat executable, letakkan di `plugins/`, selesai.

```
dev <command> [args...]
     │          │
     │          └── Diteruskan ke plugin sebagai argv[1..n]
     └── Nama plugin executable di plugins/
```

---

## Kontrak Plugin

### Wajib

| Requirement | Deskripsi |
|-------------|-----------|
| **Executable** | File harus bisa dieksekusi oleh OS |
| **Nama file** | Nama file (tanpa `.exe`) = nama command |
| **Exit code** | Return `0` sukses, non-zero error |

### Rekomendasi

| Requirement | Deskripsi |
|-------------|-----------|
| **`--help` flag** | Menampilkan usage — digunakan oleh `dev help <cmd>` |
| **Stderr untuk error** | Stdout = output, stderr = error |
| **Metadata** | Tambahkan entry di `plugins.toml` untuk deskripsi |

---

## Membuat Plugin

### Cara Cepat (scaffolding):

```bash
dev init-plugin my-tool
cd my-tool
cmake -B build && cmake --build build --config Release
cp build/my-tool.exe ../plugins/    # Windows
cp build/my-tool ../plugins/        # Linux/macOS
```

### Manual (C++23):

```cpp
#include <cstring>
#include <print>

int main(int argc, char* argv[]) {
    if (argc > 1 && std::strcmp(argv[1], "--help") == 0) {
        std::println("my-tool — description");
        std::println("");
        std::println("usage: dev my-tool [args...]");
        return 0;
    }

    std::println("Hello from my-tool!");
    return 0;
}
```

### Plugin dalam bahasa lain:

**Python** (Linux/macOS):
```python
#!/usr/bin/env python3
import sys
print(f"Args: {sys.argv[1:]}")
```

**Bash**:
```bash
#!/bin/bash
echo "Hello from bash plugin! Args: $@"
```

> Di Linux/macOS, pastikan `chmod +x` pada script plugins.

---

## Argument Passing

`dev` meneruskan argumen **apa adanya**:

```
$ dev create my-app --template cpp

Plugin menerima:
  argv[0] = "plugins/create"     (path ke plugin)
  argv[1] = "my-app"
  argv[2] = "--template"
  argv[3] = "cpp"
  argc    = 4
```

> **Catatan:** `argv[0]` adalah path ke plugin, bukan `"dev"`.

---

## Exit Codes

| Kode | Arti |
|------|------|
| `0` | Sukses |
| `1` | Error umum |
| `2` | Penggunaan salah (invalid arguments) |
| `126` | Plugin found tapi tidak executable |
| `127` | Command not found (internal `dev`) |

Exit code plugin diteruskan langsung ke shell:

```bash
dev build
echo $?   # Exit code dari plugin build, bukan dari dev
```

---

## Plugin Discovery

### Search Order

| Prioritas | Lokasi | Deskripsi |
|-----------|--------|-----------|
| 1 | `<exe-dir>/plugins/` | Relatif ke binary `dev` |
| 2 | `<cwd>/plugins/` | Relatif ke working directory |
| 3 | Config dirs | Dari `[plugins] dirs` di `dev.toml` |

### Penamaan per Platform

| Platform | Nama File |
|----------|-----------|
| Linux/macOS | `plugins/<name>` |
| Windows | `plugins\<name>.exe` |

---

## Plugin Metadata

Tambahkan entry di `plugins.toml` untuk deskripsi:

```toml
[my-tool]
description = "Does something cool"
```

Deskripsi ditampilkan di `dev list`:

```
Available commands:
  my-tool        Does something cool
```

---

## Process Execution

`dev` menggunakan process spawning langsung (bukan `std::system()`):

| Platform | Method |
|----------|--------|
| Windows | `_spawnv(P_WAIT, ...)` |
| POSIX | `fork()` + `execvp()` + `waitpid()` |

Keuntungan: no shell interpretation, argument passing aman, exit code akurat.

---

## Internal Headers

Plugin yang ingin menggunakan dev library headers (opsional):

```cpp
#include "dev/config.hpp"    // Config file parser
#include "dev/process.hpp"   // spawn()
#include "dev/style.hpp"     // ANSI colors
#include "dev/error.hpp"     // Error codes
```

> Plugin tidak wajib depend pada headers ini — mereka sepenuhnya opsional.

---

## Best Practices

### ✅ Do

- Implementasi `--help` — digunakan oleh `dev help <cmd>`
- Gunakan exit code yang benar
- Tulis ke stderr untuk error
- Buat plugin stateless
- Tambahkan metadata di `plugins.toml`

### ❌ Don't

- Jangan hardcode path
- Jangan assume working directory
- Jangan block tanpa timeout
- Jangan print debug ke stdout (gunakan stderr atau `--verbose`)
