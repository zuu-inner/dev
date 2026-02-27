# ==============================================================================
# Script: format.ps1 (atau run-clang-format.ps1 terserah kamu deh!)
# Dibuat oleh Zuu untuk Rara yang suka mindah-mindahin file sembarangan!
# Nggak usah sok minta formal deh, aku tahu kamu lebih suka digalakin. Hmph!
# ==============================================================================

# Pondasi 1: Mendapatkan direktori tempat skrip ini dieksekusi.
# Biar skripnya jalan dari mana aja, nggak gampang error.
$scriptDir = $PSScriptRoot
if ([string]::IsNullOrEmpty($scriptDir)) {
    $scriptDir = "."
}

# PONDASI BARU DARI ZUU: 
# Karena Rara iseng mindahin skrip ini ke dalam folder 'scripts/',
# kita harus mundur satu langkah (ke parent folder) untuk dapat root project-nya!
$projectRoot = Split-Path -Parent $scriptDir

# Pondasi 2: Daftar direktori target berdasarkan struktur folder Rara!
# Ingat: JANGAN PERNAH masukin folder 'build' atau 'third_party' ke sini!
$targetFolders = @(
    "include\dev",
    "src",
    "examples",
    "tests" # Awas aja kalau file test kamu masih berantakan!
)

# Pondasi 3: Filter ekstensi file. 
# Khusus buat file C/C++ kesayanganmu aja.
$extensions = @("*.cpp", "*.hpp", "*.c", "*.h")

Write-Host "Zuu mulai ngerapiin kodemu ya, Rara... Jangan berisik!" -ForegroundColor Cyan

# Pondasi 4: Melakukan iterasi pada setiap direktori target.
foreach ($folder in $targetFolders) {
    # Menggabungkan projectRoot dengan nama direktori target.
    # Lihat nih, sekarang pakai $projectRoot, bukan $scriptDir lagi!
    $fullPath = Join-Path $projectRoot $folder

    # Memeriksa keberadaan direktori sebelum memproses file.
    if (Test-Path $fullPath) {
        Write-Host "Memeriksa direktori: $folder" -ForegroundColor Yellow

        # Mengambil semua file yang sesuai dengan ekstensi secara rekursif.
        $files = Get-ChildItem -Path $fullPath -Include $extensions -Recurse -File

        foreach ($file in $files) {
            Write-Host "  -> Merapikan: $($file.Name)" -ForegroundColor DarkGray
            
            # Mengeksekusi clang-format secara in-place (-i) menggunakan 
            # konfigurasi dari file (.clang-format).
            clang-format -i -style=file $file.FullName
        }
    } else {
        Write-Warning "Eh? Direktori '$folder' nggak ketemu. Rara, kamu yakin foldernya ada?!"
    }
}

Write-Host "Selesai! Sekarang kodemu udah rapi. Awas kalau diberantakin lagi!" -ForegroundColor Green