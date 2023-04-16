/****************************************************************************
 * @file    AKeyExpressionManager.h
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
 * 键值表达式管理器
 */
class APROCH_API AKeyExpressionManager
{
    APROCH_SINGLETON(AKeyExpressionManager);

public:
    ~AKeyExpressionManager();

    /**
     * @brief 注册键值表达式
     * @tparam KeyExprType 键值表达式类型
     */
    template <class KeyExprType>
    void registerKeyExpression()
    {
        APROCH_ASSERT_IS_DERIVED(AAbstractKeyExpression, KeyExprType);
        QSharedPointer<AAbstractKeyExpression> keyExpr = QSharedPointer<AAbstractKeyExpression>(new KeyExprType);
        mKeyExprMap[keyExpr->symbol()] = keyExpr;
    }

    /**
     * @brief 根据符号获取键值表达式
     * @param symbol 符号
     * @return 键值表达式，如果符号不存在则返回nullptr
     */
    QSharedPointer<AAbstractKeyExpression> getKeyExpr(const QString &symbol) const;

    /**
     * @brief 获取所有支持的符号
     * @return 符号列表
     */
    QStringList getAllSymbols() const;

    /**
     * @brief 是否是运算符号
     * @param symbol 运算符
     * @return 是否是运算符号
     */
    bool isSymbol(const QString &symbol) const;

    /**
     * @brief 获取符号的优先级大小，越小优先级越大
     * @param symbol 符号
     * @return 优先级。如果没有该符号则返回-1
     */
    int getSymbolPriority(const QString &symbol) const;

private:
    AKeyExpressionManager();

private:
    /** 键值表达式列表 */
    QMap<QString, QSharedPointer<AAbstractKeyExpression>> mKeyExprMap;
};

APROCH_NAMESPACE_END
