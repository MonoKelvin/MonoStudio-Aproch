#include "WinUIWindow.h"
#include <QtWidgets/QApplication>
#include <QPushButton>
#include <QMessageBox>

#include "AprochAPI.h"

int main(int argc, char* argv[])
{
    auto appInst = aproch::AAppContext::getInstance();

    QObject::connect(appInst, &aproch::AAppContext::launch, [](bool& canContinue) {
        qDebug() << "launch";
    });

    QObject::connect(appInst, &aproch::AAppContext::ready, [](bool& canContinue) {

        WinUIWindow* window = new WinUIWindow();
        window->show();

        aproch::AAppContext::setMainWindow(window);

#ifdef _DEBUG
        // DEBUG 测试加载默认样式表
        /*QPushButton styleBtn(QObject::tr("重新加载样式表"), window);
        QObject::connect(&styleBtn, &QPushButton::clicked, [&] {
            QFile style(QApplication::applicationDirPath() + AStr("/../../../../Src/Resource/theme/dark.qss"));
            if (style.open(QFile::ReadOnly | QFile::Text))
                window.setStyleSheet(style.readAll());
            style.close();
        });
        styleBtn.resize(140, 40);
        styleBtn.move((window.width() - styleBtn.width()) * 0.5, styleBtn.height() - 100);
        styleBtn.click();*/
#endif // _DEBUG
    });

    QObject::connect(appInst, &aproch::AAppContext::start, []() {
        //QMessageBox::information(aproch::AAppContext::getMainWindow(), AStr("提示"), AStr("软件已启动"));
    });

    QObject::connect(appInst, &aproch::AAppContext::exit, []() {
        //QMessageBox::information(aproch::AAppContext::getMainWindow(), AStr("提示"), AStr("已经退出"));
    });

    return appInst->run(argc, argv);
}
