$ErrorActionPreference = "Stop"

$projectRoot = Split-Path -Parent $PSScriptRoot
Set-Location $projectRoot

$sources = @(
    "src/main.cpp",
    "src/Game.cpp",
    "src/Tower.cpp",
    "src/Enemy.cpp",
    "src/Entity.cpp",
    "src/Projectile.cpp",
    "src/RenderWindow.cpp",
    "src/Level.cpp"
)

$compileArgs = @(
    $sources +
    @(
        "-Isrc/include",
        "-Lsrc/lib",
        "-lmingw32",
        "-lSDL2main",
        "-lSDL2",
        "-lSDL2_image",
        "-o",
        "TD.exe"
    )
)

Write-Host "Building TD.exe ..."
& g++ @compileArgs

if ($LASTEXITCODE -ne 0) {
    throw "Build failed with exit code $LASTEXITCODE"
}

Write-Host "Build OK. Running TD.exe ..."
& ".\TD.exe"
