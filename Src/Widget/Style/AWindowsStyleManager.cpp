/****************************************************************************
 * @file    AWindowsStyleManager.cpp
 * @date    2022-05-29
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
#include "AWindowsStyleManager.h"

#ifdef Q_OS_WINDOWS

APROCH_NAMESPACE_BEGIN

#define ARGB(a, r, g, b) (RGB(b, g, r) | ((BYTE)a << 24))

void AWindowsStyleManager::ApplyAcrylic(QWidget *widget, const SStyleOption &option)
{
    if (nullptr == widget)
        return;
    widget->setAttribute(Qt::WA_TranslucentBackground);

    ApplyAcrylic(HWND(widget->winId()), option);
}

void AWindowsStyleManager::ApplyAcrylic(HWND hwnd, const SStyleOption &option)
{
    WindowsBlurHelper(hwnd, ACCENT_ENABLE_ACRYLICBLURBEHIND, option);
}

void AWindowsStyleManager::ApplyAero(QWidget *widget, const SStyleOption &option)
{
    if (nullptr == widget)
        return;
    widget->setAttribute(Qt::WA_TranslucentBackground);

    ApplyAero(HWND(widget->winId()), option);
}

void AWindowsStyleManager::ApplyAero(HWND hwnd, const SStyleOption &option)
{
    WindowsBlurHelper(hwnd, ACCENT_ENABLE_BLURBEHIND, option);
}

AWindowsStyleManager::SWindowsThemeConfig AWindowsStyleManager::GetWindowsThemeConfig()
{
    const QString strReg = "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
    const QSettings setting(strReg, QSettings::NativeFormat);

    SWindowsThemeConfig config;
    config.appsUseLightTheme = setting.value(AStr("AppsUseLightTheme")).toBool();
    config.enableTransparency = setting.value(AStr("EnableTransparency")).toBool();
    config.systemUsesLightTheme = setting.value(AStr("SystemUsesLightTheme")).toBool();

    return config;
}

void AWindowsStyleManager::WindowsBlurHelper(HWND hwnd, ACCENT_STATE state, const SStyleOption &option)
{
    HMODULE hUser = GetModuleHandleA("user32.dll");
    if (!hUser)
        return;

    pfnSetWindowCompositionAttribute setWindowCompositionAttribute = (pfnSetWindowCompositionAttribute)GetProcAddress(hUser, "SetWindowCompositionAttribute");
    if (!setWindowCompositionAttribute)
        return;

    // 获取系统主题
    EThemeType themeType = option.themeType;
    const SWindowsThemeConfig config = GetWindowsThemeConfig();
    if (option.themeType == System)
    {
        if (config.appsUseLightTheme)
            themeType = Light;
        else
            themeType = Dark;
    }

    // 主题色
    QColor color;
    switch (themeType)
    {
    case Dark:
        color = Qt::black;
        break;
    case Light:
        color = Qt::white;
        break;
    case Custom:
        color = option.color;
        break;
    default:
        break;
    }

    // 透明度
    if (!config.enableTransparency)
        color.setAlphaF(1.0);
    else if (themeType != Custom)
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

    ACCENT_POLICY accent = {state, AccentFlags, ARGB(color.alpha(), color.red(), color.green(), color.blue()), 0};
    WINDOWCOMPOSITIONATTRIBDATA data;
    data.dwAttrib = WCA_ACCENT_POLICY;
    data.pvData = &accent;
    data.cbData = sizeof(accent);
    setWindowCompositionAttribute(hwnd, &data);
}

APROCH_NAMESPACE_END
#endif
