/****************************************************************************
 * @file    IDPOService.h
 * @date    2022-07-06
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
#define AServiceName_DPO "aproch.DPOService"

APROCH_NAMESPACE_BEGIN
/**
 * @brief 动态解析控件所用数据
 */
class IDPOData
{
public:
    virtual ~IDPOData() = default;
};

/**
 * @brief 对象创建器
 */
class IObjectCreator
{
public:
    virtual ~IObjectCreator() = default;

    /**
     * @brief 对象标识名称，可以通过"|"字符分割以创建同一个对象创建器
     * @note 注册时不区分大小写
     * @return 标识名称
     */
    virtual QString name(void) const = 0;

    /**
     * @brief 创建对象
     * @param name 标识名称
     * @param data DPO数据
     * @param parent 父对象
     * @return 创建的对象
     */
    virtual QObject *createObject(const QString &name, IDPOData &data, QObject *parent = nullptr) = 0;
};

/**
 * @brief 动态解析对象（DPO, Dynamically Parse Object）服务
 */
class IDPOService : public IService
{
public:
    virtual ~IDPOService() = default;

    /**
     * @brief 注册对象创建器
     * @param creator 创建器
     * @return 是否注册成功
     */
    virtual bool registerObjectCreator(const QSharedPointer<IObjectCreator> &creator) = 0;

    /**
     * @brief 删除注册的对象创建器
     * @param name 要删除的对象创建器的名称
     * @return 是否删除成功
     */
    virtual bool unregisterObjectCreator(const QString &name) = 0;

    /**
     * @brief 获取对象创建器
     * @param name 名称
     * @return 对象创建器
     */
    virtual QSharedPointer<IObjectCreator> getObjectCreator(const QString &name) const = 0;

    /**
     * @brief 解析
     * @param data 数据
     * @param parent 父对象
     * @return
     */
    virtual QObject *prase(const QString &data, QObject *parent = nullptr) = 0;

    /**
     * @brief 序列化
     * @return QString 序列化后的字符串
     */
    virtual QString serialize(QObject *object) = 0;
};

APROCH_NAMESPACE_END
