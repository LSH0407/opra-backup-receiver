param(
    [string]$Exchange = 'OPRA'
)
$env:EXCHANGE = $Exchange
$exe = Join-Path $PSScriptRoot 'build' 'opra_backup_receiver.exe'
if (-not (Test-Path $exe)) {
    Write-Host '[build] building via cmake...'
    $buildDir = Join-Path $PSScriptRoot 'build'
    if (-not (Test-Path $buildDir)) { New-Item -ItemType Directory $buildDir | Out-Null }
    Push-Location $buildDir
    cmake -G "Visual Studio 17 2022" -A x64 .. | Out-Null
    cmake --build . --config Release | Out-Null
    $exe = Join-Path $buildDir 'Release' 'opra_backup_receiver.exe'
    if (-not (Test-Path $exe)) { $exe = Join-Path $buildDir 'opra_backup_receiver.exe' }
    Pop-Location
}
Write-Host "[run] EXCHANGE=$env:EXCHANGE"
& $exe
