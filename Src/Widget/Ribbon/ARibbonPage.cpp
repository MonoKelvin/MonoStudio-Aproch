/****************************************************************************
 * @file    ARibbonPage.cpp
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
#include <QStyleOption>
#include <QEvent>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtMath>
#else
#include <QtCore/qmath.h>
#endif

#include "ARibbonPage.h"
#include "ARibbonPage_p.h"
#include "Widget/Style/ACommonStyle_p.h"
#include "Widget/Style/AStyleHelper.h"
#include "ARibbonGroup.h"
#include "ARibbonBar.h"
#include "ARibbonBar_p.h"
#include "ARibbonGroup_p.h"
#include "ARibbonDef.h"

#ifdef Q_OS_WIN
#include <qt_windows.h>
#endif // Q_OS_WIN

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

#ifdef Q_OS_LINUX
static const int widthButtonScroll = 15;
#else
static const int widthButtonScroll = 11;
#endif

static const int pageMarginPopup = 2;
static const int minRibbonWidth  = 100;

/* ARibbonPagePrivate */
ARibbonPagePrivate::ARibbonPagePrivate()
    : m_ribbonBar(nullptr)
    , m_overflowMenu(nullptr)
    , m_scrollLeftButton(nullptr)
    , m_scrollRightButton(nullptr)
    , m_associativeTab(nullptr)
    , m_scrollTimer()
    , m_title(ARibbonBar::tr_compatible(RibbonUntitledString))
    , m_contextTitle(QString())
    , m_contextGroupName(QString())
    , m_groupsLength(-1)
    , m_pageScrollPos(0)
    , m_groupScrollPos(0)
    , m_scrollPosTarget(0)
    , m_animationStep(0.0)
    , m_timerElapse(0)
    , m_animation(false)
    , m_visibleUpdateCount(0)
{
}

ARibbonPagePrivate::~ARibbonPagePrivate()
{
    A_DELETE_AND_NULL(m_overflowMenu);
}

void ARibbonPagePrivate::init()
{
    A_P(ARibbonPage);
//    p.setAutoFillBackground(false);
    m_overflowMenu = new ARibbonOverflowMenu(&p);
}

void ARibbonPagePrivate::setRibbonBar(ARibbonBar* ribbonBar)
{
    A_P(ARibbonPage);
    m_ribbonBar = ribbonBar;
    if (m_ribbonBar != nullptr)
        p.setParent(ARibbonBarPrivate::_get(m_ribbonBar)->getPageParent());
    else
        p.setParent(nullptr);
}

void ARibbonPagePrivate::setAssociativeTab(ARibbonTab* tab)
{
    if (m_associativeTab == tab)
        return;
    m_associativeTab = tab;
    if (m_associativeTab == nullptr)
        return;

    for (QList<ARibbonGroup*>::const_iterator it = m_groupList.constBegin(); it != m_groupList.constEnd(); ++it)
    {
        if (ARibbonGroup* group = *it)
        {
            QList<QAction*> lstAction = group->actions();
            for (QList<QAction*>::iterator it = lstAction.begin(); it != lstAction.end(); ++it)
            {
                QAction* act = (*it);
                m_shortcutList.append(act);
                m_associativeTab->addAction(act);
            }
        }
    }
}

ARibbonTab* ARibbonPagePrivate::associativeTab() const
{
    return m_associativeTab;
}

bool ARibbonPagePrivate::collapseGroups(int& leftOffset, int actualWidth, ARibbonControlSizeDefinition::GroupSize size, bool adjust)
{
    bool ret = false;
    bool normDir = m_ribbonBar == nullptr || m_ribbonBar->expandDirection() == Qt::RightToLeft;

    QListIterator<ARibbonGroup *> iterator(m_groupList);
    if (normDir)
        iterator.toBack();

    while (normDir ? iterator.hasPrevious() : iterator.hasNext())
    {
        ARibbonGroup* group = normDir ? iterator.previous() : iterator.next();
        if (group->isHidden())
            continue;

        while (group->currentSize() <= size && (adjust || canReduce(group->currentSize())))
        {
            bool adjustNeeded = adjust;
            int oldWidth = group->sizeHint().width();
            if (adjust)
                adjustNeeded = ARibbonGroupPrivate::_get(group)->adjustCurrentSize(false);
            else
                ARibbonGroupPrivate::_get(group)->reduce();

            int newWidth = group->sizeHint().width();
            int delta = oldWidth - newWidth;
            leftOffset -= delta;
            ret = ret || (delta != 0);
            if (leftOffset <= actualWidth)
                return true;
 
            if (!adjustNeeded)
                break;
        }
    }
    return ret;
}

bool ARibbonPagePrivate::expandGroups(int& leftOffset, int actualWidth, ARibbonControlSizeDefinition::GroupSize size, bool adjust)
{
    bool ret = false;
    bool normDir = m_ribbonBar == nullptr || m_ribbonBar->layoutDirection() == Qt::RightToLeft;
    normDir = !normDir; //Reverse for expanding.

    QListIterator<ARibbonGroup *> iterator(m_groupList);
    if (normDir)
        iterator.toBack();

    while (normDir ? iterator.hasPrevious() : iterator.hasNext())
    {
        ARibbonGroup* group = normDir ? iterator.previous() : iterator.next();
        if (group->isHidden())
            continue;
        while (group->currentSize() >= size && (adjust || canExpand(group->currentSize())))
        {
            bool adjustNeeded = adjust;
            int oldWidth = group->sizeHint().width();
            if (adjust)
                adjustNeeded = ARibbonGroupPrivate::_get(group)->adjustCurrentSize(true);
            else
                ARibbonGroupPrivate::_get(group)->expand();

            int newWidth = group->sizeHint().width();
            int delta = oldWidth - newWidth;
            leftOffset -= delta;
            ret = ret || (delta != 0);
            if (leftOffset > actualWidth)
                return true;

            if (!adjustNeeded)
                break;
        }
    }
    return ret;
}

ARibbonControlSizeDefinition::GroupSize ARibbonPagePrivate::getMinGroupSize() const
{
    const bool simplified = m_ribbonBar != nullptr && m_ribbonBar->simplifiedMode();
    Q_UNUSED(simplified);
    ARibbonControlSizeDefinition::GroupSize size = ARibbonControlSizeDefinition::GroupSimplifiedOverflow;
    for (QList<ARibbonGroup*>::const_iterator it = m_groupList.constBegin(); it != m_groupList.constEnd(); ++it)
    {
        ARibbonGroup* group = *it;
        if (group->isHidden())
            continue;
        size = qMin(size, group->currentSize());
/*
        if (simplified)
            size = qMin(size, ARibbonGroupPrivate::_get(group)->minSize());
        if (size == ARibbonControlSizeDefinition::GroupLarge)
            break;
*/
    }
    return size;
}

ARibbonControlSizeDefinition::GroupSize ARibbonPagePrivate::getMaxGroupSize() const
{
    const bool simplified = m_ribbonBar != nullptr && m_ribbonBar->simplifiedMode();
    Q_UNUSED(simplified);
    ARibbonControlSizeDefinition::GroupSize size = ARibbonControlSizeDefinition::GroupLarge;
    for (QList<ARibbonGroup*>::const_iterator it = m_groupList.constBegin(); it != m_groupList.constEnd(); ++it)
    {
        ARibbonGroup* group = *it;
        if (group->isHidden())
            continue;
        size = qMax(size, group->currentSize());
/*
         if (simplified)
            size = qMin(size, ARibbonGroupPrivate::_get(group)->maxSize());
        if (size == ARibbonControlSizeDefinition::GroupPopup)
            break;
*/
    }
    return size;
}

bool ARibbonPagePrivate::canReduce(ARibbonControlSizeDefinition::GroupSize size) const
{
    if(m_ribbonBar && !m_ribbonBar->getAutoReduceMode())
        return false;

    const bool simplified = m_ribbonBar != nullptr && m_ribbonBar->simplifiedMode();
    if (simplified)
        return size < ARibbonControlSizeDefinition::GroupSimplifiedOverflow;
    else
        return size < ARibbonControlSizeDefinition::GroupPopup;
#if 0
    for (QList<ARibbonGroup*>::const_iterator it = m_groupList.constBegin(); it != m_groupList.constEnd(); ++it)
    {
        ARibbonGroup* group = *it;
        if (group->isHidden())
            continue;
        if (ARibbonGroupPrivate::_get(group)->canReduce())
            return true;
    }
    return false;
#endif
}

bool ARibbonPagePrivate::canExpand(ARibbonControlSizeDefinition::GroupSize size) const
{
    if(m_ribbonBar && !m_ribbonBar->getAutoReduceMode())
        return false;

    const bool simplified = m_ribbonBar != nullptr && m_ribbonBar->simplifiedMode();
    if (simplified)
        return size > ARibbonControlSizeDefinition::GroupSimplified;
    else
        return size > ARibbonControlSizeDefinition::GroupLarge;
}

bool ARibbonPagePrivate::isPopupMode() const
{
    A_P(const ARibbonPage);
    return p.ribbonBar() != nullptr && p.ribbonBar()->isMinimized();
}

static ARibbonControlSizeDefinition::GroupSize aproch_size_inc(ARibbonControlSizeDefinition::GroupSize& size)
{
    ARibbonControlSizeDefinition::GroupSize ret = size;
    size = (ARibbonControlSizeDefinition::GroupSize)(size + 1);
    return ret;
}

static ARibbonControlSizeDefinition::GroupSize aproch_size_dec(ARibbonControlSizeDefinition::GroupSize& size)
{
    ARibbonControlSizeDefinition::GroupSize ret = size;
    size = (ARibbonControlSizeDefinition::GroupSize)(size - 1);
    return ret;
}
 
int ARibbonPagePrivate::adjustGroups(int leftOffset, int actualWidth)
{
    ARibbonControlSizeDefinition::GroupSize size = getMaxGroupSize();
    while (leftOffset < actualWidth)
    {
        if (expandGroups(leftOffset, actualWidth, size, true))
            continue;
        if (!canExpand(size))
            break;
        if (expandGroups(leftOffset, actualWidth, aproch_size_dec(size), false))
            continue;
    }

    if (leftOffset > actualWidth)
    {
        size = getMinGroupSize();
        while (leftOffset > actualWidth)
        {
            if (collapseGroups(leftOffset, actualWidth, size, true))
                continue;
            if (!canReduce(size))
                break;
            if (collapseGroups(leftOffset, actualWidth, aproch_size_inc(size), false))
                continue;
        }
    }
    return leftOffset;
}

int ARibbonPagePrivate::calculateGroupsWidth()
{
    int width = 0;
    for (int i = 0, count = m_groupList.size(); i < count; ++i)
    {
        ARibbonGroup* group = m_groupList.at(i);
        if (group->isHidden())
            continue;
        width += group->sizeHint().width();
    }
    return width;
}

void ARibbonPagePrivate::updateLayout(bool updateScroll)
{
    A_P(ARibbonPage);
    int leftOffset = 0;
    int topOffset = 0;
    int actualWidth = p.width();
    QMargins pageMargins, groupMargins;
    int groupTitleHeight = 0;
    m_groupsLength = 0;
 
    if (ARibbonBar* ribbonBar = p.ribbonBar())
    {
        pageMargins = ARibbonBarPrivate::_get(ribbonBar)->pageContentMargins();
        groupMargins = ARibbonBarPrivate::_get(ribbonBar)->groupContentMargins();
        leftOffset = pageMargins.left();
        topOffset = pageMargins.top();
        actualWidth -= (pageMargins.left() + pageMargins.right());
        groupTitleHeight = ARibbonBarPrivate::_get(ribbonBar)->groupTitleHeight();
    }

    bool isMinimized = actualWidth < minRibbonWidth;
    if (!isMinimized)
    {
        // First of all, we need to calculate the size of the group's content with a valid parent, 
        // so that the controls receive the font and palette settings for the correct metrics.
        int gropsWidth = calculateGroupsWidth();
        adjustGroups(gropsWidth, actualWidth);
    }

    m_overflowMenu->clearGroups();
    for (QList<ARibbonGroup*>::const_iterator it = m_groupList.constBegin(); it != m_groupList.constEnd(); ++it)
    {
        ARibbonGroup* group = *(it);
        if (group->isHidden())
            continue;

        ARibbonGroupPrivate* group_private = ARibbonGroupPrivate::_get(group);
        QWidget* parentWidget = group_private->parentForControls();
        QSize contentSize = group_private->contentSizeHint();
        QSize groupSizeHint = group->sizeHint();
        QRect rect = QRect(QPoint(-m_groupScrollPos + leftOffset, topOffset),
            QSize(groupSizeHint.width(), p.size().height() - pageMargins.top() - pageMargins.bottom()));
        group_private->reposition(QStyle::visualRect(p.layoutDirection(), p.geometry(), rect));
        leftOffset += rect.width();

        QRect groupRect = QRect(QPoint(0, 0), groupSizeHint);
        if (group->isReduced())
            groupRect.setSize(parentWidget->sizeHint());

        groupRect.adjust(groupMargins.left(), groupMargins.top(),
            -groupMargins.right(), -(groupMargins.bottom() + groupTitleHeight));

        QRect contentRect = QStyle::alignedRect(Qt::LeftToRight, group->controlsAlignment(), contentSize, groupRect);
        group_private->updateControlsLayout(parentWidget, contentRect.left(), contentRect.top());
        group_private->updateOptionButtonLayout(parentWidget);

        if (group_private->hasOverflowContent())
            m_overflowMenu->addGroup(group);

        m_groupsLength = leftOffset;
    }

    if (updateScroll)
        showGroupScroll(false);
}

/*! \internal */
void ARibbonPagePrivate::removeGroup(int index, bool deleteGroup)
{
    if (validateGroupIndex(index)) 
    {
        ARibbonGroup* group = m_groupList.at(index);

        group->removeEventFilter(this);
        QList<QAction*> actList = group->actions();

        for (int i = actList.size()-1; i >= 0; --i) 
            m_shortcutList.removeOne(actList.at(i));

        m_groupList.removeAt(index);

        if (deleteGroup)
            delete group;
        else
            group->setParent(nullptr);

        updateLayout();
    }
    else
        Q_ASSERT(false);
}

/*! \internal */
int ARibbonPagePrivate::groupIndex(ARibbonGroup* group) const
{
    return m_groupList.indexOf(group);
}

/*! \internal */
int ARibbonPagePrivate::groupsLength() const
{
    return m_groupsLength;
}

/*! \internal */
int ARibbonPagePrivate::calcReducedGroupsWidth() const
{
    int result = m_groupsLength;
    for (QList<ARibbonGroup*>::const_iterator it = m_groupList.constBegin(); it != m_groupList.constEnd(); ++it)
    {
        ARibbonGroup* group = *(it);
        if (group->isHidden())
            continue;

        if (canReduce(group->currentSize()))
        {
            result = 0;
            break;
        }
    }

    return result;
}

void ARibbonPagePrivate::setScrollButtonsVisible(bool scrollLeft, bool scrollRight)
{
    A_P(ARibbonPage);
    if (m_scrollLeftButton == nullptr && scrollLeft)
    {
        m_scrollLeftButton = new ARibbonGroupScrollButton(&p, true);
        QObject::connect(m_scrollLeftButton, SIGNAL(pressed()), this, SLOT(pressLeftScrollButton()));
        QObject::connect(m_scrollLeftButton, SIGNAL(stopScrollTimer()), this, SLOT(forcedStopScrollTimer()));
    }

    if (m_scrollRightButton == nullptr && scrollRight)
    {
        m_scrollRightButton = new ARibbonGroupScrollButton(&p, false);
        QObject::connect(m_scrollRightButton, SIGNAL(pressed()), this, SLOT(pressRightScrollButton()));
        QObject::connect(m_scrollRightButton, SIGNAL(stopScrollTimer()), this, SLOT(forcedStopScrollTimer()));
    }

    const int buttonWidth = qMax(ACommonStylePrivate::dpiScaled(widthButtonScroll, &p), 
        p.style()->pixelMetric(QStyle::PM_MenuButtonIndicator, nullptr, &p));

    QRect rcPage(p.geometry());

    if (scrollLeft)
    {
        QRect rc(QPoint(0, 0), QSize(buttonWidth, rcPage.height()));

        if (isPopupMode())
            rc.setHeight(rc.height() + pageMarginPopup);


        m_scrollLeftButton->setGeometry(rc);
        m_scrollLeftButton->raise();
        m_scrollLeftButton->setVisible(true);
    }
    else if (m_scrollLeftButton != nullptr)
    {
        m_scrollLeftButton->setDown(false);
        m_scrollLeftButton->setVisible(false);
    }

    if (scrollRight)
    {
        QRect rc(QPoint(rcPage.width() - buttonWidth, 0), QSize(buttonWidth, rcPage.height()));

        if (isPopupMode())
            rc.setHeight(rc.height() + pageMarginPopup);

        m_scrollRightButton->setGeometry(rc);
        m_scrollRightButton->raise();
        m_scrollRightButton->setVisible(true);

    }
    else if (m_scrollRightButton != nullptr)
    {
        m_scrollRightButton->setDown(false);
        m_scrollRightButton->setVisible(false);
    }

    if (!scrollLeft && !scrollRight)
        m_pageScrollPos = 0;
}

void ARibbonPagePrivate::showGroupScroll(bool onlyCalc)
{
    A_P(ARibbonPage);
    int totalWidth = calcReducedGroupsWidth();
    if (totalWidth == 0)
    {
        if (!onlyCalc)
            setScrollButtonsVisible(false, false);
        return;
    }

    int groupsLength = p.width();
    int scrollPos = m_groupScrollPos;

    if (totalWidth > groupsLength)
    {
        if (scrollPos > totalWidth - groupsLength)
            scrollPos = totalWidth - groupsLength;
    }
    else
        scrollPos = 0;

    if (scrollPos < 0) 
        scrollPos = 0;

    m_groupScrollPos = scrollPos;
    if (!onlyCalc)
        setScrollButtonsVisible(scrollPos > 0, totalWidth - groupsLength - scrollPos > 0);
}

void ARibbonPagePrivate::scrollGroupAnimate()
{
    if (qAbs(m_groupScrollPos - m_scrollPosTarget) > qFabs(m_animationStep))
    {
        m_groupScrollPos = int((double)m_groupScrollPos + m_animationStep);
        updateLayout(false);
    }
    else
    {
        m_animation = false;
        m_groupScrollPos = m_scrollPosTarget;
        m_timerElapse = 0;
        m_scrollTimer.stop();
        showGroupScroll(false);
    }
}

void ARibbonPagePrivate::startScrollGropsAnimate(int groupScrollPos, int scrollPosTarget)
{
    A_P(ARibbonPage);
    m_animation = true;
    m_scrollPosTarget = scrollPosTarget;
    m_groupScrollPos = groupScrollPos;
    m_timerElapse = QApplication::doubleClickInterval() * 4 / 5;
    m_animationStep = double(m_scrollPosTarget - m_groupScrollPos) / (m_timerElapse > 200 ? 8.0 : 3.0);

    if (m_animationStep > 0 && m_animationStep < 1) m_animationStep = 1;
    if (m_animationStep < 0 && m_animationStep > -1) m_animationStep = -1;

    int nms = 40;
    m_scrollTimer.start(nms, &p);
    scrollGroupAnimate();
}

bool ARibbonPagePrivate::simplifiedMode() const
{
    return m_ribbonBar != nullptr && m_ribbonBar->simplifiedMode();
}

bool ARibbonPagePrivate::isOverflowMenuIsEmpty() const
{
    return m_overflowMenu->isEmpty();
}

static void listPageWidth(int totalWidth, int realWidth, QList<int>& pagesWidth) 
{
    if (totalWidth > realWidth)
    {
        pagesWidth << realWidth;
        listPageWidth(totalWidth - realWidth, realWidth, pagesWidth);
    }
    else
        pagesWidth << totalWidth;
    return;
}

void ARibbonPagePrivate::pressLeftScrollButton()
{
    A_P(ARibbonPage);
    QList<int> pagesWidth;
    listPageWidth(calcReducedGroupsWidth(), p.width(), pagesWidth);

    if (0 < m_pageScrollPos)
        m_pageScrollPos--; 

    int scrollPos = m_groupScrollPos;
    m_groupScrollPos -= pagesWidth[m_pageScrollPos];
    showGroupScroll(true);
    startScrollGropsAnimate(scrollPos, m_groupScrollPos);
}

void ARibbonPagePrivate::pressRightScrollButton()
{
    A_P(ARibbonPage);
    QList<int> pagesWidth;
    listPageWidth(calcReducedGroupsWidth(), p.width(), pagesWidth);

    m_groupScrollPos += pagesWidth[m_pageScrollPos];
    showGroupScroll(true);

    if (pagesWidth.size() - 1 > m_pageScrollPos)
        m_pageScrollPos++; 

    startScrollGropsAnimate(0, m_groupScrollPos);
}

void ARibbonPagePrivate::forcedStopScrollTimer()
{
    m_scrollTimer.stop();
    showGroupScroll(false);
}

/*! \reimp */
bool ARibbonPagePrivate::eventFilter(QObject* obj, QEvent* event)
{
    bool res = QObject::eventFilter(obj, event);

    if (!qobject_cast<ARibbonGroup*>(obj))
        return res;

    switch (event->type()) 
    {
        case QEvent::ActionAdded: 
            if (QActionEvent* actEvent = static_cast<QActionEvent*>(event))
            {
                m_shortcutList.append(actEvent->action());
                if (m_associativeTab)
                    m_associativeTab->addAction(actEvent->action());
            }
            break;
        case QEvent::ActionRemoved: 
            if (QActionEvent* actEvent = static_cast<QActionEvent*>(event))
            {
                if (m_associativeTab)
                    m_associativeTab->removeAction(actEvent->action());

                int index = m_shortcutList.indexOf(actEvent->action());
                if (index != -1)
                    m_shortcutList.removeAt(index);
            }
            break;
        default:
            break;
    }

    return res;
}

/*!
\class ARibbonPage
\inmodule QtitanRibbon
\brief ARibbonPage class implements the Ribbon UI Page. Used to place objects of type - ARibbonGroup.
*/

/*!
\fn void ARibbonPage::titleChanged(const QString& title);
The signal is emitted after the page title has been changed.
*/

/*!
\fn void ARibbonPage::activated();
The signal is emitted then current page has been activated.
*/

/*!
\fn void ARibbonPage::activating(bool& allow);
The signal is emitted then current page is activating. Parameter \a allow allows you to prevent switching to this page.
*/

/*!
Constructs ARibbonPage with the given \a parent.
*/
ARibbonPage::ARibbonPage(QWidget* parent)
    : QWidget(parent)
{
    A_INIT_PRIVATE(ARibbonPage);
    A_D(ARibbonPage);
    d.init();
}

#if 0
/*!
Constructs ARibbonPage with the given \a ribbonBar and \a title.
*/
ARibbonPage::ARibbonPage(ARibbonBar* ribbonBar, const QString& title)
    : QWidget(ARibbonBarPrivate::_get(ribbonBar)->material())
{
    A_INIT_PRIVATE(ARibbonPage);
    A_D(ARibbonPage);
    d.init();
    d.m_ribbonBar = ribbonBar;
    setTitle(title);
}
#endif

/*!
The ARibbonPage object's destructor.
*/
ARibbonPage::~ARibbonPage()
{
    if (ARibbonBar* _ribbonBar = ribbonBar())
        _ribbonBar->detachPage(this);
    A_DELETE_PRIVATE();
}

/*!
Returns the ribbon bar that holds this page.
*/
ARibbonBar* ARibbonPage::ribbonBar() const
{
    A_D(const ARibbonPage);
    return d.m_ribbonBar;
}

/*!
Overloaded method. Returns true if page is visible in the ribbon bar.
*/
bool ARibbonPage::isVisible() const
{
    A_D(const ARibbonPage);
    if (d.m_associativeTab != nullptr)
        return !d.m_associativeTab->isHidden();
    else
        return QWidget::isVisible();
}

/*! \reimp */
void ARibbonPage::setVisible(bool visible)
{
    A_D(ARibbonPage);
    if (d.m_associativeTab != nullptr)
        d.m_associativeTab->update();

    bool guard = d.m_visibleUpdateCount > 0;
    if (guard)
    {
        QWidget::setVisible(visible);
        return;
    }
    
    if (d.m_associativeTab != nullptr)
    {
        int index = d.m_associativeTab->tabBar()->tabIndex(d.m_associativeTab);
        const bool tabVisible = d.m_associativeTab->tabBar()->isTabVisible(index);
        if (tabVisible != visible)
        {
            ARibbonBarAutoUpdater autoUpdater(ribbonBar());
            d.m_associativeTab->tabBar()->setTabVisible(index, visible);
        }
        visible = visible && d.m_associativeTab->isCurrent();
    }
    QWidget::setVisible(visible);
}

/*!
Adds existing ribbon group to the ribbon page.
*/
void ARibbonPage::addGroup(ARibbonGroup* group)
{
    insertGroup(-1, group);
}

/*!
Creates and adds the ribbon group with given \a title to the ribbon page.
*/
ARibbonGroup* ARibbonPage::addGroup(const QString& title)
{
    return insertGroup(-1, title);
}

/*!
Adds group with given \a icon and \a title to the ribbon page.
*/
ARibbonGroup* ARibbonPage::addGroup(const QIcon& icon, const QString& title)
{
    return insertGroup(-1, icon, title);
}

/*!
Inserts an existing ribbon \a group with to the position \a index.
*/
void ARibbonPage::insertGroup(int index, ARibbonGroup* group)
{
    A_D(ARibbonPage);
    ARibbonBarAutoUpdater autoUpdater(ribbonBar());
    group->setParent(this);
    if (!d.validateGroupIndex(index)) 
    {
        index = d.m_groupList.count();
        d.m_groupList.append(group);
    } 
    else 
        d.m_groupList.insert(index, group);
    connect(group, SIGNAL(actionTriggered(QAction*)), this, SLOT(actionTriggered(QAction*)), Qt::QueuedConnection);
    connect(group, SIGNAL(released()), this, SLOT(released()));
    group->show();
}

/*!
Creates a new ribbon group with the given \a title and inserts it to the position \a index. Returns a pointer to the new object ARibbonGroup.
*/
ARibbonGroup* ARibbonPage::insertGroup(int index, const QString& title)
{
    ARibbonGroup* group = new ARibbonGroup(nullptr, title);
    insertGroup(index, group);
    return group;
}

/*!
Creates a new ribbon group with the given \a title, \a icon and inserts it to the position \a index. Returns a pointer to the new object ARibbonGroup.
*/
ARibbonGroup* ARibbonPage::insertGroup(int index, const QIcon& icon, const QString& title)
{
    ARibbonBarAutoUpdater autoUpdater(ribbonBar());

    if (ARibbonGroup* group = insertGroup(index, title))
    {
        group->setIcon(icon);
        return group;
    }
    return nullptr;
}

/*!
Removes an existing ribbon \a group.
*/
void ARibbonPage::removeGroup(ARibbonGroup* group)
{
    A_D(ARibbonPage);
    removeGroupByIndex(d.groupIndex(group));
}

/*!
Removes an existing ribbon group in the position \a index.
*/
void ARibbonPage::removeGroupByIndex(int index)
{
    A_D(ARibbonPage);
    d.removeGroup(index, true);
}

void ARibbonPage::detachGroup(ARibbonGroup* group)
{
    A_D(ARibbonPage);
    detachGroupByIndex(d.groupIndex(group));
}

void ARibbonPage::detachGroupByIndex(int index)
{
    A_D(ARibbonPage);
    d.removeGroup(index, false);
}

/*!
Removes all groups from this ribbon page.
*/
void ARibbonPage::clearGroups()
{
    A_D(ARibbonPage);
    for (int i = (int)d.m_groupList.count() - 1; i >= 0; i--)
        removeGroupByIndex(i);
}

QAction* ARibbonPage::defaultAction() const
{
    A_D(const ARibbonPage);
    if (d.m_associativeTab != nullptr)
        return d.m_associativeTab->defaultAction();
    return nullptr;
}

/*!
Returns ribbon group count in the ribbon page.
*/
int ARibbonPage::groupCount() const
{
    A_D(const ARibbonPage);
    return d.m_groupList.count();
}

/*!
Returns ribbon group by \a index.
*/
ARibbonGroup* ARibbonPage::group(int index) const
{
    A_D(const ARibbonPage);
    if (index < 0 || index >= d.m_groupList.size())
        return nullptr;
    return d.m_groupList[index];
}

/*!
Returns ribbon group index by \a ARibbonGroup object.
*/
int ARibbonPage::groupIndex(ARibbonGroup* group) const
{
    A_D(const ARibbonPage);
    Q_ASSERT(group != nullptr);
    if (group && d.m_groupList.contains(group))
        return d.m_groupList.indexOf(group);
    return -1;
}

QList<ARibbonGroup*> ARibbonPage::groups() const
{
    A_D(const ARibbonPage);
    return d.m_groupList;
}

/*!
Sets the predefined \a color for the contextual page.
 */ 
void ARibbonPage::setContextColor(EContextColor color)
{
    setContextColor(ACommonStyle::contextColorToColor(color));
}

/*!
Sets the \a color for the contextual page.
 */
void ARibbonPage::setContextColor(const QColor& color)
{
    A_D(ARibbonPage);
    d.m_contextColor = color;
    if (d.m_associativeTab != nullptr)
    {
        if (ARibbonBar* ribbonBar = this->ribbonBar())
        {
            QWidget* widget = ARibbonBarPrivate::_get(ribbonBar)->m_ribbonTitleBarWidget;
            if (widget != nullptr)
                widget->update();
            ribbonBar->update();
        }
    }
}

/*!
Returns the predefined color for the contextual page.
 */ 
const QColor& ARibbonPage::contextColor() const
{
    A_D(const ARibbonPage);
    return d.m_contextColor;
}

/*!
Sets the page title to the specified value.
*/
void ARibbonPage::setTitle(const QString& title)
{
    A_D(ARibbonPage);
    if (d.m_title == title)
        return;

    d.m_title = title;
    if (d.m_associativeTab != nullptr)
        d.m_associativeTab->setText(d.m_title);
    emit titleChanged(d.m_title);
}

/*!
Returns the ribbon page title.
\sa ARibbonPage::contextTitle
*/
const QString& ARibbonPage::title() const
{
    A_D(const ARibbonPage);
    return d.m_title;
}

void ARibbonPage::setContextTitle(const QString& title)
{
    A_D(ARibbonPage);
    d.m_contextTitle = title;
}

/*!
\property ARibbonPage::contextTitle
Sets the context title of the page. Context title is located on main window frame if page is contextual.
\sa ARibbonPage::title
\inmodule QtitanRibbon
*/
const QString& ARibbonPage::contextTitle() const
{
    A_D(const ARibbonPage);
    return d.m_contextTitle;
}

const QString& ARibbonPage::contextGroupName() const
{
    A_D(const ARibbonPage);
    return d.m_contextGroupName;
}

void ARibbonPage::released()
{
    ARibbonBar* _ribbonBar = ribbonBar();
    if (_ribbonBar == nullptr)
        return;

    if (_ribbonBar->isMinimized() && isVisible() && QApplication::activePopupWidget() == this)
        hide();
}

void ARibbonPage::actionTriggered(QAction* action)
{
    A_D(ARibbonPage);
    ARibbonBar* _ribbonBar = ribbonBar();
    if (_ribbonBar == nullptr)
        return;
    if (d.m_shortcutList.indexOf(action) != -1)
        return;
    if (_ribbonBar->isMinimized() && isVisible())
    {
        if (action->menu())
            return;
        if (ARibbonPagePopup* popup = qobject_cast<ARibbonPagePopup*>(parentWidget()))
            popup->close();
    }
}

void ARibbonPage::setContextGroupName(const QString& groupName)
{
    A_D(ARibbonPage);
    d.m_contextGroupName = groupName;
}

void ARibbonPage::updateLayout()
{
    A_D(ARibbonPage);
    d.updateLayout();
}

/*! \reimp */
bool ARibbonPage::event(QEvent* event)
{
    A_D(ARibbonPage);
    switch(event->type())
    {
        case QEvent::Timer:
            {
                QTimerEvent* timerEvent = static_cast<QTimerEvent*>(event);
                if (d.m_scrollTimer.timerId() == timerEvent->timerId()) 
                {
                    d.scrollGroupAnimate();
                    event->accept();
                    return true;
                }
            }
            break;
        case QEvent::WinIdChange:
            break;
        case QEvent::ChildAdded :
            {
                QChildEvent* childEvent = static_cast<QChildEvent*>(event);
                if (ARibbonGroup* group = qobject_cast<ARibbonGroup*>(childEvent->child()))
                    group->installEventFilter(&d);

                if (childEvent->added())
                {
                    QWidget* widget = qobject_cast<QWidget *>(childEvent->child());
                    if (widget != nullptr)
                    {
                    }
                }
            }
            break;
        case QEvent::ChildRemoved :
            {
                QChildEvent* childEvent = static_cast<QChildEvent*>(event);
                if (ARibbonGroup* group = qobject_cast<ARibbonGroup*>(childEvent->child()))
                    group->removeEventFilter(&d);
            }
            break;
        default:
            break;
    }

    return QWidget::event(event);
}

/*! \reimp */
QSize ARibbonPage::sizeHint() const
{
    A_D(const ARibbonPage);
    QSize resultSize = QWidget::sizeHint();
    // Calculate a max height of the group. Group may contain a different number of rows(3 - rows, 5 - rows)
    int maxHeight = 0;
    for (int i = 0, count = groupCount(); count > i; i++)
    {
        if (ARibbonGroup* _group = group(i))
            maxHeight = qMax(maxHeight, _group->sizeHint().height());
    }
    resultSize.setHeight(maxHeight);

    if (d.isPopupMode())
        resultSize.setHeight(resultSize.height() - pageMarginPopup);

    return resultSize;
}

/*! \reimp */
void ARibbonPage::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
#if 0
    QPainter p(this);
    p.setPen(Qt::red);
    p.drawRect(rect().adjusted(0, 0, -1, -1));
#endif
}

/*! \reimp */
void ARibbonPage::changeEvent(QEvent* event)
{
    switch (event->type()) 
    {
        case QEvent::StyleChange:
        case QEvent::FontChange: 
            {
                //updateLayout();
            }
            break;
        default:
            break;
    };
    return QWidget::changeEvent(event);
}

/*! \reimp */
void ARibbonPage::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
}

void APROCH_PRE_NAMESPACE(aproch_set_page_visible)(ARibbonPage* page, bool visible)
{
    ARibbonPageVisibleAutoUpdater autoUpdater(ARibbonPagePrivate::_get(page));
    page->setVisible(visible);
}

APROCH_NAMESPACE_END