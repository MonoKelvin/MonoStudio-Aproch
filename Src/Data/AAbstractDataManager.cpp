#include "stdafx.h"
#include "AAbstractDataManager.h"

#include <QIcon>

namespace aproch
{
    AAbstractDataManager::AAbstractDataManager(QObject* parent)
        : QObject(parent)
    {
    }

    AAbstractDataManager::~AAbstractDataManager()
    {
        clear();
    }

    void AAbstractDataManager::clear() const
    {
        while (!m_dataSet.isEmpty())
            delete* m_dataSet.cbegin();
    }

    QSet<AData*> AAbstractDataManager::dataSet() const
    {
        return m_dataSet;
    }

    AData* AAbstractDataManager::addData(const QString& name)
    {
        AData* data = createData();
        if (data)
        {
            data->setDataName(name);
            m_dataSet.insert(data);
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

    void AAbstractDataManager::destroyData(AData* data)
    {
        if (m_dataSet.contains(data))
        {
            emit dataDestroyed(data);
            uninitializeData(data);
            m_dataSet.remove(data);
        }
    }
}