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
#include "AprochExportDefine.h"

APROCH_NAMESPACE_BEGIN

class AThemePrivate;

/**
 * @brief 应用主题
 */
class APROCH_API ATheme : public QObject
{
    Q_OBJECT
public:
    ~ATheme();

    /** @brief 获取实例对象 */
    static ATheme* getInstance();

    /** @brief 设置应用主题类型 */
    static void setTheme(EThemeType type);

    /** @brief 设置自定义主题样式文件路径。设置后不会立即应用，需要调用 setTheme(EThemeType::Custom) */
    static bool setCustomThemeFile(const QString& strFileName);

    /** @brief 获取应用主题类型 */
    static EThemeType getTheme();

    /** @brief 获取系统主题类型 */
    static EThemeType getSystemTheme();

    /**
     * @brief 设置是否监控系统主题的变化。开启后系统主题变化时自动切换程序样式并发送 themeChanged() 信号
     * 注意：主题设置为跟随系统才启用，即调用: setThem(EThemeType::System)
     */
    static void setSystemThemeWatchEnabled(bool enabled);

Q_SIGNALS:
    void themeChanged();

public Q_SLOTS:
    /** @brief 重新加载主题样式 */
    void reload();

private:
    virtual bool eventFilter(QObject* obj, QEvent* event) override;

private:
    ATheme(QObject* parent = nullptr);

private:
    friend class AThemePrivate;
    QScopedPointer<AThemePrivate> d_ptr;
};

APROCH_NAMESPACE_END
