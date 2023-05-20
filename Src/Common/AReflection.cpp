#include "stdafx.h"
#include "AReflection.h"

APROCH_NAMESPACE_BEGIN

AReflection::Constructor AReflection::Null = []() -> void* { return nullptr; };
AObjectReflection::Constructor AObjectReflection::Null = [](QObject*) -> QObject* { return nullptr; };

// ----------------------------------------------------------------------------------------------------

void* AReflection::newInstance(const QString& name)
{
    return constructor(name)();
}

bool AReflection::registerConstructor(const QString& name, Constructor constructor)
{
    if (name.isEmpty() || constructor == AReflection::Null)
        return false;

    if (constructors().contains(name))
    {
        qWarning() << AStr("An type constructor named %1 has been overwritten.").arg(name);
    }

    constructors()[name] = constructor;

    return true;
}

void AReflection::unregister(const QString& name)
{
    constructors().remove(name);
}

bool AReflection::hasConstructor(const QString& name)
{
    return constructors().contains(name);
}

const AReflection::Constructor& AReflection::constructor(const QString& name)
{
    if (constructors().contains(name))
        return constructors()[name];
    return AReflection::Null;
}

AReflection::ConstructorMap& AReflection::constructors() noexcept
{
    static ConstructorMap constructorMap;
    return constructorMap;
}

// ----------------------------------------------------------------------------------------------------

AObjectReflection::AObjectReflection()
{
}

QObject* AObjectReflection::newInstance(const QString& className, QObject* parent)
{
    return constructor(className)(parent);
}

bool AObjectReflection::registerConstructor(const QString& className, Constructor constructor)
{
    if (className.isEmpty() || constructor == AObjectReflection::Null)
        return false;

    if (constructors().contains(className))
    {
        qWarning() << AStr("An object constructor named %1 has been overwritten.").arg(className);
    }

    constructors()[className] = constructor;

    return true;
}

void AObjectReflection::unregister(const QString& className)
{
    constructors().remove(className);
}

bool AObjectReflection::hasConstructor(const QString& className)
{
    return constructors().contains(className);
}

const AObjectReflection::Constructor& AObjectReflection::constructor(const QString& className)
{
    if (!constructors().contains(className))
        return Null;

    return constructors()[className];
}

AObjectReflection::ConstructorMap& AObjectReflection::constructors() noexcept
{
    static ConstructorMap constructorMap;
    return constructorMap;
}


APROCH_NAMESPACE_END
