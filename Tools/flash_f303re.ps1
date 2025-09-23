# Simple STM32 flash script using STM32CubeProgrammer CLI (must be in PATH)

$binPath = "..\Projects\F303RE_LEO_cube\Debug\F303RE_LEO_cube.bin"
$flashAddr = "0x08000000"

if (-not (Test-Path $binPath)) {
    Write-Host "Binary file not found: $binPath" -ForegroundColor Red
    exit 1
}

Write-Host "Flashing $binPath to STM32 at $flashAddr ..." -ForegroundColor Cyan

STM32_Programmer_CLI.exe -c port=SWD -w "$binPath" $flashAddr -v -rst

if ($LASTEXITCODE -eq 0) {
    Write-Host "Flash OK." -ForegroundColor Green
} else {
    Write-Host "Flashing failed." -ForegroundColor Red
}