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

APROCH_NAMESPACE_BEGIN

class AThemePrivate;

/**
 * @brief Windows样式管理器。只有Windows系统才可以使用各种样式设置
 *
 */
class APROCH_API ATheme : public QObject
{
    Q_OBJECT
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

    ~ATheme();

    static void setTheme(EThemeType type);
    static EThemeType getTheme();

Q_SIGNALS:
    void themeChanged();

private:
    ATheme(QObject* parent = nullptr);

private:
    friend class AThemePrivate;
    QScopedPointer<AThemePrivate> d_ptr;
};

APROCH_NAMESPACE_END
