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

#include <QWidget>

class QEventLoop;

namespace QWK
{
    class WidgetWindowAgent;
}

APROCH_NAMESPACE_BEGIN

class ACaptionBar;

/**
 * @brief 自定义窗口组件
 */
class APROCH_API AWindow : public QWidget
{
    Q_OBJECT
public:
    explicit AWindow(QWidget *parent = nullptr, Qt::WindowType type = Qt::Window);
    virtual ~AWindow();

    /**
     * @brief 以模态窗口的方式显示
     * @note 关闭后会删除该控件
     */
    int showModality();

    /**
     * 获取标题栏
     * @return AMenuBar* 标题栏控件
     */
    inline ACaptionBar *getCaptionBar(void) const noexcept
    {
        return mCaptionBar;
    }

protected:
    virtual bool event(QEvent* evt) override;
    virtual void paintEvent(QPaintEvent *ev) override;
    virtual void closeEvent(QCloseEvent *ev) override;

protected:
    /** @brief 标题栏 */
    ACaptionBar *mCaptionBar;

    /** @brief 阻塞事件循环，用于实现模态窗口 */
    QEventLoop *mEventLoop;

private:
    /** @brief 窗口代理，用于实现无边框窗口 */
    QWK::WidgetWindowAgent* mWinAgent = nullptr;
};

APROCH_NAMESPACE_END
