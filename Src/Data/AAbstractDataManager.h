/****************************************************************************
 * @file    AAbstractDataManager.h
 * @date    2022-7-16
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
#include "AData.h"

APROCH_NAMESPACE_BEGIN

class AAbstractDataManagerPrivate;

/**
 * @brief 抽象数据管理器
 */
class APROCH_API AAbstractDataManager : public QObject
{
    Q_OBJECT
public:
    explicit AAbstractDataManager(QObject* parent = nullptr);
    ~AAbstractDataManager();

    /** @brief 获取所有数据的列表 */
    QSet<AData*> getDataList() const;

    /** @brief 清空管理的数据 */
    void clear() const;

    /** @brief 添加数据 */
    AData* addData(const QString& name = QString());

    /** @brief 获取类型 */
    virtual int getType() const = 0;

Q_SIGNALS:
    /** @brief 信号：当添加子数据时。after为空时插入到parent第一个子节点前 */
    void dataInserted(AData* data, AData* parent, AData* after);

    /** @brief 信号：数据修改 */
    void dataChanged(AData* data);

    /** @brief 信号：数据从父级移除 */
    void dataRemoved(AData* data, AData* parent);

    /** @brief 信号：数据被析构前 */
    void dataDestroyed(AData* data);

protected:
    /** @brief 获取指定数据是否被管理，或是否存在真实数据 */
    virtual bool hasValue(const AData* data) const;

    /** @brief 获取指定数据对应的图标 */
    virtual QIcon valueIcon(const AData* data) const;

    /** @brief 获取数据的文本形式 */
    virtual QString toString(const AData* data) const;

    /** @brief 创建数据对象 */
    virtual AData* createData();

    /** @brief 当数据创建后并加入到管理器中后的立即初始化动作，@see createData,addData */
    virtual void initializeData(AData* data) = 0;

    /** @brief 析构数据前并不再被该管理器管理时的动作，@see dataDestroyed */
    virtual void uninitializeData(AData* data);

private:
    /** @brief 释放数据内存 */
    void _destroyData(AData* data);

private:
    friend class AData;
    QScopedPointer<AAbstractDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(AAbstractDataManager);
    Q_DISABLE_COPY_MOVE(AAbstractDataManager);
};

/**
 * @brief 数据管理器注册类
 */
class APROCH_API ADataManagerRegistry
{
public:
    typedef AAbstractDataManager* (*Constructor)(QObject* parent);
    typedef QMap<int, Constructor> ConstructorMap;

    /** @brief 空构造器 */
    static const Constructor Null;

    /** @brief 实例化对象 */
    Q_ALWAYS_INLINE static AAbstractDataManager* newDataManager(int type, QObject* parent = nullptr)
    {
        return constructor(type)(parent);
    }

    /** @brief 注册可实例化的对象 */
    template<class T>
    static bool registerDataManager(int type)
    {
        Constructor _constructor = [](QObject* parent) -> AAbstractDataManager* { return new T(parent); };
        return registerDataManager(type, _constructor);
    }

    /** @brief 注册可实例化的对象 */
    static bool registerDataManager(int type, Constructor constructor);

    /** @brief 取消注册 */
    static void unregister(int type);

private:
    ADataManagerRegistry() noexcept;
    ADataManagerRegistry(const ADataManagerRegistry&) = delete;
    ADataManagerRegistry& operator=(const ADataManagerRegistry&) = delete;

private:
    static const Constructor& constructor(int type);
    static ConstructorMap& constructors() noexcept;
    static ConstructorMap MyConstructorMap;
};

class ADataManagerContainerPrivate;

/**
 * @brief 数据管理器容器
 */
class APROCH_API ADataManagerContainer : public QObject
{
    Q_OBJECT;
public:
    explicit ADataManagerContainer(QObject* parent = nullptr);
    ~ADataManagerContainer();

    /** @brief 获取数据管理器，如果不存在则从ADataManagerRegistry中创建 */
    AAbstractDataManager* getDataManager(int type);

Q_SIGNALS:
    /** @brief 信号：数据管理器创新增 */
    void dataManagerAdded(AAbstractDataManager* dataManager);

public Q_SLOTS: 
    /** @brief 移除管理指定类型的数据管理器 */
    void removeDataManager(int type);

    /** @brief 移除所有数据管理器 */
    void removeAll();

private Q_SLOTS:
    void onDataManagerDestroyed(QObject*);

private:
    QScopedPointer<ADataManagerContainerPrivate> d_ptr;
};

/** @brief 定义数据管理器自动注册的宏 */
#define A_DATA_MANAGER_REGISTER(ClassName, MetaType)                                                \
    class A##ClassName##_##DataManager##_InitClass                                                  \
    {                                                                                               \
    public:                                                                                         \
        A##ClassName##_##DataManager##_InitClass()                                                  \
        {                                                                                           \
            APROCH_PRE_NAMESPACE(ADataManagerRegistry)::registerDataManager<ClassName>(MetaType);   \
        }                                                                                           \
    }a##ClassName##_##DataManager##_Instance

APROCH_NAMESPACE_END
