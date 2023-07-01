/****************************************************************************
 * @file    ARibbonButton.h
 * @date    2023-07-01 
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

#include <QToolButton>
#include "QtitanDef.h"

QTITAN_BEGIN_NAMESPACE

class RibbonButtonPrivate;
class APROCH_API RibbonButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(bool wordWrap READ wordWrap WRITE setWordWrap)
public:
    RibbonButton(QWidget* parent = nullptr);
    ~RibbonButton() override;
public:
    bool wordWrap() const;
    void setWordWrap(bool on);

    bool isLargeIcon() const;
    void setLargeIcon(bool large);

    bool simplifiedMode() const;
    void setSimplifiedMode(bool enabled);

    QSize sizeHint() const override;

    void changed();
protected:
    void updateIconSize();
protected:
    bool event(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void actionEvent(QActionEvent* event) override;
    void changeEvent(QEvent* event) override;
private:
    A_DECLARE_PRIVATE(RibbonButton)
    Q_DISABLE_COPY(RibbonButton)
};

QTITAN_END_NAMESPACE

#endif // A_RIBBONBUTTON_H
