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
#include "ATheme.h"

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
    struct SWinUIMaterialOption
    {
        /** @brief winui 风格材质 */
        EWinUIMaterial material;

        /** @brief 材质主题 */
        EThemeType theme;

        /** @brief 是否启用 */
        bool enabled = true;
    };

    /**
     * @brief 设置窗口背景材质
     * @note 要支持windows背景材质，qss样式表中需要添加：
     * ```qss
     * AWindow[has-material=true] {
     *     background: transparent;
     * }
     * ``` 
     * @param option 材质选项
     * @return 是否设置成功
     */
    virtual bool setWinUIMaterial(const SWinUIMaterialOption& option);

    /** @brief 获取窗口背景材质 */
    virtual SWinUIMaterialOption getWinUIMaterial();

    /** @brief 设置窗口预留的边框大小 */
    void setExtraMargins(const QMargins& margins);

    /** @brief 获取窗口预留的边框大小 */
    QMargins getExtraMargins() const;
#endif

protected:
    /** @brief 初始化样式设置 */
    void initStyle(QWidget* self);

protected:
    /** @brief 窗口代理，用于实现无边框窗口 */
    QWK::WidgetWindowAgent* mWinAgent = nullptr;

private:
    friend class AWinUIStyleHelper;
    QPointer<QWidget> _host;
    SWinUIMaterialOption _materialOption;
};

APROCH_NAMESPACE_END
