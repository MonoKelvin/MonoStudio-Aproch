/****************************************************************************
 * @file    ADataWidgetBinding.h
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
#include "ADataWidgetBindMethod.h"

APROCH_NAMESPACE_BEGIN

class ADataWidgetBindingPrivate;

class APROCH_API ADataWidgetBinding : public QObject
{
    Q_OBJECT
public:
    explicit ADataWidgetBinding(QObject* parent = nullptr);
    ~ADataWidgetBinding();

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
    virtual bool bind(const ADWBindParameter& parameter);

    /** @brief 自动根据控件类型进行解绑 */
    virtual bool unbind(AData* data, QWidget* widget, const QString& propName = QString());

private:
    static bool addBindMethod(ADataWidgetBindMethodPtr bindMethod, const char* widgetTypeName);
    static bool removeBindMethod(const char* widgetTypeName);

private:
    Q_DISABLE_COPY_MOVE(ADataWidgetBinding);
    Q_DECLARE_PRIVATE(ADataWidgetBinding);
};

APROCH_NAMESPACE_END
