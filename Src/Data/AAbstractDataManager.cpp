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

AAbstractDataManager::AAbstractDataManager(ADataContainer* parent, EMetaType type)
    : QObject(*(new AAbstractDataManagerPrivate()), parent)
{
    Q_ASSERT_X(nullptr != parent, Q_FUNC_INFO, "parent ADataContainer is null");
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

    //Q_ASSERT_X(nullptr != d->dataSet, Q_FUNC_INFO, "dataSet is null");
}

void AAbstractDataManager::clear()
{
    Q_D(AAbstractDataManager);

    if (nullptr != getDataContainer())
        getDataContainer()->deleteData(*d->dataSet);
}

AData* AAbstractDataManager::cloneData(AData* srcData)
{
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

    dt->setValue(val);

    return true;
}

QVariant AAbstractDataManager::getDefaultValue(AData* dt) const
{
    static QVariant defValue = QVariant::Type(getType());
    if (nullptr == dt || !dt->isValid())
        return defValue;

    return getDataContainer()->getDefaultValue(dt);
}

void AAbstractDataManager::resetValue(AData* dt)
{
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

void AAbstractDataManager::initializeData(AData* data)
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
