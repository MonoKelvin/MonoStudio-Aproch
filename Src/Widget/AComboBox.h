/****************************************************************************
 * @file    AComboBox.h
 * @date    2024-12-31 
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
#include <QComboBox>

APROCH_NAMESPACE_BEGIN

class AComboBoxPrivate;

class APROCH_API AComboBox : public QComboBox
{
    Q_OBJECT;
    Q_PROPERTY(qreal shadowRadius READ getShadowRadius WRITE setShadowRadius);
    Q_PROPERTY(QSize shadowOffset READ getShadowOffset WRITE setShadowOffset);
    Q_PROPERTY(QColor shadowColor READ getShadowColor WRITE setShadowColor);
public:
    explicit AComboBox(QWidget* parent = nullptr);
    ~AComboBox();

    qreal getShadowRadius() const;
    void setShadowRadius(qreal r);

    QSize getShadowOffset() const;
    void setShadowOffset(const QSize& offset);

    QColor getShadowColor() const;
    void setShadowColor(const QColor& c);

protected:
    virtual bool eventFilter(QObject*, QEvent*) override;

protected:
    AComboBox(AComboBoxPrivate&, QWidget* parent = nullptr);

protected:
    AComboBoxPrivate* d_ptr = nullptr;

private:
    Q_DISABLE_COPY(AComboBox);
};


APROCH_NAMESPACE_END
