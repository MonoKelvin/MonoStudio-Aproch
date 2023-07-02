/****************************************************************************
 * @file    ARibbonSystemMenu.cpp
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
#include "ARibbonSystemMenu_p.h"
#include <QApplication>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QWidgetAction>
#include <QFileInfo>
#include <QKeySequence>
#include <QActionGroup>

#include "ARibbonDef.h"
#include "ARibbonBar_p.h"
#include "ARibbonBackstageView.h"
#include "Widget/Style/ACommonStyle_p.h"
#include "Widget/Style/ACommonStyle.h"
#include "Widget/Style/AStyleOption.h"
#include "Widget/Style/AStyleHelper.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

static QMargins aproch_getSysMenuMargins(int top, const QWidget* widget)
{
    static const int systemPopupBorderLeft = 1;
    static const int systemPopupBorderRight = 1;
    static const int systemPopupBorderBottom = 29;

    const int sysPopupBorderBottom = ACommonStylePrivate::dpiScaled(systemPopupBorderBottom, widget);
    const int sysPopupBorderRight = ACommonStylePrivate::dpiScaled(systemPopupBorderRight, widget);
    const int sysPopupBorderLeft = ACommonStylePrivate::dpiScaled(systemPopupBorderLeft, widget);
    return QMargins(sysPopupBorderLeft, top + 2, sysPopupBorderRight, sysPopupBorderBottom);
}

static const int splitActionPopupWidth = 20; // Split button drop down width in popups

/* ARibbonSystemButton */
ARibbonSystemButton::ARibbonSystemButton(ARibbonBar* ribbonBar)
    : ARibbonSystemButton(ARibbonBarPrivate::_get(ribbonBar)->material())
{
    A_D(ARibbonSystemButton);
    d.m_ribbonBar = ribbonBar;
    qApp->installEventFilter(this);
}

ARibbonSystemButton::ARibbonSystemButton(QWidget* parent)
    : QToolButton(parent)
{
    A_INIT_PRIVATE(ARibbonSystemButton);
}

ARibbonSystemButton::~ARibbonSystemButton()
{
    qApp->removeEventFilter(this);
    A_DELETE_PRIVATE();
}

ARibbonBar* ARibbonSystemButton::ribbonBar() const
{
    A_D(const ARibbonSystemButton);
    return d.m_ribbonBar;
}

ARibbonBackstageView* ARibbonSystemButton::backstage() const
{
    A_D(const ARibbonSystemButton);
    return qobject_cast<ARibbonBackstageView*>(d.m_backstageWidget);
}

void ARibbonSystemButton::setBackstage(ARibbonBackstageView* backstage)
{
    Q_ASSERT(backstage != nullptr);
    if (backstage == nullptr)
        return; 

    A_D(ARibbonSystemButton);
    if (QAction* action = defaultAction())
    {
        d.m_backstageWidget = nullptr;
        if (QMenu* menu = action->menu())
        {
            action->setMenu(nullptr);
            delete menu;
        }

        d.m_backstageWidget = backstage;
        QObject::connect(action, SIGNAL(triggered()), backstage, SLOT(open()));
    }
}

void ARibbonSystemButton::setBackgroundColor(const QColor& color)
{
    QPalette p = palette();
    QPalette_setColor(p, QPalette::Button, color);
    setPalette(p);
    update();
}

QColor ARibbonSystemButton::backgroundColor() const
{
    return palette().color(QPalette::Button);
}

ARibbonSystemMenu* ARibbonSystemButton::systemMenu() const
{
    if (defaultAction() == nullptr)
        return nullptr;
    return qobject_cast<ARibbonSystemMenu *>(defaultAction()->menu());
}

void ARibbonSystemButton::setSystemMenu(ARibbonSystemMenu* menu)
{
    if (defaultAction() == nullptr)
    {
        QAction* action = new QAction(this);
        setDefaultAction(action);
    }

    if (QMenu* menu = defaultAction()->menu())
        delete menu;
    defaultAction()->setMenu(menu);
}

void ARibbonSystemButton::initButtonStyleOption(ASystemToolButtonStyleOption* option) const
{
    initStyleOption(option);
    option->toolButtonStyle = toolButtonStyle();
    if (!option->icon.isNull())
    {
        QPixmap pm = option->icon.pixmap(iconSize());
        option->iconSize = pm.size();
    }
}

/*! \reimp */
QSize ARibbonSystemButton::sizeHint() const
{
    QSize sz = QToolButton::sizeHint();

    ARibbonBar* _ribbonBar = ribbonBar();
    if (_ribbonBar == nullptr)
        return sz;

    ASystemToolButtonStyleOption opt;
    initStyleOption(&opt);

    QRect rc = opt.rect;
    if (opt.toolButtonStyle == Qt::ToolButtonIconOnly)
        rc.adjust(2, 2, -2, -2);

    int index = -1;
    int curArea = 0;
    const int actualArea = rc.height()*rc.width();
    const QList<QSize> lSz = opt.icon.availableSizes();
    for (int i = 0, count = lSz.count(); count > i; i++)
    {
        QSize curSz = lSz[i];
        int area = curSz.height()*curSz.width();
        if (actualArea > area)
        {
            if (area > curArea)
                index = i;
            curArea = area;
        }
    }

    ARibbonSystemButton* that = const_cast<ARibbonSystemButton*>(this);
    if (index >= 0 && index < lSz.size())
        that->setIconSize(lSz[index]);

    int indent = 0;
    if (opt.toolButtonStyle == Qt::ToolButtonTextOnly)
        indent = aproch_horizontalAdvanceFont(opt.fontMetrics, QLatin1Char('x')) * 4;
    else if (opt.toolButtonStyle == Qt::ToolButtonIconOnly)
        indent = 24;

    int heightTabs = ARibbonBarPrivate::_get(_ribbonBar)->tabBarHeight();
    return QSize(sz.width() + indent, heightTabs);
}

/*! \reimp */
bool ARibbonSystemButton::event(QEvent* event)
{
    A_D(ARibbonSystemButton);
    bool result = QToolButton::event(event);
    switch (event->type()) 
    {
        case QEvent::HoverEnter:
        case QEvent::HoverLeave:
        case QEvent::HoverMove:
            d.updateExtension();
            break;
        default:
            break;
    }
    return result;
}

/*! \reimp */
bool ARibbonSystemButton::eventFilter(QObject* watched, QEvent* event)
{
    A_D(ARibbonSystemButton);
    if (ARibbonBar* _ribbonBar = ribbonBar())
    {
        ARibbonBarPrivate* privateRibbon = ARibbonBarPrivate::_get(_ribbonBar);
        if (watched == privateRibbon->m_ribbonTitleBarWidget)
        {
            switch (event->type())
            {
                case QEvent::MouseButtonPress:
                {
                    if (toolButtonStyle() == Qt::ToolButtonIconOnly)
                    {
                        const QMouseEvent* me = static_cast<const QMouseEvent*>(event);
                        QPoint pnt = this->window()->mapFromGlobal(QCursor::pos());
                        QRect rect = geometry();
                        if (rect.contains(pnt))
                        {
                            QMouseEvent mEvent(QEvent::MouseButtonPress, rect.bottomRight(),
                                rect.bottomRight(), me->button(), me->buttons(), me->modifiers());
                            this->event(&mEvent);
                        }
                    }
                    break;
                }
                case QEvent::HoverEnter:
                case QEvent::HoverLeave:
                case QEvent::HoverMove:
                {
                    if (toolButtonStyle() == Qt::ToolButtonIconOnly)
                    {
                        QPoint pnt = this->window()->mapFromGlobal(QCursor::pos());
                        QRect rect = geometry();
                        if (rect.contains(pnt))
                            setAttribute(Qt::WA_UnderMouse, true);
                        else if (event->type() == QEvent::HoverLeave)
                            setAttribute(Qt::WA_UnderMouse, false);
                        d.updateExtension();
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
    return QToolButton::eventFilter(watched, event);
}

/*! \reimp */
void ARibbonSystemButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
    ASystemToolButtonStyleOption opt;
    initButtonStyleOption(&opt);
    style()->drawComplexControl(static_cast<QStyle::ComplexControl>(ACommonStyle::CC_RibbonFileButton), &opt, &p, this);
}

/*! \reimp */
void ARibbonSystemButton::mousePressEvent(QMouseEvent* event)
{
    A_D(ARibbonSystemButton);
    if (ARibbonBackstageView* bs = backstage())
    {
        if (bs->isVisible())
        {
            bs->close();
            return; 
        }
    }
    d.updateExtension();
    QToolButton::mousePressEvent(event);
    d.updateExtension();
}

void ARibbonSystemButtonPrivate::updateExtension()
{
    A_P(ARibbonSystemButton);
    ARibbonBar* ribbonBar = p.ribbonBar();
    if (p.toolButtonStyle() == Qt::ToolButtonIconOnly && ribbonBar != nullptr)
    {
        ARibbonBarPrivate* privateRibbon = ARibbonBarPrivate::_get(ribbonBar);
        if (privateRibbon->m_ribbonTitleBarWidget)
            privateRibbon->m_ribbonTitleBarWidget->repaint(p.rect());
        p.repaint();
    }
}

/* ARibbonSystemMenuButton */
ARibbonSystemMenuButton::ARibbonSystemMenuButton(QWidget* parent)
    : QToolButton(parent)
{
}

ARibbonSystemMenuButton::~ARibbonSystemMenuButton()
{
}

/*! \reimp */
bool ARibbonSystemMenuButton::event(QEvent* event)
{
    if (event->type() == QEvent::ToolTip)
        event->setAccepted(false);
    return QToolButton::event(event);
}

/*! \reimp */
void ARibbonSystemMenuButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    {
        QPainter p(this);
        QStyleOptionToolButton opt;
        initStyleOption(&opt);
        opt.iconSize = opt.icon.actualSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
        style()->drawPrimitive((QStyle::PrimitiveElement)ACommonStyle::PE_RibbonPopupBarButton, &opt, &p, this);
    } //QToolButton::paintEvent() will create the second painter, so we need to destroy the first painter.
    QToolButton::paintEvent(event);
}


/* ARibbonSystemMenuPrivate */
ARibbonSystemMenuPrivate::ARibbonSystemMenuPrivate()
    : m_ribbonBar(nullptr)
{
}

int ARibbonSystemMenuPrivate::ribbonBarItemHeight() const
{
    A_P(const ARibbonSystemMenu);
    if (p.ribbonBar() == nullptr)
        return 0;
    return ARibbonBarPrivate::_get(p.ribbonBar())->rowItemHeight();
}

void ARibbonSystemMenuPrivate::updateMargins()
{
    A_P(ARibbonSystemMenu);
    QMargins margins = aproch_getSysMenuMargins(ribbonBarItemHeight(), &p);
    setMargins(margins);
}

/*!
\class ARibbonSystemMenu
\inmodule QtitanRibbon
\brief Implements the system menu for ribbon bar. 
Inherited from QMenu, but has a number of additional methods for adding items as a follow ms office specification.
*/

/*!
Constructs ARibbonSystemMenu object with the given \a parent.
*/
ARibbonSystemMenu::ARibbonSystemMenu(ARibbonBar* ribbonBar)
    : APopupMenu(*new ARibbonSystemMenuPrivate, ribbonBar)
{
    A_D(ARibbonSystemMenu);
    Q_ASSERT(ribbonBar != nullptr);
    d.m_ribbonBar = ribbonBar;
}

/*!
Destructor of the ARibbonSystemMenu object.
*/
ARibbonSystemMenu::~ARibbonSystemMenu()
{
}

/*!
Returns ARibbonBar that owns the menu.
*/
ARibbonBar* ARibbonSystemMenu::ribbonBar() const
{
    A_D(const ARibbonSystemMenu);
    return d.m_ribbonBar;
}

/*!
Adds the button to the bottom of the menu. \a text - the text on the button. Returns a pointer to the QAction, associated with the button.
*/
QAction* ARibbonSystemMenu::addPopupBarAction(const QString& text)
{
    A_D(ARibbonSystemMenu);
    ARibbonSystemMenuButton* systemPopupBarButton = new ARibbonSystemMenuButton(this);
    systemPopupBarButton->setAutoRaise(true);
    systemPopupBarButton->setToolButtonStyle(Qt::ToolButtonTextOnly);

    d.m_systemButtonList.append(systemPopupBarButton);
    systemPopupBarButton->setText(text);

    QAction* pAction = new QAction(text, systemPopupBarButton);
    systemPopupBarButton->setDefaultAction(pAction);
    return pAction;
}

/*!
Adds the button to the bottom of the menu with the given \a style.
Parameter \a action - is a pointer to the QAction object used to initialize the tips (AToolTip), the text in the Status Bar (StatusTip) and icon.
*/
void ARibbonSystemMenu::addPopupBarAction(QAction* action, Qt::ToolButtonStyle style)
{
    A_D(ARibbonSystemMenu);
    if (style == Qt::ToolButtonTextUnderIcon)
    {
        Q_ASSERT_X(false, Q_FUNC_INFO, "Invalid style button");
        return;
    }

    ARibbonSystemMenuButton* systemPopupBarButton = new ARibbonSystemMenuButton(this);
    systemPopupBarButton->setAutoRaise(true);
    systemPopupBarButton->setToolButtonStyle(style);

    d.m_systemButtonList.append(systemPopupBarButton);
    systemPopupBarButton->setText(action->text());
    systemPopupBarButton->setDefaultAction(action);
}

/*!
Adds the page to the right side of the system menu for a list of recently used files.
Parameter \a caption - is a title of the recent file list page. Returns a pointer to ARibbonPageSystemRecentFileList for later use.
*/
ARibbonPageSystemRecentFileList* ARibbonSystemMenu::addPageRecentFile(const QString& caption)
{
    ARibbonPageSystemRecentFileList* recentFileList = new ARibbonPageSystemRecentFileList(caption);
    setPreviewWidget(recentFileList);
    return recentFileList;
}

/*!
Adds the system popup menu to the menu with the given \caption. Parameter \a defaultAction is a default action for the popup 
that can be additionaly configured with \a splitAction parameter. Returns a pointer to ARibbonPageSystemPopup for later use.
*/
ARibbonPageSystemPopup* ARibbonSystemMenu::addPageSystemPopup(const QString& caption, QAction* defaultAction, bool splitAction)
{
    A_D(ARibbonSystemMenu);
    ARibbonPageSystemPopup* systemPopup = new ARibbonPageSystemPopup(caption, this);
    systemPopup->setDefaultAction(defaultAction);
    if (splitAction)
        defaultAction->setProperty(_aproch_SplitActionPopup, true);
    defaultAction->setMenu(systemPopup);
    d.m_pageList.append(systemPopup);
    return systemPopup;
}

/*! \reimp */
void ARibbonSystemMenu::setVisible(bool visible)
{
    ARibbonBar* ribbonBar = qobject_cast<ARibbonBar*>(parentWidget());
    if (ribbonBar == nullptr)
        return;
    if (ribbonBar != nullptr)
    {
        ARibbonBarPrivate* ribbinPrivate = ARibbonBarPrivate::_get(ribbonBar);
        QRect rectMenu = geometry();
        QPoint posButton = ribbinPrivate->m_ribbonTabBar->mapToGlobal(QPoint(0, 0));
        QRect screen = aproch_availableGeometry(this);
        const QSize menuSizeHint = sizeHint();
        if (screen.bottom() < menuSizeHint.height() + posButton.y())
        {
            if (QWidget* parentWidget = ribbonBar->parentWidget())
            {
                QRect frameRect = parentWidget->frameGeometry();
                QRect rect = parentWidget->geometry();
                if (posButton.y() - screen.top() < menuSizeHint.height())
                    posButton.setY(screen.top());
                else
                    posButton.setY(rect.y() - (menuSizeHint.height() + qAbs(frameRect.top() - rect.top())));
            }
        }
        move(QPoint(rectMenu.x(), posButton.y()));
    }
#if 0
    else
    {
        QToolButton* button = ribbonBar ? ribbonBar->systemButton() : nullptr;
        if (button != nullptr)
        {
            QRect rectMenu = geometry();
            QPoint posButton = button->mapToGlobal(button->rect().topLeft());
            move(QPoint(rectMenu.x(), posButton.y()));
        }
    }
#endif
    APopupMenu::setVisible(visible);
}

/*! \reimp */
QSize ARibbonSystemMenu::sizeHint() const
{
    A_D(const ARibbonSystemMenu);
    const_cast<ARibbonSystemMenuPrivate*>(&d)->updateMargins();
    QSize sh = APopupMenu::sizeHint();

    int maxWidget = 0;
    for (QList<QWidget*>::const_iterator it = d.m_pageList.constBegin(); it != d.m_pageList.constEnd(); ++it)
        maxWidget = qMax(maxWidget, (*it)->sizeHint().width());

    int previewWidth = 0;
    if (d.m_preview != nullptr)
        previewWidth = d.m_preview->sizeHint().width();
//    else
//        previewWidth = 10;

    QStyleOption opt(0);
    opt.initFrom(this);
    const int margin = 0;
    if (previewWidth < maxWidget)
        sh.setWidth(sh.width() + (maxWidget - previewWidth) + margin);
    else
        sh.rwidth() += margin;
    return sh;
}

/*! \internal */
void ARibbonSystemMenu::initFrameMenuStyleOption(ARibbonFrameMenuStyleOption* option) const
{
    A_D(const ARibbonSystemMenu);
    QSize szMenu = APopupMenu::sizeHint();
    option->initFrom(this);
    option->isAdditionalPages = d.m_pageList.size() > 0 || previewWidget() != nullptr;

    int width = d.m_scrollMenuBar->size().width();
    QMargins margins = aproch_getSysMenuMargins(d.ribbonBarItemHeight(), this);
    option->margins = margins;

    option->actionsWidth = width;
    option->actionsHeight = szMenu.height();
}

/*! \reimp */
void ARibbonSystemMenu::mousePressEvent(QMouseEvent* event)
{
    if (ARibbonBar* _ribbonBar = ribbonBar())
    {
        if (QToolButton* button = _ribbonBar->systemButton())
        {
            QRect rectButton = button->rect();
            if (rectButton.isValid())
            {
                QPoint posButton = button->mapToGlobal(rectButton.topLeft());
                if (QRect(posButton, button->size()).contains(event->globalPos()))
                {
                    QMouseEvent ev(QEvent::MouseButtonPress, QPoint(-1, -1), event->button(), event->buttons(), event->modifiers());
                    QMenu::mousePressEvent(&ev);
                    return;
                }
            }
        }
    }
    QMenu::mousePressEvent(event);
}

/*! \reimp */
void ARibbonSystemMenu::mouseReleaseEvent(QMouseEvent* event)
{
    QAction* action = actionAt(event->pos());
    if (action && action->property(_aproch_SplitActionPopup).toBool())
    {
        QRect rect = actionGeometry(action);
        rect.adjust(0, 0, -splitActionPopupWidth, 0);
        if (rect.contains(event->pos()))
        {
            APopupMenu::mousePressEvent(event);
            action->trigger();
        }
    }
    else
        APopupMenu::mouseReleaseEvent(event);
}

/*! \reimp */
void ARibbonSystemMenu::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
    ARibbonFrameMenuStyleOption menuOpt;
    initFrameMenuStyleOption(&menuOpt);
    style()->drawPrimitive((QStyle::PrimitiveElement)ACommonStyle::PE_RibbonSysFrameMenu, &menuOpt, &p, this);
 
    if (ARibbonBar* _ribbonBar = ribbonBar())
    {
        if (ARibbonSystemButton* button = _ribbonBar->systemButton())
        {
            if (button->geometry().top() != this->geometry().top())
            {
                ASystemToolButtonStyleOption opt;
                button->initStyleOption(&opt);

                Qt::ToolButtonStyle styleButton = button->toolButtonStyle();
                opt.toolButtonStyle = styleButton;

                QRect rc = opt.rect;
                QPoint posButton = button->mapToGlobal(rc.topLeft());
                posButton = mapFromGlobal(posButton);
                rc.moveTo(posButton);
                opt.rect = rc;
                if (!opt.icon.isNull())
                {
                    QPixmap pm = opt.icon.pixmap(opt.iconSize);
                    opt.iconSize = pm.size();
                }
                style()->drawComplexControl((QStyle::ComplexControl)ACommonStyle::CC_RibbonFileButton, &opt, &p, button);
            }
        }
    }
}

void ARibbonSystemMenu::paintMenuItem(QPainter* painter, QStyleOptionMenuItem* option)
{
    style()->drawControl((QStyle::ControlElement)ACommonStyle::CE_RibbonSysMenuItem, option, painter, this);
}

/*! \reimp */
void ARibbonSystemMenu::keyPressEvent(QKeyEvent* event)
{
    APopupMenu::keyPressEvent(event);

    if (!event->isAccepted())
    {
        A_D(ARibbonSystemMenu);
        for (int i = 0; i < d.m_pageList.count(); i++)
        {
            if (ARibbonPageSystemRecentFileList* page = qobject_cast<ARibbonPageSystemRecentFileList*>(d.m_pageList.at(i)))
            {
                page->keyPressEvent(event);
                break;
            }
        }
    }
}

/*! \reimp */
void ARibbonSystemMenu::updateLayout()
{
    A_D(ARibbonSystemMenu);
#if 0
    APopupMenu::updateLayout();
#endif
    QMargins margins = d.margins();// aproch_getSysMenuMargins(d.ribbonBarItemHeight(), this);

    QSize scrollBarSizeHint = d.m_scrollMenuBar->sizeHint();
    scrollBarSizeHint.rheight() = height() - margins.top() - margins.bottom();

    QRect scrollBarRect(QPoint(margins.left(), margins.top()), scrollBarSizeHint);
    d.m_scrollMenuBar->setGeometry(scrollBarRect);

    QSize sz(rect().size());
    int right = sz.width() - margins.left() - margins.right();
    for (QList<ARibbonSystemMenuButton*>::iterator it = d.m_systemButtonList.begin(); it != d.m_systemButtonList.end(); ++it)
    {
        ARibbonSystemMenuButton* barButton = (*it);
        QSize szControl = barButton->sizeHint();
        QRect rc( QPoint(right - szControl.width(), sz.height() - margins.bottom() + 2), QPoint(right, sz.height() - 3));
        barButton->move( rc.left(), rc.top() );
        barButton->resize( rc.width(), rc.height() );
        right -= szControl.width() + 6;
    }

    int previewwWidth = 0;
    for (QList<QWidget*>::iterator it = d.m_pageList.begin(); it != d.m_pageList.end(); ++it)
        previewwWidth = qMax(previewwWidth, (*it)->sizeHint().width());

    if (QWidget* preview = previewWidget())
        previewwWidth = qMax(previewwWidth, preview->sizeHint().width());

    for (QList<QWidget*>::iterator it = d.m_pageList.begin(); it != d.m_pageList.end(); ++it)
        (*it)->setMinimumWidth(previewwWidth);

    if (d.m_preview != nullptr)
    {
        QSize s = scrollBarSizeHint;
        s.setWidth(previewwWidth);
        QRect previewRect(QPoint(margins.left() + scrollBarSizeHint.width(), margins.top()), s);
        d.m_preview->setGeometry(previewRect);
    }
}

/*! \reimp */
void ARibbonSystemMenu::changeEvent(QEvent* event)
{
    APopupMenu::changeEvent(event);
}


/* ARibbonPageSystemRecentFileListPrivate */
ARibbonPageSystemRecentFileListPrivate::ARibbonPageSystemRecentFileListPrivate()
{
    m_itemsDirty = true;
    m_hasCheckableItems = false;
    m_currentAction = nullptr;
}

QAction* ARibbonPageSystemRecentFileListPrivate::actionAt(const QPoint& pt) const
{
    A_P(const ARibbonPageSystemRecentFileList);

    if (!p.rect().contains(pt)) //sanity check
        return 0;

    for(int i = 0; i < m_actionRects.count(); i++)
    {
        if (m_actionRects.at(i).contains(pt))
            return m_recentFileActs.at(i);
    }
    return 0;
}

QRect ARibbonPageSystemRecentFileListPrivate::actionRect(QAction* act) const
{
    int index = m_recentFileActs.indexOf(act);
    if (index == -1)
        return QRect();
    const_cast<ARibbonPageSystemRecentFileListPrivate*>(this)->updateActionRects();
    //we found the action
    return m_actionRects.at(index);
}

void ARibbonPageSystemRecentFileListPrivate::setCurrentAction(QAction* currentAction)
{
    A_P(ARibbonPageSystemRecentFileList);

    if (m_currentAction && m_currentAction != currentAction)
        p.update(actionRect(m_currentAction));

    if (m_currentAction == currentAction)
        return;
    m_currentAction = currentAction;

    p.update(actionRect(m_currentAction));
}

void ARibbonPageSystemRecentFileListPrivate::activateAction(QAction* action, QAction::ActionEvent action_e, bool self)
{
    Q_UNUSED(self);
    Q_UNUSED(action_e);

    A_P(ARibbonPageSystemRecentFileList);
    p.parentWidget()->hide();

//    action->activate(action_e);
    emit p.openRecentFile(action->data().toString());
}

void ARibbonPageSystemRecentFileListPrivate::updateActionRects()
{
    A_P(const ARibbonPageSystemRecentFileList);
    if (!m_itemsDirty)
        return;
    m_itemsDirty = false;

    m_actionRects.resize(m_recentFileActs.count());
    m_actionRects.fill(QRect());

    // let's try to get the last visible action
    int lastVisibleAction = m_recentFileActs.count() - 1;
    for(;lastVisibleAction >= 0; --lastVisibleAction) 
    {
        const QAction *action = m_recentFileActs.at(lastVisibleAction);
        if (action->isVisible())
            break;
    }

    QStyle* style = p.style();
    QStyleOption opt;
    opt.initFrom(&p);

    const int hmargin = style->pixelMetric(QStyle::PM_MenuHMargin, &opt, &p),
        vmargin = style->pixelMetric(QStyle::PM_MenuVMargin, &opt, &p);

    const int fw = style->pixelMetric(QStyle::PM_MenuPanelWidth, &opt, &p);
    QRect rcCaption = style->subElementRect((QStyle::SubElement)ACommonStyle::SE_RibbonSysHeaderLabelPopupList, &opt, &p);
    int max_column_width = rcCaption.width() - vmargin - fw;
    int y = rcCaption.bottom() + 4;

    // calculate size
    QFontMetrics qfm = p.fontMetrics();
    for(int i = 0; i <= lastVisibleAction; i++) 
    {
        QAction* action = m_recentFileActs.at(i);

        if (!action->isVisible())
            continue; // we continue, this action will get an empty QRect

        // let the style modify the above size..
        QStyleOptionMenuItem opt;
        initStyleOption(&opt, action);
        const QFontMetrics& fm = opt.fontMetrics;

        QSize sz;
        QString s = action->text();
        sz.setWidth(fm.boundingRect(QRect(), Qt::TextSingleLine | Qt::TextShowMnemonic, s).width());
        sz.setHeight(qMax(fm.height(), qfm.height()));
        sz = style->sizeFromContents(QStyle::CT_MenuItem, &opt, sz, &p);
        //update the item
        if (!sz.isEmpty()) 
            m_actionRects[i] = QRect(0, 0, sz.width(), sz.height());
    }

    int x = hmargin + fw;
    for(int i = 0; i < m_recentFileActs.count(); i++) 
    {
        QRect& rect = m_actionRects[i];
        if (rect.isNull())
            continue;

        rect.translate(x, y); // move
        rect.setWidth(max_column_width); // uniform width

        y += rect.height();
    }
}

void ARibbonPageSystemRecentFileListPrivate::initStyleOption(QStyleOptionMenuItem* option, const QAction* action) const
{
    A_P(const ARibbonPageSystemRecentFileList);
    if (option == nullptr || action == nullptr)
        return;
    option->initFrom(&p);
    option->palette = p.palette();
    option->state = QStyle::State_None;

    if (p.isEnabled() && action->isEnabled())
        option->state |= QStyle::State_Enabled;

    option->font = action->font().resolve(p.font());
    option->fontMetrics = QFontMetrics(option->font);

    if (m_currentAction && m_currentAction == action) 
        option->state |= QStyle::State_Selected | (m_mouseDown ? QStyle::State_Sunken : QStyle::State_None);

    option->menuHasCheckableItems = m_hasCheckableItems;
    if (!action->isCheckable()) 
    {
        option->checkType = QStyleOptionMenuItem::NotCheckable;
    } 
    else 
    {
        option->checkType = (action->actionGroup() && action->actionGroup()->isExclusive())
            ? QStyleOptionMenuItem::Exclusive : QStyleOptionMenuItem::NonExclusive;
        option->checked = action->isChecked();
    }

    option->menuItemType = QStyleOptionMenuItem::Normal;
    if (action->isIconVisibleInMenu())
        option->icon = action->icon();

    QString textAndAccel = action->text();
    if (textAndAccel.indexOf(QLatin1Char('\t')) == -1) 
    {
        QKeySequence seq = action->shortcut();
        if (!seq.isEmpty())
            textAndAccel += QLatin1Char('\t') + seq.toString();
    }

    option->text = textAndAccel;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    option->tabWidth = 0;
#endif
    option->maxIconWidth = 0;
    option->menuRect = p.rect();
}

/*!
\class ARibbonPageSystemRecentFileList
\inmodule QtitanRibbon
\brief Class ARibbonPageSystemRecentFileList Used to display a list of recently used files in the Ribbon UI system menu.
*/

/*!
\fn void ARibbonPageSystemRecentFileList::openRecentFile(const QString& file);
The signal will be issued if the \a file has been choosed in the menu.
*/

/*!
Constructs ARibbonPageSystemRecentFileList object with the given \a caption.
*/
ARibbonPageSystemRecentFileList::ARibbonPageSystemRecentFileList(const QString& caption)
{
    A_INIT_PRIVATE(ARibbonPageSystemRecentFileList);
    setWindowTitle(caption);
    if (caption.isEmpty())
        setWindowTitle(ARibbonBar::tr_compatible(RibbonRecentDocumentsString));
    setAttribute(Qt::WA_MouseTracking, true);
}

/*!
Destructor of the ARibbonPageSystemRecentFileList object.
*/
ARibbonPageSystemRecentFileList::~ARibbonPageSystemRecentFileList()
{
    A_DELETE_PRIVATE();
}

/*! \reimp */
QSize ARibbonPageSystemRecentFileList::sizeHint() const
{
    QSize s;
    ARibbonSystemMenu* systemMenu = qobject_cast<ARibbonSystemMenu*>(parentWidget());
    Q_ASSERT(systemMenu != nullptr);
    QList<QAction*> actionList = systemMenu->actions();
    for (QList<QAction*>::iterator it = actionList.begin(); it != actionList.end(); ++it)
    {
        QAction* action = (*it);
        QRect rect = systemMenu->actionGeometry(action);
        if (rect.isNull())
            continue;
        s.setHeight(rect.y() + rect.height());
    }
    s.setWidth(qMax(s.width(), int(ACommonStylePrivate::dpiScaled(300., this))));
    return s;
}

/*!
Sets the \a size of the file list. The list size can not exceed 9 files.
*/
void ARibbonPageSystemRecentFileList::setSize(int size)
{
    A_D(ARibbonPageSystemRecentFileList);

    if (size < 0 || size > 9)
    {
        Q_ASSERT_X(false, Q_FUNC_INFO, "Number of files could not be more than 9.");
        return; 
    }
    // clean actions
    for (int i = 0, count = d.m_recentFileActs.count(); count > i; i++)
    {
        QAction* act = d.m_recentFileActs[i];
        removeAction(act);
        delete act;
    }
    d.m_recentFileActs.clear();

    for (int i = 0; i < size; ++i) 
    {
        QAction* recentFileAct = new QAction(this);
        recentFileAct->setVisible(false);
        addAction(recentFileAct);
        d.m_recentFileActs.append(recentFileAct);
//        connect(recentFileAct, SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }
}

/*!
Returns the size of the file list.
*/
int ARibbonPageSystemRecentFileList::getSize() const
{
    A_D(const ARibbonPageSystemRecentFileList);
    return d.m_recentFileActs.size();
}

/*!
Returns a pointer to the current QAction, which is under mouse and highlighted.
*/
QAction* ARibbonPageSystemRecentFileList::getCurrentAction() const
{
    A_D(const ARibbonPageSystemRecentFileList);
    return d.m_currentAction;
}

static QString strippedName(const QString& fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

/*!
Slot is intended to update the file list. Parameter \a files holds the file's names.
*/
void ARibbonPageSystemRecentFileList::updateRecentFileActions(const QStringList& files)
{
    A_D(ARibbonPageSystemRecentFileList);

    if (d.m_recentFileActs.size() == 0)
    {
        Q_ASSERT_X(false, Q_FUNC_INFO, "Number of files not defined.");
        return;
    }

    int numRecentFiles = qMin(files.size(), d.m_recentFileActs.size());
    for (int i = 0; i < numRecentFiles; ++i) 
    {
        QAction* recentFileAct = d.m_recentFileActs.at(i);
        QString text = QString(QStringLiteral("&%1")).arg(i + 1);
        text += QStringLiteral("%1 %2").arg(QLatin1Char(' ')).arg(strippedName(files[i]));
        recentFileAct->setText(text);
        recentFileAct->setData(files[i]);
        recentFileAct->setVisible(true);
    }
    for (int j = numRecentFiles; j < d.m_recentFileActs.size(); ++j)
        d.m_recentFileActs[j]->setVisible(false);

    d.m_itemsDirty = true;
}

/*! \reimp */
void ARibbonPageSystemRecentFileList::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QStyleOption opt;
    opt.initFrom(this);
    style()->drawPrimitive((QStyle::PrimitiveElement)ACommonStyle::PE_RibbonSysFramePagePopupList, &opt, &painter, this);
    paintItems(&painter);
}

void ARibbonPageSystemRecentFileList::paintItems(QPainter* painter)
{
    A_D(ARibbonPageSystemRecentFileList);
    QRegion emptyArea = QRegion(rect());
    //draw the items that need updating..
    for (int i = 0; i < d.m_recentFileActs.count(); ++i) 
    {
        QAction* action = d.m_recentFileActs.at(i);
        QRect adjustedActionRect = d.actionRect(action);
        //set the clip region to be extra safe (and adjust for the scrollers)
        QRegion adjustedActionReg(adjustedActionRect);
        emptyArea -= adjustedActionReg;
        painter->setClipRegion(adjustedActionReg);

        QStyleOptionMenuItem opt;
        d.initStyleOption(&opt, action);
        opt.rect = adjustedActionRect;
        style()->drawControl((QStyle::ControlElement)ACommonStyle::CE_RibbonSysRecentFileItem, &opt, painter, this);
    }
}

/*! \reimp */
void ARibbonPageSystemRecentFileList::mousePressEvent(QMouseEvent* event)
{
    QWidget::mousePressEvent(event);
}

/*! \reimp */
void ARibbonPageSystemRecentFileList::mouseMoveEvent(QMouseEvent* event)
{
    A_D(ARibbonPageSystemRecentFileList);

    if (!isVisible())
        return;
    d.setCurrentAction(d.actionAt(event->pos()));
}

/*! \reimp */
void ARibbonPageSystemRecentFileList::mouseReleaseEvent(QMouseEvent* event)
{
    A_D(ARibbonPageSystemRecentFileList);

    QAction* action = d.actionAt(event->pos());
    if (action && action == d.m_currentAction) 
    {
#if defined(Q_WS_WIN)
        //On Windows only context menus can be activated with the right button
        if (event->button() == Qt::LeftButton)
#endif
            d.activateAction(action, QAction::Trigger);
    }
    else
        QWidget::mouseReleaseEvent(event);
}

/*! \reimp */
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
void ARibbonPageSystemRecentFileList::enterEvent(QEnterEvent* event)
#else
void ARibbonPageSystemRecentFileList::enterEvent(QEvent* event)
#endif
{
    QWidget::enterEvent(event);

    QEvent leaveEvent(QEvent::Leave);
    QApplication::sendEvent(parentWidget(), &leaveEvent);

    setFocus();
}

/*! \reimp */
void ARibbonPageSystemRecentFileList::leaveEvent(QEvent* event)
{
    Q_UNUSED(event);

    A_D(ARibbonPageSystemRecentFileList);
    d.setCurrentAction(nullptr);
    parentWidget()->setFocus();
}

/*! \reimp */
void ARibbonPageSystemRecentFileList::keyPressEvent(QKeyEvent* event)
{
    A_D(ARibbonPageSystemRecentFileList);

    int key = event->key();
    bool key_consumed = false;
    switch(key) 
    {
        case Qt::Key_Up:
        case Qt::Key_Down: 
            {
                key_consumed = true;
                QAction* nextAction = nullptr;
                if (!d.m_currentAction) 
                {
                    if(key == Qt::Key_Down) 
                    {
                        for(int i = 0; i < d.m_recentFileActs.count(); ++i) 
                        {
                            QAction* act = d.m_recentFileActs.at(i);
                            if (d.m_actionRects.at(i).isNull())
                                continue;
                            if ( (style()->styleHint(QStyle::SH_Menu_AllowActiveAndDisabled, 0, this) || act->isEnabled())) 
                            {
                                nextAction = act;
                                break;
                            }
                        }
                    } 
                    else 
                    {
                        for (int i = d.m_recentFileActs.count()-1; i >= 0; --i) 
                        {
                            QAction* act = d.m_recentFileActs.at(i);
                            if (d.m_actionRects.at(i).isNull())
                                continue;
                            if ( (style()->styleHint(QStyle::SH_Menu_AllowActiveAndDisabled, 0, this) || act->isEnabled())) 
                            {
                                nextAction = act;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    for(int i = 0, y = 0; !nextAction && i < d.m_recentFileActs.count(); i++) 
                    {
                        QAction *act = d.m_recentFileActs.at(i);
                        if (act == d.m_currentAction) 
                        {
                            if (key == Qt::Key_Up) 
                            {
                                for(int next_i = i-1; true; next_i--) 
                                {
                                    if (next_i == -1) 
                                    {
                                        if(!style()->styleHint(QStyle::SH_Menu_SelectionWrap, 0, this))
                                            break;
                                        next_i = d.m_actionRects.count()-1;
                                    }
                                    QAction *next = d.m_recentFileActs.at(next_i);
                                    if (next == d.m_currentAction)
                                        break;
                                    if (d.m_actionRects.at(next_i).isNull())
                                        continue;
                                    if (next->isSeparator() ||
                                        (!next->isEnabled() &&
                                        !style()->styleHint(QStyle::SH_Menu_AllowActiveAndDisabled, 0, this)))
                                        continue;
                                    nextAction = next;
                                    break;
                                }
                            } 
                            else 
                            {
                                y += d.m_actionRects.at(i).height();
                                for(int next_i = i+1; true; next_i++) 
                                {
                                    if (next_i == d.m_recentFileActs.count()) 
                                    {
                                        if(!style()->styleHint(QStyle::SH_Menu_SelectionWrap, 0, this))
                                            break;
                                        next_i = 0;
                                    }
                                    QAction* next = d.m_recentFileActs.at(next_i);
                                    if (next == d.m_currentAction)
                                        break;
                                    if (d.m_actionRects.at(next_i).isNull())
                                        continue;
                                    if (!next->isEnabled() && !style()->styleHint(QStyle::SH_Menu_AllowActiveAndDisabled, 0, this))
                                        continue;
                                    nextAction = next;
                                    break;
                                }
                            }
                            break;
                        }
                        y += d.m_actionRects.at(i).height();
                    }
                }

                if (nextAction) 
                    d.setCurrentAction(nextAction);
            }
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter: 
            {
                if (!d.m_currentAction) 
                {
                    key_consumed = true;
                    break;
                }
                d.activateAction(d.m_currentAction, QAction::Trigger);
                key_consumed = true;
            }
            break;
        default:
            key_consumed = false;
            break;
    }

    if (!key_consumed)
    {
        if ((!event->modifiers() || event->modifiers() == Qt::AltModifier || event->modifiers() == Qt::ShiftModifier) && event->text().length()==1 ) 
        {
            bool activateAction = false;
            QAction *nextAction = 0;

            int clashCount = 0;
            QAction* first = 0, *currentSelected = 0, *firstAfterCurrent = 0;
            QChar c = event->text().at(0).toUpper();
            for(int i = 0; i < d.m_recentFileActs.size(); ++i) 
            {
                if (d.m_actionRects.at(i).isNull())
                    continue;
                QAction *act = d.m_recentFileActs.at(i);
                QKeySequence sequence = QKeySequence::mnemonic(act->text());
                int key = sequence[0] & 0xffff;
                if (key == c.unicode()) 
                {
                    clashCount++;
                    if (!first)
                        first = act;
                    if (act == d.m_currentAction)
                        currentSelected = act;
                    else if (!firstAfterCurrent && currentSelected)
                        firstAfterCurrent = act;
                }
            }
            if (clashCount == 1)
                activateAction = true;

            if (clashCount >= 1) 
            {
                if (clashCount == 1 || !currentSelected || !firstAfterCurrent)
                    nextAction = first;
                else
                    nextAction = firstAfterCurrent;
            }

            if (nextAction) 
            {
                key_consumed = true;
                if (!nextAction->menu() && activateAction)
                    d.activateAction(nextAction, QAction::Trigger);
            }
        }

#ifdef Q_OS_WIN32
        if (key_consumed && (event->key() == Qt::Key_Control || event->key() == Qt::Key_Shift || event->key() == Qt::Key_Meta))
            QApplication::beep();
#endif // Q_OS_WIN32
    }

    if (key_consumed)
        event->accept();
    else
        event->ignore();
}

/*! \reimp */
void ARibbonPageSystemRecentFileList::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    A_D(ARibbonPageSystemRecentFileList);
    d.m_itemsDirty = true;
}

void ARibbonPageSystemPopupPrivate::updateMargins()
{
    A_P(const ARibbonPageSystemPopup);
    QSize sz = captionSize();
    const int margin = aproch_DPIScaled(2, &p);
    QMargins margins(margin, sz.height(), margin, margin);
    setMargins(margins);
}

QSize ARibbonPageSystemPopupPrivate::captionSize() const
{
    A_P(const ARibbonPageSystemPopup);
    QStyleOptionMenuItem menuOpt;
    menuOpt.initFrom(&p);
    QRect rcCaption = p.style()->subElementRect(static_cast<QStyle::SubElement>(ACommonStyle::SE_RibbonSysHeaderLabelPopupList), &menuOpt, &p);
    const int hMarginCaption = p.style()->pixelMetric(static_cast<QStyle::PixelMetric>(ACommonStyle::PM_RibbonHMarginCaptionSysMenu), &menuOpt, &p);
    QFont fontCaption = p.font();
    fontCaption.setBold(true);
    QFontMetrics fm(fontCaption);
    QRect rect = fm.boundingRect(m_caption);
    QSize sz = rect.size();
    sz.rwidth() += hMarginCaption * 2;
    sz.setHeight(qMax(sz.height(), rcCaption.height()));

    int maxWidth = sz.width();
    QSize actionSize = sz;
    QList<QAction*> actionList = p.actions();
    for (QList<QAction*>::iterator it = actionList.begin(); it != actionList.end(); ++it)
    {
        QRect r = p.actionGeometry(*it);
        actionSize = r.size();
        maxWidth = qMax(maxWidth, actionSize.width());
    }
    sz.setWidth(maxWidth);
    return sz;
}

void ARibbonPageSystemPopupPrivate::initGeometry()
{
    A_P(ARibbonPageSystemPopup);
    if (ARibbonSystemMenu* systemMenu = qobject_cast<ARibbonSystemMenu*>(p.parentWidget()))
    {
        QSize sh = p.sizeHint();
        QMargins margins = aproch_getSysMenuMargins(systemMenu->aproch_d().ribbonBarItemHeight(), systemMenu);
        QRect rect = systemMenu->scrollMenuRect();
        QPoint pos = systemMenu->mapToGlobal(rect.topRight());
        int w = systemMenu->width();
        w -= rect.width() + margins.left() + margins.right();
        rect.setWidth(w);
        rect.moveTo(pos);
        p.setGeometry(rect);
    }
}

/* ARibbonPageSystemPopup */
ARibbonPageSystemPopup::ARibbonPageSystemPopup(const QString& caption, QWidget* parent)
    : APopupMenu(*new ARibbonPageSystemPopupPrivate, parent)
{
    A_D(ARibbonPageSystemPopup);
    d.m_caption = caption;
}

ARibbonPageSystemPopup::~ARibbonPageSystemPopup()
{
}

/*! \reimp */
QSize ARibbonPageSystemPopup::sizeHint() const
{
    A_D(const ARibbonPageSystemPopup);
    const_cast<ARibbonPageSystemPopupPrivate*>(&d)->updateMargins();
    QSize sz = APopupMenu::sizeHint();
    sz.setWidth(qMax(sz.width(), d.captionSize().width()));
    return sz;
}

/*! \reimp */
bool ARibbonPageSystemPopup::event(QEvent* event)
{
    A_D(ARibbonPageSystemPopup);
    bool result = APopupMenu::event(event);

    if (event->type() == QEvent::Show)
        d.initGeometry();
    return  result;
}

/*! \reimp */
void ARibbonPageSystemPopup::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    A_D(ARibbonPageSystemPopup);
    QPainter p(this);
    QStyleOptionMenuItem menuOpt;
    menuOpt.initFrom(this);
    menuOpt.state = QStyle::State_None;
    menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
    menuOpt.maxIconWidth = 0;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    menuOpt.tabWidth = 0;
#endif
    menuOpt.text = d.m_caption;
    menuOpt.menuRect = QRect(QPoint(0, 0), QSize(menuOpt.rect.width(), d.captionSize().height()));
    style()->drawPrimitive((QStyle::PrimitiveElement)ACommonStyle::PE_RibbonSysFramePagePopup, &menuOpt, &p, this);
}

void ARibbonPageSystemPopup::paintMenuItem(QPainter* painter, QStyleOptionMenuItem* option)
{
    style()->drawControl((QStyle::ControlElement)ACommonStyle::CE_RibbonSysMenuItem, option, painter, this);
}

APROCH_NAMESPACE_END