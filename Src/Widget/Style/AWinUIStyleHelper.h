/****************************************************************************
 * @file    AStyleHelper.h
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
#pragma once
#include "AprochExportDefine.h"

APROCH_NAMESPACE_BEGIN

#ifdef Q_OS_WIN

/** @brief winui样式工具类 */
class APROCH_API AWinUIStyleHelper
{
public:
    /** @brief 创建控件装饰器样式 */
    static QSharedPointer<AWidgetStyleDecoration> createStyleDecoration(QWidget* widget);

    /** @brief Windows窗口样式配置 */
    struct SStyleOption
    {
        /** @brief 主题类型 */
        EThemeType themeType;

        /** @brief 自定义颜色类型（支持透明度），只有<ThemeType>为<EThemeType::Custom>时才起作用,
         * 如果系统关闭了透明度，则自定义的颜色设置透明度会失效
         */
        QColor color;

        SStyleOption() noexcept
            : themeType(EThemeType::System), color(Qt::black)
        {
        }

        SStyleOption(const EThemeType& theme, const QColor& color) noexcept
            : themeType(theme), color(color)
        {
        }
    };

    /**
     * @brief 为控件设置亚克力效果
     * @note 使用时尽量对一些静态的、内部没有文字等的子控件的控件使用，因为界面刷新后必须调整尺寸才能更新背景色，
     *       而且会出现刷新前后重叠的影像。
     */
    static void acrylic(QWidget* widget, const SStyleOption& option = SStyleOption());

    /** @brief 为窗口句柄设置亚克力效果 */
    static void acrylic(HWND hwnd, const SStyleOption& option = SStyleOption());

    /** @brief 为窗口设置Areo毛玻璃效果 */
    static void aero(QWidget* widget, const SStyleOption& option = SStyleOption());

    /** @brief 为窗口句柄设置Areo毛玻璃效果 */
    static void aero(HWND hwnd, const SStyleOption& option = SStyleOption());

    /** @brief 为窗口设置Mica效果，仅支持win11版本 */
    static bool mica(QWidget* widget, const SStyleOption& option = SStyleOption(), bool on = true);

    /** @brief 为窗口句柄设置Mica材质效果，仅支持win11版本 */
    static bool mica(HWND hwnd, const SStyleOption& option = SStyleOption(), bool on = true);

    /** @brief 为窗口设置Mica Alt材质效果，仅支持win11版本 */
    static bool micaAlt(QWidget* widget, const SStyleOption& option = SStyleOption(), bool on = true);

    /** @brief 为窗口句柄设置Mica Alt材质效果，仅支持win11版本 */
    static bool micaAlt(HWND hwnd, const SStyleOption& option = SStyleOption(), bool on = true);

    /**
     * @brief 主题配置
     */
    struct SThemeConfig
    {
        bool appsUseLightTheme;    // 应用主题是否是亮色主题，否则为深色
        bool systemUsesLightTheme; // 系统主题是否是亮色主题，否则为深色
        bool enableTransparency;   // 是否开启透明度
    };

    /**
     * @brief 获取Windows主题配置
     * @return 主题配置
     */
    static SThemeConfig getThemeConfig();
};
#endif

APROCH_NAMESPACE_END