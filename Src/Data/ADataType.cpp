#include "stdafx.h"
#include "ADataType.h"

APROCH_NAMESPACE_BEGIN

#if 0
class ADataTypePrivate;
static QHash<int, QSharedPointer<ADataTypePrivate>> sDataTypeMap;

class ADataTypePrivate
{
public:
    inline bool operator==(const ADataTypePrivate& other) const
    {
        return typeId == other.typeId && name == other.name;
    }
    inline bool operator!=(const ADataTypePrivate& other) const
    {
        return typeId != other.typeId || name != other.name;
    }

    int typeId = QMetaType::UnknownType;
    QString name;
    QString description;
};

static ADataTypePrivate* findType(int type) noexcept
{
    return sDataTypeMap.value(type, nullptr).get();
}

static ADataTypePrivate* findType(const QString& name)
{
    for (const auto& item : sDataTypeMap)
    {
        if (nullptr != item && item->name == name)
            return item.get();
    }
    return nullptr;
}

static void newType(int type, const QString& name, const QString& desc)
{

}

// ----------------------------------------------------------------------------------------------------

ADataType::ADataType()
    : d_ptr(nullptr)
{
}

ADataType::ADataType(int type)
{
    d_ptr = findType(type);
}

ADataType::ADataType(const QString& name)
{
    d_ptr = findType(name);
}

ADataType::ADataType(const ADataType& rhs) noexcept
    : d_ptr(rhs.d_ptr)
{
}

ADataType::~ADataType()
{
    d_ptr = nullptr;
}

int ADataType::getType() const
{
    return d_ptr ? d_ptr->typeId : QVariant::Type::Invalid;
}

QMetaType::Type ADataType::getMetaType() const
{
    return d_ptr ? QMetaType::Type(d_ptr->typeId) : QMetaType::UnknownType;
}

QString ADataType::getName() const
{
    return d_ptr ? d_ptr->name : QString();
}

QString ADataType::getDescription() const
{
    return d_ptr ? d_ptr->description : QString();
}

bool ADataType::isValid() const
{
    return d_ptr && d_ptr->typeId >= 0 && !d_ptr->name.isEmpty();
}

ADataType& ADataType::operator=(const ADataType& rhs)
{
    d_ptr = rhs.d_ptr;
    return *this;
}

ADataType& ADataType::operator=(ADataType&& rhs) noexcept
{
    d_ptr = rhs.d_ptr;
    return *this;
}

bool ADataType::operator==(const ADataType& rhs) const
{
    return d_ptr && rhs.d_ptr && d_ptr->operator==(*rhs.d_ptr);
}

bool ADataType::operator!=(const ADataType& rhs) const
{
    return !(this->operator==(rhs));
}

bool ADataType::operator>(const ADataType& rhs) const
{
    return getType() > rhs.getType();
}

bool ADataType::operator>=(const ADataType& rhs) const
{
    return getType() >= rhs.getType();
}

bool ADataType::operator<(const ADataType& rhs) const
{
    return getType() < rhs.getType();
}

bool ADataType::operator<=(const ADataType& rhs) const
{
    return getType() <= rhs.getType();
}

ADataType ADataType::Register(const QString& name, const QString& description, const ACategory& category)
{
    ADataType dataType = Get(name);
    if (dataType.isValid())
    {
        qWarning() << QObject::tr("The type ") << '[' << name << ']' << QObject::tr(" already exists!");
        return dataType;
    }

    const int id = QMetaType::type(name.toLocal8Bit());
    if (id == QMetaType::UnknownType)
    {
        qWarning() << QObject::tr("The name of data type called ") << '[' << name << ']' << QObject::tr(" is not exists!");
        return dataType;
    }

    newType(id, name, description);
    return dataType;
}

#endif

APROCH_NAMESPACE_END
