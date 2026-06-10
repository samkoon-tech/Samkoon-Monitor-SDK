#include "MainWindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMap>
#include <QMessageBox>
#include <QTextStream>
#include <QVBoxLayout>
#include <QtWebEngineWidgets/QWebEngineProfile>
#include <QtWebEngineWidgets/QWebEngineSettings>

// 向上最多遍历 maxLevels 层目录，寻找 .env 文件并解析 key=value
static QMap<QString, QString> loadEnvFile(const QString &startDir, int maxLevels = 4)
{
    QMap<QString, QString> env;
    QDir dir(startDir);
    for (int i = 0; i < maxLevels; ++i) {
        QFile file(dir.filePath(".env"));
        if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine().trimmed();
                if (line.isEmpty() || line.startsWith('#'))
                    continue;
                int eq = line.indexOf('=');
                if (eq < 1)
                    continue;
                env[line.left(eq).trimmed()] = line.mid(eq + 1).trimmed();
            }
            break;
        }
        if (!dir.cdUp())
            break;
    }
    return env;
}

// 本地调试：pnpm start:sdk-server:dev 启动后将 USE_LOCAL_SDK 注释取消
// 生产/CI：使用 CDN 地址
#ifdef USE_LOCAL_SDK
static const QString SDK_EMBED_URL =
    "http://localhost:8888/sdk/sk-monitor/latest/sdk-embed";
#else
static const QString SDK_EMBED_URL =
    "https://xdata.samkoonyun.com/sdk/sk-monitor/latest/sdk-embed.html";
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Samkoon Monitor SDK - Qt5 Integration Demo");
    resize(1280, 800);
    setMinimumSize(800, 600);

    m_stack = new QStackedWidget(this);
    setCentralWidget(m_stack);

    setupConfigPage();
    setupViewerPage();

    // 从 .env 预填充凭证（兼容 VITE_ 前缀和无前缀两种格式）
    // macOS .app Bundle 内部层级较深，从可执行文件路径向上搜索
    const auto env = loadEnvFile(QCoreApplication::applicationDirPath(), 6);
    auto get = [&](const QString &key) -> QString {
        return env.value("VITE_" + key, env.value(key));
    };
    m_clientIdEdit->setText(get("SDK_CLIENT_ID"));
    m_secretKeyEdit->setText(get("SDK_SECRET_KEY"));
    m_instanceIdEdit->setText(get("SDK_INSTANCE_ID"));

    m_stack->setCurrentIndex(0);
}

void MainWindow::setupConfigPage()
{
    m_configPage = new QWidget();
    m_configPage->setObjectName("configPage");
    m_configPage->setStyleSheet(R"(
        QWidget#configPage { background: #0f172a; }
        QGroupBox {
            background: rgba(255,255,255,0.95);
            border-radius: 12px;
            border: 1px solid #e2e8f0;
            font-size: 14px;
            font-weight: bold;
            padding-top: 16px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top center;
            color: #1e293b;
            font-size: 16px;
            font-weight: 700;
            background: transparent;
        }
        QLabel {
            color: #1e293b;
            font-weight: 600;
            font-size: 13px;
            background: transparent;
        }
        QLineEdit {
            padding: 8px 12px;
            border: 1px solid #e2e8f0;
            border-radius: 6px;
            font-size: 14px;
            background: white;
            color: #1e293b;
        }
        QLineEdit:focus { border-color: #2563eb; }
        QPushButton#launchBtn {
            background: #2563eb;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 12px;
            font-size: 15px;
            font-weight: 600;
        }
        QPushButton#launchBtn:hover { background: #1d4ed8; }
    )");

    auto *outerLayout = new QVBoxLayout(m_configPage);
    outerLayout->setAlignment(Qt::AlignCenter);

    auto *card = new QGroupBox("Samkoon Monitor SDK 配置");
    card->setFixedWidth(480);

    auto *form = new QFormLayout(card);
    form->setSpacing(14);
    form->setContentsMargins(28, 20, 28, 28);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_clientIdEdit = new QLineEdit();
    m_clientIdEdit->setPlaceholderText("Enter Client ID");
    m_clientIdEdit->setMinimumWidth(300);
    form->addRow("Client ID:", m_clientIdEdit);

    m_secretKeyEdit = new QLineEdit();
    m_secretKeyEdit->setEchoMode(QLineEdit::Password);
    m_secretKeyEdit->setPlaceholderText("Enter Secret Key");
    m_secretKeyEdit->setMinimumWidth(300);
    form->addRow("Secret Key:", m_secretKeyEdit);

    m_instanceIdEdit = new QLineEdit();
    m_instanceIdEdit->setPlaceholderText("Enter Instance ID (View ID)");
    m_instanceIdEdit->setMinimumWidth(300);
    form->addRow("Instance ID:", m_instanceIdEdit);

    auto *launchBtn = new QPushButton("Initialize & Launch");
    launchBtn->setObjectName("launchBtn");
    launchBtn->setFixedHeight(44);
    connect(launchBtn, &QPushButton::clicked, this, &MainWindow::onLaunch);
    form->addRow(launchBtn);

    outerLayout->addWidget(card);
    m_stack->addWidget(m_configPage);
}

void MainWindow::setupViewerPage()
{
    m_viewerPage = new QWidget();
    auto *layout = new QVBoxLayout(m_viewerPage);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // 顶部工具栏：用 objectName 限定选择器，避免样式级联到子 Widget
    auto *toolbar = new QWidget();
    toolbar->setObjectName("viewerToolbar");
    toolbar->setFixedHeight(44);
    toolbar->setStyleSheet(R"(
        QWidget#viewerToolbar {
            background: rgba(255,255,255,0.9);
            border-bottom: 1px solid #e2e8f0;
        }
    )");

    auto *toolbarLayout = new QHBoxLayout(toolbar);
    toolbarLayout->setContentsMargins(16, 0, 16, 0);

    auto *backBtn = new QPushButton("← Back");
    backBtn->setFixedWidth(80);
    backBtn->setStyleSheet(R"(
        QPushButton {
            background: #f1f5f9;
            color: #374151;
            border: 1px solid #e2e8f0;
            border-radius: 6px;
            padding: 4px 8px;
            font-weight: 500;
            font-size: 13px;
        }
        QPushButton:hover { background: #e2e8f0; }
    )");
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::onBack);

    auto *titleLabel = new QLabel("Samkoon Monitor SDK Integration (Qt5)");
    titleLabel->setStyleSheet("background: transparent; font-weight: 700; font-size: 14px; color: #2563eb;");
    titleLabel->setAlignment(Qt::AlignCenter);

    toolbarLayout->addWidget(backBtn);
    toolbarLayout->addWidget(titleLabel, 1);
    toolbarLayout->addSpacing(80); // 对称占位

    m_webView = new QWebEngineView();

    // 使用自定义 Page，捕获 JS console 输出到 Qt 日志
    auto *sdkPage = new SdkWebPage(m_webView->page()->profile(), m_webView);
    m_webView->setPage(sdkPage);

    auto *wSettings = m_webView->settings();

    // 允许加载 HTTPS 资源
    wSettings->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    wSettings->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    wSettings->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);

    // 告知 Chromium 首选语言为中文，触发正确的 Unicode 字体回退链
    m_webView->page()->profile()->setHttpAcceptLanguage("zh-CN,zh;q=0.9,en;q=0.8");

    // 显式映射 CSS 通用字体族到支持中文的字体
    wSettings->setFontFamily(QWebEngineSettings::StandardFont,  "PingFang SC");
    wSettings->setFontFamily(QWebEngineSettings::SansSerifFont, "PingFang SC");
    wSettings->setFontFamily(QWebEngineSettings::SerifFont,     "Songti SC");
    wSettings->setFontFamily(QWebEngineSettings::FixedFont,     "Menlo");

    layout->addWidget(toolbar);
    layout->addWidget(m_webView, 1);
    m_stack->addWidget(m_viewerPage);
}

QUrl MainWindow::buildSdkUrl(const QString &instanceId,
                              const QString &clientId,
                              const QString &secretKey) const
{
    QUrl url(SDK_EMBED_URL);
    QUrlQuery query;
    query.addQueryItem("instanceId", instanceId);
    query.addQueryItem("clientId",   clientId);
    query.addQueryItem("secretKey",  secretKey);
    url.setQuery(query);
    return url;
}

void MainWindow::onLaunch()
{
    const QString clientId   = m_clientIdEdit->text().trimmed();
    const QString secretKey  = m_secretKeyEdit->text().trimmed();
    const QString instanceId = m_instanceIdEdit->text().trimmed();

    if (clientId.isEmpty() || secretKey.isEmpty() || instanceId.isEmpty()) {
        QMessageBox::warning(this, "配置不完整",
                             "请填写完整的 Client ID、Secret Key 和 Instance ID。");
        return;
    }

    // load() 加载真实 HTTP 地址，确保 ES Module 动态 import 不受 data: 源限制
    m_webView->load(buildSdkUrl(instanceId, clientId, secretKey));
    m_stack->setCurrentIndex(1);
}

void MainWindow::onBack()
{
    m_webView->stop();
    m_webView->setHtml(QString());
    m_stack->setCurrentIndex(0);
}

void SdkWebPage::javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level,
                                          const QString &message, int lineNumber,
                                          const QString &sourceID)
{
    Q_UNUSED(lineNumber)
    Q_UNUSED(sourceID)

    // [SDK-EVENT] 前缀的日志单独醒目输出，其余走普通 debug
    if (message.startsWith("[SDK-EVENT]")) {
        if (level == ErrorMessageLevel)
            qWarning().noquote() << message;
        else
            qInfo().noquote() << message;
    } else {
        qDebug().noquote() << "[JS]" << message;
    }
}