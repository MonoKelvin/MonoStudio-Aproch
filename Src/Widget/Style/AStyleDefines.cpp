/****************************************************************************
 * @file    AStyleDefines.cpp
 * @date    2023-04-08 
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
#include "AStyleDefines.h"

#include <QProcess>

APROCH_NAMESPACE_BEGIN

// 
// The most of the following code is copied from Qtitan.
// 
// Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
// Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
// 

// Qtitan begin

#ifndef APROCH_NO_PROCESS
/*!
Returns system environment variable by \a key from current process.
*/
QString getSystemEnvironmentVariable(const QString& key)
{
    QStringList all = QProcess::systemEnvironment();
    for (QStringList::const_iterator it = all.constBegin(); it != all.constEnd(); ++it)
    {
        QStringList pair = it->split(QStringLiteral("="));
        if (pair.size() != 2)
            continue;
        if (key.compare(pair[0], Qt::CaseInsensitive) == 0)
            return pair[1];
    }
    return QString();
}

/*!
Returns environment variable by \a key from \a process.
*/
QString getEnvironmentVariable(QProcess* process, const QString& key)
{
    QStringList all = process->environment();
    for (QStringList::const_iterator it = all.constBegin(); it != all.constEnd(); ++it)
    {
        QStringList pair = it->split(QStringLiteral("="));
        if (pair.size() != 2)
            continue;
        if (key.compare(pair[0], Qt::CaseInsensitive) == 0)
            return pair[1];
    }
    return QString();
}

/*!
Sets environment variable using \a key and \a value for \a process.
*/
void setEnvironmentVariable(QProcess* process, const QString& key, const QString& value)
{
    QStringList all = process->environment();
    QStringList::iterator it = all.begin();
    for (; it != all.end(); ++it)
    {
        QStringList pair = it->split(QStringLiteral("="));
        if (pair.size() != 2)
            continue;
        if (key.compare(pair[0], Qt::CaseInsensitive) == 0)
            break;
    }
    if (it != all.end())
        *it = QStringLiteral("%1=%2").arg(key).arg(value);
    else
        all.push_back(QStringLiteral("%1=%2").arg(key).arg(value));
    process->setEnvironment(all);
}
#endif

// Qtitan end

void SetElidedText(QLabel* label, int maxWidth, Qt::TextElideMode mode)
{
    label->setFixedWidth(maxWidth);

    const auto fw = label->fontMetrics();
    const auto et = fw.elidedText(label->text(), mode, maxWidth - 3 * fw.averageCharWidth());

    label->setText(et);
    label->adjustSize();
}

APROCH_NAMESPACE_END
