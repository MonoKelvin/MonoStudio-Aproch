/****************************************************************************
 * @file    AKeyExpressionService.cpp
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
#include "stdafx.h"
#include "AKeyExpressionService.h"

APROCH_NAMESPACE_BEGIN

const AKeyExpressionService::SKeyValueData AKeyExpressionService::falseKeyValueData;

AKeyExpressionService::AKeyExpressionService()
{
    // 添加默认的值
    mDefaultKeyMap.insert(AKey_True, SKeyValueData(true));
    mDefaultKeyMap.insert(AKey_False, SKeyValueData(false));
    mDefaultKeyMap.insert(AKey_0, SKeyValueData(0));
    mDefaultKeyMap.insert(AKey_1, SKeyValueData(1));
    mDefaultKeyMap.insert(AStr("pi"), SKeyValueData(M_PI));
    mDefaultKeyMap.insert(AStr("pi_2"), SKeyValueData(M_PI_2));
    mDefaultKeyMap.insert(AStr("pi_4"), SKeyValueData(M_PI_4));
}

void AKeyExpressionService::setValue(const QString &key, const QVariant &value)
{
    const QString k = key.trimmed();
    if (!isKeyValid(k) || isDefaultKey(key))
        return;

    mKeyMap[k].value = value;
}

void AKeyExpressionService::setDefault(const QString &key, const QVariant &value)
{
    const QString k = key.trimmed();
    if (!isKeyValid(k) || isDefaultKey(key))
        return;

    mKeyMap[k].defaultValue = value;
}

QVariant AKeyExpressionService::getValue(const QString &key) const
{
    const SKeyValueData &kyData = mKeyMap.value(key.trimmed(), falseKeyValueData);
    return kyData.value;
}

QVariant AKeyExpressionService::getDefault(const QString &key) const
{
    const SKeyValueData &kyData = mKeyMap.value(key.trimmed(), falseKeyValueData);
    return kyData.defaultValue;
}

void AKeyExpressionService::reset()
{
    for (auto iter = mKeyMap.keyValueBegin(); iter != mKeyMap.keyValueEnd(); ++iter)
    {
        iter.base().value().value = iter.base().value().defaultValue;
    }
}

void AKeyExpressionService::reset(const QString &key)
{
    if (mKeyMap.contains(key))
    {
        auto &&v = mKeyMap[key];
        if (v.defaultValue.isValid())
            v.value = v.defaultValue;
    }
}

bool AKeyExpressionService::has(const QString &key) const
{
    return mKeyMap.contains(key) || isDefaultKey(key);
}

bool AKeyExpressionService::isKeyValid(const QString &key) const
{
    const QRegExp reg(AStr("^[a-z|_]+[\d\a-z_]*$"), Qt::CaseInsensitive);
    return reg.exactMatch(key);
}

bool AKeyExpressionService::isSymbolValid(const QString &symbol) const
{
    // TODO
    return true;
}

bool AKeyExpressionService::parse(const QString &expr)
{
    const auto &symbols = AKeyExpressionManager::getInstance()->getAllSymbols();
    const QString theExpr = AStringToolkit::WrapSymbolsWithChars(expr, symbols, " ");

    QStringList currentExpr = theExpr.trimmed().split(' ', QString::SkipEmptyParts);
    qDebug() << "Expression Splitted:" << currentExpr;

    if (!convertToPostfixExpression(currentExpr))
        return false;
    qDebug() << "Postfix Expression:" << currentExpr;

    if (!extractKeywords(currentExpr, mPostfixExpr) || mPostfixExpr.empty())
        return false;

    return computePostfixExpr(mPostfixExpr);
}

bool AKeyExpressionService::extractKeywords(const QStringList &stringList, TKeywordList &keywordList) const
{
    keywordList.clear();

    for (const auto &k : stringList)
    {
        SKeyword keyword;
        keyword.type = SKeyword::Key;
        keyword.keyword = k;

        if (!AKeyExpressionManager::getInstance()->isSymbol(k))
        {
            if (!isKeyValid(k))
            {
                bool ok = false;
                k.toDouble(&ok);
                if (ok)
                {
                    keyword.type = SKeyword::Number;
                }
                else if ((k.startsWith('\'') || k.startsWith('\"')) && (k.endsWith('\'') || k.endsWith('\"')))
                {
                    keyword.type = SKeyword::String;
                }
                else
                {
                    // TODO: 添加日志。键出错，就当false处理
                    qDebug() << "The key [" << k << "] is invalid, treated as [false].";
                    keyword.keyword = AKey_False;
                }
            }
        }
        else if (!isSymbolValid(k)) // 符号出错，直接返回false
        {
            // TODO: 添加日志
            qDebug() << "The symbol [" << k << "] is invalid, parsing error.";
            return false;
        }
        else
        {
            keyword.type = SKeyword::Symbol;
        }

        keywordList.push_back(keyword);
    }

    return true;
}

const bool AKeyExpressionService::isDefaultKey(const QString &key) const
{
    return mDefaultKeyMap.contains(key);
}

bool AKeyExpressionService::convertToPostfixExpression(QStringList &expr) const
{
    QStringList postfixExpr;
    QStack<QString> symbolStack;

    while (!expr.empty())
    {
        const QString theVal = expr.takeFirst();
        const int priority = AKeyExpressionManager::getInstance()->getSymbolPriority(theVal);
        if (priority < 0) // 键
        {
            postfixExpr.push_back(theVal);
        }
        else // 符号
        {
            if (theVal != AKey_RightBracket)
            {
                while (!symbolStack.empty() &&
                       symbolStack.top() != AKey_LeftBracket &&
                       AKeyExpressionManager::getInstance()->getSymbolPriority(symbolStack.top()) <= priority)
                {
                    postfixExpr.push_back(symbolStack.pop());
                }
            }
            else
            {
                while (!symbolStack.empty() && symbolStack.top() != AKey_LeftBracket)
                    postfixExpr.push_back(symbolStack.pop());

                // 没有匹配到左括号
                if (symbolStack.empty())
                    return false;

                symbolStack.pop();
                continue;
            }

            symbolStack.push(theVal);
        }
    }

    // 没有匹配到右括号
    if (symbolStack.indexOf(AKey_LeftBracket) >= 0)
        return false;

    while (!symbolStack.empty())
        postfixExpr.push_back(symbolStack.pop());

    expr = postfixExpr;
    return true;
}

bool AKeyExpressionService::computePostfixExpr(TKeywordList &postfixExpr) const
{
    QStack<QString> resultStack;
    for (const auto &theVal : postfixExpr)
    {
        QString theKeyword = theVal.keyword;
        switch (theVal.type)
        {
        case SKeyword::Symbol:
        {
            const auto &keyExpr = AKeyExpressionManager::getInstance()->getKeyExpr(theKeyword);
            if (nullptr == keyExpr)
                return false;

            int opCount = keyExpr->operationCount();
            const auto &keylist = resultStack.toList();
            keyExpr->mKeys = keylist.mid(keylist.size() - opCount);

            while (opCount-- > 0)
                resultStack.pop();

            theKeyword = keyExpr->parse() ? AKey_True : AKey_False;
        }
        break;
        case SKeyword::String:
            // 去除两边的引号
            theKeyword = QString(theVal.keyword.mid(1, theVal.keyword.size() - 2));
            break;
        case SKeyword::Number:
        case SKeyword::Key:
            break;
        default:
            break;
        }

        resultStack.push(theKeyword);
    }

    if (resultStack.size() != 1)
        return false;

    return (resultStack.top() == AKey_True);
}

APROCH_NAMESPACE_END
