/****************************************************************************
 * @file    AIndicatorBar.cpp
 * @date    2024-08-04 
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
#include "AIndicatorBar.h"
#include <QParallelAnimationGroup>

APROCH_NAMESPACE_BEGIN

class AIndicatorBarPrivate
{
public:
    int offset = 0;
    bool enableAnimation = true;
    QRect finalRect;
    Qt::Orientation orientation = Qt::Vertical;
    QPropertyAnimation* propAni = nullptr;
};

AIndicatorBar::AIndicatorBar(QWidget* parent)
    : AIndicatorBar(Qt::Vertical, parent)
{
}

AIndicatorBar::AIndicatorBar(Qt::Orientation ori, QWidget* parent)
    : QWidget(parent)
    , d_ptr(new AIndicatorBarPrivate)
{
    setAttribute(Qt::WA_StyledBackground);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    d_ptr->orientation = ori;

    d_ptr->propAni = new QPropertyAnimation(this, "geometry");
    d_ptr->propAni->setDuration(300);
    d_ptr->propAni->setEasingCurve(QEasingCurve::OutQuart);

    connect(d_ptr->propAni, &QAbstractAnimation::finished, [=]() {
        QRect newGeo;
        if (d_ptr->orientation == Qt::Vertical)
            newGeo = d_ptr->finalRect.adjusted(d_ptr->offset, 0, d_ptr->offset, 0);
        else
            newGeo = d_ptr->finalRect.adjusted(0, d_ptr->offset, 0, d_ptr->offset);
        setGeometry(newGeo);
    });

    resize(sizeHint());
}

bool AIndicatorBar::isEnableAnimation() const
{
    return d_ptr->enableAnimation;
}

void AIndicatorBar::setEnableAnimation(bool enabled)
{
    d_ptr->enableAnimation = enabled;
}

int AIndicatorBar::getAnimationDuration() const
{
    return d_ptr->propAni->duration();
}

void AIndicatorBar::setAnimationDuration(int msec)
{
    d_ptr->propAni->setDuration(msec);
}

int AIndicatorBar::getOffset() const
{
    return d_ptr->offset;
}

void AIndicatorBar::setOffset(int offset)
{
    d_ptr->offset = offset;
}

Qt::Orientation AIndicatorBar::getOrientation() const
{
    return d_ptr->orientation;
}

void AIndicatorBar::setOrientation(Qt::Orientation ori)
{
    if (d_ptr->orientation == ori)
        return;

    d_ptr->orientation = ori;
    resize(height(), width());
}

QAbstractAnimation* AIndicatorBar::getAnimation() const
{
    return d_ptr->propAni;
}

QSize AIndicatorBar::sizeHint() const
{
    return d_ptr->orientation == Qt::Vertical ? QSize(4, 16) : QSize(16, 4);
}

void AIndicatorBar::showEvent(QShowEvent* evt)
{
    if (!d_ptr->enableAnimation)
        return QWidget::showEvent(evt);

    if (d_ptr->propAni->state() != QAbstractAnimation::Running)
    {
        d_ptr->finalRect = geometry();
        const QPoint center = d_ptr->finalRect.center();

        QRect startGeo = d_ptr->finalRect;
        QRect endGeo = d_ptr->finalRect;
        if (d_ptr->orientation == Qt::Vertical)
        {
            startGeo.adjust(d_ptr->offset, startGeo.height() / 2, 
                            d_ptr->offset, -startGeo.height() / 2);
            endGeo.adjust(d_ptr->offset, 0, d_ptr->offset, 0);
        }
        else
        {
            startGeo.adjust(startGeo.width() / 2, d_ptr->offset, 
                            -startGeo.width() / 2, d_ptr->offset);
            endGeo.adjust(0, d_ptr->offset, 0, d_ptr->offset);
        }

        d_ptr->propAni->setStartValue(startGeo);
        d_ptr->propAni->setEndValue(endGeo);

        d_ptr->propAni->start();
    }
}

APROCH_NAMESPACE_END
