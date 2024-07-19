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

ANavigationMenuItemModel::ANavigationMenuItemModel(QObject* parent)
    : d_ptr(new ANavigationMenuItemModelPrivate())
{
}

QVariant ANavigationMenuItemModel::data(const QModelIndex& index, int role) const
{
    return QVariant();
    SNavigationMenuItem* pItem = (SNavigationMenuItem*)index.internalPointer();
    if (!pItem)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        return pItem->item->text();
    }
    else if (role == Qt::DecorationRole)
    {
        return pItem->item->icon();
    }

    return QVariant();
}

QModelIndex ANavigationMenuItemModel::index(int row, int column, const QModelIndex& parent) const
{
    SNavigationMenuItem* pItem = (SNavigationMenuItem*)parent.internalPointer();
    if (nullptr == pItem)
    {
        return createIndex(row, column, d_ptr->rootMenuItem->subItems.at(row).get());
    }

    return createIndex(row, column, pItem->subItems.at(row).get());
}

QModelIndex ANavigationMenuItemModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
        return QModelIndex();

    SNavigationMenuItem* pItem = (SNavigationMenuItem*)child.internalPointer();
    if (!pItem)
        return QModelIndex();
    auto pParent = pItem->parent.toStrongRef();
    if (!pItem->parent)
        return QModelIndex();

    return createIndex(pParent->subItems.indexOf(pItem), 0, pParent.get());
}

int ANavigationMenuItemModel::rowCount(const QModelIndex& parent) const
{
    Q_ASSERT(d_ptr->rootMenuItem);
    if (!parent.isValid())
        return d_ptr->rootMenuItem->subItems.size();

    SNavigationMenuItem* pItem = (SNavigationMenuItem*)parent.internalPointer();
    if (!pItem)
        return 0;
    return pItem->subItems.size();
}

int ANavigationMenuItemModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

void ANavigationMenuItemModel::enumerator(const QSharedPointer<SNavigationMenuItem>& item, 
                                          QList<QSharedPointer<SNavigationMenuItem>>& subItems)
{
    Q_ASSERT(item);
    subItems.append(item->subItems);
    for (const auto& item : item->subItems)
        enumerator(item, subItems);
}

void ANavigationMenuItemModel::enumerator(const QSharedPointer<SNavigationMenuItem>& item,
                                          QList<ANavigationMenuItem*>& items)
{
    Q_ASSERT(item);
    for (const auto& subItem : item->subItems)
    {
        Q_ASSERT(subItem);
        items.push_back(subItem->item);
        enumerator(subItem, items);
    }
}

QSharedPointer<SNavigationMenuItem> ANavigationMenuItemModel::findItem(const QSharedPointer<SNavigationMenuItem>& menuItem, 
                                                                       ANavigationMenuItem* item)
{
    if (!menuItem || !item)
        return nullptr;

    if (menuItem->item == item)
        return menuItem;

    for (const auto& subMenuItem : menuItem->subItems)
    {
        auto theItem = findItem(subMenuItem, item);
        if (theItem)
            return theItem;
    }
    return nullptr;
}

QSharedPointer<SNavigationMenuItem> ANavigationMenuItemModel::findItem(ANavigationMenuItem* item)
{
    for (const auto& subMenuItem : d_ptr->rootMenuItem->subItems)
    {
        auto theItem = findItem(subMenuItem, item);
        if (theItem)
            return theItem;
    }
    return nullptr;
}


////////////////////////////////////////////////////////////////////////////////////////////////////

ANavigationMenuItemDelegate::ANavigationMenuItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
    , d_ptr(new ANavigationMenuItemDelegatePrivate())
{
}

QSize ANavigationMenuItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    SNavigationMenuItem* pItem = (SNavigationMenuItem*)index.internalPointer();
    if (!pItem || !pItem->item)
        return size;

    return QSize(size.width(), pItem->item->height());
}

void ANavigationMenuItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    SNavigationMenuItem* pItem = (SNavigationMenuItem*)index.internalPointer();
    if (pItem && pItem->item)
    {
        pItem->item->resize(option.rect.width(), pItem->item->height());
        qreal dpr = pItem->item->devicePixelRatio();
        QPixmap pixmap(option.rect.size() * dpr);
        pixmap.setDevicePixelRatio(dpr);
        pItem->item->render(&pixmap, QPoint(), QRegion(), QWidget::RenderFlags(QWidget::DrawChildren));
        painter->drawPixmap(option.rect, pixmap);
    }

    return QStyledItemDelegate::paint(painter, option, index);
}

QWidget* ANavigationMenuItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    /*ANavigationMenuItem* item = new ANavigationMenuItem(parent);
    return item;*/
    //return QStyledItemDelegate::createEditor(parent, option, index);
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
    : QTreeView(parent)
    , d_ptr(new ANavigationMenuItemTreeViewPrivate())
{
    connect(this, &QTreeView::clicked, [=](const QModelIndex& index) {
        if (isExpanded(index))
            collapse(index);
        else
            expand(index);
    });
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
    mainLayout->setSpacing(0);
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
