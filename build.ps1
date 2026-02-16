# PowerShell build helper: compiles all .cpp files under src/ plus main.cpp

$includes = "-I", "src/include", "-I", "src"   # search both include dir and src root for headers
$libDir   = "-L", "src/lib"
$output   = "-o", "main.exe"
$flags    = "-lmingw32", "-lSDL2main", "-lSDL2"

# gather source files recursively (search entire workspace for .cpp files)
$sourceFiles = Get-ChildItem -Path "." -Filter "*.cpp" -Recurse | ForEach-Object { $_.FullName }

# you can inspect which files were found

Write-Host "Compiling: $($sourceFiles -join ' ')"

g++ $includes $libDir $sourceFiles $output $flags

if ($LASTEXITCODE -eq 0) {
    Write-Host "Build successful! Executable: main.exe"
} else {
    Write-Host "Build failed with exit code $LASTEXITCODE"
}
