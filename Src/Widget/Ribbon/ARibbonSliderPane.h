/****************************************************************************
 * @file    ARibbonSliderPane.h
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

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class ARibbonSliderPanePrivate;
class APROCH_API ARibbonSliderPane : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int singleStep READ singleStep WRITE setSingleStep)
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged USER true)
    Q_PROPERTY(int sliderPosition READ sliderPosition WRITE setSliderPosition NOTIFY sliderMoved)
public:
    ARibbonSliderPane(QWidget* parent = nullptr);
    virtual ~ARibbonSliderPane();
public:
    void setRange(int min, int max);
    void setScrollButtons(bool on);

    void setSliderPosition(int);
    int sliderPosition() const;

    void setSingleStep(int);
    int singleStep() const;

    int value() const;
public Q_SLOTS:
    void setValue(int);
Q_SIGNALS:
    void valueChanged(int value);
    void sliderMoved(int position);
protected Q_SLOTS:
    void increment();
    void decrement();
private:
    A_DECLARE_PRIVATE(ARibbonSliderPane)
    Q_DISABLE_COPY(ARibbonSliderPane)
};

APROCH_NAMESPACE_END