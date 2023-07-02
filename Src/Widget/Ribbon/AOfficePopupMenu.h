/****************************************************************************
 * @file    AOfficePopupMenu.h
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
#include <QMenu>

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class AOfficePopupMenuPrivate;

class APROCH_API AOfficePopupMenu : public QMenu
{
    Q_OBJECT
    Q_PROPERTY(bool gripVisible READ isGripVisible WRITE setGripVisible)
public:
    AOfficePopupMenu(QWidget* parent = nullptr);
    virtual ~AOfficePopupMenu();
public:
    static AOfficePopupMenu* createPopupMenu(QWidget* parent = nullptr);
public:
    QAction* addWidget(QWidget* widget);
    void setGripVisible(bool visible);
    bool isGripVisible() const;
protected:
    void setWidgetBar(QWidget* widget);
protected:
    virtual bool event(QEvent* event);
    virtual void paintEvent(QPaintEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void moveEvent(QMoveEvent* event);
    virtual void showEvent(QShowEvent* event);
    virtual void hideEvent(QHideEvent* event);
private:
    friend class ARibbonGalleryPrivate;
    A_DECLARE_PRIVATE(AOfficePopupMenu)
    Q_DISABLE_COPY(AOfficePopupMenu)
};

APROCH_NAMESPACE_END