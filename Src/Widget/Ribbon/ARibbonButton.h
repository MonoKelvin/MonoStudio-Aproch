/****************************************************************************
 * @file    ARibbonButton.h
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

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class ARibbonButtonPrivate;
class APROCH_API ARibbonButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(bool wordWrap READ wordWrap WRITE setWordWrap)
public:
    ARibbonButton(QWidget* parent = nullptr);
    ~ARibbonButton() override;
public:
    bool wordWrap() const;
    void setWordWrap(bool on);

    bool isLargeIcon() const;
    void setLargeIcon(bool large);

    bool simplifiedMode() const;
    void setSimplifiedMode(bool enabled);

    QSize sizeHint() const override;

    void changed();
protected:
    void updateIconSize();
protected:
    bool event(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void actionEvent(QActionEvent* event) override;
    void changeEvent(QEvent* event) override;
private:
    A_DECLARE_PRIVATE(ARibbonButton)
    Q_DISABLE_COPY(ARibbonButton)
};

APROCH_NAMESPACE_END