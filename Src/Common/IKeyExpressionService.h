/****************************************************************************
 * @file    IKeyExpressionService.h
 * @date    2021-9-30
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

#define AServiceName_KeyExpression "aproch.KeyExpressionService"

APROCH_NAMESPACE_BEGIN

/**
 * @brief 键值表达式接口接口
 */
class IKeyExpressionService : public IService
{
public:
    virtual ~IKeyExpressionService() = default;

    /**
     * @brief 设置键值的值
     * @param key 键值
     * @param value 值
     */
    virtual void setValue(const QString &key, const QVariant &value) = 0;

    /**
     * @brief 设置键值的默认值
     * @param key 键值
     * @param value 默认值
     */
    virtual void setDefault(const QString &key, const QVariant &value) = 0;

    /**
     * @brief 获取键值对应的值
     * @param key 键值
     * @return 值
     */
    virtual QVariant getValue(const QString &key) const = 0;

    /**
     * @brief 获取键值对应的默认值
     * @param key 键值
     * @return 值
     */
    virtual QVariant getDefault(const QString &key) const = 0;

    /**
     * @brief 将所有键值重置成默认值
     */
    virtual void reset() = 0;

    /**
     * @brief 将指定键值重置成默认值
     */
    virtual void reset(const QString &key) = 0;

    /**
     * @brief 是否存在指定键值
     * @param key 键值
     * @return 键值是否存在
     */
    virtual bool has(const QString &key) const = 0;

    /**
     * @brief 是否键值有效
     * @param key 键值
     * @return 是否有效
     */
    virtual bool isKeyValid(const QString &key) const = 0;

    /**
     * @brief 是否运算符号有效
     * @param symbol 运算符号
     * @return 是否有效
     */
    virtual bool isSymbolValid(const QString &symbol) const = 0;

    /**
     * @brief 解析表达式
     * @param expr 表达式字符串
     * @return 该表达式为真（true）或假（false）
     */
    virtual bool parse(const QString &expr) = 0;
};

APROCH_NAMESPACE_END
