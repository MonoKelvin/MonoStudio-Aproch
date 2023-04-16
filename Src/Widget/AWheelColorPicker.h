/****************************************************************************
 * @file    AWheelColorPicker.h
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
#pragma once

APROCH_NAMESPACE_BEGIN

/**
 * @brief 圆盘取色器
 */
class APROCH_API AWheelColorPicker : public AAbstractColorPicker
{
    Q_OBJECT
public:
    explicit AWheelColorPicker(const QColor &initColor = Qt::white, QWidget *parent = nullptr);

    /**
     * @brief 设置指示器位置
     * @param x x坐标
     * @param y y坐标
     */
    virtual void setIndicatorPos(int x, int y);

    /**
     * @brief 获取像素位置的颜色
     * @param x X坐标
     * @param y Y坐标
     * @return 颜色
     */
    virtual QColor getPixelColor(int x, int y) const override;

    /**
     * @brief 获取颜色所在的像素位置
     * @param color 颜色
     * @return 坐标
     */
    virtual QPoint getColorPixel(const QColor &color) const override;

    /**
     * @brief 设置明度
     * @param brightness 明亮度
     */
    void setBrightness(float brightness);

    /**
     * @brief 获取明度
     * @return 明度
     */
    inline float getBrightness() const noexcept
    {
        return mBrightness;
    }

protected:
#ifndef QT_NO_OPENGL
    virtual void initializeGL() override;
    virtual void paintGL() override;

protected:
#else
    /**
     * @brief 绘制色盘
     * @param ev 绘图事件
     */
    virtual void paintEvent(QPaintEvent *ev) override;
#endif

protected:
    /** @brief 明亮度 */
    qreal mBrightness;
};

APROCH_NAMESPACE_END
