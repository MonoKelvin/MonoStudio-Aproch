/****************************************************************************
 * @file    ANavigationPanel.cpp
 * @date    2024-07-12 
 * @author  MonoKelvin
 * @email   15007083506@qq.com
 * @github  https://github.com/MonoKelvin
 * @brief
 *
 * This source file is part of Aproch.
 * Copyright (C) 2020 by MonoKelvin. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *****************************************************************************/
#include "stdafx.h"
#include "ANavigationPanel.h"
#include "Widget/Private/ANavigationPanel_p.h"
#include "Widget/ATrackBar.h"

#include <QProgressBar>
#include <QWindow>
#include <QPainterPath>

APROCH_NAMESPACE_BEGIN

////////////////////////////////////////////////////////////////////////////////////////////////////

ANavigationMenuItemDelegate::ANavigationMenuItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
    , d_ptr(new ANavigationMenuItemDelegatePrivate())
{
}

QSize ANavigationMenuItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    QTreeWidgetItem* pItem = (QTreeWidgetItem*)index.internalPointer();
    if (!pItem)
        return size;

    return QSize(size.width(), 26);
}

void ANavigationMenuItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QStyledItemDelegate::paint(painter, option, index);
}

QWidget* ANavigationMenuItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return nullptr;
}

void ANavigationMenuItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
}

void ANavigationMenuItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
}

void ANavigationMenuItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    editor->setGeometry(option.rect);
}


////////////////////////////////////////////////////////////////////////////////////////////////////


ANavigationMenuItemTreeView::ANavigationMenuItemTreeView(QWidget* parent)
    : QTreeWidget(parent)
    , d_ptr(new ANavigationMenuItemTreeViewPrivate())
{
    d_ptr->trackBar = new ATrackBar(this);
    d_ptr->trackBar->raise();

    connect(this, &QTreeView::clicked, [=](const QModelIndex& index) {

        auto pItem = itemFromIndex(index);
        if (!pItem || pItem->childCount() == 0)
            return;

        if (isExpanded(index))
            collapse(index);
        else
            expand(index);
    });

    auto selModel = selectionModel();
    connect(selModel, &QItemSelectionModel::currentRowChanged, this, &ANavigationMenuItemTreeView::selectedChagned);

    auto theModel = model();
    connect(theModel, &QAbstractItemModel::rowsInserted, this, &ANavigationMenuItemTreeView::updateExpandedState);
    connect(theModel, &QAbstractItemModel::rowsRemoved, this, &ANavigationMenuItemTreeView::updateExpandedState);
    connect(this, &ANavigationMenuItemTreeView::expanded, this, &ANavigationMenuItemTreeView::updateExpandedState);
    connect(this, &ANavigationMenuItemTreeView::collapsed, this, &ANavigationMenuItemTreeView::updateExpandedState);

    selectedChagned(QModelIndex(), QModelIndex());
}

bool ANavigationMenuItemTreeView::isTrackBarVisible() const
{
    return d_ptr->trackBar && !d_ptr->trackBar->isHidden();
}

void ANavigationMenuItemTreeView::setTrackBarVisible(bool visible)
{
    if (d_ptr->trackBar)
        d_ptr->trackBar->setVisible(visible);
    if (d_ptr->nextTrackBar)
        d_ptr->nextTrackBar->setVisible(visible);
}

QTreeWidgetItem* ANavigationMenuItemTreeView::getItemFromWidget(ANavigationViewItemBase* itemBase) const
{
    if (!itemBase)
        return nullptr;

    for (int i = 0; i < topLevelItemCount(); ++i)
    {
        auto item = topLevelItem(i);
        if (!item)
            continue;

        item = d_ptr->findMenuItem(this, item, itemBase);
        if (item)
            return item;
    }

    return nullptr;
}

ANavigationViewItemBase* ANavigationMenuItemTreeView::getWidgetFromItem(QTreeWidgetItem* itemBase) const
{
    return qobject_cast<ANavigationViewItemBase*>(itemWidget(itemBase, 0));
}

QMap<QTreeWidgetItem*, ANavigationViewItemBase*> ANavigationMenuItemTreeView::getItemMap(QTreeWidgetItem* parentItem) const
{
    QMap<QTreeWidgetItem*, ANavigationViewItemBase*> itemBaseMap;
    if (parentItem)
    {
        d_ptr->getMenuItemMap(this, parentItem, itemBaseMap);
    }
    else
    {
        for (int i = 0; i < topLevelItemCount(); ++i)
        {
            auto item = topLevelItem(i);
            if (!item)
                continue;

            d_ptr->getMenuItemMap(this, item, itemBaseMap);
        }
    }

    return itemBaseMap;
}

QList<ANavigationViewItemBase*> ANavigationMenuItemTreeView::getItemList(QTreeWidgetItem* parentItem) const
{
    QList<ANavigationViewItemBase*> itemBaseList;

    if (parentItem)
    {
        for (int i = 0; i < parentItem->childCount(); ++i)
        {
            auto item = parentItem->child(i);
            if (!item)
                continue;

            auto itemBase = getWidgetFromItem(item);
            if (itemBase)
                itemBaseList.push_back(itemBase);
        }
    }
    else
    {
        for (int i = 0; i < topLevelItemCount(); ++i)
        {
            auto item = topLevelItem(i);
            if (!item)
                continue;

            auto itemBase = getWidgetFromItem(item);
            if (itemBase)
                itemBaseList.push_back(itemBase);
        }
    }

    return itemBaseList;
}

void ANavigationMenuItemTreeView::scrollContentsBy(int dx, int dy)
{
    return QTreeWidget::scrollContentsBy(0, dy);
}

void ANavigationMenuItemTreeView::updateExpandedState(const QModelIndex& index)
{
    auto item = itemFromIndex(index);
    auto menuItem = qobject_cast<ANavigationMenuItem*>(getWidgetFromItem(item));
    if (menuItem)
    {
        if (item->childCount() > 0)
        {
            if (item->isExpanded())
                menuItem->setExpandState(ANavigationMenuItem::Expanded);
            else
                menuItem->setExpandState(ANavigationMenuItem::Collapsed);
        }
        else
            menuItem->setExpandState(ANavigationMenuItem::NoExpandState);
    }
}

void ANavigationMenuItemTreeView::selectedChagned(const QModelIndex& current, const QModelIndex& previous)
{
    auto pItem = itemFromIndex(current);
    if (!pItem)
    {
        d_ptr->trackbarVisibleRestore = isTrackBarVisible();
        setTrackBarVisible(false);
        return;
    }

    if (d_ptr->trackbarVisibleRestore)
        setTrackBarVisible(true);

    if (isTrackBarVisible())
    {
        const QRect itemRect = visualItemRect(pItem);
        d_ptr->trackBar->getAnimation()->stop();
        d_ptr->trackBar->move(itemRect.left() + d_ptr->trackBar->width() / 2,
                              itemRect.center().y() - d_ptr->trackBar->height() / 2);
    }
}

void ANavigationMenuItemTreeView::showEvent(QShowEvent* evt)
{
    QTreeWidget::showEvent(evt);
}

bool ANavigationMenuItemTreeView::eventFilter(QObject* watched, QEvent* evt)
{
    QEvent::Type et = evt->type();
    auto menuItem = qobject_cast<ANavigationMenuItem*>(watched);
    if (menuItem)
    {
        if (et == QEvent::ShowToParent)
        {
            auto item = getItemFromWidget(menuItem);
            updateExpandedState(indexFromItem(item));
        }
    }

    return QTreeWidget::eventFilter(watched, evt);
}

void ANavigationMenuItemTreeView::drawRow(QPainter* painter, const QStyleOptionViewItem& options, const QModelIndex& index) const
{
    painter->setRenderHint(QPainter::Antialiasing);

    QStyleOptionViewItem opt(options);
    QPainterPath path;
    path.addRoundedRect(opt.rect.x(), opt.rect.y() + 2,
                        opt.rect.width(), opt.rect.height() - 4, 6, 6);

    QBrush bgBrush = Qt::NoBrush;
    auto item = itemFromIndex(index);
    if (!item)
        return QTreeWidget::drawRow(painter, opt, index);

    bool hover = item == itemAt(mapFromGlobal(QCursor::pos()));
    if (item->isSelected())
    {
        if (hover)
            bgBrush = QColor::fromRgbF(1.0f, 1.0f, 1.0f, 0.04f);
        else
            bgBrush = QColor::fromRgbF(1.0f, 1.0f, 1.0f, 0.06f);
    }
    else if (hover)
        bgBrush = QColor::fromRgbF(1.0f, 1.0f, 1.0f, 0.06f);

    painter->fillPath(path, bgBrush);
}


////////////////////////////////////////////////////////////////////////////////////////////////////


ANavigationPanel::ANavigationPanel(QWidget* parent)
    : ANavigationPanel(ANavigationView::Left)
{
}

ANavigationPanel::ANavigationPanel(ANavigationView::EPanelPosition position, QWidget* parent)
    : QWidget(parent)
    , d_ptr(new ANavigationPanelPrivate())
{
    setAttribute(Qt::WA_StyledBackground);

    QBoxLayout::Direction dir = (position == ANavigationView::Top ?
                                 QBoxLayout::LeftToRight : QBoxLayout::TopToBottom);
    QBoxLayout* mainLayout = new QBoxLayout(dir, this);
    mainLayout->setSpacing(0);
    //mainLayout->setContentsMargins(4, 0, 4, 6);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setPosition(position);
}

void ANavigationPanel::setPosition(ANavigationView::EPanelPosition position)
{
    if (position == d_ptr->position)
        return;

    d_ptr->position = position;

    QBoxLayout* mainLayout = qobject_cast<QBoxLayout*>(layout());
    Q_ASSERT(mainLayout);

    if (position == ANavigationView::Top)
    {
        mainLayout->setDirection(QBoxLayout::LeftToRight);
        
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        setMinimumWidth(0);
        setMaximumWidth(QWIDGETSIZE_MAX);
        setFixedHeight(sizeHint().height());
    }
    else
    {
        mainLayout->setDirection(QBoxLayout::TopToBottom);

        setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
        setMinimumSize(minimumSizeHint());
        setMaximumWidth(QWIDGETSIZE_MAX);
        setMaximumHeight(QWIDGETSIZE_MAX);
        resize(sizeHint());
    }
}

ANavigationView::EPanelPosition ANavigationPanel::getPosition() const
{
    QBoxLayout* mainLayout = qobject_cast<QBoxLayout*>(layout());
    Q_ASSERT(mainLayout);

    return (mainLayout->direction() == QBoxLayout::LeftToRight ||
            mainLayout->direction() == QBoxLayout::RightToLeft) ? ANavigationView::Top : ANavigationView::Left;
}

QSize ANavigationPanel::sizeHint() const
{
    auto defaultSize = QWidget::sizeHint();
    switch (getPosition())
    {
    case aproch::ANavigationView::Top:
        return QSize(defaultSize.width(), 60);
    default:
        break;
    }
    return QSize(320, defaultSize.height());
}

QSize ANavigationPanel::minimumSizeHint() const
{
    auto defaultSize = QWidget::sizeHint();
    switch (getPosition())
    {
    case aproch::ANavigationView::Top:
        return QSize(defaultSize.width(), 44);
    default:
        break;
    }
    return QSize(52, defaultSize.height());
}

APROCH_NAMESPACE_END
