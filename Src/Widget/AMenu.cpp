#include "stdafx.h"
#include "AMenu.h"
#include "Style/AWinUIStyle.h"

#include <QApplication>
#include <QGraphicsDropShadowEffect>

APROCH_NAMESPACE_BEGIN

class AMenuPrivate
{
public:

};

AMenu::AMenu(QWidget* parent)
    : AMenu(QString(), parent)
{
}

AMenu::AMenu(const QString& title, QWidget* parent)
    : QMenu(parent)
    , d_ptr(new AMenuPrivate())
{
    //setWindowFlag(Qt::Popup);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    //setAttribute(Qt::WA_StyledBackground);
    setContentsMargins(12, 12, 12, 20);

    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(12);
    effect->setYOffset(8);
    effect->setColor(QColor(45, 45, 45));
    setGraphicsEffect(effect);

    setStyle(new AWinUIStyle());
}

AMenu::~AMenu()
{
}

AMenu* AMenu::addMenu(const QString& title)
{
    AMenu* menu = new AMenu(title, this);
    addAction(menu->menuAction());
    return menu;
}

AMenu* AMenu::addMenu(const QIcon& icon, const QString& title)
{
    AMenu* menu = new AMenu(title, this);
    menu->setIcon(icon);
    addAction(menu->menuAction());
    return menu;
}

void AMenu::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QStyleOptionMenuItem menuOpt;
    menuOpt.initFrom(this);
    menuOpt.state = QStyle::State_None;
    menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
    menuOpt.maxIconWidth = 0;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    menuOpt.reservedShortcutWidth = 0;
#endif
    QApplication::style()->drawPrimitive(QStyle::PE_PanelMenu, &menuOpt, &painter, this);

}

APROCH_NAMESPACE_END
