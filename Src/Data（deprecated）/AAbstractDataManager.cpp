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
#include "ADataContainer.h"

#include <private/qobject_p.h>
#include <QIcon>

APROCH_NAMESPACE_BEGIN

class AAbstractDataManagerPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(AAbstractDataManager)
public:
    ADataSet* dataSet = nullptr;
    EMetaType type = EMetaType::UnknownType;
};

// ----------------------------------------------------------------------------------------------------

AAbstractDataManager::AAbstractDataManager(EMetaType type, ADataContainer* parent)
    : QObject(*(new AAbstractDataManagerPrivate()), parent)
{
    setType(type);

    if (nullptr != parent)
        parent->addDataManager(this, type);
}

AAbstractDataManager::~AAbstractDataManager()
{
    clear();
}

EMetaType AAbstractDataManager::getType() const
{
    Q_D(const AAbstractDataManager);
    return d->type;
}

void AAbstractDataManager::setType(EMetaType type)
{
    Q_D(AAbstractDataManager);
    d->type = type;
}

void AAbstractDataManager::init(ADataSet* dataset)
{
    Q_D(AAbstractDataManager);

    ADataContainer* container = getDataContainer();
    if (!container)
    {
        Q_ASSERT(false);
        return;
    }

    d->dataSet = dataset;
}

void AAbstractDataManager::clear()
{
    Q_D(AAbstractDataManager);

    if (nullptr != getDataContainer())
        getDataContainer()->deleteData(*d->dataSet);
}

AData* AAbstractDataManager::cloneData(AData* srcData)
{
    Q_ASSERT(getDataContainer());

    Q_D(AAbstractDataManager);
    if (!d->dataSet->contains(srcData))
        return nullptr;

    AData* newData = new AData(*srcData, ECopyOption::DeepCopy);
    getDataContainer()->addData(this, newData, getDefaultValue(srcData));

    return newData;
}

bool AAbstractDataManager::setValue(AData* dt, const QVariant& val)
{
    if (!val.canConvert(getType()))
    {
        Q_ASSERT_X(false, Q_FUNC_INFO, "Incompatible data type");
        return false;
    }

    dt->setValueInternal(val);

    return true;
}

QVariant AAbstractDataManager::getDefaultValue(AData* dt) const
{
    Q_ASSERT(getDataContainer());

    static QVariant defValue = QVariant::Type(getType());
    if (nullptr == dt || !dt->isValid())
        return defValue;

    return getDataContainer()->getDefaultValue(dt);
}

void AAbstractDataManager::resetValue(AData* dt)
{
    Q_ASSERT(getDataContainer());

    getDataContainer()->resetValue(dt);
}

ADataSet* AAbstractDataManager::getDataSet() const
{
    Q_D(const AAbstractDataManager);
    return d->dataSet;
}

const ADataSet& AAbstractDataManager::getDataSet()
{
    Q_D(const AAbstractDataManager);
    Q_ASSERT(nullptr != d->dataSet);
    return *d->dataSet;
}

ADataSet* AAbstractDataManager::getOrCreateDataSet()
{
    Q_ASSERT(getDataContainer());

    Q_D(AAbstractDataManager);

    if (nullptr == d->dataSet)
    {
        d->dataSet = getDataContainer()->getDataSet(this);
        Q_ASSERT(nullptr != d->dataSet);
    }

    return d->dataSet;
}

ADataContainer* AAbstractDataManager::getDataContainer() const
{
    return qobject_cast<ADataContainer*>(parent());
}

bool AAbstractDataManager::isValid() const
{
    Q_D(const AAbstractDataManager);
    return d->dataSet && QMetaType(getType()).isValid();
}

AData* AAbstractDataManager::addData(const QString& name)
{
    return addData(name, getDefaultValue(nullptr));
}

AData* AAbstractDataManager::addData(const QString& name, const QVariant& defaultValue)
{
    Q_ASSERT(getDataContainer());

    Q_D(AAbstractDataManager);

    AData* data = createData();
    if (nullptr != data)
    {
        if (data->getType() != getType())
        {
            blockSignals(true);
            delete data;
            blockSignals(false);

            return nullptr;
        }

        data->setName(name);

        if (!getDataContainer()->addData(this, data, defaultValue))
        {
            blockSignals(true);
            delete data;
            blockSignals(false);

            return nullptr;
        }
    }

    return data;
}

void AAbstractDataManager::initialize(ADataContainer*)
{
}

void AAbstractDataManager::initializeData(AData*)
{
}

AData* AAbstractDataManager::createData()
{
    return new AData(this);
}

APROCH_API QDebug operator<<(QDebug dbg, const AAbstractDataManager& dataMgr)
{
    const char* clsName = dataMgr.metaObject()->className();

    if (!dataMgr.isValid())
        dbg << clsName << "(Invalid)";
    else
    {
        const char* typeName = QMetaType::typeName(dataMgr.getType());
        dbg << QString("%1(id=%2, typename=%3)").arg(clsName).arg(dataMgr.getType()).arg(dataMgr.getType());
    }

    return dbg;
}

APROCH_NAMESPACE_END
