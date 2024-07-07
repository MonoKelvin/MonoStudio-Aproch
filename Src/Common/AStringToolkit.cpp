/****************************************************************************
 * @file    AStringToolkit.cpp
 * @date    2021-10-08
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
#include "AStringToolkit.h"

#include <QCryptographicHash>

APROCH_NAMESPACE_BEGIN

QString AStringToolkit::WrapSymbolsWithChars(const QString &source, const QStringList &substrs, const char *addition)
{
    QString result = source;
    const size_t addedLen = std::strlen(addition);

    for (const auto &subStr : substrs)
    {
        if (subStr.isEmpty() || subStr == addition)
            continue;

        int from = 0, to = 0;
        while (from != -1)
        {
            from = result.indexOf(subStr, from);
            if (from < 0)
                break;
            result.insert(from, addition);

            to = from + subStr.length() + addedLen;
            result.insert(to, addition);

            from = to + addedLen;
        }
    }

    return result;
}

QString AStringToolkit::NormalizeCategory(const QString &category, bool withName)
{
    QString cat = category;

    // 替换所有连续的"\\"或者"/"符号为"/"
    cat.replace(QRegularExpression("((/|\\)[(.)\1]*(/|\\)*)"), "/");

    if (cat.front() != '/')
        cat.push_front('/');

    if (!withName && cat.back() != '/')
        cat.push_back('/');

    return cat;
}

QList<QString> AStringToolkit::GetCategoryLevels(const QString &category)
{
    QList<QString> result;

    qsizetype i = category.indexOf("/");
    while (i >= 0 && i < category.size())
    {
        qsizetype j = category.indexOf("/", i + 1);
        if (j == -1)
            break;

        i = j;
        result.append(category.mid(i + 1, j - i - 1));
    }

    return result;
}

QString AStringToolkit::ReplacToSlash(const QString &source)
{
    QString result = source;
    result.replace(QRegularExpression("((/|\\)[(.)\1]*(/|\\)*)"), "/");
    return result;
}

QString AStringToolkit::Color2HexStr(const QColor &color, bool isAlpha)
{
    QString hexStr("#");
    if (isAlpha)
        hexStr += AStr("%1").arg(color.alpha(), 2, 16, QChar('0'));

    const QString redStr = AStr("%1").arg(color.red(), 2, 16, QChar('0'));
    const QString greenStr = AStr("%1").arg(color.green(), 2, 16, QChar('0'));
    const QString blueStr = AStr("%1").arg(color.blue(), 2, 16, QChar('0'));
    hexStr += redStr + greenStr + blueStr;

    return hexStr.toUpper();
}

QString AStringToolkit::Color2RgbStr(const QColor &color, bool isAlpha)
{
    QString hexStr;
    if (isAlpha)
        hexStr = "A";
    hexStr += AKey_RGB + AKey_LeftBracket;

    const QString commaSpace(", ");
    if (isAlpha)
        hexStr += QString::number(color.alpha()) + commaSpace;

    hexStr += QString::number(color.red());
    hexStr += commaSpace + QString::number(color.green());
    hexStr += commaSpace + QString::number(color.blue());
    hexStr += AKey_RightBracket;

    return hexStr;
}

QString AStringToolkit::TrimLeft(const QString &str)
{
    const QChar *begin = str.cbegin();
    const QChar *end = str.cend();
    while (begin < end && begin->isSpace())
        begin++;
    if (begin == str.cbegin() && end == str.cend())
        return str;

    if (str.isDetached())
    {
        QChar *data = const_cast<QChar *>(str.cbegin());
        if (begin != data)
            memmove(data, begin, (end - begin) * sizeof(QChar));
        const_cast<QString &>(str).resize(end - begin);
        return std::move(const_cast<QString &>(str));
    }

    return QString(begin, end - begin);
}

QString AStringToolkit::TrimRight(const QString &str)
{
    const QChar *begin = str.cbegin();
    const QChar *end = str.cend();
    while (begin < end && end[-1].isSpace())
        --end;
    if (begin == str.cbegin() && end == str.cend())
        return str;

    if (str.isDetached())
    {
        QChar *data = const_cast<QChar *>(str.cbegin());
        if (begin != data)
            memmove(data, begin, (end - begin) * sizeof(QChar));
        const_cast<QString &>(str).resize(end - begin);
        return std::move(const_cast<QString &>(str));
    }

    return QString(begin, end - begin);
}

bool AStringToolkit::TrimCompare(const QString &str1, const QString &str2, Qt::CaseSensitivity cs)
{
    QString trimmedStr1 = str1.trimmed();
    QString trimmedStr2 = str2.trimmed();

    return trimmedStr1.compare(trimmedStr2, cs) == 0;
}

QString AStringToolkit::ReplaceContinuousString(const QString &str, const QString &rpStr)
{
    QString result = str;
    result.replace(QRegularExpression("(.)(\1)+"), rpStr);
    return result;
}

QString AStringToolkit::ReplaceContinuousString(const QString &str, const QString &target, const QString &rpStr)
{
    // 将元字符添加'\'符号
    QString regTar = target;
    QList<QChar> metaChars;
    metaChars << '.' << '*' << '+' << '(' << ')' << '$' << '/' << '\\' << '?' << '['
              << ']' << '^' << '{' << '}' << '-' << '!' << '<' << '>';
    for (int i = 0; i < regTar.size();)
    {
        if (metaChars.contains(regTar.at(i)))
        {
            regTar.insert(i, '\\');
            i += 2;
        }
        else
        {
            ++i;
        }
    }

    QString result = str;
    result.replace(QRegularExpression("(" + regTar + ")(\1)+"), rpStr);
    return result;
}

QVariant AStringToolkit::FindValue(const QVariantMap &varMap, const QString &key,
                                   Qt::CaseSensitivity sensitivity, const QVariant &defaultVal)
{
    return FindValue<QVariant>(varMap, key, sensitivity, defaultVal);
}

const QString AStringToolkit::EncryptByMD5(const QString &source, bool isHalf)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(source.toLatin1());
    const QString result = QString(hash.result().toHex());
    return isHalf ? result.mid(8, 16) : result;
}

APROCH_NAMESPACE_END
