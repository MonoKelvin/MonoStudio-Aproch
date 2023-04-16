#include "stdafx.h"
#include "ADataContainer.h"

#include <private/qobject_p.h>

#define A_DATALIST_USE_LINKLIST

#ifdef A_DATALIST_USE_LINKLIST
#include <QLinkedList>
#endif // A_DATALIST_USE_LINKLIST

APROCH_NAMESPACE_BEGIN

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
        qDeleteAll(dataList);
        qDeleteAll(managerDataSetMap);
    }

    struct DataValues
    {
        QVariant defaultVal;

        // others
    };

    bool hasDataManager(AAbstractDataManager* manager, bool& hasPtr) const;
    void addDataManager(AAbstractDataManager* manager);
    void deleteData(AData* dt);
    void addData(AAbstractDataManager* manager, AData* dt, const QVariant& defaultVal);

#ifdef A_DATALIST_USE_LINKLIST
    QLinkedList<AData*> dataList;
#else
    QSet<AData*> dataList;
#endif
    QHash<AData*, DataValues> valuesMap;
    QMap<AAbstractDataManager*, ADataSet*> managerDataSetMap;
};

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

void ADataContainerPrivate::addDataManager(AAbstractDataManager* manager)
{
    Q_Q(ADataContainer);

    manager->setParent(q);

    ADataSet* dataset = new ADataSet();
    managerDataSetMap[manager] = dataset;
    manager->init(dataset);
}

void ADataContainerPrivate::deleteData(AData* dt)
{
#ifdef A_DATALIST_USE_LINKLIST
    dataList.removeOne(dt);
#else
    dataList.remove(dt);
#endif
    valuesMap.remove(dt);
    for (auto iter = managerDataSetMap.begin(); iter != managerDataSetMap.end(); ++iter)
        iter.value()->remove(dt);

    delete dt;
    dt = nullptr;
}

void ADataContainerPrivate::addData(AAbstractDataManager* manager, AData* dt, const QVariant& defaultVal)
{
    dataList.push_back(dt);
    valuesMap[dt].defaultVal = defaultVal;
    managerDataSetMap[manager]->insert(dt);
}

// ----------------------------------------------------------------------------------------------------

ADataContainer::ADataContainer(QObject* parent)
    : QObject(*(new ADataContainerPrivate()), parent)
{
}

ADataContainer::~ADataContainer()
{
}

void ADataContainer::addManager(AAbstractDataManager* manager)
{
    if (nullptr == manager)
        return;

    if (!QMetaType(manager->type()).isValid())
    {
        qWarning("Invalid data type: %d", manager->type());
        return;
    }

    Q_D(ADataContainer);

    for (auto iter = d->managerDataSetMap.constBegin(); iter != d->managerDataSetMap.constEnd(); ++iter)
    {
        bool hasPtr = false;
        if (!d->hasDataManager(manager, hasPtr))
            continue;

        if (hasPtr)
            return;

        const char* clsName = manager->metaObject()->className();
        const char* typeName = QMetaType::typeName(manager->type());
        qWarning("The same EDataType(typename=%s, classname=%s) of data manager exists.", typeName, clsName);
        return;
    }

    d->addDataManager(manager);
}

AAbstractDataManager* ADataContainer::getManager(EMetaType type, const char* className) const
{
    Q_D(const ADataContainer);

    for (auto iter = d->managerDataSetMap.constBegin(); iter != d->managerDataSetMap.constEnd(); ++iter)
    {
        if (iter.key()->type() != type)
            continue;

        const char* mgrClsName = iter.key()->metaObject()->className();
        if (strcmp(mgrClsName, className) == 0)
            return iter.key();
    }

    return nullptr;
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
    if (dt->getType() != defaultVal.type() || !defaultVal.isValid())
        return false;
    
    d->valuesMap[dt].defaultVal = defaultVal;
    return true;
}

void ADataContainer::resetValue(AData* dt)
{
    if (nullptr == dt)
        return;

    dt->setValue(getDefaultValue(dt));
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
            addManager(manager);
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

AData* ADataContainer::cloneData(AData* srcData)
{
    // todo
    return nullptr;
}

void ADataContainer::deleteData(ADataSet* dataset)
{
    Q_D(ADataContainer);
    for (ADataSet* ds : d->managerDataSetMap)
    {
        if (ds == dataset)
        {
            qDeleteAll(*ds);
            ds->clear();
        }
    }
}

void ADataContainer::deleteData(AData*& dt)
{
    if (nullptr == dt)
        return;

    Q_D(ADataContainer);
    d->deleteData(dt);
}

APROCH_NAMESPACE_END
