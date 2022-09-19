/****************************************************************************
 * @file    AIconFont.cpp
 * @date    2021-10-27
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
#include "AIconFont.h"

#include <QFontDatabase>

namespace aproch
{
    QList<AIconFont> AIconFont::sFontList = QList<AIconFont>();

    AIconFont::AIconFont(const QString& iconFontPath, const QString& iconNameFile)
    {
        mID = QFontDatabase::addApplicationFont(iconFontPath);
        const QStringList fontFamilies = QFontDatabase::applicationFontFamilies(mID);
        if (!fontFamilies.empty())
        {
            mFontFamily = fontFamilies.at(0);
            parseIconFont(iconNameFile, mCharMap);
        }
    }

    AIconFont::~AIconFont()
    {
    }

    TIcon AIconFont::getIcon(const TIconName& iconName)
    {
        return mCharMap.value(iconName);
    }

    TIconName AIconFont::getIconName(const TIcon& iconChar)
    {
        for (auto kv = mCharMap.constKeyValueBegin(); kv != mCharMap.constKeyValueEnd(); ++kv)
        {
            if (kv.base().key() == iconChar)
                return kv.base().value();
        }

        return TIconName();
    }

    QString AIconFont::getFontFamily() const
    {
        return mFontFamily;
    }

    int AIconFont::AddIconFont(const AIconFont& iconFont)
    {
        const int index = sFontList.indexOf(iconFont);
        if (index >= 0)
        {
            sFontList.replace(index, iconFont);
            return true;
        }

        sFontList.push_back(iconFont);
        return false;
    }

    void AIconFont::RemoveIconFont(const QString& iconFontName)
    {
        for (auto iter = sFontList.begin(); iter != sFontList.end(); )
        {
            if (iter->mFontFamily.toUpper() == iconFontName.trimmed().toUpper())
                iter = sFontList.erase(iter);
            else
                ++iter;
        }
    }

    TIcon AIconFont::Icon(const TIconName& iconName, const QString& iconFontName)
    {
        const QString normalIFN = iconFontName.trimmed().toUpper();
        for (const auto& iFont : sFontList)
        {
            if (iFont.mFontFamily.toUpper() == normalIFN)
            {
                return iFont.mCharMap.value(iconName);
            }
        }

        return TIcon();
    }

    QFont AIconFont::IconFont(const QString& iconFontName)
    {
        const QString normalIFN = iconFontName.trimmed().toUpper();
        for (const auto& iFont : sFontList)
        {
            if (iFont.mFontFamily.toUpper() == normalIFN)
            {
                return QFont(iFont.mFontFamily);
            }
        }

        return QFont();
    }

    QFont AIconFont::IconFont(int fontId)
    {
        const QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        if (!fontFamilies.empty())
        {
            return QFont(fontFamilies.at(0));
        }

        return QFont();
    }

    bool AIconFont::parseIconFont(const QString& charNameFile, QMap<TIconName, TIcon>& charMap) const
    {
        return false;
    }
}
