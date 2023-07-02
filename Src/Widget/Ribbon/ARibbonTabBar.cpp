/****************************************************************************
 * @file    ARibbonTabBar.cpp
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
#include <QPainter>
#include <QTime>
#include <QWidgetAction>
#include <QStylePainter>
#include <QStyleOption>
#include <QEvent>

#include "ARibbonTabBar.h"
#include "ARibbonBar_p.h"
#include "ARibbonPage_p.h"
#include "ARibbonBackstageView.h"

#include "Widget/Style/AStyleHelper.h"
#include "Widget/Style/AStyleOption.h"
#include "Widget/Style/ACommonStyle_p.h"
#include "ARibbonQuickAccessBar.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

/* AContextualTab */
AContextualTab::AContextualTab(ARibbonBar* bar, ARibbonTab* tab)
    : ribbonBar(bar), firstTab(tab), lastTab(tab)
{
    Q_ASSERT(bar != nullptr && tab != nullptr);
    firstTab->setContextualTab(this);
}

AContextualTab::~AContextualTab()
{
    if (firstTab != nullptr)
        firstTab->setContextualTab(nullptr);
    if (lastTab != nullptr)
        lastTab->setContextualTab(nullptr);
}

const QColor& AContextualTab::color() const
{
    if (firstTab != nullptr)
        return firstTab->contextColor();
    static QColor empty = QColor();
    return empty;
}

const QString& AContextualTab::title() const
{
    if (firstTab != nullptr)
        return firstTab->contextTitle();
    static QString empty = QString();
    return empty;
}

const QString& AContextualTab::groupName() const
{
    if (firstTab != nullptr)
        return firstTab->contextGroupName();
    static QString empty = QString();
    return empty;
}

QRect AContextualTab::rect() const
{
    ARibbonBarPrivate* ribbonBarPrivate = ARibbonBarPrivate::_get(ribbonBar);

    int offset = 0;
    if (AWindowTitleBar* titleBar = ribbonBarPrivate->m_ribbonTitleBarWidget->getWindowTitleBar())
        offset = ribbonBarPrivate->m_ribbonTitleBarWidget->geometry().left() - titleBar->borderThickness();

    QRect firstRect = firstTab->geometry();
    QRect lastRect = lastTab->geometry();
    firstRect.moveTo(firstTab->mapTo(ribbonBar, QPoint()));
    lastRect.moveTo(lastTab->mapTo(ribbonBar, QPoint()));
    QRect rect(QPoint(firstRect.x(), 0), QPoint(lastRect.right(), ribbonBarPrivate->m_ribbonTitleBarWidget->height()));
    rect.translate(-offset, 0);
    return rect;
}

QFont AContextualTab::font() const
{
    ARibbonBarPrivate* ribbonBarPrivate = ARibbonBarPrivate::_get(ribbonBar);
    return ribbonBarPrivate->m_ribbonTitleBarWidget->font();
}

class ARibbonTabPrivate : public QObject
{
public:
    A_DECLARE_PUBLIC(ARibbonTab)
public:
    explicit ARibbonTabPrivate();
    static ARibbonTabPrivate* get(ARibbonTab* tab)
    { return &tab->aproch_d(); }
public:
    void init();
    void updateLabel();
public:
    Qt::Alignment m_align;
    AContextualTab* m_contextualTab;
    QAction* m_defaultAction;
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    QTime m_dblClickInterval;
#endif
    mutable uint m_isTextLabel : 1;
};

ARibbonTabPrivate::ARibbonTabPrivate()
{
}

void ARibbonTabPrivate::init()
{
    A_P(ARibbonTab);
    m_contextualTab = nullptr;
    m_isTextLabel   = false;
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    m_dblClickInterval.start();
#endif
    m_align = Qt::Alignment(Qt::AlignLeft | Qt::AlignVCenter /*| Qt::TextExpandTabs*/);
    m_defaultAction = new QWidgetAction(&p);
    p.setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred, QSizePolicy::Label));
}

void ARibbonTabPrivate::updateLabel()
{
    A_P(ARibbonTab);
    if (m_isTextLabel)
    {
        QSizePolicy policy = p.sizePolicy();
        const bool wrap = m_align & Qt::TextWordWrap;
        policy.setHeightForWidth(wrap);
        // should be replaced by WA_WState_OwnSizePolicy idiom
        if (policy != p.sizePolicy())  
            p.setSizePolicy(policy);
    }
    p.update(p.contentsRect());
}

/* ARibbonTab */
ARibbonTab::ARibbonTab(ARibbonTabBar* parent, const QString& text)
    : AAbstractTab(parent, QIcon(), QString())
{
    A_INIT_PRIVATE(ARibbonTab);
    A_D(ARibbonTab);
    d.init();
    setText(text);
    setAlignment(Qt::AlignCenter);
}

ARibbonTab::~ARibbonTab()
{
    A_D(ARibbonTab);
    if (d.m_contextualTab != nullptr)
    {
        if (d.m_contextualTab->firstTab == this)
            d.m_contextualTab->firstTab = nullptr;
        if (d.m_contextualTab->lastTab == this)
            d.m_contextualTab->lastTab  = nullptr;
    }
    A_DELETE_PRIVATE();
}

ARibbonTabBar* ARibbonTab::tabBar() const
{ 
    return static_cast<ARibbonTabBar*>(m_tabBar);
}

ARibbonPage* ARibbonTab::page() const
{ 
    return qobject_cast<ARibbonPage*>(object());
}

Qt::Alignment ARibbonTab::alignment() const
{
    A_D(const ARibbonTab);
    return QFlag(d.m_align & (Qt::AlignVertical_Mask|Qt::AlignHorizontal_Mask));
}

void ARibbonTab::setAlignment(Qt::Alignment alignment)
{
    A_D(ARibbonTab);
    if (alignment == (d.m_align & (Qt::AlignVertical_Mask|Qt::AlignHorizontal_Mask)))
        return;
    d.m_align = (d.m_align & ~(Qt::AlignVertical_Mask|Qt::AlignHorizontal_Mask)) | 
        (alignment & (Qt::AlignVertical_Mask|Qt::AlignHorizontal_Mask));

    d.updateLabel();
}

void ARibbonTab::setText(const QString& text)
{
    A_D(ARibbonTab);
    d.m_isTextLabel = true;
    AAbstractTab::setText(text);
    d.updateLabel();
}

const QString& ARibbonTab::text() const
{
    return m_text;
}

const QString& ARibbonTab::contextTitle() const
{
    if (page() != nullptr)
        return page()->contextTitle();
    static QString empty = QString();
    return empty;
}

const QString& ARibbonTab::contextGroupName() const
{
    if (page() != nullptr)
        return page()->contextGroupName();
    static QString empty = QString();
    return empty;
}

const QColor& ARibbonTab::contextColor() const
{
    if (page() != nullptr)
        return page()->contextColor();
    static QColor empty = QColor();
    return empty;
}

void ARibbonTab::setContextualTab(AContextualTab* tab)
{
    A_D(ARibbonTab);
    d.m_contextualTab = tab;
}

AContextualTab* ARibbonTab::contextualTab() const
{
    A_D(const ARibbonTab);
    return d.m_contextualTab;
}

QAction* ARibbonTab::defaultAction() const
{
    A_D(const ARibbonTab);
    return d.m_defaultAction;
}

QSize ARibbonTab::sizeHint() const
{
    QSize sh = AAbstractTab::sizeHint();
    if (contextualTab() != nullptr)
    {
        int spacing = style()->pixelMetric(QStyle::PM_TabBarTabVSpace, nullptr, tabBar());
        sh.rwidth() = qMax(aproch_horizontalAdvanceFont(QFontMetrics(contextualTab()->font()), contextualTab()->title()) + spacing * 2, sh.width());
    }
    return sh;
}
QSize ARibbonTab::minimumSizeHint() const
{
    return AAbstractTab::minimumSizeHint();
}

void ARibbonTab::clicked()
{
    bool selectionChanged = isCurrent();
    AAbstractTab::clicked();
    selectionChanged = selectionChanged != isCurrent();

    ARibbonBar* _ribbonBar = tabBar()->ribbonBar();
    
    Q_ASSERT(_ribbonBar != nullptr);
    if (_ribbonBar == nullptr)
        return;

    if (_ribbonBar->isBackstageVisible())
    {
        if (ARibbonSystemButton* button = _ribbonBar->systemButton())
        {
            if (ARibbonBackstageView* backstage = qobject_cast<ARibbonBackstageView*>(button->backstage()))
                backstage->close();
        }
    }

    if (_ribbonBar->isMinimized())
    {
        ARibbonBarPrivate* barprivate = ARibbonBarPrivate::_get(_ribbonBar);
        if (selectionChanged || !barprivate->isPagePopupVisible())
            barprivate->showPagePopup(page());
        else
            barprivate->hidePagePopup();
    }
}

void ARibbonTab::dblClicked()
{
    AAbstractTab::dblClicked();
    ARibbonBar* _ribbonBar = tabBar()->ribbonBar();
    if (_ribbonBar != nullptr)
        ARibbonBarPrivate::_get(_ribbonBar)->toggledMinimized();
}

/*! \reimp */
void ARibbonTab::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    A_D(ARibbonTab);

    QPainter p(this);
    p.setClipRegion(event->region());

    ARibbonOptionHeaderStyleOption opt;
    initStyleOption(&opt);
    opt.firstTab = contextualTab() != nullptr && contextualTab()->firstTab == this;
    opt.lastTab = contextualTab() != nullptr && contextualTab()->lastTab == this;
    opt.rcFirst = contextualTab() != nullptr ? contextualTab()->firstTab->rect() : QRect();
    opt.rcLast = contextualTab() != nullptr ? contextualTab()->lastTab->rect() : QRect();
    opt.textAlignment = d.m_align;
    opt.contextColor = contextColor();

    const bool isHighlight = opt.state & QStyle::State_MouseOver;

    bool minimized = false;
    bool backstageVisible = false;
    ARibbonTabBar* _tabBar = this->tabBar();
    ARibbonBar* _ribbonBar = _tabBar->ribbonBar();
    opt.tabBarPosition = _ribbonBar->tabBarPosition();

    minimized = _ribbonBar->isMinimized();
    opt.minimized = minimized;
    backstageVisible = _ribbonBar->isBackstageVisible();

    int max = m_maxSize;
    int w = opt.rect.width();
    if (w < max)
        opt.state |= QStyle::State_Small;
    else
        opt.state &= ~QStyle::State_Small;

    ARibbonPage* _page = page();
    const bool selected = !backstageVisible && isCurrent() && _page != nullptr && _page->isVisible();
    if (selected)
    {
        opt.state |= QStyle::State_Selected;
        if (minimized || backstageVisible)
            opt.state |= QStyle::State_Sunken;
    }
    else
    {
        opt.state &= ~QStyle::State_Selected;
        if (minimized || backstageVisible)
            opt.state &= ~QStyle::State_Sunken;
    }

    bool drawControl = true;
    if (minimized)
    {
        ARibbonBarPrivate* barprivate = ARibbonBarPrivate::_get(_ribbonBar);
        if (!barprivate->isPagePopupVisible())
        {
            drawControl = false;
            opt.state &= ~QStyle::State_Selected;
        }
    }

    if (m_dragOffset != 0)
        opt.rect.moveLeft(opt.rect.x() + m_dragOffset);

    if (!_tabBar->m_startDrag)
    {
        if (isHighlight || drawControl)
            style()->drawControl(static_cast<QStyle::ControlElement>(ACommonStyle::CE_RibbonTab), &opt, &p, this);
        style()->drawControl(static_cast<QStyle::ControlElement>(ACommonStyle::CE_RibbonTabShapeLabel), &opt, &p, this);
    }
}

/* ARibbonTabBar */
ARibbonTabBar::ARibbonTabBar(ARibbonBar* ribbonBar)
    : AAbstractTabBar(ARibbonBarPrivate::_get(ribbonBar)->material())
{
    setScrollable(true);
//    setAlignmentRightLayout(Qt::AlignRight);
    setElideMode(Qt::ElideRight);
    m_ribbonBar = ribbonBar;
}

ARibbonTabBar::~ARibbonTabBar()
{
}

ARibbonBar* ARibbonTabBar::ribbonBar() const
{ return m_ribbonBar; }

ARibbonTab* ARibbonTabBar::getTab(int index)
{ return qobject_cast<ARibbonTab*>(tabAt(index)); }

const ARibbonTab* ARibbonTabBar::getTab(int index) const
{ return qobject_cast<const ARibbonTab*>(tabAt(index)); }

QAction* ARibbonTabBar::addAction(const QIcon& icon, const QString& text, Qt::ToolButtonStyle style, QMenu* menu)
{
    QAction* action = new QAction(icon, text, this);
    if (menu != nullptr)
        action->setMenu(menu);
    return addAction(action, style);
}

QAction* ARibbonTabBar::addAction(QAction* action, Qt::ToolButtonStyle style)
{
    if ( Qt::ToolButtonTextUnderIcon == style )
    {
        Q_ASSERT(false);
        style = Qt::ToolButtonTextBesideIcon;
    }
    QWidget::addAction(action);
    ARibbonButton* button = new ARibbonButton(this);

    button->setAutoRaise(true);
    button->setFocusPolicy(Qt::NoFocus);
    button->setToolButtonStyle(style);

    action->setIconText(action->text());
    button->setDefaultAction(action);
    button->setVisible(true);
    addWidgetToRightSide(button);
    return action;
}

QMenu* ARibbonTabBar::addMenu(const QString& title)
{
    QMenu* menu = new QMenu(title, this);
    ARibbonButton* button = new ARibbonButton(this);
    button->setAutoRaise(true);
    button->setFocusPolicy(Qt::NoFocus);
    button->setPopupMode(QToolButton::InstantPopup);
    button->setMenu(menu);
    button->setToolButtonStyle(Qt::ToolButtonTextOnly);
    button->setDefaultAction(menu->menuAction());
    button->setVisible(true);
    addWidgetToRightSide(button);
    return menu;
}

void ARibbonTabBar::paintTab(QPainter* painter, AAbstractTab* tab) const
{
    ARibbonOptionHeaderStyleOption opt;
    tab->initStyleOption(&opt);
    opt.textAlignment = ARibbonTabPrivate::get(static_cast<ARibbonTab *>(tab))->m_align;
    opt.position = QStyleOptionTab::OnlyOneTab;
    style()->drawControl(static_cast<QStyle::ControlElement>(ACommonStyle::CE_RibbonTab), &opt, painter, tab);
    style()->drawControl(static_cast<QStyle::ControlElement>(ACommonStyle::CE_RibbonTabShapeLabel), &opt, painter, tab);
}

void ARibbonTabBar::paintTabBarFrame(QPainter* painter, QStyleOptionTabBarBase* optTabBase)
{
    Q_UNUSED(painter);
    Q_UNUSED(optTabBase);
}

/*! \reimp */
bool ARibbonTabBar::event(QEvent* event)
{
    switch(event->type())
    {
        case QEvent::PaletteChange:
        {
        }
        break;
        case QEvent::Wheel:
            {
                if (ribbonBar()->isMinimized())
                {
                    event->ignore();
                    return true;
                }
                break;
            }
        default:
            break;
    }
    return AAbstractTabBar::event(event);
}

/*! \reimp */
void ARibbonTabBar::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
    QStyleOption opt;
    opt.initFrom(this);
    style()->drawControl(static_cast<QStyle::ControlElement>(ACommonStyle::CE_RibbonTabBar), &opt, &p, this);
}

/*! \reimp */
void ARibbonTabBar::mouseDoubleClickEvent(QMouseEvent* event)
{
    event->setAccepted(false);
    AAbstractTabBar::mouseDoubleClickEvent(event);
}

AAbstractTab* ARibbonTabBar::createTab(const QIcon& icon, const QString& text, QObject* object)
{
    Q_UNUSED(icon);
    ARibbonTab* tab = new ARibbonTab(this, text);
    tab->setObject(object);
    ARibbonPagePrivate::_get(tab->page())->setAssociativeTab(tab);
    return tab;
}

/*! \reimp */
QSize ARibbonTabBar::sizeHint() const
{
    QSize sh = AAbstractTabBar::sizeHint();
    return sh;
}

APROCH_NAMESPACE_END
