/****************************************************************************
 * @file    ARibbonPage_p.h
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
#pragma once
#include <QBasicTimer>

#include "ARibbonPopup.h"
#include "ARibbonTabBar.h"
#include "ARibbonControls.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

/* ARibbonPagePrivate */
class ARibbonPagePrivate : public QObject
{
    Q_OBJECT;
    A_DECLARE_PUBLIC(ARibbonPage);
public:
    explicit ARibbonPagePrivate();
    virtual ~ARibbonPagePrivate();
public:
    inline static ARibbonPagePrivate* _get(ARibbonPage* page)
    { return &page->aproch_d(); }
    inline static const ARibbonPagePrivate* _get(const ARibbonPage* page)
    { return &page->aproch_d(); }
public:
    void init();
    void setRibbonBar(ARibbonBar* ribbonBar);
    void setAssociativeTab(ARibbonTab* tab);
    ARibbonTab* associativeTab() const;
    bool validateGroupIndex(int index) const { return index >= 0 && index < m_groupList.count(); }
    void removeGroup(int index, bool deleteGroup);
    int groupIndex(ARibbonGroup* page) const;

    int groupsLength() const;
    int calcReducedGroupsWidth() const;
    void setScrollButtonsVisible(bool scrollLeft, bool scrollRight);
    void showGroupScroll(bool onlyCalc);
    void scrollGroupAnimate();
    void startScrollGropsAnimate(int groupScrollPos, int scrollPosTarget);
    bool simplifiedMode() const;
    bool isOverflowMenuIsEmpty() const;
public:
    virtual bool eventFilter(QObject*, QEvent* event);
public Q_SLOTS:
    void pressLeftScrollButton();
    void pressRightScrollButton();
    void forcedStopScrollTimer();
public:
    void updateLayout(bool updateScroll = true);
    int calculateGroupsWidth();
    int adjustGroups(int leftOffset, int actualWidth);
    bool collapseGroups(int& leftOffset, int actualWidth, ARibbonControlSizeDefinition::GroupSize size, bool adjust);
    bool expandGroups(int& leftOffset, int actualWidth, ARibbonControlSizeDefinition::GroupSize size, bool adjust);
    ARibbonControlSizeDefinition::GroupSize getMinGroupSize() const;
    ARibbonControlSizeDefinition::GroupSize getMaxGroupSize() const;
    bool canReduce(ARibbonControlSizeDefinition::GroupSize size) const;
    bool canExpand(ARibbonControlSizeDefinition::GroupSize size) const;
    bool isPopupMode() const;
public:
    ARibbonBar* m_ribbonBar;
    ARibbonOverflowMenu* m_overflowMenu;
    ARibbonGroupScrollButton* m_scrollLeftButton;
    ARibbonGroupScrollButton* m_scrollRightButton;
    QList<ARibbonGroup*> m_groupList;
    QList<QAction*> m_shortcutList;
    ARibbonTab* m_associativeTab;
    QBasicTimer m_scrollTimer;
    QString m_title;
    QString m_contextTitle;
    QString m_contextGroupName;
    QColor m_contextColor;

    int m_groupsLength;
    int m_pageScrollPos;
    int m_groupScrollPos;
    int m_scrollPosTarget;
    double m_animationStep;
    int m_timerElapse;
    bool m_animation;
    int m_visibleUpdateCount;
private:
    Q_DISABLE_COPY(ARibbonPagePrivate)
};

class ARibbonPageVisibleAutoUpdater
{
public:
    ARibbonPageVisibleAutoUpdater(ARibbonPagePrivate* page)
        : m_page(page)
    {
        m_page->m_visibleUpdateCount++;
    }

    ~ARibbonPageVisibleAutoUpdater()
    {
        m_page->m_visibleUpdateCount--;
    }
private:
    ARibbonPagePrivate* m_page;
};

void aproch_set_page_visible(ARibbonPage* page, bool visible);

APROCH_NAMESPACE_END