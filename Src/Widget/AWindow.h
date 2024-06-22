/****************************************************************************
 * @file    AWindow.h
 * @date    2021-1-24
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
#include <QMainWindow>

namespace QWK
{
    class WidgetWindowAgent;
}

APROCH_NAMESPACE_BEGIN

class ACaptionBar;

/**
 * @brief 窗口
 */
class APROCH_API AWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit AWindow(QWidget *parent = nullptr, Qt::WindowType type = Qt::Window);
    virtual ~AWindow();

    /** @brief 获取标题栏 */
    ACaptionBar* getCaptionBar(void) const;

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
    bool setBackgroundMaterial(EWinBackgroundMaterial bkMaterial, bool on);

    /** @brief 获取窗口背景材质 */
    EWinBackgroundMaterial getBackgroundMaterial() const;
#endif

protected:
    virtual bool event(QEvent* evt) override;
    virtual void paintEvent(QPaintEvent *ev) override;
    virtual void closeEvent(QCloseEvent *ev) override;

protected:
    /** @brief 窗口代理，用于实现无边框窗口 */
    QWK::WidgetWindowAgent* mWinAgent = nullptr;
};

APROCH_NAMESPACE_END
