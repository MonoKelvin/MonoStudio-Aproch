/****************************************************************************
 * @file    ADataContainer.cpp
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
#include "ADataContainer.h"
#include "ADataManager.h"

#include <private/qobject_p.h>

#define A_DATALIST_USE_LINKLIST

#ifdef A_DATALIST_USE_LINKLIST
#include <QLinkedList>
#endif // A_DATALIST_USE_LINKLIST

APROCH_NAMESPACE_BEGIN

namespace
{
    static void visitDataPath(AData* cur, ADataPath& path, ADataPathList& pathList)
    {
        if (!cur)
            return;

        path.insert(0, cur);

        const ADataSet& parentSet = cur->getParentDataSetRef();
        if (parentSet.isEmpty())
        {
            pathList.push_back(path);
            return;
        }

        for (const auto& nextPar : parentSet)
        {
            ADataPath newPath = path;
            visitDataPath(nextPar, newPath, pathList);
        }
    }

    static void visitDataPath(AData* cur, const AData* root, ADataPath& path, ADataPathList& pathList, const bool isFindAll)
    {
        if (!cur || (!isFindAll && !pathList.isEmpty()))
            return;

        path.insert(0, cur);

        if (cur == root)
        {
            pathList.push_back(path);
            return;
        }

        const ADataSet& parentSet = cur->getParentDataSetRef();
        for (const auto& nextPar : parentSet)
        {
            ADataPath newPath = path;
            visitDataPath(nextPar, root, newPath, pathList, isFindAll);

            if (!isFindAll && !pathList.isEmpty())
                return;
        }
    }

}

class ADataContainerPrivate : QObjectPrivate
{
    Q_DECLARE_PUBLIC(ADataContainer)
public:
    ADataContainerPrivate(int version = QObjectPrivateVersion)
        : QObjectPrivate(version)
    {
    }
    ~ADataContainerPrivate()
    {
    }

    struct DataValues
    {
        QVariant defaultVal;

        // others
    };

    void destroyed();
    bool hasDataManager(AAbstractDataManager* manager, bool& hasPtr) const;
    void addDataManager(AAbstractDataManager* manager, EMetaType type);
    void deleteData(AData* dt);
    void addData(AAbstractDataManager* manager, AData* dt, const QVariant& defaultVal);
    bool setDefaultValue(AData* dt, const QVariant& defaultVal);

#ifdef A_DATALIST_USE_LINKLIST
    QLinkedList<AData*> dataList;
#else
    QSet<AData*> dataList;
#endif
    QHash<AData*, DataValues> valuesMap;
    QMap<AAbstractDataManager*, ADataSet*> managerDataSetMap;
};

void ADataContainerPrivate::destroyed()
{
    qDeleteAll(dataList);
    qDeleteAll(managerDataSetMap);
}

bool ADataContainerPrivate::hasDataManager(AAbstractDataManager* manager, bool& hasPtr) const
{
    hasPtr = false;
    for (auto iter = managerDataSetMap.constBegin(); iter != managerDataSetMap.constEnd(); ++iter)
    {
        if (iter.key() == manager)
        {
            hasPtr = true;
            return true;
        }

        if (hasPtr)
            continue;

        const char* mgrClsName = iter.key()->metaObject()->className();
        const char* managerClsName = manager->metaObject()->className();
        if (strcmp(mgrClsName, managerClsName) == 0)
            return true;
    }

    return false;
}

void ADataContainerPrivate::addDataManager(AAbstractDataManager* manager, EMetaType type)
{
    Q_Q(ADataContainer);

    ADataSet* dataset = new ADataSet();
    managerDataSetMap[manager] = dataset;

    manager->setType(type);
    manager->setParent(q);
    manager->init(dataset);
    manager->initialize(q);
}

void ADataContainerPrivate::deleteData(AData* dt)
{
    Q_Q(ADataContainer);

#ifdef A_DATALIST_USE_LINKLIST
    dataList.removeOne(dt);
#else
    dataList.remove(dt);
#endif
    valuesMap.remove(dt);
    for (auto iter = managerDataSetMap.begin(); iter != managerDataSetMap.end(); ++iter)
        iter.value()->remove(dt);

    emit q->dataDestroyed(dt);

    delete dt;
    dt = nullptr;
}

void ADataContainerPrivate::addData(AAbstractDataManager* manager, AData* dt, const QVariant& defaultVal)
{
    dataList.push_back(dt);
    valuesMap[dt].defaultVal = defaultVal;
    managerDataSetMap[manager]->insert(dt);

    manager->initializeData(dt);
}

bool ADataContainerPrivate::setDefaultValue(AData* dt, const QVariant& defaultVal)
{
    Q_Q(ADataContainer);

    if (valuesMap[dt].defaultVal == defaultVal)
        return false;

    QVariant old = valuesMap[dt].defaultVal;
    valuesMap[dt].defaultVal = defaultVal;

    emit q->defaultValueChanged(dt, old);

    return true;
}

// ----------------------------------------------------------------------------------------------------

ADataContainer::ADataContainer(QObject* parent)
    : QObject(*(new ADataContainerPrivate()), parent)
{
}

ADataContainer::ADataContainer(const QList<AAbstractDataManager*>& dataMgrList, QObject* parent)
    : QObject(*(new ADataContainerPrivate()), parent)
{
    for (AAbstractDataManager* dm : dataMgrList)
        addDataManager(dm);
}

ADataContainer::~ADataContainer()
{
    Q_D(ADataContainer);

    d->destroyed();
}

bool ADataContainer::addDataManager(AAbstractDataManager* manager)
{
    if (nullptr == manager)
        return false;

    return addDataManager(manager, manager->getType());
}

bool ADataContainer::addDataManager(AAbstractDataManager* manager, EMetaType type)
{
    if (!QMetaType(type).isValid())
    {
        qWarning("Invalid data type: %d", type);
        return false;
    }

    Q_D(ADataContainer);

    for (auto iter = d->managerDataSetMap.constBegin(); iter != d->managerDataSetMap.constEnd(); ++iter)
    {
        bool hasPtr = false;
        if (!d->hasDataManager(manager, hasPtr))
            continue;

        if (hasPtr)
            return true;

        const char* clsName = manager->metaObject()->className();
        const char* typeName = QMetaType::typeName(type);
        qWarning("The same DataType(typename=%s, classname=%s) of data manager exists.", typeName, clsName);
        return false;
    }

    d->addDataManager(manager, type);
    
    return true;
}

AAbstractDataManager* ADataContainer::getDataManager(EMetaType type) const
{
    Q_D(const ADataContainer);

    for (auto iter = d->managerDataSetMap.constBegin(); iter != d->managerDataSetMap.constEnd(); ++iter)
    {
        if (iter.key()->getType() != type)
            continue;

        return iter.key();
    }

    return nullptr;
}

bool ADataContainer::setValue(AData* data, const QVariant& val)
{
    if (!data)
        return false;

    return data->setValue(val);
}

QVariant ADataContainer::getDefaultValue(AData* dt) const
{
    Q_D(const ADataContainer);
    return d->valuesMap.value(dt).defaultVal;
}

bool ADataContainer::setDefaultValue(AData* dt, const QVariant& defaultVal)
{
    if (nullptr == dt)
        return false;

    Q_D(ADataContainer);
    if (!d->dataList.contains(dt))
        return false;

    // data type is incompatible
    if (!defaultVal.isValid() || !defaultVal.canConvert(dt->getType()))
        return false;

    if (!d->setDefaultValue(dt, defaultVal))
        return false;

    return true;
}

void ADataContainer::resetValue(AData* dt)
{
    if (nullptr == dt)
        return;

    Q_D(const ADataContainer);
    for (auto iter = d->managerDataSetMap.begin(); iter != d->managerDataSetMap.end(); ++iter)
    {
        if (iter.value()->contains(dt))
        {
            iter.key()->setValue(dt, getDefaultValue(dt));
            break;
        }
    }
}

void ADataContainer::resetValues(const ADataSet& dataSet)
{
    for (AData* dt : dataSet)
        resetValue(dt);
}

ADataSet* ADataContainer::getDataSet(AAbstractDataManager* manager, bool isCreateIfNull)
{
    Q_D(const ADataContainer);
    if (!d->managerDataSetMap.contains(manager))
    {
        if (isCreateIfNull)
            addDataManager(manager, manager->getType());
        else
            return nullptr;
    }

    return d->managerDataSetMap.value(manager);
}

ADataSet ADataContainer::getAllData() const
{
    Q_D(const ADataContainer);

#ifdef A_DATALIST_USE_LINKLIST
    ADataSet dataSet;
    for (AData* dt : d->dataList)
        dataSet.insert(dt);
    
    return dataSet;
#else
    return d->dataList;
#endif
}

bool ADataContainer::addData(AAbstractDataManager* manager, AData* dt, const QVariant& defaultVal)
{
    if (nullptr == manager || nullptr == dt)
        return false;
    
    Q_D(ADataContainer);

    bool hasPtr = true;
    if (!d->hasDataManager(manager, hasPtr) || d->dataList.contains(dt))
        return false;

    d->addData(manager, dt, defaultVal);

    return true;
}

AData* ADataContainer::addData(EMetaType type, const QString& name)
{
    AAbstractDataManager* dm = getDataManager(type);
    if (!dm)
        return nullptr;

    return dm->addData(name);
}

AData* ADataContainer::cloneData(AData* srcData)
{
    // todo
    return nullptr;
}

void ADataContainer::deleteData(ADataSet& dataset)
{
    Q_D(ADataContainer);
    ADataSet tempDataset;

    if (d->managerDataSetMap.values().contains(&dataset))
    {
        tempDataset.swap(dataset);
    }
    else
    {
        for (ADataSet* ds : d->managerDataSetMap)
        {
            for (auto iter = dataset.begin(); iter != dataset.end(); )
            {
                if (ds->remove(*iter))
                {
                    tempDataset.insert(*iter);
                    iter = dataset.erase(iter);
                }
                else
                {
                    ++iter;
                }
            }
        }
    }

    for (AData* dt : tempDataset)
        d->deleteData(dt);
}

void ADataContainer::deleteData(AData* dt)
{
    if (nullptr == dt)
        return;

    Q_D(ADataContainer);
    d->deleteData(dt);
}

ADataPathList ADataContainer::getDataPath(AData* data, AData* root, bool isFindAll)
{
    ADataPathList result;
    
    if (!data)
        return result;

    ADataPath tmpSet;
    if (root == nullptr)
    {
        if (isFindAll)
        {
            visitDataPath(data, tmpSet, result);
            return result;
        }

        AData* curPar = data;
        while(curPar)
        {
            if (tmpSet.contains(curPar))
                break;

            tmpSet.insert(0, curPar);
            curPar = *data->getParentDataSetRef().begin();
        }

        result.push_back(tmpSet);
    }
    else
    {
        visitDataPath(data, root, tmpSet, result, isFindAll);
    }

    return result;
}


APROCH_NAMESPACE_END
