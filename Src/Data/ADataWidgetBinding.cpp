#include "stdafx.h"
#include "ADataWidgetBinding.h"

#include <private/qobject_p.h>

APROCH_NAMESPACE_BEGIN

class ADWBMethodMap
{
public:
    ~ADWBMethodMap()
    {
    }

    QMap<QString, ADataWidgetBindMethod*> MethodMap;
};

Q_GLOBAL_STATIC(ADWBMethodMap, globalBindMethodMap);

// ----------------------------------------------------------------------------------------------------

class ADataWidgetBindingPrivate : public QObjectPrivate
{
public:
    Q_DECLARE_PUBLIC(ADataWidgetBinding);
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

    connect(bindMethod, &ADataWidgetBindMethod::destroyed, [widgetTypeName]() {
        globalBindMethodMap->MethodMap.remove(QString(widgetTypeName));
    });

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
        return nullptr;

    const QString type = QString::fromLocal8Bit(widgetTypeName);
    return globalBindMethodMap->MethodMap.value(type);
}

bool ADataWidgetBinding::bind(const ADWBindParameter& parameter)
{
    EDataBindType type = parameter.getBindType();
    if (!parameter.isValid() || type == EDataBindType::None)
        return false;

    const char* widgetTypeName = parameter.getWidget()->metaObject()->className();
    ADataWidgetBindMethod* method = getBindMethod(widgetTypeName);
    if (!method)
        return false;

    // 确保只绑定一次
    if (type == EDataBindType::FirstTime || 
        type == EDataBindType::FirstTimeRevise)
    {
        if (type == EDataBindType::FirstTime)
            method->onValueChanged(parameter.getData(), parameter.getWidget(), parameter.getBindProperty(), QVariant());
        else
            method->onWidgetValueChanged(parameter.getData(), parameter.getWidget(), parameter.getBindProperty());
        return true;
    }

    bool ok = method->addBind(parameter);
    method->unbind(parameter.getData(), parameter.getWidget(), parameter.getBindProperty());

    return ok;
}

bool ADataWidgetBinding::unbind(AData* data, QWidget* widget, const QString& propName)
{
    if (!data || !widget)
        return false;

    const char* widgetTypeName = widget->metaObject()->className();
    ADataWidgetBindMethod* method = getBindMethod(widgetTypeName);
    if (!method)
        return false;

    return method->unbind(data, widget, propName);
}

APROCH_NAMESPACE_END
