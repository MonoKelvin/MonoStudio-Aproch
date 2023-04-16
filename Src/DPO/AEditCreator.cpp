/****************************************************************************
 * @file    ABoxLayoutCreator.cpp
 * @date    2022-07-07
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
#include "AEditCreator.h"

APROCH_NAMESPACE_BEGIN

const QString aStr_LineEdit = "lineedit";
const QString aStr_Edit = "edit";
const QString aStr_TextEdit = "textedit";
const QString aStr_SpinEdit = "spinedit";
const QString aStr_SpinBox = "spinbox";
const QString aStr_DoubleSpinEdit = "doublespinedit";
const QString aStr_DoubleSpinBox = "doublespinbox";
const QString aStr_DateEdit = "dateedit";
const QString aStr_TimeEdit = "timeedit";
const QString aStr_DateTimeEdit = "datetimeedit";

AEditCreator::AEditCreator()
{
    addName(aStr_LineEdit);
    addName(aStr_Edit);
    addName(aStr_TextEdit);
    addName(aStr_SpinEdit);
    addName(aStr_SpinBox);
    addName(aStr_DoubleSpinEdit);
    addName(aStr_DoubleSpinBox);
    addName(aStr_DateEdit);
    addName(aStr_TimeEdit);
    addName(aStr_DateTimeEdit);
}

QObject *AEditCreator::createObject(const QString &name, IDPOData &data, QObject *parent)
{
    const QString objName = name.trimmed().toLower();
    QWidget *parWidget = qobject_cast<QWidget *>(parent);

    QObject *pEdit = nullptr;
    if (objName == aStr_LineEdit || objName == aStr_Edit)
        pEdit = new QLineEdit(parWidget);
    else if (objName == aStr_TextEdit)
        pEdit = new QTextEdit(parWidget);
    else if (objName == aStr_SpinEdit || objName == aStr_SpinBox)
        pEdit = new QSpinBox(parWidget);
    else if (objName == aStr_DoubleSpinEdit || objName == aStr_DoubleSpinBox)
        pEdit = new QDoubleSpinBox(parWidget);
    else if (objName == aStr_DateEdit)
        pEdit = new QDateEdit(parWidget);
    else if (objName == aStr_TimeEdit)
        pEdit = new QTimeEdit(parWidget);
    else if (objName == aStr_DateTimeEdit)
        pEdit = new QDateTimeEdit(parWidget);

    return pEdit;
}

APROCH_NAMESPACE_END
