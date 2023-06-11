/****************************************************************************
 * @file    AGraphicsToolkit.cpp
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
#include "stdafx.h"
#include "AGraphicsToolkit.h"

#include <QApplication>
#include <QPainterPath>
#include <QCoreApplication>

APROCH_NAMESPACE_BEGIN

QPixmap AGraphicsToolkit::fillet(const QPixmap& pixmap,
                                 const SCornerF& borderRadius,
                                 const QRectF& region,
                                 QPainter* painter,
                                 const QColor& fill)
{
    const QSize realSize = pixmap.size();
    QPixmap resultPixmap(realSize);
    resultPixmap.fill(fill);

    QPainter tempPainter(&resultPixmap);
    if (!painter)
        painter = &tempPainter;

    QRectF realRegion = region;
    if (!realRegion.isValid())
        realRegion = QRectF(QPointF(0.0, 0.0), realSize);
    else 
        realRegion.intersects(QRectF(QPointF(0.0, 0.0), realSize));

    QPainterPath path;
    drawRoundedRect(path, realRegion, borderRadius);

    const int realWidth = qMin(int(realRegion.width() + 0.5), realSize.width());
    const int realHeight = qMin(int(realRegion.height() + 0.5), realSize.height());

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    painter->setRenderHints(QPainter::SmoothPixmapTransform);
    painter->setClipPath(path);
    painter->drawPixmap(realRegion.topLeft().x(), realRegion.topLeft().y(), realWidth, realHeight, pixmap);
    painter->restore();

    return resultPixmap;
}

QImage AGraphicsToolkit::fillet(const QImage& image,
                                const SCornerF& borderRadius,
                                const QRectF& region,
                                QPainter* painter,
                                const QColor& fill)
{
    const QSize realSize = image.size();
    QImage resultPixmap(realSize, QImage::Format_ARGB32);
    resultPixmap.fill(fill);

    QPainter tempPainter(&resultPixmap);
    if (!painter)
        painter = &tempPainter;

    QRectF realRegion = region;
    if (!realRegion.isValid())
        realRegion = QRectF(QPointF(0.0, 0.0), realSize);
    else
        realRegion.intersects(QRectF(QPointF(0.0, 0.0), realSize));

    QPainterPath path;
    drawRoundedRect(path, realRegion, borderRadius);

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    painter->setRenderHints(QPainter::SmoothPixmapTransform);
    painter->setClipPath(path);
    painter->drawImage(realRegion, image);
    painter->restore();

    return resultPixmap;
}

void AGraphicsToolkit::drawRoundedRect(QPainterPath& path, const QRectF& rect, const SCornerF& borderRadius)
{
    if (!rect.isValid())
        return;

    SCornerF effectiveBR = effectiveBorderRadius(rect, borderRadius);
    const qreal& tl = borderRadius.TopLeft;
    const qreal& tr = borderRadius.TopRight;
    const qreal& bl = borderRadius.BottomLeft;
    const qreal& br = borderRadius.BottomRight;

    path.moveTo(rect.bottomRight() - QPointF(0, br));
    path.lineTo(rect.topRight() + QPointF(0, tr));
    if (!qFuzzyIsNull(tr))
        path.arcTo(QRectF(QPointF(rect.topRight() - QPointF(tr * 2, 0)), QSize(tr * 2, tr * 2)), 0, 90);
    path.lineTo(rect.topLeft() + QPointF(tl, 0));
    if (!qFuzzyIsNull(tl))
        path.arcTo(QRectF(QPointF(rect.topLeft()), QSize(tl * 2, tl * 2)), 90, 90);
    path.lineTo(rect.bottomLeft() - QPointF(0, bl));
    if (!qFuzzyIsNull(bl))
        path.arcTo(QRectF(QPointF(rect.bottomLeft() - QPointF(0, bl * 2)), QSize(bl * 2, bl * 2)), 180, 90);
    path.lineTo(rect.bottomLeft() + QPointF(br, 0));
    if (!qFuzzyIsNull(br))
        path.arcTo(QRectF(QPointF(rect.bottomRight() - QPointF(br * 2, br * 2)), QSize(br * 2, br * 2)), 270, 90);
}

SCornerF AGraphicsToolkit::effectiveBorderRadius(const QRectF& rect, const SCornerF& borderRadius)
{
    const qreal w = rect.width();
    const qreal h = rect.height();
    const qreal w_2 = w * .5;
    const qreal h_2 = h * .5;
    const qreal tl = qMax(0.0, borderRadius.TopLeft);
    const qreal tr = qMax(0.0, borderRadius.TopRight);
    const qreal bl = qMax(0.0, borderRadius.BottomLeft);
    const qreal br = qMax(0.0, borderRadius.BottomRight);

    SCornerF effectiveBR(tl, tr, bl, br);
    if (!rect.isValid())
        return effectiveBR;

    // vertical
    if (tl > h_2 && bl > h_2)
        effectiveBR.TopLeft = effectiveBR.BottomLeft = h_2;
    else if (tl > h_2)
    {
        effectiveBR.TopLeft = qMin(tl, h);
        effectiveBR.BottomLeft = qMin(bl, h - effectiveBR.TopLeft);
    }
    else if (bl < h_2)
    {
        effectiveBR.BottomLeft = qMin(bl, h);
        effectiveBR.TopLeft = qMin(tr, h - effectiveBR.BottomLeft);
    }

    // horizontal
    if (tr > w_2 && br > w_2)
        effectiveBR.TopRight = effectiveBR.BottomRight = w_2;
    else if (tr > w_2)
    {
        effectiveBR.TopRight = qMin(tr, w);
        effectiveBR.BottomRight = qMin(br, w - effectiveBR.TopRight);
    }
    else if (br < w_2)
    {
        effectiveBR.BottomRight = qMin(br, w);
        effectiveBR.TopRight = qMin(tr, w - effectiveBR.BottomRight);
    }

    return effectiveBR;
}

QString AGraphicsToolkit::color2Text(const QColor& c)
{
    return QCoreApplication::translate("Aproch", "RGBA(%1, %2, %3, %4)")
        .arg(c.red())
        .arg(c.green())
        .arg(c.blue())
        .arg(c.alpha());
}

QString AGraphicsToolkit::font2Text(const QFont& f)
{
    return QCoreApplication::translate("Aproch", "[%1, %2px]").arg(f.family()).arg(f.pixelSize());
}

QPixmap AGraphicsToolkit::drawBrushToPixmap(const QBrush& b, const QSize& size)
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

QPixmap AGraphicsToolkit::drawFontToPixmap(const QFont& font, const QSize& size, int ptSize, const QString& text)
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

QPixmap AGraphicsToolkit::drawCheckBox(bool value)
{
    QStyleOptionButton opt;
    opt.state |= value ? QStyle::State_On : QStyle::State_Off;
    opt.state |= QStyle::State_Enabled;
    const QStyle* style = QApplication::style();
    // Figure out size of an indicator and make sure it is not scaled down in a list view item
    // by making the pixmap as big as a list view icon and centering the indicator in it.
    // (if it is smaller, it can't be helped)
    const int indicatorWidth = style->pixelMetric(QStyle::PM_IndicatorWidth, &opt);
    const int indicatorHeight = style->pixelMetric(QStyle::PM_IndicatorHeight, &opt);
    const int listViewIconSize = indicatorWidth;
    const int pixmapWidth = indicatorWidth;
    const int pixmapHeight = qMax(indicatorHeight, listViewIconSize);

    opt.rect = QRect(0, 0, indicatorWidth, indicatorHeight);
    QPixmap pixmap = QPixmap(pixmapWidth, pixmapHeight);
    pixmap.fill(Qt::transparent);
    {
        // Center?
        const int xoff = (pixmapWidth > indicatorWidth) ? (pixmapWidth - indicatorWidth) / 2 : 0;
        const int yoff = (pixmapHeight > indicatorHeight) ? (pixmapHeight - indicatorHeight) / 2 : 0;
        QPainter painter(&pixmap);
        painter.translate(xoff, yoff);
        style->drawPrimitive(QStyle::PE_IndicatorCheckBox, &opt, &painter);
    }
    return pixmap;
}

APROCH_NAMESPACE_END
