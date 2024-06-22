/****************************************************************************
 * @file    AStyleDefines.h
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
#include "AprochGlobal.h"

class QProcess;
class QLabel;

APROCH_NAMESPACE_BEGIN

// ------------------------------
// macro defines
// ------------------------------

#if defined(Q_OS_WIN)
#pragma comment(lib, "UXTheme.lib")
#endif

#ifdef Q_OS_WIN
/** @brief Windows 窗口背景材质 */
enum EWinBackgroundMaterial
{
    NoneMaterial,
    DWMBlur,
    Acrylic,
    Mica,
    MicaAlt
};
#endif

enum EExpandingMode
{
    Expanded = 0,
    Compact = 1,
    Minimal = 2,
    Popup = 3
};

/** @brief 控件可见性状态 */
enum EWidgetVisiblityState
{
    WVS_Show,
    WVS_Hide,
    WVS_Invisible,
};

/** @brief 窗口标题控件选项 */
enum EWindowCaptionWidget
{
    WindowIcon = 0x00000001,
    WindowMenu = 0x00000002,
    WindowTitle = 0x00000004,
    WindowAppendixLayout = 0x00000008,
    WindowHelpButton = 0x00000010,
    WindowMinimizeButton = 0x00000020,
    WindowMaximizeButton = 0x00000040,
    WindowCloseButton = 0x00000080,

    WindowControllerButtons = WindowMinimizeButton | WindowMaximizeButton | WindowCloseButton,
    DialogWidgets = WindowIcon | WindowTitle | WindowAppendixLayout | WindowHelpButton | WindowCloseButton,
    WindowWidgets = WindowIcon | WindowTitle | WindowAppendixLayout | WindowControllerButtons,
};
Q_DECLARE_FLAGS(FWindowCaptionWidgets, EWindowCaptionWidget)
Q_DECLARE_OPERATORS_FOR_FLAGS(FWindowCaptionWidgets)

using Margins = QMargins;
using Paddings = QMargins;
using Spacing = qint32;
using BorderThickness = qint32;
using BorderThicknessF = qreal;
using LineWidth = qint32;
using LineWidthF = qreal;
using ForegroundColor = QColor;
using BackgroundColor = QColor;

// ------------------------------
// data stucture defines
// ------------------------------

/** @brief 阴影参数 */
struct SShadowParam
{
    /** @brief 阴影颜色 */
    QColor Color;

    /** @brief 模糊半径 */
    qreal Radius;

    /** @brief X方向偏移 */
    qreal XOffset;

    /** @brief Y方向偏移 */
    qreal YOffset;

    /** @brief 是否启用 */
    bool IsEnabled;

    SShadowParam(void) noexcept
        : Color("#292929"), Radius(20.0), XOffset(0.0), YOffset(0.0), IsEnabled(true)
    {
    }
};
Q_DECLARE_METATYPE(SShadowParam)

struct SCorner
{
    int TopLeft = 0;
    int TopRight = 0;
    int BottomLeft = 0;
    int BottomRight = 0;
    SCorner()
    {
    }
    SCorner(int corner) : TopLeft(corner), TopRight(corner), BottomLeft(corner), BottomRight(corner)
    {
    }
    SCorner(int tl, int tr, int bl, int br) : TopLeft(tl), TopRight(tr), BottomLeft(bl), BottomRight(br)
    {
    }
};
Q_DECLARE_METATYPE(SCorner)

struct SCornerF
{
    qreal TopLeft = 0.0;
    qreal TopRight = 0.0;
    qreal BottomLeft = 0.0;
    qreal BottomRight = 0.0;
    SCornerF()
    {
    }
    SCornerF(int corner) : TopLeft(corner), TopRight(corner), BottomLeft(corner), BottomRight(corner)
    {
    }
    SCornerF(qreal tl, qreal tr, qreal bl, qreal br) : TopLeft(tl), TopRight(tr), BottomLeft(bl), BottomRight(br)
    {
    }
};
Q_DECLARE_METATYPE(SCornerF)

struct SBorderThickness
{
    int Left = 0;
    int Top = 0;
    int Right = 0;
    int Bottom = 0;
    SBorderThickness()
    {
    }
    SBorderThickness(int thickness) : Left(thickness), Top(thickness), Right(thickness), Bottom(thickness)
    {
    }
    SBorderThickness(int l, int t, int r, int b) : Left(l), Top(t), Right(r), Bottom(b)
    {
    }
};
Q_DECLARE_METATYPE(SBorderThickness)

struct SBorderThicknessF
{
    qreal Left = 0;
    qreal Top = 0;
    qreal Right = 0;
    qreal Bottom = 0;
    SBorderThicknessF()
    {
    }
    SBorderThicknessF(qreal thickness) : Left(thickness), Top(thickness), Right(thickness), Bottom(thickness)
    {
    }
    SBorderThicknessF(qreal l, qreal t, qreal r, qreal b) : Left(l), Top(t), Right(r), Bottom(b)
    {
    }
};
Q_DECLARE_METATYPE(SBorderThicknessF)

// ------------------------------
// extern function defines
// ------------------------------

#ifndef APROCH_NO_PROCESS
APROCH_API QString getSystemEnvironmentVariable(const QString& key);
APROCH_API QString getEnvironmentVariable(QProcess* process, const QString& key);
APROCH_API void setEnvironmentVariable(QProcess* process, const QString& key, const QString& value);
#endif

template<class T>
inline bool aIsCornerValid(const T& corner)
{
    return corner.TopLeft > 0 && corner.TopRight > 0 && corner.BottomLeft > 0 && corner.BottomRight;
}

inline int GetFontPixelWidth(const QFont& font, const QString& text)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
    return QFontMetrics(font).horizontalAdvance(text);
#else
    return QFontMetrics(font).width(text);
#endif
}

inline int GetFontPixelHeight(const QFont& font)
{
    return QFontMetrics(font).height();
}

/**
 * @brief 给标签控件设置超出给定最大宽度的省略字符串
 * @param label 给定的QLabel控件
 * @param maxWidth 最大宽度，当控件字体像素宽度超过该值后就显示省略号
 * @param mode 省略模式
 */
extern APROCH_API void SetElidedText(QLabel* label, int maxWidth, Qt::TextElideMode mode = Qt::ElideRight);

APROCH_NAMESPACE_END
