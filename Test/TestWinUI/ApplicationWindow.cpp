#include "ApplicationWindow.h"

ApplicationWindow::ApplicationWindow(QWidget* parent /*= nullptr*/)
    : aproch::AWindow(parent)
{
    mMainWidget = new QWidget(this);
    QBoxLayout* layout = new QBoxLayout(QBoxLayout::LeftToRight, mMainWidget);

    setWinUIMaterial(aproch::Mica);
    setMinimumSize(1080, 640);
    setWindowTitle("Mono Dream v1.0");
    getCaptionBar()->getTitle()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    aproch::ANavigationView* navView = new aproch::ANavigationView();
    navView->setHeaderText(AStr("Aproch Studio"));
    layout->addWidget(navView);

    setCentralWidget(mMainWidget);
}

ApplicationWindow::~ApplicationWindow()
{

}
