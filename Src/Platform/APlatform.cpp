/****************************************************************************
 * @file    APlatform.cpp
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
#include <QApplication>
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

#include "APlatform.h"

int APROCH_PRE_NAMESPACE(aproch_horizontalAdvanceFont)(const QFontMetrics &fm, const QString &str, int len)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 11, 0))
    return fm.horizontalAdvance(str, len);
#else
    return fm.width(str, len);
#endif
}

int APROCH_PRE_NAMESPACE(aproch_horizontalAdvanceFont)(const QFontMetrics &fm, const QChar &ch)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 11, 0))
    return fm.horizontalAdvance(ch);
#else
    return fm.width(ch);
#endif
}

#if defined(A_PRIVATE_HEADERS) && defined(QT_VERSION_SUPPORT_HIGHDPI)
qreal aproch_highDpiScale(qreal value, qreal scaleFactor, const QPointF & /* origin */)
{
    return value * scaleFactor;
}

QSize aproch_highDpiScale(const QSize &value, qreal scaleFactor, const QPointF & /* origin */)
{
    return value * scaleFactor;
}

QSizeF aproch_highDpiScale(const QSizeF &value, qreal scaleFactor, const QPointF & /* origin */)
{
    return value * scaleFactor;
}

QVector2D aproch_highDpiScale(const QVector2D &value, qreal scaleFactor, const QPointF & /* origin */)
{
    return value * float(scaleFactor);
}

QPointF aproch_highDpiScale(const QPointF &pos, qreal scaleFactor, const QPointF &origin)
{
    return (pos - origin) * scaleFactor + origin;
}

QPoint aproch_highDpiScale(const QPoint &pos, qreal scaleFactor, const QPoint &origin)
{
    return (pos - origin) * scaleFactor + origin;
}

QRect aproch_highDpiScale(const QRect &rect, qreal scaleFactor, const QPoint &origin)
{
    return QRect(aproch_highDpiScale(rect.topLeft(), scaleFactor, origin), aproch_highDpiScale(rect.size(), scaleFactor, QPoint()));
}

QRectF aproch_highDpiScale(const QRectF &rect, qreal scaleFactor, QPoint origin = QPoint(0, 0))
{
    return QRectF(aproch_highDpiScale(rect.topLeft(), scaleFactor, origin), aproch_highDpiScale(rect.size(), scaleFactor, QPointF()));
}

QMargins aproch_highDpiScale(const QMargins &margins, qreal scaleFactor, QPoint /*origin*/)
{
    return QMargins(qRound(qreal(margins.left()) * scaleFactor), qRound(qreal(margins.top()) * scaleFactor),
                    qRound(qreal(margins.right()) * scaleFactor), qRound(qreal(margins.bottom()) * scaleFactor));
}

QRegion aproch_highDpiScale(const QRegion &region, qreal scaleFactor, QPoint origin = QPoint(0, 0))
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

QPair<qreal, QPoint> aproch_highDpiScaleAndOrigin(const QWindow *window)
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
    QScreen *screen = window != nullptr ? window->screen() : QGuiApplication::primaryScreen();
    if (screen == nullptr)
        return QPair<qreal, QPoint>(factor, origin);
    origin = QHighDpiScaling::origin(screen->handle());
#endif
    return QPair<qreal, QPoint>(factor, origin);
}

template <typename T, typename C>
T aproch_fromNativePixels(const T &value, const C *context, bool local)
{
    QPair<qreal, QPoint> so = aproch_highDpiScaleAndOrigin(context);
    if (local)
        so.second = QPoint(0, 0);
    return aproch_highDpiScale(value, qreal(1) / so.first, so.second);
}

template <typename T, typename C>
T aproch_toNativePixels(const T &value, const C *context, bool local)
{
    QPair<qreal, QPoint> so = aproch_highDpiScaleAndOrigin(context);
    if (local)
        so.second = QPoint(0, 0);
    return aproch_highDpiScale(value, so.first, so.second);
}
#endif

QRect APROCH_PRE_NAMESPACE(aproch_fromNativeDPI)(const QRect &rect, const QWidget *window, bool local)
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

QRect APROCH_PRE_NAMESPACE(aproch_toNativeDPI)(const QRect &rect, const QWidget *window, bool local)
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

QSize APROCH_PRE_NAMESPACE(aproch_fromNativeDPI)(const QSize &size, const QWidget *window, bool local)
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

QSize APROCH_PRE_NAMESPACE(aproch_toNativeDPI)(const QSize &size, const QWidget *window, bool local)
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

QPoint APROCH_PRE_NAMESPACE(aproch_fromNativeDPI)(const QPoint &point, const QWidget *window, bool local)
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

QPoint APROCH_PRE_NAMESPACE(aproch_toNativeDPI)(const QPoint &point, const QWidget *window, bool local)
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

QMargins APROCH_PRE_NAMESPACE(aproch_fromNativeDPI)(const QMargins &margins, const QWidget *window, bool local)
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

QMargins APROCH_PRE_NAMESPACE(aproch_toNativeDPI)(const QMargins &margins, const QWidget *window, bool local)
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

QRegion APROCH_PRE_NAMESPACE(aproch_fromNativeDPI)(const QRegion &region, const QWidget *window, bool local)
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

QRegion APROCH_PRE_NAMESPACE(aproch_toNativeDPI)(const QRegion &region, const QWidget *window, bool local)
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

qreal APROCH_PRE_NAMESPACE(aproch_pixelRatioDPI)(const QWidget *window)
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

void APROCH_PRE_NAMESPACE(aproch_initTitleBarPalette)(QPalette &palette)
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

int APROCH_PRE_NAMESPACE(aproch_getDPI)(const QWidget *window)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    if (QCoreApplication::instance()->testAttribute(Qt::AA_Use96Dpi))
        return 96;

    QScreen *screen = nullptr;
    if (window)
    {
        if (QWidget *topLevel = window->window())
        {
            if (QWindow *topLevelWindow = topLevel->windowHandle())
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

QRect APROCH_PRE_NAMESPACE(aproch_screenGeometry)(const QPoint &pos, const QWidget *widget)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    if (QGuiApplication::primaryScreen()->virtualSiblings().size() > 1)
    {
        QScreen *screen = QGuiApplication::screenAt(pos);
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

QRect APROCH_PRE_NAMESPACE(aproch_availableGeometry)(const QWidget *widget)
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
