/****************************************************************************
 * @file    AMenu_p.h
 * @date    2024-10-05 
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
#include <QtWidgets/private/qwidget_p.h>
#include <QtWidgets/private/qmenu_p.h>
#include <qpa/qplatformmenu.h>

class QEventLoop;

APROCH_NAMESPACE_BEGIN

class AMenu;

class AMenuPrivate
{
public:
    AMenuPrivate(AMenu* q_ptr) 
        : q(q_ptr)
        , itemsDirty(false)
    {
    }

    ~AMenuPrivate()
    {
        delete scroll;
    }

    void init();
    void updateActionRects() const;
    void updateActionRects(const QRect& screen) const;
    QRect popupGeometry(QScreen* screen = nullptr) const;
    bool useFullScreenForPopup() const;
    int getLastVisibleAction() const;
    bool isContextMenu() const;

    //scrolling support
    struct AMenuScroller
    {
        enum ScrollLocation
        {
            ScrollStay, ScrollBottom, ScrollTop, ScrollCenter
        };
        enum ScrollDirection
        {
            ScrollNone = 0, ScrollUp = 0x01, ScrollDown = 0x02
        };
        int scrollOffset = 0;
        QBasicTimer scrollTimer;
        quint8 scrollFlags = ScrollNone;
        quint8 scrollDirection = ScrollNone;

        AMenuScroller()
        {
        }
        ~AMenuScroller()
        {
        }
    } *scroll = nullptr;
    int scrollerHeight() const;

    class ScrollerTearOffItem : public QWidget
    {
    public:
        enum Type
        {
            ScrollUp, ScrollDown
        };
        ScrollerTearOffItem(Type type, AMenuPrivate* mPrivate,
                            QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
        void paintEvent(QPaintEvent* e) override;
        void updateScrollerRects(const QRect& rect);

    private:
        AMenuPrivate* menuPrivate;
        Type scrollType;
    };
    void drawScroller(QPainter* painter, ScrollerTearOffItem::Type type, const QRect& rect);

    AMenu* q = nullptr;
    QWidget* rframe = nullptr;
    mutable QList<QRect> actionRects;
    mutable QHash<QAction*, QWidget*> widgetItems;
    ScrollerTearOffItem* scrollUpTearOffItemEx = nullptr;
    ScrollerTearOffItem* scrollDownItemEx = nullptr;
    mutable bool itemsDirty : 1;
};

APROCH_NAMESPACE_END