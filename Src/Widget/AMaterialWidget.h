/****************************************************************************
 * @file    AMaterialWidget.h
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
#include <QWidget>
#include <QGraphicsEffect>

// TODO
#ifdef A_OPENGL_ENABLED
#include "AOpenGLEffect.h"
#endif

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class APROCH_API AMaterialWidget : public QWidget
{
    Q_OBJECT
    Q_ENUMS(BlendType)
    Q_PROPERTY(BlendType blend READ blendType WRITE setBlendType)
    Q_PROPERTY(QColor tintColor READ tintColor WRITE setTintColor)
    Q_PROPERTY(double tintOpacity READ tintOpacity WRITE setTintOpacity)
    Q_PROPERTY(double noiseOpacity READ noiseOpacity WRITE setNoiseOpacity)
public:
    enum BlendType
    {
        DisabledBlend,
        InAppBlend,
        BackgroundBlend
    };
    explicit AMaterialWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~AMaterialWidget();
    BlendType blendType() const;
    void setBlendType(BlendType type);
    const QColor& tintColor() const;
    void setTintColor(const QColor& color);
    double tintOpacity() const;
    void setTintOpacity(double opacity);
    double noiseOpacity() const;
    void setNoiseOpacity(double opacity);

protected:
    QBrush createAcrylicBrush() const;
    void paintBackground(QPainter* painter, const QRegion& clip);
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void showEvent(QShowEvent *event);
    virtual void hideEvent(QHideEvent *event);
    virtual void changeEvent(QEvent * event);
    const QImage& grabBackstageToBuffer(const QRegion& clip, bool& hasChanges);
    void setBackstageToWindow(const QImage& newBackstage, const QRegion& clip);
    QRect clipRect() const;

private:
    friend class AMaterialWidgetEffect;
    BlendType m_blend;
    QColor m_tintColor;
    double m_tintOpacity;
    double m_noiseOpacity;
    QBrush m_acrylicBrush;
    QImage m_buffer;
};

APROCH_NAMESPACE_END