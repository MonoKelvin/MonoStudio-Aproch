/****************************************************************************
 * @file    ADoubleValidator.cpp
 * @date    2021-11-17
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
#include "ADoubleValidator.h"

APROCH_NAMESPACE_BEGIN

ADoubleValidator::ADoubleValidator(QObject *parent)
    : QDoubleValidator(parent)
{
    setNotation(QDoubleValidator::StandardNotation);
}

ADoubleValidator::ADoubleValidator(double bottom, double top, int decimals, QObject *parent)
    : QDoubleValidator(bottom, top, decimals, parent)
{
    setNotation(QDoubleValidator::StandardNotation);
}

QValidator::State ADoubleValidator::validate(QString &str, int &i) const
{
    if (str.isEmpty())
        return QValidator::Intermediate;

    bool cOK = false;
    const double val = str.toDouble(&cOK);

    if (!cOK)
        return QValidator::Invalid;

    const int dotPos = str.indexOf(".");
    if (dotPos > 0)
    {
        if (str.right(str.length() - dotPos - 1).length() > decimals())
            return QValidator::Invalid;
    }

    if (val <= top() && val >= bottom())
        return QValidator::Acceptable;

    return QValidator::Invalid;
}

void ADoubleValidator::fixup(QString &s) const
{
    if (s.toDouble() < bottom())
    {
        s = QString::number(bottom(), 'g', decimals());
    }
    else if (s.toDouble() > top())
    {
        s = QString::number(top(), 'g', decimals());
    }
}

APROCH_NAMESPACE_END
