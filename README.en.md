# Samkoon Monitor SDK Playground

A collection of integration examples showing how to embed the **Industrial SCADA SDK** into different frontend environments. Use these samples to quickly get started with credential-based authentication and rendering SCADA dashboards.

🔗 **Links**:
[GitHub](https://github.com/samkoon-tech/Samkoon-Monitor-SDK) · [Gitee](https://gitee.com/samkoon-sdk/samkoon-monitor-sdk) · [中文文档](./README.md)

---

## 📂 Examples

| Path | Integration | Best For |
| :--- | :--- | :--- |
| [`/html-example`](./html-example) | **Web Component** | Plain HTML, jQuery, PHP, JSP and other non-framework environments. |
| [`/react-example`](./react-example) | **React Component** | React / Next.js projects with props control and lifecycle hooks. |
| [`/vue-example`](./vue-example) | **ReactDOM Mount** | Vue 2.x/3.x projects — mounts the React component onto a DOM node. |
| [`/qt5-example`](./qt5-example) | **QWebEngineView** | Qt5 C++ desktop apps — renders the SDK Web Component via Chromium. |

---

## 🚀 Quick Start

Run the following from the project root:

```bash
# Start HTML demo (http://localhost:8081)
pnpm start:demo-html

# Start React demo
pnpm start:demo-react

# Start Vue demo
pnpm start:demo-vue
```

### Qt5 Example

The Qt5 example is a native C++ desktop app and must be compiled separately:

```bash
cd qt5-example

# Option 1: CMake (recommended)
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/SamkoonMonitorQt5       # macOS / Linux
# build\Release\SamkoonMonitorQt5.exe  # Windows

# Option 2: qmake
qmake SamkoonMonitorQt5.pro
make
./SamkoonMonitorQt5
```

**Requirements**: Qt 5.15+ with the `qtwebengine` module. On macOS: `brew install qt@5`.

> ⚠️ **Windows note**: Extract the package to a local drive (e.g. `C:\`) before running.
> Launching from a network share (such as a VM shared folder like `\\Mac\Home\...`) causes
> `QtWebEngineProcess.exe` to fail loading DLLs from the UNC path, reporting "Qt5Core.dll not found".

Windows builds are automatically produced by the [GitHub Actions CI](./.github/workflows/build-qt5-windows.yml) on every push to `main`.

---

## ⚙️ Configuration

The SDK uses **enterprise credential authentication**:

- **Security**: Authenticate with `clientId` and `secretKey`. The SDK handles secure transport internally — keys are never exposed in plain-text network requests.
- **Access control**: Credentials are tied to your enterprise developer account and support fine-grained access management across projects and devices.

---

## 🛠️ Integration Snippets

### HTML (Web Component)

```html
<!-- Latest version (recommended) -->
<script type="module" src="https://xdata.samkoonyun.com/sdk/sk-monitor/latest/monitor-sdk.mjs"></script>

<!-- Pin to a specific version -->
<!-- <script type="module" src="https://xdata.samkoonyun.com/sdk/sk-monitor/v1.1.6/monitor-sdk.mjs"></script> -->

<samkoon-monitor-sdk
    instance-id="YOUR_INSTANCE_ID"
    client-id="YOUR_CLIENT_ID"
    secret-key="YOUR_SECRET_KEY">
</samkoon-monitor-sdk>
```

### React

Use the SDK like a regular HTML element:

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

Qt5 embeds the Web Component inside a native C++ window via `QWebEngineView::setHtml()`:

```cpp
// CMakeLists.txt: find_package(Qt5 REQUIRED COMPONENTS Widgets WebEngineWidgets)

#include <QtWebEngineWidgets/QWebEngineView>

// Build the HTML page and inject credentials as Web Component attributes
QString html = QString(R"(
<html><head>
  <script type="module" src="https://xdata.samkoonyun.com/sdk/sk-monitor/latest/monitor-sdk.mjs"></script>
  <style>html,body{margin:0;height:100%} samkoon-monitor-sdk{display:block;width:100%;height:100%}</style>
</head><body>
  <samkoon-monitor-sdk instance-id="%1" client-id="%2" secret-key="%3"/>
</body></html>)")
    .arg(instanceId, clientId, secretKey);

// Set baseUrl to the CDN origin so ES Module cross-origin scripts load correctly
m_webView->setHtml(html, QUrl("https://xdata.samkoonyun.com/"));
```

---

## 📄 License

Copyright (c) 2026 Samkoon Tech Co., Ltd. Licensed under the [MIT License](./LICENSE).
