/****************************************************************************
 * @file    ARibbonBar_p.h
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

#include <QStack>
#include <QLabel>
#include <QPointer>
#include <QPropertyAnimation>

#include "Widget/AMaterialWidget.h"
#include "ARibbonPopup.h"
#include "ARibbonTabBar.h"
#include "ARibbonCustomizePage.h"
#include "ARibbonQuickAccessBar.h"
#include "ARibbonTitleBarWidget.h"
#include "ARibbonSystemMenu.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

#define _aproch_Index                  "Index"
#define _aproch_Wrap                   "Wrap"
#define _aproch_Wrap_Group             "WrapGroup"

#define _aproch_Begin_Group            "BeginToolBarGroup"
#define _aproch_Group                  "ToolBarGroup"
#define _aproch_End_Group              "EndToolBarGroup"

#define _aproch_Index_Group            "IndexGroup"
#define _aproch_Switch_Count           "SwitchCount"
#define _aproch_KeyTip                 "aproch::KeyTip"

class ARibbonBarBanner : public QLabel
{
public:
    explicit ARibbonBarBanner(QWidget* parent);
public:
    Qt::AlignmentFlag alignment() const  { return m_alignment; }
    void setAlignment(Qt::AlignmentFlag alignment) { m_alignment = alignment; }
protected:
    virtual void paintEvent(QPaintEvent* p);
private:
    Qt::AlignmentFlag m_alignment;
};

class ATitleBarStyleOption;
class ARibbonBackstageCloseButton;
class AMenuMinimizedGroups;
class ARibbonKeyTip;
class ARibbonCustomizeManager;

class ARibbonToolTipManager : public QObject
{
public:
    explicit ARibbonToolTipManager(QObject* parent);
    virtual ~ARibbonToolTipManager();
protected:
    bool showToolTip(const QPoint& pos, QWidget* w);
protected:
    virtual bool eventFilter(QObject*, QEvent* event);
};

/* ARibbonBarView */
class ARibbonBarView : public AMaterialWidget
{
public:
    ARibbonBarView(ARibbonBar* ribbonBar);
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    ARibbonBar* m_ribbonBar;
};

/* ARibbonBarPrivate */
class ARibbonBarPrivate : public QObject
{
public:
    Q_OBJECT;
    A_DECLARE_PUBLIC(ARibbonBar);

public:
    explicit ARibbonBarPrivate();
    virtual ~ARibbonBarPrivate();
    inline static ARibbonBarPrivate* _get(ARibbonBar* ribbon)
    {
        return &ribbon->aproch_d();
    }
    inline static const ARibbonBarPrivate* _get(const ARibbonBar* ribbon)
    {
        return &ribbon->aproch_d();
    }
    static ARibbonBar* findRibbonBar(const QWidget* widget);

public:
    void init();
    void setMetricsDirty();
    void updateLayout();
    void layoutCorner();
    void createRibbonTitleBarWidget();
    void deleteRibbonTitleBarWidget();
    void setBackstageViewVisible(bool visible);
    void updateQuickAccessBarPosition();

    QAction* createSystemButton(const QIcon& icon, const QString& text);

    void insertPage(int index, ARibbonPage* page);
    void removePage(int index, bool deletePage = true);
    int pageIndex(ARibbonPage* page) const;

    Q_ALWAYS_INLINE bool isIndexValid(int index) const;
    Q_ALWAYS_INLINE int tabBarHeight() const;
    int backstageViewTop() const;

    QMenu* createContextMenu();
    void setKeyTipsActive(bool active);
    bool isKeyTipsActive() const {
        return m_keyTipsActive;
    }
    void showKeyTips(QWidget* w);
    void hideKeyTips();
    void createKeyTips();
    void createGroupKeyTips();
    void createWidgetKeyTips(ARibbonGroup* group, QWidget* widget, const QString& prefix, const QRect& rcGroups, bool visible);
    bool createPopupKeyTips(QMenu* menu);
    void calcPositionKeyTips();
    int rowItemHeight() const;
    int rowItemCount() const;
    void setRowItemCount(const int& nCount);
    void createCustomizeDialog();
    void showPagePopup(ARibbonPage* page);
    void hidePagePopup();
    bool isPagePopupVisible() const;
    void preparePages();
    QWidget* getPageParent();
    AWindowTitleBar* findTitleBar() const;
    bool isUpdating() const;
    QMargins pageContentMargins() const;
    QMargins groupContentMargins() const;
    AMaterialWidget* material() const;
    int extendedTitleBarHeight() const;
    int groupTitleHeight() const;
    QFont getPageFont() const;

public Q_SLOTS:
    void toggledCustomizeBar();
    void toggledQuickAccessBarPos();
    void toggledMinimized();
    void toggledSimplifiedMode(bool);
    void currentChanged(int index);
    void currentAboutToBeChanged(int index, bool& changed);
    void activePageAnimationFinished();
    void activePageAnimationValueChanged(const QVariant&);

protected:
    bool isTitleBarCustomizationSupported() const;
    void changed(bool dirty = false);
    void updateMetrics();
    int calcTabBarHeight() const;
    int calcGroupTitleHeight() const;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    void updateTitleBar();
#endif
    void updateFont(ARibbonPage* page) const;
    void updateLayoutRects();
    bool updateOverflowButton();
    void updateTitleBarWidget();
    void updateActivePageLayout(ARibbonPage* activePage, bool animated);
    bool pressTipCharEvent(const QKeyEvent* key);
    virtual bool eventFilter(QObject*, QEvent* event) override;

private:
    bool m_metricsDirty;
    QMargins m_pageContentMargins;
    QMargins m_groupContentMargins;
    QPropertyAnimation* m_activePageAnimation;
    ARibbonBarView* m_view;

    int m_ribbonItemHeight;
    int m_groupTitleHeight;

    QRect m_tabBarRect;
    QRect m_contextualTabBarRect;
    QRect m_pageContentRect;
    QRect m_pageWindowRect;

    QRect m_bannerRect;
    QRect m_overflowButtonRect;
    QRect m_sysButtonRect;
    QRect m_quickBarRect;
    QSize m_sizeHint;

    QRect getPageFrameRect() const;

public:
    ARibbonToolTipManager* m_ribbonToolTipManager;
    ARibbonTitleBarWidget* m_ribbonTitleBarWidget;
    int m_nQucikAccessBarIndex = -1;
    QAction* m_simplifiedAction;
    ARibbonTabBar* m_ribbonTabBar;
    ARibbonPagePopup* m_pagePopup;
    ARibbonQuickAccessBar* m_quickAccessBar; 
    ARibbonSystemButton* m_systemButton;
    ARibbonBarBanner* m_banner;
    ARibbonOverflowMenuButton* m_overflowMenuButton;
    QVector<QWidget*> m_visibleWidgets;
    QVector<QAction*> m_visibleActions;

    bool m_titleBarCustomized;
    bool m_saveVisibleSysMenu;
    bool m_contextualTabsVisible;
    bool m_minimizationEnabled;
    bool m_simplifiedMode;
    Qt::LayoutDirection m_expandDirection;
    bool m_minimized;
    bool m_keyTipsActive;
    bool m_keyTipsReadyToActivate;
    bool m_keyTipsComplement;
    bool m_keyTipsEnabled;
    bool m_titleGroupsVisible;
    bool m_bAutoReduce = true;
	bool m_bShowARibbonBarView = true;
    QPointer<QWidget> m_keyboardFocusWidget;
    ARibbonBar::BarPosition m_quickAccessBarPosition;
    ARibbonBar::BarPosition m_tabBarPosition;

    QList<ARibbonKeyTip*> m_keyTips;
    QStack<QWidget*> m_levels;
    int m_countKey;

    QStyle::SubControl m_hoveredSubControl;
    QStyle::SubControl m_activeSubControl;

    QPixmap m_titleBackground;
    ARibbonCustomizeManager* m_customizeManager;
    ARibbonCustomizeDialog* m_customizeDialog;

protected:
    int m_rowItemCount;
    int m_updateLayoutCount;
    bool m_destoying;
    QFrame* m_SysBtnSplitLine;
    QFrame* m_QuckAcessSplitLine;

private:
    Q_DISABLE_COPY(ARibbonBarPrivate);
};

Q_ALWAYS_INLINE bool ARibbonBarPrivate::isIndexValid(int index) const
{
    return m_ribbonTabBar->isIndexValid(index);
}

Q_ALWAYS_INLINE int ARibbonBarPrivate::tabBarHeight() const
{
    return m_ribbonTabBar->height();
}

template<class T>
const T* aproch_getParentWidget(const QWidget* widget)
{
    while (widget)
    {
        if (const T* parent = qobject_cast<const T*>(widget))
            return parent;
        widget = widget->parentWidget();
    }
    return nullptr;
}

template<class T>
static QWidget* aproch_getPrevParentWidget(QWidget* pWidget)
{
    if (qobject_cast<T*>(pWidget))
        return nullptr;

    QWidget* pPrevWidget = pWidget;
    while (pWidget)
    {
        pWidget = pWidget->parentWidget();
        if (qobject_cast<T*>(pWidget))
            return pPrevWidget;
        else
            pPrevWidget = pWidget;
    }
    return nullptr;
}

APROCH_NAMESPACE_END