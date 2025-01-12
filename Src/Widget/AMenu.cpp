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
    switch (e->type())
    {
    case QEvent::Show:
    {
        // 默认弹出的菜单由于有阴影会有偏差，所以显示的时候进行位置矫正
        const int dx = d->shadowRadius - d->shadowOffset.width();
        const int dxRight = d->shadowRadius + d->shadowOffset.width();
        const int dy = d->shadowRadius - d->shadowOffset.height();
        const int dyBottom = d->shadowRadius + d->shadowOffset.height();
        const QSize popup_size = sizeHint();
        QWidget* pw = parentWidget();

        QPoint pos;
        bool defaultPopDown = true;
        QRect adjustedActionRect;
        QMenuBar* mb = qobject_cast<QMenuBar*>(pw);
        QMenu* mm = qobject_cast<QMenu*>(pw);
        if (mb)
        {
            adjustedActionRect = mb->actionGeometry(mb->activeAction());
            defaultPopDown = !mb->isDefaultUp();
            pos = mb->mapToGlobal(QPoint(adjustedActionRect.left(), adjustedActionRect.bottom()));
        }
        else if (mm)
        {
            adjustedActionRect = mm->actionGeometry(mm->activeAction());
            pos = mm->mapToGlobal(QPoint(adjustedActionRect.right(), adjustedActionRect.top()));
        }
        else if (pw)
        {
            adjustedActionRect = pw->rect();
            //pos = pw->mapToGlobal(QPoint(adjustedActionRect.left(), adjustedActionRect.top()));
            pos = QCursor::pos();
        }
        else
        {
            adjustedActionRect = QRect(0, 0, width(), height());
            pos = mapToGlobal(QPoint(adjustedActionRect.left(), adjustedActionRect.bottom()));
        }

        // we put the popup menu on the screen containing the bottom-center of the action rect
        QScreen* menubarScreen = window()->windowHandle()->screen();
        QScreen* popupScreen = menubarScreen->virtualSiblingAt(pos + QPoint(adjustedActionRect.width() / 2, 0));
        if (!popupScreen)
            popupScreen = menubarScreen;
        QRect screenRect = popupScreen->geometry();
        QPoint originPos = pos;
        pos = QPoint(qMax(pos.x(), screenRect.x()), qMax(pos.y(), screenRect.y()));
        const bool fitDown = (pos.y() + popup_size.height() - dyBottom - dy <= screenRect.bottom());
        const bool fitRight = (pos.x() + popup_size.width() - dxRight - dx <= screenRect.right());

        if (!fitRight)
        {
            if (mm)
                pos.rx() = originPos.x() - adjustedActionRect.width() - popup_size.width() + dxRight;
            else
                pos.rx() = screenRect.right() - popup_size.width() + dxRight;
        }
        else
        {
            pos.rx() -= dx;
        }

        if (!defaultPopDown || !fitDown)
            pos.ry() = screenRect.bottom() - popup_size.height() + dyBottom;
        else
            pos.ry() -= dy;
        setGeometry(pos.x(), pos.y(), width(), height());
        
        // 有子菜单的情况增加展出箭头的间距
        if (findChildren<QMenu*>().isEmpty())
            setStyleSheet("QMenu::item{padding: 0px 10px 0px 10px;}");  // TODO: 不要写死大小
        else
            setStyleSheet("QMenu::item{padding: 0px 30px 0px 10px;}");
    }
        break;
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseMove:
    {
        QMouseEvent* me = (QMouseEvent*)e;
        const QPoint mpos = me->pos();
        if ((mpos.x() < d->shadowRadius - d->shadowOffset.width()) ||
            (mpos.x() > width() - d->shadowRadius - d->shadowOffset.width()) ||
            (mpos.y() < d->shadowRadius - d->shadowOffset.height()) ||
            (mpos.y() > height() - d->shadowRadius - d->shadowOffset.height()))
        {
            // 将事件传递给父控件
            QWidget* pw = parentWidget();
            QMenuBar* mb = qobject_cast<QMenuBar*>(pw);
            QMenu* mm = qobject_cast<QMenu*>(pw);
            if (mb || mm)
            {
                QMouseEvent newEvent(me->type(),
                                     pw->mapFromGlobal(mapToGlobal(me->pos())),
                                     me->button(),
                                     me->buttons(),
                                     me->modifiers());
                QCoreApplication::sendEvent(pw, &newEvent);

                // 阻止事件进一步传播
                return true;
            }
        }
    }
    break;
    default:
        break;
    }

    return QMenu::event(e);
}

bool AMenu::eventFilter(QObject* watched, QEvent* evt)
{
    return QMenu::eventFilter(watched, evt);
}

APROCH_NAMESPACE_END