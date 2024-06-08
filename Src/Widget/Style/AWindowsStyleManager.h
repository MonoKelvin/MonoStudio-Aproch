/****************************************************************************
 * @file    AWindowsStyleManager.h
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
#pragma once

#ifdef Q_OS_WINDOWS

#include <Windows.h>

APROCH_NAMESPACE_BEGIN

/**
 * @brief Windows样式管理器。只有Windows系统才可以使用各种样式设置
 *
 */
class APROCH_API AWindowsStyleManager
{
public:
    /**
     * @brief 主题类型
     */
    enum EThemeType
    {
        Dark,   // 深色
        Light,  // 浅色
        System, // 跟随系统
        Custom, // 自定义
    };

    /**
     * @brief Windows主题颜色配置
     */
    struct SWindowsThemeConfig
    {
        bool appsUseLightTheme;    // 应用主题是否是亮色主题，否则为深色
        bool enableTransparency;   // 是否开启透明度
        bool systemUsesLightTheme; // 系统主题是否是亮色主题，否则为深色
    };

    /** @brief Windows窗口样式配置 */
    struct SStyleOption
    {
        /** @brief 主题类型 */
        EThemeType themeType;

        /** @brief 自定义颜色类型（支持透明度），只有<ThemeType>为<EThemeType::Custom>时才起作用,
         * 如果系统关闭了透明度，则自定义的颜色设置透明度会失效
         */
        QColor color;

        /** @brief 设置样式的时候是否影响窗口阴影 */
        bool isEffectShadow;

        SStyleOption() noexcept
            : themeType(System), color(0, 0, 0, 240), isEffectShadow(false)
        {
        }
    };

    /**
     * @brief 为控件设置亚克力效果
     * @note 使用时尽量对一些静态的、内部没有文字等的子控件的控件使用，因为界面刷新后必须调整尺寸才能更新背景色，
     *       而且会出现刷新前后重叠的影像。
     * @param widget 控件
     * @param option 选项
     * @example
     *
     * QWidget widget;
     * AWindowsStyleManager::SStyleOption option;
     * option.themeType = AWindowsStyleManager::Custom;
     * option.color = "#FFFFFF";
     * option.color.setAlphaF(0.9);
     * AWindowsStyleManager::ApplyAcrylic(&widget, option);
     * widget.show();
     *
     * @return
     */
    static void ApplyAcrylic(QWidget *widget, const SStyleOption &option = SStyleOption());

    /**
     * @brief 为窗口句柄设置亚克力效果
     * @param hwnd 窗口句柄
     * @param option 选项
     * @return
     */
    static void ApplyAcrylic(HWND hwnd, const SStyleOption &option = SStyleOption());

    /**
     * @brief 为窗口设置Areo毛玻璃效果
     * @param widget 控件
     * @param option 选项
     * @return
     */
    static void ApplyAero(QWidget *widget, const SStyleOption &option = SStyleOption());

    /**
     * @brief 为窗口句柄设置Areo毛玻璃效果
     * @param hwnd 窗口句柄
     * @param option 选项
     * @return
     */
    static void ApplyAero(HWND hwnd, const SStyleOption &option = SStyleOption());

    /**
     * @brief 获取Windows主题配置
     * @return 主题配置
     */
    static SWindowsThemeConfig GetWindowsThemeConfig();

protected:
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

    typedef BOOL(WINAPI *pfnSetWindowCompositionAttribute)(HWND, WINDOWCOMPOSITIONATTRIBDATA *);

    static void WindowsBlurHelper(HWND hwnd, ACCENT_STATE state, const SStyleOption &option);
};

APROCH_NAMESPACE_END

#endif
