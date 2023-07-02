/****************************************************************************
 * @file    ARibbonTitleBarWidget.cpp
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
#include <QLayout>
#include <QEvent>
#include <QStyleOption>
#include <QApplication>
#include <QMainWindow>
#include <QWidgetAction>

#include "ARibbonTitleBarWidget.h"
#include "Platform/APlatform.h"
#include "ARibbonBar.h"
#include "ARibbonBar_p.h"
#include "ARibbonBackstageView.h"
#include "ARibbonBackstageView_p.h"
#include "ARibbonSearchBar.h"
#include "Widget/Style/ACommonStyle.h"
#include "Widget/Style/ACommonStyle_p.h"
#include "Widget/Style/AStyleOption.h"
#include "Widget/AToggleSwitch.h"
#include "Widget/Style/AStyleHelper.h"
#include "ARibbonBar_p.h"
#include "ARibbonQuickAccessBar.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

static ARibbonQuickAccessBar* aproch_findQuickAccessBar(QLayout* layout)
{
    for (int i = 0; i < layout->count(); ++i)
    {
        QLayoutItem* item = layout->itemAt(i);
        if (ARibbonQuickAccessBar* quickAccessBar = qobject_cast<ARibbonQuickAccessBar*>(item->widget()))
            return quickAccessBar;
    }
    return nullptr;
}

/*!
    \class ARibbonTitleBarWidget
    \internal
*/
ARibbonTitleBarWidget::ARibbonTitleBarWidget(ARibbonBar* ribbonBar)
    : QWidget(ARibbonBarPrivate::_get(ribbonBar)->material())
    , m_ribbonBar(ribbonBar)
    , m_layout(nullptr)
    , m_searchBarAppearance(ARibbonBar::SearchBarCentral)
    , m_dirtyTextTitle(false)
{
    setAutoFillBackground(false);
    setMouseTracking(true);
    setAttribute(Qt::WA_NoSystemBackground);
    _pTopWidget = topWidget();
    if (_pTopWidget != nullptr)
    {
        connect(_pTopWidget, &QObject::destroyed, this, [&]() {_pTopWidget = nullptr; });
        topWidget()->installEventFilter(this);
    }
    ribbonTabBar()->scrollView()->installEventFilter(this);

    m_searchBar = new ARibbonSearchBar(ribbonBar);

    m_layout = new QHBoxLayout(this);
    m_layout->setSpacing(1); 

    m_leftItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_layout->addSpacerItem(m_leftItem);
    
    m_layout->addWidget(m_searchBar, 0);

    m_rightItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_layout->addSpacerItem(m_rightItem);

    //Hide search bar by default.
    setSearchBarAppearance(ARibbonBar::SearchBarHidden);
}

ARibbonTitleBarWidget::~ARibbonTitleBarWidget()
{
    topWidget()->removeEventFilter(this);
    ribbonTabBar()->scrollView()->removeEventFilter(this);
    removeContextualTabs();
}

void ARibbonTitleBarWidget::addWidget(QWidget* widget, int stretch)
{
    if (widget == nullptr)
        return;
    int index = lastLeftWidgetIndex();
    m_layout->insertWidget(index, widget, stretch);
    updateLayout();
}

void ARibbonTitleBarWidget::insertWidget(int index, QWidget* widget, int stretch, Qt::Alignment alignment)
{
	if (widget == nullptr || m_layout == nullptr)
		return;
	m_layout->insertWidget(index, widget, stretch);
	updateLayout();
}

int ARibbonTitleBarWidget::indexOf(QWidget* widget)
{
	if (widget == nullptr || m_layout == nullptr)
		return -1;

    return m_layout->indexOf(widget);
}

void ARibbonTitleBarWidget::addWidgetToRightSide(QWidget* widget, int stretch)
{
    if (widget == nullptr)
        return;
    int index = m_layout->count();
    m_layout->insertWidget(index, widget, stretch);
    updateLayout();
}

void ARibbonTitleBarWidget::removeWidget(QWidget* widget)
{
    if (widget == nullptr)
        return;
    m_layout->removeWidget(widget);
    updateLayout();
}

void ARibbonTitleBarWidget::adjustSizeTitleBar()
{
}

ARibbonBar* ARibbonTitleBarWidget::ribbonBar() const
{
    return m_ribbonBar;
}

ARibbonTabBar* ARibbonTitleBarWidget::ribbonTabBar() const
{
    return ARibbonBarPrivate::_get(ribbonBar())->m_ribbonTabBar;
}

ARibbonSearchBar* ARibbonTitleBarWidget::searchBar() const
{
    return m_searchBar;
}

AWindowTitleBar* ARibbonTitleBarWidget::getWindowTitleBar() const
{
    if (AWindowTitleBar* titleBar = ARibbonBarPrivate::_get(m_ribbonBar)->findTitleBar())
    {
        if (titleBar->widget() == this)
            return titleBar;
    }
    return nullptr;
}

class RibbonTitleBarButton : public QToolButton
{
public:
    RibbonTitleBarButton(ARibbonTitleBarWidget* titleBarWidget) 
        : QToolButton(), m_titleBarWidget(titleBarWidget) {
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        setFocusPolicy(Qt::WheelFocus);
    }

    ~RibbonTitleBarButton() override {
    }

    void initOption(QStyleOptionTitleBar& option) const
    {
        option.initFrom(this);
        option.titleBarFlags = Qt::WindowShadeButtonHint;
        option.subControls = QStyle::SC_TitleBarShadeButton;
        option.activeSubControls = QStyle::SC_TitleBarShadeButton;

        if (defaultAction() != nullptr)
        {
            option.icon = defaultAction()->icon();
            option.text = defaultAction()->text();
        }
    }

    QSize minimumSizeHint() const override
    {
        return sizeHint();
    }

    QSize sizeHint() const override {
        AWindowTitleBar* titleBar = m_titleBarWidget->getWindowTitleBar();
        int h = titleBar != nullptr ? titleBar->titleHeight() : height();
        int w = h;

        const int margin = static_cast<int>(h * 0.25);
        QString text = defaultAction() != nullptr ? defaultAction()->text() : QString();
        if (!text.isEmpty())
        {
            QFontMetrics fm = this->fontMetrics();
            QRect rect = fm.boundingRect(text);
            w += rect.width();
            w += margin;
        }
        return QSize(w, h);
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        Q_UNUSED(event);
        QStyleOptionTitleBar option;
        initOption(option);
        QPainter painter(this);
        style()->drawControl((QStyle::ControlElement)ACommonStyle::CE_RibbonTitleBarButton, &option, &painter, this);
    }

    void mousePressEvent(QMouseEvent* event) override {
        QToolButton::mousePressEvent(event);
    }
private:
    ARibbonTitleBarWidget* m_titleBarWidget;
};

QAction* ARibbonTitleBarWidget::addTitleButton(const QIcon& icon, const QString& help)
{
    QAction* result = new QAction(icon, QString(), nullptr);
    result->setToolTip(help);
    RibbonTitleBarButton* button = new RibbonTitleBarButton(this);
    button->setDefaultAction(result);
    addWidgetToRightSide(button, false);
    return result;
}

void ARibbonTitleBarWidget::removeTitleButton(QAction* action)
{
    for (int i = 0; i < m_layout->count(); ++i)
    {
        QLayoutItem* item = m_layout->itemAt(i);
        if (QToolButton* button = qobject_cast<QToolButton*>(item->widget()))
        {
            if (button->defaultAction() == action)
            {
                m_layout->removeItem(item);
                delete button;
                return;
            }
        }
    }
}

void ARibbonTitleBarWidget::setSearchBarAppearance(ARibbonBar::SearchBarAppearance appearance)
{
    if (m_searchBarAppearance == appearance)
        return;
    m_searchBarAppearance = appearance;
    if (appearance == ARibbonBar::SearchBarCentral)
    {
        m_rightItem->changeSize(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        m_searchBar->setCompact(false);
        m_searchBar->setVisible(true);
    }
    else if (appearance == ARibbonBar::SearchBarCompact)
    {
        m_rightItem->changeSize(0, 0, QSizePolicy::Minimum, QSizePolicy::Minimum);
        m_searchBar->setCompact(true);
        m_searchBar->setVisible(true);
    }
    else
    {
        m_rightItem->changeSize(0, 0, QSizePolicy::Minimum, QSizePolicy::Minimum);
        m_searchBar->setVisible(false);
    }
    layout()->invalidate();
}

ARibbonBar::SearchBarAppearance ARibbonTitleBarWidget::searchBarAppearance() const
{
    return m_searchBarAppearance;
}

QSize ARibbonTitleBarWidget::calcMinSize(QWidget* widget) const
{
    const QSize sizeHint = widget->sizeHint();
    const QSize minSizeHint = widget->minimumSizeHint();
    const QSize minSize = widget->minimumSize();
    const QSize maxSize = widget->maximumSize();
    const QSizePolicy sizePolicy = widget->sizePolicy();

    QSize size(0, 0);
    if (sizePolicy.horizontalPolicy() != QSizePolicy::Ignored)
    {
        if (sizePolicy.horizontalPolicy() & QSizePolicy::ShrinkFlag)
            size.setWidth(minSizeHint.width());
        else
            size.setWidth(qMax(sizeHint.width(), minSizeHint.width()));
    }

    if (sizePolicy.verticalPolicy() != QSizePolicy::Ignored)
    {
        if (sizePolicy.verticalPolicy() & QSizePolicy::ShrinkFlag)
            size.setHeight(minSizeHint.height());
        else
            size.setHeight(qMax(sizeHint.height(), minSizeHint.height()));
    }

    size = size.boundedTo(maxSize);
    if (minSize.width() > 0)
        size.setWidth(minSize.width());
    if (minSize.height() > 0)
        size.setHeight(minSize.height());
    return size.expandedTo(QSize(0, 0));
}

QRect ARibbonTitleBarWidget::calcTextRect() const
{
    int left = 0;
    int right = width();
    left = qMax(left, m_leftItem->geometry().left());
    right = qMin(right, m_leftItem->geometry().right());

    QRect rcTitleText(left, 0, right - left, height());
    if (m_contextualTabs.count() > 0)
    {
        QRect cr = geometry();
        QRect rcHeaders(QPoint(m_contextualTabs[0]->rect().left(), cr.top()),
            QPoint(m_contextualTabs[m_contextualTabs.count() - 1]->rect().right(), cr.bottom()));

        int leftLength = rcHeaders.left() - rcTitleText.left();
        int rightLength = rcTitleText.right() - rcHeaders.right();
        if (leftLength < rightLength)
        {
            rcTitleText.setLeft(rcHeaders.right());
            rcTitleText.setRight(rcTitleText.right());
        }
        else
        {
            rcTitleText.setRight(rcHeaders.left());
        }
    }
    return rcTitleText;
}

QRect ARibbonTitleBarWidget::calcContextualAreaRect() const
{
    int offset = 0;
    if (AWindowTitleBar* titleBar = getWindowTitleBar())
        offset = geometry().left() - titleBar->borderThickness();
    int left = offset;
    int right = width();
    left = qMax(left, m_leftItem->geometry().left());
    right = qMin(right, /*m_rightItem*/m_leftItem->geometry().right());
    return QRect(QPoint(left, 0), QPoint(right, height() - 1));
}

int ARibbonTitleBarWidget::lastLeftWidgetIndex() const
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
    return m_layout->indexOf(m_leftItem);
#else
    for (int i = 0; i < m_layout->count(); ++i)
        if (m_layout->itemAt(i) == m_leftItem)
            return i;
    return -1;
#endif
}

void ARibbonTitleBarWidget::setVisible(bool visible)
{
    QWidget::setVisible(visible);
}

void ARibbonTitleBarWidget::updateLayout()
{
    int spacing = 6;
    if (ARibbonSystemButton* systemButton = m_ribbonBar->systemButton())
    {
        if (systemButton->toolButtonStyle() == Qt::ToolButtonIconOnly)
            spacing += systemButton->width();
    }
    m_layout->setContentsMargins(QMargins(spacing, 0, 0, 0));

    //m_layout->invalidate();
    for (int i = 0; i < m_layout->count(); ++i)
    {
        QLayoutItem* item = m_layout->itemAt(i);
        if (item->widget() != nullptr)
            item->widget()->updateGeometry(); //Update layout size cache.
    }

    updateContextualTabs();
    update();
}

/*! \internal */
QWidget* ARibbonTitleBarWidget::titleBarWindow() const
{
    QWidget* ret = m_ribbonBar->parentWidget();
    if (ret == nullptr)
        return nullptr;
    if (ret->isWindow() || qobject_cast<QMainWindow*>(ret) != nullptr)
        return ret;
    ret = ret->parentWidget();

    if (ret->isWindow() || qobject_cast<QMainWindow*>(ret) != nullptr)
        return ret;
    return nullptr;
}

QWidget* ARibbonTitleBarWidget::topWidget() const
{
#ifndef QTITAN_DESIGNER
    if (m_ribbonBar->parentWidget() && m_ribbonBar->parentWidget()->isWindow())
        return m_ribbonBar->parentWidget();
    else
        return m_ribbonBar;
#else
    return m_ribbonBar;
#endif
}

void ARibbonTitleBarWidget::updateTextTitle()
{
    if (!m_dirtyTextTitle)
        return;

    const bool ribbonBarEmbeded = parentWidget() == ribbonBar();
    if (ribbonBarEmbeded)
        m_strTitle = QString();
    else
        m_strTitle = titleBarWindow() != nullptr ? titleBarWindow()->windowTitle() : QString();

    m_dirtyTextTitle = false;
}

void ARibbonTitleBarWidget::windowTitleChanged(const QString& text)
{
    m_strTitle = text;
    m_dirtyTextTitle = true;
    updateLayout();
}

QFont ARibbonTitleBarWidget::titleFont() const
{
    return QApplication::font("QMdiSubWindowTitleBar");
}

void ARibbonTitleBarWidget::initTitleBarOption(ATitleBarStyleOption* opt) const
{
    opt->initFrom(this);
    if (ARibbonSystemButton* button = m_ribbonBar->systemButton())
    {
        if (const ARibbonBackstageView* backstage = qobject_cast<const ARibbonBackstageView*>(button->backstage()))
        {
            opt->isBackstageVisible = !backstage->isHidden();
            opt->menuWidth = ARibbonBackstageViewPrivate::_get(backstage)->m_backstageMenu->width();
            opt->backstageFrameMenuPalette = ARibbonBackstageViewPrivate::_get(backstage)->m_backstageMenu->palette();
        }
    }

    if (!opt->isBackstageVisible)
    {
        opt->contextualAreaRect = calcContextualAreaRect();
        //opt->contextualAreaRect.translate(-opt->contextualAreaRect.left(), 0);
        for (int i = 0, count = m_contextualTabs.count(); i < count; ++i)
        {
            AContextualTab* contextualTab = m_contextualTabs[i];
            opt->contextualTabs.append(ATitleBarStyleOption::ContextualTabInfo(contextualTab->rect(), contextualTab->title(), contextualTab->color()));
        }
    }

    opt->text = m_strTitle;
    if (searchBarAppearance() == ARibbonBar::SearchBarCentral)
        opt->alignment = Qt::AlignVCenter | Qt::AlignLeft | Qt::TextSingleLine;
    else
        opt->alignment = Qt::AlignCenter | Qt::TextSingleLine;

    opt->titleRect = calcTextRect();
    opt->contextFont = font();
    opt->titleFont = titleFont();
    if (ARibbonSystemButton* systemButton = m_ribbonBar->systemButton())
    {
        if (systemButton->toolButtonStyle() == Qt::ToolButtonIconOnly)
        {
            systemButton->initButtonStyleOption(&opt->optButton);
            const int halfHeight = opt->optButton.rect.height() / 2;
            opt->optButton.rect.translate(0, geometry().bottom() - halfHeight);
            if (opt->optButton.state & QStyle::State_Sunken)
                opt->optButton.state |= QStyle::State_Sunken;
        }
    }
}

AContextualTab* ARibbonTitleBarWidget::hitContextHeaders(const QPoint& point) const
{
    QRect contextualAreaRect = calcContextualAreaRect();
    for (int i = 0, count = m_contextualTabs.count(); i < count; ++i)
    {
        AContextualTab* contextualTab = m_contextualTabs[i];
        if (contextualTab->rect().intersected(contextualAreaRect).contains(point))
            return contextualTab;
    }
    return nullptr;
}

void ARibbonTitleBarWidget::updateContextualTabs()
{
    removeContextualTabs();
    if (!ribbonBar()->isContextualTabsVisible())
        return;
    AContextualTab* prevContextualTab = nullptr;
    ARibbonTabBar* tabBar = ribbonTabBar();
    int count = tabBar->count();
    if (count == 0)
        return;

    for (int i = 0; i < count; ++i)
    {
        ARibbonTab* tab = tabBar->getTab(i);
        if (tab->isHidden())
            continue;

        if (tab->contextTitle().isEmpty())
        {
            prevContextualTab = nullptr;
            continue;
        }

        if (prevContextualTab != nullptr &&
            !prevContextualTab->groupName().isEmpty() &&
            prevContextualTab->groupName() == tab->contextGroupName())
        {
            prevContextualTab->lastTab = tab;
            tab->setContextualTab(prevContextualTab);
        }
        else
        {
            prevContextualTab = new AContextualTab(ribbonBar(), tab);
            m_contextualTabs.append(prevContextualTab);
        }
    }
}

void ARibbonTitleBarWidget::removeContextualTabs()
{
    for (int i = 0; i < m_contextualTabs.count(); i++)
        delete m_contextualTabs[i];
    m_contextualTabs.clear();
}

/*! \reimp */
bool ARibbonTitleBarWidget::event(QEvent* event)
{
    switch (event->type())
    {
        case QEvent::ParentChange:
        {
            m_dirtyTextTitle = true;
            break;
        }
        case QEvent::Resize:
            updateLayout();
            break;
#if 0
        case QEvent::PaletteChange:
        {
            QColor c = palette().windowText().color();
            c = c;
        }
        break;
#endif
        case QEvent::StyleChange:
            break;
        default:
            break;
    }
    return QWidget::event(event);
}

bool ARibbonTitleBarWidget::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ribbonBar() && event->type() == QEvent::ParentChange)
    {
        if (_pTopWidget != nullptr)
            _pTopWidget->removeEventFilter(this);

		_pTopWidget = topWidget();
        if (_pTopWidget != nullptr)
        {
			connect(_pTopWidget, &QObject::destroyed, this, [&]() {_pTopWidget = nullptr; });
            _pTopWidget->installEventFilter(this);
        }
    }

    if (watched == ribbonTabBar()->scrollView())
    {
        if (event->type() == QEvent::Move)
        {
            //Repaint contextual tabs on scroll the ribbonBar tabs.
            update();
        }
    }
    else if (watched == topWidget())
    {
        if (event->type() == QEvent::WindowTitleChange ||
            event->type() == QEvent::FontChange)
        {
            m_dirtyTextTitle = true;
            m_searchBar->setFont(titleFont());
            updateLayout();
        }
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        else if (event->type() == QEvent::WinIdChange)
        {
            if (QObject* wnd = (QObject*)(topWidget()->windowHandle()))
                QObject::connect(wnd, SIGNAL(windowTitleChanged(const QString&)), this, SLOT(windowTitleChanged(const QString&)));
        }
#endif
    }
    return QWidget::eventFilter(watched, event);
}

/*! \reimp */
void ARibbonTitleBarWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    updateTextTitle();
    QPainter painter(this);

    ATitleBarStyleOption opt;
    initTitleBarOption(&opt);
    style()->drawComplexControl(static_cast<QStyle::ComplexControl>(ACommonStyle::CC_RibbonTitleBarWidget), &opt, &painter, this);

    if (ARibbonQuickAccessBar* quickAccessBar = aproch_findQuickAccessBar(m_layout))
    {
        if (quickAccessBar->visibleCount() > 0)
        {
            painter.translate(quickAccessBar->geometry().topLeft());
            quickAccessBar->paintContent(&painter);
        #if 0 //remove me
            RibbonQuickAccessBarStyleOption opt;
            opt.initFrom(accessBar);
            const int hor = ACommonStylePrivate::dpiScaled(2, accessBar);
            const int ver = ACommonStylePrivate::dpiScaled(1, accessBar);
            opt.rect = accessBar->geometry().adjusted(-hor, -ver, hor, ver);
            opt.quickAccessBarPosition = ARibbonBar::TopPosition;
            ARibbonSystemButton* systemButton = ribbonBar()->systemButton();
            opt.roundButton = systemButton && systemButton->toolButtonStyle() == Qt::ToolButtonIconOnly;
            QPalette_setColor(opt.palette, QPalette::Window, accessBar->parentWidget()->palette().color(QPalette::Window));
            style()->drawControl(QStyle::CE_ToolBar, &opt, &painter, accessBar);
        #endif
        }
    }
}

/*! \reimp */
void ARibbonTitleBarWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (AContextualTab* contextualTab = hitContextHeaders(event->pos()))
        {
            int index = ribbonTabBar()->tabIndex(contextualTab->firstTab);
            if (index != -1)
            {
                m_ribbonBar->setCurrentPageIndex(index);
                return;
            }
        }
    }
    event->ignore();
}

APROCH_NAMESPACE_END