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
#include "Widget/Private/ATextEditor_p.h"

#include <QStackedLayout>

APROCH_NAMESPACE_BEGIN

ATextEditor::ATextEditor(QWidget* parent)
    : QTextEdit(parent)
    , d_ptr(new ATextEditorPrivate)
{
    d_ptr->toolBar = new ATextEditorToolBar(this);
    d_ptr->toolBar->setObjectName("aproch-texteditor-toolbar");

    d_ptr->titleEdit = new QLineEdit(this);
    d_ptr->titleEdit->setObjectName("aproch-texteditor-title");
    d_ptr->titleEdit->setPlaceholderText(tr("标题："));

    QStackedLayout* stackLayout = new QStackedLayout;
    stackLayout->addWidget(d_ptr->titleEdit);
    stackLayout->setStackingMode(QStackedLayout::StackAll);
    setLayout(stackLayout);

    d_ptr->firstViewMargin = viewportMargins();
}

ATextEditorToolBar* ATextEditor::getToolBar() const
{
    return d_ptr->toolBar;
}

bool ATextEditor::getToolBarVisible() const
{
    return d_ptr->toolBar ? d_ptr->toolBar->isVisible() : false;
}

void ATextEditor::setToolBarVisible(bool visible)
{
    if (!d_ptr->toolBar)
        return;
    d_ptr->toolBar->setVisible(visible);
}

void ATextEditor::resizeEvent(QResizeEvent* evt)
{
    if (d_ptr->toolBar)
    {
        d_ptr->toolBar->setGeometry(0, 0, width(), d_ptr->toolBar->height());
    }

    // update margins
    QMargins curMargins = viewportMargins();
    curMargins.setTop(d_ptr->firstViewMargin.top() + d_ptr->titleEdit->height());
    setViewportMargins(curMargins);

    QTextEdit::resizeEvent(evt);
}

APROCH_NAMESPACE_END
