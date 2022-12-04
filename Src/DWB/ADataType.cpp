#include "stdafx.h"
#include "ADataType.h"


namespace aproch
{
    //static QAtomicInteger<TDataTypeId> xAtomicId = 0;
    static TDataTypeId xAtomicId = 0;

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
        return getId() > 0 && !m_name.trimmed().isEmpty();
    }

    bool ADataType::operator==(const ADataType& rhs) const
    {
        return getId() == rhs.getId() || AStringToolkit::TrimCompare(m_name, rhs.m_name);
    }

    bool ADataType::operator!=(const ADataType& rhs) const
    {
        return !(this->operator==(rhs));
    }

    bool ADataType::operator>(const ADataType& rhs) const
    {
        return getId() > rhs.getId();
    }

    bool ADataType::operator>=(const ADataType& rhs) const
    {
        return getId() >= rhs.getId();
    }

    bool ADataType::operator<(const ADataType& rhs) const
    {
        return getId() < rhs.getId();
    }

    bool ADataType::operator<=(const ADataType& rhs) const
    {
        return getId() <= rhs.getId();
    }

    ADataType ADataType::Register(const QString& name, const QString& description, const QIcon& icon)
    {
        ADataType dataType = Get(name);
        if (dataType.isValid())
        {
            qWarning() << QObject::tr("The type ") << '[' << name << ']' << QObject::tr(" already exists!");
            return dataType;
        }

        ++xAtomicId;

        dataType.m_typeId = xAtomicId;
        dataType.m_name = name.trimmed().toLower();
        dataType.m_description = description;
        dataType.m_icon = icon;
        m_dataTypeMap.insert(xAtomicId, dataType);
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

    ADataType ADataType::Get(const QString& name)
    {
        for (const auto& item : m_dataTypeMap)
        {
            if (AStringToolkit::TrimCompare(item.getName(), name))
                return item;
        }
        return ADataType();
    }

    const TDataTypeMap& ADataType::GetAllType()
    {
        return m_dataTypeMap;
    }

    /*bool ADataType::SetName(TDataTypeId id, const QString& newName)
    {
        if (newName.trimmed().isEmpty())
            return false;

        auto iter = m_dataTypeMap.find(id);
        if (iter == m_dataTypeMap.end())
            return false;
        iter->m_name = newName;
        return true;
    }*/

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