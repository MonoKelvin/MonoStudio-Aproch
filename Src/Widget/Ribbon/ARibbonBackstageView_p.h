/****************************************************************************
 * @file    ARibbonBackstageView_p.h
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
#include <QScrollBar>
#include <QToolButton>
#include <QHash>

#include "ARibbonBackstageView.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

class QStyleOptionMenuItem;
class QWidgetAction;

APROCH_NAMESPACE_BEGIN

/* ARibbonBackstageCloseButton */
class ARibbonBackstageCloseButton : public QToolButton
{
public:
    explicit ARibbonBackstageCloseButton(QWidget* parent);
public:
    QSize sizeHint() const override;
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private:
    Q_DISABLE_COPY(ARibbonBackstageCloseButton)
};

class ARibbonBackstageStyleOption;
/* ARibbonBackstageViewMenu */
class ARibbonBackstageViewMenu : public QWidget
{
    Q_OBJECT
public:
    explicit ARibbonBackstageViewMenu(ARibbonBackstageView* backstageView);
public:
    void createBackstageCloseButton();
    ARibbonBackstageCloseButton* closeButton() const { return m_backStageCloseButton; }
    QAction* actionAt(const QPoint& p) const;
    QRect actionRect(QAction* act) const;
    void resetItemsDirty();
    void setOffsetScroll(int offset);
    int totalHeight() const;
public:
    QSize sizeHint() const override;
protected:
    void initStyleOption(ARibbonBackstageStyleOption* opt) const;
    void initStyleOption(QStyleOptionMenuItem* option, const QAction* action) const;
    void updateActionRects() const;
protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;
private:
    ARibbonBackstageView* m_backstageView;
    ARibbonBackstageCloseButton* m_backStageCloseButton;
    int m_totalHeight;
    int m_offsetScroll;
    bool m_mouseDown;
    mutable bool m_itemsDirty;
    mutable int m_menuWidth;
    mutable int m_maxIconWidth;
    mutable QVector<QRect> m_actionRects;
private:
    Q_DISABLE_COPY(ARibbonBackstageViewMenu)
};

class ARibbonBar;
class HackFilter;
/* ARibbonBackstageViewPrivate */
class ARibbonBackstageViewPrivate : public QObject
{
public:
    Q_OBJECT
    A_DECLARE_PUBLIC(ARibbonBackstageView)
public:
    explicit ARibbonBackstageViewPrivate();
    ~ARibbonBackstageViewPrivate() override;
    inline static ARibbonBackstageViewPrivate* _get(ARibbonBackstageView* view)
    { return &view->aproch_d(); }
    inline static const ARibbonBackstageViewPrivate* _get(const ARibbonBackstageView* view)
    { return &view->aproch_d(); }
public:
    void init();
    void adjustLocations();
    void adjustScrollBars();
    void updateLayoutInternal();
    void updateLayout();
    void updateGeometryPage(QWidget* widget);

    void setCurrentAction(QAction *action);
    QAction* currentAction() const;
    QWidgetAction* getAction(QWidget* w) const;
public Q_SLOTS:
    void scrollVertTriggered(int action);
    void scrollHorzTriggered(int action);
public:
    bool m_closePrevented;
    int m_scrollOffset;
    int m_scrollOffsetHorz;
    int m_totalHeight;
    QMap<QWidget*, QSize> m_mapSize;
    QScrollBar* m_scrollBarVert;
    QScrollBar* m_scrollBarHorz;
    ARibbonBackstageViewMenu* m_backstageMenu;
    QWidget* m_activePage;
    QAction* m_currentAction;
    QSize m_sizeRightPage;

    mutable QVector<QRect> m_actionRects;
    mutable QHash<QAction*, QWidget*> m_widgetItems;
private:
    Q_DISABLE_COPY(ARibbonBackstageViewPrivate)
};

APROCH_NAMESPACE_END