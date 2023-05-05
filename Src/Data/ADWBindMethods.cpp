#include "stdafx.h"
#include "ADWBindMethods.h"

#include <QCheckBox>
#include <QRadioButton>

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
        if (data->getDataManager())
            editor->setText(data->getDataManager()->toText(data));
        else
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

// ----------------------------------------------------------------------------------------------------

ACheckBoxBindMethod::ACheckBoxBindMethod(QObject* parent)
    : ADataWidgetBindMethod(parent)
{
}

ACheckBoxBindMethod::~ACheckBoxBindMethod()
{
    // 必须在所有子类中调用解绑方法
    unbindAll();
}

bool ACheckBoxBindMethod::bind(const ADWBindParameter& param)
{
    QCheckBox* editor = qobject_cast<QCheckBox*>(param.getWidget());
    if (editor)
    {
        QObject::connect(editor, &QCheckBox::stateChanged,
                         this, &ACheckBoxBindMethod::checkBoxValueChanged);
        return true;
    }

    return false;
}

bool ACheckBoxBindMethod::unbind(AData* data, QWidget* widget, const QString& propName)
{
    QCheckBox* editor = qobject_cast<QCheckBox*>(widget);
    if (editor)
    {
        QObject::disconnect(editor, &QCheckBox::stateChanged,
                            this, &ACheckBoxBindMethod::checkBoxValueChanged);
        return true;
    }

    return false;
}

void ACheckBoxBindMethod::onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old)
{
    if (!data)
        return;

    QCheckBox* editor = qobject_cast<QCheckBox*>(widget);
    if (editor)
    {
        const int type = data->getValue().type();
        if (type == EMetaType::Int)
            editor->setCheckState(Qt::CheckState(data->getValue().toInt()));
        else
            editor->setChecked(data->getValue().toBool());
    }
}

void ACheckBoxBindMethod::onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName)
{
    if (!data)
        return;

    QCheckBox* editor = qobject_cast<QCheckBox*>(const_cast<QWidget*>(widget));
    if (editor && data->getDataManager())
    {
        const int type = data->getDataManager()->getType();
        if (type == EMetaType::Int)
            data->getDataManager()->setValue(data, int(editor->checkState()));
        else 
            data->getDataManager()->setValue(data, editor->isChecked());
    }
}

void ACheckBoxBindMethod::checkBoxValueChanged(int val)
{
    widgetValueChanged(val);
}

// ----------------------------------------------------------------------------------------------------

ARadioButtonBindMethod::ARadioButtonBindMethod(QObject* parent)
    : ADataWidgetBindMethod(parent)
{
}

ARadioButtonBindMethod::~ARadioButtonBindMethod()
{
    // 必须在所有子类中调用解绑方法
    unbindAll();
}

bool ARadioButtonBindMethod::bind(const ADWBindParameter& param)
{
    QRadioButton* editor = qobject_cast<QRadioButton*>(param.getWidget());
    if (editor)
    {
        QObject::connect(editor, &QRadioButton::clicked,
                         this, &ARadioButtonBindMethod::radioButtonValueChanged);
        return true;
    }

    return false;
}

bool ARadioButtonBindMethod::unbind(AData* data, QWidget* widget, const QString& propName)
{
    QRadioButton* editor = qobject_cast<QRadioButton*>(widget);
    if (editor)
    {
        QObject::disconnect(editor, &QRadioButton::clicked,
                            this, &ARadioButtonBindMethod::radioButtonValueChanged);
        return true;
    }

    return false;
}

void ARadioButtonBindMethod::onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old)
{
    if (!data)
        return;

    QRadioButton* editor = qobject_cast<QRadioButton*>(widget);
    if (editor)
    {
        editor->setChecked(data->getValue().toBool());
    }
}

void ARadioButtonBindMethod::onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName)
{
    if (!data)
        return;

    QRadioButton* editor = qobject_cast<QRadioButton*>(const_cast<QWidget*>(widget));
    if (editor && data->getDataManager())
    {
        data->getDataManager()->setValue(data, editor->isChecked());
    }
}

void ARadioButtonBindMethod::radioButtonValueChanged(bool val)
{
    widgetValueChanged(val);
}

// ----------------------------------------------------------------------------------------------------

AComboBoxBindMethod::AComboBoxBindMethod(QObject* parent)
    : ADataWidgetBindMethod(parent)
{
}

AComboBoxBindMethod::~AComboBoxBindMethod()
{
    // 必须在所有子类中调用解绑方法
    unbindAll();
}

bool AComboBoxBindMethod::bind(const ADWBindParameter& param)
{
    QComboBox* editor = qobject_cast<QComboBox*>(param.getWidget());
    if (editor)
    {
        const QString propName = param.getBindProperty();
        if (propName == AStr("currentText") || propName.isEmpty())
        {
            QObject::connect(editor, &QComboBox::currentTextChanged,
                             this, QOverload<const QString&>::of(&AComboBoxBindMethod::comboBoxValueChanged));
        }
        else if (propName == AStr("currentIndex"))
        {
            QObject::connect(editor, QOverload<int>::of(&QComboBox::currentIndexChanged),
                             this, QOverload<int>::of(&AComboBoxBindMethod::comboBoxValueChanged));
        }

        return true;
    }

    return false;
}

bool AComboBoxBindMethod::unbind(AData* data, QWidget* widget, const QString& propName)
{
    QComboBox* editor = qobject_cast<QComboBox*>(widget);
    if (editor)
    {
        if (propName == AStr("currentText") || propName.isEmpty())
        {
            QObject::connect(editor, &QComboBox::currentTextChanged,
                             this, QOverload<const QString&>::of(&AComboBoxBindMethod::comboBoxValueChanged));
        }
        else if (propName == AStr("currentIndex"))
        {
            QObject::connect(editor, QOverload<int>::of(&QComboBox::currentIndexChanged),
                             this, QOverload<int>::of(&AComboBoxBindMethod::comboBoxValueChanged));
        }

        return true;
    }

    return false;
}

void AComboBoxBindMethod::onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old)
{
    if (!data)
        return;

    QComboBox* editor = qobject_cast<QComboBox*>(widget);
    if (editor)
    {
        if (propertyName == AStr("currentText") || propertyName.isEmpty())
        {
            if (data->getDataManager())
                editor->setCurrentText(data->getDataManager()->toText(data));
            else
                editor->setCurrentText(data->getValue().toString());
        }
        else if (propertyName == AStr("currentIndex"))
        {
            editor->setCurrentIndex(data->getValue().toInt());
        }
    }
}

void AComboBoxBindMethod::onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName)
{
    if (!data)
        return;

    QComboBox* editor = qobject_cast<QComboBox*>(const_cast<QWidget*>(widget));
    if (editor && data->getDataManager())
    {
        if (propertyName == AStr("currentText") || propertyName.isEmpty())
        {
            data->getDataManager()->setValue(data, editor->currentText());
        }
        else if (propertyName == AStr("currentIndex"))
        {
            data->getDataManager()->setValue(data, editor->currentIndex());
        }
    }
}

void AComboBoxBindMethod::comboBoxValueChanged(const QString& val)
{
    widgetValueChanged(val);
}

void AComboBoxBindMethod::comboBoxValueChanged(int val)
{
    widgetValueChanged(val);
}

A_DECLARE_DATAWIDGET_BINDMETHOD(QSpinBox, ASpinBoxBindMethod);
A_DECLARE_DATAWIDGET_BINDMETHOD(QDoubleSpinBox, ADoubleSpinBoxBindMethod);
A_DECLARE_DATAWIDGET_BINDMETHOD(QLineEdit, ALineEditBindMethod);
A_DECLARE_DATAWIDGET_BINDMETHOD(QCheckBox, ACheckBoxBindMethod);
A_DECLARE_DATAWIDGET_BINDMETHOD(QRadioButton, ARadioButtonBindMethod);
A_DECLARE_DATAWIDGET_BINDMETHOD(QComboBox, AComboBoxBindMethod);

APROCH_NAMESPACE_END
