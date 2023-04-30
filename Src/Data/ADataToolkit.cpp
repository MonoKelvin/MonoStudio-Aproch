/****************************************************************************
 * @file    ADataToolkit.cpp
 * @date    2023-04-22 
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
#include "ADataToolkit.h"

#include <QCoreApplication>

APROCH_NAMESPACE_BEGIN

QString ADataToolkit::ColorText(const QColor &c)
{
    return QCoreApplication::translate("Aproch", "RGBA(%1, %2, %3, %4)")
        .arg(c.red())
        .arg(c.green())
        .arg(c.blue())
        .arg(c.alpha());
}

QPixmap ADataToolkit::DrawBrushToPixmap(const QBrush &b, const QSize &size)
{
    QImage img(size, QImage::Format_ARGB32_Premultiplied);
    img.fill(0);

    QPainter painter(&img);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(0, 0, img.width(), img.height(), b);

    QColor color = b.color();
    
    if (color.alpha() != 255)
    {
        QBrush opaqueBrush = b;
        color.setAlpha(255);
        opaqueBrush.setColor(color);
        painter.fillRect(img.width() / 4, img.height() / 4, img.width() / 2, img.height() / 2, opaqueBrush);
    }

    painter.end();

    return QPixmap::fromImage(img);
}

QString ADataToolkit::FontText(const QFont &f)
{
    return QCoreApplication::translate("Aproch", "[%1, %2px]").arg(f.family()).arg(f.pixelSize());
}

QPixmap ADataToolkit::DrawFontToPixmap(const QFont &font, const QSize &size, int ptSize, const QString& text)
{
    QFont f = font;
    QImage img(size, QImage::Format_ARGB32_Premultiplied);
    img.fill(0);

    QPainter p(&img);
    p.setRenderHint(QPainter::TextAntialiasing, true);
    p.setRenderHint(QPainter::Antialiasing, true);
    f.setPointSize(ptSize);
    p.setFont(f);

    QTextOption t;
    t.setAlignment(Qt::AlignCenter);
    p.drawText(QRect(QPoint(0, 0), size), text, t);

    return QPixmap::fromImage(img);
}

APROCH_NAMESPACE_END
