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
#include <QCommonStyle>

APROCH_NAMESPACE_BEGIN

class AMenuStyle : public QProxyStyle
{
public:
    AMenuStyle(int size)
        : QProxyStyle()
        , iconSize(size)
    {
    }

    int pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const override
    {
        if (metric == QStyle::PM_SmallIconSize)
        {
            return iconSize;
        }

        return QCommonStyle::pixelMetric(metric, option, widget);
    }

    int iconSize;
};

void AMenuPrivate::updateContentMargins()
{
    const int l = shadowRadius - shadowOffset.width();
    const int r = shadowRadius + shadowOffset.width();
    const int t = shadowRadius - shadowOffset.height();
    const int b = shadowRadius + shadowOffset.height();
    const int h_2 = (l + r) / 2;
    const int v_2 = (t + b) / 2;
    q->setContentsMargins(l - h_2, t - v_2, r - h_2, b - v_2);
}

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

    AMenuStyle* menuStyle = new AMenuStyle(AFontIcon::DefaultIconSize.width());
    menuStyle->setParent(this);
    setStyle(menuStyle);
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

int AMenu::getIconSize() const
{
    AMenuStyle* menuStyle = findChild<AMenuStyle*>();
    if (menuStyle)
        return menuStyle->iconSize;

    return AFontIcon::DefaultIconSize.width();
}

void AMenu::setIconSize(int s)
{
    AMenuStyle* menuStyle = findChild<AMenuStyle*>();
    if (menuStyle)
        menuStyle->iconSize = s;
}

qreal AMenu::getShadowRadius() const
{
    return d->shadowRadius;
}

void AMenu::setShadowRadius(qreal r)
{
    d->shadowRadius = r;
    d->updateContentMargins();
}

QSize AMenu::getShadowOffset() const
{
    return QSize(d->shadowOffset);
}

void AMenu::setShadowOffset(const QSize& offset)
{
    d->shadowOffset = offset;
    d->updateContentMargins();
}

QColor AMenu::getShadowColor() const
{
    return d->shadowColor;
}

void AMenu::setShadowColor(const QColor& c)
{
    d->shadowColor = c;
}

QSize AMenu::getShadowTopRadius() const
{
    return d->shadowTopRadius;
}

void AMenu::setShadowTopRadius(const QSize& s)
{
    d->shadowTopRadius = s;
}

QSize AMenu::getShadowBottomRadius() const
{
    return d->shadowBottomRadius;
}

void AMenu::setShadowBottomRadius(const QSize& s)
{
    d->shadowBottomRadius = s;
}

void AMenu::paintEvent(QPaintEvent* evt)
{
    QPainter painter(this);
    SCornerF borderRadius(d->shadowTopRadius.width(), d->shadowTopRadius.height(),
                          d->shadowBottomRadius.width(), d->shadowBottomRadius.height());
    const QPointF offset(d->shadowOffset.width(), d->shadowOffset.height());
    AGraphicsToolkit::drawShadow(&painter, size(), d->shadowRadius, offset, d->shadowColor, borderRadius);

    QMenu::paintEvent(evt);
}

bool AMenu::event(QEvent* e)
{
    QMenu::event(e);

    switch (e->type())
    {
    case QEvent::Show:
    {
        // 默认弹出的菜单由于有阴影会有偏差，所以显示的时候进行位置矫正，点击时鼠标位置下就是菜单左上角的位置
        int newX = 0, newY = 0;
        if (x() < QCursor::pos().x())
            newX = x() + (d->shadowRadius + d->shadowOffset.width());
        else
            newX = x() - (d->shadowRadius - d->shadowOffset.width());
        const bool rootMenu = nullptr == qobject_cast<QMenu*>(parentWidget());    // 一级菜单
        if (y() < QCursor::pos().y() && rootMenu)
            newY = y() + (d->shadowRadius + d->shadowOffset.height());
        else if (rootMenu)
            newY = y() - (d->shadowRadius - d->shadowOffset.height());
        else 
            newY = y();
        setGeometry(newX, newY, width(), height());

        // 有子菜单的情况增加展出箭头的间距
        if (findChildren<QMenu*>().isEmpty())
            setStyleSheet("QMenu::item{padding: 0px 10px 0px 10px;}");  // TODO: 不要写死大小
        else
            setStyleSheet("QMenu::item{padding: 0px 30px 0px 10px;}");
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