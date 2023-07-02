/****************************************************************************
 * @file    ARibbonPopup.h
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

#include <QEvent>
#include <QMenu>
#include <QToolButton>
#include <QStyleOption>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidgetAction>

#include "ARibbonButton.h"
#include "Widget/APopupMenu.h"

//
// The most of the following code is copied from Qtitan.
//
// Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
// Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
//

APROCH_NAMESPACE_BEGIN

class ARibbonGroup;
class ARibbonBar;

class ARibbonPagePopup : public QWidget
{
    Q_OBJECT
public:
    ARibbonPagePopup(ARibbonBar *ribbonBar);
    ARibbonBar *ribbonBar() const;
    void popup(const QRect &rect);

protected:
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *event) override;

private:
    ARibbonBar *m_ribbonBar;
};

class ARibbonGroupPopup : public QWidget
{
public:
    explicit ARibbonGroupPopup(ARibbonGroup *group);
    virtual ~ARibbonGroupPopup();
    void popup(const QPoint &pos);

public:
    virtual QSize sizeHint() const override;

protected:
    void hideEvent(QHideEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    ARibbonGroup *m_group;

private:
    Q_DISABLE_COPY(ARibbonGroupPopup)
};

class ARibbonGroupOption : public QToolButton
{
    Q_OBJECT
public:
    explicit ARibbonGroupOption(QWidget *parent);
    virtual ~ARibbonGroupOption();

public:
    QString text() const;

public:
    virtual QSize sizeHint() const override;

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void actionEvent(QActionEvent *event) override;

private:
    Q_DISABLE_COPY(ARibbonGroupOption)
};

class ARibbonGroupScrollButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ARibbonGroupScrollButton(QWidget *parent, bool scrollLeft);
    virtual ~ARibbonGroupScrollButton();
Q_SIGNALS:
    void stopScrollTimer();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

protected:
    bool m_scrollLeft;

private:
    Q_DISABLE_COPY(ARibbonGroupScrollButton)
};

class ARibbonOverflowMenuButton : public ARibbonButton
{
public:
    ARibbonOverflowMenuButton(QWidget *parent);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Q_DISABLE_COPY(ARibbonOverflowMenuButton)
};

class ARibbonOverflowAction : public QWidgetAction
{
public:
    ARibbonOverflowAction(QWidget *control, QWidget *parent);
    virtual ~ARibbonOverflowAction();

public:
    virtual QWidget *createWidget(QWidget *parent) override;
    virtual void deleteWidget(QWidget *widget) override;

private:
    QWidget *m_control;
};

class ARibbonOverflowMenu : public APopupMenu
{
    Q_OBJECT
public:
    ARibbonOverflowMenu(QWidget *parent);
    virtual ~ARibbonOverflowMenu();

public:
    void clearGroups();
    void addGroup(ARibbonGroup *group);
    void fillMenu();
    bool isEmpty() const;

protected:
    virtual void paintMenuItem(QPainter *painter, QStyleOptionMenuItem *option) override;

private:
    QList<ARibbonGroup *> m_groups;
    Q_DISABLE_COPY(ARibbonOverflowMenu)
};

APROCH_NAMESPACE_END
