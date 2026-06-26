# Samkoon Monitor SDK Playground

这是一个演示如何将 **工业组态 SDK** 集成到不同前端环境的案例库。通过这些案例，你可以快速了解如何使用企业凭证（Client ID / Secret Key）进行鉴权并渲染组态看板。

🔗 **相关链接**：[SDK 官方仓库 (Gitee)](https://gitee.com/samkoon-sdk/samkoon-monitor-sdk)

---

## 📂 案例目录说明

| 路径 | 接入方式 | 适用场景 |
| :--- | :--- | :--- |
| [`/html-example`](./html-example) | **Web Component** | 原生 HTML、JQuery、PHP、JSP 等传统页面及无框架环境。 |
| [`/react-example`](./react-example) | **React Component** | React / Next.js 项目，支持 Props 控制和生命周期钩子。 |
| [`/vue-example`](./vue-example) | **ReactDOM 挂载** | Vue 2.x/3.x 项目，通过在 DOM 节点上手动挂载 React 组件实现兼容。 |
| [`/qt5-example`](./qt5-example) | **QWebEngineView** | Qt5 C++ 桌面应用，通过嵌入 Chromium 内核渲染 SDK Web Component。 |

---

## 🚀 快速启动

在项目根目录下执行：

```bash
# 启动 HTML 演示 (http://localhost:8081)
pnpm start:demo-html

# 启动 React 演示
pnpm start:demo-react

# 启动 Vue 演示
pnpm start:demo-vue
```

### Qt5 案例

Qt5 案例为 C++ 桌面应用，需独立编译：

```bash
cd qt5-example

# 方式一：CMake（推荐）
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/SamkoonMonitorQt5   # macOS/Linux
# build\Release\SamkoonMonitorQt5.exe  # Windows

# 方式二：qmake
qmake SamkoonMonitorQt5.pro
make
./SamkoonMonitorQt5
```

**依赖**：Qt 5.15+ 并安装 `qtwebengine` 模块。macOS 可通过 `brew install qt@5` 安装。

---

## ⚙️ 关键配置说明

SDK 采用 **企业凭证模式** 进行授权：

- **安全加固**：使用 `clientId` 和 `secretKey` 进行鉴权。SDK 内部会自动处理安全传输，确保密钥不会以明文形式暴露在网络请求中。
- **权限控制**：凭据与企业开发者账号绑定，可平滑管理不同项目和设备的访问权限。

---

## 🛠️ 集成代码片段

### HTML (Web Component)

```html
<!-- 最新版（推荐） -->
<script type="module" src="https://xdata.samkoonyun.com/sdk/sk-monitor/latest/monitor-sdk.mjs"></script>

<!-- 或锁定特定版本 -->
<!-- <script type="module" src="https://xdata.samkoonyun.com/sdk/sk-monitor/v1.1.6/monitor-sdk.mjs"></script> -->

<samkoon-monitor-sdk 
    instance-id="YOUR_INSTANCE_ID"
    client-id="YOUR_CLIENT_ID"
    secret-key="YOUR_SECRET_KEY">
</samkoon-monitor-sdk>
```

### React

在 React 中，你可以像使用普通 HTML 标签一样使用 SDK：

```tsx
function App() {
  return (
    <samkoon-monitor-sdk 
      style={{ display: 'block', width: '100%', height: '100%' }}
      instance-id="YOUR_INSTANCE_ID"
      client-id="YOUR_CLIENT_ID"
      secret-key="YOUR_SECRET_KEY"
    />
  );
}
```

### Qt5 (QWebEngineView)

Qt5 通过 `QWebEngineView::setHtml()` 将 Web Component 嵌入到原生 C++ 窗口中：

```cpp
// CMakeLists.txt: find_package(Qt5 REQUIRED COMPONENTS Widgets WebEngineWidgets)

#include <QtWebEngineWidgets/QWebEngineView>

// 动态生成 HTML，将凭证直接注入到 Web Component 属性
QString html = QString(R"(
<html><head>
  <script type="module" src="https://xdata.samkoonyun.com/sdk/sk-monitor/latest/monitor-sdk.mjs"></script>
  <style>html,body{margin:0;height:100%} samkoon-monitor-sdk{display:block;width:100%;height:100%}</style>
</head><body>
  <samkoon-monitor-sdk instance-id="%1" client-id="%2" secret-key="%3"/>
</body></html>)")
    .arg(instanceId, clientId, secretKey);

// baseUrl 设为 CDN 域，确保 ES Module 的跨域脚本可正常加载
m_webView->setHtml(html, QUrl("https://xdata.samkoonyun.com/"));
```

---

## 📄 许可证 (License)

Copyright (c) 2026 Samkoon Tech Co., Ltd. 根据 [MIT 许可证](./LICENSE) 授权。
