/****************************************************************************
 * @file    AButton.h
 * @date    2024-08-28 
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
#include <QPushButton>

APROCH_NAMESPACE_BEGIN

class AButtonPrivate;

class APROCH_API AButton : public QPushButton
{
    Q_OBJECT;

    Q_PROPERTY(QColor normalBkColor READ getNormalBkColor WRITE setNormalBkColor);
    Q_PROPERTY(QColor hoveredBkColor READ getHoveredBkColor WRITE setHoveredBkColor);
    Q_PROPERTY(QColor pressedBkColor READ getPressedBkColor WRITE setPressedBkColor);
    Q_PROPERTY(QColor checkedBkColor READ getCheckedBkColor WRITE setCheckedBkColor);
    Q_PROPERTY(QColor checkHoveredBkColor READ getCheckHoveredBkColor WRITE setCheckHoveredBkColor);

    Q_PROPERTY(QColor normalColor READ getNormalColor WRITE setNormalColor);
    Q_PROPERTY(QColor hoveredColor READ getHoveredColor WRITE setHoveredColor);
    Q_PROPERTY(QColor pressedColor READ getPressedColor WRITE setPressedColor);
    Q_PROPERTY(QColor checkedColor READ getCheckedColor WRITE setCheckedColor);
    Q_PROPERTY(QColor checkHoveredColor READ getCheckHoveredColor WRITE setCheckHoveredColor);

    Q_PROPERTY(int animationDuration READ getAnimationDuration WRITE setAnimationDuration);
    Q_PROPERTY(QEasingCurve::Type animationType READ getAnimationType WRITE setAnimationType);
    
public:
    explicit AButton(QWidget* parent = nullptr);
    explicit AButton(const QString& text, QWidget* parent = nullptr);
    ~AButton();

    QColor getNormalBkColor() const;
    void setNormalBkColor(const QColor& color);

    QColor getHoveredBkColor() const;
    void setHoveredBkColor(const QColor& color);

    QColor getPressedBkColor() const;
    void setPressedBkColor(const QColor& color);

    QColor getCheckedBkColor() const;
    void setCheckedBkColor(const QColor& color);

    QColor getCheckHoveredBkColor() const;
    void setCheckHoveredBkColor(const QColor& color);

    QColor getNormalColor() const;
    void setNormalColor(const QColor& color);

    QColor getHoveredColor() const;
    void setHoveredColor(const QColor& color);

    QColor getPressedColor() const;
    void setPressedColor(const QColor& color);

    QColor getCheckedColor() const;
    void setCheckedColor(const QColor& color);

    QColor getCheckHoveredColor() const;
    void setCheckHoveredColor(const QColor& color);

    int getAnimationDuration() const;
    void setAnimationDuration(int duration);

    QEasingCurve::Type getAnimationType() const;
    void setAnimationType(QEasingCurve::Type type);

    void setTextAlignment(Qt::Alignment a);
    Qt::Alignment getTextAlignment() const;

protected:
    virtual void paintEvent(QPaintEvent* e) override;
    virtual bool event(QEvent* e) override;

private:
    Q_DISABLE_COPY_MOVE(AButton);
    QSharedPointer<AButtonPrivate> d_ptr;
};

APROCH_NAMESPACE_END