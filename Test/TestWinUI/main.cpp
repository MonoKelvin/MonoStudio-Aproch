#include "WinUIWindow.h"
#include <QtWidgets/QApplication>
#include <QQmlApplicationEngine>
#include <QtQuick/QQuickWindow>
//#include "AprochAPI.h"

int main(int argc, char* argv[])
{
    const char* uri = "example";
    int major = 1;
    int minor = 0;
#ifdef WIN32
    //::SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
    qputenv("QT_QPA_PLATFORM", "windows:darkmode=2");
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    qputenv("QT_QUICK_CONTROLS_STYLE", "Basic");
#else
    qputenv("QT_QUICK_CONTROLS_STYLE", "Default");
#endif
#ifdef Q_OS_LINUX
    //fix bug UOSv20 does not print logs
    qputenv("QT_LOGGING_RULES", "");
    //fix bug UOSv20 v-sync does not work
    qputenv("QSG_RENDER_LOOP", "basic");
#endif
    QGuiApplication::setOrganizationName("Mono Studio");
    QGuiApplication::setOrganizationDomain("https://github.com/monokelvin");
    QGuiApplication::setApplicationName("Mono Dream");
    QGuiApplication::setApplicationDisplayName(QStringLiteral("梦游记"));
    QGuiApplication::setApplicationVersion("1.0.0.0");
    //QGuiApplication::setQuitOnLastWindowClosed(false);
    /*SettingsHelper::getInstance()->init(argv);
    Log::setup(argv, uri);*/
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
#endif
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/qml/App.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject* obj, const QUrl& objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    const int exec = QGuiApplication::exec();
    if (exec == 931)
    {
        // 重启
        // QProcess::startDetached(qApp->applicationFilePath(), qApp->arguments());
    }
    return exec;
}
