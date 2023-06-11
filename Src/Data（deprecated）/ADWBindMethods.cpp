/****************************************************************************
 * @file    ADWBindMethods.cpp
 * @date    2023-05-14 
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
        data->setValue(spinBox->value());
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
        editor->setText(data->toString());
    }
}

void ALineEditBindMethod::onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName)
{
    if (!data)
        return;

    QLineEdit* editor = qobject_cast<QLineEdit*>(const_cast<QWidget*>(widget));
    if (editor && data->getDataManager())
    {
        data->setValue(editor->text());
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
        const int type = data->getType();
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
        const int type = data->getType();
        if (type == EMetaType::Int)
            data->setValue(int(editor->checkState()));
        else 
            data->setValue(editor->isChecked());
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
        data->setValue(editor->isChecked());
    }
}

void ARadioButtonBindMethod::radioButtonValueChanged(bool val)
{
    widgetValueChanged(val);
}

// ----------------------------------------------------------------------------------------------------

const QString AComboBoxBindMethod::Items = AStr("items");
const QString AComboBoxBindMethod::CurrentIndex = AStr("currentIndex");
const QString AComboBoxBindMethod::CurrentText = AStr("currentText");

AComboBoxBindMethod::AComboBoxBindMethod(QObject* parent)
    : ADataWidgetBindMethod(parent)
{
}

AComboBoxBindMethod::~AComboBoxBindMethod()
{
    // 必须在所有子类中调用解绑方法
    unbindAll();
}

bool AComboBoxBindMethod::checkBind(const ADWBindParameter& param) const
{
    const QString propName = param.getBindProperty();
    if (propName == CurrentText)
    {
        ADWBindParameterList paramList = getByWidget(param.getWidget());
        for (const ADWBindParameter& param : paramList)
        {
            if (param.getBindProperty() == CurrentIndex)
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "data-widget binding conflict, "
                           "cannot bind peroperty names 'currentIndex' and 'currentText' simultaneously");
                return false;
            }
        }
    }
    else if (propName == CurrentIndex)
    {
        ADWBindParameterList paramList = getByWidget(param.getWidget());
        for (const ADWBindParameter& param : paramList)
        {
            if (param.getBindProperty() == CurrentText)
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "data-widget binding conflict, "
                           "cannot bind peroperty names 'currentIndex' and 'currentText' simultaneously");
                return false;
            }
        }
    }

    return true;
}

bool AComboBoxBindMethod::bind(const ADWBindParameter& param)
{
    QComboBox* editor = qobject_cast<QComboBox*>(param.getWidget());
    if (editor)
    {
        const QString propName = param.getBindProperty();
        if (propName == CurrentText)
        {
            QObject::connect(editor, &QComboBox::currentTextChanged,
                             this, QOverload<const QString&>::of(&AComboBoxBindMethod::comboBoxValueChanged));
        }
        else if (propName == CurrentIndex)
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
        if (propName == CurrentText)
        {
            QObject::disconnect(editor, &QComboBox::currentTextChanged,
                                this, QOverload<const QString&>::of(&AComboBoxBindMethod::comboBoxValueChanged));
        }
        else if (propName == CurrentIndex)
        {
            QObject::disconnect(editor, QOverload<int>::of(&QComboBox::currentIndexChanged),
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
        if (propertyName == Items)
        {
            editor->clear();
            editor->insertItems(0, data->getValue().toStringList());
        }
        else if (propertyName == CurrentIndex)
        {
            editor->setCurrentIndex(data->getValue().toInt());
        }
        else if (propertyName == CurrentText)
        {
            if (data->getType() == EMetaType::QString)
                editor->setCurrentText(data->getValue().toString());
            else if (data->getDataManager())
                editor->setCurrentText(data->getDataManager()->toString(data));
        }
    }
}

void AComboBoxBindMethod::onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName)
{
    if (!data)
        return;

    QComboBox* editor = qobject_cast<QComboBox*>(const_cast<QWidget*>(widget));
    if (editor)
    {
        if (propertyName == CurrentIndex)
        {
            data->setValue(editor->currentIndex());
        }
        else if (propertyName == CurrentText)
        {
            data->setValue(editor->currentText());
        }
    }
}

void AComboBoxBindMethod::comboBoxValueChanged(const QString& val)
{
    widgetValueChanged(val, CurrentText);
}

void AComboBoxBindMethod::comboBoxValueChanged(int val)
{
    widgetValueChanged(val, CurrentIndex);
}

A_DECLARE_DATAWIDGET_BINDMETHOD(QSpinBox, ASpinBoxBindMethod);
A_DECLARE_DATAWIDGET_BINDMETHOD(QDoubleSpinBox, ADoubleSpinBoxBindMethod);
A_DECLARE_DATAWIDGET_BINDMETHOD(QLineEdit, ALineEditBindMethod);
A_DECLARE_DATAWIDGET_BINDMETHOD(QCheckBox, ACheckBoxBindMethod);
A_DECLARE_DATAWIDGET_BINDMETHOD(QRadioButton, ARadioButtonBindMethod);
A_DECLARE_DATAWIDGET_BINDMETHOD(QComboBox, AComboBoxBindMethod);

APROCH_NAMESPACE_END
