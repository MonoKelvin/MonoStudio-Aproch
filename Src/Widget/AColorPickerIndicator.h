/****************************************************************************
 * @file    AColorPickerIndicator.h
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
#include <QEasingCurve>

APROCH_NAMESPACE_BEGIN

/**
 * @brief 颜色拾取器指示器控件
 */
class APROCH_API AColorPickerIndicator : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(AColorPickerIndicator)
public:
    explicit AColorPickerIndicator(QWidget *parent = nullptr);

    /**
     * @brief 设置指示器颜色
     * @param color 指示器颜色
     */
    void setColor(const QColor &color);

    /**
     * @brief 移动到指定位置
     * @param x X坐标
     * @param y Y坐标
     * @param isStopAniForced 是否强制停止使用动画
     */
    virtual void moveTo(int x, int y, bool isStopAniForced = false);

    /**
     * @brief 更新提示
     * @param 颜色
     */
    virtual void updateTooltip(const QColor &color);

protected:
    virtual void paintEvent(QPaintEvent *ev) override;

public:
    /** @brief 动画曲线，@see mIsUseAnimation 为true时该字段才起作用 */
    QEasingCurve mEasingCurve;

    /** @brief 指示器颜色 */
    QColor mColor;

    /** @brief 一次动画的时间（毫秒） */
    int mDuration;

    /** @brief 是否使用动画 */
    bool mIsUseAnimation;
};

APROCH_NAMESPACE_END
