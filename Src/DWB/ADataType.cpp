#include "stdafx.h"
#include "ADataType.h"

namespace aproch
{
    TDataTypeMap ADataType::m_dataTypeMap;

    ADataType::ADataType()
        : m_typeId(0)
        , m_name(QObject::tr("NONEYPE"))
    {
    }
    
    ADataType::~ADataType()
    {
    }

    bool ADataType::isValid() const
    {
        return m_typeId > 0 && !m_name.trimmed().isEmpty();
    }

    bool ADataType::operator==(const ADataType& rhs) const
    {
        return m_typeId == rhs.m_typeId || m_name == rhs.m_name;
    }

    bool ADataType::operator!=(const ADataType& rhs) const
    {
        return m_typeId != rhs.m_typeId || m_name != rhs.m_name;
    }

    bool ADataType::operator>(const ADataType& rhs) const
    {
        return m_typeId > rhs.m_typeId;
    }

    bool ADataType::operator>=(const ADataType& rhs) const
    {
        return m_typeId >= rhs.m_typeId;
    }

    bool ADataType::operator<(const ADataType& rhs) const
    {
        return m_typeId < rhs.m_typeId;
    }

    bool ADataType::operator<=(const ADataType& rhs) const
    {
        return m_typeId <= rhs.m_typeId;
    }

    ADataType ADataType::Add(TDataTypeId id, const QString& name, const QString& description, const QIcon& icon)
    {
        ADataType dataType;
        dataType.m_typeId = id;
        dataType.m_name = name;
        dataType.m_description = description;
        dataType.m_icon = icon;

        if (dataType.isValid())
        {
            auto iter = m_dataTypeMap.find(id);
            if (iter == m_dataTypeMap.end())
                m_dataTypeMap.insert(id, dataType);
            else
                *iter = dataType;
        }
        return dataType;
    }

    void ADataType::Remove(TDataTypeId id)
    {
        m_dataTypeMap.remove(id);
    }

    ADataType ADataType::Get(TDataTypeId id)
    {
        auto iter = m_dataTypeMap.constFind(id);
        if (iter == m_dataTypeMap.constEnd())
            return ADataType();
        return *iter;
    }

    const TDataTypeMap& ADataType::GetAllType()
    {
        return m_dataTypeMap;
    }

    bool ADataType::SetName(TDataTypeId id, const QString& newName)
    {
        if (newName.trimmed().isEmpty())
            return false;

        auto iter = m_dataTypeMap.find(id);
        if (iter == m_dataTypeMap.end())
            return false;
        iter->m_name = newName;
        return true;
    }

    bool ADataType::SetDescription(TDataTypeId id, const QString& newDescription)
    {
        auto iter = m_dataTypeMap.find(id);
        if (iter == m_dataTypeMap.end())
            return false;
        iter->m_description = newDescription;
        return true;
    }

    bool ADataType::SetIcon(TDataTypeId id, const QIcon& newIcon)
    {
        auto iter = m_dataTypeMap.find(id);
        if (iter == m_dataTypeMap.end())
            return false;
        iter->m_icon = newIcon;
        return true;
    }

    QDebug operator<<(QDebug dbg, const ADataType& dataType)
    {
        QDebugStateSaver saver(dbg);
        dbg.nospace();

        dbg << "ADataType";
        if (dataType.getId() == 0)
        {
            if (!dataType.getName().isEmpty())
                dbg << ":" << dataType.getName() << " " << QObject::tr("is invalid data type!");
            else
                dbg << ": " << QObject::tr("Invalid data type!");
        }
        else
        {
            dbg << "(Id:" << dataType.getId() << ", " << "Name:" << dataType.getName() << ')';
            if (!dataType.getDescription().isEmpty())
                dbg << " - " << dataType.getDescription();
        }

        return dbg;
    }
}