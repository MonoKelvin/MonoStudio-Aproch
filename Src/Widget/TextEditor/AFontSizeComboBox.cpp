/****************************************************************************
 * @file    AFontSizeComboBox.cpp
 * @date    2024-08-18 
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
#include "AFontSizeComboBox.h"
#include <private/qcombobox_p.h>

APROCH_NAMESPACE_BEGIN

const QList<int> sFontSizeList = {
    5,6,7,8,9,10,11,12,14,16,18,20,22,24,26,28,36,48,56,72
};

class AFontSizeComboBoxPrivate
{
public:
};

AFontSizeComboBox::AFontSizeComboBox(QWidget* parent)
    : QComboBox(parent)
    , d_ptr(new AFontSizeComboBoxPrivate())
{
    for (int s : sFontSizeList)
        addItem(QString::number(s));

    QListView* listView = new QListView(this);
    setView(listView);

    listView->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    listView->window()->setAttribute(Qt::WA_TranslucentBackground);
    
    setCurrentFontSize(14); // = 10.5 = 五号
    setMaxVisibleItems(16);
    setMaxCount(sFontSizeList.size());
    setEditable(true);
    setValidator(new QIntValidator(1, 1638, this));

    connect(this, &AFontSizeComboBox::currentTextChanged, 
            this, &AFontSizeComboBox::_currentTextChanged);
}

AFontSizeComboBox::~AFontSizeComboBox()
{
}

int AFontSizeComboBox::getCurrentFontSize() const
{
    return currentText().toInt();
}

void AFontSizeComboBox::setCurrentFontSize(int s)
{
    setCurrentText(QString::number(s));
}

void AFontSizeComboBox::_currentTextChanged(const QString& text)
{
    if (text != currentText())
        emit currentFontSizeChanged(text.toInt());
}

APROCH_NAMESPACE_END
