/****************************************************************************
 * @file    ADataWidgetBinding.cpp
 * @date    2023-05-02 
 * @author  MonoKelvin
 * @email   15007083506@qq.com
 * @github  https://github.com/MonoKelvin
 * @brief
 *
 * This source file is part of Aproch.
 * Copyright (C) 2020 by MonoKelvin. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *****************************************************************************/
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

    if (type == EDataBindType::OneWay ||
        type == EDataBindType::TwoWay ||
        type == EDataBindType::FirstTime)
    {
        method->onValueChanged(parameter.getData(), parameter.getWidget(), parameter.getBindProperty(), QVariant());
        if (type == EDataBindType::FirstTime)
            return true;
    }
    else if (type == EDataBindType::OneWayRevise ||
             type == EDataBindType::TwoWay ||
             type == EDataBindType::FirstTimeRevise)
    {
        method->onWidgetValueChanged(parameter.getData(), parameter.getWidget(), parameter.getBindProperty());
        if (type == EDataBindType::FirstTimeRevise)
            return true;
    }

    bool ok = method->addBind(parameter);

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

    return method->removeBind(data, widget, propName);
}

APROCH_NAMESPACE_END
