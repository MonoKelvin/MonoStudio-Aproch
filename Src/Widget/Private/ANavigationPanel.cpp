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
    connect(this, &QTreeView::clicked, [=](const QModelIndex& index) {

        auto pItem = itemFromIndex(index);
        if (!pItem || pItem->childCount() == 0)
            return;

        if (isExpanded(index))
            collapse(index);
        else
            expand(index);
    });
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
