/****************************************************************************
 * @file    IDataBindable.h
 * @date    2023-05-01 
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

class ADWBindParameterPrivate;

/**
 * 数据-控件绑定参数.
 */
class APROCH_API ADWBindParameter
{
public:
    ADWBindParameter();
    ADWBindParameter(AData* data, QWidget* widget, const QString& propName = QString(), EDataBindType type = EDataBindType::TwoWay);
    ADWBindParameter(const ADWBindParameter& other);
    ~ADWBindParameter();

    AData* getData() const;
    void setData(AData* data);

    QWidget* getWidget() const;
    void setWidget(QWidget* widget);

    QString getBindProperty() const;
    void setBindProperty(const QString& name);

    EDataBindType getBindType() const;
    void setBindType(EDataBindType type);

    /** @brief 是否有效。数据源和控件对象都不为空 */
    bool isValid() const;

    /** @brief 比较参数是否相等。判断时比较数据、控件和属性，不包括类型 */
    bool operator==(const ADWBindParameter& other) const;
    bool operator!=(const ADWBindParameter& other) const;
    ADWBindParameter& operator=(const ADWBindParameter& other);

private:
    QScopedPointer<ADWBindParameterPrivate> d;
};
Q_DECLARE_METATYPE(ADWBindParameter);
using ADWBindParameterList = QList<ADWBindParameter>;

class ADataWidgetBindMethodPrivate;

/**
 * 数据和控件的绑定方法
 */
class APROCH_API ADataWidgetBindMethod : public QObject
{
    Q_OBJECT
public:
    ADataWidgetBindMethod(QObject* parent = nullptr);
    ~ADataWidgetBindMethod();

    /** @brief 添加绑定。如果绑定对象都一样，绑定类型不一样，则不重复添加，只更新绑定类型 */
    bool addBind(const ADWBindParameter& param);

    /** @brief 移除绑定 */
    bool removeBind(AData* data, QWidget* widget, const QString& propName = QString());

protected:
    /** @brief 绑定 */
    virtual bool bind(const ADWBindParameter& param) = 0;

    /** @brief 解绑 */
    virtual bool unbind(AData* data, QWidget* widget, const QString& propName = QString()) = 0;

    /** @brief 数据修改更新控件的方法 */
    virtual void onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old) = 0;

    /** @brief 控件修改更新数据的方法 */
    virtual void onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName) = 0;

    /** @brief 获取数据所在的数据容器 */
    static ADataContainer* getDataContainer(AData* data);

protected Q_SLOTS:
    void valueChanged(AData* data, const QVariant& old = QVariant());
    void widgetValueChanged(const QVariant& val, const QString& propertyName = QString());
    void widgetDestroyed(QObject* obj);

private Q_SLOTS:
    void onDestroyed(QObject* obj);

private:
    friend class ADataWidgetBinding;
    Q_DISABLE_COPY_MOVE(ADataWidgetBindMethod);
    Q_DECLARE_PRIVATE(ADataWidgetBindMethod);
};


APROCH_NAMESPACE_END
