/****************************************************************************
 * @file    ANavigationPanel_p.h
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
#include "Widget/Private/ANavigationPanel.h"

APROCH_NAMESPACE_BEGIN

class ANavigationViewItemBase;
class ANavigationMenuItemTreeView;
class ATrackBar;

class ANavigationMenuItemDelegatePrivate
{
public:
};

class ANavigationMenuItemTreeViewPrivate
{
public:
    ANavigationMenuItemTreeViewPrivate()
    {
    }

    QTreeWidgetItem* findMenuItem(const ANavigationMenuItemTreeView* view,
                                  QTreeWidgetItem* theItem,
                                  ANavigationViewItemBase* itemBase)
    {
        if (view->itemWidget(theItem, 0) == itemBase)
            return theItem;
        for (int i = 0; i < theItem->childCount(); ++i)
        {
            auto subItem = findMenuItem(view, theItem->child(i), itemBase);
            if (subItem)
                return subItem;
        }
        return nullptr;
    }

    void getMenuItemMap(const ANavigationMenuItemTreeView* view, QTreeWidgetItem* parentItem,
                        QMap<QTreeWidgetItem*, ANavigationViewItemBase*>& itemBaseMap)
    {
        if (!parentItem)
            return;

        auto itemBase = qobject_cast<ANavigationViewItemBase*>(view->itemWidget(parentItem, 0));
        if (itemBase)
            itemBaseMap[parentItem] = itemBase;

        for (int i = 0; i < parentItem->childCount(); ++i)
            getMenuItemMap(view, parentItem->child(i), itemBaseMap);
    }

    ATrackBar* trackBar = nullptr;
    ATrackBar* nextTrackBar = nullptr;
    bool trackbarVisibleRestore = false;
};

class ANavigationMenuItemModelPrivate
{
public:
    ANavigationMenuItemModelPrivate()
    {
    }
public:
};

class ANavigationPanelPrivate
{
public:
    ANavigationView::EPanelPosition position = (ANavigationView::EPanelPosition)0;
};

APROCH_NAMESPACE_END