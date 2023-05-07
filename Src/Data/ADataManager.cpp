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

// ----------------------------------------------------------------------------------------------------

ASizeDataManager::ASizeDataManager(ADataContainer* parent)
    : AAbstractDataManager(parent, EMetaType::QSize)
{
    connect(parent, &ADataContainer::valueChanged, this, &ASizeDataManager::onValueChanged);
    connect(parent, &ADataContainer::dataDestroyed, this, &ASizeDataManager::onDataDestroyed);
}

QString ASizeDataManager::toText(const AData* dt) const
{
    if (!dt)
        return QString();

    QSize size = dt->getValue().toSize();

    return AStr("(%1, %2)").arg(size.width()).arg(size.height());
}

AData* ASizeDataManager::getDataFromWidth(AData* dt) const
{
    for (auto iter = m_dataToW.cbegin(); iter != m_dataToW.cend(); ++iter)
    {
        if (dt == iter.value())
            return iter.key();
    }

    return nullptr;
}

AData* ASizeDataManager::getDataFromHeight(AData* dt) const
{
    for (auto iter = m_dataToH.cbegin(); iter != m_dataToH.cend(); ++iter)
    {
        if (dt == iter.value())
            return iter.key();
    }

    return nullptr;
}

AData* ASizeDataManager::getWidth(AData* dt) const
{
    return m_dataToW.value(dt, nullptr);
}

AData* ASizeDataManager::getHeight(AData* dt) const
{
    return m_dataToH.value(dt, nullptr);
}

void ASizeDataManager::initializeData(AData* data)
{
    AIntegerDataManager* intDataMgr = getDataContainer()->getDataManager<AIntegerDataManager>(EMetaType::Int);
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

    m_dataToW[data] = wData;
    m_dataToH[data] = hData;
}

void ASizeDataManager::onValueChanged(AData* data, const QVariant& oldVal)
{
    if (AData* sizeData = getDataFromWidth(data))
    {
        QSize newSize = sizeData->getValue().toSize();
        newSize.setWidth(data->getValue().toInt());
        setValue(sizeData, newSize);
    }
    else if (AData* sizeData = getDataFromHeight(data))
    {
        QSize newSize = sizeData->getValue().toSize();
        newSize.setHeight(data->getValue().toInt());
        setValue(sizeData, newSize);
    }
    else 
    {
        AData* width = getWidth(data);
        if (width && width->getDataManager())
            width->getDataManager()->setValue(width, data->getValue().toSize().width());

        AData* height = getHeight(data);
        if (height && height->getDataManager())
            height->getDataManager()->setValue(height, data->getValue().toSize().height());
    }
}

void ASizeDataManager::onDataDestroyed(AData* data)
{
    if (AData* sizeData = getDataFromWidth(data))
    {
        m_dataToW.remove(sizeData);
    }
    else if (AData* sizeData = getDataFromHeight(data))
    {
        m_dataToH.remove(sizeData);
    }
    else
    {
        getDataContainer()->deleteData(m_dataToW.value(data, nullptr));
        m_dataToW.remove(data);
        
        getDataContainer()->deleteData(m_dataToH.value(data, nullptr));
        m_dataToH.remove(data);
    }
}

// ----------------------------------------------------------------------------------------------------

QString AStringListDataManager::toText(const AData* dt) const
{
    QStringList stringList = dt->getValue().toStringList();
    if (stringList.isEmpty())
        return QString();

    return AStr("(") + stringList.join(QLatin1String(", ")) + AStr(")");
}

APROCH_NAMESPACE_END
