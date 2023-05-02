#include "stdafx.h"
#include "ADWBindMethods.h"

APROCH_NAMESPACE_BEGIN

// ----------------------------------------------------------------------------------------------------

ADWBM_SpinBox::~ADWBM_SpinBox()
{
}

bool ADWBM_SpinBox::bind(AData* data, QWidget* widget, const SBindParameter& parameter)
{
    ADataContainer* dc = dataContainer(data);
    QSpinBox* spinBox = qobject_cast<QSpinBox*>(widget);
    if (spinBox && dc)
    {
        QObject::connect(dc, &ADataContainer::valueChanged,
                         this, &ADataWidgetBindMethodBase::slotValueChanged);
        QObject::connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), 
                         this, &ADataWidgetBindMethodBase::slotWidgetValueChanged);
    }
}

bool ADWBM_SpinBox::unbind(AData* data, QWidget* widget, const char* propName)
{
    QSpinBox* spinBox = qobject_cast<QSpinBox*>(widget);
    if (spinBox)
        QObject::disconnect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), 
                            this, &ADataWidgetBindMethodBase::slotWidgetValueChanged);
}

void ADWBM_SpinBox::onValueChanged(const AData* data, QWidget* widget, const QVariant& old)
{
    if (!data)
        return;

    QSpinBox* spinBox = qobject_cast<QSpinBox*>(widget);
    if (spinBox)
    {
        spinBox->setValue(data->getValue().toInt());
    }
}

void ADWBM_SpinBox::onWidgetValueChanged(AData* data, const QWidget* widget)
{
    if (!data)
        return;

    QSpinBox* spinBox = qobject_cast<QSpinBox*>(widget);
    if (spinBox && data->getDataManager())
    {
        data->getDataManager()->setValue(data, spinBox->value());
    }
}

APROCH_NAMESPACE_END
