/****************************************************************************
 * @file    ARibbonQuickAccessBar.h
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
#include <QToolBar>

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class ARibbonQuickAccessBarPrivate;
class APROCH_API ARibbonQuickAccessBar : public QToolBar
{
    Q_OBJECT
public:
    explicit ARibbonQuickAccessBar(QWidget* parent = nullptr);
    virtual ~ARibbonQuickAccessBar();
public:
    QAction* actionCustomizeButton() const;
    void setActionVisible(QAction* action, bool visible);
    bool isActionVisible(QAction* action) const;
    int visibleCount() const;
    void paintContent(QPainter* painter);
public:
    virtual QSize sizeHint() const;

Q_SIGNALS:        
    void showCustomizeMenu(QMenu* menu);

private Q_SLOTS:
    void customizeAction(QAction*);
    void aboutToShowCustomizeMenu();
    void aboutToHideCustomizeMenu();

protected:
    virtual bool event(QEvent* event);
    virtual void actionEvent(QActionEvent*);
    virtual void paintEvent(QPaintEvent*);
    virtual void resizeEvent(QResizeEvent* event);

private:
    A_DECLARE_PRIVATE(ARibbonQuickAccessBar);
    Q_DISABLE_COPY(ARibbonQuickAccessBar);
};

APROCH_NAMESPACE_END