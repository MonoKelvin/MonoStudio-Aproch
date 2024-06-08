#include "WinUIWindow.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>
//#include "AprochAPI.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_DontUseNativeMenuBar);
    a.setOrganizationName(QStringLiteral("Mono Studio"));

    //aproch::AWinUIStyleEngine::instance()->initStyleTheme();

    /*QPalette pt;
    pt.setColor(QPalette::Background, QColor(4, 159, 241));
    pt.setColor(QPalette::WindowText, Qt::red);
    pt.setColor(QPalette::Button, Qt::blue);
    pt.setColor(QPalette::ToolTipText, Qt::yellow);
    pt.setColor(QPalette::Foreground, Qt::green);
    pt.setColor(QPalette::Shadow, Qt::black);
    pt.setColor(QPalette::Window, Qt::darkBlue);
    pt.setColor(QPalette::Base, Qt::darkCyan);
    pt.setColor(QPalette::ButtonText, Qt::cyan);
    pt.setColor(QPalette::PlaceholderText, Qt::darkMagenta);
    QApplication::setPalette(pt);*/

    WinUIWindow w;
    w.show();
    return a.exec();
}
