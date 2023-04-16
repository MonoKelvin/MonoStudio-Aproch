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
};

// ----------------------------------------------------------------------------------------------------

AAbstractDataManager::AAbstractDataManager(ADataContainer* parent)
    : QObject(*(new AAbstractDataManagerPrivate()), parent)
{
    Q_ASSERT_X(nullptr != parent, Q_FUNC_INFO, "parent ADataContainer is null");
}

AAbstractDataManager::~AAbstractDataManager()
{
    clear();
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

    Q_ASSERT_X(nullptr != d->dataSet, Q_FUNC_INFO, "dataSet is null");
}

void AAbstractDataManager::clear()
{
    Q_D(AAbstractDataManager);

    getDataContainer()->deleteData(d->dataSet);
}

AData* AAbstractDataManager::cloneData(AData* srcData)
{
    Q_D(AAbstractDataManager);
    if (!d->dataSet->contains(srcData))
        return nullptr;

    AData* newData = new AData(*srcData, ECopyOption::DeepCopy);
    getDataContainer()->addData(this, newData, getDefaultValue(srcData));

    initializeData(newData);

    return newData;
}

QVariant AAbstractDataManager::getDefaultValue(AData* dt) const
{
    Q_ASSERT(dt);

    static QVariant defValue = QVariant::Type(type());
    if (nullptr == dt || !dt->isValid())
        return defValue;

    return getDataContainer()->getDefaultValue(dt);
}

bool AAbstractDataManager::setDefaultValue(AData* dt, const QVariant& defaultVal) const
{
    return getDataContainer()->setDefaultValue(dt, defaultVal);
}

ADataSet* AAbstractDataManager::getDataSet() const
{
    Q_D(const AAbstractDataManager);
    Q_ASSERT(nullptr != d->dataSet);
    return d->dataSet;
}

const ADataSet& AAbstractDataManager::getDataSet()
{
    Q_D(const AAbstractDataManager);
    Q_ASSERT(nullptr != d->dataSet);
    return *d->dataSet;
}

ADataContainer* AAbstractDataManager::getDataContainer() const
{
    ADataContainer* containter = qobject_cast<ADataContainer*>(parent());
    Q_ASSERT_X(nullptr != containter, Q_FUNC_INFO, "The DataManager does not specify a DataContainer as a parent");
    return containter;
}

bool AAbstractDataManager::isValid() const
{
    Q_D(const AAbstractDataManager);
    return d->dataSet && QMetaType(type()).isValid();
}

AData* AAbstractDataManager::addData(const QString& name)
{
    return addData(name, getDefaultValue(nullptr));
}

AData* AAbstractDataManager::addData(const QString& name, const QVariant& defaultValue)
{
    Q_D(AAbstractDataManager);

    AData* data = createData();
    if (nullptr != data)
    {
        if (data->getType() != type())
        {
            blockSignals(true);
            delete data;
            blockSignals(false);

            return nullptr;
        }

        data->setName(name);

        getDataContainer()->addData(this, data, defaultValue);

        initializeData(data);
    }

    return data;
}

AData* AAbstractDataManager::createData()
{
    return new AData(this);
}

void AAbstractDataManager::destroyData(AData* data)
{
    Q_D(AAbstractDataManager);

    if (d->dataSet->contains(data))
    {
        uninitializeData(data);
        emit dataDestroyed(data);
        d->dataSet->remove(data);
    }
}

APROCH_API QDebug operator<<(QDebug dbg, const AAbstractDataManager& dataMgr)
{
    const char* clsName = dataMgr.metaObject()->className();

    if (!dataMgr.isValid())
        dbg << clsName << "(Invalid)";
    else
    {
        const char* typeName = QMetaType::typeName(dataMgr.type());
        dbg << QString("%1(id=%2, typename=%3)").arg(clsName).arg(dataMgr.type()).arg(dataMgr.type());
    }

    return dbg;
}

APROCH_NAMESPACE_END
