/****************************************************************************
 * @file    ADataType.h
 * @date    2022-11-27
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

using ADataType = QMetaType;

#if 0
class ADataTypePrivate;
class ADataType;

/**
 * @brief 数据类型
 */
class APROCH_API ADataType final
{
public:
    ADataType();
    explicit ADataType(int type);
    explicit ADataType(const QString& name);
    ADataType(const ADataType& rhs) noexcept;
    ~ADataType();

    using Type = QVariant::Type;

    int getType() const;
    QMetaType::Type getMetaType() const;
    QString getName() const;
    QString getDescription() const;
    bool isValid() const;
    
    ADataType& operator=(const ADataType& rhs);
    ADataType& operator=(ADataType&& rhs) noexcept;
    bool operator==(const ADataType& rhs) const;
    bool operator!=(const ADataType& rhs) const;
    bool operator>(const ADataType& rhs) const;
    bool operator>=(const ADataType& rhs) const;
    bool operator<(const ADataType& rhs) const;
    bool operator<=(const ADataType& rhs) const;

public:
    /** the internal category named 'Aproch.DataType.Default' */
    static const ACategory DefaultCategory;

public:
    /** 注册类型。已存在的数据类型不会被覆盖，将注册失败，并返回该存在的类型 */
    inline static ADataType Register(const QString& name, const ACategory& category = ADataType::DefaultCategory)
    {
        Register(name, QString(), category);
    }
    static ADataType Register(const QString& name, const QString& description,
                              const ACategory& category = ADataType::DefaultCategory);

    static void Remove(int type);
    static void Remove(const QString& name);
    static ADataType Get(int type);
    static ADataType Get(const QString& name);
    static ADataType Get(const QVariant& value);
    static QList<ADataType> AllTypes();

    static bool SetDescription(int type, const QString& newDescription);
    static bool SetDescription(const QString& name, const QString& newDescription);

private:
    ADataTypePrivate* d_ptr;
};

#ifndef QT_NO_DEBUG_STREAM
APROCH_API QDebug operator<<(QDebug, const ADataType&);
#endif

#if !defined(QT_NO_DATASTREAM) || (defined(QT_BOOTSTRAPPED) && !defined(QT_BUILD_QMAKE))
APROCH_API QDataStream &operator<<(QDataStream &, const ADataType&);
APROCH_API QDataStream &operator>>(QDataStream &, ADataType&);
#endif

#endif

APROCH_NAMESPACE_END