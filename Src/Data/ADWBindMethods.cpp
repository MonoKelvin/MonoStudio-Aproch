#include "stdafx.h"
#include "ADWBindMethods.h"

APROCH_NAMESPACE_BEGIN

ASpinBoxBindMethod::ASpinBoxBindMethod(QObject* parent)
    : ADataWidgetBindMethod(parent)
{
}

ASpinBoxBindMethod::~ASpinBoxBindMethod()
{
    // 必须在所有子类中调用解绑方法
    unbindAll();
}

bool ASpinBoxBindMethod::bind(const ADWBindParameter& param)
{
    QSpinBox* spinBox = qobject_cast<QSpinBox*>(param.getWidget());
    if (spinBox)
    {
        QObject::connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
                         this, &ASpinBoxBindMethod::spinboxValueChanged);
        return true;
    }

    return false;
}

bool ASpinBoxBindMethod::unbind(AData* data, QWidget* widget, const QString& propName)
{
    QSpinBox* spinBox = qobject_cast<QSpinBox*>(widget);
    if (spinBox)
    {
        QObject::disconnect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
                            this, &ASpinBoxBindMethod::spinboxValueChanged);
        return true;
    }

    return false;
}

void ASpinBoxBindMethod::onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old)
{
    if (!data)
        return;

    QSpinBox* spinBox = qobject_cast<QSpinBox*>(widget);
    if (spinBox)
    {
        spinBox->setValue(data->getValue().toInt());
    }
}

void ASpinBoxBindMethod::onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName)
{
    if (!data)
        return;

    QSpinBox* spinBox = qobject_cast<QSpinBox*>(const_cast<QWidget*>(widget));
    if (spinBox && data->getDataManager())
    {
        data->getDataManager()->setValue(data, spinBox->value());
    }
}

void ASpinBoxBindMethod::spinboxValueChanged(int val)
{
    widgetValueChanged(val);
}

// ----------------------------------------------------------------------------------------------------

ADoubleSpinBoxBindMethod::ADoubleSpinBoxBindMethod(QObject* parent)
    : ADataWidgetBindMethod(parent)
{
}

ADoubleSpinBoxBindMethod::~ADoubleSpinBoxBindMethod()
{
    // 必须在所有子类中调用解绑方法
    unbindAll();
}

bool ADoubleSpinBoxBindMethod::bind(const ADWBindParameter& param)
{
    QDoubleSpinBox* editor = qobject_cast<QDoubleSpinBox*>(param.getWidget());
    if (editor)
    {
        QObject::connect(editor, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                         this, &ADoubleSpinBoxBindMethod::spinboxValueChanged);
        return true;
    }

    return false;
}

bool ADoubleSpinBoxBindMethod::unbind(AData* data, QWidget* widget, const QString& propName)
{
    QDoubleSpinBox* editor = qobject_cast<QDoubleSpinBox*>(widget);
    if (editor)
    {
        QObject::disconnect(editor, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                            this, &ADoubleSpinBoxBindMethod::spinboxValueChanged);
        return true;
    }

    return false;
}

void ADoubleSpinBoxBindMethod::onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old)
{
    if (!data)
        return;

    QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(widget);
    if (spinBox)
    {
        spinBox->setValue(data->getValue().toDouble());
    }
}

void ADoubleSpinBoxBindMethod::onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName)
{
    if (!data)
        return;

    QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(const_cast<QWidget*>(widget));
    if (spinBox && data->getDataManager())
    {
        data->getDataManager()->setValue(data, spinBox->value());
    }
}

void ADoubleSpinBoxBindMethod::spinboxValueChanged(double val)
{
    widgetValueChanged(val);
}

// ----------------------------------------------------------------------------------------------------

ALineEditBindMethod::ALineEditBindMethod(QObject* parent)
    : ADataWidgetBindMethod(parent)
{
}

ALineEditBindMethod::~ALineEditBindMethod()
{
    // 必须在所有子类中调用解绑方法
    unbindAll();
}

bool ALineEditBindMethod::bind(const ADWBindParameter& param)
{
    QLineEdit* editor = qobject_cast<QLineEdit*>(param.getWidget());
    if (editor)
    {
        QObject::connect(editor, &QLineEdit::textChanged,
                         this, &ALineEditBindMethod::lineEditValueChanged);
        return true;
    }

    return false;
}

bool ALineEditBindMethod::unbind(AData* data, QWidget* widget, const QString& propName)
{
    QLineEdit* editor = qobject_cast<QLineEdit*>(widget);
    if (editor)
    {
        QObject::disconnect(editor, &QLineEdit::textChanged,
                            this, &ALineEditBindMethod::lineEditValueChanged);
        return true;
    }

    return false;
}

void ALineEditBindMethod::onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old)
{
    if (!data)
        return;

    QLineEdit* editor = qobject_cast<QLineEdit*>(widget);
    if (editor)
    {
        editor->setText(data->getValue().toString());
    }
}

void ALineEditBindMethod::onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName)
{
    if (!data)
        return;

    QLineEdit* editor = qobject_cast<QLineEdit*>(const_cast<QWidget*>(widget));
    if (editor && data->getDataManager())
    {
        data->getDataManager()->setValue(data, editor->text());
    }
}

void ALineEditBindMethod::lineEditValueChanged(const QString& val)
{
    widgetValueChanged(val);
}

A_DECLARE_DATAWIDGET_BINDMETHOD(QSpinBox, ASpinBoxBindMethod);
A_DECLARE_DATAWIDGET_BINDMETHOD(QDoubleSpinBox, ADoubleSpinBoxBindMethod);
A_DECLARE_DATAWIDGET_BINDMETHOD(QLineEdit, ALineEditBindMethod);

APROCH_NAMESPACE_END
