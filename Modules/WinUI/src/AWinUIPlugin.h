#ifndef WINUI_PLUGIN_H
#define WINUI_PLUGIN_H

#include <QQmlExtensionPlugin>

class AWinUIPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) override;
};

#endif // WINUI_PLUGIN_H
