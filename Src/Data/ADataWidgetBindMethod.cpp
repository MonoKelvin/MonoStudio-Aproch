/****************************************************************************
 * @file    ADataWidgetBindMethod.cpp
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
#include "ADataWidgetBindMethod.h"

#include <private/qobject_p.h>

APROCH_NAMESPACE_BEGIN

class ADWBMethodMap
{
public:
    ~ADWBMethodMap()
    {
        for (auto iter = MethodMap.begin(); iter != MethodMap.end();)
        {
            if (iter.value())
            {
                delete iter.value();
                iter = MethodMap.erase(iter);
            }
            else
            {
                ++iter;
            }
        }

        MethodMap.clear();
    }

    QMap<QString, ADataWidgetBindMethodPtr> MethodMap;
};

Q_GLOBAL_STATIC(ADWBMethodMap, globalBindMethodMap);

class ADataWidgetBindMethodPrivate : public QObjectPrivate
{
public:
    Q_DECLARE_PUBLIC(ADataWidgetBindMethod);

    bool hasBind(AData* data, QWidget* widget, const QString& propName = QString(), bool isContainsEmpty = true) const
    {
        if (propName.isEmpty() && !isContainsEmpty)
        {
            for (const ADWBindParameter& param : params)
                if (param.getData() == data && param.getWidget() == widget)
                    return true;
        }
        else
        {
            for (const ADWBindParameter& param : params)
                if (param.getData() == data && param.getWidget() == widget && param.getBindProperty() == propName)
                    return true;
        }

        return false;
    }

    ADWBindParameterList findByData(AData* data) const
    {
        ADWBindParameterList result;
        for (const ADWBindParameter& param : params)
        {
            if (param.getData() == data)
                result.push_back(param);
        }
        return result;
    }

    ADWBindParameterList findByWidget(QWidget* widget, const QString& propName = QString()) const
    {
        ADWBindParameterList result;

        if (propName != nullptr)
        {
            for (const ADWBindParameter& param : params)
            {
                if (param.getWidget() == widget && param.getBindProperty() == propName)
                {
                    result.push_back(param);
                    break;
                }
            }
        }
        else
        {
            for (const ADWBindParameter& param : params)
            {
                if (param.getWidget() == widget)
                {
                    result.push_back(param);
                }
            }
        }

        return result;
    }

    ADWBindParameterList params;
};

// ----------------------------------------------------------------------------------------------------

ADataWidgetBindMethod::ADataWidgetBindMethod(QObject* parent)
    : QObject(*(new ADataWidgetBindMethodPrivate()), parent)
{
}

ADataWidgetBindMethod::~ADataWidgetBindMethod()
{
}

bool ADataWidgetBindMethod::addBindMethod(ADataWidgetBindMethodPtr bindMethod, const char* widgetTypeName)
{
    if (!bindMethod || !widgetTypeName)
        return false;

    ADataWidgetBindMethodPtr method = getBindMethod(widgetTypeName);
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

bool ADataWidgetBindMethod::removeBindMethod(const char* widgetTypeName)
{
    ADataWidgetBindMethodPtr method = getBindMethod(widgetTypeName);
    if (method == nullptr)
        return false;

    method->deleteLater();

    const QString type = QString::fromLocal8Bit(widgetTypeName);
    const int c = globalBindMethodMap->MethodMap.remove(type);
    return c > 0;
}

void ADataWidgetBindMethod::removeBindMethod(ADataWidgetBindMethodPtr bindMethod)
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

ADataWidgetBindMethodPtr ADataWidgetBindMethod::getBindMethod(const char* widgetTypeName)
{
    if (!widgetTypeName)
        return nullptr;

    const QString type = QString::fromLocal8Bit(widgetTypeName);
    return globalBindMethodMap->MethodMap.value(type);
}

bool ADataWidgetBindMethod::addBind(const ADWBindParameter& param)
{
    if (!param.isValid() || param.getBindType() == EDataBindType::None)
        return false;

    const char* widgetTypeName = param.getWidget()->metaObject()->className();
    ADataWidgetBindMethodPtr method = getBindMethod(widgetTypeName);
    if (!method)
        return false;

    if (!method->checkBind(param))
        return false;

    EDataBindType type = param.getBindType();
    if (type == EDataBindType::OneWay ||
        type == EDataBindType::TwoWay ||
        type == EDataBindType::FirstTime)
    {
        method->onDataChanged(param.getData(), param.getWidget(), param.getBindProperty());
        if (type == EDataBindType::FirstTime)
            return true;
    }
    else if (type == EDataBindType::OneWayRevise ||
             type == EDataBindType::TwoWay ||
             type == EDataBindType::FirstTimeRevise)
    {
        method->onWidgetChanged(param.getData(), param.getWidget(), param.getBindProperty());
        if (type == EDataBindType::FirstTimeRevise)
            return true;
    }

    bool hasBind = false;
    for (ADWBindParameter& ps : method->d_func()->params)
    {
        if (ps == param)
        {
            if (ps.getBindType() == type || 
                (type == EDataBindType::FirstTime || type == EDataBindType::FirstTimeRevise))
                return false;

            ps.setBindType(type);
            hasBind = true;
            break;
        }
    }

    connect(param.getWidget(), &QWidget::destroyed, 
            method, &ADataWidgetBindMethod::widgetDestroyed,
            Qt::ConnectionType(Qt::AutoConnection | Qt::UniqueConnection));

    // 先解绑，再绑定
    method->unbind(param.getData(), param.getWidget(), param.getBindProperty());
    if (!method->bind(param))
        return false;

    if (!hasBind)
    {
        method->d_func()->params.push_back(param);
        connect(param.getData()->getDataManager(), &AAbstractDataManager::dataDestroyed,
                method, &ADataWidgetBindMethod::dataDestroyed);
    }

    return true;
}

bool ADataWidgetBindMethod::removeBind(AData* data, QWidget* widget, const QString& propName)
{
    const char* widgetTypeName = widget->metaObject()->className();
    ADataWidgetBindMethodPtr method = getBindMethod(widgetTypeName);
    if (!method)
        return false;

    ADWBindParameter tmpParam(data, widget, propName);
    if (!tmpParam.isValid())
        return false;

    ADWBindParameterList& params = method->d_func()->params;

    for (int i = 0; i < params.size(); ++i)
    {
        if (params.at(i) == tmpParam)
        {
            // 实现解绑
            if (!method->unbind(data, widget, propName))
                return false;

            params.removeAt(i);

            return true;
        }
    }

    return false;
}

bool ADataWidgetBindMethod::checkBind(const ADWBindParameter& param) const
{
    return true;
}

bool ADataWidgetBindMethod::hasBind(AData* data, QWidget* widget, const QString& propName, bool isContainsEmpty) const
{
    Q_D(const ADataWidgetBindMethod);
    return d->hasBind(data, widget, propName, isContainsEmpty);
}

ADWBindParameterList ADataWidgetBindMethod::getParameters(AData* data) const
{
    Q_D(const ADataWidgetBindMethod);
    return d->findByData(data);
}

ADWBindParameterList ADataWidgetBindMethod::getParameters(QWidget* widget, const QString& propName) const
{
    Q_D(const ADataWidgetBindMethod);
    return d->findByWidget(widget, propName);
}

void ADataWidgetBindMethod::dataChanged(AData* data, const QVariant& newVal)
{
    Q_UNUSED(newVal);
    Q_D(ADataWidgetBindMethod);

    ADWBindParameterList widgets = d->findByData(data);
    for (const ADWBindParameter& param : widgets)
    {
        const EDataBindType type = param.getBindType();

        if (type == EDataBindType::TwoWay || type == EDataBindType::OneWay)
        {
            onDataChanged(qAsConst<AData*>(data), param.getWidget(), param.getBindProperty());
        }
    }
}

void ADataWidgetBindMethod::widgetChanged(const QVariant& val)
{
    widgetPropertyChanged(val, QString());
}

void ADataWidgetBindMethod::widgetPropertyChanged(const QVariant& val, const QString& propertyName)
{
    Q_D(ADataWidgetBindMethod);

    QWidget* widget = qobject_cast<QWidget*>(sender());
    ADWBindParameterList params = d->findByWidget(widget);

    for (const ADWBindParameter& param : params)
    {
        const EDataBindType type = param.getBindType();

        if (param.getBindProperty() == propertyName &&
            (type == EDataBindType::TwoWay || type == EDataBindType::OneWayRevise))
        {
            onWidgetChanged(param.getData(), widget, propertyName);
            return;
        }
    }
}

void ADataWidgetBindMethod::widgetDestroyed(QObject* obj)
{
    Q_D(ADataWidgetBindMethod);

    for (auto iter = d->params.begin(); iter != d->params.end(); )
    {
        QWidget* widget = iter->getWidget();
        if (widget == obj)
            iter = d->params.erase(iter);
        else
            ++iter;
    }
}

void ADataWidgetBindMethod::dataDestroyed(AData* data)
{
    Q_D(ADataWidgetBindMethod);

    for (auto iter = d->params.begin(); iter != d->params.end(); )
    {
        AData* theData = iter->getData();
        if (theData == data)
            iter = d->params.erase(iter);
        else
            ++iter;
    }
}

void ADataWidgetBindMethod::unbindAll()
{
    Q_D(ADataWidgetBindMethod);

    for (ADWBindParameter& param : d->params)
    {
        unbind(param.getData(), param.getWidget(), param.getBindProperty());
    }

    d->params.clear();
}

APROCH_NAMESPACE_END
