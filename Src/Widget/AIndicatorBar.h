/****************************************************************************
 * @file    AIndicatorBar.h
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
#pragma once
#include "AprochExportDefine.h"
#include <QWidget>

class QAbstractAnimation;

APROCH_NAMESPACE_BEGIN

class AIndicatorBarPrivate;

class APROCH_API AIndicatorBar : public QWidget
{
    Q_OBJECT;
    Q_PROPERTY(bool enableAnimation READ isEnableAnimation WRITE setEnableAnimation);
    Q_PROPERTY(int animationDuration READ getAnimationDuration WRITE setAnimationDuration);
    Q_PROPERTY(int offset READ getOffset WRITE setOffset);
    Q_PROPERTY(Qt::Orientation orientation READ getOrientation WRITE setOrientation);
public:
    explicit AIndicatorBar(QWidget* parent = nullptr);
    explicit AIndicatorBar(Qt::Orientation ori, QWidget* parent = nullptr);

    bool isEnableAnimation() const;
    void setEnableAnimation(bool enabled);

    int getAnimationDuration() const;
    void setAnimationDuration(int msec);

    int getOffset() const;
    void setOffset(int offset);

    Qt::Orientation getOrientation() const;
    void setOrientation(Qt::Orientation ori);

    QAbstractAnimation* getAnimation() const;

public:
    virtual QSize sizeHint() const override;

protected:
    virtual void showEvent(QShowEvent* evt) override;

private:
    Q_DISABLE_COPY_MOVE(AIndicatorBar);
    QSharedPointer<AIndicatorBarPrivate> d_ptr;
};

APROCH_NAMESPACE_END