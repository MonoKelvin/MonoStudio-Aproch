/****************************************************************************
 * @file    ATrackBar.cpp
 * @date    2024-07-30 
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
#include "ATrackBar.h"
#include <QParallelAnimationGroup>

APROCH_NAMESPACE_BEGIN

class ATrackBarPrivate
{
public:
    int offset = 0;
    bool enableAnimation = true;
    bool backwordMove = false;
    QSize oldSize;
    QPoint finalPos;
    Qt::Orientation orientation = Qt::Vertical;
    QVariantAnimation* posAni = nullptr;
    QVariantAnimation* lenAni = nullptr;
    QParallelAnimationGroup* animationGroup = nullptr;
};

ATrackBar::ATrackBar(QWidget* parent)
    : ATrackBar(Qt::Vertical, parent)
{
}

ATrackBar::ATrackBar(Qt::Orientation ori, QWidget* parent)
    : QWidget(parent)
    , d_ptr(new ATrackBarPrivate)
{
    setAttribute(Qt::WA_StyledBackground);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    d_ptr->orientation = ori;
    setProperty("orientation", ori == Qt::Vertical ? "vertical" : "horizontal");

    d_ptr->posAni = new QVariantAnimation(this);
    d_ptr->posAni->setDuration(300);
    d_ptr->posAni->setEasingCurve(QEasingCurve::OutQuart);

    d_ptr->lenAni = new QVariantAnimation(this);
    d_ptr->lenAni->setDuration(300);
    d_ptr->lenAni->setEasingCurve(QEasingCurve::OutQuad);

    connect(d_ptr->posAni, &QVariantAnimation::valueChanged, [=](const QVariant& val) { 
        if (d_ptr->posAni->state() != QAbstractAnimation::Running)
            return;

        const QPoint newPos = val.toPoint();
        /*if (d_ptr->backwordMove)
        {
            if (d_ptr->orientation == Qt::Vertical)
            {
                move(newPos.x(), newPos.y() - (height() - d_ptr->oldSize.height()));
            }
            else
            {
                move(newPos.x() - (width() - d_ptr->oldSize.width()), newPos.y());
            }
        }
        else*/
        {
            move(newPos);
        }

        d_ptr->oldSize = size();
    });

    connect(d_ptr->lenAni, &QVariantAnimation::valueChanged, [=](const QVariant& val) {
        if (d_ptr->lenAni->state() != QAbstractAnimation::Running)
            return;

        const QSize newSize = val.toSize();
        /*if (d_ptr->backwordMove)
        {
            if (d_ptr->orientation == Qt::Vertical)
            {
                move(x(), y() - (newSize.height() - d_ptr->oldSize.height()));
            }
            else
            {
            }
        }*/

        resize(newSize);

        d_ptr->oldSize = size();
    });

    connect(d_ptr->animationGroup, &QAbstractAnimation::finished, [=]() {
        move(d_ptr->finalPos);
    });

    d_ptr->animationGroup = new QParallelAnimationGroup(this);
    d_ptr->animationGroup->addAnimation(d_ptr->posAni);
    d_ptr->animationGroup->addAnimation(d_ptr->lenAni);

    d_ptr->oldSize = size();
}

bool ATrackBar::isEnableAnimation() const
{
    return d_ptr->enableAnimation;
}

void ATrackBar::setEnableAnimation(bool enabled)
{
    d_ptr->enableAnimation = enabled;
}

int ATrackBar::getAnimationDuration() const
{
    return d_ptr->animationGroup->duration();
}

void ATrackBar::setAnimationDuration(int msec)
{
    d_ptr->lenAni->setDuration(msec);
    d_ptr->posAni->setDuration(msec);
}

int ATrackBar::getOffset() const
{
    return d_ptr->offset;
}

void ATrackBar::setOffset(int offset)
{
    d_ptr->offset = offset;
}

QAbstractAnimation* ATrackBar::getAnimation() const
{
    return d_ptr->animationGroup;
}

QSize ATrackBar::sizeHint() const
{
    return d_ptr->orientation == Qt::Vertical ? QSize(4, 16) : QSize(16, 4);
}

void ATrackBar::moveEvent(QMoveEvent* evt)
{
    if (!d_ptr->enableAnimation)
        return QWidget::moveEvent(evt);

    if (d_ptr->animationGroup->state() != QAbstractAnimation::Running)
    {
        QPoint oldPos = evt->oldPos();
        QPoint newPos = evt->pos();
        QSize oldSize = sizeHint();
        QSize newSize = oldSize;
        if (d_ptr->orientation == Qt::Vertical)
        {
            d_ptr->backwordMove = oldPos.y() < newPos.y();

            newSize.rheight() *= 4;
            newPos.rx() += d_ptr->offset;
        }
        else 
        {
            d_ptr->backwordMove = oldPos.x() < newPos.x();

            newSize.rwidth() *= 4;
            newPos.ry() += d_ptr->offset;
        }

        d_ptr->finalPos = newPos;

        d_ptr->posAni->setStartValue(oldPos);
        d_ptr->posAni->setEndValue(newPos);

        d_ptr->lenAni->setStartValue(oldSize);
        d_ptr->lenAni->setKeyValueAt(0.5, (oldSize + newSize) / 2);
        d_ptr->lenAni->setEndValue(oldSize);
        
        if (d_ptr->backwordMove)
        {
            d_ptr->posAni->setEasingCurve(QEasingCurve::OutQuad);
        }
        else
        {
            d_ptr->posAni->setEasingCurve(QEasingCurve::OutQuart);
        }

        d_ptr->animationGroup->start();
    }
}

void ATrackBar::resizeEvent(QResizeEvent* evt)
{
    if (!d_ptr->enableAnimation)
        return QWidget::resizeEvent(evt);

    return QWidget::resizeEvent(evt);
}

APROCH_NAMESPACE_END
