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
#include "ADataManager.h"

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
    AIntDataManager* mgr = dynamic_cast<AIntDataManager*>(param.getData()->getDataManager());
    if (!mgr)
        return false;

    QSpinBox* spinBox = qobject_cast<QSpinBox*>(param.getWidget());
    if (spinBox)
    {
        connect(mgr, &AIntDataManager::valueChanged, this, &ADataWidgetBindMethod::dataChanged,
                Qt::ConnectionType(Qt::AutoConnection | Qt::UniqueConnection));
        // todo: 添加range、singleStep信号槽连接
        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ADataWidgetBindMethod::widgetChanged,
                Qt::ConnectionType(Qt::AutoConnection | Qt::UniqueConnection));
        return true;
    }

    return false;
}

bool ASpinBoxBindMethod::unbind(AData* data, QWidget* widget, const QString& propName)
{
    AIntDataManager* mgr = dynamic_cast<AIntDataManager*>(data->getDataManager());
    if (!mgr)
        return false;

    QSpinBox* spinBox = qobject_cast<QSpinBox*>(widget);
    if (spinBox)
    {
        disconnect(mgr, &AIntDataManager::valueChanged, this, &ADataWidgetBindMethod::dataChanged);
        disconnect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ASpinBoxBindMethod::widgetChanged);
        return true;
    }

    return false;
}

void ASpinBoxBindMethod::onDataChanged(const AData* data, QWidget* widget, const QString& propertyName)
{
    AIntDataManager* mgr = dynamic_cast<AIntDataManager*>(data->getDataManager());
    if (!mgr)
        return;

    QSpinBox* spinBox = qobject_cast<QSpinBox*>(widget);
    if (spinBox)
    {
        spinBox->setRange(mgr->minimum(data), mgr->maximum(data));
        spinBox->setSingleStep(mgr->singleStep(data));
        spinBox->setValue(mgr->value(data));
    }
}

void ASpinBoxBindMethod::onWidgetChanged(AData* data, const QWidget* widget, const QString& propertyName)
{
    AIntDataManager* mgr = dynamic_cast<AIntDataManager*>(data->getDataManager());
    if (!mgr)
        return;

    QSpinBox* spinBox = qobject_cast<QSpinBox*>(const_cast<QWidget*>(widget));
    if (spinBox)
    {
        mgr->setValue(data, spinBox->value());
    }
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
    ADoubleDataManager* mgr = dynamic_cast<ADoubleDataManager*>(param.getData()->getDataManager());
    if (!mgr)
        return false;

    QDoubleSpinBox* editor = qobject_cast<QDoubleSpinBox*>(param.getWidget());
    if (editor)
    {
        connect(mgr, &ADoubleDataManager::valueChanged, this, &ADataWidgetBindMethod::dataChanged,
                Qt::ConnectionType(Qt::AutoConnection | Qt::UniqueConnection));
        // todo: 添加range、singleStep信号槽连接
        connect(editor, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ADataWidgetBindMethod::widgetChanged,
                Qt::ConnectionType(Qt::AutoConnection | Qt::UniqueConnection));
        return true;
    }

    return false;
}

bool ADoubleSpinBoxBindMethod::unbind(AData* data, QWidget* widget, const QString& propName)
{
    ADoubleDataManager* mgr = dynamic_cast<ADoubleDataManager*>(data->getDataManager());
    if (!mgr)
        return false;

    QDoubleSpinBox* editor = qobject_cast<QDoubleSpinBox*>(widget);
    if (editor)
    {
        disconnect(mgr, &ADoubleDataManager::valueChanged, this, &ADataWidgetBindMethod::dataChanged);
        disconnect(editor, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ADataWidgetBindMethod::widgetChanged);
        return true;
    }

    return false;
}

void ADoubleSpinBoxBindMethod::onDataChanged(const AData* data, QWidget* widget, const QString& propertyName)
{
    ADoubleDataManager* mgr = dynamic_cast<ADoubleDataManager*>(data->getDataManager());
    if (!mgr)
        return;

    QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(widget);
    if (spinBox)
    {
        spinBox->setRange(mgr->minimum(data), mgr->maximum(data));
        spinBox->setSingleStep(mgr->singleStep(data));
        spinBox->setValue(mgr->value(data));
    }
}

void ADoubleSpinBoxBindMethod::onWidgetChanged(AData* data, const QWidget* widget, const QString& propertyName)
{
    ADoubleDataManager* mgr = dynamic_cast<ADoubleDataManager*>(data->getDataManager());
    if (!mgr)
        return;

    QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(const_cast<QWidget*>(widget));
    if (spinBox)
    {
        mgr->setValue(data, spinBox->value());
    }
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
    AStringDataManager* mgr = dynamic_cast<AStringDataManager*>(param.getData()->getDataManager());
    if (!mgr)
        return false;

    QLineEdit* editor = qobject_cast<QLineEdit*>(param.getWidget());
    if (editor)
    {
        connect(mgr, &AStringDataManager::valueChanged, this, &ADataWidgetBindMethod::dataChanged,
                Qt::ConnectionType(Qt::AutoConnection | Qt::UniqueConnection));
        connect(editor, &QLineEdit::textChanged, this, &ADataWidgetBindMethod::widgetChanged,
                Qt::ConnectionType(Qt::AutoConnection | Qt::UniqueConnection));
        return true;
    }

    return false;
}

bool ALineEditBindMethod::unbind(AData* data, QWidget* widget, const QString& propName)
{
    AStringDataManager* mgr = dynamic_cast<AStringDataManager*>(data->getDataManager());
    if (!mgr)
        return false;

    QLineEdit* editor = qobject_cast<QLineEdit*>(widget);
    if (editor)
    {
        disconnect(mgr, &AStringDataManager::valueChanged, this, &ADataWidgetBindMethod::dataChanged);
        disconnect(editor, &QLineEdit::textChanged, this, &ADataWidgetBindMethod::widgetChanged);
        return true;
    }

    return false;
}

void ALineEditBindMethod::onDataChanged(const AData* data, QWidget* widget, const QString& propertyName)
{
    AStringDataManager* mgr = dynamic_cast<AStringDataManager*>(data->getDataManager());
    if (!mgr)
        return;

    QLineEdit* editor = qobject_cast<QLineEdit*>(widget);
    if (editor)
    {
        const QRegExp regExp = mgr->regExp(data);
        const QValidator* oldValidator = editor->validator();
        QValidator* newValidator = nullptr;
        if (regExp.isValid())
            newValidator = new QRegExpValidator(regExp, editor);
        editor->setValidator(newValidator);
        if (oldValidator)
            delete oldValidator;

        editor->setText(mgr->value(data));
    }
}

void ALineEditBindMethod::onWidgetChanged(AData* data, const QWidget* widget, const QString& propertyName)
{
    AStringDataManager* mgr = dynamic_cast<AStringDataManager*>(data->getDataManager());
    if (!mgr)
        return;

    QLineEdit* editor = qobject_cast<QLineEdit*>(const_cast<QWidget*>(widget));
    if (editor)
    {
        mgr->setValue(data, editor->text());
    }
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
        ABoolDataManager* mgr = dynamic_cast<ABoolDataManager*>(param.getData()->getDataManager());
        if (mgr)
        {
            connect(mgr, &ABoolDataManager::valueChanged, this, &ADataWidgetBindMethod::dataChanged,
                    Qt::ConnectionType(Qt::AutoConnection | Qt::UniqueConnection));
        }
        else
        {
            AIntDataManager* intMgr = dynamic_cast<AIntDataManager*>(param.getData()->getDataManager());
            if (intMgr)
                connect(intMgr, &AIntDataManager::valueChanged, this, &ADataWidgetBindMethod::dataChanged,
                        Qt::ConnectionType(Qt::AutoConnection | Qt::UniqueConnection));
            else
                return false;
        }

        connect(editor, &QCheckBox::stateChanged, this, &ACheckBoxBindMethod::widgetChanged,
                Qt::ConnectionType(Qt::AutoConnection | Qt::UniqueConnection));
        return true;
    }

    return false;
}

bool ACheckBoxBindMethod::unbind(AData* data, QWidget* widget, const QString& propName)
{
    QCheckBox* editor = qobject_cast<QCheckBox*>(widget);
    if (editor)
    {
        ABoolDataManager* mgr = dynamic_cast<ABoolDataManager*>(data->getDataManager());
        if (mgr)
        {
            disconnect(mgr, &ABoolDataManager::valueChanged, this, &ADataWidgetBindMethod::dataChanged);
        }
        else
        {
            AIntDataManager* intMgr = dynamic_cast<AIntDataManager*>(data->getDataManager());
            if (intMgr)
                disconnect(intMgr, &AIntDataManager::valueChanged, this, &ADataWidgetBindMethod::dataChanged);
        }

        disconnect(editor, &QCheckBox::stateChanged, this, &ACheckBoxBindMethod::widgetChanged);
        return true;
    }

    return false;
}

void ACheckBoxBindMethod::onDataChanged(const AData* data, QWidget* widget, const QString& propertyName)
{
    QCheckBox* editor = qobject_cast<QCheckBox*>(widget);
    if (editor)
    {
        ABoolDataManager* mgr = dynamic_cast<ABoolDataManager*>(data->getDataManager());
        if (mgr)
        {
            editor->setChecked(mgr->value(data));
        }
        else
        {
            AIntDataManager* intMgr = dynamic_cast<AIntDataManager*>(data->getDataManager());
            if (intMgr)
                editor->setCheckState(Qt::CheckState(intMgr->value(data)));
        }
    }
}

void ACheckBoxBindMethod::onWidgetChanged(AData* data, const QWidget* widget, const QString& propertyName)
{
    QCheckBox* editor = qobject_cast<QCheckBox*>(const_cast<QWidget*>(widget));
    if (editor)
    {
        ABoolDataManager* mgr = dynamic_cast<ABoolDataManager*>(data->getDataManager());
        if (mgr)
        {
            mgr->setValue(data, editor->isChecked());
        }
        else
        {
            AIntDataManager* intMgr = dynamic_cast<AIntDataManager*>(data->getDataManager());
            if (intMgr)
                intMgr->setValue(data, int(editor->checkState()));
        }
    }
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
    ABoolDataManager* mgr = dynamic_cast<ABoolDataManager*>(param.getData()->getDataManager());
    if (!mgr)
        return false;

    QRadioButton* editor = qobject_cast<QRadioButton*>(param.getWidget());
    if (editor)
    {
        connect(mgr, &ABoolDataManager::valueChanged, this, &ADataWidgetBindMethod::dataChanged,
                Qt::ConnectionType(Qt::AutoConnection | Qt::UniqueConnection));
        connect(editor, &QRadioButton::clicked, this, &ARadioButtonBindMethod::widgetChanged,
                Qt::ConnectionType(Qt::AutoConnection | Qt::UniqueConnection));
        return true;
    }

    return false;
}

bool ARadioButtonBindMethod::unbind(AData* data, QWidget* widget, const QString& propName)
{
    ABoolDataManager* mgr = dynamic_cast<ABoolDataManager*>(data->getDataManager());
    if (!mgr)
        return false;

    QRadioButton* editor = qobject_cast<QRadioButton*>(widget);
    if (editor)
    {
        disconnect(mgr, &ABoolDataManager::valueChanged, this, &ADataWidgetBindMethod::dataChanged);
        disconnect(editor, &QRadioButton::clicked, this, &ARadioButtonBindMethod::widgetChanged);
        return true;
    }

    return false;
}

void ARadioButtonBindMethod::onDataChanged(const AData* data, QWidget* widget, const QString& propertyName)
{
    ABoolDataManager* mgr = dynamic_cast<ABoolDataManager*>(data->getDataManager());
    if (!mgr)
        return;

    QRadioButton* editor = qobject_cast<QRadioButton*>(widget);
    if (editor)
    {
        editor->setChecked(mgr->value(data));
    }
}

void ARadioButtonBindMethod::onWidgetChanged(AData* data, const QWidget* widget, const QString& propertyName)
{
    ABoolDataManager* mgr = dynamic_cast<ABoolDataManager*>(data->getDataManager());
    if (!mgr)
        return;

    QRadioButton* editor = qobject_cast<QRadioButton*>(const_cast<QWidget*>(widget));
    if (editor)
    {
        mgr->setValue(data, editor->isChecked());
    }
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
        ADWBindParameterList paramList = getParameters(param.getWidget());
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
        ADWBindParameterList paramList = getParameters(param.getWidget());
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
            AStringDataManager* mgr = dynamic_cast<AStringDataManager*>(param.getData()->getDataManager());
            if (!mgr)
                return false;

            connect(mgr, &AStringDataManager::valueChanged, this, &ADataWidgetBindMethod::dataChanged,
                    Qt::ConnectionType(Qt::AutoConnection | Qt::UniqueConnection));
            connect(editor, &QComboBox::currentTextChanged, this, &AComboBoxBindMethod::comboBoxCurrentTextChanged,
                    Qt::ConnectionType(Qt::AutoConnection | Qt::UniqueConnection));
        }
        else if (propName == CurrentIndex)
        {
            AIntDataManager* mgr = dynamic_cast<AIntDataManager*>(param.getData()->getDataManager());
            if (!mgr)
                return false;

            connect(mgr, &AIntDataManager::valueChanged, this, &ADataWidgetBindMethod::dataChanged,
                    Qt::ConnectionType(Qt::AutoConnection | Qt::UniqueConnection));
            connect(editor, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AComboBoxBindMethod::comboBoxCurrentIndexChanged,
                    Qt::ConnectionType(Qt::AutoConnection | Qt::UniqueConnection));
        }
        else
        {
            AStringListDataManager* mgr = dynamic_cast<AStringListDataManager*>(param.getData()->getDataManager());
            if (!mgr)
                return false;

            connect(mgr, &AStringListDataManager::valueChanged, this, &ADataWidgetBindMethod::dataChanged,
                    Qt::ConnectionType(Qt::AutoConnection | Qt::UniqueConnection));
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
            AStringDataManager* mgr = dynamic_cast<AStringDataManager*>(data->getDataManager());
            if (!mgr)
                return false;

            disconnect(mgr, &AStringDataManager::valueChanged, this, &ADataWidgetBindMethod::dataChanged);
            disconnect(editor, &QComboBox::currentTextChanged, this, &AComboBoxBindMethod::comboBoxCurrentTextChanged);
        }
        else if (propName == CurrentIndex)
        {
            AIntDataManager* mgr = dynamic_cast<AIntDataManager*>(data->getDataManager());
            if (!mgr)
                return false;

            disconnect(mgr, &AIntDataManager::valueChanged, this, &ADataWidgetBindMethod::dataChanged);
            disconnect(editor, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AComboBoxBindMethod::comboBoxCurrentIndexChanged);
        }
        else
        {
            AStringListDataManager* mgr = dynamic_cast<AStringListDataManager*>(data->getDataManager());
            if (!mgr)
                return false;

            disconnect(mgr, &AStringListDataManager::valueChanged, this, &ADataWidgetBindMethod::dataChanged);
        }

        return true;
    }

    return false;
}

void AComboBoxBindMethod::onDataChanged(const AData* data, QWidget* widget, const QString& propertyName)
{
    QComboBox* editor = qobject_cast<QComboBox*>(widget);
    if (editor)
    {
        if (propertyName == CurrentText)
        {
            AStringDataManager* mgr = dynamic_cast<AStringDataManager*>(data->getDataManager());
            if (!mgr)
                return;

            editor->setCurrentText(mgr->value(data));
        }
        else if (propertyName == CurrentIndex)
        {
            AIntDataManager* mgr = dynamic_cast<AIntDataManager*>(data->getDataManager());
            if (!mgr)
                return;

            editor->setCurrentIndex(mgr->value(data));
        }
        else
        {
            AStringListDataManager* mgr = dynamic_cast<AStringListDataManager*>(data->getDataManager());
            if (!mgr)
                return;

            editor->clear();
            editor->insertItems(0, mgr->value(data));
        }
    }
}

void AComboBoxBindMethod::onWidgetChanged(AData* data, const QWidget* widget, const QString& propertyName)
{
    QComboBox* editor = qobject_cast<QComboBox*>(const_cast<QWidget*>(widget));
    if (editor)
    {
        if (propertyName == CurrentText)
        {
            AStringDataManager* mgr = dynamic_cast<AStringDataManager*>(data->getDataManager());
            if (!mgr)
                return;

            mgr->setValue(data, editor->currentText());
        }
        else if (propertyName == CurrentIndex)
        {
            AIntDataManager* mgr = dynamic_cast<AIntDataManager*>(data->getDataManager());
            if (!mgr)
                return;

            mgr->setValue(data, editor->currentIndex());
        }
        else
        {
            AStringListDataManager* mgr = dynamic_cast<AStringListDataManager*>(data->getDataManager());
            if (!mgr)
                return;

            QStringList val;
            for (int i = 0; i < editor->count(); ++i)
                val.push_back(editor->itemText(i));
            mgr->setValue(data, val);
        }
    }
}

void AComboBoxBindMethod::comboBoxCurrentTextChanged(const QString& val)
{
    widgetPropertyChanged(val, CurrentText);
}

void AComboBoxBindMethod::comboBoxCurrentIndexChanged(int val)
{
    widgetPropertyChanged(val, CurrentIndex);
}

A_DECLARE_DATAWIDGET_BINDMETHOD(QSpinBox, ASpinBoxBindMethod);
A_DECLARE_DATAWIDGET_BINDMETHOD(QDoubleSpinBox, ADoubleSpinBoxBindMethod);
A_DECLARE_DATAWIDGET_BINDMETHOD(QLineEdit, ALineEditBindMethod);
A_DECLARE_DATAWIDGET_BINDMETHOD(QCheckBox, ACheckBoxBindMethod);
A_DECLARE_DATAWIDGET_BINDMETHOD(QRadioButton, ARadioButtonBindMethod);
A_DECLARE_DATAWIDGET_BINDMETHOD(QComboBox, AComboBoxBindMethod);

APROCH_NAMESPACE_END
