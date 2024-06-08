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

#include <QScreen>

 //
 // The most of the following code is copied from Qtitan.
 //
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 //

#ifdef A_PRIVATE_HEADERS
#include <qpa/qplatformnativeinterface.h>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
#define QT_VERSION_SUPPORT_HIGHDPI
#include <QtGui/private/qhighdpiscaling_p.h>
#endif
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QGuiApplication>
#include <QWindow>
#endif
#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
#include <QDesktopWidget>
#endif

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

QPixmap AStyleHelper::preparePixmap(const QIcon& icon, const QSize& iconSize, QIcon::Mode mode, QIcon::State state, const QWidget* wd)
{
    QSize pmSize = iconSize;
    QPixmap pm = aproch_iconPixmap(icon, wd, iconSize, mode, state);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    qreal pixelRatio = wd ? wd->devicePixelRatioF() : pm.devicePixelRatioF();
    pmSize = QSize(qRound(pm.width() / pixelRatio), qRound(pm.height() / pixelRatio)); // pm.size() / pm.devicePixelRatio();
#else
    pmSize = pm.size();
#endif
#if A_DEPRECATED_CODE
    if (pmSize.width() * pmSize.height() < iconSize.width() * iconSize.height())
        pm = pm.scaled(iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
#endif
    return pm;
}

APROCH_NAMESPACE_END

int APROCH_PRE_NAMESPACE(aproch_horizontalAdvanceFont)(const QFontMetrics& fm, const QString& str, int len)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 11, 0))
    return fm.horizontalAdvance(str, len);
#else
    return fm.width(str, len);
#endif
}

int APROCH_PRE_NAMESPACE(aproch_horizontalAdvanceFont)(const QFontMetrics& fm, const QChar& ch)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 11, 0))
    return fm.horizontalAdvance(ch);
#else
    return fm.width(ch);
#endif
}

#if defined(A_PRIVATE_HEADERS) && defined(QT_VERSION_SUPPORT_HIGHDPI)
qreal aproch_highDpiScale(qreal value, qreal scaleFactor, const QPointF& /* origin */)
{
    return value * scaleFactor;
}

QSize aproch_highDpiScale(const QSize& value, qreal scaleFactor, const QPointF& /* origin */)
{
    return value * scaleFactor;
}

QSizeF aproch_highDpiScale(const QSizeF& value, qreal scaleFactor, const QPointF& /* origin */)
{
    return value * scaleFactor;
}

QVector2D aproch_highDpiScale(const QVector2D& value, qreal scaleFactor, const QPointF& /* origin */)
{
    return value * float(scaleFactor);
}

QPointF aproch_highDpiScale(const QPointF& pos, qreal scaleFactor, const QPointF& origin)
{
    return (pos - origin) * scaleFactor + origin;
}

QPoint aproch_highDpiScale(const QPoint& pos, qreal scaleFactor, const QPoint& origin)
{
    return (pos - origin) * scaleFactor + origin;
}

QRect aproch_highDpiScale(const QRect& rect, qreal scaleFactor, const QPoint& origin)
{
    return QRect(aproch_highDpiScale(rect.topLeft(), scaleFactor, origin), aproch_highDpiScale(rect.size(), scaleFactor, QPoint()));
}

QRectF aproch_highDpiScale(const QRectF& rect, qreal scaleFactor, QPoint origin = QPoint(0, 0))
{
    return QRectF(aproch_highDpiScale(rect.topLeft(), scaleFactor, origin), aproch_highDpiScale(rect.size(), scaleFactor, QPointF()));
}

QMargins aproch_highDpiScale(const QMargins& margins, qreal scaleFactor, QPoint /*origin*/)
{
    return QMargins(qRound(qreal(margins.left()) * scaleFactor), qRound(qreal(margins.top()) * scaleFactor),
                    qRound(qreal(margins.right()) * scaleFactor), qRound(qreal(margins.bottom()) * scaleFactor));
}

QRegion aproch_highDpiScale(const QRegion& region, qreal scaleFactor, QPoint origin = QPoint(0, 0))
{
    if (!QHighDpiScaling::isActive())
        return region;
    QRegion scaled;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
    for (QRegion::const_iterator it = region.begin(); it != region.end(); ++it)
#else
    QVector<QRect> rcs = region.rects();
    for (QVector<QRect>::const_iterator it = rcs.constBegin(); it != rcs.constEnd(); ++it)
#endif
        scaled += aproch_highDpiScale(QRectF(*it), scaleFactor, origin).toRect();
    return scaled;
}

QPair<qreal, QPoint> aproch_highDpiScaleAndOrigin(const QWindow* window)
{
    qreal factor = 1.0;
    QPoint origin = QPoint(0, 0);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 5))
    QHighDpiScaling::ScaleAndOrigin so = QHighDpiScaling::scaleAndOrigin(window);
    factor = so.factor;
    origin = so.origin; // TODO
#else
    if (!QHighDpiScaling::isActive())
        return QPair<qreal, QPoint>(factor, origin);
    factor = QHighDpiScaling::factor(window);
    QScreen* screen = window != nullptr ? window->screen() : QGuiApplication::primaryScreen();
    if (screen == nullptr)
        return QPair<qreal, QPoint>(factor, origin);
    origin = QHighDpiScaling::origin(screen->handle());
#endif
    return QPair<qreal, QPoint>(factor, origin);
}

template <typename T, typename C>
T aproch_fromNativePixels(const T& value, const C* context, bool local)
{
    QPair<qreal, QPoint> so = aproch_highDpiScaleAndOrigin(context);
    if (local)
        so.second = QPoint(0, 0);
    return aproch_highDpiScale(value, qreal(1) / so.first, so.second);
}

template <typename T, typename C>
T aproch_toNativePixels(const T& value, const C* context, bool local)
{
    QPair<qreal, QPoint> so = aproch_highDpiScaleAndOrigin(context);
    if (local)
        so.second = QPoint(0, 0);
    return aproch_highDpiScale(value, so.first, so.second);
}
#endif

QRect APROCH_PRE_NAMESPACE(aproch_fromNativeDPI)(const QRect& rect, const QWidget* window, bool local)
{
#if defined(A_PRIVATE_HEADERS) && defined(QT_VERSION_SUPPORT_HIGHDPI)
    if (window == nullptr)
        return rect;
    return aproch_fromNativePixels(rect, window->windowHandle(), local);
#else
    Q_UNUSED(window);
    Q_UNUSED(local);
    return rect;
#endif
}

QRect APROCH_PRE_NAMESPACE(aproch_toNativeDPI)(const QRect& rect, const QWidget* window, bool local)
{
#if defined(A_PRIVATE_HEADERS) && defined(QT_VERSION_SUPPORT_HIGHDPI)
    if (window == nullptr)
        return rect;
    return aproch_toNativePixels(rect, window->windowHandle(), local);
#else
    Q_UNUSED(window);
    Q_UNUSED(local);
    return rect;
#endif
}

QSize APROCH_PRE_NAMESPACE(aproch_fromNativeDPI)(const QSize& size, const QWidget* window, bool local)
{
#if defined(A_PRIVATE_HEADERS) && defined(QT_VERSION_SUPPORT_HIGHDPI)
    if (window == nullptr)
        return size;
    return aproch_fromNativePixels(size, window->windowHandle(), local);
#else
    Q_UNUSED(window);
    Q_UNUSED(local);
    return size;
#endif
}

QSize APROCH_PRE_NAMESPACE(aproch_toNativeDPI)(const QSize& size, const QWidget* window, bool local)
{
#if defined(A_PRIVATE_HEADERS) && defined(QT_VERSION_SUPPORT_HIGHDPI)
    if (window == nullptr)
        return size;
    return aproch_toNativePixels(size, window->windowHandle(), local);
#else
    Q_UNUSED(window);
    Q_UNUSED(local);
    return size;
#endif
}

QPoint APROCH_PRE_NAMESPACE(aproch_fromNativeDPI)(const QPoint& point, const QWidget* window, bool local)
{
#if defined(A_PRIVATE_HEADERS) && defined(QT_VERSION_SUPPORT_HIGHDPI)
    if (window == nullptr)
        return point;
    return aproch_fromNativePixels(point, window->windowHandle(), local);
#else
    Q_UNUSED(window);
    Q_UNUSED(local);
    return point;
#endif
}

QPoint APROCH_PRE_NAMESPACE(aproch_toNativeDPI)(const QPoint& point, const QWidget* window, bool local)
{
#if defined(A_PRIVATE_HEADERS) && defined(QT_VERSION_SUPPORT_HIGHDPI)
    if (window == nullptr)
        return point;
    return aproch_toNativePixels(point, window->windowHandle(), local);
#else
    Q_UNUSED(window);
    Q_UNUSED(local);
    return point;
#endif
}

QMargins APROCH_PRE_NAMESPACE(aproch_fromNativeDPI)(const QMargins& margins, const QWidget* window, bool local)
{
#if defined(A_PRIVATE_HEADERS) && defined(QT_VERSION_SUPPORT_HIGHDPI)
    if (window == nullptr)
        return margins;
    return aproch_fromNativePixels(margins, window->windowHandle(), local);
#else
    Q_UNUSED(window);
    Q_UNUSED(local);
    return margins;
#endif
}

QMargins APROCH_PRE_NAMESPACE(aproch_toNativeDPI)(const QMargins& margins, const QWidget* window, bool local)
{
#if defined(A_PRIVATE_HEADERS) && defined(QT_VERSION_SUPPORT_HIGHDPI)
    if (window == nullptr)
        return margins;
    return aproch_toNativePixels(margins, window->windowHandle(), local);
    // TODO    return QHighDpi::toNativePixels(margins, window->windowHandle());
#else
    Q_UNUSED(window);
    Q_UNUSED(local);
    return margins;
#endif
}

QRegion APROCH_PRE_NAMESPACE(aproch_fromNativeDPI)(const QRegion& region, const QWidget* window, bool local)
{
#if defined(A_PRIVATE_HEADERS) && defined(QT_VERSION_SUPPORT_HIGHDPI)
    if (window == nullptr)
        return region;
    return aproch_fromNativePixels(region, window->windowHandle(), local);
#else
    Q_UNUSED(window);
    Q_UNUSED(local);
    return region;
#endif
}

QRegion APROCH_PRE_NAMESPACE(aproch_toNativeDPI)(const QRegion& region, const QWidget* window, bool local)
{
#if defined(A_PRIVATE_HEADERS) && defined(QT_VERSION_SUPPORT_HIGHDPI)
    if (window == nullptr)
        return region;
    return aproch_toNativePixels(region, window->windowHandle(), local);
#else
    Q_UNUSED(window);
    Q_UNUSED(local);
    return region;
#endif
}

qreal APROCH_PRE_NAMESPACE(aproch_pixelRatioDPI)(const QWidget* window)
{
#if defined(A_PRIVATE_HEADERS) && defined(QT_VERSION_SUPPORT_HIGHDPI)
    if (window == nullptr)
        return 1.0;
    return QHighDpiScaling::factor(window->windowHandle());
#else
    Q_UNUSED(window);
    return 1.0;
#endif
}

bool APROCH_PRE_NAMESPACE(aproch_isDPIScalingActive)()
{
#if defined(A_PRIVATE_HEADERS) && defined(QT_VERSION_SUPPORT_HIGHDPI)
    return QHighDpiScaling::isActive();
#else
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    return QApplication::testAttribute(Qt::AA_EnableHighDpiScaling);
#else
    return false;
#endif
#endif
}

#if defined(Q_OS_LINUX) || defined(Q_OS_MAC) || defined(Q_OS_WASM)
void APROCH_PRE_NAMESPACE(aproch_initializeDrawingResources)()
{
    static bool tried = false;
    if (!tried)
    {
        tried = true;
    }
}

QColor APROCH_PRE_NAMESPACE(aproch_getColorizationColor)()
{
    return Qt::darkGray;
}

void APROCH_PRE_NAMESPACE(aproch_initTitleBarPalette)(QPalette& palette)
{
    QColor bgcolor = Qt::gray;
    QColor fgcolor = Qt::black;
    QColor frameColor = Qt::darkGray;

    palette.setColor(QPalette::Active, QPalette::Window, bgcolor);
    palette.setColor(QPalette::Active, QPalette::WindowText, fgcolor);
    palette.setColor(QPalette::Active, QPalette::ButtonText, fgcolor);
    palette.setColor(QPalette::Active, QPalette::Shadow, frameColor);

    bgcolor = Qt::white;
    fgcolor = Qt::gray;
    frameColor = Qt::darkGray;

    palette.setColor(QPalette::Inactive, QPalette::Window, bgcolor);
    palette.setColor(QPalette::Inactive, QPalette::WindowText, fgcolor);
    palette.setColor(QPalette::Inactive, QPalette::ButtonText, fgcolor);
    palette.setColor(QPalette::Inactive, QPalette::Shadow, frameColor);
}
#endif

int APROCH_PRE_NAMESPACE(aproch_getDPI)(const QWidget* window)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    if (QCoreApplication::instance()->testAttribute(Qt::AA_Use96Dpi))
        return 96;

    QScreen* screen = nullptr;
    if (window)
    {
        if (QWidget* topLevel = window->window())
        {
            if (QWindow* topLevelWindow = topLevel->windowHandle())
                screen = topLevelWindow->screen();
        }
    }
    if (!screen && QGuiApplication::primaryScreen())
        screen = QGuiApplication::primaryScreen();
    return qRound(screen->logicalDotsPerInch());
#else // (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    Q_UNUSED(window)
#ifdef Q_OS_MAC
        return 72;
#else
        return 96;
#endif
#endif // (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
}

Qt::MouseButtons APROCH_PRE_NAMESPACE(aproch_getMouseButtons)()
{
#if defined(Q_OS_LINUX)
    return aproch_get_mouseButtons_linux();
#elif defined(Q_OS_MAC)
    return aproch_get_mouseButtons_mac();
#else
    return QApplication::mouseButtons();
#endif
}

#if defined(Q_OS_LINUX) || defined(Q_OS_WASM)
QImage APROCH_PRE_NAMESPACE(aproch_getDesktopImage)(int screen)
{
    Q_UNUSED(screen)
        return QImage();
}

DesktopImageAspectStyle APROCH_PRE_NAMESPACE(aproch_getDesktopAspectStyle)(int screen)
{
    Q_UNUSED(screen)
        DesktopImageAspectStyle ret = AspectStyleCentral;
    return ret;
}
#endif

QRect APROCH_PRE_NAMESPACE(aproch_screenGeometry)(const QPoint& pos, const QWidget* widget)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    if (QGuiApplication::primaryScreen()->virtualSiblings().size() > 1)
    {
        QScreen* screen = QGuiApplication::screenAt(pos);
        if (!screen)
            screen = QGuiApplication::primaryScreen();
        return screen->geometry();
    }
    else
        return widget->screen()->geometry();
#else
    int index = 0;
    if (QApplication::desktop()->isVirtualDesktop())
        index = QApplication::desktop()->screenNumber(pos);
    else
        index = QApplication::desktop()->screenNumber(widget);
    return QApplication::desktop()->screenGeometry(index);
#endif
}

QRect APROCH_PRE_NAMESPACE(aproch_availableGeometry)(const QWidget* widget)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    if (widget == nullptr)
        return QGuiApplication::primaryScreen()->virtualGeometry();
    else
        return widget->screen()->availableGeometry();
#else
    return QApplication::desktop()->availableGeometry(widget);
#endif
}

QPixmap APROCH_PRE_NAMESPACE(aproch_iconPixmap)(const QIcon& icon, const QWidget* widget, const QSize& size, QIcon::Mode mode, QIcon::State state)
{
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    qreal devicePixelRatio = widget != nullptr ? widget->devicePixelRatio() : qApp->devicePixelRatio();
    return icon.pixmap(size, devicePixelRatio, mode, state);
#elif (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    QWindow* w = widget != nullptr ? widget->window()->windowHandle() : nullptr;
    return icon.pixmap(w, size, mode, state);
#else
    Q_UNUSED(widget)
        return icon.pixmap(iconSize, mode, state);
#endif
}
