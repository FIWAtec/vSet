param(
    [string]$VcpkgRoot = "C:\vcpkg"
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path $VcpkgRoot)) {
    throw "vcpkg nicht gefunden: $VcpkgRoot"
}

cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE="$VcpkgRoot\scripts\buildsystems\vcpkg.cmake"
if ($LASTEXITCODE -ne 0) { throw "CMake-Konfiguration fehlgeschlagen." }

cmake --build build
if ($LASTEXITCODE -ne 0) { throw "Build fehlgeschlagen." }

Write-Host "Build erfolgreich: .\build\VirtualDjSetPro.exe"
