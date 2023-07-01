/****************************************************************************
 * @file    AStyleHelper.h
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
#include <QList>
#include <QStyleOption>

#include "Widget/Style/ACommonStyle.h"

APROCH_NAMESPACE_BEGIN

class APROCH_API AStyleHelper
{
public:
    AStyleHelper();
    virtual ~AStyleHelper();
public:
    static void draw3DRect(QPainter& pnt, QColor& col, int x, int y, int w, int h, bool up);
    static void draw3DRect(QPainter& pnt, const QColor& colLight, const QColor& colDark, int x, int y, int w, int h, bool up);

    static void draw3dRectEx(QPainter& pnt, const QRect& rect, const QColor& clrTopLeft, const QColor& clrBottomRight);
    static void draw3dRectEx(QPainter& pnt, int x, int y, int cx, int cy, const QColor& clrTopLeft, const QColor& clrBottomRight);

    static void drawRectangle(QPainter& pnt, const QRect& rc, const QColor& pen, const QColor& brush);
    static void drawTriangle(QPainter& pnt, const QPoint& pt0, const QPoint& pt1, const QPoint& pt2, const QColor& color);

    static void drawGradientFill(QPainter& pnt, const QRect& rect, const QColor& crFrom, const QColor& crTo, bool horz);
    static void drawGradientFill4(QPainter& pnt, const QRect& rect, const QColor& crFrom1, const QColor& crTo1, 
        const QColor& crFrom2, const QColor& crTo2, bool horz = true, int percentage = 50);

    static QRgb lightColor(const QRgb& rgb, int tint);
    static QRgb darkColor(const QRgb& rgb, int tint);

    static void RGBtoHSL(const QRgb& rgb, double& h, double& s, double& l);
    static QRgb HSLtoRGB(double h, double s, double l);
    static QRgb HLStoRGB_ONE( double h, double l, double s );
    static double MakeHue_ONE(double H);
    static double MakeHue_TWO(double H);

    static QRgb colorMakePale(const QRgb& rgb, double ratio = .97);
    static QRgb colorMakeDarker(const QRgb& rgb, double ratio = .1);
    static QRgb colorMakeLighter(const QRgb& rgb, double ratio = .1);

    static QRgb pixelAlpha(const QRgb& srcPixel, int percent);
    static QRgb blendColors(const QRgb& rgb1, const QRgb& rgb2, double ratio);

    static QImage updateImage(const QImage& imageSrc, QRgb clrBase, QRgb clrTone);
    static QImage addaptColors(const QImage& imageSrc, QRgb clrBase, QRgb clrTone, bool clampHue = true);
    static QImage invertColors(const QImage& pxSrc);
private:
    static double HueToRGB(double temp1, double temp2, double temp3);
    static int HueToRGB(int m1, int m2, int h);
    static QLinearGradient generateGradient(const QRect& rect, const QColor& crFrom, const QColor& crTo, bool bHorz);
private:
    static double m_colorTolerance; // Used by 'addaptColors' method
};

APROCH_NAMESPACE_END