#include "WinUIWindow.h"
#include "ApplicationWindow.h"

#include <QtWidgets/QApplication>
#include <QPushButton>
#include <QMessageBox>

#include "AprochAPI.h"

void _setStyleSheet(QWidget* widget)
{
#if _DEBUG
    QPushButton* styleBtn = new QPushButton(QObject::tr("Load Style"), widget);
    QObject::connect(styleBtn, &QPushButton::clicked, [=] {
        QString themeResFile = aproch::ATheme::getSystemTheme() == aproch::EThemeType::Light ? "light" : "dark";
        QFile style(QApplication::applicationDirPath() + AStr("/../../../../Src/Resource/theme/%1.qss").arg(themeResFile));
        if (style.open(QFile::ReadOnly | QFile::Text))
        {
            widget->setStyleSheet(style.readAll());
        }
        style.close();
    });
    styleBtn->move((widget->width() - styleBtn->width()) * 0.5, styleBtn->height());
    styleBtn->click();
    styleBtn->setStyleSheet("background:black; color: white; padding: 4px");
    styleBtn->raise();
    styleBtn->show();
#endif // _DEBUG
}

int main(int argc, char* argv[])
{
    auto appInst = aproch::AAppContext::getInstance();

    QObject::connect(appInst, &aproch::AAppContext::launch, [](bool& canContinue) {
        qDebug() << "launch";
    });

    QObject::connect(appInst, &aproch::AAppContext::ready, [](bool& canContinue) {

        WinUIWindow* window = new WinUIWindow();
        window->show();

        /*ApplicationWindow* window = new ApplicationWindow();
        window->show();*/

        aproch::AAppContext::setMainWindow(window);

        _setStyleSheet(window);
    });

    QObject::connect(appInst, &aproch::AAppContext::start, []() {
        //QMessageBox::information(aproch::AAppContext::getMainWindow(), AStr("提示"), AStr("软件已启动"));
    });

    QObject::connect(appInst, &aproch::AAppContext::exit, []() {
        //QMessageBox::information(aproch::AAppContext::getMainWindow(), AStr("提示"), AStr("已经退出"));
    });

    return appInst->run(argc, argv);
}
