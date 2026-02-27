# Arsitektur `dev`

> Dokumen ini menjelaskan arsitektur internal `dev` v1.0.0 — design decisions, komponen, dan alur dispatch.

---

## Filosofi Desain

1. **Single Responsibility** — `dev` hanya *dispatch*. Seluruh logika bisnis ada di plugin.
2. **Zero Coupling** — Plugin tidak perlu link ke library `dev`. Plugin adalah executable mandiri.
3. **Convention over Configuration** — Plugin dikenali dari nama file-nya.
4. **Fail Fast** — Jika command tidak ditemukan, beri pesan error yang jelas (berwarna).
5. **Transparent Forwarding** — Argumen dan exit code diteruskan tanpa modifikasi.

---

## Overview

```mermaid
graph TB
    subgraph Shell["User Shell"]
        CMD["$ dev build --release"]
    end

    subgraph Dispatcher["dev (dispatcher)"]
        direction TB
        CONFIG["Config<br/>dev.toml + aliases"]
        STYLE["Style<br/>ANSI colors + TTY"]
        DISPATCH["Dispatcher<br/>resolve + spawn"]
        VERSION["Version<br/>metadata"]
        ERROR["Error<br/>codes"]
        PROCESS["Process<br/>_spawnv / fork"]
    end

    subgraph Plugins["plugins/ directory"]
        direction LR
        P1["create"]
        P2["build"]
        P3["run"]
        P4["clean"]
        P5["open"]
        P6["completion"]
        P7["init-plugin"]
    end

    CMD --> Dispatcher
    DISPATCH --> Plugins
    CONFIG --> DISPATCH
    STYLE --> DISPATCH
    PROCESS --> DISPATCH
```

---

## Alur Dispatch

```mermaid
flowchart TD
    A["main(argc, argv)"] --> B["Init colors + load config"]
    B --> C["Pre-scan flags<br/>--verbose, --quiet"]
    C --> D{"Alias?"}
    D -->|"Ya: b → build"| E["Resolve alias"]
    D -->|Tidak| F{"Built-in?"}
    E --> F

    F -->|"--version"| G["Print version → exit"]
    F -->|"--help"| H["Print help colored → exit"]
    F -->|"list"| I["list_plugins + metadata → exit"]
    F -->|"help cmd"| J["spawn plugin --help → exit"]
    F -->|"Plugin cmd"| K["Resolve plugin multi-path"]

    K --> L{"Found?"}
    L -->|Tidak| M["Error merah + exit 127"]
    L -->|Ya| N["spawn(plugin, argv)"]
    N --> O["Propagate exit code"]
```

---

## Plugin Search Order

```mermaid
flowchart LR
    A["resolve_plugin(name)"] --> B["exe-dir/plugins/"]
    B -->|"not found"| C["cwd/plugins/"]
    C -->|"not found"| D["config dirs"]
    D -->|"not found"| E["Error 127"]
    B -->|"found"| F["Return path"]
    C -->|"found"| F
    D -->|"found"| F
```

---

## Komponen Internal

### Header Map

```mermaid
graph TD
    DEV["dev.hpp<br/>(umbrella)"]
    DEV --> VER["version.hpp<br/>CMake generated"]
    DEV --> ERR["error.hpp<br/>exit codes"]
    DEV --> STY["style.hpp<br/>ANSI colors"]
    DEV --> CFG["config.hpp<br/>TOML parser"]
    DEV --> PRC["process.hpp<br/>spawn"]
    DEV --> DIS["dispatcher.hpp<br/>resolve + dispatch"]

    DIS --> CFG
    DIS --> ERR
    DIS --> PRC
    DIS --> STY

    style DEV fill:#0d6efd,color:#fff
    style DIS fill:#198754,color:#fff
    style CFG fill:#6f42c1,color:#fff
    style STY fill:#fd7e14,color:#fff
```

### `dev/version.hpp` — Project Metadata

Auto-generated oleh CMake dari `version.hpp.in`:

```cpp
namespace dev {
    constexpr std::string_view version    = "1.0.0";
    constexpr std::string_view author     = "zuudevs";
    constexpr std::string_view build_type = "Release";
}
```

### `dev/error.hpp` — Error Codes

```cpp
namespace dev {
    enum class Error {
        InvalidUsage     = 2,
        PermissionDenied = 126,
        CommandNotFound  = 127,
    };
}
```

### `dev/style.hpp` — Terminal Colors

- `style::init()` — enable Windows VT + detect TTY
- `style::bold_text("text")` — styled string (plain jika piped)
- Colors: `red`, `green`, `yellow`, `cyan`, `gray`, `dim`, `bold`

### `dev/config.hpp` — Configuration

Parser INI/TOML-like untuk `dev.toml`:
- `Config::find(argv0)` — search `./dev.toml` → global config
- `Config::get(section, key)` / `get_list()` / `get_section()`

### `dev/process.hpp` — Process Spawning

- **Windows:** `_spawnv(P_WAIT, ...)`
- **POSIX:** `fork()` + `execvp()` + `waitpid()`

### `dev/dispatcher.hpp` — Plugin Discovery

- `find_all_plugin_dirs()` — exe-relative + cwd + config
- `resolve_plugin()` / `list_plugins()` / `dispatch()`

---

## Config Search

```mermaid
flowchart TD
    A["Config::find()"] --> B{"./dev.toml?"}
    B -->|exists| C["Load local config"]
    B -->|not found| D{"Global config?"}
    D -->|"Windows: %APPDATA%/dev/config.toml"| E["Load global"]
    D -->|"POSIX: ~/.config/dev/config.toml"| E
    D -->|not found| F["Empty config"]
```

---

## Plugin System

### Built-in Plugins

```mermaid
graph LR
    subgraph Core["Core (v0.2)"]
        create
        open
        build
        run
        clean
    end

    subgraph DX["DX (v0.4+)"]
        completion
        direction TB
        init-plugin["init-plugin"]
    end

    subgraph Examples["Examples"]
        hello
        sysinfo
    end
```

### Plugin Contract

| Requirement | Wajib? | Deskripsi |
|-------------|--------|-----------|
| Executable | ✅ | File harus bisa dieksekusi |
| Nama = Command | ✅ | Nama file (tanpa `.exe`) = command name |
| Exit code | ✅ | `0` sukses, non-zero error |
| `--help` flag | ⭐ | Tampilkan usage info |
| Stderr for errors | ⭐ | Stdout = output, stderr = error |

---

## Cross-Platform

```mermaid
graph TD
    DEV["dev dispatcher"]
    DEV -->|"_spawnv"| WIN["Windows<br/>MSVC 19.35+<br/>.exe plugins"]
    DEV -->|"fork+execvp"| LIN["Linux<br/>GCC 13+<br/>chmod +x plugins"]
    DEV -->|"fork+execvp"| MAC["macOS<br/>Clang 17+<br/>chmod +x plugins"]
```
