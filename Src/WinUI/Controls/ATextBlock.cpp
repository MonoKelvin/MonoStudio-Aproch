/****************************************************************************
 * @file    ATextBlock.cpp
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
#include "stdafx.h"
#include "ATextBlock.h"

#include <QMetaProperty>

APROCH_NAMESPACE_BEGIN

class ATextBlockPrivate
{
public:
    ATextBlockPrivate();
    ~ATextBlockPrivate();

    QString m_text;
    Qt::TextElideMode m_elideMode = Qt::TextElideMode::ElideNone;
    bool m_settedText = false;
};

ATextBlockPrivate::ATextBlockPrivate()
{
}

ATextBlockPrivate::~ATextBlockPrivate()
{
}

// ----------------------------------------------------------------------------------------------------

ATextBlock::ATextBlock(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent, f)
    , d_ptr(new ATextBlockPrivate())
{
    setAlignment(Qt::AlignVCenter);
    //setWordWrap(true);
}

ATextBlock::ATextBlock(const QString& text, QWidget* parent, Qt::WindowFlags f)
    : ATextBlock(parent, f)
{
    setText(text);
    d_ptr->m_text;
}

ATextBlock::~ATextBlock()
{
}

void ATextBlock::setText(const QString& content, Qt::TextElideMode mode, bool isShowSrcTextTooltip)
{
    d_ptr->m_text = content;
    d_ptr->m_elideMode = mode;
    d_ptr->m_settedText = true;

	QString elidedText = content;
    QFontMetrics metrics = fontMetrics();
	if (metrics.horizontalAdvance(elidedText) > width())
    {
        elidedText = metrics.elidedText(elidedText, mode, width());
        if (elidedText != d_ptr->m_text && isShowSrcTextTooltip)
        {
            setToolTip(d_ptr->m_text);
        }
    }

    setText(elidedText);
}

QString ATextBlock::getNoElideText() const
{
    if (d_ptr->m_settedText)
        return d_ptr->m_text;
    return text();
}

void ATextBlock::resizeEvent(QResizeEvent* event)
{
    if (d_ptr->m_settedText)
        setText(d_ptr->m_text, d_ptr->m_elideMode);
    
    return QLabel::resizeEvent(event);
}

APROCH_NAMESPACE_END