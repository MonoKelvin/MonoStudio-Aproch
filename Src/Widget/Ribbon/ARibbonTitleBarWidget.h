/****************************************************************************
 * @file    ARibbonTitleBarWidget.h
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
#include <QStyle>
#include <QBoxLayout>
#include <QToolButton>
#include <QLineEdit>

#include "ARibbonBar.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class AContextualTab;
class ARibbonBar;
class ARibbonTabBar;
class ARibbonButton;
class AWindowTitleBar;
class ATitleBarStyleOption;
class ARibbonSearchBar;

class APROCH_API ARibbonTitleBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ARibbonTitleBarWidget(ARibbonBar* ribbonBar);
    ~ARibbonTitleBarWidget() override;
public:
public:
    ARibbonBar* ribbonBar() const;
    ARibbonTabBar* ribbonTabBar() const;
    ARibbonSearchBar* searchBar() const;
    void addWidget(QWidget* widget, int stretch = 0);
    void insertWidget(int index, QWidget* widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
    int  indexOf(QWidget* widget);
    void addWidgetToRightSide(QWidget *widget, int stretch = 0);
    void removeWidget(QWidget* widget);
    void adjustSizeTitleBar();
    void updateLayout();
    void setVisible(bool visible) override;
    QRect calcContextualAreaRect() const;
    AWindowTitleBar* getWindowTitleBar() const;
    QAction* addTitleButton(const QIcon& icon, const QString& help);
    void removeTitleButton(QAction* action);
    void setSearchBarAppearance(ARibbonBar::SearchBarAppearance appearance);
    ARibbonBar::SearchBarAppearance searchBarAppearance() const;
public Q_SLOTS:
    void windowTitleChanged(const QString&);
protected:
    QWidget* topWidget() const;
    QWidget* titleBarWindow() const;
    void updateTextTitle();
    QFont titleFont() const;
    void initTitleBarOption(ATitleBarStyleOption* opt) const;
    AContextualTab* hitContextHeaders(const QPoint& point) const;
    void updateContextualTabs();
    void removeContextualTabs();
    QSize calcMinSize(QWidget* widget) const;
    QRect calcTextRect() const;
    int lastLeftWidgetIndex() const;
protected:
    bool event(QEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private:
    ARibbonBar* m_ribbonBar;
    QBoxLayout* m_layout;
    QSpacerItem* m_leftItem;
    QSpacerItem* m_rightItem;
    ARibbonBar::SearchBarAppearance m_searchBarAppearance;
    ARibbonSearchBar* m_searchBar;
    QVector<AContextualTab *> m_contextualTabs;
    QString m_strTitle;
    bool m_dirtyTextTitle;
    QWidget* _pTopWidget = nullptr;
private:
    Q_DISABLE_COPY(ARibbonTitleBarWidget)
};

APROCH_NAMESPACE_END