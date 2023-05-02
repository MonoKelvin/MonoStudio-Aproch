#include "stdafx.h"
#include "ADataWidgetBinding.h"

#include <private/qobject_p.h>

APROCH_NAMESPACE_BEGIN

class ADWBMethodMap
{
public:
    ~ADWBMethodMap()
    {
        qDeleteAll(MethodMap);
    }

    QMap<QString, ADataWidgetBindMethod*> MethodMap;
};

Q_GLOBAL_STATIC(ADWBMethodMap, globalBindMethodMap);


class ADataWidgetBindingPrivate : public QObjectPrivate
{
public:
    Q_DECLARE_PUBLIC(ADataWidgetBinding);

    struct BindData
    {
        AData* data = nullptr;
        QWidget* widget = nullptr;
        const char* propertyName = nullptr;
        EDataBindType type = EDataBindType::None;
    };

    QList<BindData> bindCollection;
};

// ----------------------------------------------------------------------------------------------------

ADataWidgetBinding::ADataWidgetBinding(QObject* parent)
    : QObject(*(new ADataWidgetBindingPrivate()), parent)
{
}

ADataWidgetBinding::~ADataWidgetBinding()
{
}

bool ADataWidgetBinding::addBindMethod(ADataWidgetBindMethod* bindMethod, const char* widgetTypeName)
{
    if (!bindMethod || !widgetTypeName)
        return false;

    ADataWidgetBindMethod* method = getBindMethod(widgetTypeName);
    if (method == bindMethod)
        return false;

    if (method)
    {
        qWarning(AStr("the ADataWidgetBindMethod of type %1 already exists "
                 "and will be overwritten with a new one").arg(widgetTypeName).toLocal8Bit());

        method->deleteLater();
    }

    const QString type = QString::fromLocal8Bit(widgetTypeName);

    globalBindMethodMap->MethodMap[type] = bindMethod;

    return true;
}

bool ADataWidgetBinding::removeBindMethod(const char* widgetTypeName)
{
    ADataWidgetBindMethod* method = getBindMethod(widgetTypeName);
    if (method == nullptr)
        return false;

    method->deleteLater();

    const QString type = QString::fromLocal8Bit(widgetTypeName);
    const int c = globalBindMethodMap->MethodMap.remove(type);
    return c > 0;
}

void ADataWidgetBinding::removeBindMethod(ADataWidgetBindMethod* bindMethod)
{
    if (!bindMethod)
        return;

    for (auto iter = globalBindMethodMap->MethodMap.begin();
         iter != globalBindMethodMap->MethodMap.end();
         ++iter)
    {
        if (iter.value() == bindMethod)
        {
            bindMethod->deleteLater();
            globalBindMethodMap->MethodMap.erase(iter);
            return;
        }
    }

}

ADataWidgetBindMethod* ADataWidgetBinding::getBindMethod(const char* widgetTypeName)
{
    if (!widgetTypeName)
        return;

    const QString type = QString::fromLocal8Bit(widgetTypeName);
    return globalBindMethodMap->MethodMap.value(type);
}

bool ADataWidgetBinding::bind(AData* data, QWidget* widget, const SBindParameter& parameter)
{
    if (!data || !widget || parameter.type == EDataBindType::None)
        return false;

    const char* widgetTypeName = widget->metaObject()->className();
    ADataWidgetBindMethod* method = getBindMethod(widgetTypeName);
    if (!method)
        return false;

    method->bind(data, widget, parameter);

    // 确保只绑定一次
    if (parameter.type == EDataBindType::FirstTime ||
        parameter.type == EDataBindType::FirstTimeRevise)
    {
        if (parameter.type == EDataBindType::FirstTime)
            method->onValueChanged(data, widget, QVariant());
        else
            method->onWidgetValueChanged(data, widget);

        method->unbind(data, widget, parameter.propertyName);
    }

    emit bindDone(data, widget, const_cast<SBindParameter*>(&parameter));

    return false;
}

bool ADataWidgetBinding::unbind(AData* data, QWidget* widget, const char* propName)
{
    if (!data || !widget)
        return false;

    const char* widgetTypeName = widget->metaObject()->className();
    ADataWidgetBindMethod* method = getBindMethod(widgetTypeName);
    if (!method)
        return false;

    method->unbind(data, widget, propName);

    SBindParameter parameter;
    parameter.propertyName = propName;
    emit unbindDone(data, widget, &parameter);

    return false;
}

APROCH_NAMESPACE_END
