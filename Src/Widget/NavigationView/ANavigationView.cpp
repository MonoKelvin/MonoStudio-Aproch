#include "stdafx.h"
#include "ANavigationView.h"
#include "ANavigationMenuItem.h"
#include "ANavigationPageView.h"
#include "Widget/Private/ANavigationPanel.h"
#include "Widget/ASplitterHandle.h"

#include "Widget/Private/ANavigationView_p.h"
#include "Widget/Private/ANavigationPanel_p.h"

#include <QStandardItemModel>

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
    setAttribute(Qt::WA_StyledBackground);

    d_ptr->isResizable = true;

    // panel
    d_ptr->panel = new ANavigationPanel(position);
    d_ptr->oldPanelWidth = d_ptr->panel->sizeHint().width();
    
    // back button
    d_ptr->backButton = new ANavigationBackButton(d_ptr->panel);
    d_ptr->backButton->setEnabled(false);

    // compact button
    d_ptr->compactButton = new ANavigationCompactButton(d_ptr->headerText, d_ptr->panel);
    d_ptr->compactButton->setChecked(d_ptr->isExpanded);
    connect(d_ptr->compactButton, &ANavigationCompactButton::toggled, this, &ANavigationView::setExpanded);

    // menu item view(horizontal)
    d_ptr->menuItemView = new ANavigationMenuItemTreeView(d_ptr->panel);
    d_ptr->menuItemView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    d_ptr->menuItemView->setHeaderHidden(true);
    d_ptr->menuItemView->setIndentation(30);
    d_ptr->menuItemView->setRootIsDecorated(false);
    d_ptr->menuItemView->setAnimated(true);
    d_ptr->menuItemView->setIconSize(AFontIcon::DefaultIconSize);
    d_ptr->menuItemView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    d_ptr->menuItemView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d_ptr->menuItemView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    d_ptr->menuItemView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    // view delegate 
    //d_ptr->menuItemDelegate = new ANavigationMenuItemDelegate(d_ptr->menuItemView);
    //d_ptr->menuItemView->setItemDelegate(d_ptr->menuItemDelegate);

    // view model
    //d_ptr->menuItemModel = new ANavigationMenuItemModel(d_ptr->menuItemView);
    //d_ptr->menuItemView->setModel(d_ptr->menuItemModel);

    d_ptr->footerWidget = new QWidget(d_ptr->panel);
    d_ptr->headerWidget = new QWidget(d_ptr->panel);
    setHeaderVisible(false);
    setFooterVisible(false);

    // footer separator
    ANavigationViewItemSeparator* footerSeparator = new ANavigationViewItemSeparator(mainDir, d_ptr->panel);
    connect(this, &ANavigationView::panelPositionChanged, footerSeparator, [=]() {
        footerSeparator->setOrientation(getPanelPosition() == EPanelPosition::Top ? Qt::Vertical : Qt::Horizontal);
    });

    // settings button
    d_ptr->settingsButton = new ANavigationSettingsButton(d_ptr->panel);

    // widgets
    d_ptr->panel->layout()->addWidget(d_ptr->backButton);
    d_ptr->panel->layout()->addWidget(d_ptr->compactButton);
    d_ptr->panel->layout()->addWidget(d_ptr->headerWidget);
    d_ptr->panel->layout()->addWidget(d_ptr->menuItemView);
    d_ptr->panel->layout()->addWidget(d_ptr->footerWidget);
    d_ptr->panel->layout()->addWidget(footerSeparator);
    d_ptr->panel->layout()->addWidget(d_ptr->settingsButton);

    // page view
    d_ptr->pageView = new ANavigationPageView();
    d_ptr->pageView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    addWidget(d_ptr->panel);
    addWidget(d_ptr->pageView);

    setPanelPosition(position);

    // computer the min-compact-width
    QStyleOptionButton opt;
    opt.initFrom(d_ptr->compactButton);
    opt.features = QStyleOptionButton::Flat;
    opt.iconSize = d_ptr->compactButton->iconSize();
    opt.icon = d_ptr->compactButton->icon();
    opt.text = d_ptr->compactButton->text();
    const QRect contentsRect = d_ptr->compactButton->style()->subElementRect(QStyle::SE_PushButtonContents, &opt);
    const int realOffset = qreal(contentsRect.left() * 2.0) / devicePixelRatio();
    d_ptr->compactWidth = d_ptr->compactButton->iconSize().width() + realOffset;
    d_ptr->compactWidth = 50;
}

ANavigationView::EPanelPosition ANavigationView::getPanelPosition() const
{
    return d_ptr->panelPosition;
}

void ANavigationView::setPanelPosition(EPanelPosition position)
{
    if (position == d_ptr->panelPosition)
        return;

    d_ptr->panel->setPosition(position);

    if (position == EPanelPosition::Top)
    {
        setOrientation(Qt::Vertical);
        setHandleWidth(0);

        handle(1)->setEnabled(false);
        handle(1)->setVisible(false);
    }
    else
    {
        setOrientation(Qt::Horizontal);

        if (d_ptr->isResizable)
        {
            d_ptr->panel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);

            handle(1)->setEnabled(true);
            handle(1)->setVisible(true);
        }
        else
        {
            d_ptr->panel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
            d_ptr->panel->setFixedWidth(qMin(d_ptr->panel->width(), d_ptr->oldPanelWidth));

            handle(1)->setEnabled(false);
            handle(1)->setVisible(false);
        }
    }

    d_ptr->panelPosition = position;
    emit panelPositionChanged();
}

/*ANavigationView::ECompactMode ANavigationView::getCompactMode() const
{
    return d_ptr->compactMode;
}

void ANavigationView::setCompactMode(ECompactMode compactMode)
{
    if (compactMode == d_ptr->compactMode)
        return;

    d_ptr->compactMode = compactMode;
    emit compactModeChanged();
}*/

bool ANavigationView::isExpanded() const
{
    return d_ptr->isExpanded;
}

int ANavigationView::getCompactWidth() const
{
    return d_ptr->compactWidth;
}

void ANavigationView::setCompactWidth(int w)
{
    d_ptr->compactWidth = w;
    if (getPanelPosition() == EPanelPosition::Top || d_ptr->isExpanded)
        return;
}

bool ANavigationView::isPanelResizable() const
{
    return d_ptr->isResizable;
}

void ANavigationView::setPanelResizable(bool enabled)
{
    if (d_ptr->isResizable == enabled)
        return;

    d_ptr->isResizable = enabled;

    // TODO
}

bool ANavigationView::isTrackBarVisible() const
{
    return d_ptr->menuItemView->isTrackBarVisible();
}

void ANavigationView::setTrackBarVisible(bool visible)
{
    d_ptr->menuItemView->setTrackBarVisible(visible);
}

bool ANavigationView::isBackEnabled() const
{
    return d_ptr->backButton && d_ptr->backButton->isEnabled() && d_ptr->backButton->isVisible();
}

void ANavigationView::setBackEnabled(bool enabled)
{
    if (!d_ptr->backButton)
        return;
    d_ptr->backButton->setEnabled(enabled);
}

bool ANavigationView::isBackVisible() const
{
    return d_ptr->backButton && d_ptr->backButton->isVisible();
}

void ANavigationView::setBackVisible(bool visible)
{
    if (!d_ptr->backButton)
        return;
    d_ptr->backButton->setVisible(visible);
}

QString ANavigationView::getHeaderText() const
{
    if (d_ptr->compactButton)
        d_ptr->headerText = d_ptr->compactButton->text();

    return d_ptr->headerText;
}

void ANavigationView::setHeaderText(const QString& text)
{
    d_ptr->headerText = text;

    if (d_ptr->compactButton)
    {
        d_ptr->compactButton->setText(text);

        const auto odlsp = d_ptr->compactButton->sizePolicy();
        if (text.isEmpty())
            d_ptr->compactButton->setSizePolicy(QSizePolicy::Fixed, odlsp.verticalPolicy());
        else
            d_ptr->compactButton->setSizePolicy(QSizePolicy::Expanding, odlsp.verticalPolicy());
    }
}

QWidget* ANavigationView::getHeader() const
{
    return d_ptr->headerWidget;
}

void ANavigationView::setHeader(QWidget* header)
{
    Q_ASSERT(d_ptr->panel->layout());
    
    if (header)
        d_ptr->panel->layout()->replaceWidget(d_ptr->headerWidget, header);
    else 
        d_ptr->panel->layout()->removeWidget(d_ptr->headerWidget);

    if (d_ptr->headerWidget)
    {
        d_ptr->headerWidget->deleteLater();
    }

    d_ptr->headerWidget = header;
}

bool ANavigationView::isHeaderVisible() const
{
    return d_ptr->headerWidget && d_ptr->headerWidget->isVisible();
}

void ANavigationView::setHeaderVisible(bool visible)
{
    if (!d_ptr->headerWidget)
        return;
    d_ptr->headerWidget->setVisible(visible);
}

ANavigationMenuItem* ANavigationView::insertMenuItem(const QString& text, const QIcon& icon, int index, ANavigationMenuItem* parentItem)
{
    ANavigationMenuItem* newItem = new ANavigationMenuItem(text, icon, d_ptr->panel);
    if (insertItem(newItem, index, parentItem))
        return newItem;

    delete newItem;
    return nullptr;
}

ANavigationMenuItemGroup* ANavigationView::insertMenuItemGroup(const QString& text, int index)
{
    ANavigationMenuItemGroup* newItem = new ANavigationMenuItemGroup(text, d_ptr->panel);
    if (insertItem(newItem, index, nullptr))
        return newItem;

    delete newItem;
    return nullptr;
}

ANavigationViewItemSeparator* ANavigationView::insertSeparator(int index)
{
    ANavigationViewItemSeparator* newItem = new ANavigationViewItemSeparator(orientation(), d_ptr->panel);
    if (insertItem(newItem, index, nullptr))
        return newItem;

    delete newItem;
    return nullptr;
}

bool ANavigationView::insertItem(ANavigationViewItemBase* newItem, int index, ANavigationViewItemBase* parentItem)
{
    if (!newItem)
        return false;

    newItem->setParent(d_ptr->menuItemView);

    QTreeWidgetItem* pItem = nullptr;
    if (parentItem)
    {
        QTreeWidgetItem* parentMenuItem = d_ptr->menuItemView->getItemFromWidget(parentItem);
        if (!parentMenuItem)
            return false;

        Q_ASSERT(!(index < 0 || index > parentMenuItem->childCount()));
        if (index < 0 || index > parentMenuItem->childCount())
            return false;

        pItem = new QTreeWidgetItem(parentMenuItem);
    }
    else
    {
        Q_ASSERT(!(index < 0 || index > d_ptr->menuItemView->topLevelItemCount()));
        if (index < 0 || index > d_ptr->menuItemView->topLevelItemCount())
            return false;

        pItem = new QTreeWidgetItem(d_ptr->menuItemView);
    }

    d_ptr->menuItemView->setItemWidget(pItem, 0, newItem);
    newItem->installEventFilter(d_ptr->menuItemView);

    auto itemGroup = qobject_cast<ANavigationMenuItemGroup*>(newItem);
    if (itemGroup)
    {
        pItem->setFlags(Qt::ItemFlags(Qt::ItemFlag::NoItemFlags));
    }

    return newItem;
}

bool ANavigationView::appendItem(ANavigationViewItemBase* newItem, ANavigationViewItemBase* parentItem)
{
    if (parentItem)
    {
        QTreeWidgetItem* twItem = d_ptr->menuItemView->getItemFromWidget(parentItem);
        if (!twItem)
            return false;

        return insertItem(newItem, twItem->childCount(), parentItem);
    }

    return insertItem(newItem, d_ptr->menuItemView->topLevelItemCount(), parentItem);
}

bool ANavigationView::removeItem(ANavigationViewItemBase* item)
{
    QTreeWidgetItem* twItem = d_ptr->menuItemView->getItemFromWidget(item);
    if (!twItem)
        return false;

    auto menuItemMap = d_ptr->menuItemView->getItemMap(twItem);
    for (auto iter = menuItemMap.cbegin(); iter != menuItemMap.cend(); ++iter)
        d_ptr->menuItemView->removeItemWidget(iter.key(), 0);

    delete twItem;

    return true;
}

bool ANavigationView::removeItem(int index, ANavigationViewItemBase* parentItem)
{
    ANavigationViewItemBase* removedMenuItem;
    if (parentItem)
    {
        QTreeWidgetItem* twItem = d_ptr->menuItemView->getItemFromWidget(parentItem);
        if (!twItem)
            return false;
        removedMenuItem = d_ptr->menuItemView->getWidgetFromItem(twItem->child(index));
    }
    else
        removedMenuItem = d_ptr->menuItemView->getWidgetFromItem(d_ptr->menuItemView->topLevelItem(index));
    return removedMenuItem ? removeItem(removedMenuItem) : false;
}

int ANavigationView::getItemCount(ANavigationViewItemBase* parent) const
{
    QTreeWidgetItem* twItem = d_ptr->menuItemView->getItemFromWidget(parent);
    auto menuItemList = d_ptr->menuItemView->getItemList(twItem);
    return menuItemList.size();
}

ANavigationViewItemBase* ANavigationView::getItemAt(int index, ANavigationViewItemBase* parent) const
{
    QTreeWidgetItem* twItem = d_ptr->menuItemView->getItemFromWidget(parent);
    auto menuItemList = d_ptr->menuItemView->getItemList(twItem);
    return menuItemList.at(index);
}

QWidget* ANavigationView::getFooter() const
{
    return d_ptr->footerWidget;
}

void ANavigationView::setFooter(QWidget* footer)
{
    Q_ASSERT(d_ptr->panel->layout());

    if (footer)
        d_ptr->panel->layout()->replaceWidget(d_ptr->footerWidget, footer);
    else
        d_ptr->panel->layout()->removeWidget(d_ptr->footerWidget);

    if (d_ptr->footerWidget)
    {
        d_ptr->footerWidget->deleteLater();
    }

    d_ptr->footerWidget = footer;
}

bool ANavigationView::isFooterVisible() const
{
    return d_ptr->footerWidget && d_ptr->footerWidget->isVisible();
}

void ANavigationView::setFooterVisible(bool visible)
{
    if (!d_ptr->footerWidget)
        return;
    d_ptr->footerWidget->setVisible(visible);
}

bool ANavigationView::isSettingsVisible() const
{
    return d_ptr->settingsButton && d_ptr->settingsButton->isVisible();
}

void ANavigationView::setSettingsVisible(bool visible)
{
    if (!d_ptr->settingsButton)
        return;
    d_ptr->settingsButton->setVisible(visible);
}

ANavigationPageView* ANavigationView::getPageView() const
{
    return d_ptr->pageView;
}

QSize ANavigationView::sizeHint() const
{
    auto defaultSize = QSplitter::sizeHint();
    switch (getPanelPosition())
    {
    case aproch::ANavigationView::Top:
        return QSize(defaultSize.width(), 60);
    default:
        break;
    }
    return QSize(defaultSize.width(), 680);
}

QSplitterHandle* ANavigationView::createHandle()
{
    auto sh = new ASplitterHandle(orientation(), this);
    sh->grabMouse();
    sh->installEventFilter(this);

    d_ptr->oldHandleWidth = orientation() == Qt::Horizontal ? sh->sizeHint().width() : sh->sizeHint().height();
    connect(this, &ANavigationView::splitterMoved, this, &ANavigationView::updatePageViewPosition);

    return sh;
}

void ANavigationView::showEvent(QShowEvent* evt)
{
    updatePageViewPosition(d_ptr->panel->sizeHint().width(), 1);

    d_ptr->oldPanelWidth = d_ptr->panel->width();

    return QSplitter::showEvent(evt);
}

void ANavigationView::paintEvent(QPaintEvent* evt)
{
    return QSplitter::paintEvent(evt);
}

bool ANavigationView::eventFilter(QObject* watched, QEvent* evt)
{
    auto hd = handle(1);
    if (watched == hd)
    {
        if (!isExpanded() || !isPanelResizable())
        {
            hd->setEnabled(false);
            hd->setVisible(false);
        }
        else
        {
            hd->setEnabled(true);
            hd->setVisible(true);

            if (orientation() == Qt::Horizontal)
            {
                updatePageViewPosition(hd->pos().x(), 1);
            }
        }
    }

    return QSplitter::eventFilter(watched, evt);
}

void ANavigationView::updatePageViewPosition(int pos, int index)
{
    auto hd = handle(index);
    const bool isHor = orientation() == Qt::Horizontal;
    const auto& geo = d_ptr->pageView->geometry();
    if (isHor)
    {
        int offset = hd ? hd->width() : d_ptr->oldHandleWidth;
        d_ptr->pageView->setGeometry(pos, geo.y(), geo.width() + offset, geo.height());

        const int minWidth = d_ptr->compactWidth * 2;
        if (pos > minWidth)
            d_ptr->menuItemView->setColumnWidth(0, pos);
        else
            d_ptr->menuItemView->setColumnWidth(0, minWidth);
    }
    else
    {
        int offset = hd ? hd->height() : d_ptr->oldHandleWidth;
        d_ptr->pageView->setGeometry(geo.x(), pos, geo.width(), geo.height() + offset);
    }
}

void ANavigationView::setExpanded(bool expand)
{
    if (getPanelPosition() == EPanelPosition::Top)
        return;

    if (d_ptr->isExpanded == expand)
        return;

    // TODO: add animation

    d_ptr->isExpanded = expand;
    auto oldVPolicy = d_ptr->panel->sizePolicy().verticalPolicy();
    if (expand)
    {
        if (d_ptr->isResizable)
        {
            d_ptr->panel->setMinimumWidth(d_ptr->compactWidth);
            d_ptr->panel->setMaximumWidth(QWIDGETSIZE_MAX);
            d_ptr->panel->resize(d_ptr->oldPanelWidth, d_ptr->panel->height());
        }
        else
        {
            d_ptr->panel->setFixedWidth(d_ptr->oldPanelWidth);
        }
    }
    else
    {
        d_ptr->oldPanelWidth = d_ptr->panel->width();
        d_ptr->panel->setFixedWidth(d_ptr->compactWidth);
    }

    static QMap<int, bool> expandedStateMap;

    // show/hide menu item group and expand/collapse button
    auto allMenuItemMap = d_ptr->menuItemView->getItemMap();
    for (auto iter = allMenuItemMap.cbegin(); iter != allMenuItemMap.cend(); ++iter)
    {
        auto pTreeItem = iter.key();
        auto pItem = iter.value();

        // top-level items need to remember expanded/collapse state
        if (pTreeItem->parent() == nullptr)
        {
            const int topLevelIndex = d_ptr->menuItemView->indexOfTopLevelItem(pTreeItem);
            if (expand)
                pTreeItem->setExpanded(expandedStateMap.value(topLevelIndex, false));
            else
            {
                expandedStateMap[topLevelIndex] = pTreeItem->isExpanded();
                pTreeItem->setExpanded(false);
            }
        }

        // hide all group items
        if (qobject_cast<ANavigationMenuItemGroup*>(pItem))
        {
            iter.key()->setHidden(!expand);
            continue;
        }

        auto menuItem = qobject_cast<ANavigationMenuItem*>(pItem);
        if (menuItem)
            menuItem->setExpandedButtonVisible(expand);
    }

    if (expand)
    {
        d_ptr->menuItemView->setColumnWidth(0, d_ptr->menuItemView->width());
    }
    else
    {
        d_ptr->menuItemView->setColumnWidth(0, d_ptr->panel->width());
    }

    emit expandChanged();
}

APROCH_NAMESPACE_END
