# Samkoon Monitor SDK - Qt5 Windows 打包脚本
#
# 用法（本地）：
#   pwsh scripts/build-windows.ps1 -QtDir "C:\Qt\Qt5.15.2\msvc2019_64"
#
# 用法（CI，Codeup 流水线）：
#   & scripts/build-windows.ps1 `
#     -QtDir "C:\hostedtoolcache\windows\Qt\Qt5.15.2\msvc2019_64" `
#     -OutputDir "../dist/windows" `
#     -ZipName "SamkoonMonitorQt5-windows-x64.zip"
#
# 依赖：
#   - Qt 5.15.2 (MSVC2019 64-bit) + Qt WebEngine 组件
#   - Visual Studio 2019/2022 Build Tools
#   - CMake 3.16+
#   - Ninja
#   - 7-Zip (可选，用于打包)

param(
    [Parameter(Mandatory = $true)]
    [string]$QtDir,

    [Parameter(Mandatory = $false)]
    [string]$OutputDir = "../dist/windows",

    [Parameter(Mandatory = $false)]
    [string]$ZipName = "SamkoonMonitorQt5-windows-x64.zip"
)

$ErrorActionPreference = "Stop"
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$ProjectDir = Split-Path -Parent $ScriptDir
$BuildDir = Join-Path $ProjectDir "build-windows-release"
$InstallDir = Join-Path $ProjectDir $OutputDir
$ZipPath = Join-Path $ProjectDir $ZipName

Write-Host "=== Samkoon Monitor SDK Qt5 Windows Build ===" -ForegroundColor Cyan
Write-Host "QtDir:    $QtDir"
Write-Host "Project:  $ProjectDir"
Write-Host "Build:    $BuildDir"
Write-Host "Install:  $InstallDir"
Write-Host "Zip:      $ZipPath"
Write-Host ""

# 1. 配置环境变量
Write-Host "[1/6] 配置环境变量..." -ForegroundColor Yellow
$env:CMAKE_PREFIX_PATH = $QtDir
$env:PATH = "$QtDir\bin;$env:PATH"

# 2. CMake 配置
Write-Host "[2/6] CMake 配置..." -ForegroundColor Yellow
if (Test-Path $BuildDir) {
    Remove-Item -Recurse -Force $BuildDir
}
cmake -S $ProjectDir -B $BuildDir -G "Ninja" `
    -DCMAKE_BUILD_TYPE=Release `
    -DCMAKE_PREFIX_PATH=$QtDir
if ($LASTEXITCODE -ne 0) {
    throw "CMake configure failed"
}

# 3. 编译
Write-Host "[3/6] 编译 Release 版本..." -ForegroundColor Yellow
cmake --build $BuildDir --config Release
if ($LASTEXITCODE -ne 0) {
    throw "Build failed"
}

# 4. 清理旧安装目录
Write-Host "[4/6] 准备安装目录..." -ForegroundColor Yellow
if (Test-Path $InstallDir) {
    Remove-Item -Recurse -Force $InstallDir
}
New-Item -ItemType Directory -Force -Path $InstallDir | Out-Null

# 拷贝可执行文件和构建产物
Copy-Item -Path "$BuildDir\SamkoonMonitorQt5.exe" -Destination $InstallDir -Force

# 5. windeployqt 收集 Qt 运行时
Write-Host "[5/6] 运行 windeployqt 收集运行时..." -ForegroundColor Yellow
$windeployqt = Join-Path $QtDir "bin\windeployqt.exe"
if (-not (Test-Path $windeployqt)) {
    throw "windeployqt not found: $windeployqt"
}
& $windeployqt `
    --release `
    --qmldir $QtDir `
    --no-translations `
    --no-system-d3d-compiler `
    --no-opengl-sw `
    (Join-Path $InstallDir "SamkoonMonitorQt5.exe")
if ($LASTEXITCODE -ne 0) {
    throw "windeployqt failed"
}

# 6. 打包成 zip
Write-Host "[6/6] 打包成 zip..." -ForegroundColor Yellow
if (Test-Path $ZipPath) {
    Remove-Item $ZipPath -Force
}

# 优先用 7z（压缩率高），找不到则用 PowerShell Compress-Archive
$7z = Get-Command 7z -ErrorAction SilentlyContinue
if ($7z) {
    $ZipBaseName = [System.IO.Path]::GetFileNameWithoutExtension($ZipPath)
    $AppDir = Join-Path (Split-Path $ZipPath) $ZipBaseName
    Rename-Item -Path $InstallDir -NewName $AppDir -Force
    & 7z a -tzip -mx=9 $ZipPath "$AppDir\*" | Out-Null
    # 7z 默认会把目录结构带进去，重命名为产物名即可
} else {
    Compress-Archive -Path "$InstallDir\*" -DestinationPath $ZipPath -Force
}

# 统计产物大小
$size = [math]::Round((Get-Item $ZipPath).Length / 1MB, 2)
Write-Host ""
Write-Host "=== Build Success ===" -ForegroundColor Green
Write-Host "产物: $ZipPath ($size MB)"
Write-Host "解压后: $InstallDir"
