/****************************************************************************
 * @file    AGraphicsToolkit.h
 * @date    2023-04-08
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
#include "Widget/Style/AStyleDefines.h"

APROCH_NAMESPACE_BEGIN

class APROCH_API AGraphicsToolkit
{
public:
    /** @brief 颜色转字符串 */
    static QString color2Text(const QColor& c);

    /** @brief 字体转字符串 */
    static QString font2Text(const QFont& f);

    /** @brief 图片圆角 */
    static QPixmap fillet(const QPixmap& pixmap,
                          const SCornerF& borderRadius,
                          const QRectF& region = QRectF(0, 0, -1, -1),
                          QPainter* painter = nullptr,
                          const QColor& fill = Qt::transparent);

    /** @brief 图片圆角 */
    static QImage fillet(const QImage& image,
                         const SCornerF& borderRadius,
                         const QRectF& region = QRectF(0, 0, -1, -1),
                         QPainter* painter = nullptr,
                         const QColor& fill = Qt::transparent);

    /** @brief 获取有效的圆角半径 */
    static SCornerF effectiveBorderRadius(const QRectF& rect, const SCornerF& borderRadius);

    /** @brief 绘制圆角矩形 */
    static void drawRoundedRect(QPainterPath& path, const QRectF& rect, const SCornerF& borderRadius);

    /** @brief 将笔刷颜色绘制到图片中 */
    static QPixmap drawBrushToPixmap(const QBrush& b, const QSize& size = QSize(16, 16));

    /** @brief 将字体绘制到图片上 */
    static QPixmap drawFontToPixmap(const QFont& font, const QSize& size = QSize(16, 16),
                                    int ptSize = 13, const QString& text = QString(QLatin1Char('A')));

    /** @brief 绘制矩形阴影 */
    static void drawShadow(QPainter* painter, 
                           const QSize& size,
                           qreal blurRadius, 
                           const QPointF& offset, 
                           const QColor& color,
                           const SCornerF& borderRadius = SCornerF());
};

APROCH_NAMESPACE_END
