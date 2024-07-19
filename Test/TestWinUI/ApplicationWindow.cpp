#include "ApplicationWindow.h"

ApplicationWindow::ApplicationWindow(QWidget* parent /*= nullptr*/)
    : aproch::AWindow(parent)
{
    mMainWidget = new QWidget(this);
    QBoxLayout* layout = new QBoxLayout(QBoxLayout::LeftToRight, mMainWidget);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    setWinUIMaterial(aproch::Mica);
    resize(1080, 640);
    setWindowTitle("Mono Dream v1.0");
    getCaptionBar()->getTitle()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    aproch::ANavigationView* navView = new aproch::ANavigationView();
    navView->setHeaderText(AStr("Aproch Studio"));
    layout->addWidget(navView);

    qDebug() << navView->size();

    QList<aproch::ANavigationMenuItem*> items;
    aproch::ANavigationMenuItem* item1 = new aproch::ANavigationMenuItem(AStr("Menu Item1"), QIcon("E:/素材资源/图片/ico/girl2.ico"), navView);
    aproch::ANavigationMenuItem* item2 = new aproch::ANavigationMenuItem(AStr("Menu Item2"), QIcon("E:/素材资源/图片/ico/girl2.ico"), navView);
    aproch::ANavigationMenuItem* item21 = new aproch::ANavigationMenuItem(AStr("Sub Menu Item1"), QIcon("E:/素材资源/图片/ico/girl2.ico"), navView);
    aproch::ANavigationMenuItem* item211 = new aproch::ANavigationMenuItem(AStr("Sub Menu Item1"), QIcon("E:/素材资源/图片/ico/girl2.ico"), navView);
    aproch::ANavigationMenuItem* item212 = new aproch::ANavigationMenuItem(AStr("Sub Menu Item1"), QIcon("E:/素材资源/图片/ico/girl2.ico"), navView);
    aproch::ANavigationMenuItem* item22 = new aproch::ANavigationMenuItem(AStr("Sub Menu Item2"), QIcon("E:/素材资源/图片/ico/girl2.ico"), navView);
    aproch::ANavigationMenuItemGroup* group1 = new aproch::ANavigationMenuItemGroup(AStr("Group1"), navView);
    aproch::ANavigationMenuItem* item3 = new aproch::ANavigationMenuItem(AStr("Menu Item3"), QIcon("E:/素材资源/图片/ico/girl2.ico"), navView);
    aproch::ANavigationMenuItem* item4 = new aproch::ANavigationMenuItem(AStr("Menu Item4"), QIcon("E:/素材资源/图片/ico/girl2.ico"), navView);
    aproch::ANavigationMenuItem* item5 = new aproch::ANavigationMenuItem(AStr("Menu Item5"), QIcon("E:/素材资源/图片/ico/girl2.ico"), navView);
    aproch::ANavigationMenuItem* item6 = new aproch::ANavigationMenuItem(AStr("Menu Item6"), QIcon("E:/素材资源/图片/ico/girl2.ico"), navView);
    aproch::ANavigationMenuItem* item7 = new aproch::ANavigationMenuItem(AStr("Menu Item7"), QIcon("E:/素材资源/图片/ico/girl2.ico"), navView);
    aproch::ANavigationMenuItem* item8 = new aproch::ANavigationMenuItem(AStr("Menu Item8"), QIcon("E:/素材资源/图片/ico/girl2.ico"), navView);
    aproch::ANavigationMenuItem* item9 = new aproch::ANavigationMenuItem(AStr("Menu Item9"), QIcon("E:/素材资源/图片/ico/girl2.ico"), navView);

    navView->appendMenuItem(item1);
    navView->appendMenuItem(item2);
    navView->insertMenuItem(item21, 0, item2);
    navView->insertMenuItem(item211, 0, item21);
    navView->insertMenuItem(item212, 1, item21);
    navView->insertMenuItem(item22, 1, item2);
    navView->appendMenuItem(group1);
    navView->appendMenuItem(item3);
    navView->appendMenuItem(item4);
    navView->appendMenuItem(item5);
    navView->appendMenuItem(item6);
    navView->appendMenuItem(item7);
    navView->appendMenuItem(item8);
    navView->appendMenuItem(item9);

    setContentsMargins(0, 0, 0, 0);
    setCentralWidget(mMainWidget);
}

ApplicationWindow::~ApplicationWindow()
{

}
