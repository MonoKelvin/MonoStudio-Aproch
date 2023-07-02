/****************************************************************************
 * @file    ARibbonSliderPane_p.h
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
#include <QMouseEvent>
#include <QBasicTimer>
#include <QPainter>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStyleOption>

#include "ARibbonSliderPane.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class ARibbonSliderButton : public QPushButton
{
    Q_OBJECT
public:
    ARibbonSliderButton(QWidget* parent, QStyle::PrimitiveElement typeBut = QStyle::PE_IndicatorArrowUp);
    ~ARibbonSliderButton();
protected:
    virtual void  paintEvent(QPaintEvent* event);
    virtual QSize sizeHint() const;
protected:
    QStyle::PrimitiveElement m_typeBut;
private:
    Q_DISABLE_COPY(ARibbonSliderButton)
};

class ARibbonSliderSlider : public QSlider
{
    Q_OBJECT
public:
    explicit ARibbonSliderSlider(Qt::Orientation orientation, QWidget* parent = nullptr)
        : QSlider(orientation, parent)
    {
    }
private:
    Q_DISABLE_COPY(ARibbonSliderSlider)
};

class ARibbonSliderPanePrivate : QObject
{
public:
    A_DECLARE_PUBLIC(ARibbonSliderPane)
public:
    explicit ARibbonSliderPanePrivate();
public:
    void initSlider();
public:
    ARibbonSliderSlider* m_slider;
    ARibbonSliderButton* m_buttonUp;
    ARibbonSliderButton* m_buttonDown;
    QHBoxLayout* m_layout;
private:
    Q_DISABLE_COPY(ARibbonSliderPanePrivate)
};

APROCH_NAMESPACE_END