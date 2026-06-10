#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QUrl>
#include <QUrlQuery>
#include <QtWebEngineWidgets/QWebEnginePage>
#include <QtWebEngineWidgets/QWebEngineView>

// 捕获 WebEngine 内 JS console 输出，转发到 Qt 日志系统
class SdkWebPage : public QWebEnginePage
{
    Q_OBJECT
public:
    explicit SdkWebPage(QWebEngineProfile *profile, QObject *parent = nullptr)
        : QWebEnginePage(profile, parent) {}

protected:
    void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level,
                                  const QString &message, int lineNumber,
                                  const QString &sourceID) override;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onLaunch();
    void onBack();

private:
    void setupConfigPage();
    void setupViewerPage();
    QUrl buildSdkUrl(const QString &instanceId,
                     const QString &clientId,
                     const QString &secretKey) const;

    QStackedWidget   *m_stack       = nullptr;
    QWidget          *m_configPage  = nullptr;
    QWidget          *m_viewerPage  = nullptr;
    QWebEngineView   *m_webView     = nullptr;

    QLineEdit *m_clientIdEdit   = nullptr;
    QLineEdit *m_secretKeyEdit  = nullptr;
    QLineEdit *m_instanceIdEdit = nullptr;
};
