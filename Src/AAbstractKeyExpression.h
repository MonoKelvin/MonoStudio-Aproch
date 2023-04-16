/****************************************************************************
 * @file    AAbstractKeyExpression.h
 * @date    2021-1-10
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

/**
 * 抽象键值表达式。该表达式返回值只有true和false
 */
class APROCH_API AAbstractKeyExpression
{
    friend class AKeyExpressionService;

public:
    AAbstractKeyExpression();
    virtual ~AAbstractKeyExpression();

    /**
     * 键值表达式类型
     */
    enum EKeyExprType
    {
        None = 0,      // 该表达式无效
        Key,           // 键
        Brackets,      // 括号
        False,         // 真
        True,          // 假
        Not,           // 非
        And,           // 与
        Or,            // 或
        Xor,           // 异或
        Equals,        // 等于
        NotEquals,     // 不等于
        Greater,       // 大于
        GreaterEquals, // 大于等于
        Smaller,       // 小于
        SmallerEquals, // 小于等于
        Regex,         // 等于正则表达式
        NotRegex,      // 不等于该正则表达式
        Custom = 256,  // 用户自定义
    };

    /**
     * @brief 优先级
     */
    enum ESymbolPriority
    {
        High = 1, // 高
        Medium,   // 中
        Low,      // 低
    };

    /**
     * @brief 获取键值列表
     * @return 键值列表
     */
    const QStringList &keys(void) const noexcept
    {
        return mKeys;
    }

    /**
     * @brief 获取键值列表
     * @return 键值列表
     */
    QStringList keys(void)
    {
        return mKeys;
    }

    /**
     * @brief 等价于bool类型
     */
    inline operator bool()
    {
        return parse();
    }

    /**
     * @brief 表达式运算符号。
     * @note 这些符号运算结果必须是bool值，一般不会用 +,-,*,/ 等计算符号，而是用 !,>,>=,=,&&,&,|| 等，当然用户可以自定义符号运算规则，
     * 如： key1 + key2 定义规则为：key1 > 0 && key2 > 0。另外符号可以不连续出现，可以是包裹关键字的符号，如: ()，需要在解析函数@see
     * parse 内分析其意义
     * @return 运算符
     */
    virtual const char *symbol() const = 0;

    /**
     * @brief 表达式类型（唯一标识）
     * @return 类型标识，一般返回@see EContextKeyExprType 中的值，如果是用户自定义，则返回 Custom + x, (x = 1,2,3...)
     */
    virtual int type() const = 0;

    /**
     * @brief 符号优先级
     * @return 优先级
     */
    virtual ESymbolPriority priority() const = 0;

    /**
     * @brief 返回操作数据的个数。该返回值决定了键值列表@see keys 中数据的个数
     * @return 操作数据的个数
     */
    virtual int operationCount() const = 0;

protected:
    /**
     * @brief 解析键值表达式
     * @return 解析结果
     */
    virtual bool parse() = 0;

    /**
     * @brief 键值1，运算符号左边
     * @return 键值1的值。如果没有则返回false
     */
    bool key1() const;

    /**
     * @brief 键值2，运算符号右边
     * @return 键值2的值。如果没有则返回false
     */
    bool key2() const;

private:
    /** @brief 键值列表 */
    QStringList mKeys;
};

APROCH_NAMESPACE_END
