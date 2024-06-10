#include "stdafx.h"
#include "AWinUIPlugin.h"
#include "AWinUIApplication.h"

#include <QtQml/qqmlextensionplugin.h>

APROCH_NAMESPACE_BEGIN

APROCH_INIT_SINGLETON(AWinUIPlugin)

AWinUIPlugin::AWinUIPlugin() = default;

void AWinUIPlugin::registerTypes(const char* uri)
{
    Q_INIT_RESOURCE(Resource);
}

void AWinUIPlugin::initializeEngine(QQmlEngine* engine, const char* uri)
{
}

APROCH_NAMESPACE_END
