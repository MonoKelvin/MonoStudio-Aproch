/****************************************************************************
 * @file    AKeyExpressionManager.cpp
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
#include "stdafx.h"
#include "AKeyExpressionManager.h"

APROCH_NAMESPACE_BEGIN

APROCH_INIT_SINGLETON(AKeyExpressionManager);

AKeyExpressionManager::AKeyExpressionManager()
{
    // 注册默认的键值表达式
    registerKeyExpression<AAndKeyExpression>();
    registerKeyExpression<AOrKeyExpression>();
}

AKeyExpressionManager::~AKeyExpressionManager()
{
}

int AKeyExpressionManager::getSymbolPriority(const QString &symbol) const
{
    if (symbol == AKey_LeftBracket)
        return 0;
    if (symbol == AKey_RightBracket)
        return 10;

    const auto &keyExpr = getKeyExpr(symbol);
    if (nullptr == keyExpr)
        return -1;

    return int(keyExpr->priority());
}

bool AKeyExpressionManager::isSymbol(const QString &symbol) const
{
    return getKeyExpr(symbol) || symbol == AKey_LeftBracket || symbol == AKey_RightBracket;
}

QSharedPointer<AAbstractKeyExpression> AKeyExpressionManager::getKeyExpr(const QString &symbol) const
{
    return mKeyExprMap.value(symbol, nullptr);
}

QStringList AKeyExpressionManager::getAllSymbols() const
{
    QStringList stringList;
    stringList.push_back(AKey_LeftBracket);
    stringList.push_back(AKey_RightBracket);

    const auto &keys = mKeyExprMap.keys();
    for (const auto& key : keys)
    {
        if (stringList.contains(key))
            continue;
        stringList.push_back(key);
    }

    return stringList;
}

APROCH_NAMESPACE_END
