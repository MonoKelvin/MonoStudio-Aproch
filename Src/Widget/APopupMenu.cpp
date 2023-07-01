/****************************************************************************
 * @file    APopupMenu.cpp
 * @date    2023-07-01
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
#include "APopupMenu.h"
#include <QApplication>
#include <QStyleOption>
#include <QWidgetAction>
#include <QMouseEvent>
#include <QMenuBar>

#include "Style/ACommonStyle_p.h"
#include "APopupMenu_p.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

static const int splitActionPopupWidth = 20; // Split button drop down width in popups
#define aproch_PopupMenuIgnorePaint "PopupMenuIgnorePaint"

/* APopupMenuView */
APopupMenuView::APopupMenuView(QWidget* parent, APopupMenu* menu)
    : QWidget(parent)
    , m_menu(menu)
{
    setMouseTracking(true);
}

APopupMenuBar* APopupMenuView::scrollMenuBar() const
{
    return qobject_cast<APopupMenuBar*>(parentWidget());
}

/*! \reimp */
void APopupMenuView::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
#if 0
    QRegion emptyArea = QRegion(rect());

    QStyleOptionMenuItem menuOpt;
    menuOpt.initFrom(this);
    menuOpt.state = QStyle::State_None;
    menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
    menuOpt.maxIconWidth = 0;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    menuOpt.tabWidth = 0;
#endif
    style()->drawPrimitive(QStyle::PE_PanelMenu, &menuOpt, &p, m_menu);
#endif

    QRect rc = this->geometry();
    QPoint offset = m_menu->isMultiColumn() ? QPoint(rc.left() - scrollMenuBar()->scrollOffset(), 0) :
        QPoint(0, rc.top() - scrollMenuBar()->scrollOffset());

    QList<QAction*> actionList = m_menu->actions();
    for (QList<QAction*>::iterator it = actionList.begin(); it != actionList.end(); ++it)
    {
        QAction* action = (*it);
        if (action->property(aproch_PopupMenuIgnorePaint).toBool())
            continue;
        QRect adjustedActionRect = m_menu->actionGeometry(action);
        adjustedActionRect.moveTo(offset + this->mapFrom(m_menu, adjustedActionRect.topLeft()));
        if (!event->rect().intersects(adjustedActionRect))
            continue;
#if 0
        emptyArea -= QRegion(adjustedActionRect);
#endif
        AStyleOptionMenuScrollItem opt;
        m_menu->initStyleOption(&opt, action);
        opt.rect = adjustedActionRect;
        opt.toolTip = action->toolTip();

        if (action->property(_aproch_SplitActionPopup).toBool())
        {
            opt.text = QStringLiteral(_aproch_SplitActionPopup) + opt.text;
            QPoint pnt = mapFromGlobal(QCursor::pos());
            if (opt.rect.adjusted(0, 0, -splitActionPopupWidth, 0).contains(pnt))
                opt.checkType = QStyleOptionMenuItem::Exclusive;
        }

        QRegion adjustedActionReg(adjustedActionRect);
        p.setClipRegion(adjustedActionReg);
        m_menu->paintMenuItem(&p, &opt);
    }

    const int fw = style()->pixelMetric(QStyle::PM_MenuPanelWidth, nullptr, m_menu);
    // draw border
    if (fw)
    {
        QRegion borderReg;
        borderReg += QRect(0, 0, fw, height()); //left
        borderReg += QRect(width() - fw, 0, fw, height()); //right
        borderReg += QRect(0, 0, width(), fw); //top
        borderReg += QRect(0, height() - fw, width(), fw); //bottom
        p.setClipRegion(borderReg);
#if 0
        emptyArea -= borderReg;
#endif
        QStyleOptionFrame frame;
        frame.rect = rect();
        frame.palette = palette();
        frame.state = QStyle::State_None;
        frame.lineWidth = style()->pixelMetric(QStyle::PM_MenuPanelWidth, &frame);
        frame.midLineWidth = 0;
        style()->drawPrimitive(QStyle::PE_FrameMenu, &frame, &p, m_menu);
    }
}

class MenuRectHack
{
public:
    MenuRectHack(APopupMenu* menu) : m_menu(menu)
    {
        if (!m_menu->isMultiColumn())
            return;
        QWidgetData* data = qt_qwidget_data(m_menu);
        APopupMenuPrivate* menuPrivate = APopupMenuPrivate::_get(m_menu);
        m_old = data->crect;
        data->crect.setWidth(data->crect.width() - menuPrivate->scrollMenuBar()->scrollOffset());
    }
    ~MenuRectHack()
    {
        if (!m_menu->isMultiColumn())
            return;
        QWidgetData* data = qt_qwidget_data(m_menu);
        data->crect = m_old;
    }
private:
    APopupMenu* m_menu;
    QRect m_old;
};

/*! \reimp */
void APopupMenuView::mousePressEvent(QMouseEvent* event)
{
    QPoint pnt = event->pos();
    pnt = this->mapTo(m_menu, pnt);

    MenuRectHack hack(m_menu);
    QMouseEvent ev(event->type(), pnt, event->button(), event->buttons(), event->modifiers());
    m_menu->mousePressEvent(&ev);
}

/*! \reimp */
void APopupMenuView::mouseReleaseEvent(QMouseEvent* event)
{
    QPoint pnt = event->pos();
    pnt = this->mapTo(m_menu, pnt);
    QMouseEvent ev(event->type(), pnt, event->button(), event->buttons(), event->modifiers());

    MenuRectHack hack(m_menu);
    m_menu->mouseReleaseEvent(&ev);
}

/*! \reimp */
void APopupMenuView::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pnt = event->pos();
    pnt = this->mapTo(m_menu, pnt);
    QMouseEvent ev(event->type(), pnt, event->button(), event->buttons(), event->modifiers());

    MenuRectHack hack(m_menu);
    m_menu->mouseMoveEvent(&ev);
}

/*! \reimp */
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
void APopupMenuView::enterEvent(QEnterEvent* event)
#else
void APopupMenuView::enterEvent(QEvent* event)
#endif
{
    m_menu->enterEvent(event);
}

/*! \reimp */
void APopupMenuView::leaveEvent(QEvent* event)
{
    m_menu->leaveEvent(event);
}

/* APopupMenuBar */
APopupMenuBar::APopupMenuBar(APopupMenu* parent)
    : AAbstractScrollWidgetBar(parent, Qt::Vertical)
    , m_menu(parent)
    , m_scrollOffset(0)
{
    setPageIndent(0);
    setItemIndent(0);
    setMargins(QMargins(0, 0, 0, 0));
    setAmimationSettings(300, QEasingCurve::OutCubic /* QEasingCurve::OutCubic */);
    setFocusProxy(m_menu);
}

APopupMenuView* APopupMenuBar::menuViewWidget() const
{
    return qobject_cast<APopupMenuView*>(this->viewWidget());
}

QWidget* APopupMenuBar::createViewWidget()
{
    return new APopupMenuView(this, m_menu);
}

void APopupMenuBar::updateLayout()
{
    AAbstractScrollWidgetBar::updateLayout();
    APopupMenuView* view = menuViewWidget();
    QSize viewSize = view->geometry().size();
    QPoint offset = viewOffset();
    APopupMenuPrivate* menuPrivate = APopupMenuPrivate::_get(m_menu);
    QMargins m = QMargins(menuPrivate->margins().left(), menuPrivate->margins().top(),
                          menuPrivate->margins().right(), menuPrivate->margins().bottom());

    if (orientation() == Qt::Vertical)
    {
        m_scrollOffset = offset.y();
        m.setTop(m.top() + m_scrollOffset);
        m.setBottom(-viewSize.height());
    }
    else
    {
        m_scrollOffset = offset.x();
        m.setLeft(m.left() + m_scrollOffset);
        //m.setRight(-viewSize.height());
        m.setRight(-m_scrollOffset);
    }

    m_menu->setContentsMargins(m.left(), m.top(), m.right(), m.bottom());
}

void APopupMenuBar::updateItemsLayout()
{
}

int APopupMenuBar::getColumnCount() const
{
    APopupMenuPrivate* menuPrivate = APopupMenuPrivate::_get(m_menu);
    return  m_menu->isMultiColumn() ? menuPrivate->m_columns.size() : m_menu->actions().count();
}

QSize APopupMenuBar::getColumnSize(int column) const
{
    if (column >= 0 && column < getColumnCount())
    {
        APopupMenuPrivate* menuPrivate = APopupMenuPrivate::_get(m_menu);
        return m_menu->isMultiColumn() ?
            menuPrivate->m_columns[column] :
            m_menu->actionGeometry(m_menu->actions()[column]).size();
    }
    else
        return QSize();
}

void APopupMenuBar::keyPressEvent(QKeyEvent* event)
{
    Q_UNUSED(event);
    if (QAction* action = m_menu->activeAction())
    {
        int index = m_menu->actions().indexOf(action);
        if (index != -1)
            ensureVisible(index);
    }
}

/* APopupMenuPrivate */
APopupMenuPrivate::APopupMenuPrivate()
    : m_preview(nullptr)
    , m_scrollMenuBar(nullptr)
    , m_ignoreResize(false)
{
}

void APopupMenuPrivate::styleChanged()
{
    A_P(APopupMenu);
    QStyleOption opt;
    opt.initFrom(&p);
    const int fw = p.style()->pixelMetric(QStyle::PM_MenuPanelWidth, &opt, &p);
    const int hmargin = p.style()->pixelMetric(QStyle::PM_MenuHMargin, &opt, &p);
    const int vmargin = p.style()->pixelMetric(QStyle::PM_MenuVMargin, &opt, &p);
    m_scrollMenuBar->setMargins(QMargins(hmargin + fw, vmargin + fw, hmargin + fw, vmargin + fw));
    updateActionRects();
}

const QMargins& APopupMenuPrivate::margins() const
{
    return m_margins;
}

void APopupMenuPrivate::setMargins(const QMargins& margins)
{
    A_P(APopupMenu);
    if (m_margins == margins)
        return;
    m_margins = margins;

    p.setContentsMargins(m_margins);
}

class QWidgetActionEx : public QWidgetAction
{
public:
    friend class APopupMenuPrivate;
};

void APopupMenuPrivate::updateActionRects(bool force)
{
    A_P(APopupMenu);
    if (force)
    {
        if (m_ignoreResize)
            return;
        //Hack: Reset actions geometry.
        m_ignoreResize = true;
        QResizeEvent e(p.size(), p.size());
        p.event(&e);
        m_ignoreResize = false;
    }

    if (p.isMultiColumn())
    {
        m_columns.clear();
        QList<QAction*> actionList = p.actions();
        int x = 0;
        int w = 0;
        for (QList<QAction*>::iterator it = actionList.begin(); it != actionList.end(); ++it)
        {
            QAction* action = (*it);
            const QRect rect = p.actionGeometry(action);
            if (rect.isNull())
                continue;
            w = qMax(w, rect.width());
            if (m_columns.size() == 0 || rect.left() > x)
            {
                m_columns.append(QSize(w, 0));
                x = rect.left();
                w = 0;
            }
            m_columns.last().rheight() += rect.height();
        }
    }

    QList<QAction*> actionList = p.actions();
    for (QList<QAction*>::iterator it = actionList.begin(); it != actionList.end(); ++it)
    {
        QAction* action = (*it);
        if (QWidgetAction* widgetAction = qobject_cast<QWidgetAction*>(action))
        {
            QWidgetList widgets = static_cast<QWidgetActionEx*>(widgetAction)->createdWidgets();
            QWidget* w = nullptr;
            for (QWidgetList::const_iterator wit = widgets.constBegin(); wit != widgets.constEnd(); ++wit)
            {
                w = *wit;
                if (w->parentWidget() == &p)
                {
                    w->setParent(scrollMenuBar()->viewWidget());
                    widgetAction->setProperty(aproch_PopupMenuIgnorePaint, true);
                    break;
                }
                w = nullptr;
            }
            if (w != nullptr)
                continue;
        }
    }
}

/*!
\class APopupMenu
\inmodule QtitanBase
\brief The aproch::APopupMenu class provides a menu widget for use in menu bars, context menus, and other popup menus.

The aproch::APopupMenu is derived from QMenu and should be used in the same cases where regular QMenu is used.
Additionally to the QMenu features, this class implements the ability to scroll items vertically if they do not fit completely on the screen
or the number of items on the screen is limited by the property - APopupMenu::setMaxItemCount().
Moreover, it allows to display a custom QWidget on the right part from the menu items view
which can be used as a preview for the selected item or jsut for additional info for the end-users.
*/

/*!
Constructor of the class aproch::APopupMenu. The parameter \c parent is a widget which will be parent and owner for the menu.
*/
APopupMenu::APopupMenu(QWidget* parent)
    : QMenu(parent)
{
    A_INIT_PRIVATE(APopupMenu);
    A_D(APopupMenu);
    d.m_scrollMenuBar = new APopupMenuBar(this);
    d.m_scrollMenuBar->viewWidget();
    d.styleChanged();
}

APopupMenu::APopupMenu(APopupMenuPrivate& d, QWidget* parent)
    : QMenu(parent)
{
    aproch_d_ptr = &d;
    d.setPublic(this);
    d.m_scrollMenuBar = new APopupMenuBar(this);
    d.m_scrollMenuBar->viewWidget();
    d.styleChanged();
}

APopupMenu::~APopupMenu()
{
    A_DELETE_PRIVATE();
}

/*!
Returns the preview widget for the menu.
*/
QWidget* APopupMenu::previewWidget() const
{
    A_D(const APopupMenu);
    return d.m_preview;
}

/*!
Sets the \a preview widget for the menu. It will be shown at the right side from the menu items.
*/
void APopupMenu::setPreviewWidget(QWidget* preview)
{
    A_D(APopupMenu);
    if (d.m_preview != preview)
    {
        d.m_preview = preview;
        d.m_preview->setParent(this);
        d.m_preview->setFont(font());
    }
}

/*!
Sets the maximum count of items at the menu view to \a maxCount. By default the count of items is not limited, but the menu height will be limited by the screen size.
If the menu items do not fit completely at the view, then scrolling buttons will appear.
*/
void APopupMenu::setMaxItemCount(int maxCount)
{
    A_D(APopupMenu);
    d.m_scrollMenuBar->setMaxViewItemCount(maxCount);
}

/*!
Returns the maximum count of items at the menu view.
*/
int APopupMenu::maxItemCount() const
{
    A_D(const APopupMenu);
    return d.m_scrollMenuBar->maxViewItemCount();
}

/*! \reimp */
QSize APopupMenu::sizeHint() const
{
    A_D(const APopupMenu);
    const_cast<APopupMenuPrivate*>(&d)->updateActionRects();
    QSize s = d.m_scrollMenuBar->sizeHint();
    if (d.m_preview != nullptr)
        s.setWidth(s.width() + d.m_preview->sizeHint().width());

    QStyleOption opt(0);
    opt.initFrom(this);
    QSize sz(s.width() + d.m_margins.left() + d.m_margins.right(), s.height() + d.m_margins.top() + d.m_margins.bottom());
    sz = style()->sizeFromContents(QStyle::CT_Menu, &opt, sz, this);
    if (isMultiColumn() && minimumWidth() > 0 && sz.width() > minimumWidth())
        sz.rwidth() = minimumWidth();
    return sz;
}

bool APopupMenu::isMultiColumn() const
{
    A_D(const APopupMenu);
    return d.m_scrollMenuBar->orientation() == Qt::Horizontal;
}

void APopupMenu::setMultiColumn(bool multiColumn)
{
    A_D(APopupMenu);
    d.m_scrollMenuBar->setOrientation(multiColumn ? Qt::Horizontal : Qt::Vertical);
}

QRect APopupMenu::scrollMenuRect() const
{
    A_D(const APopupMenu);
    return d.m_scrollMenuBar->geometry();
}


void APopupMenu::updateLayout()
{
    A_D(APopupMenu);
    d.updateActionRects();

    QSize s = size();
    int width = s.width() - d.margins().left() - d.margins().right();
    if (d.m_preview != nullptr)
        width = d.m_scrollMenuBar->sizeHint().width();

    s.rwidth() = width;
    s.rheight() = s.height() - d.margins().top() - d.margins().bottom();

    d.m_scrollMenuBar->setGeometry(QRect(QPoint(d.margins().left(), d.margins().top()), s));
    d.m_scrollMenuBar->updateLayout();

    if (d.m_preview != nullptr)
    {
        QSize sh = d.m_preview->sizeHint();
        sh.setHeight(qMin(sh.height(), s.height()));
        QRect rc(QPoint(s.width() + d.margins().left(), d.margins().top()), sh);
        d.m_preview->setGeometry(rc);
    }
}


/*! \reimp */
void APopupMenu::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
#if 0
    QRegion emptyArea = QRegion(rect());
#endif
    QStyleOptionMenuItem menuOpt;
    menuOpt.initFrom(this);
    menuOpt.rect = rect();
    menuOpt.menuRect = rect();
    menuOpt.maxIconWidth = 0;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    menuOpt.tabWidth = 0;
#endif

    menuOpt.state = QStyle::State_None;
    menuOpt.menuItemType = QStyleOptionMenuItem::EmptyArea;
    menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
    style()->drawControl(QStyle::CE_MenuEmptyArea, &menuOpt, &p, this);

    menuOpt.menuItemType = QStyleOptionMenuItem::Normal;
    menuOpt.state = QStyle::State_None;
    menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
    style()->drawPrimitive(QStyle::PE_PanelMenu, &menuOpt, &p, this);

}

void APopupMenu::paintMenuItem(QPainter* painter, QStyleOptionMenuItem* option)
{
    style()->drawControl(QStyle::CE_MenuItem, option, painter, this);
}

/*! \reimp */
void APopupMenu::changeEvent(QEvent* event)
{
    A_D(APopupMenu);
    QMenu::changeEvent(event);
    if (event->type() == QEvent::StyleChange)
        d.styleChanged();
}

/*! \reimp */
void APopupMenu::keyPressEvent(QKeyEvent* event)
{
    A_D(APopupMenu);
    QMenu::keyPressEvent(event);
    d.m_scrollMenuBar->keyPressEvent(event);
}

/*! \reimp */
void APopupMenu::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    A_D(APopupMenu);
    QMenu::resizeEvent(event);
    if (d.m_ignoreResize)
        return;
    updateLayout();
}

bool APopupMenu::event(QEvent* event)
{
    A_D(APopupMenu);
    if (event->type() == QEvent::ContentsRectChange)
        d.updateActionRects(true);
    return QMenu::event(event);
}

APROCH_NAMESPACE_END