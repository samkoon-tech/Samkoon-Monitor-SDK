# Qt5 Windows 打包说明

本目录提供 Qt5 桌面应用在 Windows 平台的可分发明台包。

## 两种获取方式

### 方式一：下载预编译包（推荐集成方）

每个 release tag 会自动触发 [Aliyun Codeup 流水线](../../.aliyun-flow/build-qt5-windows.yml)，构建出可分发的 zip 包：

1. 打开 [Releases 页面](https://codeup.aliyun.com/samkoon/samkoon-monitor-sdk/-/releases)
2. 下载 `SamkoonMonitorQt5-windows-x64.zip`
3. 解压到任意目录，双击 `SamkoonMonitorQt5.exe` 即可运行
4. 整个目录拷给客户，无需安装 Qt

> ⚠️ 预编译包默认加载线上 CDN SDK：`https://xdata.samkoonyun.com/sdk/sk-monitor/latest/sdk-embed.html`
> 如需锁定版本，改 `MainWindow.cpp` 中 `SDK_EMBED_URL` 后重新构建。

### 方式二：本地手动构建

适用场景：自定制 SDK URL、打补丁验证、临时调试。

#### 前置依赖

- Windows 10/11
- **Qt 5.15.2**（勾选 `MSVC 2019 64-bit` 和 `Qt WebEngine`），[下载地址](https://download.qt.io/official_releases/qt/5.15/5.15.2/)
- **Visual Studio 2019/2022 Build Tools**（C++ 桌面开发）
- **CMake 3.16+**（[下载](https://cmake.org/download/)）
- **Ninja**（`choco install ninja` 或 [GitHub Release](https://github.com/ninja-build/ninja/releases)）
- 可选：**7-Zip**（提升压缩率）

#### 一行命令打包

```powershell
cd playground/qt5-example
pwsh scripts/build-windows.ps1 -QtDir "C:\Qt\Qt5.15.2\msvc2019_64"
```

参数：
- `-QtDir`（必填）：Qt 安装根目录
- `-OutputDir`：产物输出目录，默认 `../dist/windows`
- `-ZipName`：zip 文件名，默认 `SamkoonMonitorQt5-windows-x64.zip`

脚本会按顺序执行：
1. CMake 配置（Ninja + Release）
2. 编译
3. 拷贝可执行文件
4. `windeployqt` 收集 Qt 运行时 DLL（约 200MB）
5. 打包成 zip

完成后产物：

```
playground/dist/windows/
├── SamkoonMonitorQt5.exe
├── Qt5Core.dll
├── Qt5WebEngineCore.dll
├── ...
├── platforms/qwindows.dll
└── resources/icudtl.dat
```

## 跨平台构建

| 平台 | 编译器 | Qt 后端 | 本地构建命令 |
|---|---|---|---|
| **Windows** | MSVC 2019+ | Chromium | `pwsh scripts/build-windows.ps1 -QtDir <Qt根目录>` |
| **macOS** | Apple Clang | Chromium | `cmake -S . -B build && cmake --build build && open build/SamkoonMonitorQt5.app` |
| **Linux** | GCC 8+ | Chromium | `cmake -S . -B build && cmake --build build && ./build/SamkoonMonitorQt5` |

## 常见问题

**Q: 启动时提示 "无法找到 Qt5Core.dll"？**
A: `windeployqt` 没正常运行。检查 `-QtDir` 路径正确，确保 Qt 安装包含 WebEngine 组件。

**Q: 启动后页面空白？**
A: 99% 是网络问题。Qt WebEngine 默认走系统代理，但 SSL 握手对中间人代理敏感，建议：
- 检查目标机器能否访问 `https://xdata.samkoonyun.com` 和 `https://zutai.samkoonyun.com`
- 或改用本地 SDK 模式（`USE_LOCAL_SDK=ON` 重新编译）

**Q: 怎么集成进我自己的 Qt5 项目？**
A: 拷贝 `MainWindow.cpp/.h` 到你项目，照搬 `buildSdkUrl()` 即可。关键是：
- `QWebEngineView::settings()` 启用 `JavascriptEnabled` 和 `LocalContentCanAccessRemoteUrls`
- `QWebEngineView::page()->profile()` 设置中文偏好字体
