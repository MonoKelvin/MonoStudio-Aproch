/****************************************************************************
 * @file    ARibbonSearchBar.h
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

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class ARibbonBar;
class ARibbonSearchBarPrivate;

class APROCH_API ARibbonSearchBar : public QLineEdit
{
    Q_OBJECT
public:
    ARibbonSearchBar(ARibbonBar* ribbonBar);
    ~ARibbonSearchBar() override;
    QSize minimumSizeHint() const override;
    void setCompact(bool compact);
    const QIcon& icon() const;
    bool isHelpEnabled() const;
    void setHelpEnabled(bool enabled);
    const QList<QAction*>& suggestedActions() const;
    void addSuggestedAction(QAction* action);
    void removeSuggestedAction(QAction* action);
    int maxSearchItemCount() const;
    void setMaxSearchItemCount(int count);
protected Q_SLOTS:
    void animationFinished();
    void animationValueChanged(const QVariant&);
    void helpClicked();
public Q_SLOTS:
    void showPopup(const QString& text);
    void closePopup();
Q_SIGNALS:
    void showHelp(const QString& help);
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent*) override;
    void paintEvent(QPaintEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
private:
    A_DECLARE_PRIVATE(ARibbonSearchBar)
    Q_DISABLE_COPY(ARibbonSearchBar)
};

APROCH_NAMESPACE_END