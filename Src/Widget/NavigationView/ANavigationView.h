/****************************************************************************
 * @file    ANavigationView.h
 * @date    2024-07-09 
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
#include "AprochExportDefine.h"
#include <QFrame>

class QTreeView;

APROCH_NAMESPACE_BEGIN

class ANavigationViewPrivate;
class ANavigationViewItem;
class ANavigationViewItemGroup;
class ANavigationPageView;

/**
 * @brief 导航视图
 */
class APROCH_API ANavigationView : public QFrame
{
    Q_OBJECT;

    Q_PROPERTY(EPanelPosition panelPosition READ getPanelPosition WRITE setPanelPosition NOTIFY panelPositionChanged);
    Q_PROPERTY(ECompactMode compactMode READ getCompactMode WRITE setCompactMode);
    Q_PROPERTY(QString headerText READ getHeaderText WRITE setHeaderText);
    Q_PROPERTY(bool isHeaderVisible READ isHeaderVisible WRITE setHeaderVisible);
    Q_PROPERTY(bool isFooterVisible READ isFooterVisible WRITE setFooterVisible);
    Q_PROPERTY(bool isPanelResizable READ isPanelResizable WRITE setPanelResizable);
    Q_PROPERTY(bool isBackEnabled READ isBackEnabled WRITE setBackEnabled);
    Q_PROPERTY(bool isBackVisible READ isBackVisible WRITE setBackVisible);
    Q_PROPERTY(bool isSettingsVisible READ isSettingsVisible WRITE setSettingsVisible);

public:
    enum EPanelPosition
    {
        Left = 0x01,
        //Right = 0x02,
        Top = 0x03,
        //Bottom = 0x04
    };
    Q_ENUM(EPanelPosition);

    enum ECompactMode
    {
        Expand = 0x01,
        Compact = 0x02,
        Floating = 0x03,
    };
    Q_ENUM(ECompactMode);

    enum ItemRole
    {
        Other,
        BackButton,
        CompactButton,
        MenuItem,
        Settings,
    };
    Q_ENUM(ItemRole);

public:
    explicit ANavigationView(QWidget* parent = nullptr);
    explicit ANavigationView(EPanelPosition position, QWidget* parent = nullptr);

    EPanelPosition getPanelPosition() const;
    void setPanelPosition(EPanelPosition position);

    ECompactMode getCompactMode() const;
    void setCompactMode(ECompactMode compactMode);

    bool isPanelResizable() const;
    void setPanelResizable(bool enabled);

    /** Back Button */

    bool isBackEnabled() const;
    void setBackEnabled(bool enabled);
    bool isBackVisible() const;
    void setBackVisible(bool visible);

    /** Header */

    QString getHeaderText() const;
    void setHeaderText(const QString& text);

    QWidget* getHeader() const;
    void setHeader(QWidget* header);
    bool isHeaderVisible() const;
    void setHeaderVisible(bool visible);

    /** MenuItems */

    void setMenuItems(QList<ANavigationViewItem*> menuItems);
    QList<ANavigationViewItem*> getMenuItems() const;

    ANavigationViewItem* insertMenuItem(const QString& text, const QIcon& icon, int index, ANavigationViewItem* parentItem = nullptr);
    bool insertMenuItem(ANavigationViewItem* newItem, int index, ANavigationViewItem* parentItem = nullptr);
    bool appendMenuItem(ANavigationViewItem* newItem, ANavigationViewItem* parentItem = nullptr);
    bool removeMenuItem(ANavigationViewItem* item);
    bool removeMenuItem(int index, ANavigationViewItem* parentItem = nullptr);
    ANavigationViewItem* getMenuItem(int index, ANavigationViewItem* parent = nullptr) const;

    ANavigationViewItemGroup* insertItemGroup(const QString& text, ANavigationViewItem* rootItem = nullptr);
    bool insertItemGroup(ANavigationViewItemGroup* group, ANavigationViewItem* rootItem);
    bool appendItemGroup(ANavigationViewItemGroup* group);
    bool removeItemGroup(ANavigationViewItemGroup* group);
    bool removeItemGroup(int index);
    ANavigationViewItemGroup* getItemGroup(int index) const;

    QTreeView* getMenuItemView() const;

    /** Footer */

    QWidget* getFooter() const;
    void setFooter(QWidget* footer);
    bool isFooterVisible() const;
    void setFooterVisible(bool visible);

    /** Settings */
    bool isSettingsVisible() const;
    void setSettingsVisible(bool visible);

    /** page View */

    ANavigationPageView* getContentView() const;

    /** Other Methods */

    virtual QSize sizeHint() const override;

protected:

Q_SIGNALS:
    void panelPositionChanged();
    void compactModeChanged();
    void selectionChanged(ANavigationViewItem* item);
    void trigger(ItemRole role);

private:
    Q_DISABLE_COPY_MOVE(ANavigationView);
    QSharedPointer<ANavigationViewPrivate> d_ptr;
};

APROCH_NAMESPACE_END