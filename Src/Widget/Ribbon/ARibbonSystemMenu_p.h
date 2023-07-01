/****************************************************************************
 * @file    ARibbonSystemMenu_p.h
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

#include "ARibbonSystemMenu.h"
#include "Widget/AScrollWidgetBar.h"
#include "Widget/APopupMenu_p.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class ARibbonSystemButtonPrivate : public QObject
{
public:
    A_DECLARE_PUBLIC(ARibbonSystemButton)
public:
    explicit ARibbonSystemButtonPrivate()
        : m_ribbonBar(nullptr), m_backstageWidget(nullptr)
    {
    }
public:
    void updateExtension();
public:
    ARibbonBar* m_ribbonBar;
    QWidget* m_backstageWidget;
};

class ARibbonSystemMenuButton : public QToolButton
{
    Q_OBJECT
public:
    ARibbonSystemMenuButton(QWidget* parent);
    ~ARibbonSystemMenuButton() override;
protected:
    bool event(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
private:
    Q_DISABLE_COPY(ARibbonSystemMenuButton);
};

class ARibbonSystemMenuPrivate : public APopupMenuPrivate
{
    A_DECLARE_EX_PUBLIC(ARibbonSystemMenu)
public:
    explicit ARibbonSystemMenuPrivate();
    void updateMargins();
    int ribbonBarItemHeight() const;
public:
    ARibbonBar* m_ribbonBar;
    QList<ARibbonSystemMenuButton*> m_systemButtonList;
    QList<QWidget*> m_pageList;
private:
    Q_DISABLE_COPY(ARibbonSystemMenuPrivate)
};

class ARibbonPageSystemRecentFileListPrivate : public QObject
{
public:
    A_DECLARE_PUBLIC(ARibbonPageSystemRecentFileList)
public:
    explicit ARibbonPageSystemRecentFileListPrivate();
    void initStyleOption(QStyleOptionMenuItem* option, const QAction* action) const;
    QAction* actionAt(const QPoint& p) const;
    QRect actionRect(QAction* act) const;
    void setCurrentAction(QAction* currentAction);
    void activateAction(QAction* action, QAction::ActionEvent action_e, bool self = true);
public:
    void updateActionRects();
    bool m_itemsDirty;
    bool m_mouseDown;
    QAction* m_currentAction;
    QList<QAction*> m_recentFileActs; // contents of the MRU list 
    int m_totalHeight;

    bool m_hasCheckableItems;
    QVector<QRect> m_actionRects;
};

class ARibbonPageSystemPopupPrivate : public APopupMenuPrivate
{
    A_DECLARE_EX_PUBLIC(ARibbonPageSystemPopup)
public:
    explicit ARibbonPageSystemPopupPrivate()
    {
    }
    void updateMargins();
    QSize captionSize() const;
    void initGeometry();
public:
    QString m_caption;
};

APROCH_NAMESPACE_END