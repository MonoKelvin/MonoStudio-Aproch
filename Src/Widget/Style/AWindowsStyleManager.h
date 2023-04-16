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
 * @brief Windows��ʽ��������ֻ��Windowsϵͳ�ſ���ʹ�ø�����ʽ����
 *
 */
class APROCH_API AWindowsStyleManager
{
public:
    /**
     * @brief ��������
     */
    enum EThemeType
    {
        Dark,   // ��ɫ
        Light,  // ǳɫ
        System, // ����ϵͳ
        Custom, // �Զ���
    };

    /**
     * @brief Windows������ɫ����
     */
    struct SWindowsThemeConfig
    {
        bool appsUseLightTheme;    // Ӧ�������Ƿ�����ɫ���⣬����Ϊ��ɫ
        bool enableTransparency;   // �Ƿ���͸����
        bool systemUsesLightTheme; // ϵͳ�����Ƿ�����ɫ���⣬����Ϊ��ɫ
    };

    /** @brief Windows������ʽ���� */
    struct SStyleOption
    {
        /** @brief �������� */
        EThemeType themeType;

        /** @brief �Զ�����ɫ���ͣ�֧��͸���ȣ���ֻ��<ThemeType>Ϊ<EThemeType::Custom>ʱ��������,
         * ���ϵͳ�ر���͸���ȣ����Զ������ɫ����͸���Ȼ�ʧЧ
         */
        QColor color;

        /** @brief ������ʽ��ʱ���Ƿ�Ӱ�촰����Ӱ */
        bool isEffectShadow;

        SStyleOption() noexcept
            : themeType(System), color(0, 0, 0, 240), isEffectShadow(false)
        {
        }
    };

    /**
     * @brief Ϊ�ؼ������ǿ���Ч��
     * @note ʹ��ʱ������һЩ��̬�ġ��ڲ�û�����ֵȵ��ӿؼ��Ŀؼ�ʹ�ã���Ϊ����ˢ�º��������ߴ���ܸ��±���ɫ��
     *       ���һ����ˢ��ǰ���ص���Ӱ��
     * @param widget �ؼ�
     * @param option ѡ��
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
     * @brief Ϊ���ھ�������ǿ���Ч��
     * @param hwnd ���ھ��
     * @param option ѡ��
     * @return
     */
    static void ApplyAcrylic(HWND hwnd, const SStyleOption &option = SStyleOption());

    /**
     * @brief Ϊ��������Areoë����Ч��
     * @param widget �ؼ�
     * @param option ѡ��
     * @return
     */
    static void ApplyAero(QWidget *widget, const SStyleOption &option = SStyleOption());

    /**
     * @brief Ϊ���ھ������Areoë����Ч��
     * @param hwnd ���ھ��
     * @param option ѡ��
     * @return
     */
    static void ApplyAero(HWND hwnd, const SStyleOption &option = SStyleOption());

    /**
     * @brief ��ȡWindows��������
     * @return ��������
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
