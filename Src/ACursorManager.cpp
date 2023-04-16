/****************************************************************************
 * @file    ACursorManager.cpp
 * @date    2021-9-26
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
#include "ACursorManager.h"

#include <QApplication>

APROCH_NAMESPACE_BEGIN

APROCH_INIT_SINGLETON(ACursorManager);

ACursorManager::ACursorManager()
    : mDefaultCursor(Qt::ArrowCursor)
{
}

void ACursorManager::setDefault(const QCursor &cursor)
{
    mDefaultCursor = cursor;
}

QCursor ACursorManager::getDefault(void) const
{
    return mDefaultCursor;
}

void ACursorManager::setCurrent(const QCursor &cursor, bool isNoPushIfSameShape)
{
    if (isNoPushIfSameShape && !mCursorStack.empty() && mCursorStack.top().shape() == cursor.shape())
        return;
    mCursorStack.push(cursor);
}

QCursor ACursorManager::getCurrent(void) const
{
    if (mCursorStack.empty())
        return mDefaultCursor;

    return mCursorStack.top();
}

QCursor ACursorManager::pop()
{
    if (mCursorStack.empty())
        return mDefaultCursor;
    return mCursorStack.pop();
}

QCursor ACursorManager::getLast(void) const
{
    if (mCursorStack.size() < 2)
        return mDefaultCursor;

    return mCursorStack.at(mCursorStack.size() - 2);
}

void ACursorManager::SetGlobalCursor(const QCursor &cursor)
{
    QApplication::setOverrideCursor(cursor);
}

void ACursorManager::RestoreGlobalCursor()
{
    QApplication::restoreOverrideCursor();
}

void ACursorManager::DynamicCursor(bool condition, const QCursor &cursor)
{
    static bool isModified = false;
    if (condition)
    {
        if (!isModified)
        {
            ACursorManager::SetGlobalCursor(cursor);
            isModified = true;
#ifdef _DEBUG
            static int s = 1;
            qDebug() << "setOverrideCursor: " << s++;
#endif
        }
    }
    else if (isModified)
    {
        ACursorManager::RestoreGlobalCursor();
        isModified = false;
#ifdef _DEBUG
        static int s = 1;
        qDebug() << "restoreOverrideCursor: " << s++;
#endif
    }
}

APROCH_NAMESPACE_END
