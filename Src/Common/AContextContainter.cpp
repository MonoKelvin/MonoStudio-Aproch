/****************************************************************************
 * @file    AContextContainter.cpp
 * @date    2023-05-26 
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
#include "AContextContainter.h"

APROCH_NAMESPACE_BEGIN

static QList<IContextObject*> g_contextObjMap;

IContextObject* AContextContainter::get(const QString& id)
{
    for (IContextObject* ctxObj : g_contextObjMap)
    {
        if (ctxObj && ctxObj->getContextId() == id)
            return ctxObj;
    }

    return nullptr;
}

IContextObject* AContextContainter::remove(const QString& id)
{
    for (IContextObject* ctxObj : g_contextObjMap)
    {
        if (ctxObj && ctxObj->getContextId() == id)
        {
            g_contextObjMap.removeAll(ctxObj);
            return ctxObj;
        }
    }

    return nullptr;
}

void AContextContainter::remove(IContextObject* ctxObj)
{
    g_contextObjMap.removeAll(ctxObj);
}

bool AContextContainter::add(IContextObject* ctxObj)
{
    if (g_contextObjMap.contains(ctxObj))
        return false;
    g_contextObjMap.push_front(ctxObj);
    return true;
}

APROCH_NAMESPACE_END
