/****************************************************************************
 * @file    ARibbonSliderPane.cpp
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
#include "stdafx.h"
#include "Widget/Style/ACommonStyle.h"
#include "Widget/Style/ACommonStyle_p.h"
#include "ARibbonSliderPane_p.h"

// 
// The most of the following code is copied from Qtitan.
// 
// Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
// Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
// 

APROCH_NAMESPACE_BEGIN

/* ARibbonSliderButton */
ARibbonSliderButton::ARibbonSliderButton(QWidget* parent, QStyle::PrimitiveElement typeBut)
  : QPushButton(parent)
{
    m_typeBut = typeBut;
}

ARibbonSliderButton::~ARibbonSliderButton()
{
}

/*! \reimp */
void ARibbonSliderButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QStyleOptionButton option;
    initStyleOption(&option);
    option.direction = m_typeBut == QStyle::PE_IndicatorArrowUp ? Qt::LeftToRight : Qt::RightToLeft;
    style()->drawPrimitive((QStyle::PrimitiveElement)ACommonStyle::PE_RibbonSliderButton, &option, &painter, this);
}

/*! \reimp */
QSize ARibbonSliderButton::sizeHint() const
{
    QSize sz(ACommonStylePrivate::dpiScaled(12., this), ACommonStylePrivate::dpiScaled(12., this));
    QStyleOptionButton option;
    initStyleOption(&option);
    option.direction = m_typeBut == QStyle::PE_IndicatorArrowUp ? Qt::LeftToRight : Qt::RightToLeft;
    sz = style()->sizeFromContents((QStyle::ContentsType)ACommonStyle::CT_RibbonSliderButton, &option, sz, this);
    return sz;
}


/* ARibbonSliderPanePrivate */
ARibbonSliderPanePrivate::ARibbonSliderPanePrivate()
    : m_slider(nullptr)
    , m_buttonUp(nullptr)
    , m_buttonDown(nullptr)
    , m_layout(nullptr)
{
}

void ARibbonSliderPanePrivate::initSlider()
{
    A_P(ARibbonSliderPane);
    m_slider = new ARibbonSliderSlider(Qt::Horizontal, &p);
    m_slider->setTickPosition(QSlider::NoTicks);

    m_layout = new QHBoxLayout();
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(4);

    m_layout->addWidget(m_slider);
    p.setLayout(m_layout);
}


/* ARibbonSliderPane */
ARibbonSliderPane::ARibbonSliderPane(QWidget* parent)
    : QWidget(parent)
{
    A_INIT_PRIVATE(ARibbonSliderPane);
    A_D(ARibbonSliderPane);
    d.initSlider();
    connect(d.m_slider, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged(int)));
    connect(d.m_slider, SIGNAL(sliderMoved(int)), this, SIGNAL(sliderMoved(int)));
}

ARibbonSliderPane::~ARibbonSliderPane()
{
    A_DELETE_PRIVATE();
}

void ARibbonSliderPane::setRange(int min, int max)
{
    A_D(ARibbonSliderPane);
    d.m_slider->setMinimum( min ); // min - Range
    d.m_slider->setMaximum( max ); // max - Range
}

void ARibbonSliderPane::setScrollButtons(bool on)
{
    A_D(ARibbonSliderPane);
    if (on)
    {
        d.m_buttonDown = new ARibbonSliderButton(this, QStyle::PE_IndicatorArrowDown );
        d.m_layout->insertWidget(0, d.m_buttonDown, 0, Qt::Alignment(Qt::AlignLeft | Qt::AlignVCenter));
        d.m_buttonUp   = new ARibbonSliderButton(this, QStyle::PE_IndicatorArrowUp);
        d.m_layout->addWidget(d.m_buttonUp, 0, Qt::Alignment(Qt::AlignRight | Qt::AlignVCenter));
        connect(d.m_buttonDown, SIGNAL(pressed()), this, SLOT(decrement()));
        connect(d.m_buttonUp,   SIGNAL(pressed()), this, SLOT(increment()));
    } 
    else 
    {
        disconnect(d.m_buttonUp, SIGNAL(pressed()), this, SLOT(increment()));
        disconnect(d.m_buttonDown, SIGNAL(pressed()), this, SLOT(decrement()));
        A_DELETE_AND_NULL(d.m_buttonUp);
        A_DELETE_AND_NULL(d.m_buttonDown);
    }
}

void ARibbonSliderPane::setSliderPosition(int position)
{
    A_D(ARibbonSliderPane);
    d.m_slider->setSliderPosition(position);
}

int ARibbonSliderPane::sliderPosition() const
{
    A_D(const ARibbonSliderPane);
    return d.m_slider->sliderPosition();
}

void ARibbonSliderPane::setSingleStep(int nPos)
{
    A_D(ARibbonSliderPane);
    d.m_slider->setSingleStep(nPos);
}

int ARibbonSliderPane::singleStep() const
{
    A_D(const ARibbonSliderPane);
    return d.m_slider->singleStep();
}

int ARibbonSliderPane::value() const
{
    A_D(const ARibbonSliderPane);
    return d.m_slider->value();
}

void ARibbonSliderPane::setValue(int value)
{
    A_D(ARibbonSliderPane);
    d.m_slider->setValue(value);
}

void ARibbonSliderPane::increment()
{
    A_D(ARibbonSliderPane);
    int nPos = d.m_slider->sliderPosition() + d.m_slider->singleStep();
    if (nPos <= d.m_slider->maximum()) 
        d.m_slider->setSliderPosition(nPos);
}

void ARibbonSliderPane::decrement()
{
    A_D(ARibbonSliderPane);
    int nPos = d.m_slider->sliderPosition() - d.m_slider->singleStep();
    if (nPos >= d.m_slider->minimum()) 
        d.m_slider->setSliderPosition(nPos);
}

APROCH_NAMESPACE_END