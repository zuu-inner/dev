# Security Policy

## Versi yang Didukung

| Versi | Didukung |
|-------|----------|
| 1.0.x | ✅ |
| < 1.0 | ❌ |

## Melaporkan Kerentanan

Jika Anda menemukan kerentanan keamanan, **JANGAN** membuat issue publik.

Silakan kirim email ke: **zuudevs@gmail.com**

Sertakan informasi berikut:

1. Deskripsi kerentanan
2. Langkah reproduksi
3. Potensi dampak
4. Saran perbaikan (jika ada)

### Response Timeline

- **Acknowledgment:** dalam 48 jam
- **Initial assessment:** dalam 1 minggu
- **Fix release:** secepat mungkin setelah assessment

## Pertimbangan Keamanan

Karena `dev` mengeksekusi plugin sebagai subprocess, perhatikan:

- **Hanya jalankan plugin dari sumber terpercaya**
- **Plugin memiliki akses penuh** ke sistem sesuai permission user
- **Jangan letakkan plugin dari sumber yang tidak diverifikasi** di folder `plugins/`
- **Verifikasi integritas** plugin sebelum penggunaan pertama
