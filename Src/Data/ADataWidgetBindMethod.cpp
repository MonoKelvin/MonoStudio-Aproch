#include "stdafx.h"
#include "ADataWidgetBindMethod.h"

#include <private/qobject_p.h>

APROCH_NAMESPACE_BEGIN

typedef QMap<AData*, QWidgetList> DataToWidgetListMap;
typedef QMap<QWidget*, AData*> WidgetToDataMap;

class DataWidgetCollection
{
public:
    void addBind(AData* data, QWidget* widget)
    {
        typename DataToWidgetListMap::iterator it = allWidgets.find(data);
        if (it == allWidgets.end())
            it = allWidgets.insert(data, QWidgetList());
        it.value().append(widget);
        widgetToData.insert(widget, data);
    }

    void onWidgetDestroyed(QObject* object)
    {
        const typename WidgetToDataMap::iterator ecend = widgetToData.end();
        for (typename WidgetToDataMap::iterator itWidget = widgetToData.begin(); itWidget != ecend; ++itWidget)
        {
            if (itWidget.key() == object)
            {
                QWidget* widget = itWidget.key();
                AData* data = itWidget.value();
                const typename DataToWidgetListMap::iterator pit = allWidgets.find(data);
                if (pit != allWidgets.end())
                {
                    pit.value().removeAll(widget);
                    if (pit.value().empty())
                        allWidgets.erase(pit);
                }
                widgetToData.erase(itWidget);
                return;
            }
        }
    }

    DataToWidgetListMap allWidgets;
    WidgetToDataMap widgetToData;
};

class ADataWidgetBindMethodPrivate : public QObjectPrivate
{
public:
    Q_DECLARE_PUBLIC(ADataWidgetBindMethod);

    DataWidgetCollection dwCollection;
};

// ----------------------------------------------------------------------------------------------------

ADataWidgetBindMethod::ADataWidgetBindMethod(QObject* parent)
    : QObject(*(new ADataWidgetBindMethodPrivate()), parent)
{
}

void ADataWidgetBindMethod::addBind(AData* data, QWidget* widget)
{
    Q_D(ADataWidgetBindMethod);
    d->dwCollection.addBind(data, widget);
}

ADataContainer* ADataWidgetBindMethod::dataContainer(AData* data)
{
    if (!data && !data->getDataManager())
        return data->getDataManager()->getDataContainer();
    return nullptr;
}

void ADataWidgetBindMethod::valueChanged(AData* data, const QVariant& old)
{
    Q_D(ADataWidgetBindMethod);

    const auto it = d->dwCollection.allWidgets.constFind(data);
    if (it == d->dwCollection.allWidgets.cend())
        return;

    for (QWidget* editor : it.value())
    {
        onValueChanged(qAsConst<AData*>(data), editor, old);
    }
}

void ADataWidgetBindMethod::widgetValueChanged(const QVariant& val)
{
    Q_D(ADataWidgetBindMethod);

    QObject* obj = sender();
    auto ecend = d->dwCollection.widgetToData.constEnd();

    for (auto itEditor = d->dwCollection.widgetToData.constBegin(); itEditor != ecend; ++itEditor)
    {
        if (itEditor.key() == obj)
        {
            AData* data = itEditor.value();
            if (!data)
                return;

            onWidgetValueChanged(data, itEditor.key());
            return;
        }
    }
}

APROCH_NAMESPACE_END
