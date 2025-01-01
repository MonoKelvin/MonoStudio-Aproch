/****************************************************************************
 * @file    AprochGlobal.cpp
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
#include "AprochGlobal.h"

const QVariant ANull_Variant = QVariant();
const QString ANull_String = QString();
const QString AKey_True = AStr("true");
const QString AKey_False = AStr("false");
const QString AKey_0 = AStr("0");
const QString AKey_1 = AStr("1");
const QString AKey_LeftBracket = AStr("(");
const QString AKey_RightBracket = AStr(")");
const QString AKey_Id = AStr("id");
const QString AKey_Default = AStr("default");
const QString AKey_Value = AStr("value");
const QString AKey_Type = AStr("type");
const QString AKey_Name = AStr("name");
const QString AKey_Slash = AStr("/");
const QString AKey_Backslash = AStr("\\");
const QString AKey_Comma = AStr(",");
const QString AKey_NewLine = AStr("\n");
const QString AKey_Table = AStr("\t");
const QString AKey_RGB = AStr("RGB");
const QString AKey_ARGB = AStr("ARBG");

const QString AKey_Const = AStr("const");
const QString AKey_Ptr = AStr("pointer");
const QString AKey_Ref = AStr("reference");
const QString AKey_Zero = AStr("0");
const QString AKey_One = AStr("1");
const QString AKey_NAN = AStr("nan");
const QString AKey_INF = AStr("inf");

const QString AKey_Bool = AStr("bool");
const QString AKey_Boolean = AStr("boolean");
const QString AKey_String = AStr("string");
const QString AKey_UChar = AStr("uchar");
const QString AKey_Char = AStr("char");
const QString AKey_UShort = AStr("ushort");
const QString AKey_Short = AStr("short");
const QString AKey_Int = AStr("int");
const QString AKey_UInt = AStr("uint");
const QString AKey_Float = AStr("float");
const QString AKey_Double = AStr("double");
const QString AKey_ULong = AStr("ulong");
const QString AKey_Long = AStr("long");
const QString AKey_LongLong = AStr("longlong");
const QString AKey_ULongLong = AStr("ulonglong");
const QString AKey_BigNumber = AStr("bignumber");
const QString AKey_Real = AStr("real");
const QString AKey_StdString = AStr("std::string");
const QString AKey_Byte = AStr("byte");

const QStringList ASL_BaseDataType = {
    AKey_Bool,
    AKey_Boolean,
    AKey_String,
    AKey_UChar,
    AKey_Char,
    AKey_UShort,
    AKey_Short,
    AKey_Int,
    AKey_UInt,
    AKey_Float,
    AKey_Double,
    AKey_Long,
    AKey_LongLong,
    AKey_ULongLong,
};

const QString AKey_Vector = AStr("vector");
const QString AKey_List = AStr("list");
const QString AKey_Map = AStr("map");
const QString AKey_UnorderedMap = AStr("unorderedmap");
const QString AKey_Hash = AStr("hash");
const QString AKey_Array = AStr("array");
const QString AKey_Set = AStr("set");

const char* APropName_BtnType = "buttonType";
const char* APropValue_TextBtn = "textButton";
const char* APropName_WidgetType = "widgetType";
const char* APropValue_Panel = "panel";
const char* APropValue_Single = "single";

QSize qBound(QSize minVal, QSize val, QSize maxVal)
{
    return qBoundSize(minVal, val, maxVal);
}

QSizeF qBound(QSizeF minVal, QSizeF val, QSizeF maxVal)
{
    return qBoundSize(minVal, val, maxVal);
}

QPoint qBound(QPoint minVal, QPoint val, QPoint maxVal)
{
    return qBoundV2D(minVal, val, maxVal);
}

QPointF qBound(QPointF minVal, QPointF val, QPointF maxVal)
{
    return qBoundV2D(minVal, val, maxVal);
}

APROCH_NAMESPACE_BEGIN

QVariant GetDefaultValueByType(const QString& typeName)
{
    if (0 == typeName.compare(AKey_Int, Qt::CaseInsensitive) ||
        0 == typeName.compare(AKey_Long, Qt::CaseInsensitive) ||
        0 == typeName.compare(AKey_Char, Qt::CaseInsensitive) ||
        0 == typeName.compare(AKey_UChar, Qt::CaseInsensitive) ||
        0 == typeName.compare(AKey_Short, Qt::CaseInsensitive) ||
        0 == typeName.compare(AKey_UShort, Qt::CaseInsensitive))
        return QVariant(int(0));
    if (0 == typeName.compare(AKey_String, Qt::CaseInsensitive) ||
        0 == typeName.compare(AKey_StdString, Qt::CaseInsensitive))
        return QVariant(ANull_String);
    if (0 == typeName.compare(AKey_Float, Qt::CaseInsensitive))
        return QVariant(0.f);
    if (0 == typeName.compare(AKey_Double, Qt::CaseInsensitive) ||
        0 == typeName.compare(AKey_Real, Qt::CaseInsensitive))
        return QVariant(0.0);
    if (0 == typeName.compare(AKey_Bool, Qt::CaseInsensitive) ||
        0 == typeName.compare(AKey_Boolean, Qt::CaseInsensitive))
        return QVariant(false);
    if (0 == typeName.compare(AKey_UInt, Qt::CaseInsensitive) ||
        0 == typeName.compare(AKey_ULong, Qt::CaseInsensitive))
        return QVariant(0u);
    if (0 == typeName.compare(AKey_LongLong, Qt::CaseInsensitive))
        return QVariant(0ll);
    if (0 == typeName.compare(AKey_ULongLong, Qt::CaseInsensitive))
        return QVariant(0ull);

    const QVariant::Type type = QVariant::nameToType(typeName.toLatin1());
    return QVariant(type);
}

APROCH_NAMESPACE_END