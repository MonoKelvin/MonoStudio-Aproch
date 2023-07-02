/****************************************************************************
 * @file    ARibbonSearchBar_p.h
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
#include <QLineEdit>
#include <QVariantAnimation>
#include <QKeyEvent>
#include <QActionGroup>

#include "AprochGlobal.h"
#include "Widget/APopupMenu.h"

// 
// The most of the following code is copied from Qtitan.
// 
// Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
// Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
// 

APROCH_NAMESPACE_BEGIN

class ARibbonBar;
class ARibbonSearchBar;
class ARibbonSearchBarPopup;
typedef QMap<QString, QList<QAction*>> ActionSearchIndex;

class ARibbonSearchBarPrivate: public QObject
{
public:
    A_DECLARE_PUBLIC(ARibbonSearchBar)
    explicit ARibbonSearchBarPrivate();
    ~ARibbonSearchBarPrivate() override;
    void init(ARibbonBar* ribbonBar);
    void showPopup(const QString& text);
private:
    void updateGeometry(bool animated);
    void buildIndex();
    QList<QAction*> findActions(const QString& text);
    ARibbonBar* m_ribbonBar;
    ARibbonSearchBarPopup* m_popup;
    bool m_compact;
    bool m_showPopupNeeded;
    bool m_helpEnabled;
    QString m_helpText;
    int m_maxSearchItemCount;
    QVariantAnimation* m_animation;
    QList<QAction*> m_suggestedActions;
    QAction* m_actionsSep;
    QAction* m_helpSep;
    QAction* m_suggestedSep;
    QAction* m_helpAction;
    QIcon m_icon;
    ActionSearchIndex m_searchIndex;
};

class ARibbonSearchBarPopup : public APopupMenu
{
public:
    ARibbonSearchBarPopup(ARibbonSearchBar* searchBar);
    ~ARibbonSearchBarPopup() override;
    QSize sizeHint() const override;
    void setUpdateActions(bool updateActions);
protected:
    bool event(QEvent* e) override;
    void paintEvent(QPaintEvent* e) override;
private:
    ARibbonSearchBar* m_searchBar;
    bool m_updateActions;
};

APROCH_NAMESPACE_END