/****************************************************************************
 * @file    AFontComboBox.h
 * @date    2025-01-11 
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
#include "Widget/AComboBox.h"

#include <QFontComboBox>

APROCH_NAMESPACE_BEGIN

class AFontComboBoxPrivate;

class APROCH_API AFontComboBox : public AComboBox
{
    Q_OBJECT;
    Q_PROPERTY(QFontDatabase::WritingSystem writingSystem READ getWritingSystem WRITE setWritingSystem);
    Q_PROPERTY(QFontComboBox::FontFilters fontFilters READ getFontFilters WRITE setFontFilters);
    Q_PROPERTY(QFont currentFont READ getCurrentFont WRITE setCurrentFont NOTIFY currentFontChanged);
public:
    explicit AFontComboBox(QWidget* parent = nullptr);
    ~AFontComboBox();

    void setWritingSystem(QFontDatabase::WritingSystem);
    QFontDatabase::WritingSystem getWritingSystem() const;

    void setFontFilters(QFontComboBox::FontFilters filters);
    QFontComboBox::FontFilters getFontFilters() const;

    QFont getCurrentFont() const;

    void setSampleTextForSystem(QFontDatabase::WritingSystem writingSystem, const QString& sampleText);
    QString getSampleTextForSystem(QFontDatabase::WritingSystem writingSystem) const;

    void setSampleTextForFont(const QString& fontFamily, const QString& sampleText);
    QString getSampleTextForFont(const QString& fontFamily) const;

    void setDisplayFont(const QString& fontFamily, const QFont& font);
    std::optional<QFont> getDisplayFont(const QString& fontFamily) const;

public:
    virtual QSize sizeHint() const override;

public Q_SLOTS:
    void setCurrentFont(const QFont& f);

private Q_SLOTS:
    void _currentChanged(const QString&);
    void _updateModel();

Q_SIGNALS:
    void currentFontChanged(const QFont& f);

private:
    Q_DISABLE_COPY(AFontComboBox);
};

APROCH_NAMESPACE_END