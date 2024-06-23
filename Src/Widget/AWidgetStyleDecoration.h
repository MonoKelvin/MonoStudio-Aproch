/****************************************************************************
 * @file    AWidgetStyleDecoration.h
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
#pragma once
#include "Style/ATheme.h"

namespace QWK
{
    class WidgetWindowAgent;
}

APROCH_NAMESPACE_BEGIN

/**
 * @brief 窗口
 */
class APROCH_API AWidgetStyleDecoration
{
public:
    AWidgetStyleDecoration();
    virtual ~AWidgetStyleDecoration();

#ifdef Q_OS_WIN
    /**
     * @brief 设置窗口背景材质
     * @note 要支持windows背景材质，qss样式表中需要添加：
     * ```qss
     * AWindow[has-material=true] {
     *     background: transparent;
     * }
     * ``` 
     * @param bkMaterial 材质类型
     * @param on         是否开启，开启后会将之前设置过的材质样式清空
     * @return 是否设置成功
     */
    bool setBackgroundMaterial(EWinBackgroundMaterial bkMaterial, bool on = true);

    /** @brief 获取窗口背景材质 */
    EWinBackgroundMaterial getBackgroundMaterial() const;
#endif

    /** @brief 设置材质主题色 */
    void setMaterialTheme(ATheme::EThemeType type);
    
    /** @brief 获取材质主题色 */
    ATheme::EThemeType getMaterialTheme() const;

public:
    /** @brief 创建样式 */
    static QSharedPointer<AWidgetStyleDecoration> Create(QWidget* widget);

#ifdef Q_OS_WIN

    /** @brief Windows窗口样式配置 */
    struct SStyleOption
    {
        /** @brief 主题类型 */
        ATheme::EThemeType themeType;

        /** @brief 自定义颜色类型（支持透明度），只有<ThemeType>为<EThemeType::Custom>时才起作用,
         * 如果系统关闭了透明度，则自定义的颜色设置透明度会失效
         */
        QColor color;

        /** @brief 设置样式的时候是否影响窗口阴影 */
        bool isEffectShadow;

        SStyleOption() noexcept
            : themeType(ATheme::System), color(0, 0, 0, 240), isEffectShadow(false)
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
    static void ApplyAcrylic(QWidget* widget, const SStyleOption& option = SStyleOption());

    /**
     * @brief 为窗口句柄设置亚克力效果
     * @param hwnd 窗口句柄
     * @param option 选项
     * @return
     */
    static void ApplyAcrylic(HWND hwnd, const SStyleOption& option = SStyleOption());

    /**
     * @brief 为窗口设置Areo毛玻璃效果
     * @param widget 控件
     * @param option 选项
     * @return
     */
    static void ApplyAero(QWidget* widget, const SStyleOption& option = SStyleOption());

    /**
     * @brief 为窗口句柄设置Areo毛玻璃效果
     * @param hwnd 窗口句柄
     * @param option 选项
     * @return
     */
    static void ApplyAero(HWND hwnd, const SStyleOption& option = SStyleOption());

    /**
     * @brief Windows主题颜色配置
     */
    struct SWindowsThemeConfig
    {
        bool appsUseLightTheme;    // 应用主题是否是亮色主题，否则为深色
        bool enableTransparency;   // 是否开启透明度
        bool systemUsesLightTheme; // 系统主题是否是亮色主题，否则为深色
    };

    /**
     * @brief 获取Windows主题配置
     * @return 主题配置
     */
    static SWindowsThemeConfig GetWindowsThemeConfig();
#endif

protected:
    /** @brief 初始化样式设置 */
    void initStyle(QWidget* self);

protected:
    /** @brief 窗口代理，用于实现无边框窗口 */
    QWK::WidgetWindowAgent* mWinAgent = nullptr;

private:
    QPointer<QWidget> _host;
};

APROCH_NAMESPACE_END
