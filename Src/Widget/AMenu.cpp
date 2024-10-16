/****************************************************************************
 * @file    AMenu.cpp
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
#include "stdafx.h"
#include "AMenu.h"
#include "Private/AMenu_p.h"

#include <QtWidgets/private/qwidget_p.h>
#include <QtWidgets/private/qtwidgetsglobal_p.h>
#include <QtWidgets/private/qwidgetwindow_p.h>
#include <QWidgetAction>
#include <private/qguiapplication_p.h>
#include <qpa/qplatformtheme.h>

APROCH_NAMESPACE_BEGIN

AMenu::AMenu(QWidget* parent)
    : AMenu(QString(), parent)
{
}

AMenu::AMenu(const QString& title, QWidget* parent)
    : QMenu(title, parent)
    , d(new AMenuPrivate(this))
{
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowFlag(Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_StyledBackground);
    setAutoFillBackground(false);

    d->shadow = new QGraphicsDropShadowEffect(this);
    d->shadow->setBlurRadius(30);
    d->shadow->setOffset(0, 6);
    d->shadow->setColor(QColor(0, 0, 0, 80));
    //setGraphicsEffect(d->shadow);

    setShadowHMargin(QSize(14, 14));
    setShadowVMargin(QSize(2, 26));
}

AMenu::~AMenu()
{
}

AMenu* AMenu::addMenu()
{
    return addMenu(QString());
}

AMenu* AMenu::addMenu(const QString& title)
{
    AMenu* newMenu = new AMenu(title, this);
    QAction* menuAction = QMenu::addMenu(newMenu);
    return menuAction->menu<AMenu*>();
}

qreal AMenu::getShadowRadius() const
{
    return d->shadow->blurRadius();
}

void AMenu::setShadowRadius(qreal r)
{
    d->shadow->setBlurRadius(r);
}

QSize AMenu::getShadowOffset() const
{
    QPointF o = d->shadow->offset();
    return QSize(o.x(), o.y());
}

void AMenu::setShadowOffset(const QSize& offset)
{
    d->shadow->setOffset(offset.width(), offset.height());
}

QColor AMenu::getShadowColor() const
{
    return d->shadow->color();
}

void AMenu::setShadowColor(const QColor& c)
{
    d->shadow->setColor(c);
}

QSize AMenu::getShadowHMargin() const
{
    return QSize(d->shadowMargin.left(), d->shadowMargin.right());
}

void AMenu::setShadowHMargin(const QSize& s)
{
    d->shadowMargin.setLeft(s.width());
    d->shadowMargin.setRight(s.height());

    const QMargins cm = contentsMargins();
    const int h_2 = (s.width() + s.height()) / 2;
    setContentsMargins(s.width() - h_2, cm.top(), s.height() - h_2, cm.bottom());
}

QSize AMenu::getShadowVMargin() const
{
    return QSize(d->shadowMargin.top(), d->shadowMargin.bottom());
}

void AMenu::setShadowVMargin(const QSize& s)
{
    d->shadowMargin.setTop(s.width());
    d->shadowMargin.setBottom(s.height());

    const QMargins cm = contentsMargins();
    const int v_2 = (s.width() + s.height()) / 2;
    setContentsMargins(cm.left(), s.width() - v_2, cm.right(), s.height() - v_2);
}

void AMenu::paintEvent(QPaintEvent* e)
{
    return QMenu::paintEvent(e);
}

bool AMenu::event(QEvent* e)
{
    QMenu::event(e);

    switch (e->type())
    {
#ifdef Q_OS_WIN
    /*case QEvent::WinIdChange:
    {
        static bool class_amended = false;
        HWND hwnd = reinterpret_cast<HWND>(winId());
        if (class_amended == false)
        {
            class_amended = true;
            DWORD class_style = ::GetClassLong(hwnd, GCL_STYLE);
            class_style &= ~CS_DROPSHADOW;
            ::SetClassLong(hwnd, GCL_STYLE, class_style);
        }
    }
    break;*/
#endif
    case QEvent::Show:
    {
        setGeometry(x() - d->shadowMargin.left(), y() - d->shadowMargin.top(), width(), height());
    }
    break;
    default:
        break;
    }

    return false;
}

bool AMenu::eventFilter(QObject* watched, QEvent* evt)
{
    return QMenu::eventFilter(watched, evt);
}

APROCH_NAMESPACE_END
