#include "stdafx.h"
#include "ANavigationView.h"
#include "ANavigationViewItem.h"
#include "ANavigationPageView.h"
#include "Widget/Private/ANavigationPanel.h"

#include "Widget/Private/ANavigationView_p.h"
#include "Widget/Private/ANavigationPanel_p.h"

APROCH_NAMESPACE_BEGIN

ANavigationView::ANavigationView(QWidget* parent)
    : ANavigationView(EPanelPosition::Left, parent)
{
}

ANavigationView::ANavigationView(EPanelPosition position, QWidget* parent)
    : QFrame(parent)
    , d_ptr(new ANavigationViewPrivate(this))
{
    // view layout
    QBoxLayout::Direction mainDir = position == EPanelPosition::Top ?
        QBoxLayout::TopToBottom : QBoxLayout::LeftToRight;
    QBoxLayout* mainLayout = new QBoxLayout(mainDir);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);

    // panel
    d_ptr->panel = new ANavigationPanel(position, this);
    mainLayout->addWidget(d_ptr->panel);

    // page view
    //d_ptr->pageView = new ANavigationPageView(this);
    //mainLayout->addWidget(d_ptr->pageView);

    setPanelPosition(position);
}

ANavigationView::EPanelPosition ANavigationView::getPanelPosition() const
{
    return d_ptr->panelPosition;
}

void ANavigationView::setPanelPosition(EPanelPosition position)
{
    if (position == d_ptr->panelPosition)
        return;

    // view layout
    QBoxLayout* mainLayout = qobject_cast<QBoxLayout*>(layout());
    Q_ASSERT(mainLayout);

    QBoxLayout::Direction mainDir = (position == EPanelPosition::Top ?
                                     QBoxLayout::TopToBottom : QBoxLayout::LeftToRight);
    mainLayout->setDirection(mainDir);

    // navigation panel


    d_ptr->panelPosition = position;
    emit panelPositionChanged();
}

ANavigationView::ECompactMode ANavigationView::getCompactMode() const
{
    return d_ptr->compactMode;
}

void ANavigationView::setCompactMode(ECompactMode compactMode)
{
    if (compactMode == d_ptr->compactMode)
        return;

    d_ptr->compactMode = compactMode;
    emit compactModeChanged();
}

bool ANavigationView::isPanelResizable() const
{
    return false;
}

void ANavigationView::setPanelResizable(bool enabled)
{
}

bool ANavigationView::isBackEnabled() const
{
    return false;
}

void ANavigationView::setBackEnabled(bool enabled)
{
}

bool ANavigationView::isBackVisible() const
{
    return false;
}

void ANavigationView::setBackVisible(bool visible)
{
}

QString ANavigationView::getHeaderText() const
{
    return QString();
}

void ANavigationView::setHeaderText(const QString& text)
{
}

QWidget* ANavigationView::getHeader() const
{
    return nullptr;
}

void ANavigationView::setHeader(QWidget* header)
{
}

bool ANavigationView::isHeaderVisible() const
{
    return false;
}

void ANavigationView::setHeaderVisible(bool visible)
{
}

void ANavigationView::setMenuItems(QList<ANavigationViewItem*> menuItems)
{
}

QList<ANavigationViewItem*> ANavigationView::getMenuItems() const
{
    return QList<ANavigationViewItem*>();
}

ANavigationViewItem* ANavigationView::insertMenuItem(const QString& text, const QIcon& icon, int index, ANavigationViewItem* parentItem)
{
    return nullptr;
}

bool ANavigationView::insertMenuItem(ANavigationViewItem* newItem, int index, ANavigationViewItem* parentItem)
{
    return false;
}

bool ANavigationView::appendMenuItem(ANavigationViewItem* newItem, ANavigationViewItem* parentItem)
{
    return false;
}

bool ANavigationView::removeMenuItem(ANavigationViewItem* item)
{
    return false;
}

bool ANavigationView::removeMenuItem(int index, ANavigationViewItem* parentItem)
{
    return false;
}

ANavigationViewItem* ANavigationView::getMenuItem(int index, ANavigationViewItem* parent) const
{
    return nullptr;
}

ANavigationViewItemGroup* ANavigationView::insertItemGroup(const QString& text, ANavigationViewItem* rootItem)
{
    return nullptr;
}

bool ANavigationView::insertItemGroup(ANavigationViewItemGroup* group, ANavigationViewItem* rootItem)
{
    return false;
}

bool ANavigationView::appendItemGroup(ANavigationViewItemGroup* group)
{
    return false;
}

bool ANavigationView::removeItemGroup(ANavigationViewItemGroup* group)
{
    return false;
}

bool ANavigationView::removeItemGroup(int index)
{
    return false;
}

ANavigationViewItemGroup* ANavigationView::getItemGroup(int index) const
{
    return nullptr;
}

QTreeView* ANavigationView::getMenuItemView() const
{
    return nullptr;
}

QWidget* ANavigationView::getFooter() const
{
    return nullptr;
}

void ANavigationView::setFooter(QWidget* footer)
{
}

bool ANavigationView::isFooterVisible() const
{
    return false;
}

void ANavigationView::setFooterVisible(bool visible)
{
}

bool ANavigationView::isSettingsVisible() const
{
    return false;
}

void ANavigationView::setSettingsVisible(bool visible)
{
}

ANavigationPageView* ANavigationView::getContentView() const
{
    return nullptr;
}

QSize ANavigationView::sizeHint() const
{
    switch (getPanelPosition())
    {
    case aproch::ANavigationView::Top:
    //case aproch::ANavigationView::Bottom:
        return QSize(1000, 60);
    default:
        break;
    }
    return QSize(400, 680);
}

APROCH_NAMESPACE_END
