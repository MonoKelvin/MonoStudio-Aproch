/****************************************************************************
 * @file    AWinUIStyleHelper.cpp
 * @date    2024-06-24 
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
#include "AWinUIStyleHelper.h"
#include "AWidgetStyleDecoration.h"

#include <QWKCore/private/qwkwindowsextra_p.h>
#include <QWKCore/qwindowkit_windows.h>

APROCH_NAMESPACE_BEGIN

#ifdef Q_OS_WIN

#include <Windows.h>
#define ARGB(a, r, g, b) (RGB(b, g, r) | ((BYTE)a << 24))

/** @brief 设置窗口黑色主题 */
static bool setTheme(HWND hwnd, EThemeType themeType, bool enable)
{
    if (!QWK::isWin101809OrGreater())
        return false;

    const QWK::DynamicApis& apis = QWK::DynamicApis::instance();

    // 获取系统主题
    const AWinUIStyleHelper::SThemeConfig config = AWinUIStyleHelper::getThemeConfig();
    if (themeType == EThemeType::System)
        themeType = config.appsUseLightTheme ? EThemeType::Light : EThemeType::Dark;

    // 主题色
    bool isDark = themeType == EThemeType::Dark;

    if (QWK::isWin101903OrGreater())
    {
        apis.pSetPreferredAppMode(isDark ? QWK::PAM_AUTO : QWK::PAM_DEFAULT);
    }
    else
    {
        apis.pAllowDarkModeForApp(isDark);
    }
    for (const auto attr : {
             QWK::_DWMWA_USE_IMMERSIVE_DARK_MODE_BEFORE_20H1,
             QWK::_DWMWA_USE_IMMERSIVE_DARK_MODE,
         })
    {
        apis.pDwmSetWindowAttribute(hwnd, attr, &isDark, sizeof(isDark));
    }

    apis.pFlushMenuThemes();
    return true;
}

static constexpr const MARGINS g_extendedMargins = { -1, -1, -1, -1 };
void restoreMargins(HWND hwnd)
{
    static auto margins = QWK::qmargins2margins(QMargins(0, 1, 0, 0));   // TODO
    QWK::DynamicApis::instance().pDwmExtendFrameIntoClientArea(hwnd, &margins);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

QSharedPointer<AWidgetStyleDecoration> AWinUIStyleHelper::createStyleDecoration(QWidget* widget)
{
    if (!widget)
        return nullptr;

    QSharedPointer<AWidgetStyleDecoration> wsd(new AWidgetStyleDecoration());
    wsd->initStyle(widget);

    return wsd;
}

typedef enum _WINDOWCOMPOSITIONATTRIB
{
    WCA_UNDEFINED = 0,
    WCA_NCRENDERING_ENABLED = 1,
    WCA_NCRENDERING_POLICY = 2,
    WCA_TRANSITIONS_FORCEDISABLED = 3,
    WCA_ALLOW_NCPAINT = 4,
    WCA_CAPTION_BUTTON_BOUNDS = 5,
    WCA_NONCLIENT_RTL_LAYOUT = 6,
    WCA_FORCE_ICONIC_REPRESENTATION = 7,
    WCA_EXTENDED_FRAME_BOUNDS = 8,
    WCA_HAS_ICONIC_BITMAP = 9,
    WCA_THEME_ATTRIBUTES = 10,
    WCA_NCRENDERING_EXILED = 11,
    WCA_NCADORNMENTINFO = 12,
    WCA_EXCLUDED_FROM_LIVEPREVIEW = 13,
    WCA_VIDEO_OVERLAY_ACTIVE = 14,
    WCA_FORCE_ACTIVEWINDOW_APPEARANCE = 15,
    WCA_DISALLOW_PEEK = 16,
    WCA_CLOAK = 17,
    WCA_CLOAKED = 18,
    WCA_ACCENT_POLICY = 19,
    WCA_FREEZE_REPRESENTATION = 20,
    WCA_EVER_UNCLOAKED = 21,
    WCA_VISUAL_OWNER = 22,
    WCA_LAST = 23
} WINDOWCOMPOSITIONATTRIB;

typedef struct _WINDOWCOMPOSITIONATTRIBDATA
{
    WINDOWCOMPOSITIONATTRIB dwAttrib;
    PVOID pvData;
    SIZE_T cbData;
} WINDOWCOMPOSITIONATTRIBDATA;

typedef enum _ACCENT_STATE
{
    ACCENT_DISABLED = 0,
    ACCENT_ENABLE_GRADIENT = 1,
    ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
    ACCENT_ENABLE_BLURBEHIND = 3,
    ACCENT_ENABLE_ACRYLICBLURBEHIND = 4,
    ACCENT_INVALID_STATE = 5
} ACCENT_STATE;

typedef struct _ACCENT_POLICY
{
    ACCENT_STATE AccentState;
    DWORD AccentFlags;
    DWORD GradientColor;
    DWORD AnimationId;
} ACCENT_POLICY;

typedef BOOL(WINAPI* pfnSetWindowCompositionAttribute)(HWND, WINDOWCOMPOSITIONATTRIBDATA*);

void WindowsBlurHelper(HWND hwnd, ACCENT_STATE state, const AWinUIStyleHelper::SStyleOption& option)
{
    HMODULE hUser = GetModuleHandleA("user32.dll");
    if (!hUser)
        return;

    pfnSetWindowCompositionAttribute setWindowCompositionAttribute = (pfnSetWindowCompositionAttribute)GetProcAddress(hUser, "SetWindowCompositionAttribute");
    if (!setWindowCompositionAttribute)
        return;

    // 获取系统主题
    EThemeType themeType = option.themeType;
    const AWinUIStyleHelper::SThemeConfig config = AWinUIStyleHelper::getThemeConfig();
    if (option.themeType == EThemeType::System)
    {
        if (config.appsUseLightTheme)
            themeType = EThemeType::Light;
        else
            themeType = EThemeType::Dark;
    }

    // 主题色
    QColor color;
    switch (themeType)
    {
    case EThemeType::Dark:
        color = Qt::black;
        break;
    case EThemeType::Light:
        color = Qt::white;
        break;
    case EThemeType::Custom:
        color = option.color;
        break;
    default:
        break;
    }

    // 透明度
    if (!config.enableTransparency)
        color.setAlphaF(1.0);
    else if (themeType != EThemeType::Custom)
        color.setAlphaF(0.92);

    // 模糊的类型
    DWORD AccentFlags = 1;
    switch (state)
    {
    case ACCENT_ENABLE_BLURBEHIND:
        AccentFlags = 2;
        break;
    case ACCENT_ENABLE_ACRYLICBLURBEHIND:
        AccentFlags = 1;
        break;
    default:
        break;
    }

    ACCENT_POLICY accent = { state, AccentFlags, ARGB(color.alpha(), color.red(), color.green(), color.blue()), 0 };
    WINDOWCOMPOSITIONATTRIBDATA data;
    data.dwAttrib = WCA_ACCENT_POLICY;
    data.pvData = &accent;
    data.cbData = sizeof(accent);
    setWindowCompositionAttribute(hwnd, &data);
}

void AWinUIStyleHelper::acrylic(QWidget* widget, const SStyleOption& option)
{
    if (!widget)
        return;
    widget->setAttribute(Qt::WA_TranslucentBackground);

    acrylic(HWND(widget->winId()), option);
}

void AWinUIStyleHelper::acrylic(HWND hwnd, const SStyleOption& option)
{
    WindowsBlurHelper(hwnd, ACCENT_ENABLE_ACRYLICBLURBEHIND, option);
}

void AWinUIStyleHelper::aero(QWidget* widget, const SStyleOption& option)
{
    if (!widget)
        return;
    widget->setAttribute(Qt::WA_TranslucentBackground);

    aero(HWND(widget->winId()), option);
}

void AWinUIStyleHelper::aero(HWND hwnd, const SStyleOption& option)
{
    WindowsBlurHelper(hwnd, ACCENT_ENABLE_BLURBEHIND, option);
}

bool AWinUIStyleHelper::mica(QWidget* widget, const SStyleOption& option, bool on)
{
    if (!widget)
        return false;
    widget->setAttribute(Qt::WA_TranslucentBackground);
    return mica(HWND(widget->winId()), option, on);
}

bool AWinUIStyleHelper::mica(HWND hwnd, const SStyleOption& option, bool on)
{
    const QWK::DynamicApis& apis = QWK::DynamicApis::instance();
    if (on)
    {
        // We need to extend the window frame into the whole client area to be able
        // to see the blurred window background.
        apis.pDwmExtendFrameIntoClientArea(hwnd, &g_extendedMargins);
        if (QWK::isWin1122H2OrGreater())
        {
            // Use official DWM API to enable Mica, available since Windows 11 22H2 (10.0.22621).
            const QWK::_DWM_SYSTEMBACKDROP_TYPE backdropType = QWK::_DWMSBT_MAINWINDOW;
            apis.pDwmSetWindowAttribute(hwnd, QWK::_DWMWA_SYSTEMBACKDROP_TYPE, &backdropType,
                                        sizeof(backdropType));
        }
        else
        {
            // Use undocumented DWM API to enable Mica, available since Windows 11 (10.0.22000).
            const BOOL enable = TRUE;
            apis.pDwmSetWindowAttribute(hwnd, QWK::_DWMWA_MICA_EFFECT, &enable, sizeof(enable));
        }
    }
    else
    {
        if (QWK::isWin1122H2OrGreater())
        {
            const QWK::_DWM_SYSTEMBACKDROP_TYPE backdropType = QWK::_DWMSBT_AUTO;
            apis.pDwmSetWindowAttribute(hwnd, QWK::_DWMWA_SYSTEMBACKDROP_TYPE, &backdropType,
                                        sizeof(backdropType));
        }
        else
        {
            const BOOL enable = FALSE;
            apis.pDwmSetWindowAttribute(hwnd, QWK::_DWMWA_MICA_EFFECT, &enable, sizeof(enable));
        }
        restoreMargins(hwnd);
    }

    return setTheme(hwnd, option.themeType, on);
}

bool AWinUIStyleHelper::micaAlt(QWidget* widget, const SStyleOption& option, bool on)
{
    if (!widget)
        return false;
    widget->setAttribute(Qt::WA_TranslucentBackground);
    return micaAlt(HWND(widget->winId()), option, on);
}

bool AWinUIStyleHelper::micaAlt(HWND hwnd, const SStyleOption& option, bool on)
{
    const QWK::DynamicApis& apis = QWK::DynamicApis::instance();
    if (!QWK::isWin1122H2OrGreater())
        return false;

    if (on)
    {
        // We need to extend the window frame into the whole client area to be able
        // to see the blurred window background.
        apis.pDwmExtendFrameIntoClientArea(hwnd, &g_extendedMargins);
        // Use official DWM API to enable Mica Alt, available since Windows 11 22H2 (10.0.22621).
        const QWK::_DWM_SYSTEMBACKDROP_TYPE backdropType = QWK::_DWMSBT_TABBEDWINDOW;
        apis.pDwmSetWindowAttribute(hwnd, QWK::_DWMWA_SYSTEMBACKDROP_TYPE, &backdropType,
                                    sizeof(backdropType));
    }
    else
    {
        const QWK::_DWM_SYSTEMBACKDROP_TYPE backdropType = QWK::_DWMSBT_AUTO;
        apis.pDwmSetWindowAttribute(hwnd, QWK::_DWMWA_SYSTEMBACKDROP_TYPE, &backdropType,
                                    sizeof(backdropType));
        restoreMargins(hwnd);
    }

    return setTheme(hwnd, option.themeType, on);
}

AWinUIStyleHelper::SThemeConfig AWinUIStyleHelper::getThemeConfig()
{
    SThemeConfig config;

    const QString strReg = "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
    const QSettings setting(strReg, QSettings::NativeFormat);
    config.appsUseLightTheme = setting.value(AStr("AppsUseLightTheme")).toBool();
    config.systemUsesLightTheme = setting.value(AStr("SystemUsesLightTheme")).toBool();
    config.enableTransparency = setting.value(AStr("EnableTransparency")).toBool();

    return config;
}
#endif

APROCH_NAMESPACE_END
