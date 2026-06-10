#include <QApplication>
#include <QDir>
#include <QFile>
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

#ifdef Q_OS_WIN
    // CI 构建的 Qt 会把构建机路径写死进 QLibraryInfo，部署后子进程可能从错误位置启动；
    // 显式指定 QtWebEngineProcess.exe 为 exe 同目录，绕过路径推导
    const QString webProc = QCoreApplication::applicationDirPath()
                            + QStringLiteral("/QtWebEngineProcess.exe");
    if (QFile::exists(webProc))
        qputenv("QTWEBENGINEPROCESS_PATH",
                QDir::toNativeSeparators(webProc).toLocal8Bit());
#endif

    MainWindow window;
    window.show();

    return app.exec();
}
