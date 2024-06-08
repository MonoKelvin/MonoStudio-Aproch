/****************************************************************************
 * @file    ADataWidgetBindMethod.h
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
#pragma once

APROCH_NAMESPACE_BEGIN

#define A_DECLARE_DATAWIDGET_BINDMETHOD(WidgetType, DWBindMethod)                                       \
    class A##WidgetType##_##DWBindMethod##_InitClass                                                    \
    {                                                                                                   \
    public:                                                                                             \
        A##WidgetType##_##DWBindMethod##_InitClass()                                                    \
        {                                                                                               \
            APROCH_PRE_NAMESPACE(ADataWidgetBindMethod)::addBindMethod<WidgetType>(new DWBindMethod);   \
        }                                                                                               \
    }a##WidgetType##_##DWBindMethod##_Instance

/**
 * 数据-控件绑定参数.
 */
class ADWBindParameter
{
public:
    ADWBindParameter()
        : ADWBindParameter(nullptr, nullptr, QString(), EDataBindType::None)
    {
    }

    ADWBindParameter(AData* data, QWidget* widget, const QString& propName = QString(), EDataBindType type = EDataBindType::TwoWay)
        : m_data(data), m_widget(widget), m_propName(propName), m_type(type)
    {
    }

    ADWBindParameter(const ADWBindParameter& other)
        : ADWBindParameter(other.m_data, other.m_widget, other.m_propName, other.m_type)
    {
    }

    ~ADWBindParameter()
    {
    }

    Q_ALWAYS_INLINE AData* getData() const noexcept
    {
        return m_data;
    }

    Q_ALWAYS_INLINE void setData(AData* data) noexcept
    {
        m_data = data;
    }

    Q_ALWAYS_INLINE QWidget* getWidget() const noexcept
    {
        return m_widget;
    }

    Q_ALWAYS_INLINE void setWidget(QWidget* widget) noexcept
    {
        m_widget = widget;
    }

    Q_ALWAYS_INLINE QString getBindProperty() const noexcept
    {
        return m_propName;
    }

    Q_ALWAYS_INLINE void setBindProperty(const QString& name) noexcept
    {
        m_propName = name;
    }

    Q_ALWAYS_INLINE EDataBindType getBindType() const noexcept
    {
        return m_type;
    }

    Q_ALWAYS_INLINE void setBindType(EDataBindType type) noexcept
    {
        m_type = type;
    }

    /** @brief 是否有效。数据源和控件对象都不为空 */
    Q_ALWAYS_INLINE bool isValid() const noexcept
    {
        return m_data && m_widget;
    }

    /** @brief 是否和另一个绑定数据完全相等 */
    Q_ALWAYS_INLINE bool isEqual(const ADWBindParameter& other) const
    {
        return (m_data == other.m_data) && (m_widget == other.m_widget) &&
            (m_propName == other.m_propName) && (m_type == other.m_type);
    }

    /** @brief 比较参数是否相等。判断时比较数据、控件和属性，不包括类型 */
    Q_ALWAYS_INLINE bool operator==(const ADWBindParameter& other) const
    {
        return (m_data == other.m_data) && (m_widget == other.m_widget) && (m_propName == other.m_propName);
    }

    Q_ALWAYS_INLINE bool operator!=(const ADWBindParameter& other) const
    {
        return !(operator==(other));
    }

    Q_ALWAYS_INLINE ADWBindParameter& operator=(const ADWBindParameter& other)
    {
        m_data = other.m_data;
        m_widget = other.m_widget;
        m_propName = other.m_propName;
        m_type = other.m_type;
        return *this;
    }

private:
    /** @brief 数据 */
    AData* m_data = nullptr;

    /** @brief 控件 */
    QWidget* m_widget = nullptr;

    /** @brief 控件属性 */
    QString m_propName;

    /** @brief 绑定类型 */
    EDataBindType m_type = EDataBindType::None;
};
Q_DECLARE_METATYPE(ADWBindParameter);
using ADWBindParameterList = QList<ADWBindParameter>;

class ADataWidgetBindMethod;
class ADataWidgetBindMethodPrivate;
typedef QPointer<ADataWidgetBindMethod> ADataWidgetBindMethodPtr;

/**
 * 数据和控件的绑定方法
 */
class APROCH_API ADataWidgetBindMethod : public QObject
{
    Q_OBJECT
public:
    ADataWidgetBindMethod(QObject* parent = nullptr);
    ~ADataWidgetBindMethod();

    /** @brief 添加数据-控件的绑定方法 */
    template<class WidgetType>
    static bool addBindMethod(ADataWidgetBindMethodPtr bindMethod)
    {
        const char* widgetTypeName = WidgetType::staticMetaObject.className();
        return addBindMethod(bindMethod, widgetTypeName);
    }

    /** @brief 移除数据-控件的绑定方法 */
    template<class WidgetType>
    static void removeBindMethod()
    {
        const char* widgetTypeName = WidgetType::staticMetaObject.className();
        removeBindMethod(widgetTypeName);
    }
    static void removeBindMethod(ADataWidgetBindMethodPtr bindMethod);

    /** @brief 获取数据-控件的绑定方法 */
    template<class WidgetType>
    static ADataWidgetBindMethodPtr getBindMethod()
    {
        const char* widgetTypeName = WidgetType::staticMetaObject.className();
        return getBindMethod(widgetTypeName);
    }
    static ADataWidgetBindMethodPtr getBindMethod(const char* widgetTypeName);
    
    /** @brief 自动根据控件类型添加绑定 */
    static bool addBind(const ADWBindParameter& parameter);

    /** @brief 自动根据控件类型进行解绑 */
    static bool removeBind(AData* data, QWidget* widget, const QString& propName = QString());

public:
    /** @brief 检查参数是否可以进行绑定 */
    virtual bool checkBind(const ADWBindParameter& param) const;

    /**
     * @brief 数据和控件之间是否存在绑定。
     * isConsiderEmptyProp: 是否考虑空属性。
     *      true: propName为空时只查找数据-控件之间为空属性的绑定，
     *      false: 查找数据-控件之间任意的属性绑定
     */
    bool hasBind(AData* data, QWidget* widget, const QString& propName = QString(), bool isConsiderEmptyProp = true) const;

    /** @brief 获取和指定数据存在绑定的参数列表 */
    ADWBindParameterList getParameters(AData* data) const;

    /** @brief 获取和指定控件存在绑定的参数列表。未指定属性名称时，返回查找到的所有参数 */
    ADWBindParameterList getParameters(QWidget* widget, const QString& propName = QString()) const;

protected:
    /** @brief 绑定 */
    virtual bool bind(const ADWBindParameter& param) = 0;

    /** @brief 解绑 */
    virtual bool unbind(AData* data, QWidget* widget, const QString& propName = QString()) = 0;

    /** @brief 数据修改更新控件的方法 */
    virtual void onDataChanged(const AData* data, QWidget* widget, const QString& propertyName) = 0;

    /** @brief 控件修改更新数据的方法 */
    virtual void onWidgetChanged(AData* data, const QWidget* widget, const QString& propertyName) = 0;

public Q_SLOTS:
    /** @brief 解绑所有 */
    void unbindAll();

    /** @brief 数据修改 */
    void dataChanged(AData* data, const QVariant& newVal);

    /** @brief 控件修改 */
    void widgetChanged(const QVariant& val);

    /** @brief 控件属性修改 */
    void widgetPropertyChanged(const QVariant& val, const QString& propertyName);

private Q_SLOTS:
    void widgetDestroyed(QObject* obj);
    void dataDestroyed(AData* data);

private:
    static bool addBindMethod(ADataWidgetBindMethodPtr bindMethod, const char* widgetTypeName);
    static bool removeBindMethod(const char* widgetTypeName);

private:
    friend class ADataWidgetBinding;
    Q_DISABLE_COPY_MOVE(ADataWidgetBindMethod);
    Q_DECLARE_PRIVATE(ADataWidgetBindMethod);

private:
    QScopedPointer<ADataWidgetBindMethodPrivate> d_ptr;
};

APROCH_NAMESPACE_END
