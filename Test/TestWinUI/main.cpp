#include "WinUIWindow.h"
#include <QtWidgets/QApplication>
#include <QPushButton>
#include "AprochAPI.h"

int main(int argc, char* argv[])
{
    QGuiApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    //aproch::AWinUIApplication app(argc, argv);
    QApplication app(argc, argv);

    WinUIWindow window;
    window.show();

    // DEBUG 测试加载默认样式表
    QPushButton styleBtn(QObject::tr("重新加载样式表"), &window);
    QObject::connect(&styleBtn, &QPushButton::clicked, [&] {
        QFile style(QApplication::applicationDirPath() + AStr("/../../../../Src/Resource/theme/dark.qss"));
        if (style.open(QFile::ReadOnly | QFile::Text))
            window.setStyleSheet(style.readAll());
        style.close();
    });
    styleBtn.resize(140, 40);
    styleBtn.move((window.width() - styleBtn.width()) * 0.5, styleBtn.height() - 100);
    styleBtn.click();

    //const QUrl url(QStringLiteral("qrc:/qml/App.qml"));
    //return app.exec(url);
    return app.exec();
}
