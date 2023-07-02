/****************************************************************************
 * @file    AOfficePopupMenu.cpp
 * @date    2023-07-02 
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
#include <QApplication>
#include <QStyleOption>
#include <QWidgetAction>
#include <QPainter>
#include <qevent.h>
#include <QLayout>

#include "Widget/Style/ACommonStyle_p.h"
#include "Widget/Style/AStyleHelper.h"
#include "AOfficePopupMenu.h"
#include "ARibbonGallery.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

//static const int popupMenuGripHeight = 12; // height of the MenuGrip

/*!
\property AOfficePopupMenu::gripVisible
*/

class AOfficePopupMenuPrivate : public QObject
{
public:
    A_DECLARE_PUBLIC(AOfficePopupMenu)
public:
    explicit AOfficePopupMenuPrivate();
public:
    void init();
    int calcMinimumHeight(bool without = false) const;
    QWidget* findWidget(const char* nameWidget) const;
    void updateSizeGallery();
    void setTransparentWidgetForMouseEvents(bool transparent);
    void setDragCursor();
    void unsetDragCursor();
public:
    QWidget* m_widgetPopup; 
    bool m_resizable;
    bool m_pressSizeGrip;
    bool m_showGrip;
    bool m_hasCursor;
    int m_offsetHeight;
    int m_minimumHeight;
    int m_minimumWidth;
    int m_lastWidth;
};

AOfficePopupMenuPrivate::AOfficePopupMenuPrivate()
    : m_widgetPopup(nullptr)
    , m_resizable(false)
    , m_pressSizeGrip(false)
    , m_showGrip(true)
    , m_hasCursor(false)
    , m_offsetHeight(0)
    , m_minimumHeight(-1)
    , m_minimumWidth(-1)
    , m_lastWidth(-1)
{
}

void AOfficePopupMenuPrivate::init()
{
    A_P(AOfficePopupMenu);
    p.setProperty(_aproch_PopupBar, true);
    p.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    p.setMouseTracking(true);
    p.setContentsMargins(QMargins(1, 1, 1, m_resizable ?
        p.style()->pixelMetric(static_cast<QStyle::PixelMetric>(ACommonStyle::PM_RibbonPopupMenuGripHeight), nullptr, &p) + 2 : 1));
}

int AOfficePopupMenuPrivate::calcMinimumHeight(bool without) const
{
    A_P(const AOfficePopupMenu);
    int height = p.style()->pixelMetric(static_cast<QStyle::PixelMetric>(ACommonStyle::PM_RibbonPopupMenuGripHeight), nullptr, &p);
    QList<QAction*> listActions = p.actions();
    for (int i = 0; i < listActions.count(); i++) 
    {
        if (QWidgetAction* widgetAction = qobject_cast<QWidgetAction*>(listActions.at(i)))
        {
            if (QWidget* w = widgetAction->defaultWidget())
            {
                if (!without)
                {
                    int minHeight = w->minimumSizeHint().height();
                    if (minHeight == -1)
                        minHeight = w->sizeHint().height();
                    height += minHeight;
                }
            }
        }
        else
            height += p.actionGeometry(listActions.at(i)).height();
    }
    return height;
}

QWidget* AOfficePopupMenuPrivate::findWidget(const char* nameWidget) const
{
    A_P(const AOfficePopupMenu);
    const QObjectList& listChildren = p.children();
    for (int i = 0; i < listChildren.size(); ++i) 
    {
        QWidget* w = qobject_cast<QWidget*>(listChildren.at(i));
        if (w && w->property(nameWidget).toBool() )
            return w;
    }
    return nullptr;
}

void AOfficePopupMenuPrivate::setTransparentWidgetForMouseEvents(bool transparent)
{
    A_P(AOfficePopupMenu);
    const QObjectList& listChildren = p.children();
    for (int i = 0; i < listChildren.size(); ++i)
    {
        if (QWidget* widget = qobject_cast<QWidget*>(listChildren.at(i)))
        {
            if (!widget->isWindow() && !widget->isHidden())
                widget->setAttribute(Qt::WA_TransparentForMouseEvents, transparent);
        }
    }
}

void AOfficePopupMenuPrivate::setDragCursor()
{
    if (m_hasCursor)
        return;
    m_hasCursor = true;
    QApplication::setOverrideCursor(Qt::SizeFDiagCursor);
}

void AOfficePopupMenuPrivate::unsetDragCursor()
{
    if (!m_hasCursor)
        return;
    QApplication::restoreOverrideCursor();
    m_hasCursor = false;
}

void AOfficePopupMenuPrivate::updateSizeGallery()
{
    A_P(const AOfficePopupMenu);
    const QObjectList& listChildren = p.children();
    for (QObjectList::const_iterator it = listChildren.begin(); it != listChildren.end(); ++it)
    {
        if (QWidget* w = qobject_cast<QWidget*>(*it))
        {
            if (!w->isWindow() && !w->isHidden() && w->property("isResizable").toBool())
            {
                QSize sizeWidget = w->geometry().size();
                sizeWidget.setWidth(sizeWidget.width());
                sizeWidget.setHeight(sizeWidget.height() - m_offsetHeight);
                w->resize(sizeWidget);
                if (ARibbonGallery* gallery = qobject_cast<ARibbonGallery*>(w))
                    gallery->updatelayout();
            }
        }
    }
}

/*!
class AOfficePopupMenu
\brief Constructs the AOfficePopupMenu object with the given \a parent.
*/ 
AOfficePopupMenu::AOfficePopupMenu(QWidget* parent)
    : QMenu(parent)
{
    A_INIT_PRIVATE(AOfficePopupMenu);
    A_D(AOfficePopupMenu);
    d.init();
}


/*!
\brief Destructor of the AOfficePopupMenu object.
*/ 
AOfficePopupMenu::~AOfficePopupMenu()
{
    A_DELETE_PRIVATE();
}

/*!
\brief Creates an instance of the AOfficePopupMenu object with given \a parent.
*/ 
AOfficePopupMenu* AOfficePopupMenu::createPopupMenu(QWidget* parent)
{
    AOfficePopupMenu* popupBar = new AOfficePopupMenu(parent);
    return popupBar;
}

/*!
\brief Adds \a widget to the Office Popup Window and return an action associated with them.
*/ 
QAction* AOfficePopupMenu::addWidget(QWidget* widget)
{
    A_D(AOfficePopupMenu);
    QWidgetAction* action = new QWidgetAction(this);
    action->setDefaultWidget(widget);
    addAction(action);
    d.m_resizable = widget && widget->property("isResizable").toBool();
    setContentsMargins(QMargins(1, 1, 1, d.m_resizable ?
        style()->pixelMetric(static_cast<QStyle::PixelMetric>(ACommonStyle::PM_RibbonPopupMenuGripHeight), nullptr, this) + 2 : 1));
    return action;
}

/*!
\brief Sets visibility of the Popup Window Grip to \a visible.
*/ 
void AOfficePopupMenu::setGripVisible(bool visible)
{
    A_D(AOfficePopupMenu);
    d.m_showGrip = visible;
}

/*!
\brief Returns the visibility of the Popup Window Grip.
*/ 
bool AOfficePopupMenu::isGripVisible() const
{
    A_D(const AOfficePopupMenu);
    return d.m_showGrip;
}

/*! \internal */
void AOfficePopupMenu::setWidgetBar(QWidget* widget)
{
    A_D(AOfficePopupMenu);
    Q_ASSERT(widget != nullptr && d.m_widgetPopup == nullptr);
    d.m_widgetPopup = widget;
}

/*! \reimp */
bool AOfficePopupMenu::event(QEvent* event)
{
    A_D(AOfficePopupMenu);
    if (event->type() == QEvent::ChildAdded && d.m_resizable)
    {
        QSize sz = sizeHint();
        if (sz.isValid()) 
        {
            QSize minSz = minimumSize();
            if (minSz.height() > sz.height())
                setMinimumHeight(minSz.height() - (minSz.height() - sz.height()));
        }
    }

    bool ok = QMenu::event(event);

    if (event->type() == QEvent::Resize)
    {
        QList<QAction*> listActions = actions();
        for (int i = 0; i < listActions.count(); i++)
        {
            if (QWidgetAction* action = qobject_cast<QWidgetAction*>(listActions.at(i)))
            {
                QRect rect = actionGeometry(action);
                if (QWidget* wd = action->defaultWidget())
                    wd->resize(QSize(width() - 3, rect.height()));
            }
        }
    }
    return ok;
}

/*! \reimp */
void AOfficePopupMenu::paintEvent(QPaintEvent* event)
{
    A_D(AOfficePopupMenu);
    QMenu::paintEvent(event);
    if (d.m_resizable)
    {
        QPainter p(this);
        QStyleOptionSizeGrip opt;
        opt.initFrom(this);
        opt.rect.adjust(1, 0, -1, -1);
        opt.rect.setTop(opt.rect.bottom() - style()->pixelMetric(static_cast<QStyle::PixelMetric>(ACommonStyle::PM_RibbonPopupMenuGripHeight), nullptr, this));
        style()->drawControl((QStyle::ControlElement)ACommonStyle::CE_RibbonGripBar, &opt, &p, this);
    }
}

/*! \reimp */
void AOfficePopupMenu::mousePressEvent(QMouseEvent* event)
{
    A_D(AOfficePopupMenu);
    if (d.m_resizable)
    {
        if (event->buttons() == Qt::LeftButton) 
        {
            QRect rcResizeGripper(rect());
            rcResizeGripper.setTop(rcResizeGripper.bottom() - style()->pixelMetric(static_cast<QStyle::PixelMetric>(ACommonStyle::PM_RibbonPopupMenuGripHeight), nullptr, this));
            QRect rcResizeGripperAll = rcResizeGripper;
            rcResizeGripper.setLeft(rcResizeGripper.right() - rcResizeGripper.height());
            if (rcResizeGripper.contains(event->pos()))
            {
                d.setTransparentWidgetForMouseEvents(true);
                d.m_pressSizeGrip = true;
                d.m_offsetHeight = 0;
                return;
            }
            else if (rcResizeGripperAll.contains(event->pos()))
                return;
        }
    }
    QMenu::mousePressEvent(event);
}

/*! \reimp */
void AOfficePopupMenu::mouseMoveEvent(QMouseEvent* event)
{
    A_D(AOfficePopupMenu);
    if (d.m_resizable)
    {
        QRect rcResizeGripper(rect());
        rcResizeGripper.setTop(rcResizeGripper.bottom() - style()->pixelMetric(static_cast<QStyle::PixelMetric>(ACommonStyle::PM_RibbonPopupMenuGripHeight), nullptr, this));
        rcResizeGripper.setLeft(rcResizeGripper.right() - rcResizeGripper.height());

        if (d.m_pressSizeGrip || rcResizeGripper.contains(event->pos()))
            d.setDragCursor();
        else
            d.unsetDragCursor();

        if (d.m_pressSizeGrip)
        {
            QPoint pnt(event->globalPos());
            QRect rect = geometry();

            if (pnt.x() - rect.left() > minimumWidth())
                rect.setRight(pnt.x());
            else
                rect.setWidth(minimumWidth());

            if (pnt.y() - rect.top() > minimumHeight())
                rect.setBottom(pnt.y());
            else
                rect.setHeight(minimumHeight());

            d.m_offsetHeight = height() - rect.height();
            d.updateSizeGallery();

            resize(rect.size());
            d.m_lastWidth = width();
            return;
        }
    }

    if (!d.m_pressSizeGrip)
        QMenu::mouseMoveEvent(event);
}

/*! \reimp */
void AOfficePopupMenu::mouseReleaseEvent(QMouseEvent* event)
{
    A_D(AOfficePopupMenu);
    d.m_pressSizeGrip = false;
    d.m_offsetHeight = 0;
    d.setTransparentWidgetForMouseEvents(false);
    QMenu::mouseReleaseEvent(event);
}

/*! \reimp */
void AOfficePopupMenu::moveEvent(QMoveEvent* event)
{
    Q_UNUSED(event);
    A_D(AOfficePopupMenu);
    if (d.m_pressSizeGrip)
        setCursor(Qt::SizeFDiagCursor);
}

/*! \reimp */
void AOfficePopupMenu::showEvent(QShowEvent* event)
{
    A_D(AOfficePopupMenu);
    QMenu::showEvent(event);
    d.m_offsetHeight = 0;
    if (d.m_widgetPopup && d.m_resizable)
    {
        d.m_minimumWidth = d.m_widgetPopup->width() + 4;
        d.m_minimumHeight = d.calcMinimumHeight();
    }

    if (d.m_widgetPopup)
    {
        setMinimumHeight(d.m_minimumHeight);
        setMinimumWidth(d.m_minimumWidth);
    }
}

void AOfficePopupMenu::hideEvent(QHideEvent* event)
{
    A_D(AOfficePopupMenu);
    d.unsetDragCursor();
    QMenu::hideEvent(event);
}

APROCH_NAMESPACE_END