﻿/****************************************************************************
 * @file    AFlowLayout.h
 * @date    2021-1-23
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

#include <QLayout>
#include <QStyle>

APROCH_NAMESPACE_BEGIN

/**
 * 流式布局
 */
class APROCH_API AFlowLayout : public QLayout
{
public:
    explicit AFlowLayout(QWidget *parent);
    ~AFlowLayout();

    void addItem(QLayoutItem *item) override;

    int horizontalSpacing() const;
    int verticalSpacing() const;

    void setHorizontalSpacing(int value);
    void setVerticalSpacing(int value);

    Qt::Orientations expandingDirections() const override;

    bool hasHeightForWidth() const override;
    int heightForWidth(int width) const override;

    int count() const override;
    QLayoutItem *itemAt(int index) const override;

    QSize minimumSize() const override;
    void setGeometry(const QRect &rect) override;
    QSize sizeHint() const override;

    QLayoutItem *takeAt(int index) override;

private:
    int doLayout(const QRect &rect, bool testOnly) const;
    int smartSpacing(QStyle::PixelMetric pm) const;

private:
    /** item列表 */
    QList<QLayoutItem *> mItemList;

    /** 水平间隔 */
    int mHorizontalSpacing;

    /** 垂直间隔 */
    int mVerticalSpacing;
};

APROCH_NAMESPACE_END
