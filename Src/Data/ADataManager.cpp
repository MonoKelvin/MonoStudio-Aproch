/****************************************************************************
 * @file    ADataManager.cpp
 * @date    2022-7-16
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
#include "ADataManager.h"

APROCH_NAMESPACE_BEGIN

AIntegerDataManager::AIntegerDataManager(ADataContainer* parent)
    : AAbstractDataManager(parent, EMetaType::Int)
{
}

AIntegerDataManager::~AIntegerDataManager()
{
}

QString AIntegerDataManager::toText(const AData* dt) const
{
    if (!dt)
        return QString();

    return QString::number(dt->getValue().toInt());
}

// ----------------------------------------------------------------------------------------------------

ASizeDataManager::ASizeDataManager(ADataContainer* parent)
    : AAbstractDataManager(parent, EMetaType::QSize)
{
}

ASizeDataManager::~ASizeDataManager()
{
}

QString ASizeDataManager::toText(const AData* dt) const
{
    if (!dt)
        return QString();

    QSize size = dt->getValue().toSize();

    return AStr("(%1, %2)").arg(size.width()).arg(size.height());
}

void ASizeDataManager::initializeData(AData* data)
{
    AIntegerDataManager* intDataMgr = getDataContainer()->getManager<AIntegerDataManager>(EMetaType::Int);
    if (!intDataMgr)
    {
        Q_ASSERT_X(false, Q_FUNC_INFO, "no data manager of integer type");
        return;
    }

    AData* wData = intDataMgr->addData(tr("Width"));
    Q_ASSERT(nullptr != wData);
    data->addSubData(wData);

    AData* hData = intDataMgr->addData(tr("Height"));
    Q_ASSERT(nullptr != hData);
    data->addSubData(hData);
}

APROCH_NAMESPACE_END