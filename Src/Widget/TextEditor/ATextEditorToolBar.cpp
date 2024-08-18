/****************************************************************************
 * @file    ATextEditorToolBar.cpp
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
#include "ATextEditorToolBar.h"
#include "Widget/Private/ATextEditorToolBar_p.h"

#include <QFontComboBox>
#include <QListView>

APROCH_NAMESPACE_BEGIN

ATextEditorToolBar::ATextEditorToolBar(QWidget* parent)
    : QToolBar(parent)
    , d_ptr(new ATextEditorToolBarPrivate)
{
    QFontComboBox* fontCb = new QFontComboBox(this);
    fontCb->setEditable(false);
    fontCb->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy::AdjustToMinimumContentsLengthWithIcon);

    QListView* listView = qobject_cast<QListView*>(fontCb->view());
    listView->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    listView->window()->setAttribute(Qt::WA_TranslucentBackground);
    //fontCb->setView(listView);

    addWidget(fontCb);

    AFontSizeComboBox* fontSizeCb = new AFontSizeComboBox(this);
    addWidget(fontSizeCb);
}

bool ATextEditorToolBar::load(const QString& strUIFile)
{
    // TODO
    throw std::exception("method not implemented");
    return false;
}

APROCH_NAMESPACE_END
