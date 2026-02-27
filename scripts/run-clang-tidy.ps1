# ==============================================================================
# Script: run-clang-tidy.ps1
# Dibuat oleh Zuu buat Rara yang bawel tapi rajin belajar C/C++!
# Awas aja kalau masih nanya kenapa error padahal belum baca warning-nya!
# ==============================================================================

# Pondasi 1: Cari root project Rara, karena script ini ada di dalam folder scripts/
$scriptDir = $PSScriptRoot
if ([string]::IsNullOrEmpty($scriptDir)) {
    $scriptDir = "."
}
$projectRoot = Split-Path -Parent $scriptDir

# ==============================================================================
# PONDASI 2: PENGECEKAN COMPILE_COMMANDS.JSON (SANGAT PENTING!)
# ==============================================================================
$compileCommands = Join-Path $projectRoot "build\compile_commands.json"
if (-Not (Test-Path $compileCommands)) {
    Write-Host "BAKA RARA! 😡" -ForegroundColor Red
    Write-Host "File compile_commands.json nggak ada di folder build/ kamu!" -ForegroundColor Red
    Write-Host "Clang-Tidy itu butuh file ini buat ngerti arsitektur kodemu!" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Jalanin perintah ini dulu di terminal buat bikin filenya:" -ForegroundColor White
    Write-Host "cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON" -ForegroundColor Cyan
    exit # Berhenti! Jangan paksa jalanin clang-tidy kalau filenya nggak ada!
}

# Pondasi 3: Daftar folder yang mau di-lint (diperiksa)
$targetFolders = @(
    "include\dev",
    "src",
    "examples",
    "tests"
)

# Filter file C/C++
$extensions = @("*.cpp", "*.hpp", "*.c", "*.h")

Write-Host "Zuu mulai meriksa logika kodemu pakai Clang-Tidy 'ramah' ya... Jangan deg-degan!" -ForegroundColor Cyan

# Pondasi 4: Iterasi dan Eksekusi
foreach ($folder in $targetFolders) {
    $fullPath = Join-Path $projectRoot $folder

    if (Test-Path $fullPath) {
        Write-Host "Memeriksa folder: $folder" -ForegroundColor Yellow

        $files = Get-ChildItem -Path $fullPath -Include $extensions -Recurse -File

        foreach ($file in $files) {
            Write-Host "  -> Menganalisis: $($file.Name)" -ForegroundColor DarkGray
            
            # PONDASI 5: Cara manggil clang-tidy
            # -p build : Kasih tahu lokasi compile_commands.json ada di folder build/
            # Kita nggak pakai --checks="..." di sini, biar dia otomatis baca 
            # file .clang-tidy 'ramah' yang ada di root project!
            clang-tidy -p "$projectRoot/build" $file.FullName
        }
    } else {
        Write-Warning "Eh? Folder '$folder' nggak ketemu. Rara, kamu yakin foldernya ada?!"
    }
}

Write-Host "Selesai! Kalau nggak ada error merah-merah, berarti logika kodemu aman. Tumben pinter!" -ForegroundColor Green