#include "WinUIWindow.h"
#include <QtWidgets/QApplication>
#include <QQmlApplicationEngine>
#include <QtQuick/QQuickWindow>
#include "AprochAPI.h"

int main(int argc, char* argv[])
{
    //aproch::AWinUIApplication app(argc, argv);
    QApplication app(argc, argv);

    QGuiApplication::setApplicationName("Mono Dream");
    QGuiApplication::setApplicationDisplayName(QStringLiteral("梦游记"));
    QGuiApplication::setApplicationVersion("1.0.0.0");

    WinUIWindow window;
    window.show();

    //const QUrl url(QStringLiteral("qrc:/qml/App.qml"));
    //return app.exec(url);
    return app.exec();
}
