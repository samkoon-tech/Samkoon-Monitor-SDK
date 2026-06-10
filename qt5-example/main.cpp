#include <QApplication>
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    // 启用 Chrome DevTools 远程调试，访问 http://localhost:9222 即可调试 WebView
    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "9222");

    // QWebEngineView 要求在 QApplication 创建前设置 QtWebEngine 属性
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);
    app.setApplicationName("Samkoon Monitor SDK Demo");
    app.setOrganizationName("Samkoon");

    MainWindow window;
    window.show();

    return app.exec();
}
