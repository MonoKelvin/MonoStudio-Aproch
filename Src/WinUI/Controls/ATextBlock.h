/****************************************************************************
 * @file    ATextBlock.h
 * @date    2023-04-05 
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
#ifndef ATEXTBLOCK_H
#define ATEXTBLOCK_H
#include <QLabel>

APROCH_NAMESPACE_BEGIN

class ATextBlockPrivate;
class APROCH_API ATextBlock : public QLabel
{
    Q_OBJECT
public:
    explicit ATextBlock(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    explicit ATextBlock(const QString& text, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~ATextBlock();

    using QLabel::setText;

    /**
     * @brief 设置文本内容
     * @param content 文本内容
     * @param mode 省略模式
     * @param isShowSrcTextTooltip 是否显示原始字符串的工具提示
     */
    void setText(const QString& content, Qt::TextElideMode mode, bool isShowSrcTextTooltip = false);

    /** @brief 获取省略前的原始字符串 */
    QString getNoElideText() const;

protected:
    virtual void resizeEvent(QResizeEvent* event);

private:
    QScopedPointer<ATextBlockPrivate> d_ptr;

private:
    Q_DISABLE_COPY(ATextBlock)
};

APROCH_NAMESPACE_END

#endif // ATEXTBLOCK_H
