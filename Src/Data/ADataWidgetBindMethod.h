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

/** @brief 数据绑定类型 */
enum class EDataBindType
{
    /** @brief 不进行绑定 */
    None,

    /** @brief 单向绑定 */
    OneWay,
    
    /** @brief 单向绑定的反向情况 */
    OneWayRevise,

    /** @brief 双向绑定 */
    TwoWay,

    /** @brief 仅在初始时绑定一次 */
    FirstTime,

    /** @brief 仅在初始时绑定一次，并且是反向绑定 */
    FirstTimeRevise,
};

/**
 * 绑定参数
 */
struct SBindParameter
{
    /** @brief 要绑定的数据名称，如果未指定，则可根据控件特性自动设置主要属性的绑定 */
    const char* propertyName = nullptr;

    /** @brief 绑定类型 */
    EDataBindType type = EDataBindType::TwoWay;
};

class ADataWidgetBindMethodPrivate;

/**
 * 数据和控件的绑定方法
 */
class APROCH_API ADataWidgetBindMethod : public QObject
{
    Q_OBJECT
public:
    ADataWidgetBindMethod(QObject* parent = nullptr);

    /** @brief 添加绑定 */
    void addBind(AData* data, QWidget* widget);

    /** @brief 获取数据所绑定的所有控件 */
    QWidgetList getBindWidgets(AData* data);

    /** @brief 获取控件所绑定的数据 */
    QMap<QString, AData*> getBindData(QWidget* widget);

    /** @brief 绑定 */
    virtual bool bind(AData* data, QWidget* widget, const SBindParameter& parameter) = 0;

    /** @brief 解绑 */
    virtual bool unbind(AData* data, QWidget* widget, const char* propName) = 0;

    /** @brief 数据修改更新控件的方法 */
    virtual void onValueChanged(const AData* data, QWidget* widget, const QVariant& old) = 0;

    /** @brief 控件修改更新数据的方法 */
    virtual void onWidgetValueChanged(AData* data, const QWidget* widget) = 0;

    static ADataContainer* dataContainer(AData* data);

protected Q_SLOTS:
    void valueChanged(AData* data, const QVariant& old);
    void widgetValueChanged(const QVariant& val);

private:
    Q_DISABLE_COPY_MOVE(ADataWidgetBindMethod);
    Q_DECLARE_PRIVATE(ADataWidgetBindMethod);
};


APROCH_NAMESPACE_END
