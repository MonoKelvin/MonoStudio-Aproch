/****************************************************************************
 * @file    APromptWidget.h
 * @date    2022-06-03
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

/**
 * @brief 提示控件类。
 */
class APROCH_API APromptWidget : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(APromptWidget)

public:
    /**
     * @brief 提示控件的类型
     */
    enum EPromptType
    {
        Alert, // 警告
        Prompt // 普通的提示
    };
    Q_ENUM(EPromptType);

    /**
     * @brief 构造函数
     * @param prompt 文本内容
     * @param parent 要显示在那个父对象上
     * @param type 提示控件的类型 @see EPromptType
     * @param duration 等待时长
     * @note 一般在构建时只要通过new构建出来即可，不许过多步骤，显示完后会自动释放
     * @example
     *
     * QWidget mainWindow;
     * mainWindow.show();
     *
     * new PromptWidget("This is an alert!", &mainWindow, EPromptType::Alert);
     *
     */
    APromptWidget(const QString &content, QWidget *parent, EPromptType type = Prompt, int duration = 3000);

protected:
    /**
     * @brief 构建UI方法
     * @param prompt 文本内容
     */
    /*virtual */ void buildUI(const QString &prompt);

    /**
     * @brief 显示方法
     */
    // virtual  void show();

    void paintEvent(QPaintEvent *) override;
    void showEvent(QShowEvent *) override;

    /**
     * @brief 调整文本内容（增加换行），使之可达到的宽度为给定的最大宽度
     * @param maxWidth 最大宽度
     * @param font 文本的字体
     * @param text 文本
     * @return 调整后的文本大小
     */
    QSize adjustText(int maxWidth, const QFont &font, QString &text);

private:
    /** @brief 内容文本 */
    QLabel *mContentLabel;

    /** @brief 停留时长 */
    int mDuration;
};

APROCH_NAMESPACE_END
