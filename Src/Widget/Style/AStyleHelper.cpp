/****************************************************************************
 * @file    AStyleHelper.cpp
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
#include "stdafx.h"
#include "AStyleHelper.h"
#include <QApplication>
#include <QPainter>
#include <qmath.h>

#include "ACommonStyle.h"

APROCH_NAMESPACE_BEGIN

double AStyleHelper::m_colorTolerance = 0.2;

/*!
    \class AStyleHelper
    \internal
*/
AStyleHelper::AStyleHelper()
{
}

AStyleHelper::~AStyleHelper()
{
}

void AStyleHelper::draw3DRect(QPainter& pnt, QColor& col, int x, int y, int w, int h, bool up)
{
    const QPen savePen = pnt.pen();

    QColor light = col.lighter(135);
    QColor dark = col.darker(140);
    pnt.setPen(up ? light : dark);
    pnt.drawLine(x, y, x+w, y);
    pnt.drawLine(x, y, x, y+h);
    pnt.setPen(up ? dark : light);
    pnt.drawLine(x, y+h, x+w, y+h);
    pnt.drawLine(x+w, y, x+w, y+h);
    pnt.setPen(col);
    pnt.drawPoint(x+w, y);
    pnt.drawPoint(x, y+h);

    pnt.setPen(savePen);
}

void AStyleHelper::draw3DRect(QPainter& pnt, const QColor& colLight, const QColor& colDark, int x, int y, int w, int h, bool up)
{
    const QPen savePen = pnt.pen();

    QColor light = colLight;
    QColor dark = colDark;
    pnt.setPen(up ? light : dark);
    pnt.drawLine(x, y, x + w, y);
    pnt.drawLine(x, y, x, y + h);

    pnt.setPen(up ? dark : light);
    pnt.drawLine(x, y + h, x + w, y + h);
    pnt.drawLine(x + w, y, x + w, y + h);

    pnt.setPen(colLight);
    pnt.drawPoint(x+w, y);
    pnt.drawPoint(x, y+h);

    pnt.setPen(savePen);
}

void AStyleHelper::draw3dRectEx(QPainter& pnt, const QRect& rect, const QColor& clrTopLeft, const QColor& clrBottomRight)
{
    AStyleHelper::draw3dRectEx(pnt, rect.x(), rect.y(), rect.width(), rect.height(), clrTopLeft, clrBottomRight);
}

void AStyleHelper::draw3dRectEx(QPainter& pnt, int x, int y, int cx, int cy, const QColor& clrTopLeft, const QColor& clrBottomRight)
{
    pnt.fillRect(x, y, cx - 1, 1, clrTopLeft);
    pnt.fillRect(x, y, 1, cy - 1, clrTopLeft);
    pnt.fillRect(x + cx, y, -1, cy, clrBottomRight);
    pnt.fillRect(x, y + cy, cx, -1, clrBottomRight);
}

void AStyleHelper::drawRectangle(QPainter& pnt, const QRect& rect, const QColor& pen, const QColor& brush)
{
    if (brush.isValid()) 
        pnt.fillRect(rect, QBrush(brush));

    if (pen != brush && pen.isValid()) 
        draw3DRect(pnt, pen, pen, rect.x(), rect.y(), rect.width(), rect.height(), true);
}

void AStyleHelper::drawTriangle(QPainter& pnt, const QPoint& pt0, const QPoint& pt1, const QPoint& pt2, const QColor& color)
{
//    pnt.setRenderHint(QPainter::Antialiasing);
    QPen oldPen = pnt.pen();
    QBrush oldBrush = pnt.brush();

    pnt.setPen(color);
    pnt.setBrush(QBrush(color));

    QPoint pts[] = {pt0, pt1, pt2};
    pnt.drawPolygon(pts, 3);

    pnt.setBrush(oldBrush);
    pnt.setPen(oldPen);
}

void AStyleHelper::drawGradientFill(QPainter& pnt, const QRect& rect, const QColor& crFrom, const QColor& crTo, bool horz)
{
    pnt.save();
    pnt.setBrush(generateGradient(rect, crFrom, crTo, horz));
    pnt.setPen(Qt::transparent);
    pnt.drawRect(rect);
    pnt.restore();
}

void AStyleHelper::drawGradientFill4(QPainter& pnt, const QRect& rect, const QColor& crFrom1, const QColor& crTo1, 
                                    const QColor& crFrom2, const QColor& crTo2, bool horz, int percentage)
{
    percentage = qMin(qMax(0, percentage), 100);

    QRect rectFirst = rect;
    QRect rectSecond = rect;

    if (!horz)
    {
        rectFirst.setRight(rectFirst.left() + rectFirst.width() * percentage / 100);
        rectSecond.setLeft(rectFirst.right());
    }
    else
    {
        rectFirst.setBottom(rectFirst.top() + rectFirst.height() * percentage / 100);
        rectSecond.setTop(rectFirst.bottom());
    }

    drawGradientFill(pnt, rectFirst, crFrom1, crTo1, horz);
    drawGradientFill(pnt, rectSecond, crFrom2, crTo2, horz);
}

QRgb AStyleHelper::lightColor(const QRgb& rgb, int tint)
{
    double h, s, l;

    RGBtoHSL(rgb, h, s, l);
    return HSLtoRGB(h, s, l + (1.0 - l) * (double)tint / 100.0);
}

QRgb AStyleHelper::darkColor(const QRgb& rgb, int tint)
{
    double h, s, l;

    RGBtoHSL(rgb, h, s, l);
    return HSLtoRGB(h, s, l * (1 + (double)tint / 100.0));
}

void AStyleHelper::RGBtoHSL(const QRgb& rgb, double& h, double& s, double& l)
{
    double r = (double)qRed(rgb)/255.0;
    double g = (double)qGreen(rgb)/255.0;
    double b = (double)qBlue(rgb)/255.0;

    double maxcolor = qMax(r, qMax(g, b));
    double mincolor = qMin(r, qMin(g, b));

    l = (maxcolor + mincolor)/2;

    if (maxcolor == mincolor)
    {
        h = 0;
        s = 0;
    }
    else
    {
        if (l < 0.5)
            s = (maxcolor-mincolor)/(maxcolor + mincolor);
        else
            s = (maxcolor-mincolor)/(2.0-maxcolor-mincolor);

        if (r == maxcolor)
            h = (g-b)/(maxcolor-mincolor);
        else if (g == maxcolor)
            h = 2.0+(b-r)/(maxcolor-mincolor);
        else
            h = 4.0+(r-g)/(maxcolor-mincolor);

        h /= 6.0;
        if (h < 0.0) 
            h += 1;
    }
}

QRgb AStyleHelper::HSLtoRGB(double h, double s, double l)
{
    double r, g, b;
    double temp1, temp2;

    if (s == 0)
    {
        r = g = b = l;
    }
    else
    {
        if (l < 0.5)
            temp2 = l*(1.0 + s);
        else
            temp2 = l + s-l*s;

        temp1 = 2.0 * l-temp2;

        r = HueToRGB(temp1, temp2, h + 1.0/3.0);
        g = HueToRGB(temp1, temp2, h);
        b = HueToRGB(temp1, temp2, h - 1.0/3.0);
    }

    return qRgb((int)(r * 255.0), (int)(g * 255.0), (int)(b * 255.0));
}

QRgb AStyleHelper::blendColors(const QRgb& rgb1, const QRgb& rgb2, double ratio)
{
    double fAmountB = (1.0 - ratio);
    int btR = (int)(qRed(rgb1) * ratio + qRed(rgb2) * fAmountB);
    int btG = (int)(qGreen(rgb1) * ratio + qGreen(rgb2) * fAmountB);
    int btB = (int)(qBlue(rgb1) * ratio + qBlue(rgb2) * fAmountB);

    return qRgb(qMin(255, btR), qMin(255, btG), qMin(255, btB));
}

double AStyleHelper::HueToRGB(double temp1, double temp2, double temp3)
{
    if (temp3 < 0)
        temp3 = temp3 + 1.0;
    if (temp3 > 1)
        temp3 = temp3-1.0;

    if (6.0*temp3 < 1)
        return (temp1+(temp2-temp1)*temp3 * 6.0);

    else if (2.0*temp3 < 1)
        return temp2;

    else if (3.0*temp3 < 2.0)
        return (temp1+(temp2-temp1)*((2.0/3.0)-temp3)*6.0);

    return temp1;
}


int AStyleHelper::HueToRGB(int m1, int m2, int h)
{
    if (h < 0)
        h += 255;

    if (h > 255)
        h -= 255;

    if ((6 * h) < 255)
        return ((m1 + ((m2 - m1) / 255 * h * 6)) / 255);

    if ((2 * h) < 255)
        return m2 / 255;

    if ((3 * h) < (2 * 255))
        return ((m1 + (m2 - m1) / 255 * ((255 * 2 / 3) - h) * 6) / 255);

    return (m1 / 255);
}

QRgb AStyleHelper::HLStoRGB_ONE(double h, double l, double s)
{
    double r, g, b;
    double m1, m2;

    if(s==0) 
    {
        r=g=b=l;
    } 
    else 
    {
        if(l <=0.5)
            m2 = l*(1.0+s);
        else
            m2 = l+s-l*s;
        m1 = 2.0*l-m2;
        r = HueToRGB(m1, m2, h+1.0/3.0);
        g = HueToRGB(m1, m2, h);
        b = HueToRGB(m1, m2, h-1.0/3.0);
    }
    return qRgb((unsigned char)(r*255), (unsigned char)(g*255), (unsigned char)(b*255));
}

double AStyleHelper::MakeHue_ONE(double H)
{
    H -= (int)(H);
    if (H < 0.0)
        H += 1.0;
    return H;
}

double AStyleHelper::MakeHue_TWO(double H)
{
    return MakeHue_ONE(H / 360.0) * 360.0;
}

QRgb AStyleHelper::colorMakePale(const QRgb& rgb, double ratio)
{
    double h, s, l;
    AStyleHelper::RGBtoHSL(rgb, h, s, l);
    return AStyleHelper::HLStoRGB_ONE(h, ratio, s);
}

QRgb AStyleHelper::colorMakeDarker(const QRgb& rgb, double ratio)
{
    double h, s, l;
    ratio = qMax(0., 1. - ratio);
    RGBtoHSL(rgb, h, s, l);
    return HLStoRGB_ONE(h, qMin(1., l * ratio), qMin(1., s * ratio));
}

inline int aproch_round(double val)
{
    return int((val - int(val - 0.5)) >= 0.5 ? val + 0.5 : val - 0.5);
}

QRgb AStyleHelper::colorMakeLighter(const QRgb& rgb, double ratio)
{
    double h, s, l;
    double min = 0.01;
    AStyleHelper::RGBtoHSL(rgb, h, s, l);

    if (h < min && s < min && l < min)
    {
        int val = aproch_round(ratio * 255.0);
        return qRgb(val, val, val);
    }
    else
    {
        ratio += 1.;
        return HLStoRGB_ONE(h, qMin(1., l * ratio), qMin(1., s * ratio));
    }
}

QRgb AStyleHelper::pixelAlpha(const QRgb& srcPixel, int percent)
{
    QRgb clrFinal = qRgb( qMin(255, (qRed(srcPixel) * percent) / 100), 
        qMin(255, (qGreen(srcPixel) * percent) / 100), 
        qMin(255, (qBlue(srcPixel) * percent) / 100));
    return clrFinal;
}

QLinearGradient AStyleHelper::generateGradient(const QRect& rect, const QColor& crFrom, const QColor& crTo, bool bHorz)
{
    QLinearGradient gradient(rect.topLeft(), bHorz ? rect.bottomLeft() : rect.topRight());
    gradient.setColorAt(0.0, crFrom);
    gradient.setColorAt(1.0, crTo);
    return gradient;
}

QImage AStyleHelper::updateImage(const QImage& imageSrc, QRgb clrBase, QRgb clrHighlight)
{
    QImage image = imageSrc;

    QColor clrTr = clrBase;

    QColor clrHL = clrHighlight;
    QColor clrShadow = pixelAlpha(clrHL.rgb(), 67);
    QRgb iClrShadow = /*6579300;*/clrShadow.rgb();

    for (int x = 0; x < image.width(); x++)
    {
        for (int y = 0; y < image.height(); y++)
        {
            QColor clr = image.pixel(x, y);
            if (clr != clrTr)
                image.setPixel(x, y, iClrShadow);
        }
    }
    return image;
}

QImage AStyleHelper::addaptColors(const QImage& imageSrc, QRgb clrBase, QRgb clrTone, bool clampHue/*= true*/)
{
    double dSrcH, dSrcS, dSrcL;
    AStyleHelper::RGBtoHSL(clrBase, dSrcH, dSrcS, dSrcL);

    double dDestH, dDestS, dDestL;
    AStyleHelper::RGBtoHSL(clrTone, dDestH, dDestS, dDestL);

    double DH = dDestH - dSrcH;
    double DL = dDestL - dSrcL;
    double DS = dDestS - dSrcS;

    clampHue = (dDestH > 0.5) && clampHue;
    int bitsPerPixel = imageSrc.depth();
    if (bitsPerPixel >= 24)
    {
        QImage image = imageSrc;
        QRgb* bits = (QRgb*)image.bits();
        for (int i = 0; i < image.width() * image.height(); i++)
        {
            QRgb* bit = (QRgb*)bits + i;

            int alpha = qAlpha(*bit);
            if (alpha == 0)
                continue;

            double aR = 255.0 / (double)alpha;
            QRgb clrOrig = qRgb(qRed(*bit) * aR, qGreen(*bit) * aR, qBlue(*bit) * aR);

            double H,S,L;
            AStyleHelper::RGBtoHSL(clrOrig, H, S, L);

            if (qFabs(dSrcH - H) < m_colorTolerance)
            {
                double HNew = AStyleHelper::MakeHue_ONE(H + DH);
                double SNew = qMax(0., qMin(1.00, S + DS));
                double LNew = qMax(0., qMin(1.00, L + DL));

                QRgb color = AStyleHelper::HLStoRGB_ONE(HNew, clampHue ? L : LNew, SNew);
                *bit = qRgb(qRed(color), qGreen(color), qBlue(color));
            }
        }
        return image;
    }
    else if (bitsPerPixel < 24)
    {
        Q_ASSERT(false);
    }

    return QImage(imageSrc);
}

QImage AStyleHelper::invertColors(const QImage& imageSrc)
{
    QImage image(imageSrc.size(), QImage::Format_ARGB32_Premultiplied);
    for (int y = 0; y < imageSrc.height(); ++y) 
    {
        uint* dest = (uint *) image.scanLine(y);
        const uint* src = (uint *) imageSrc.scanLine(y);
        for (int x = 0; x < imageSrc.width(); ++x) 
        {
            if (qRed(src[x]) == 0 && qGreen(src[x]) == 0 && qBlue(src[x]) == 0)
                continue;
            dest[x] = 0xffffffff - (0x00ffffff & src[x]);
        }
    }
    return image;
}

APROCH_NAMESPACE_END