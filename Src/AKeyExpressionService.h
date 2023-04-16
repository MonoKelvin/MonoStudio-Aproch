/****************************************************************************
 * @file    AKeyExpressionService.h
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

APROCH_NAMESPACE_BEGIN

class AAbstractKeyExpression;

/**
 * @brief 键值表达式服务
 */
class APROCH_API AKeyExpressionService : public IKeyExpressionService
{
public:
    /**
     * @brief 键值数据
     */
    struct SKeyValueData
    {
        QVariant value;        // 值
        QVariant defaultValue; // 默认值

        SKeyValueData()
            : value(false), defaultValue(false)
        {
        }

        SKeyValueData(const QVariant &val)
            : value(val)
        {
        }

        SKeyValueData(const QVariant &val, const QVariant &defaultVal)
            : value(val), defaultValue(defaultVal)
        {
        }
    };
    using TKeyMap = QMap<QString, SKeyValueData>;

    /**
     * @brief 关键字
     */
    struct SKeyword
    {
        enum Type
        {
            Symbol,
            Key,
            Number,
            String,
        };
        QString keyword; // 关键字
        Type type;       // 关键字类型
    };
    using TKeywordList = QVector<SKeyword>;

    AKeyExpressionService();

    /**
     * @brief 设置键值的值。如果没有键将新增该键并赋值
     * @param key 键值
     * @param value 值
     */
    virtual void setValue(const QString &key, const QVariant &value) override;

    /**
     * @brief 设置键值的默认值。如果没有键将新增该键并赋值
     * @param key 键值
     * @param value 默认值
     */
    virtual void setDefault(const QString &key, const QVariant &value) override;

    /**
     * @brief 获取键值对应的值
     * @param key 键值
     * @return 值。如果值无效返回或者没有对应的键，则返回 QVariant(false)
     */
    virtual QVariant getValue(const QString &key) const override;

    /**
     * @brief 获取键值对应的默认值
     * @param key 键值
     * @return 值。如果值无效返回或者没有对应的键，则返回 QVariant(false)
     */
    virtual QVariant getDefault(const QString &key) const override;

    /**
     * @brief 将所有键值重置成默认值，如果键值没有设置默认值，则不重置
     */
    virtual void reset() override;

    /**
     * @brief 将指定键值重置成默认值，如果键值没有设置默认值，则不重置
     */
    virtual void reset(const QString &key) override;

    /**
     * @brief 是否存在指定键值
     * @param key 键值
     * @return 键值是否存在
     */
    virtual bool has(const QString &key) const override;

    /**
     * @brief 是否键值有效
     * @param key 键值
     * @return 是否有效
     */
    virtual bool isKeyValid(const QString &key) const override;

    /**
     * @brief 是否运算符号有效
     * @param symbol 运算符号
     * @return 是否有效
     */
    virtual bool isSymbolValid(const QString &symbol) const override;

    /**
     * @brief 解析表达式，如果表达式解析出错则返回false，错误信息通过@see getError 获取
     * @param expr 表达式字符串
     * @return 该表达式为真（true）或假（false）
     */
    virtual bool parse(const QString &expr) override;

public:
    /** @brief false键值数据。对于无效的值或者查找不到的键，都返回该字段作为默认值 */
    static const SKeyValueData falseKeyValueData;

protected:
    /**
     * @brief 提取关键字
     * @param stringList 字符列表
     * @param keywordList 关键字列表
     * @return 是否出错
     */
    bool extractKeywords(const QStringList &stringList, TKeywordList &keywordList) const;

    /**
     * @brief 给定键是否是默认的键
     * @param key 键
     * @return 如果是默认的键就返回true，否则返回false
     */
    const bool isDefaultKey(const QString &key) const;

    /**
     * @brief 中缀表达式转后缀表达式
     * @param expr 表达式
     * @return bool 是否转换失败
     */
    bool convertToPostfixExpression(QStringList &expr) const;

    /**
     * @brief 计算后缀表达式
     * @param postfixExpr 后缀表达式
     * @return 计算结果
     */
    bool computePostfixExpr(TKeywordList &postfixExpr) const;

protected:
    /** @brief 键值表 */
    TKeyMap mKeyMap;

    /** @brief 默认的键值表，不可更改里面的值 */
    TKeyMap mDefaultKeyMap;

private:
    /** @brief 当前正在处理的后缀表达式 */
    TKeywordList mPostfixExpr;
};

APROCH_NAMESPACE_END
