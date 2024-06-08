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

    static QPixmap preparePixmap(const QIcon& icon, const QSize& iconSize, QIcon::Mode mode, QIcon::State state, const QWidget* wd);

private:
    static double HueToRGB(double temp1, double temp2, double temp3);
    static int HueToRGB(int m1, int m2, int h);
    static QLinearGradient generateGradient(const QRect& rect, const QColor& crFrom, const QColor& crTo, bool bHorz);
private:
    static double m_colorTolerance; // Used by 'addaptColors' method
};

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
enum HBitmapFormat
{
    HBitmapNoAlpha,
    HBitmapPremultipliedAlpha,
    HBitmapAlpha
};
#endif

#ifdef Q_OS_MAC
bool aproch_set_window_hook(QWidget* w);
bool aproch_unset_window_hook(QWidget* w, bool restore);
bool aproch_set_window_frameless_mac(QWidget* w);
void aproch_paint_title_bar_mac(const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget);
bool aproch_window_start_native_drag_mac(QWidget* w, const QPoint& pos);
void aproch_window_resize_mac(QWidget* w);
QPixmap aproch_titlebar_standard_pixmap(QStyle::StandardPixmap sp, const QStyleOption* opt, const QWidget* widget);
Qt::MouseButtons aproch_get_mouseButtons_mac();
#endif
#ifdef Q_OS_LINUX
bool aproch_window_start_native_drag_linux(QWidget* w, const QPoint& pos, Qt::WindowFrameSection frameSection);
Qt::MouseButtons aproch_get_mouseButtons_linux();
QRect aproch_get_window_geometry_linux(QWidget* w);
#endif

APROCH_API QPixmap aproch_iconPixmap(const QIcon& icon, const QWidget* widget, const QSize& size, QIcon::Mode mode = QIcon::Normal, QIcon::State state = QIcon::Off);

APROCH_API void aproch_initializeDrawingResources();
APROCH_API void aproch_setWidgetPostion(QWidget* widget, const QPoint& position);
APROCH_API void aproch_paintAirEffect(QPainter* painter, const QRect& rect, const QRegion& clip);
APROCH_API bool aproch_setBlurBehindWindow(QWidget* widget, bool enabled);
APROCH_API bool aproch_blurBehindEnabled(QWidget* widget);
APROCH_API bool aproch_blurBehindSupported();
APROCH_API QColor aproch_getColorizationColor();
APROCH_API void aproch_initTitleBarPalette(QPalette& palette);
APROCH_API bool aproch_getDesktopLightTheme();
APROCH_API Qt::MouseButtons aproch_getMouseButtons();

APROCH_API QSize aproch_fromNativeDPI(const QSize& size, const QWidget* window, bool local = false);
APROCH_API QSize aproch_toNativeDPI(const QSize& size, const QWidget* window, bool local = false);
APROCH_API QRect aproch_fromNativeDPI(const QRect& rect, const QWidget* window, bool local = false);
APROCH_API QRect aproch_toNativeDPI(const QRect& rect, const QWidget* window, bool local = false);
APROCH_API QPoint aproch_fromNativeDPI(const QPoint& point, const QWidget* window, bool local = false);
APROCH_API QPoint aproch_toNativeDPI(const QPoint& point, const QWidget* window, bool local = false);
APROCH_API QMargins aproch_fromNativeDPI(const QMargins& margins, const QWidget* window, bool local = false);
APROCH_API QMargins aproch_toNativeDPI(const QMargins& margins, const QWidget* window, bool local = false);
APROCH_API QRegion aproch_fromNativeDPI(const QRegion& region, const QWidget* window, bool local = false);
APROCH_API QRegion aproch_toNativeDPI(const QRegion& region, const QWidget* window, bool local = false);
APROCH_API qreal aproch_pixelRatioDPI(const QWidget* window);
APROCH_API int aproch_getDPI(const QWidget* window);
APROCH_API bool aproch_isDPIScalingActive();

APROCH_API bool aproch_image_copy(QImage& dest, const QPoint& p, const QImage& src);
void aproch_paintTitleBarText(QPainter* painter, const QString& text, const QRect& rect, bool active, const QColor& color = QColor());
void aproch_paintTitleBarIcon(QPainter* painter, const QIcon& icon, const QRect& rect);
APROCH_API QPixmap aproch_getTitleBarIcon(QWidget* window);
APROCH_API void aproch_updateTitleBarWidgetPosition(QWidget* window);

enum DesktopImageAspectStyle
{
    AspectStyleCentral,
    AspectStyleTiled,
    AspectStyleIgnoreRatio,
    AspectStyleKeepRatio,
    AspectStyleKeepRatioByExpanding
};

QImage aproch_getDesktopImage(int screen = -1);
DesktopImageAspectStyle aproch_getDesktopAspectStyle(int screen = -1);

APROCH_API int aproch_horizontalAdvanceFont(const QFontMetrics& fm, const QString& str, int len = -1);
APROCH_API int aproch_horizontalAdvanceFont(const QFontMetrics& fm, const QChar& ch);
APROCH_API QRect aproch_screenGeometry(const QPoint& pos, const QWidget* widget);
APROCH_API QRect aproch_availableGeometry(const QWidget* widget = nullptr);

inline void QPalette_setBrush(QPalette& palette, QPalette::ColorGroup acg, QPalette::ColorRole acr, const QBrush& abrush)
{
    // QPalette-Bug: if the brush in the palette is the same so it does not detach palette private object but change the resolveMask.
    if (acg == QPalette::All)
    {
        for (uint i = 0; i < QPalette::NColorGroups; i++)
            QPalette_setBrush(palette, QPalette::ColorGroup(i), acr, abrush);
        return;
    }

    if (palette.brush(acg, acr) != abrush)
        palette.setBrush(acg, acr, abrush);
    else
    {
        QBrush b = abrush;
        palette.setBrush(acg, acr, Qt::transparent);
        palette.setBrush(acg, acr, b);
    }
}

inline void QPalette_setColor(QPalette& palette, QPalette::ColorGroup acg, QPalette::ColorRole acr, const QColor& acolor)
{
    QPalette_setBrush(palette, acg, acr, QBrush(acolor));
}

inline void QPalette_setColor(QPalette& palette, QPalette::ColorRole acr, const QColor& acolor)
{
    QPalette_setColor(palette, QPalette::All, acr, acolor);
}

inline void QPalette_setBrush(QPalette& palette, QPalette::ColorRole acr, const QBrush& abrush)
{
    QPalette_setBrush(palette, QPalette::All, acr, abrush);
}

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
inline uint QFont_resolveMask(const QFont& font)
{
    return font.resolveMask();
}

inline void QFont_setResolveMask(QFont& font, uint mask)
{
    font.setResolveMask(mask);
}

inline uint QPalette_resolveMask(const QPalette& palette)
{
    return palette.resolveMask();
}

inline void QPalette_setResolveMask(QPalette& palette, uint mask)
{
    palette.setResolveMask(mask);
}
#else
inline uint QPalette_resolveMask(const QPalette& palette)
{
    return palette.resolve();
}

inline void QPalette_setResolveMask(QPalette& palette, uint mask)
{
    palette.resolve(mask);
}

inline uint QFont_resolveMask(const QFont& font)
{
    return font.resolve();
}

inline void QFont_setResolveMask(QFont& font, uint mask)
{
    font.resolve(mask);
}
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
inline QMetaType::Type qvariant_type(const QVariant& variant)
{
    return (QMetaType::Type)variant.metaType().id();
}

inline QVariant qvariant_create(QMetaType::Type type)
{
    return QVariant(QMetaType(type), nullptr);
}
#else
inline QVariant qvariant_create(QMetaType::Type type)
{
    return QVariant((int)type, nullptr);
}

inline QMetaType::Type qvariant_type(const QVariant& variant)
{
    return (QMetaType::Type)variant.type();
}

#endif

APROCH_NAMESPACE_END

#ifndef QT_NO_EFFECTS
struct QEffects
{
    enum Direction
    {
        LeftScroll = 0x0001,
        RightScroll = 0x0002,
        UpScroll = 0x0004,
        DownScroll = 0x0008
    };

    typedef uint DirFlags;
};

extern void Q_GUI_EXPORT qScrollEffect(QWidget*, QEffects::DirFlags dir = QEffects::DownScroll, int time = -1);
extern void Q_GUI_EXPORT qFadeEffect(QWidget*, int time = -1);
#endif

