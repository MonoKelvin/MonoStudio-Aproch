/****************************************************************************
 * @file    ADWBindMethods.h
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
#pragma once

APROCH_NAMESPACE_BEGIN

class APROCH_API ASpinBoxBindMethod : public ADataWidgetBindMethod
{
    Q_OBJECT
public:
    ASpinBoxBindMethod(QObject* parent = nullptr);
    ~ASpinBoxBindMethod();

    virtual bool bind(const ADWBindParameter& param) override;
    virtual bool unbind(AData* data, QWidget* widget, const QString& propName = QString()) override;
    virtual void onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old) override;
    virtual void onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName) override;

protected Q_SLOTS:
    void spinboxValueChanged(int);
};

class APROCH_API ADoubleSpinBoxBindMethod : public ADataWidgetBindMethod
{
    Q_OBJECT
public:
    ADoubleSpinBoxBindMethod(QObject* parent = nullptr);
    ~ADoubleSpinBoxBindMethod();

    virtual bool bind(const ADWBindParameter& param) override;
    virtual bool unbind(AData* data, QWidget* widget, const QString& propName = QString()) override;
    virtual void onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old) override;
    virtual void onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName) override;

protected Q_SLOTS:
    void spinboxValueChanged(double);
};

class APROCH_API ALineEditBindMethod : public ADataWidgetBindMethod
{
    Q_OBJECT
public:
    ALineEditBindMethod(QObject* parent = nullptr);
    ~ALineEditBindMethod();

    virtual bool bind(const ADWBindParameter& param) override;
    virtual bool unbind(AData* data, QWidget* widget, const QString& propName = QString()) override;
    virtual void onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old) override;
    virtual void onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName) override;

protected Q_SLOTS:
    void lineEditValueChanged(const QString&);
};

class APROCH_API ACheckBoxBindMethod : public ADataWidgetBindMethod
{
    Q_OBJECT
public:
    ACheckBoxBindMethod(QObject* parent = nullptr);
    ~ACheckBoxBindMethod();

    virtual bool bind(const ADWBindParameter& param) override;
    virtual bool unbind(AData* data, QWidget* widget, const QString& propName = QString()) override;
    virtual void onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old) override;
    virtual void onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName) override;

protected Q_SLOTS:
    void checkBoxValueChanged(int);
};

class APROCH_API ARadioButtonBindMethod : public ADataWidgetBindMethod
{
    Q_OBJECT
public:
    ARadioButtonBindMethod(QObject* parent = nullptr);
    ~ARadioButtonBindMethod();

    virtual bool bind(const ADWBindParameter& param) override;
    virtual bool unbind(AData* data, QWidget* widget, const QString& propName = QString()) override;
    virtual void onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old) override;
    virtual void onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName) override;

protected Q_SLOTS:
    void radioButtonValueChanged(bool);
};

class APROCH_API AComboBoxBindMethod : public ADataWidgetBindMethod
{
    Q_OBJECT
public:
    AComboBoxBindMethod(QObject* parent = nullptr);
    ~AComboBoxBindMethod();

    /** @brief Supported property names */
    static const QString Items;             // "items"
    static const QString CurrentIndex;      // "currentIndex"
    static const QString CurrentText;       // "currentText"

    virtual bool checkBind(const ADWBindParameter& param) const;
    virtual bool bind(const ADWBindParameter& param) override;
    virtual bool unbind(AData* data, QWidget* widget, const QString& propName = QString()) override;
    virtual void onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old) override;
    virtual void onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName) override;

protected Q_SLOTS:
    void comboBoxValueChanged(const QString&);
    void comboBoxValueChanged(int);
};


APROCH_NAMESPACE_END
