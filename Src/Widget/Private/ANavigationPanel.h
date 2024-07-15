/****************************************************************************
 * @file    ANavigationPanel.h
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
#pragma once
#include "Widget/NavigationView/ANavigationView.h"
#include "Widget/NavigationView/ANavigationMenuItem.h"

#include <QWidget>
#include <QStyledItemDelegate>
#include <QTreeView>

APROCH_NAMESPACE_BEGIN

class ANavigationMenuItemDelegatePrivate;
class ANavigationMenuItemTreeViewPrivate;
class ANavigationPanelPrivate;

class ANavigationMenuItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT;
public:
    explicit ANavigationMenuItemDelegate(QObject* parent = nullptr);

    void setMenuItems(QList<ANavigationMenuItem*> menuItems);
    QList<ANavigationMenuItem*> getMenuItems() const;

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    Q_DISABLE_COPY_MOVE(ANavigationMenuItemDelegate);
    QScopedPointer<ANavigationMenuItemDelegatePrivate> d_ptr;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class ANavigationMenuItemTreeView : public QTreeView
{
    Q_OBJECT;
public:
    explicit ANavigationMenuItemTreeView(QWidget* parent = nullptr);

private:
    Q_DISABLE_COPY_MOVE(ANavigationMenuItemTreeView);
    QScopedPointer<ANavigationMenuItemTreeViewPrivate> d_ptr;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class ANavigationPanel : public QWidget
{
    Q_OBJECT;
public:
    explicit ANavigationPanel(QWidget* parent = nullptr);
    explicit ANavigationPanel(ANavigationView::EPanelPosition position, QWidget* parent = nullptr);

    void setPosition(ANavigationView::EPanelPosition position);
    ANavigationView::EPanelPosition getPosition() const;

    virtual QSize sizeHint() const override;

private:
    friend class ANavigationView;
    friend class ANavigationViewPrivate;
    Q_DISABLE_COPY_MOVE(ANavigationPanel);
    QScopedPointer<ANavigationPanelPrivate> d_ptr;
};

APROCH_NAMESPACE_END