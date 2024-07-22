/****************************************************************************
 * @file    AFontDatabase.cpp
 * @date    2024-07-22 
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
#include "AFontDatabase.h"

APROCH_NAMESPACE_BEGIN

static QMap<int, unsigned int> sFontId2TypeMap = QMap<int, unsigned int>();
static QMap<unsigned int, int> sType2FontIdMap = QMap<unsigned int, int>();
static QMap<unsigned int, QPair<QString, QString>> sType2FamilyMap = {
    QPair<unsigned int, QPair<QString, QString>>(AFontDatabase::SegoeFluentIcons, 
         QPair<QString, QString>(AStr("Segoe Fluent Icons"), AStr(":/fonts/segoeicons.ttf"))),
    QPair<unsigned int, QPair<QString, QString>>(AFontDatabase::SegoeMDL2Assets, 
         QPair<QString, QString>(AStr("Segoe MDL2 Assets"), AStr(":/fonts/segmdl2.ttf"))),
    QPair<unsigned int, QPair<QString, QString>>(AFontDatabase::SegoeUIEmoji, 
         QPair<QString, QString>(AStr("Segoe UI Emoji"), AStr(":/fonts/seguiemj.ttf"))),
};
static unsigned int sDefaultIconFontType = AFontDatabase::SegoeFluentIcons;

AFontDatabase::AFontDatabase()
{
}

AFontDatabase::~AFontDatabase()
{
}

int AFontDatabase::addFont(const QString& fontFilePath, unsigned int type)
{
    if (sType2FontIdMap.contains(type))
    {
        qWarning() << "a font of type [" << type << "] already exists [id=" << sType2FontIdMap.value(type) << "]";
        return -1;
    }

    int fontId = QFontDatabase::addApplicationFont(fontFilePath);
    if (fontId == -1)
    {
        qWarning() << "add font \"" << fontFilePath << "\" failed";
        return fontId;
    }

    // has font type
    if (sFontId2TypeMap.contains(fontId))
    {
        if (type != sFontId2TypeMap.value(fontId))
            qWarning() << "the same font is set to different types";
    }

    if (getFontById(fontId).family().isEmpty())
    {
        QFontDatabase::removeApplicationFont(fontId);
        return -1;
    }

    sFontId2TypeMap[fontId] = type;
    sType2FontIdMap[type] = fontId;

    return fontId;
}

bool AFontDatabase::removeFont(int fontId)
{
    if (!QFontDatabase::removeApplicationFont(fontId))
        return false;

    auto type = sFontId2TypeMap.value(fontId);
    sType2FontIdMap.remove(type);
    sFontId2TypeMap.remove(fontId);
    return true;
}

QFont AFontDatabase::getFont(unsigned int type)
{
    switch (type)
    {
    case Default:
        return getDefaultFont();
    case SegoeFluentIcons:
    case SegoeMDL2Assets:
    case SegoeUIEmoji:
    {
        // 如果系统中没有，则首次加载
        bool added = false;
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(QFontDatabase::Any);
        for (const QString& fontFamily : fontFamilies)
        {
            auto font2file = sType2FamilyMap.value(type);
            if (font2file.first.toUpper() != fontFamily.toUpper())
                continue;

            added = true;
            break;
        }

        // 系统中有，但是不是通过addFont添加的
        if (!added || !sType2FontIdMap.contains(type))
        {
            auto font2file = sType2FamilyMap.value(type);
            if (addFont(font2file.second, type) == -1)
                return QFont();
        }
    }
        break;
    default:
        break;
    }

    return getFontById(sType2FontIdMap.value(type, -1));
}

QFont AFontDatabase::getFontById(int fontId)
{
    const QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    if (fontFamilies.empty())
        return QFont();

    return fontFamilies.at(0);
}

QFont AFontDatabase::getDefaultFont()
{
    return QFontDatabase::systemFont(QFontDatabase::GeneralFont);
}

QFont AFontDatabase::getDefaultIconFont()
{
    return getFont(sDefaultIconFontType);
}

bool AFontDatabase::setDefaultIconFont(unsigned int type)
{
    QFont font = getFont(type);
    if (font.family().isEmpty())
        return false;

    sDefaultIconFontType = type;
    return true;
}

APROCH_NAMESPACE_END
