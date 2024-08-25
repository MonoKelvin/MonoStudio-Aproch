/****************************************************************************
 * @file    ATextEditor.cpp
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
#include "stdafx.h"
#include "ATextEditor.h"
#include "AFontSizeComboBox.h"
#include "Widget/Private/ATextEditor_p.h"

#include <QFontComboBox>
#include <QTextCursor>
#include <QTextBlock>
#include <QTextDocumentFragment>
#include <QStackedLayout>
#include <QApplication>

APROCH_NAMESPACE_BEGIN

ATextEditor::ATextEditor(QWidget* parent)
    : QTextEdit(parent)
    , d_ptr(new ATextEditorPrivate)
{
    setAttribute(Qt::WA_StyledBackground);

    // format
    setTabWidth(4);
    setLineHeight(0);
    setSegmentSpacing(10);
}

void ATextEditor::setTabWidth(int w)
{
    QFontMetrics metrics(font());
    setTabStopDistance(w * metrics.averageCharWidth());
}

int ATextEditor::getTabWidth() const
{
    return tabStopDistance();
}

void ATextEditor::setLineHeight(int h)
{
    selectAll();
    QTextCursor currentCursor = textCursor();

    QTextBlockFormat blockFormat;
    blockFormat.setLineHeight(h, QTextBlockFormat::LineDistanceHeight);
    currentCursor.setBlockFormat(blockFormat);
    setTextCursor(currentCursor);

    currentCursor.clearSelection();
    setTextCursor(currentCursor);
}

int ATextEditor::getLineHeight() const
{
    return textCursor().blockFormat().lineHeight();
}

void ATextEditor::setSegmentSpacing(int s)
{
    selectAll();
    QTextCursor currentCursor = textCursor();

    QTextBlockFormat blockFormat;
    blockFormat.setBottomMargin(s);
    currentCursor.setBlockFormat(blockFormat);
    setTextCursor(currentCursor);

    currentCursor.clearSelection();
    setTextCursor(currentCursor);
}

int ATextEditor::getSegmentSpacing() const
{
    return textCursor().blockFormat().bottomMargin();
}

int ATextEditor::getLeftMargin() const
{
    return viewportMargins().left();
}

void ATextEditor::setLeftMargin(int margin)
{
    auto m = viewportMargins();
    m.setLeft(margin);
    setViewportMargins(m);

    QBoxLayout* theLayout = qobject_cast<QBoxLayout*>(layout());
    if (theLayout)
    {
        auto m = theLayout->contentsMargins();
        m.setLeft(margin);
        theLayout->setContentsMargins(m);
    }
}

int ATextEditor::getRightMargin() const
{
    return viewportMargins().right();
}

void ATextEditor::setRightMargin(int margin)
{
    auto m = viewportMargins();
    m.setRight(margin);
    setViewportMargins(m);

    QBoxLayout* theLayout = qobject_cast<QBoxLayout*>(layout());
    if (theLayout)
    {
        auto m = theLayout->contentsMargins();
        m.setRight(margin);
        theLayout->setContentsMargins(m);
    }
}

void ATextEditor::setDocumentMargins(int left, int top, int right, int bottom)
{
    setViewportMargins(left, top, right, bottom);
}

void ATextEditor::setDocumentMargins(const QMargins& margins)
{
    setViewportMargins(margins);
}

QMargins ATextEditor::getDocumentMargins() const
{
    return viewportMargins();
}

void ATextEditor::setFontFamily(const QString& family)
{
    setFocus();

    QTextCharFormat tcf = currentCharFormat();
    QFont font = tcf.font();
    font.setFamily(family);
    tcf.setFont(font);

    mergeCurrentCharFormat(tcf);
}

void ATextEditor::setFontSize(int size)
{
    setFocus();

    QTextCharFormat tcf = currentCharFormat();
    QFont font = tcf.font();
    font.setPixelSize(size);
    tcf.setFont(font);

    mergeCurrentCharFormat(tcf);
}

void ATextEditor::setBold(bool enabled)
{
    setFocus();

    QTextCharFormat tcf = currentCharFormat();
    QFont font = tcf.font();
    font.setBold(enabled);
    tcf.setFont(font);

    mergeCurrentCharFormat(tcf);
}

void ATextEditor::setItalic(bool enabled)
{
    setFocus();

    QTextCharFormat tcf = currentCharFormat();
    QFont font = tcf.font();
    font.setItalic(enabled);
    tcf.setFont(font);

    mergeCurrentCharFormat(tcf);
}

void ATextEditor::setUnderline(bool enabled)
{
    setFocus();

    QTextCharFormat tcf = currentCharFormat();
    QFont font = tcf.font();
    font.setUnderline(enabled);
    tcf.setFont(font);

    mergeCurrentCharFormat(tcf);
}

void ATextEditor::setStrikethrough(bool enabled)
{
    setFocus();

    QTextCharFormat tcf = currentCharFormat();
    QFont font = tcf.font();
    font.setStrikeOut(enabled);
    tcf.setFont(font);

    mergeCurrentCharFormat(tcf);
}

APROCH_NAMESPACE_END
