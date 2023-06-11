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

// ----------------------------------------------------------------------------------------------------

const ADataManagerRegistry::Constructor ADataManagerRegistry::Null = [](QObject*) -> AAbstractDataManager* { return nullptr; };
ADataManagerRegistry::ConstructorMap ADataManagerRegistry::MyConstructorMap;

ADataManagerRegistry::ADataManagerRegistry() noexcept
{
}

bool ADataManagerRegistry::registerDataManager(int type, Constructor constructor)
{
    if (constructors().contains(type))
    {
        qWarning() << AStr("The data manager of type '%1' has been registered and will be overwritten "
                           "by a new data manager.").arg(QString::fromLatin1(QMetaType::typeName(type)));
    }

    constructors()[type] = constructor;
    return true;
}

void ADataManagerRegistry::unregister(int type)
{
    constructors().remove(type);
}

const ADataManagerRegistry::Constructor& ADataManagerRegistry::constructor(int type)
{
    if (constructors().contains(type))
        return constructors()[type];
    return Null;
}

ADataManagerRegistry::ConstructorMap& ADataManagerRegistry::constructors() noexcept
{
    return MyConstructorMap;
}

// ----------------------------------------------------------------------------------------------------

class ADataManagerContainerPrivate
{
public:
    QMap<int, AAbstractDataManager*> dataManagerMap;
};

ADataManagerContainer::ADataManagerContainer(QObject* parent)
    : QObject(parent)
    , d_ptr(new ADataManagerContainerPrivate)
{

}

ADataManagerContainer::~ADataManagerContainer()
{
    removeAll();
}

AAbstractDataManager* ADataManagerContainer::getDataManager(int type)
{
    AAbstractDataManager* dataManager = d_ptr->dataManagerMap.value(type, nullptr);
    if (nullptr != dataManager)
        return dataManager;

    dataManager = ADataManagerRegistry::newDataManager(type, this);
    if (nullptr == dataManager)
    {
        qWarning() << AStr("The data manager of type '%1' does not exist.").arg(type);
        return nullptr;
    }

    connect(dataManager, &AAbstractDataManager::destroyed, this, &ADataManagerContainer::onDataManagerDestroyed);

    // TODO: 支持多线程
    d_ptr->dataManagerMap[type] = dataManager;

    emit dataManagerAdded(dataManager);

    return dataManager;
}

void ADataManagerContainer::removeDataManager(int type)
{
    AAbstractDataManager* dataManager = d_ptr->dataManagerMap.value(type, nullptr);
    if (nullptr != dataManager)
        delete dataManager;
}

void ADataManagerContainer::removeAll()
{
    QMap<int, AAbstractDataManager*> tempMap = d_ptr->dataManagerMap;
    d_ptr->dataManagerMap.clear();

    qDeleteAll(tempMap);
}

void ADataManagerContainer::onDataManagerDestroyed(QObject* obj)
{
    for (auto iter = d_ptr->dataManagerMap.begin(); iter != d_ptr->dataManagerMap.end(); ++iter)
    {
        if (iter.value() == obj)
        {
            d_ptr->dataManagerMap.erase(iter);
            return;
        }
    }
}

APROCH_NAMESPACE_END
