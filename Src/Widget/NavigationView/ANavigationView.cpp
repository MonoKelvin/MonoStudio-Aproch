#include "stdafx.h"
#include "ANavigationView.h"
#include "ANavigationMenuItem.h"
#include "ANavigationPageView.h"
#include "Widget/Private/ANavigationPanel.h"
#include "Widget/ASplitterHandle.h"

#include "Widget/Private/ANavigationView_p.h"
#include "Widget/Private/ANavigationPanel_p.h"

APROCH_NAMESPACE_BEGIN

ANavigationView::ANavigationView(QWidget* parent)
    : ANavigationView(EPanelPosition::Left, parent)
{
}

ANavigationView::ANavigationView(EPanelPosition position, QWidget* parent)
    : QSplitter(parent)
    , d_ptr(new ANavigationViewPrivate(this))
{
    // view layout
    Qt::Orientation mainDir = position == EPanelPosition::Top ? Qt::Vertical : Qt::Horizontal;
    setOrientation(mainDir);
    setChildrenCollapsible(false);

    // panel
    d_ptr->panel = new ANavigationPanel(position, this);
    d_ptr->panel->resize(d_ptr->panel->sizeHint());

    // panel controls
    d_ptr->backButton = new ANavigationMenuItem(AStr("返回"), QIcon(), d_ptr->panel);      // icon
    d_ptr->compactButton = new ANavigationMenuItem(d_ptr->headerText, QIcon(), d_ptr->panel);      // icon
    d_ptr->settingsButton = new ANavigationMenuItem(AStr("设置"), QIcon(), d_ptr->panel);  // icon
    d_ptr->menuItemView = new ANavigationMenuItemTreeView(d_ptr->panel);
    d_ptr->menuItemViewDelegate = new ANavigationMenuItemDelegate(d_ptr->menuItemView);
    d_ptr->menuItemView->setItemDelegate(d_ptr->menuItemViewDelegate);

    d_ptr->panel->layout()->addWidget(d_ptr->backButton);
    d_ptr->panel->layout()->addWidget(d_ptr->compactButton);
    d_ptr->panel->layout()->addWidget(d_ptr->menuItemView);
    d_ptr->panel->layout()->addItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));
    d_ptr->panel->layout()->addWidget(d_ptr->settingsButton);

    // page view
    d_ptr->pageView = new ANavigationPageView(this);

    addWidget(d_ptr->panel);
    addWidget(d_ptr->pageView);

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

    if (position == EPanelPosition::Top)
    {
        // panel
        d_ptr->panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        d_ptr->pageView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // self
        setOrientation(Qt::Vertical);
        setHandleWidth(0);
    }
    else
    {
        const bool& bRsz = !d_ptr->isResizable;
        d_ptr->panel->setSizePolicy(bRsz ? QSizePolicy::Maximum : QSizePolicy::Fixed,
                                    bRsz ? QSizePolicy::Expanding : QSizePolicy::Fixed);
        d_ptr->pageView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        setOrientation(Qt::Horizontal);
        setHandleWidth(bRsz ? 12 : 0);
    }

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
    d_ptr->compactButton->setText(text);
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

ANavigationMenuItem* ANavigationView::insertMenuItem(const QString& text, const QIcon& icon, int index, ANavigationMenuItem* parentItem)
{
    ANavigationMenuItem* newItem = new ANavigationMenuItem(text, icon, d_ptr->panel);
    if (insertMenuItem(newItem, index, parentItem))
        return newItem;

    delete newItem;
    return nullptr;
}

ANavigationMenuItemGroup* ANavigationView::insertMenuItemGroup(const QString& text, int index)
{
    ANavigationMenuItemGroup* newItem = new ANavigationMenuItemGroup(text, d_ptr->panel);
    if (insertMenuItem(newItem, index, nullptr))
        return newItem;

    delete newItem;
    return nullptr;
}

bool ANavigationView::insertMenuItem(ANavigationMenuItem* newItem, int index, ANavigationMenuItem* parentItem)
{
    if (parentItem)
    {
        TNavMenuItemPtr parentMenuItem = d_ptr->findItem(parentItem);
        if (!parentMenuItem)
            return false;

        Q_ASSERT(!(index < 0 || index > parentMenuItem->subItems.size()));
        if (index < 0 || index > parentMenuItem->subItems.size())
            return false;

        TNavMenuItemPtr newMenuItem(new ANavigationViewPrivate::MenuItem(newItem, parentMenuItem));
        parentMenuItem->subItems.push_back(newMenuItem);
    }
    else
    {
        Q_ASSERT(!(index < 0 || index > d_ptr->rootMenuItem->subItems.size()));
        if (index < 0 || index > d_ptr->rootMenuItem->subItems.size())
            return false;

        TNavMenuItemPtr newMenuItem(new ANavigationViewPrivate::MenuItem(newItem, d_ptr->rootMenuItem));
        d_ptr->rootMenuItem->subItems.push_back(newMenuItem);
    }

    newItem->setParent(d_ptr->panel);

    // TODO: update

    return newItem;
}

bool ANavigationView::appendMenuItem(ANavigationMenuItem* newItem, ANavigationMenuItem* parentItem)
{
    return insertMenuItem(newItem, getMenuItemCount(parentItem), parentItem);
}

bool ANavigationView::removeMenuItem(ANavigationMenuItem* item)
{
    TNavMenuItemPtr menuItem = d_ptr->findItem(item);
    if (!menuItem)
        return false;

    QList<ANavigationMenuItem*> deleteItems;
    deleteItems.push_back(item);
    d_ptr->enumerator(menuItem, deleteItems);

    if (menuItem->parent)
        menuItem->parent.toStrongRef()->subItems.removeOne(menuItem);

    for (ANavigationMenuItem* delItem : deleteItems)
    {
        if (!delItem)
        {
            Q_ASSERT(false);
            continue;
        }

        delItem->deleteLater();
    }

    // TODO: update

    return true;
}

bool ANavigationView::removeMenuItem(int index, ANavigationMenuItem* parentItem)
{
    TNavMenuItemPtr removedMenuItem;
    if (parentItem)
    {
        TNavMenuItemPtr parentMenuItem = d_ptr->findItem(parentItem);
        if (!parentMenuItem)
            return false;
        removedMenuItem = parentMenuItem->subItems.at(index);
    }
    else 
        removedMenuItem = d_ptr->rootMenuItem->subItems.at(index);
    return removedMenuItem ? removeMenuItem(removedMenuItem->item) : false;
}

int ANavigationView::getMenuItemCount(ANavigationMenuItem* parent) const
{
    if (!parent)
        return d_ptr->rootMenuItem->subItems.size();

    const TNavMenuItemPtr parentMenuItem = d_ptr->findItem(parent);
    return parentMenuItem ? parentMenuItem->subItems.size() : 0;
}

ANavigationMenuItem* ANavigationView::getMenuItem(int index, ANavigationMenuItem* parent) const
{
    if (!parent)
        return d_ptr->rootMenuItem->subItems.at(index)->item;

    const TNavMenuItemPtr parentMenuItem = d_ptr->findItem(parent);
    if (!parentMenuItem)
        return nullptr;
    
    return parentMenuItem->subItems.at(index)->item;
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
    return QSize(1000, 680);
}

QSplitterHandle* ANavigationView::createHandle()
{
    auto sh = new ASplitterHandle(orientation(), this);
    /*connect(this, &ANavigationView::splitterMoved, [this](int pos, int index) {
        auto h = this->handle(index);
        if (!h)
            return;
        h->blockSignals(true);
        h->move(pos + 12, h->y());
        h->blockSignals(false);
    });*/

    return sh;
}

void ANavigationView::paintEvent(QPaintEvent* evt)
{
    APROCH_USE_STYLE_SHEET();
    return QSplitter::paintEvent(evt);
}

APROCH_NAMESPACE_END
