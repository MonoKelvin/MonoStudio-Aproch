/****************************************************************************
 * @file    AColorPickerIndicator.cpp
 * @date    2021-10-29
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
#include "AColorPickerIndicator.h"

APROCH_NAMESPACE_BEGIN

AColorPickerIndicator::AColorPickerIndicator(QWidget *parent)
    : QWidget(parent), mEasingCurve(QEasingCurve::OutCubic), mColor(Qt::white), mDuration(120), mIsUseAnimation(true)
{
    setFixedSize(20, 20);
    setAttribute(Qt::WA_TranslucentBackground);
    setToolTipDuration(4000);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(0, 2);
    effect->setColor(Qt::lightGray);
    effect->setBlurRadius(5);
    setGraphicsEffect(effect);
}

void AColorPickerIndicator::setColor(const QColor &color)
{
    if (color != mColor)
    {
        mColor = color;
        update();
    }
}

void AColorPickerIndicator::moveTo(int x, int y, bool isStopAniForced)
{
    const QPoint thePos(x - width() / 2, y - height() / 2);
    if (!isStopAniForced && mIsUseAnimation)
    {
        if (mDuration <= 0)
            mDuration = 50;

        QPropertyAnimation *pPosAnimation = new QPropertyAnimation(this, "pos", this);
        pPosAnimation->setDuration(mDuration);
        pPosAnimation->setStartValue(pos());
        pPosAnimation->setEndValue(thePos);
        pPosAnimation->setEasingCurve(mEasingCurve);
        pPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        move(thePos);
    }
}

void AColorPickerIndicator::updateTooltip(const QColor &color)
{
    setToolTip(AStringToolkit::Color2HexStr(color) + AKey_NewLine + AStringToolkit::Color2RgbStr(color));
}

void AColorPickerIndicator::paintEvent(QPaintEvent *ev)
{
    APROCH_USE_STYLE_SHEET();

    constexpr int thickness = 6;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // painter.setPen(QPen(mColor, thickness));
    // painter.drawEllipse(thickness / 2, thickness / 2, width() - thickness, height() - thickness);
    painter.setPen(QPen(QColor("#5e6482"), 2));
    painter.setBrush(QBrush(mColor));
    painter.drawEllipse(rect().adjusted(1, 1, -1, -1));
}

APROCH_NAMESPACE_END
