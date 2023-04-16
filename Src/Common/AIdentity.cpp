/****************************************************************************
 * @file    AIdentity.cpp
 * @date    2021-8-12
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
#include "AIdentity.h"

#include <QMutex>

APROCH_NAMESPACE_BEGIN

QMutex aproch_id_generator_mutex;
volatile AIntegralNumberGenerator::value_type AIntegralNumberGenerator::sCurrentNumber = 1;

AIntegralNumberGenerator::value_type AIntegralNumberGenerator::fromString(const QString &val)
{
    bool ok = true;
    const volatile value_type tmpNum = val.toLongLong(&ok);
    if (!ok)
    {
        return get();
    }

    QMutexLocker locker(&aproch_id_generator_mutex);
    sCurrentNumber = qMax(sCurrentNumber, tmpNum);
    return tmpNum;
}

APROCH_NAMESPACE_END
