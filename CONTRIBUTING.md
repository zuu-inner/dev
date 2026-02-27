# Panduan Kontribusi

Terima kasih atas minat Anda untuk berkontribusi pada `dev`!

---

## Cara Berkontribusi

### 1. Laporkan Bug

- Gunakan [GitHub Issues](https://github.com/zuudevs/dev/issues)
- Sertakan: OS, compiler, versi `dev`, langkah reproduksi, dan output error

### 2. Usulkan Fitur

- Buka issue baru dengan label `feature-request`
- Jelaskan use case, expected behavior, dan alternatif yang sudah dipertimbangkan

### 3. Submit Pull Request

```bash
# 1. Fork & clone
git clone https://github.com/<username>/dev.git
cd dev

# 2. Buat branch
git checkout -b feat/nama-fitur

# 3. Buat perubahan & commit
git add .
git commit -m "feat: deskripsi perubahan"

# 4. Push & buat PR
git push origin feat/nama-fitur
```

---

## Konvensi

### Commit Message

Ikuti format [Conventional Commits](https://www.conventionalcommits.org/):

| Prefix | Deskripsi |
|--------|-----------|
| `feat:` | Fitur baru |
| `fix:` | Perbaikan bug |
| `docs:` | Perubahan dokumentasi |
| `refactor:` | Refaktoring (tanpa perubahan behavior) |
| `test:` | Penambahan/perbaikan test |
| `chore:` | Tugas maintenance |

### Code Style

- Gunakan **C++23** standard
- Ikuti `.clang-format` yang ada di root project
- Ikuti `.clang-tidy` untuk static analysis
- Naming: `snake_case` untuk fungsi/variabel, `PascalCase` untuk class/struct
- Tab untuk indentation

### Plugin Contributions

Jika Anda ingin menambahkan plugin bawaan:

1. Buat plugin sebagai executable terpisah
2. Tempatkan source code di `plugins/` atau buat CMake target baru
3. Tambahkan dokumentasi di `docs/`
4. Tambahkan entry di `CHANGELOG.md`

---

## Development Setup

```bash
# Clone
git clone https://github.com/zuudevs/dev.git
cd dev

# Build (Debug)
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Jalankan
./build/bin/dev
```

---

## Code of Conduct

Proyek ini mengikuti [Code of Conduct](CODE_OF_CONDUCT.md). Dengan berpartisipasi, Anda setuju untuk mematuhi aturan tersebut.
