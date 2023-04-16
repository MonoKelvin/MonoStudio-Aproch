/****************************************************************************
 * @file    ADataUtils.h
 * @date    2022-7-23
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
 * @brief DWB工具类
 */
class APROCH_API ADPWUtils
{
public:
    /**
     * @brief 颜色转字符串
     * @param c 颜色
     * @return 颜色字符串，带有RGBA信息
     */
    static QString ColorText(const QColor &c);

    /**
     * @brief 将笔刷颜色绘制到图片中
     * @param b 笔刷
     * @param size 图片大小
     * @return 图片
     */
    static QPixmap DrawBrushToPixmap(const QBrush &b, const QSize &size = QSize(16, 16));

    /**
     * @brief 字体转字符串
     * @param f 字体
     * @return 字体字符串，带有大小信息
     */
    static QString FontText(const QFont &f);

    /**
     * @brief 将字体绘制到图片上
     * @param font 字体
     * @param size 图片大小
     * @param ptSize 字体大小
     * @return 图片
     */
    static QPixmap DrawFontToPixmap(const QFont &font, const QSize &size = QSize(16, 16), int ptSize = 13);
};

APROCH_NAMESPACE_END
