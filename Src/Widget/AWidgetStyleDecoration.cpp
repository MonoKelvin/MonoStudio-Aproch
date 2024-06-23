/****************************************************************************
 * @file    AWidgetStyleDecoration.cpp
 * @date    2024-06-23 
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
#include "AWidgetStyleDecoration.h"

#include <QWindowKit/QWKWidgets/widgetwindowagent.h>

APROCH_NAMESPACE_BEGIN

AWidgetStyleDecoration::AWidgetStyleDecoration()
{
}

AWidgetStyleDecoration::~AWidgetStyleDecoration()
{
}

#ifdef Q_OS_WIN

static QMap<EWinBackgroundMaterial, QString> s_bkMaterial2Name = {
    QPair<EWinBackgroundMaterial, QString>(EWinBackgroundMaterial::DWMBlur, AStr("dwm-blur")),
    QPair<EWinBackgroundMaterial, QString>(EWinBackgroundMaterial::Acrylic, AStr("acrylic-material")),
    QPair<EWinBackgroundMaterial, QString>(EWinBackgroundMaterial::Mica, AStr("mica")),
    QPair<EWinBackgroundMaterial, QString>(EWinBackgroundMaterial::MicaAlt, AStr("mica-alt")),
};

bool AWidgetStyleDecoration::setBackgroundMaterial(EWinBackgroundMaterial bkMaterial, bool on)
{
    if (!_host)
        return false;

    Q_ASSERT(mWinAgent);

    bool hasMaterial = false;
    QString name = s_bkMaterial2Name.value(bkMaterial);
    if (name.isEmpty() || !on)
    {
        for (auto value : s_bkMaterial2Name)
            mWinAgent->setWindowAttribute(value, false);
    }
    else
    {
        hasMaterial = mWinAgent->setWindowAttribute(name, true);
        if (!hasMaterial)
            return false;
    }

    _host->setProperty("has-material", hasMaterial);
    _host->style()->polish(_host);
    return true;
}

EWinBackgroundMaterial AWidgetStyleDecoration::getBackgroundMaterial() const
{
    Q_ASSERT(mWinAgent);

    for (auto iter = s_bkMaterial2Name.cbegin(); iter != s_bkMaterial2Name.cend(); ++iter)
    {
        if (mWinAgent->windowAttribute(iter.value()).toBool())
            return iter.key();
    }

    return EWinBackgroundMaterial::NoneMaterial;
}
#endif

void AWidgetStyleDecoration::setMaterialTheme(ATheme::EThemeType type)
{
    switch (type)
    {
    case ATheme::Dark:
        mWinAgent->setWindowAttribute("dark-mode", true);
        break;
    case ATheme::Light:
        mWinAgent->setWindowAttribute("dark-mode", false);
        break;
    case ATheme::System:
        // TODO
        break;
    case ATheme::Custom:
        // TODO
        break;
    default:
        break;
    }
}

ATheme::EThemeType AWidgetStyleDecoration::getMaterialTheme() const
{
    bool isDark = mWinAgent->windowAttribute("dark-mode").toBool();
    if (isDark)
        return ATheme::Dark;

    // TODO

    return ATheme::Light;
}

QSharedPointer<AWidgetStyleDecoration> AWidgetStyleDecoration::Create(QWidget* widget)
{
    if (!widget)
        return nullptr;

    QSharedPointer<AWidgetStyleDecoration> wsd(new AWidgetStyleDecoration());
    wsd->initStyle(widget);

    return wsd;
}

void AWidgetStyleDecoration::initStyle(QWidget* self)
{
    Q_ASSERT(self);

    _host = self;
    mWinAgent = new QWK::WidgetWindowAgent(_host);
    mWinAgent->setup(_host);
}

#ifdef Q_OS_WIN

#include <Windows.h>

#define ARGB(a, r, g, b) (RGB(b, g, r) | ((BYTE)a << 24))

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

void WindowsBlurHelper(HWND hwnd, ACCENT_STATE state, const AWidgetStyleDecoration::SStyleOption& option)
{
    HMODULE hUser = GetModuleHandleA("user32.dll");
    if (!hUser)
        return;

    pfnSetWindowCompositionAttribute setWindowCompositionAttribute = (pfnSetWindowCompositionAttribute)GetProcAddress(hUser, "SetWindowCompositionAttribute");
    if (!setWindowCompositionAttribute)
        return;

    // 获取系统主题
    ATheme::EThemeType themeType = option.themeType;
    const AWidgetStyleDecoration::SWindowsThemeConfig config = AWidgetStyleDecoration::GetWindowsThemeConfig();
    if (option.themeType == ATheme::System)
    {
        if (config.appsUseLightTheme)
            themeType = ATheme::Light;
        else
            themeType = ATheme::Dark;
    }

    // 主题色
    QColor color;
    switch (themeType)
    {
    case ATheme::Dark:
        color = Qt::black;
        break;
    case ATheme::Light:
        color = Qt::white;
        break;
    case ATheme::Custom:
        color = option.color;
        break;
    default:
        break;
    }

    // 透明度
    if (!config.enableTransparency)
        color.setAlphaF(1.0);
    else if (themeType != ATheme::Custom)
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

void AWidgetStyleDecoration::ApplyAcrylic(QWidget* widget, const SStyleOption& option)
{
    if (nullptr == widget)
        return;
    widget->setAttribute(Qt::WA_TranslucentBackground);

    ApplyAcrylic(HWND(widget->winId()), option);
}

void AWidgetStyleDecoration::ApplyAcrylic(HWND hwnd, const SStyleOption& option)
{
    WindowsBlurHelper(hwnd, ACCENT_ENABLE_ACRYLICBLURBEHIND, option);
}

void AWidgetStyleDecoration::ApplyAero(QWidget* widget, const SStyleOption& option)
{
    if (nullptr == widget)
        return;
    widget->setAttribute(Qt::WA_TranslucentBackground);

    ApplyAero(HWND(widget->winId()), option);
}

void AWidgetStyleDecoration::ApplyAero(HWND hwnd, const SStyleOption& option)
{
    WindowsBlurHelper(hwnd, ACCENT_ENABLE_BLURBEHIND, option);
}

AWidgetStyleDecoration::SWindowsThemeConfig AWidgetStyleDecoration::GetWindowsThemeConfig()
{
    const QString strReg = "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
    const QSettings setting(strReg, QSettings::NativeFormat);

    SWindowsThemeConfig config;
    config.appsUseLightTheme = setting.value(AStr("AppsUseLightTheme")).toBool();
    config.enableTransparency = setting.value(AStr("EnableTransparency")).toBool();
    config.systemUsesLightTheme = setting.value(AStr("SystemUsesLightTheme")).toBool();

    return config;
}

#endif

APROCH_NAMESPACE_END
