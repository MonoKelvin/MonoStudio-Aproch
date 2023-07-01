/****************************************************************************
 * @file    ARibbonQuickAccessBar_p.h
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
#include <QToolButton>
#include <QActionGroup>

#include "ARibbonBar.h"
#include "Widget/Style/AStyleOption.h"
#include "ARibbonBar_p.h"
#include "ARibbonQuickAccessBar.h"
#include "ARibbonCustomizeManager_p.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

/* ARibbonQuickAccessButton */
class ARibbonQuickAccessButton : public QToolButton
{
    Q_OBJECT;
public:
    ARibbonQuickAccessButton(QWidget* parent = nullptr);
    virtual ~ARibbonQuickAccessButton();
public:
    virtual QSize sizeHint() const;
protected:
    virtual void paintEvent(QPaintEvent* event);
private:
    Q_DISABLE_COPY(ARibbonQuickAccessButton);
};

/* AQuickAccessAction */
class AQuickAccessAction : public QAction
{
    Q_OBJECT;
public:
    AQuickAccessAction(QObject* p, QAction* srcAction)
        : QAction(srcAction->text(), p)
        , m_srcAction(srcAction)
    {
        setCheckable(true);
        QToolBar* toolBar = qobject_cast<QToolBar*>(parent());
        Q_ASSERT(toolBar);
        setChecked(toolBar->widgetForAction(srcAction));
    }
    void update()
    {
        QToolBar* toolBar = qobject_cast<QToolBar*>(parent());
        Q_ASSERT(toolBar);
        setChecked(toolBar->widgetForAction(m_srcAction));
        setText(m_srcAction->text());
    }
public:
    QAction* m_srcAction;
private:
    Q_DISABLE_COPY(AQuickAccessAction)
};

/* AQuickAccessActionInvisible */
class AQuickAccessActionInvisible : public QAction
{
    Q_OBJECT
public:
    AQuickAccessActionInvisible(QObject* p, QActionGroup* data)
        : QAction(p)
        , m_data(data)
    {
        setProperty(__aproch_Action_Invisible, QLatin1String(__aproch_Action_Invisible));
        setVisible(false);
    }
public:
    QActionGroup* m_data;
private:
    Q_DISABLE_COPY(AQuickAccessActionInvisible)
};

/* ARibbonQuickAccessBarPrivate */
class ARibbonQuickAccessBarPrivate : public QObject
{
    Q_OBJECT
public:
    A_DECLARE_PUBLIC(ARibbonQuickAccessBar);
public:
    explicit ARibbonQuickAccessBarPrivate();
    virtual ~ARibbonQuickAccessBarPrivate();
public:
    void init();
    ARibbonBar* ribbonBar() const;
    AQuickAccessAction* findQuickAccessAction(QAction* action) const;
    QAction* findBeforeAction(QAction* action) const;
    void updateAction(QAction* action);
    void setActionVisible(QAction* action, bool visible);
public:
    QMenu* m_menu;
    QAction* m_actionAccessPopup;
    QActionGroup* m_customizeGroupAction;
    ARibbonQuickAccessButton* m_accessPopup;
    ARibbonToolTipManager* m_ribbonToolTipManager;
    QList<QAction*> m_actionList;
    bool m_removeAction : 1;
    bool m_customizeAction : 1;
private:
    Q_DISABLE_COPY(ARibbonQuickAccessBarPrivate)
};

APROCH_NAMESPACE_END