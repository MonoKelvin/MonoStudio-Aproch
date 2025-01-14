/****************************************************************************
 * @file    AToggleSwitch_p.h
 * @date    2025-01-12 
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
#include "Widget/AToggleSwitch.h"

APROCH_NAMESPACE_BEGIN

class AToggleSwitchIndicatorContainer : public QFrame
{
    Q_OBJECT;
    Q_PROPERTY(int spacing READ getSpacing WRITE setSpacing);
public:
    AToggleSwitchIndicatorContainer(QWidget* parent)
        : QFrame(parent)
    {
        setObjectName("aproch-toggleswitch-indicator-container");
        setAttribute(Qt::WA_StyledBackground);
        //setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    QSize sizeHint() const override
    {
        ensurePolished();
        QStyleOptionFrame opt;
        initStyleOption(&opt);
        QSize sz = style()->subElementRect(QStyle::SE_RadioButtonIndicator, &opt, this).size();
        return sz;
    }

    inline void setSpacing(int s)
    {
        spacing = s;
        auto layout = parentWidget()->layout();
        if (layout)
            layout->setContentsMargins(s, 0, s, 0);
    }

    inline int getSpacing() const
    {
        return spacing;
    }

    int spacing = 4;
};

class AToggleSwitchIndicator : public QWidget
{
    Q_OBJECT;
public:
    AToggleSwitchIndicator(QWidget* parent)
        : QWidget(parent)
    {
        setObjectName("aproch-toggleswitch-indicator");
        setAttribute(Qt::WA_StyledBackground);
        //setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    virtual QSize sizeHint() const override
    {
        return QSize(12, 12);
    }

public Q_SLOTS:
    void onHoverEnter();
    void onHoverLeave();
    void onPressed();
    void onRelease();
};

class AToggleSwitchPrivate
{
public:
    AToggleSwitchPrivate(AToggleSwitch* q_ptr)
        : q(q_ptr)
    {
    }

    void updateIndicatorPosition(bool toggled);

    AToggleSwitch* q = nullptr;
    AToggleSwitchIndicator* indicator = nullptr;
    AToggleSwitchIndicatorContainer* container = nullptr;
    QPropertyAnimation* indicatorAnim = nullptr;
    QString offText;
    QString onText;
    QPoint mousePressedPoint;
    QPoint indicatorStartPoint;
    bool isMousePressed = false;
    bool isPressedMoving = false;
};

APROCH_NAMESPACE_END