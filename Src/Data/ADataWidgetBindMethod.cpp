#include "stdafx.h"
#include "ADataWidgetBindMethod.h"

#include <private/qobject_p.h>

APROCH_NAMESPACE_BEGIN

// ----------------------------------------------------------------------------------------------------

class ADWBindParameterPrivate
{
public:
    ADWBindParameterPrivate(AData* data, QWidget* widget, const QString& propName, EDataBindType type)
        : data(data), widget(widget), propName(propName), type(type)
    {
    }

    AData* data = nullptr;
    QWidget* widget = nullptr;
    QString propName;
    EDataBindType type = EDataBindType::None;
};

// ----------------------------------------------------------------------------------------------------

ADWBindParameter::ADWBindParameter()
    : ADWBindParameter(nullptr, nullptr, nullptr, EDataBindType::None)
{
}

ADWBindParameter::ADWBindParameter(AData* data, QWidget* widget, const QString& propName, EDataBindType type)
    : d(new ADWBindParameterPrivate(data, widget, propName, type))
{
}

ADWBindParameter::ADWBindParameter(const ADWBindParameter& other)
    : ADWBindParameter(other.d->data, other.d->widget, other.d->propName, other.d->type)
{
}

ADWBindParameter::~ADWBindParameter()
{
}

AData* ADWBindParameter::getData() const
{
    return d->data;
}

void ADWBindParameter::setData(AData* data)
{
    d->data = data;
}

QWidget* ADWBindParameter::getWidget() const
{
    return d->widget;
}

void ADWBindParameter::setWidget(QWidget* widget)
{
    d->widget = widget;
}

QString ADWBindParameter::getBindProperty() const
{
    return d->propName;
}

void ADWBindParameter::setBindProperty(const QString& name)
{
    d->propName = name;
}

EDataBindType ADWBindParameter::getBindType() const
{
    return d->type;
}

void ADWBindParameter::setBindType(EDataBindType type)
{
    d->type = type;
}

bool ADWBindParameter::isValid() const
{
    return d->data && d->widget;
}

bool ADWBindParameter::operator==(const ADWBindParameter& other) const
{
    return (d->data == other.d->data) && (d->widget == other.d->widget) && (d->propName == other.d->propName);
}

bool ADWBindParameter::operator!=(const ADWBindParameter& other) const
{
    return !(operator==(other));
}

ADWBindParameter& ADWBindParameter::operator=(const ADWBindParameter& other)
{
    d->data = other.d->data;
    d->widget = other.d->widget;
    d->propName = other.d->propName;
    d->type = other.d->type;
    return *this;
}

// ----------------------------------------------------------------------------------------------------

class ADataWidgetBindMethodPrivate : public QObjectPrivate
{
public:
    Q_DECLARE_PUBLIC(ADataWidgetBindMethod);

    ADWBindParameterList findByData(AData* data)
    {
        ADWBindParameterList result;
        for (const ADWBindParameter& param : params)
        {
            if (param.getData() == data)
                result.push_back(param);
        }
        return result;
    }

    ADWBindParameterList findByWidget(QWidget* widget, const char* propName = nullptr)
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
    connect(this, &ADataWidgetBindMethod::destroyed, this, &ADataWidgetBindMethod::onDestroyed);
}

ADataWidgetBindMethod::~ADataWidgetBindMethod()
{
}

bool ADataWidgetBindMethod::addBind(const ADWBindParameter& param)
{
    Q_D(ADataWidgetBindMethod);

    for (ADWBindParameter& ps : d->params)
    {
        if (ps == param)
        {
            ps.setBindProperty(param.getBindProperty());
            return true;
        }
    }

    // 默认添加的绑定
    ADataContainer* dc = getDataContainer(param.getData());
    if (dc)
    {
        connect(dc, &ADataContainer::valueChanged, this, &ADataWidgetBindMethod::valueChanged);
        connect(param.getWidget(), &QWidget::destroyed, this, &ADataWidgetBindMethod::widgetDestroyed);
    }

    // 实现绑定
    if (!bind(param))
        return false;

    d->params.push_back(param);

    return true;
}

bool ADataWidgetBindMethod::removeBind(AData* data, QWidget* widget, const QString& propName)
{
    Q_D(ADataWidgetBindMethod);

    ADWBindParameter tmpParam(data, widget, propName);
    if (!tmpParam.isValid())
        return false;

    for (int i = 0; i < d->params.size(); ++i)
    {
        if (d->params.at(i) == tmpParam)
        {
            // 默认移除的绑定
            ADataContainer* dc = getDataContainer(data);
            if (dc)
            {
                disconnect(dc, &ADataContainer::valueChanged, this, &ADataWidgetBindMethod::valueChanged);
                disconnect(widget, &QWidget::destroyed, this, &ADataWidgetBindMethod::widgetDestroyed);
            }

            // 实现解绑
            if (!unbind(data, widget, propName))
                return false;

            d->params.removeAt(i);

            return true;
        }
    }

    return false;
}

ADataContainer* ADataWidgetBindMethod::getDataContainer(AData* data)
{
    if (data && data->getDataManager())
        return data->getDataManager()->getDataContainer();
    return nullptr;
}

void ADataWidgetBindMethod::valueChanged(AData* data, const QVariant& old)
{
    Q_D(ADataWidgetBindMethod);

    ADWBindParameterList widgets = d->findByData(data);
    for (const ADWBindParameter& param : widgets)
    {
        onValueChanged(qAsConst<AData*>(data), param.getWidget(), param.getBindProperty(), old);
    }
}

void ADataWidgetBindMethod::widgetValueChanged(const QVariant& val, const QString& propertyName)
{
    Q_D(ADataWidgetBindMethod);

    QWidget* widget = qobject_cast<QWidget*>(sender());
    ADWBindParameterList params = d->findByWidget(widget);

    for (const ADWBindParameter& param : params)
    {
        if (param.getBindProperty() == propertyName)
        {
            onWidgetValueChanged(param.getData(), widget, propertyName);
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

void ADataWidgetBindMethod::onDestroyed(QObject* obj)
{
    Q_D(ADataWidgetBindMethod);

    for (ADWBindParameter& param : d->params)
    {
        unbind(param.getData(), param.getWidget(), param.getBindProperty());
    }

    d->params.clear();
}

APROCH_NAMESPACE_END
