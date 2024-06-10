#include "AWinUIPlugin.h"

#include "AWinUIApplication.h"

#include <qqml.h>

void AWinUIPlugin::registerTypes(const char *uri)
{
    Q_INIT_RESOURCE(WinUI);

    // @uri Aproch.WinUI
    qmlRegisterType<AWinUIApplication>(uri, 1, 0, "AWinUIApplication");
    qmlRegisterType(QUrl("qrc:/qml/ATestButton.qml"), uri, 1, 0, "ATestButton");
}
