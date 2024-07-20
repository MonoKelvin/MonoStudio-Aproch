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
        if (isExpanded(index))
            collapse(index);
        else
            expand(index);
    });
}

QTreeWidgetItem* ANavigationMenuItemTreeView::itemFromMenuItem(ANavigationMenuItem* menuItem) const
{
    if (!menuItem)
        return nullptr;

    for (int i = 0; i < topLevelItemCount(); ++i)
    {
        auto item = topLevelItem(i);
        if (!item)
            continue;

        item = d_ptr->findMenuItem(this, item, menuItem);
        if (item)
            return item;
    }

    return nullptr;
}

ANavigationMenuItem* ANavigationMenuItemTreeView::menuItemFromItem(QTreeWidgetItem* menuItem) const
{
    return qobject_cast<ANavigationMenuItem*>(itemWidget(menuItem, 0));
}

QMap<QTreeWidgetItem*, ANavigationMenuItem*> ANavigationMenuItemTreeView::getMenuItemMap(QTreeWidgetItem* parentItem) const
{
    QMap<QTreeWidgetItem*, ANavigationMenuItem*> menuItemMap;
    if (parentItem)
    {
        d_ptr->getMenuItemMap(this, parentItem, menuItemMap);
    }
    else
    {
        for (int i = 0; i < topLevelItemCount(); ++i)
        {
            auto item = topLevelItem(i);
            if (!item)
                continue;

            d_ptr->getMenuItemMap(this, item, menuItemMap);
        }
    }
    
    return menuItemMap;
}

QList<ANavigationMenuItem*> ANavigationMenuItemTreeView::getMenuItemList(QTreeWidgetItem* parentItem) const
{
    QList<ANavigationMenuItem*> menuItemList;

    // TODO
    Q_ASSERT(false);

    return menuItemList;
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
    QBoxLayout::Direction dir = (position == ANavigationView::Top ?
                                 QBoxLayout::LeftToRight : QBoxLayout::TopToBottom);
    QBoxLayout* mainLayout = new QBoxLayout(dir, this);
    mainLayout->setSpacing(6);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setPosition(position);
}

void ANavigationPanel::setPosition(ANavigationView::EPanelPosition position)
{
    if (position == d_ptr->position)
        return;

    QBoxLayout* mainLayout = qobject_cast<QBoxLayout*>(layout());
    Q_ASSERT(mainLayout);

    QBoxLayout::Direction dir = (position == ANavigationView::Top ?
                                 QBoxLayout::LeftToRight : QBoxLayout::TopToBottom);
    mainLayout->setDirection(dir);
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
    switch (getPosition())
    {
    case aproch::ANavigationView::Top:
        //case aproch::ANavigationView::Bottom:
        return QSize(1000, 60);
    default:
        break;
    }
    return QSize(400, 680);
}

void ANavigationPanel::paintEvent(QPaintEvent* evt)
{
    APROCH_USE_STYLE_SHEET();

    QWidget::paintEvent(evt);
}

APROCH_NAMESPACE_END
