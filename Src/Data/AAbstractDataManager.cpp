/****************************************************************************
 * @file    AAbstractDataManager.cpp
 * @date    2023-05-14
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
#include "AAbstractDataManager.h"

APROCH_NAMESPACE_BEGIN

const QString AAbstractDataManager::constraintAttribute(QLatin1String("constraint"));
const QString AAbstractDataManager::singleStepAttribute(QLatin1String("singleStep"));
const QString AAbstractDataManager::decimalsAttribute(QLatin1String("decimals"));
const QString AAbstractDataManager::enumIconsAttribute(QLatin1String("enumIcons"));
const QString AAbstractDataManager::enumNamesAttribute(QLatin1String("enumNames"));
const QString AAbstractDataManager::flagNamesAttribute(QLatin1String("flagNames"));
const QString AAbstractDataManager::maximumAttribute(QLatin1String("maximum"));
const QString AAbstractDataManager::minimumAttribute(QLatin1String("minimum"));
const QString AAbstractDataManager::regExpAttribute(QLatin1String("regExp"));

class AAbstractDataManagerPrivate
{
public:
    QSet<AData*> m_dataList;
};

// ----------------------------------------------------------------------------------------------------

AAbstractDataManager::AAbstractDataManager(QObject* parent)
    : QObject(parent), d_ptr(new AAbstractDataManagerPrivate)
{
}

AAbstractDataManager::~AAbstractDataManager()
{
    clear();
}

void AAbstractDataManager::clear() const
{
    while (!d_ptr->m_dataList.isEmpty())
        delete* d_ptr->m_dataList.cbegin();
}

QSet<AData*> AAbstractDataManager::getDataList() const
{
    return d_ptr->m_dataList;
}

bool AAbstractDataManager::hasValue(const AData* data) const
{
    Q_UNUSED(data);
    return true;
}

QIcon AAbstractDataManager::valueIcon(const AData* data) const
{
    Q_UNUSED(data);
    return QIcon();
}

QString AAbstractDataManager::toString(const AData* data) const
{
    Q_UNUSED(data);
    return QString();
}

AData* AAbstractDataManager::addData(const QString& name)
{
    AData* data = createData();
    if (data)
    {
        data->setName(name);
        d_ptr->m_dataList.insert(data);
        initializeData(data);
    }

    return data;
}

QVariant AAbstractDataManager::getAttribute(AData* data, const QString& attribute) const
{
    Q_UNUSED(data);
    Q_UNUSED(attribute);
    return QVariant();
}

void AAbstractDataManager::setAttribute(AData* data, const QString& attribute, const QVariant& value)
{
    Q_UNUSED(data);
    Q_UNUSED(attribute);
    Q_UNUSED(value);
}

AData* AAbstractDataManager::createData()
{
    return new AData(this);
}

void AAbstractDataManager::uninitializeData(AData* data)
{
    Q_UNUSED(data);
}

void AAbstractDataManager::_destroyData(AData* data)
{
    if (d_ptr->m_dataList.contains(data))
    {
        emit dataDestroyed(data);
        uninitializeData(data);
        d_ptr->m_dataList.remove(data);
    }
}

APROCH_NAMESPACE_END
