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

void AMenuPrivate::init()
{
    q->setWindowFlag(Qt::FramelessWindowHint);
    q->setWindowFlag(Qt::NoDropShadowWindowHint);
    q->setAttribute(Qt::WA_TranslucentBackground);
    q->setAttribute(Qt::WA_StyledBackground);

    QVBoxLayout* layout = new QVBoxLayout(q);
    layout->setContentsMargins(0, 0, 0, 0);
    q->setContentsMargins(14, 2, 14, 20);

    rframe = new QFrame(q);
    rframe->setAttribute(Qt::WA_StyledBackground);
    rframe->setAttribute(Qt::WA_TransparentForMouseEvents);
    rframe->setObjectName("background-panel");
    rframe->setContentsMargins(10, 0, 10, 0);
    layout->addWidget(rframe);

    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(34);
    effect->setOffset(0, 6);
    effect->setColor(QColor(0, 0, 0, 80));
    rframe->setGraphicsEffect(effect);
    rframe->installEventFilter(q);

    //rframe->setStyle(new AWinUIStyle());

    q->setMouseTracking(true);
    if (q->style()->styleHint(QStyle::SH_Menu_Scrollable, nullptr, q))
    {
        scroll = new AMenuPrivate::AMenuScroller;
        scroll->scrollFlags = AMenuPrivate::AMenuScroller::ScrollNone;
    }
}

void AMenuPrivate::updateActionRects() const
{
    updateActionRects(popupGeometry());
}

void AMenuPrivate::updateActionRects(const QRect& screen) const
{
    if (!itemsDirty)
        return;

    q->ensurePolished();

    //let's reinitialize the buffer
    actionRects.resize(q->actions().size());
    actionRects.fill(QRect());

    int lastVisibleAction = getLastVisibleAction();

    QMargins margins = rframe->contentsMargins();
    const int leftmargin = margins.left();
    const int rightmargin = margins.right();
    const int topmargin = margins.top();
    const int bottommargin = margins.bottom();

    QStyle* style = q->style();
    QStyleOption opt;
    opt.initFrom(rframe);
    const int hmargin = style->pixelMetric(QStyle::PM_MenuHMargin, &opt, q),
        vmargin = style->pixelMetric(QStyle::PM_MenuVMargin, &opt, q),
        icone = style->pixelMetric(QStyle::PM_SmallIconSize, &opt, q);
    const int fw = style->pixelMetric(QStyle::PM_MenuPanelWidth, &opt, q);
    const int deskFw = style->pixelMetric(QStyle::PM_MenuDesktopFrameWidth, &opt, q);
    const int base_y = vmargin + fw + topmargin + (scroll ? scroll->scrollOffset : 0);
    const int column_max_y = screen.height() - 2 * deskFw - (vmargin + bottommargin + fw);
    int max_column_width = 0;
    int y = base_y;

    // for compatibility now - will have to refactor this away
    int tabWidth = 0;
    int maxIconWidth = 0;
    int hasCheckableItems = false;
    int ncols = 1;

    const auto& actions = q->actions();
    for (int i = 0; i < actions.size(); ++i)
    {
        QAction* action = actions.at(i);
        if (action->isSeparator() || !action->isVisible() || widgetItems.contains(action))
            continue;
        //..and some members
        hasCheckableItems |= action->isCheckable();
        QIcon is = action->icon();
        if (!is.isNull())
        {
            maxIconWidth = qMax<uint>(maxIconWidth, icone + 4);
        }
    }

    //calculate size
    QFontMetrics qfm = q->fontMetrics();
    bool previousWasSeparator = true; // this is true to allow removing the leading separators
#if QT_CONFIG(shortcut)
    const bool contextMenu = isContextMenu();
#endif
    for (int i = 0; i <= lastVisibleAction; i++)
    {
        QAction* action = actions.at(i);
        const bool isSection = action->isSeparator() && (!action->text().isEmpty() || !action->icon().isNull());
        const bool isPlainSeparator = (isSection && !q->style()->styleHint(QStyle::SH_Menu_SupportsSections))
            || (action->isSeparator() && !isSection);

        if (!action->isVisible() || (true && previousWasSeparator && isPlainSeparator))
            continue; // we continue, this action will get an empty QRect

        previousWasSeparator = isPlainSeparator;

        //let the style modify the above size..
        QStyleOptionMenuItem opt;
        q->initStyleOption(&opt, action);
        const QFontMetrics& fm = opt.fontMetrics;

        QSize sz;
        if (QWidget* w = widgetItems.value(action))
        {
            sz = w->sizeHint().expandedTo(w->minimumSize()).expandedTo(w->minimumSizeHint()).boundedTo(w->maximumSize());
        }
        else
        {
            //calc what I think the size is..
            if (action->isSeparator())
            {
                sz = QSize(2, 2);
            }
            else
            {
                QString s = action->text();
                qsizetype t = s.indexOf(u'\t');
                if (t != -1)
                {
                    tabWidth = qMax(int(tabWidth), qfm.horizontalAdvance(s.mid(t + 1)));
                    s = s.left(t);
#if QT_CONFIG(shortcut)
                }
                else if (action->isShortcutVisibleInContextMenu() || !contextMenu)
                {
                    QKeySequence seq = action->shortcut();
                    if (!seq.isEmpty())
                        tabWidth = qMax(int(tabWidth), qfm.horizontalAdvance(seq.toString(QKeySequence::NativeText)));
#endif
                }
                sz.setWidth(fm.boundingRect(QRect(), Qt::TextSingleLine | Qt::TextShowMnemonic, s).width());
                sz.setHeight(qMax(fm.height(), qfm.height()));

                QIcon is = action->icon();
                if (!is.isNull())
                {
                    QSize is_sz = QSize(icone, icone);
                    if (is_sz.height() > sz.height())
                        sz.setHeight(is_sz.height());
                }
            }
            sz = style->sizeFromContents(QStyle::CT_MenuItem, &opt, sz, q);
        }


        if (!sz.isEmpty())
        {
            max_column_width = qMax(max_column_width, sz.width());
            //wrapping
            if (!scroll && y + sz.height() > column_max_y)
            {
                ncols++;
                y = base_y;
            }
            else
            {
                y += sz.height();
            }
            //update the item
            actionRects[i] = QRect(0, 0, sz.width(), sz.height());
        }
    }

    max_column_width += tabWidth; //finally add in the tab width
    if (scroll)
    { // exclude non-scrollable tear-off menu since the tear-off menu has a fixed size
        const int sfcMargin = style->sizeFromContents(QStyle::CT_Menu, &opt, QSize(0, 0), q).width();
        const int min_column_width = q->minimumWidth() - (sfcMargin + leftmargin + rightmargin + 2 * (fw + hmargin));
        max_column_width = qMax(min_column_width, max_column_width);
    }

    //calculate position
    int x = hmargin + fw + leftmargin;
    y = base_y;

    for (int i = 0; i < actions.size(); i++)
    {
        QRect& rect = actionRects[i];
        if (rect.isNull())
            continue;
        if (!scroll && y + rect.height() > column_max_y)
        {
            x += max_column_width + hmargin;
            y = base_y;
        }
        rect.translate(x, y);                        //move
        rect.setWidth(max_column_width); //uniform width

        //we need to update the widgets geometry
        if (QWidget* widget = widgetItems.value(actions.at(i)))
        {
            widget->setGeometry(rect);
            widget->setVisible(actions.at(i)->isVisible());
        }

        y += rect.height();
    }
    itemsDirty = 0;
}

QRect AMenuPrivate::popupGeometry(QScreen* screen) const
{
    if (useFullScreenForPopup())
        return screen ? screen->geometry() : QWidgetPrivate::screenGeometry(q);
    return screen ? screen->availableGeometry() : QWidgetPrivate::availableScreenGeometry(q);
}

bool AMenuPrivate::useFullScreenForPopup() const
{
    auto theme = QGuiApplicationPrivate::platformTheme();
    return theme && theme->themeHint(QPlatformTheme::UseFullScreenForPopupMenu).toBool();
}

int AMenuPrivate::getLastVisibleAction() const
{
    //let's try to get the last visible action
    int lastVisibleAction = q->actions().size() - 1;
    for (; lastVisibleAction >= 0; --lastVisibleAction)
    {
        const QAction* action = q->actions().at(lastVisibleAction);
        if (action->isVisible())
        {
            //removing trailing separators
            if (action->isSeparator())
                continue;
            break;
        }
    }
    return lastVisibleAction;
}

bool AMenuPrivate::isContextMenu() const
{
    // TODO
    return true;
}

int AMenuPrivate::scrollerHeight() const
{
    return q->style()->pixelMetric(QStyle::PM_MenuScrollerHeight, nullptr, q);
}

void AMenuPrivate::drawScroller(QPainter* painter, AMenuPrivate::ScrollerTearOffItem::Type type, const QRect& rect)
{
    if (!painter || rect.isEmpty())
        return;

    if (!scroll || !(scroll->scrollFlags & (AMenuPrivate::AMenuScroller::ScrollUp
        | AMenuPrivate::AMenuScroller::ScrollDown)))
        return;

    QStyleOptionMenuItem menuOpt;
    menuOpt.initFrom(q);
    menuOpt.state = QStyle::State_None;
    menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
    menuOpt.maxIconWidth = 0;
    menuOpt.reservedShortcutWidth = 0;
    menuOpt.rect = rect;
    menuOpt.menuItemType = QStyleOptionMenuItem::Scroller;
    menuOpt.state |= QStyle::State_Enabled;
    if (type == AMenuPrivate::ScrollerTearOffItem::ScrollDown)
        menuOpt.state |= QStyle::State_DownArrow;

    painter->setClipRect(menuOpt.rect);
    q->style()->drawControl(QStyle::CE_MenuScroller, &menuOpt, painter, q);
}

AMenuPrivate::ScrollerTearOffItem::ScrollerTearOffItem(AMenuPrivate::ScrollerTearOffItem::Type type, AMenuPrivate* mPrivate, QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f), menuPrivate(mPrivate), scrollType(type)
{
    if (parent)
        setMouseTracking(parent->style()->styleHint(QStyle::SH_Menu_MouseTracking, nullptr, parent));
}

void AMenuPrivate::ScrollerTearOffItem::paintEvent(QPaintEvent* e)
{
    if (!e->rect().intersects(rect()))
        return;

    QPainter p(this);
    QWidget* parent = parentWidget();

    //paint scroll up / down arrows
    menuPrivate->drawScroller(&p, scrollType, QRect(0, 0, width(), menuPrivate->scrollerHeight()));
    //paint the tear off
    if (scrollType == AMenuPrivate::ScrollerTearOffItem::ScrollUp)
    {
        QRect rect(0, 0, width(), parent->style()->pixelMetric(QStyle::PM_MenuTearoffHeight, nullptr, parent));
        if (menuPrivate->scroll && menuPrivate->scroll->scrollFlags & AMenuPrivate::AMenuScroller::ScrollUp)
            rect.translate(0, menuPrivate->scrollerHeight());
    }
}

void AMenuPrivate::ScrollerTearOffItem::updateScrollerRects(const QRect& rect)
{
    if (rect.isEmpty())
        setVisible(false);
    else
    {
        setGeometry(rect);
        raise();
        setVisible(true);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

AMenu::AMenu(QWidget* parent)
    : AMenu(QString(), parent)
{
}

AMenu::AMenu(const QString& title, QWidget* parent)
    : QMenu(title, parent)
    , d(new AMenuPrivate(this))
{
    d->init();
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

void AMenu::paintEvent(QPaintEvent* e)
{
    // draw nothing
    return;
}

bool AMenu::event(QEvent* e)
{
    switch (e->type())
    {
#ifdef Q_OS_WIN
    case QEvent::WinIdChange:
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
    break;
#endif
    case QEvent::Show:
    {
        // sub menu
        if (qobject_cast<QMenu*>(parent()) != nullptr)
            setGeometry(pos().x() + 14, pos().y() + 2, width() + 14, height());
        else
            setGeometry(pos().x() - 14, pos().y() - 2, width() + 14, height());
    }
        break;
    default:
        break;
    }
    return QMenu::event(e);
}

bool AMenu::eventFilter(QObject* watched, QEvent* evt)
{
    if (watched == d->rframe)
    {
        switch (evt->type())
        {
        case QEvent::Paint:
        {
            auto e = (QPaintEvent*)evt;
            d->updateActionRects();
            QStylePainter p(d->rframe);
            QRegion emptyArea = QRegion(d->rframe->rect());

            QStyleOptionMenuItem menuOpt;
            menuOpt.initFrom(d->rframe);
            menuOpt.state = QStyle::State_None;
            menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
            menuOpt.maxIconWidth = 0;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            menuOpt.reservedShortcutWidth = 0;
#endif
            p.drawPrimitive(QStyle::PE_PanelMenu, menuOpt);

            //calculate the scroll up / down rect
            const int fw = style()->pixelMetric(QStyle::PM_MenuPanelWidth, nullptr, this);
            const int hmargin = style()->pixelMetric(QStyle::PM_MenuHMargin, nullptr, this);
            const int vmargin = style()->pixelMetric(QStyle::PM_MenuVMargin, nullptr, this);

            QMargins margins = d->rframe->contentsMargins();

            QRect scrollUpRect, scrollDownRect;
            const int leftmargin = fw + hmargin + margins.left();
            const int topmargin = fw + vmargin + margins.top();
            const int bottommargin = fw + vmargin + margins.bottom();
            const int contentWidth = d->rframe->width() - (fw + hmargin) * 2 - margins.left() - margins.right();
            if (d->scroll)
            {
                if (d->scroll->scrollFlags & AMenuPrivate::AMenuScroller::ScrollUp)
                    scrollUpRect.setRect(leftmargin, topmargin, contentWidth, d->scrollerHeight());

                if (d->scroll->scrollFlags & AMenuPrivate::AMenuScroller::ScrollDown)
                    scrollDownRect.setRect(leftmargin, d->rframe->height() - d->scrollerHeight() - bottommargin,
                                           contentWidth, d->scrollerHeight());
            }

            //draw the items that need updating..
            QRect scrollUpTearOffRect = scrollUpRect;
            for (int i = 0; i < actions().size(); ++i)
            {
                QAction* action = actions().at(i);
                QRect actionRect = d->actionRects.at(i);
                if (!e->rect().intersects(actionRect)
                    || d->widgetItems.value(action))
                    continue;
                //set the clip region to be extra safe (and adjust for the scrollers)
                emptyArea -= QRegion(actionRect);

                QRect adjustedActionRect = actionRect;
                if (!scrollUpTearOffRect.isEmpty() && adjustedActionRect.bottom() <= scrollUpTearOffRect.top())
                    continue;

                if (!scrollDownRect.isEmpty() && adjustedActionRect.top() >= scrollDownRect.bottom())
                    continue;

                if (adjustedActionRect.intersects(scrollUpTearOffRect))
                {
                    if (adjustedActionRect.bottom() <= scrollUpTearOffRect.bottom())
                        continue;
                    else
                        adjustedActionRect.setTop(scrollUpTearOffRect.bottom() + 1);
                }

                if (adjustedActionRect.intersects(scrollDownRect))
                {
                    if (adjustedActionRect.top() >= scrollDownRect.top())
                        continue;
                    else
                        adjustedActionRect.setBottom(scrollDownRect.top() - 1);
                }

                QRegion adjustedActionReg(adjustedActionRect);
                p.setClipRegion(adjustedActionReg);

                QStyleOptionMenuItem opt;
                initStyleOption(&opt, action);
                opt.rect = actionRect;
                p.drawControl(QStyle::CE_MenuItem, opt);
            }

            emptyArea -= QRegion(scrollUpTearOffRect);
            emptyArea -= QRegion(scrollDownRect);

            if (d->scrollUpTearOffItemEx || d->scrollDownItemEx)
            {
                if (d->scrollUpTearOffItemEx)
                    d->scrollUpTearOffItemEx->updateScrollerRects(scrollUpTearOffRect);
                if (d->scrollDownItemEx)
                    d->scrollDownItemEx->updateScrollerRects(scrollDownRect);
            }
            else
            {
                //paint scroll up /down
                d->drawScroller(&p, AMenuPrivate::ScrollerTearOffItem::ScrollUp, scrollUpRect);
                d->drawScroller(&p, AMenuPrivate::ScrollerTearOffItem::ScrollDown, scrollDownRect);
            }

            //draw border
            if (fw)
            {
                QRegion borderReg;
                borderReg += QRect(0, 0, fw, height()); //left
                borderReg += QRect(width() - fw, 0, fw, height()); //right
                borderReg += QRect(0, 0, width(), fw); //top
                borderReg += QRect(0, height() - fw, width(), fw); //bottom
                p.setClipRegion(borderReg);
                emptyArea -= borderReg;
                QStyleOptionFrame frame;
                frame.rect = rect();
                frame.palette = palette();
                frame.state = QStyle::State_None;
                frame.lineWidth = style()->pixelMetric(QStyle::PM_MenuPanelWidth, &frame, this);
                frame.midLineWidth = 0;
                p.drawPrimitive(QStyle::PE_FrameMenu, frame);
            }

            //finally the rest of the spaces
            p.setClipRegion(emptyArea);
            menuOpt.state = QStyle::State_None;
            menuOpt.menuItemType = QStyleOptionMenuItem::EmptyArea;
            menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
            menuOpt.rect = d->rframe->rect();
            menuOpt.menuRect = d->rframe->rect();
            p.drawControl(QStyle::CE_MenuEmptyArea, menuOpt);
            return true;
        }
        break;
        case QEvent::MouseMove:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        {
            //if (d->rframe->parentWidget())
            //{
            //    QMouseEvent* e = (QMouseEvent*)evt;
            //    // 将自己设为鼠标事件透明并重新搜索是否有后面的控件会响应鼠标事件。
            //    d->rframe->setAttribute(Qt::WA_TransparentForMouseEvents, true);
            //    QPoint pt = d->rframe->mapTo(d->rframe->parentWidget(), e->pos());
            //    //QWidget* w = d->rframe->parentWidget()->childAt(pt);
            //    //if (w)
            //    {
            //        pt = this->mapFrom(d->rframe->parentWidget(), pt);
            //        QMouseEvent* event = new QMouseEvent(e->type(), pt, e->button(), e->buttons(), e->modifiers());
            //        QApplication::postEvent(this, event);
            //    }
            //    // 将自己设为鼠标事件不透明，以免影响button的功能
            //    d->rframe->setAttribute(Qt::WA_TransparentForMouseEvents, false);
            //}
        }
        break;
        default:
            break;
        }
    }

    return QMenu::eventFilter(watched, evt);
}

/*!
  \reimp
*/
void AMenu::actionEvent(QActionEvent* e)
{
    QMenu::actionEvent(e);

    d->itemsDirty = 1;
    setAttribute(Qt::WA_Resized, false);

    if (e->type() == QEvent::ActionAdded)
    {
        if (QWidgetAction* wa = qobject_cast<QWidgetAction*>(e->action()))
        {
            QWidget* widget = wa->requestWidget(this);
            if (widget)
            {
                d->widgetItems.insert(wa, widget);
                if (d->scroll)
                {
                    if (!d->scrollUpTearOffItemEx)
                        d->scrollUpTearOffItemEx =
                        new AMenuPrivate::ScrollerTearOffItem(AMenuPrivate::ScrollerTearOffItem::ScrollUp, d.get(), this);
                    if (!d->scrollDownItemEx)
                        d->scrollDownItemEx =
                        new AMenuPrivate::ScrollerTearOffItem(AMenuPrivate::ScrollerTearOffItem::ScrollDown, d.get(), this);
                }
            }
        }
    }
    else if (e->type() == QEvent::ActionRemoved)
    {
        d->widgetItems.remove(static_cast<QAction*>(e->action()));
    }
}

APROCH_NAMESPACE_END
