/****************************************************************************
 * @file    AAbstractTabBar.h
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
#pragma once
#include <QPainter>
#include <QToolButton>
#include <QBoxLayout>
#include <QStyleOption>

#include "Style/AStyleOption.h"
#include "Style/ASegoeMDL2Assets.h"
#include "AScrollWidgetBar.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class AAbstractTab;
class AScrollContainer;
class AMovableTabWidget;
class APROCH_API AAbstractTabBar : public QWidget
{
    Q_OBJECT
#if 0
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentChanged)
    Q_PROPERTY(SelectionBehavior selectionBehaviorOnRemove READ selectionBehaviorOnRemove WRITE setSelectionBehaviorOnRemove)
#endif
public:
    explicit AAbstractTabBar(QWidget* parent = nullptr);
    virtual ~AAbstractTabBar();
    enum SelectionBehavior
    {
        SelectLeftTab,
        SelectRightTab,
        SelectPreviousTab,
    };
public:
    int addTab(const QString& text, QObject* object = nullptr);
    int addTab(const QIcon& icon, const QString& text, QObject* object = nullptr);

    int insertTab(int index, const QString& text, QObject* object = nullptr);
    int insertTab(int index, const QIcon& icon, const QString& text, QObject* object = nullptr);

    void removeTab(int index);
    virtual void moveTab(int from, int to);

    bool isTabEnabled(int index) const;
    void setTabEnabled(int index, bool enabled);

    bool isTabVisible(int index) const;
    void setTabVisible(int index, bool visible);

    Qt::TextElideMode elideMode() const;
    void setElideMode(Qt::TextElideMode mode);

    void setTabToolTip(int index, const QString &tip);
    QString tabToolTip(int index) const;

    int currentIndex() const;
    const AAbstractTab* currentTab() const;
    int count() const;

    QSize iconSize() const;
    void setIconSize(const QSize& size);

    bool tabsClosable() const;
    void setTabsClosable(bool closable);

    bool isMovable() const;
    void setMovable(bool movable);

    bool isScrollable() const;
    void setScrollable(bool scrollable);

    bool scrollButtonsBeside() const;
    void setScrollButtonsBeside(bool beside);

    void addWidgetToLeftSide(QWidget* widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
    int insertWidgetToLeftSide(int index, QWidget* widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
    void setDirectionRightSide(QBoxLayout::Direction direction);
    void addWidgetToRightSide(QWidget *widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
    int insertWidgetToRightSide(int index, QWidget *widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
    void removeWidget(QWidget* widget);
public:
    virtual AAbstractTab* createTab(const QIcon& icon, const QString& text, QObject* object) = 0;
    virtual QSize sizeHint() const override;
    inline bool isIndexValid(int index) const { return index >= 0 && index < m_tabList.count(); }
    AAbstractTab* tabAt(int index);
    const AAbstractTab* tabAt(int index) const;
    int tabIndex(const AAbstractTab* tab) const;
    QRect tabRect(int index) const;
    const QList<AAbstractTab*>& visibleTabs() const;
    void updateLayout(bool force = false);
    QWidget* scrollView() const;
protected:
    virtual void paintTab(QPainter* painter, AAbstractTab* tab) const;
    virtual void paintTabBarFrame(QPainter* painter, QStyleOptionTabBarBase* optTabBase) = 0;
    void moveTabOffset(int index, int offset);
    QSize calcMinSize(QWidget* widget) const;
    QRect calcCenterRect() const;
    int indexToLastRightWidget() const;
    void updateALayoutWidgets();
private:
    void init();
    void calcFirstLastVisible(int index, bool visible, bool remove);
    int selectNewCurrentIndexFrom(int m_currentIndex);
    int calcPosition(int from, int to, int index) const;
    void moveTabPosition(int from, int to);

    int indexAtPos(const QPoint& p) const;
    int tabAt(const QPoint& pos) const;

    SelectionBehavior selectionBehaviorOnRemove() const;
    void setSelectionBehaviorOnRemove(SelectionBehavior behavior);

    bool isAnimated() const;
    bool stopAnimation();
    void setCurrentNextEnabledIndex(int offset);
    void moveTabStopped(int index);

    void refresh();
    void createMovableTab();
public Q_SLOTS:
    void setCurrentIndex(int index);
Q_SIGNALS:
    void currentAboutToBeChanged(int index, bool& changed);
    void currentChanged(int index);
    void tabCloseRequested(int index);
    void tabMoved(int from, int to);
protected:
    virtual bool event(QEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void showEvent(QShowEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void changeEvent(QEvent* event) override;
protected:
    void closeButtonPressed(int index);
    virtual void setLayoutDirty() { m_layoutDirty = true; }
protected:
    int m_currentIndex;
    int m_pressedIndex;
    int m_previousIndex;
    int m_beginVisible;
    int m_endVisible;
    bool m_layoutDirty;
    QList<AAbstractTab*> m_tabList;
    QList<AAbstractTab*> m_visibleTabList;
    QSize m_iconSize;
    Qt::TextElideMode m_elideMode;
    bool m_tabsClosable;

    QPoint m_pntStartDrag;
    bool m_movable;
    bool m_startDrag;
    SelectionBehavior m_selectionBehaviorOnRemove;
    AMovableTabWidget* m_movableTabWidget;
    AScrollContainer* m_scrollContainer;
    QBoxLayout::Direction m_layoutDirection;

    struct STabBarItem
    {
        STabBarItem(QWidget* w, int s, Qt::Alignment a, bool r)
            : stretch(s), widget(w), right(r), customWidget(false), 
              alignment(a), action(nullptr) {}
        int stretch;
        QWidget* widget;
        bool right;
        bool customWidget;
        Qt::Alignment alignment;
        QAction* action;
    };
    QBoxLayout* m_customWidgetLayout;
    QList<STabBarItem*> m_items;
private:
    friend class AAbstractTab;
    friend class ADragTabAnimation;
    friend class AMovableTabWidget;
    Q_DISABLE_COPY(AAbstractTabBar)
};

class APROCH_API AAbstractTab : public QWidget
{
    Q_OBJECT
public:
    explicit AAbstractTab(AAbstractTabBar* bar, const QIcon& icon, const QString& text);
    ~AAbstractTab() override;
public:
    void startAnimation(AAbstractTabBar* tabBar, int duration);
    AAbstractTabBar* tabBar() const;
    int index() const;
    QObject* object() const;
    void setObject(QObject* obj);
    virtual void setText(const QString& text);
    void initStyleOption(AStyleOptionTab* option) const;
    virtual void initBasicStyleOption(AStyleOptionTab* option) const;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    bool isCurrent() const;
public Q_SLOTS:
    virtual void clicked();
    virtual void dblClicked();
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    void enterEvent(QEnterEvent* event) override;
#else
    void enterEvent(QEvent* event) override;
#endif
    void leaveEvent(QEvent* event) override;
    bool event(QEvent* event) override;
    int shortcutId() const;
    void updateShortcut();
protected:
    int m_shortcutId;
    QString m_text;
    QString m_toolTip;
    QIcon m_icon;
    int m_minSize;
    int m_maxSize;
    bool m_hoverClose;
    bool m_pressClose;
    QColor m_textColor;
    int m_lastTab;
    int m_dragOffset;
    QVariantAnimation* m_animation;
    AAbstractTabBar* m_tabBar;
private:
    QObject* m_object;
private:
    Q_DISABLE_COPY(AAbstractTab);
    friend class AAbstractTabBar;
};

APROCH_NAMESPACE_END