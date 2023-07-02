/****************************************************************************
 * @file    ARibbonTabBar.h
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
#include <QWidget>
#include <QMenu>

#include "Widget/Style/ACommonStyle.h"
#include "Widget/AAbstractTabBar.h"
#include "ARibbonPage.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class ARibbonTabBar;
class ARibbonTabPrivate;
class ARibbonTab;

// TODO: EXPROT 

class AContextualTab
{
public:
    AContextualTab(ARibbonBar* bar, ARibbonTab* tab);
    virtual ~AContextualTab();
    const QColor& color() const;
    const QString& title() const;
    const QString& groupName() const;
    QRect rect() const;
    QFont font() const;
public:
    ARibbonBar* ribbonBar;
    ARibbonTab* firstTab;
    ARibbonTab* lastTab;
};

class ARibbonTab : public AAbstractTab
{
    Q_OBJECT
public:
    explicit ARibbonTab(ARibbonTabBar* parent, const QString& text);
    ~ARibbonTab() override;
public:
    ARibbonTabBar* tabBar() const;
    ARibbonPage* page() const;
    Qt::Alignment alignment() const;
    void setAlignment(Qt::Alignment);
    void setText(const QString& text) override;
    const QString& text() const;
    const QString& contextTitle() const;
    const QString& contextGroupName() const;
    const QColor& contextColor() const;
    AContextualTab* contextualTab() const;
    void setContextualTab(AContextualTab* tab);
    QAction* defaultAction() const;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
public:
    void clicked() override;
    void dblClicked() override;
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    friend class ARibbonPage;
    A_DECLARE_PRIVATE(ARibbonTab)
    Q_DISABLE_COPY(ARibbonTab)
};

/* ARibbonTabBar */
class ARibbonTabBar : public AAbstractTabBar
{
    Q_OBJECT
public:
    explicit ARibbonTabBar(ARibbonBar* ribbonBar);
    ~ARibbonTabBar() override;
public:
    ARibbonBar* ribbonBar() const;
    ARibbonTab* getTab(int index);
    const ARibbonTab* getTab(int index) const;
protected:
    QAction* addAction(const QIcon& icon, const QString& text, Qt::ToolButtonStyle style, QMenu* menu = nullptr);
    QAction* addAction(QAction* action, Qt::ToolButtonStyle style);
    QMenu* addMenu(const QString& title);
protected:
    void paintTab(QPainter* painter, AAbstractTab* tab) const override;
    void paintTabBarFrame(QPainter* painter, QStyleOptionTabBarBase* optTabBase) override;
public:
    AAbstractTab* createTab(const QIcon& icon, const QString& text, QObject* object) override;
    QSize sizeHint() const override;
protected:
    bool event(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
private:
    ARibbonBar* m_ribbonBar;
private:
    friend class ARibbonTab;
    friend class ARibbonBar;
    Q_DISABLE_COPY(ARibbonTabBar)
};

APROCH_NAMESPACE_END