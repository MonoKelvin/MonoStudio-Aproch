/****************************************************************************
 * @file    ACategory.cpp
 * @date    2022-08-13
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
#include "ACategory.h"

APROCH_NAMESPACE_BEGIN

static const QChar DefualtSplitterChar = '.';

ACategory::ACategory()
    : m_splitter(AKey_Slash)
{
}

ACategory::ACategory(const QString& category, ESplitBehavior behavior)
    : m_splitter(AKey_Slash)
{
    fromString(category, behavior);
}

ACategory::ACategory(const QStringList& stringList, ESplitBehavior behavior)
    : m_splitter(AKey_Slash)
{
    fromString(stringList.join(m_splitter), behavior);
}

void ACategory::fromString(const QString& category, ESplitBehavior behavior)
{
    Qt::SplitBehavior qsb = (behavior & SkipEmptyLevel) ? Qt::SkipEmptyParts : Qt::KeepEmptyParts;
    Qt::CaseSensitivity cs = (behavior & CaseInsensitive) ? Qt::CaseInsensitive : Qt::CaseSensitive;

    QString theCat = category;
    const int spltSize = m_splitter.size();
    if (theCat.startsWith(m_splitter))
        theCat.remove(0, spltSize);
    if (theCat.endsWith(m_splitter))
        theCat.remove(theCat.size() - spltSize, spltSize);

    m_category = category.split(m_splitter, qsb, cs);

    if (behavior & TrimmedLevel)
    {
        for (QString& level : m_category)
            level = level.trimmed();
    }
}

void ACategory::fromStringList(const QStringList& categories, ESplitBehavior behavior)
{
    fromString(categories.join(getSplitter()), behavior);
}

QString ACategory::toString() const
{
    return m_category.join(getSplitter());
}

const QStringList& ACategory::getLevels() const
{
    return m_category;
}

QString ACategory::getLevel(int level) const
{
    if (level < 0 || level >= m_category.size())
        return ANull_String;

    return m_category[level];
}

void ACategory::setLevel(int level, const QString& str)
{
    if (level < 0 || level >= m_category.size())
        return;

    m_category[level] = str;
}

ACategory& ACategory::pushLevel(const QString& level, ESplitBehavior behavior)
{
    return insertLevel(m_category.size(), level, behavior);
}

ACategory& ACategory::insertLevel(int position, const QString& level, ESplitBehavior behavior)
{
    if (position < 0 || position > m_category.size())
        return *this;

    if ((behavior & SkipEmptyLevel) && level.isEmpty())
        return *this;

    const QString trimmedLevel = level.trimmed();
    if (behavior & TrimmedLevel)
    {
        if ((behavior & SkipEmptyLevel) && trimmedLevel.isEmpty())
            return *this;
        m_category.insert(position, trimmedLevel);
    }
    else
    {
        m_category.insert(position, level);
    }

    return *this;
}

ACategory& ACategory::removeLevel(int position)
{
    if (position < 0 || position > m_category.size())
        return *this;
    m_category.removeAt(position);
    return *this;
}

ACategory& ACategory::popLevel()
{
    m_category.pop_back();
    return *this;
}

void ACategory::setSplitter(const QString& splitter)
{
    const QString newSplitter = splitter.trimmed();
    if (newSplitter.isEmpty())
        return;

    m_splitter = newSplitter;
}

QString ACategory::getSplitter() const
{
    if (m_splitter.trimmed().isEmpty())
        return DefualtSplitterChar;
    return m_splitter;
}

bool ACategory::isValid() const
{
    return !m_category.empty();
}

bool ACategory::hasLevel(const QString& level, Qt::CaseSensitivity cs) const
{
    for (const QString& catLevel : m_category)
    {
        if (catLevel.compare(level, cs) == 0)
            return true;
    }
    return false;
}

ACategory ACategory::previous() const
{
    ACategory preCategory;
    preCategory.m_splitter = m_splitter;
    preCategory.m_category = m_category;
    preCategory.m_category.pop_back();
    return preCategory;
}

ACategory::operator QString() const
{
    return toString();
}

ACategory& ACategory::operator=(const QString& str)
{
    this->fromString(str);
    return *this;
}

ACategory& ACategory::operator=(const char* str)
{
    this->fromString(QString::fromLocal8Bit(str));
    return *this;
}

bool ACategory::operator==(const ACategory& rhs) const
{
    return (m_splitter == rhs.m_splitter) && (m_category == rhs.m_category);
}

bool ACategory::operator!=(const ACategory& rhs) const
{
    return !(this->operator==(rhs));
}

ACategory& ACategory::operator<<(const QString& level)
{
    return pushLevel(level);
}

QDataStream& operator<<(QDataStream& ds, const ACategory& cat)
{
    ds << cat.m_category;
    ds << cat.m_splitter;
    return ds;
}

QDataStream& operator>>(QDataStream& ds, ACategory& cat)
{
    ds >> cat.m_category;
    ds >> cat.m_splitter;
    return ds;
}

QDebug operator<<(QDebug dbg, const ACategory& category)
{
    if (category.isValid())
        dbg << "ACategory(" << category.toString() << ")";
    else
        dbg << "ACategory(Invalid)";
    return dbg;
}

APROCH_NAMESPACE_END
