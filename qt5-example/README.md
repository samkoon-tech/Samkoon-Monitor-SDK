# Samkoon Monitor SDK — Qt5 集成示例

本示例演示如何在 Qt5 C++ 桌面应用中通过 `QWebEngineView` 嵌入 Samkoon Monitor SDK。

---

## 目录说明

```
qt5-example/
  MainWindow.cpp    Qt 窗口实现，含 WebView 初始化与 SDK 事件桥接
  MainWindow.h
  main.cpp
  CMakeLists.txt
  SamkoonMonitorQt5.pro
```

---

## sdk-embed.html 说明

`sdk-embed.html` 是 Qt WebView 专用的宿主页面，随 SDK 一起发布到 CDN，负责：

- 加载 SDK Web Component（`monitor-sdk.mjs`）
- 将 URL query 参数（`instanceId / clientId / secretKey`）注入组件属性
- 把 SDK 结构化事件（`sdk:ready` / `sdk:error`）转发到 Qt 的 `javaScriptConsoleMessage`

其他集成方式（HTML / React / Vue）各自管理自己的入口 HTML，不需要此文件。

---

## 部署方式选择

`sdk-embed.html` 有两种加载方式，**按需选择**，修改 `MainWindow.cpp` 中的 `SDK_EMBED_URL` 即可。

### 方式一：远程 CDN（默认）

直接加载 CDN 上的 `sdk-embed.html`，无需在本地维护任何 SDK 文件。

```cpp
// 最新版
static const QString SDK_EMBED_URL =
    "https://xdata.samkoonyun.com/sdk/sk-monitor/latest/sdk-embed.html";

// 锁定特定版本（避免 latest 带来的不可预期更新）
static const QString SDK_EMBED_URL =
    "https://xdata.samkoonyun.com/sdk/sk-monitor/v1.1.6/sdk-embed.html";
```

Qt5 WebEngine 会缓存入口 HTML 和 ES Module。示例工程默认在 `buildSdkUrl()`
为 `sdk-embed.html` 追加 `_sdkCacheKey`，并在 `QWebEngineProfile` 上关闭 HTTP cache，
确保重新启动后能加载到线上最新 SDK。集成到业务 Qt5 项目时请保留这两处逻辑。

### 方式二：本地 HTTP Server

将 SDK 文件随应用一起分发，应用启动时在本机起一个 HTTP server 提供服务。

**适用场景**：CDN 访问受限，或对版本稳定性要求高。注意：SDK 仍需通过网络连接网关服务器获取业务数据。

SDK 文件可从 CDN 下载对应版本后放到本地目录：

```
<安装目录>/
  sdk/
    sdk-embed.html
    monitor-sdk.mjs
    vendor/
    workers/
    locales/
```

`sdk-embed.html` 中的脚本路径改为相对路径：

```html
<script type="module" src="./monitor-sdk.mjs"></script>
```

Qt 应用内嵌轻量 HTTP server（可使用 `QTcpServer` 或第三方库 `qhttpengine`）后加载：

```cpp
static const QString SDK_EMBED_URL = "http://localhost:PORT/sdk-embed.html";
```

> **为什么不用 `file://` 协议？**
> `QWebEngineView` 加载 `file://` 页面时，ES Module 的动态 `import()` 受同源策略限制，
> `vendor/` 目录下的代码分块会被拦截。使用本地 HTTP server 可绕过此限制。

---

## 构建与运行

**依赖**：Qt 5.15+ 并安装 `qtwebengine` 模块。

```bash
# macOS
brew install qt@5

# 方式一：CMake（推荐）
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/SamkoonMonitorQt5

# 方式二：qmake
qmake SamkoonMonitorQt5.pro
make
./SamkoonMonitorQt5
```

启动后在配置页填入凭证即可：

| 字段 | 说明 |
|------|------|
| Client ID | 企业开发者账号的 Client ID |
| Secret Key | 对应的 Secret Key |
| Instance ID | 要展示的组态看板实例 ID |

---

## SDK 事件监听

Qt 端通过重写 `javaScriptConsoleMessage` 接收 SDK 事件：

| 事件 | 格式 | 含义 |
|------|------|------|
| `sdk:ready` | `[SDK-EVENT] [INFO] sdk:ready {...}` | SDK 初始化完成 |
| `sdk:error` | `[SDK-EVENT] [ERROR] sdk:error {...}` | SDK 发生错误 |
| 未处理异常 | `[SDK-EVENT] [ERROR] sdk:unhandledRejection {...}` | Promise 异常 |
| 全局错误 | `[SDK-EVENT] [ERROR] sdk:uncaughtError {...}` | 脚本运行时错误 |
