/****************************************************************************
 * @file    AWidgetFactory.h
 * @date    2023-06-11 
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

template<class WidgetClass>
class ASimpleWidgetFactory;

/**
 * @brief 控件工厂
 */
class APROCH_API AWidgetFactory
{
public:
    /** @brief 实例化控件 */
    static QWidget* newWidget(int type, QWidget* parent = nullptr);

    /** @brief 实例化控件 */
    template<class WidgetType>
    Q_ALWAYS_INLINE static WidgetType* newWidget(int type, QWidget* parent = nullptr)
    {
        return qobject_cast<WidgetType*>(newWidget(type, parent));
    }

    /** @brief 注册工厂 */
    static bool registerFactory(int type, AWidgetFactory* factory);

    /** @brief 注册创建控件的简单工厂 */
    template<class WidgetClass>
    Q_ALWAYS_INLINE static bool registerFactory(int type)
    {
        AWidgetFactory* _factory = new ASimpleWidgetFactory<WidgetClass>();
        return registerFactory(type, _factory);
    }

    /** @brief 取消注册控件工厂 */
    static void unregister(int type);

    /** @brief 是否存在对应名称的控件工厂 */
    static bool hasFactory(int type);

    /** @brief 获取控件工厂 */
    static AWidgetFactory* getFactory(int type);

public:
    AWidgetFactory();
    virtual ~AWidgetFactory();
    
    /** @brief 创建控件的方法 */
    virtual QWidget* createWidget(QWidget* parent) = 0;
};

/**
 * @brief 简单控件工厂
 */
template<class WidgetClass>
class ASimpleWidgetFactory : public AWidgetFactory
{
public:
    virtual QWidget* createWidget(QWidget* parent)
    {
        return new WidgetClass(parent);
    }
};

/** @brief 通过注册简单控件工厂创建的控件工厂宏 */
#define A_WIDGET_FACTORY_REGISTER(WidgetClass, DataType, FlagName)                    \
class A##WidgetClass##_WidgetFactory_##FlagName##_InitClass                           \
{                                                                                     \
public:                                                                               \
    A##WidgetClass##_WidgetFactory_##FlagName##_InitClass()                           \
    {                                                                                 \
        APROCH_PRE_NAMESPACE(AWidgetFactory)::registerFactory<WidgetClass>(DataType); \
    }                                                                                 \
}a##WidgetClass##_WidgetFactory_##FlagName##_Instance

APROCH_NAMESPACE_END
