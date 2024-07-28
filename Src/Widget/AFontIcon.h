/****************************************************************************
 * @file    AFontIcon.h
 * @date    2021-10-27
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
#include "AFontDatabase.h"

#include <QWidget>

APROCH_NAMESPACE_BEGIN

class AFontIconPrivate;

/**
 * @brief 图标控件
 */
class APROCH_API AFontIcon : public QWidget
{
    Q_OBJECT;
    Q_PROPERTY(QString glyph READ getGlyph WRITE setGlyph NOTIFY glyphChanged);
public:
    explicit AFontIcon(const QString& glyph, QWidget* parent = nullptr);
    AFontIcon(const QString& glyph, AFontDatabase::EAppFontType type, QWidget* parent = nullptr);
    AFontIcon(const QString& glyph, const QFont& font, QWidget* parent = nullptr);

    QString getGlyph() const;
    void setGlyph(const QString& glyph);

    static QIcon icon(const QString& glyph, const QSize& size = DefaultIconSize);
    static QIcon icon(const QString& glyph, AFontDatabase::EAppFontType type, const QColor& color, const QSize& size = DefaultIconSize);
    static QIcon icon(const QString& glyph, const QFont& font, const QColor& color, const QSize& size = DefaultIconSize);

    virtual QSize sizeHint() const override;

public:
    static QSize DefaultIconSize;

Q_SIGNALS:
    void glyphChanged();

protected:
    virtual void paintEvent(QPaintEvent* evt) override;

private:
    Q_DISABLE_COPY_MOVE(AFontIcon);
    QSharedPointer<AFontIconPrivate> d_ptr;
};

APROCH_NAMESPACE_END
