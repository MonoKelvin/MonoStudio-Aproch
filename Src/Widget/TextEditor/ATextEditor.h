/****************************************************************************
 * @file    ATextEditor.h
 * @date    2024-08-17 
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

#include <QTextEdit>

APROCH_NAMESPACE_BEGIN

class ATextEditorPrivate;

class APROCH_API ATextEditor : public QTextEdit
{
    Q_OBJECT;
    Q_PROPERTY(int tabWidth READ getTabWidth WRITE setTabWidth);
    Q_PROPERTY(int lineHeight READ getLineHeight WRITE setLineHeight);
    Q_PROPERTY(int segmentSpacing READ getSegmentSpacing WRITE setSegmentSpacing);
    Q_PROPERTY(int leftMargin READ getLeftMargin WRITE setLeftMargin);
    Q_PROPERTY(int rightMargin READ getRightMargin WRITE setRightMargin);

public:
    explicit ATextEditor(QWidget* parent = nullptr);

    void setTabWidth(int w);
    int getTabWidth() const;

    void setLineHeight(int h);
    int getLineHeight() const;

    void setSegmentSpacing(int s);
    int getSegmentSpacing() const;

    int getLeftMargin() const;
    void setLeftMargin(int margin);

    int getRightMargin() const;
    void setRightMargin(int margin);

    void setDocumentMargins(int left, int top, int right, int bottom);
    void setDocumentMargins(const QMargins& margins);
    QMargins getDocumentMargins() const;

public Q_SLOTS:
    void setFontFamily(const QString& family);
    void setFontSize(int size);
    void setBold(bool enabled);
    void setItalic(bool enabled);
    void setUnderline(bool enabled);
    void setStrikethrough(bool enabled);

private:
    Q_DISABLE_COPY_MOVE(ATextEditor);
    QSharedPointer<ATextEditorPrivate> d_ptr;
};

APROCH_NAMESPACE_END
