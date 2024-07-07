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
#include "Style/AWidgetStyleDecoration.h"

#include <QMainWindow>

APROCH_NAMESPACE_BEGIN

class AWindowPrivate;
class ACaptionBar;

/**
 * @brief 窗口
 */
class APROCH_API AWindow : public QMainWindow, public AWidgetStyleDecoration
{
    Q_OBJECT
public:
    explicit AWindow(QWidget *parent = nullptr);
    explicit AWindow(const FWindowCaptionWidgets& captionWidgets, QWidget* parent = nullptr);
    virtual ~AWindow();

    /**
     * @brief 以模态窗口的方式显示
     * @note 关闭后会删除该控件
     */
    int showModality();

    /** @brief 获取标题栏 */
    ACaptionBar* getCaptionBar(void) const;

    /** @brief 更新标题栏。当标题栏控件内容修改，如reset, setMinButton等之后需要重新更新标题栏 */
    void updateCaptionBar();

protected:
    virtual bool event(QEvent* evt) override;
    virtual void paintEvent(QPaintEvent *ev) override;
    virtual void closeEvent(QCloseEvent *ev) override;
    virtual void showEvent(QShowEvent* ev) override;

private:
    Q_DISABLE_COPY_MOVE(AWindow);
    QScopedPointer<AWindowPrivate> d_ptr;
};

APROCH_NAMESPACE_END
