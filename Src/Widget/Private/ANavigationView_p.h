/****************************************************************************
 * @file    ANavigationView_p.h
 * @date    2024-07-11 
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

APROCH_NAMESPACE_BEGIN

class ANavigationPanel;
class ANavigationPageView;

class ANavigationViewPrivate
{
public:
    struct MenuItem
    {
        ANavigationMenuItem* item = nullptr;
        QWeakPointer<MenuItem> parent;
        QList<QSharedPointer<MenuItem>> subItems;

        MenuItem()
        {
        }

        explicit MenuItem(ANavigationMenuItem* menuItem, const QSharedPointer<MenuItem>& parentItem = nullptr)
            : item(menuItem)
            , parent(parentItem)
        {
        }

        bool operator==(const MenuItem& rhs) const noexcept
        {
            return rhs.item == item;
        }
    };

public:
    ANavigationViewPrivate(ANavigationView* q)
        : q_ptr(q)
    {
        rootMenuItem = QSharedPointer<MenuItem>(new MenuItem());
    }

    static void enumerator(const QSharedPointer<MenuItem>& item, QList<QSharedPointer<MenuItem>>& subItems)
    {
        Q_ASSERT(item);
        subItems.append(item->subItems);
        for (const auto& item : item->subItems)
            enumerator(item, subItems);
    }

    static void enumerator(const QSharedPointer<MenuItem>& item, QList<ANavigationMenuItem*>& items)
    {
        Q_ASSERT(item);
        for (const auto& subItem : item->subItems)
        {
            Q_ASSERT(subItem);
            items.push_back(subItem->item);
            enumerator(subItem, items);
        }
    }

    static QSharedPointer<MenuItem> findItem(const QSharedPointer<MenuItem>& menuItem, ANavigationMenuItem* item)
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

    QSharedPointer<MenuItem> findItem(ANavigationMenuItem* item)
    {
        for (const auto& subMenuItem : rootMenuItem->subItems)
        {
            auto theItem = findItem(subMenuItem, item);
            if (theItem)
                return theItem;
        }
        return nullptr;
    }

public:
    ANavigationView* q_ptr = nullptr;
    ANavigationPageView* pageView = nullptr;

    ANavigationPanel* panel = nullptr;
    QSharedPointer<MenuItem> rootMenuItem;
    ANavigationMenuItem* backButton = nullptr;
    ANavigationMenuItem* compactButton = nullptr;
    QWidget* headerWidget = nullptr;
    ANavigationMenuItemTreeView* menuItemView = nullptr;
    ANavigationMenuItemDelegate* menuItemViewDelegate = nullptr;
    QWidget* footerWidget = nullptr;
    ANavigationMenuItem* settingsButton = nullptr;

    ANavigationView::EPanelPosition panelPosition = (ANavigationView::EPanelPosition)0;
    ANavigationView::ECompactMode compactMode = (ANavigationView::ECompactMode)0;
    QString headerText;
    bool isResizable = false;
};

typedef QSharedPointer<ANavigationViewPrivate::MenuItem> TNavMenuItemPtr;

APROCH_NAMESPACE_END