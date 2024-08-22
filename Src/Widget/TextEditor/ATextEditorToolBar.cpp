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
    // font
    d_ptr->fontComboBox = new QFontComboBox(this);
    d_ptr->fontComboBox->setObjectName("aproch-texteditor-tool-font");
    d_ptr->fontComboBox->setEditable(false);
    d_ptr->fontComboBox->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy::AdjustToMinimumContentsLengthWithIcon);

    QListView* listView = qobject_cast<QListView*>(d_ptr->fontComboBox->view());
    listView->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    listView->window()->setAttribute(Qt::WA_TranslucentBackground);
    //fontCb->setView(listView);

    // font size
    d_ptr->fontSizeComboBox = new AFontSizeComboBox(this);
    d_ptr->fontSizeComboBox->setObjectName("aproch-texteditor-tool-fontsize");

    // bold
    d_ptr->boldBtn = new QPushButton(this);
    d_ptr->boldBtn->setObjectName("aproch-texteditor-tool-bold");
    d_ptr->boldBtn->setIcon(AFontIcon::icon("\uE8DD"));
    d_ptr->boldBtn->setCheckable(true);
    d_ptr->boldBtn->setFixedSize(32, 32);

    // italic
    d_ptr->italicBtn = new QPushButton(this);
    d_ptr->italicBtn->setObjectName("aproch-texteditor-tool-italic");
    d_ptr->italicBtn->setIcon(AFontIcon::icon("\uE8DB"));
    d_ptr->italicBtn->setCheckable(true);
    d_ptr->italicBtn->setFixedSize(32, 32);

    // underline
    d_ptr->underlineBtn = new QPushButton(this);
    d_ptr->underlineBtn->setObjectName("aproch-texteditor-tool-underline");
    d_ptr->underlineBtn->setIcon(AFontIcon::icon("\uE8DC"));
    d_ptr->underlineBtn->setCheckable(true);
    d_ptr->underlineBtn->setFixedSize(32, 32);

    // strikeout
    d_ptr->strikethroughBtn = new QPushButton(this);
    d_ptr->strikethroughBtn->setObjectName("aproch-texteditor-tool-strikethrough");
    d_ptr->strikethroughBtn->setIcon(AFontIcon::icon("\uEDE0"));
    d_ptr->strikethroughBtn->setCheckable(true);
    d_ptr->strikethroughBtn->setFixedSize(32, 32);

    // color
    // TODO

    // background color
    // TODO

    QPushButton* h1Btn = nullptr;
    QPushButton* h2Btn = nullptr;
    QPushButton* h3Btn = nullptr;

    // image
    d_ptr->imageBtn = new QPushButton(this);
    d_ptr->imageBtn->setObjectName("aproch-texteditor-tool-image");
    d_ptr->imageBtn->setIcon(AFontIcon::icon("\uEB9F"));
    d_ptr->imageBtn->setFixedSize(32, 32);

    // link
    d_ptr->linkBtn = new QPushButton(this);
    d_ptr->linkBtn->setObjectName("aproch-texteditor-tool-link");
    d_ptr->linkBtn->setIcon(AFontIcon::icon("\uE71B"));
    d_ptr->linkBtn->setFixedSize(32, 32);

    // h1
    d_ptr->h1Btn = new QPushButton(this);
    d_ptr->h1Btn->setObjectName("aproch-texteditor-tool-h1");
    d_ptr->h1Btn->setIcon(AFontIcon::icon("\uF146"));   // TODO use H1
    d_ptr->h1Btn->setCheckable(true);
    d_ptr->h1Btn->setFixedSize(32, 32);

    // h2
    d_ptr->h2Btn = new QPushButton(this);
    d_ptr->h2Btn->setObjectName("aproch-texteditor-tool-h2");
    d_ptr->h2Btn->setIcon(AFontIcon::icon("\uF147"));   // TODO use H2
    d_ptr->h2Btn->setCheckable(true);
    d_ptr->h2Btn->setFixedSize(32, 32);

    // h3
    d_ptr->h3Btn = new QPushButton(this);
    d_ptr->h3Btn->setObjectName("aproch-texteditor-tool-h3");
    d_ptr->h3Btn->setIcon(AFontIcon::icon("\uF148"));   // TODO use H3
    d_ptr->h3Btn->setCheckable(true);
    d_ptr->h3Btn->setFixedSize(32, 32);

    AColorPickerIndicator* cpi = new AColorPickerIndicator();
    cpi->resize(800, 600);

    addWidget(d_ptr->fontComboBox);
    addWidget(d_ptr->fontSizeComboBox);
    addWidget(d_ptr->boldBtn);
    addWidget(d_ptr->italicBtn);
    addWidget(d_ptr->underlineBtn);
    addWidget(d_ptr->strikethroughBtn);
    addWidget(d_ptr->imageBtn);
    addWidget(d_ptr->linkBtn);
    addWidget(d_ptr->h1Btn);
    addWidget(d_ptr->h2Btn);
    addWidget(d_ptr->h3Btn);
    //addWidget(cpi);
}

bool ATextEditorToolBar::load(const QString& strUIFile)
{
    // TODO
    throw std::exception("method not implemented");
    return false;
}

APROCH_NAMESPACE_END
