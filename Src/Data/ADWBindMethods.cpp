#include "stdafx.h"
#include "ADWBindMethods.h"

APROCH_NAMESPACE_BEGIN

ADWBM_SpinBox::ADWBM_SpinBox(QObject* parent)
    : ADataWidgetBindMethod(parent)
{
}

ADWBM_SpinBox::~ADWBM_SpinBox()
{
    unbindAll();
}

bool ADWBM_SpinBox::bind(const ADWBindParameter& param)
{
    QSpinBox* spinBox = qobject_cast<QSpinBox*>(param.getWidget());
    if (spinBox)
    {
        const EDataBindType type = param.getBindType();
        if (type == EDataBindType::TwoWay || type == EDataBindType::OneWayRevise)
        {
            return QObject::connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
                                    this, &ADWBM_SpinBox::spinboxValueChanged);
        }

        return true;
    }

    return false;
}

bool ADWBM_SpinBox::unbind(AData* data, QWidget* widget, const QString& propName)
{
    QSpinBox* spinBox = qobject_cast<QSpinBox*>(widget);
    if (spinBox)
    {
        QObject::disconnect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
                            this, &ADWBM_SpinBox::spinboxValueChanged);
        return true;
    }

    return false;
}

void ADWBM_SpinBox::onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old)
{
    if (!data)
        return;

    QSpinBox* spinBox = qobject_cast<QSpinBox*>(widget);
    if (spinBox)
    {
        spinBox->setValue(data->getValue().toInt());
    }
}

void ADWBM_SpinBox::onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName)
{
    if (!data)
        return;

    QSpinBox* spinBox = qobject_cast<QSpinBox*>(const_cast<QWidget*>(widget));
    if (spinBox && data->getDataManager())
    {
        data->getDataManager()->setValue(data, spinBox->value());
    }
}

void ADWBM_SpinBox::spinboxValueChanged(int val)
{
    widgetValueChanged(val);
}

APROCH_NAMESPACE_END
