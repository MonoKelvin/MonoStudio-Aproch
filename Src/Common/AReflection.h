/****************************************************************************
 * @file    AReflection.h
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

#include <QMetaType>

#define A_REFLECTION_IMPLEMENTS(ClassName, Type)                                \
    ClassName::Constructor ClassName::Null = []() -> Type* { return nullptr; }; \
    ClassName::ConstructorMap ClassName::MyConstructorMap

#define A_SHAREDPTR_REFLECTION_IMPLEMENTS(ClassName, Type)                                      \
    ClassName::Constructor ClassName::Null = []() -> QSharedPointer<Type> { return nullptr; };  \
    ClassName::ConstructorMap ClassName::MyConstructorMap

APROCH_NAMESPACE_BEGIN

/**
 * @brief 反射
 *
 * @example
 *
 * class MyClass
 * {
 * public:
 *     int n;
 *     QString str;
 * };
 *
 * // 使用方法
 * AReflection::registerConstructor<MyClass>("My Class");
 * MyClass* obj = AReflection::newInstance<MyClass>("My Class");
 */
class AReflection
{
public:
    typedef void* (*Constructor)();
    typedef QMap<QString, Constructor> ConstructorMap;

    static Constructor Null;

    /** @brief 实例化对象 */
    static void* newInstance(const QString& name);

    /** @brief 实例化对象 */
    template<class T>
    Q_ALWAYS_INLINE static T* newInstance(const QString& name)
    {
        return static_cast<T*>(newInstance(name));
    }

    /** @brief 注册可实例化的对象 */
    template<class T>
    static bool registerConstructor(const QString& name)
    {
        Constructor theConstructor = []() -> void* { return (void*)(new T()); };
        return registerConstructor(name, std::move(theConstructor));
    }

    /** @brief 注册可实例化的对象 */
    static bool registerConstructor(const QString& name, Constructor constructor);

    /** @brief 取消注册。移除可实例化对象的构造器 */
    static void unregister(const QString& name);

    /** @brief 是否存在对应名称的对象构造器 */
    static bool hasConstructor(const QString& name);

    /** @brief 获取可实例化对象的构造器 */
    static const Constructor& constructor(const QString& name);

private:
    static ConstructorMap& constructors() noexcept;
};

/**
 * @brief 模板类反射
 * 
 * @example
 * 
 * // MyColorReflection.h
 * class MyColorReflection : AReflectionT<QColor>
 * {
 * public:
 * };
 *
 * // MyColorReflection.cpp
 * A_REFLECTION_IMPLEMENTS(MyColorReflection, QColor);
 * 
 * // main.cpp
 * MyColorReflection::registerConstructor("lightColor");
 * QColor* pColor = MyColorReflection::newInstance("lightColor");
 * // ...
 * delete pColor;
 * 
 */
template<class T>
class AReflectionT
{
public:
    typedef T* (*Constructor)();
    typedef QMap<QString, Constructor> ConstructorMap;

    static Constructor Null;

    /** @brief 实例化对象 */
    static T* newInstance(const QString& name)
    {
        return constructor(name)();
    }

    /** @brief 注册可实例化的对象 */
    static bool registerConstructor(const QString& name, Constructor constructor)
    {
        if (name.isEmpty())
            return false;
        if (constructors().contains(name))
            qWarning() << AStr("An type constructor named %1 has been overwritten.").arg(name);
        constructors()[name] = constructor;
        return true;
    }

    /** @brief 取消注册。移除可实例化对象的构造器 */
    static void unregister(const QString& name)
    {
        constructors().remove(name);
    }

    /** @brief 是否存在对应名称的对象构造器 */
    static bool hasConstructor(const QString& name)
    {
        return constructors().contains(name);
    }

    /** @brief 获取可实例化对象的构造器 */
    static const Constructor& constructor(const QString& name)
    {
        if (constructors().contains(name))
            return constructors()[name];
        return AReflectionT::Null;
    }

private:
    static ConstructorMap& constructors() noexcept
    {
        return MyConstructorMap;
    }

    static ConstructorMap MyConstructorMap;
};

/**
 * @brief 智能指针模板类反射
 *
 * @example
 *
 * // MyColorPtrReflection.h
 * class MyColorPtrReflection : AReflectionPtrT<QColor>
 * {
 * public:
 * };
 *
 * // MyColorPtrReflection.cpp
 * A_SHAREDPTR_REFLECTION_IMPLEMENTS(MyColorPtrReflection, QColor);
 *
 * // main.cpp
 * MyColorPtrReflection::registerConstructor("lightColor");
 * QSharedPointer<QColor> pColor = MyColorPtrReflection::newInstance("lightColor");
 *
 */
template<class T>
class AReflectionPtrT
{
public:
    typedef QSharedPointer<T> (*Constructor)();
    typedef QMap<QString, Constructor> ConstructorMap;

    static Constructor Null;

    /** @brief 实例化对象 */
    static QSharedPointer<T> newInstance(const QString& name)
    {
        return constructor(name)();
    }

    /** @brief 注册可实例化的对象 */
    static bool registerConstructor(const QString& name, Constructor constructor)
    {
        if (name.isEmpty())
            return false;
        if (constructors().contains(name))
            qWarning() << AStr("An type constructor named %1 has been overwritten.").arg(name);
        constructors()[name] = constructor;
        return true;
    }

    /** @brief 取消注册。移除可实例化对象的构造器 */
    static void unregister(const QString& name)
    {
        constructors().remove(name);
    }

    /** @brief 是否存在对应名称的对象构造器 */
    static bool hasConstructor(const QString& name)
    {
        return constructors().contains(name);
    }

    /** @brief 获取可实例化对象的构造器 */
    static const Constructor& constructor(const QString& name)
    {
        if (constructors().contains(name))
            return constructors()[name];
        return AReflectionPtrT::Null;
    }

private:
    static ConstructorMap& constructors() noexcept
    {
        return MyConstructorMap;
    }

    static ConstructorMap MyConstructorMap;
};


/**
 * @brief 元数据反射。可进行元数据反射的类型需要添加Q_DECLARE_METATYPE宏声明
 *
 * class MyClass
 * {
 * public:
 *     int n;
 *     QString str;
 * };
 * Q_DECLARE_METATYPE(MyClass);
 *
 * // 使用方法
 * MyClass* obj = AReflection::newInstance<MyClass>();
 *
 */
class APROCH_API AMetaReflection
{
public:
    /** @brief 实例化对象 */
    template<class T>
    static T* newInstance()
    {
        const int id = type<T>();
        if (id != QMetaType::UnknownType)
            return static_cast<T*>(QMetaType::create(id));
        return nullptr;
    }

    /** @brief 获取类型的唯一元数据类型 */
    template<class T>
    Q_ALWAYS_INLINE static int type()
    {
        return QMetaType::fromType<T>().id();
    }

    /** @brief 获取类型的类型名称 */
    template<class T>
    Q_ALWAYS_INLINE static QString typeName()
    {
        return QString::fromLocal8Bit(QMetaType::fromType<T>().name());
    }
};

/**
 * @brief 对象类反射
 */
class APROCH_API AObjectReflection
{
public:
    AObjectReflection();

    typedef QObject* (*Constructor)(QObject*);
    typedef QMap<QString, Constructor> ConstructorMap;

    static Constructor Null;

    /** @brief 实例化对象 */
    static QObject* newInstance(const QString& className, QObject* parent = nullptr);

    /** @brief 实例化对象 */
    template<class T>
    Q_ALWAYS_INLINE static T* newInstance(const QString& className, QObject* parent = nullptr)
    {
        return qobject_cast<T*>(newInstance(className, parent));
    }

    /** @brief 注册可实例化对象的构造器 */
    static bool registerConstructor(const QString& className, Constructor constructor);

    /** @brief 注册可实例化对象的构造器 */
    template<class T>
    Q_ALWAYS_INLINE static bool registConstructor()
    {
        Constructor constructor = [](QObject* parent) { return new T(parent); };
        registConstructor(T::staticMetaObject.className(), std::move(constructor));
    }

    /** @brief 取消注册可实例化对象的构造器 */
    static void unregister(const QString& className);

    /** @brief 取消注册可实例化对象的构造器 */
    template<class T>
    Q_ALWAYS_INLINE static void unregister()
    {
        unregister(T::staticMetaObject.className());
    }

    /** @brief 是否存在对应名称的对象构造器 */
    static bool hasConstructor(const QString& className);

    /** @brief 是否存在对应名称的对象构造器 */
    template<class T>
    Q_ALWAYS_INLINE static bool hasConstructor()
    {
        return hasConstructor(T::staticMetaObject.className());
    }

    /** @brief 获取可实例化对象的构造器 */
    static const Constructor& constructor(const QString& className);

    /** @brief 获取可实例化对象的构造器 */
    template<class T>
    Q_ALWAYS_INLINE static const Constructor& constructor()
    {
        return constructor(T::staticMetaObject.className());
    }

private:
    static ConstructorMap& constructors() noexcept;
};

APROCH_NAMESPACE_END
