/****************************************************************************
 * @file    APlatform.h
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

#include <QWidget>
#include <QFont>
#include <QMouseEvent>
#include <QStyleOption>

#ifdef Q_OS_WIN
#include "APlatform_win.h"
#endif
#ifdef Q_OS_LINUX
#endif
#ifdef Q_OS_MAC
#endif

//
// The most of the following code is copied from Qtitan.
//
// Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
// Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
//

QT_BEGIN_NAMESPACE

class AMouseEventQt : public QMouseEvent
{
public:
    AMouseEventQt(QMouseEvent &e, const QPoint &localPosition) :
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
                                                                 QMouseEvent(e.type(), localPosition, e.scenePosition(), e.globalPosition(), e.button(), e.buttons(), e.modifiers())
    // QMouseEvent(e.type(), QPointF(localPosition), e.button(), e.buttons(), e.modifiers(), e.pointingDevice())
#else
#if (QT_VERSION > QT_VERSION_CHECK(5, 0, 0))
                                                                   QMouseEvent(e.type(), localPosition, localPosition, e.globalPos(), e.button(), e.buttons(), e.modifiers())
#else // for Qt4
                                                                   QMouseEvent(e.type(), localPosition, e.globalPos(), e.button(), e.buttons(), e.modifiers())
#endif
#endif
    {
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
#endif
    }
};

class AWheelEventQt : public QWheelEvent
{
public:
    AWheelEventQt(QWheelEvent &e, const QPoint &localPosition) :
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
                                                                 QWheelEvent(localPosition, e.globalPosition(), e.pixelDelta(), e.angleDelta(), e.buttons(), e.modifiers(), e.phase(), e.inverted())
#else
#if (QT_VERSION > QT_VERSION_CHECK(5, 14, 0))
                                                                   QWheelEvent(localPosition, e.globalPosition(), e.pixelDelta(), e.angleDelta(), e.buttons(), e.modifiers(), e.phase(), e.inverted())
#else // for Qt4-Qt5_old
                                                                   QWheelEvent(localPosition, e.globalPos(), e.delta(), e.buttons(), e.modifiers(), e.orientation())
#endif
#endif
    {
    }
};

class ContextMenuEventQt : public QContextMenuEvent
{
public:
    ContextMenuEventQt(ContextMenuEventQt &e) : QContextMenuEvent(e) {}
    ContextMenuEventQt(QContextMenuEvent &e) : QContextMenuEvent(e) {}
    void setPos(const QPoint &pos)
    {
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
#if 0
        Q_ASSERT(!m_points.isEmpty());
        QEventPoint p = m_points[0];
        m_points[0] = QEventPoint(p.id(), p.state(), pos, pos);
#endif
        this->m_pos = pos;
#else
        this->p = pos;
#endif
    }
};

class PaintEventQt : public QPaintEvent
{
public:
    PaintEventQt(PaintEventQt &e) : QPaintEvent(e) {}
    PaintEventQt(QPaintEvent &e) : QPaintEvent(e) {}
    void setRect(const QRect &rect) { this->m_rect = rect; }
    void setRegion(const QRegion &region) { this->m_region = region; }
};

Q_GUI_EXPORT void qt_blurImage(QImage &blurImage, qreal radius, bool quality, int transposed = 0);
Q_GUI_EXPORT void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
Q_GUI_EXPORT QImage qt_halfScaled(const QImage &source);

QT_END_NAMESPACE

APROCH_NAMESPACE_BEGIN

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
enum HBitmapFormat
{
    HBitmapNoAlpha,
    HBitmapPremultipliedAlpha,
    HBitmapAlpha
};
#endif

#ifdef Q_OS_MAC
bool aproch_set_window_hook(QWidget *w);
bool aproch_unset_window_hook(QWidget *w, bool restore);
bool aproch_set_window_frameless_mac(QWidget *w);
void aproch_paint_title_bar_mac(const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget);
bool aproch_window_start_native_drag_mac(QWidget *w, const QPoint &pos);
void aproch_window_resize_mac(QWidget *w);
QPixmap aproch_titlebar_standard_pixmap(QStyle::StandardPixmap sp, const QStyleOption *opt, const QWidget *widget);
Qt::MouseButtons aproch_get_mouseButtons_mac();
#endif
#ifdef Q_OS_LINUX
bool aproch_window_start_native_drag_linux(QWidget *w, const QPoint &pos, Qt::WindowFrameSection frameSection);
Qt::MouseButtons aproch_get_mouseButtons_linux();
QRect aproch_get_window_geometry_linux(QWidget *w);
#endif

APROCH_API void aproch_initializeDrawingResources();
APROCH_API void aproch_setWidgetPostion(QWidget *widget, const QPoint &position);
APROCH_API void aproch_paintAirEffect(QPainter *painter, const QRect &rect, const QRegion &clip);
APROCH_API bool aproch_setBlurBehindWindow(QWidget *widget, bool enabled);
APROCH_API bool aproch_blurBehindEnabled(QWidget *widget);
APROCH_API bool aproch_blurBehindSupported();
APROCH_API QColor aproch_getColorizationColor();
APROCH_API void aproch_initTitleBarPalette(QPalette &palette);
APROCH_API bool aproch_getDesktopLightTheme();
APROCH_API Qt::MouseButtons aproch_getMouseButtons();

APROCH_API QSize aproch_fromNativeDPI(const QSize &size, const QWidget *window, bool local = false);
APROCH_API QSize aproch_toNativeDPI(const QSize &size, const QWidget *window, bool local = false);
APROCH_API QRect aproch_fromNativeDPI(const QRect &rect, const QWidget *window, bool local = false);
APROCH_API QRect aproch_toNativeDPI(const QRect &rect, const QWidget *window, bool local = false);
APROCH_API QPoint aproch_fromNativeDPI(const QPoint &point, const QWidget *window, bool local = false);
APROCH_API QPoint aproch_toNativeDPI(const QPoint &point, const QWidget *window, bool local = false);
APROCH_API QMargins aproch_fromNativeDPI(const QMargins &margins, const QWidget *window, bool local = false);
APROCH_API QMargins aproch_toNativeDPI(const QMargins &margins, const QWidget *window, bool local = false);
APROCH_API QRegion aproch_fromNativeDPI(const QRegion &region, const QWidget *window, bool local = false);
APROCH_API QRegion aproch_toNativeDPI(const QRegion &region, const QWidget *window, bool local = false);
APROCH_API qreal aproch_pixelRatioDPI(const QWidget *window);
APROCH_API int aproch_getDPI(const QWidget *window);
APROCH_API bool aproch_isDPIScalingActive();

APROCH_API bool aproch_image_copy(QImage &dest, const QPoint &p, const QImage &src);
void aproch_paintTitleBarText(QPainter *painter, const QString &text, const QRect &rect, bool active, const QColor &color = QColor());
void aproch_paintTitleBarIcon(QPainter *painter, const QIcon &icon, const QRect &rect);
APROCH_API QPixmap aproch_getTitleBarIcon(QWidget *window);
APROCH_API void aproch_updateTitleBarWidgetPosition(QWidget *window);

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

APROCH_API int aproch_horizontalAdvanceFont(const QFontMetrics &fm, const QString &str, int len = -1);
APROCH_API int aproch_horizontalAdvanceFont(const QFontMetrics &fm, const QChar &ch);
APROCH_API QRect aproch_screenGeometry(const QPoint &pos, const QWidget *widget);
APROCH_API QRect aproch_availableGeometry(const QWidget *widget = nullptr);

inline void QPalette_setBrush(QPalette &palette, QPalette::ColorGroup acg, QPalette::ColorRole acr, const QBrush &abrush)
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

inline void QPalette_setColor(QPalette &palette, QPalette::ColorGroup acg, QPalette::ColorRole acr, const QColor &acolor)
{
    QPalette_setBrush(palette, acg, acr, QBrush(acolor));
}

inline void QPalette_setColor(QPalette &palette, QPalette::ColorRole acr, const QColor &acolor)
{
    QPalette_setColor(palette, QPalette::All, acr, acolor);
}

inline void QPalette_setBrush(QPalette &palette, QPalette::ColorRole acr, const QBrush &abrush)
{
    QPalette_setBrush(palette, QPalette::All, acr, abrush);
}

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
inline uint QFont_resolveMask(const QFont &font)
{
    return font.resolveMask();
}

inline void QFont_setResolveMask(QFont &font, uint mask)
{
    font.setResolveMask(mask);
}

inline uint QPalette_resolveMask(const QPalette &palette)
{
    return palette.resolveMask();
}

inline void QPalette_setResolveMask(QPalette &palette, uint mask)
{
    palette.setResolveMask(mask);
}
#else
inline uint QPalette_resolveMask(const QPalette &palette)
{
    return palette.resolve();
}

inline void QPalette_setResolveMask(QPalette &palette, uint mask)
{
    palette.resolve(mask);
}

inline uint QFont_resolveMask(const QFont &font)
{
    return font.resolve();
}

inline void QFont_setResolveMask(QFont &font, uint mask)
{
    font.resolve(mask);
}
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
inline QMetaType::Type qvariant_type(const QVariant &variant)
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

inline QMetaType::Type qvariant_type(const QVariant &variant)
{
    return (QMetaType::Type)variant.type();
}

#endif

APROCH_NAMESPACE_END
