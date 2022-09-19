/****************************************************************************
 * @file    ADataConverter.h
 * @date    2021-9-23
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

/** @brief 基本数据类型的转换 */
#define APROCH_BASE_DATA_TYPE_CONVERTER(_From_, _To_, _AliasName_)          \
template<>                                                                  \
class aproch::ADataConverter<_From_, _To_>                \
{                                                                           \
public:                                                                     \
    virtual ~ADataConverter() = default;                                    \
    virtual _To_ convertTo(const _From_& val)                               \
    {                                                                       \
        return static_cast<_To_>(val);                                      \
    }                                                                       \
};                                                                          \
using _AliasName_ = aproch::ADataConverter<_From_, _To_>

/** @brief 基本数据类型的转换 */
#define APROCH_BASE_DATA_TO_STRING(_From_, _AliasName_)                     \
template<>                                                                  \
class aproch::ADataConverter<_From_, QString>             \
{                                                                           \
public:                                                                     \
    virtual ~ADataConverter() = default;                                    \
    virtual QString convertTo(const _From_& val)                            \
    {                                                                       \
        return QString::number(val);                                        \
    }                                                                       \
};                                                                          \
using _AliasName_ = aproch::ADataConverter<_From_, QString>

namespace aproch
{
    /**
     * @brief 数据转换器模板
     * @tparam FromType 原始数据
     * @tparam ToType 转换后的数据
     */
    template<class FromType, class ToType>
    class ADataConverter
    {
    public:
        virtual ~ADataConverter() = default;

        /**
         * @brief 将输入数据转换为指定数据
         * @param data 原始数据
         * @return 转换后的数据
         */
        virtual ToType convertTo(const FromType& val)
        {
            return ToType();
        }
    };

    /**
     * @brief 字符串转整数
     */
    template<>
    class ADataConverter<QString, int>
    {
    public:
        virtual ~ADataConverter() = default;
        virtual int convertTo(const QString& val)
        {
            return val.toInt();
        }
    };

    /**
     * @brief 字符串转长整数
     */
    template<>
    class ADataConverter<QString, long>
    {
    public:
        virtual ~ADataConverter() = default;
        virtual long convertTo(const QString& val)
        {
            return val.toLong();
        }
    };

    /**
     * @brief 字符串转长整数
     */
    template<>
    class ADataConverter<QString, long long>
    {
    public:
        virtual ~ADataConverter() = default;
        virtual long long convertTo(const QString& val)
        {
            return val.toLongLong();
        }
    };

    /**
     * @brief 字符串转非负长整数
     */
    template<>
    class ADataConverter<QString, unsigned long long>
    {
    public:
        virtual ~ADataConverter() = default;
        virtual unsigned long long convertTo(const QString& val)
        {
            return val.toULongLong();
        }
    };

    /**
     * @brief 字符串转实数
     * @tparam NumType 数字的数据类型
     */
    template<>
    class ADataConverter<QString, double>
    {
    public:
        virtual ~ADataConverter() = default;
        virtual double convertTo(const QString& val)
        {
            return val.toDouble();
        }
    };

    /** 基本数据转字符串 */

    APROCH_BASE_DATA_TO_STRING(bool, ADC_Bool2String);
    APROCH_BASE_DATA_TO_STRING(char, ADC_Char2String);
    APROCH_BASE_DATA_TO_STRING(unsigned char, ADC_UChar2String);
    APROCH_BASE_DATA_TO_STRING(short, ADC_Short2String);
    APROCH_BASE_DATA_TO_STRING(unsigned short, ADC_UShort2String);
    APROCH_BASE_DATA_TO_STRING(int, ADC_Int2String);
    APROCH_BASE_DATA_TO_STRING(unsigned int, ADC_UInt2String);
    APROCH_BASE_DATA_TO_STRING(float, ADC_Float2String);
    APROCH_BASE_DATA_TO_STRING(double, ADC_Double2String);
    APROCH_BASE_DATA_TO_STRING(long, ADC_Long2String);
    APROCH_BASE_DATA_TO_STRING(long long, ADC_LongLong2String);
    APROCH_BASE_DATA_TO_STRING(unsigned long long, ADC_ULongLong2String);

    /** 基本数据转换 */

    APROCH_BASE_DATA_TYPE_CONVERTER(bool, int, ADC_Bool2Int);
    APROCH_BASE_DATA_TYPE_CONVERTER(bool, float, ADC_Bool2Float);
    APROCH_BASE_DATA_TYPE_CONVERTER(bool, double, ADC_Bool2Double);

    APROCH_BASE_DATA_TYPE_CONVERTER(char, int, ADC_Char2Int);
    APROCH_BASE_DATA_TYPE_CONVERTER(char, short, ADC_Char2Short);
    APROCH_BASE_DATA_TYPE_CONVERTER(char, float, ADC_Char2Float);
    APROCH_BASE_DATA_TYPE_CONVERTER(char, double, ADC_Char2Double);
    APROCH_BASE_DATA_TYPE_CONVERTER(char, long, ADC_Char2Long);
    APROCH_BASE_DATA_TYPE_CONVERTER(char, long long, ADC_Char2LongLong);

    APROCH_BASE_DATA_TYPE_CONVERTER(short, int, ADC_Short2Int);
    APROCH_BASE_DATA_TYPE_CONVERTER(short, float, ADC_Short2Float);
    APROCH_BASE_DATA_TYPE_CONVERTER(short, double, ADC_Short2Double);
    APROCH_BASE_DATA_TYPE_CONVERTER(short, long, ADC_Short2Long);
    APROCH_BASE_DATA_TYPE_CONVERTER(short, long long, ADC_Short2LongLong);

    APROCH_BASE_DATA_TYPE_CONVERTER(int, float, ADC_Int2Float);
    APROCH_BASE_DATA_TYPE_CONVERTER(int, double, ADC_Int2Double);
    APROCH_BASE_DATA_TYPE_CONVERTER(int, long, ADC_Int2Long);
    APROCH_BASE_DATA_TYPE_CONVERTER(int, long long, ADC_Int2LongLong);
    APROCH_BASE_DATA_TYPE_CONVERTER(int, unsigned long long, ADC_Int2ULongLong);

    APROCH_BASE_DATA_TYPE_CONVERTER(unsigned int, int, ADC_UInt2Int);
    APROCH_BASE_DATA_TYPE_CONVERTER(unsigned int, float, ADC_UInt2Float);
    APROCH_BASE_DATA_TYPE_CONVERTER(unsigned int, double, ADC_UInt2Double);
    APROCH_BASE_DATA_TYPE_CONVERTER(unsigned int, long, ADC_UInt2Long);
    APROCH_BASE_DATA_TYPE_CONVERTER(unsigned int, long long, ADC_UInt2LongLong);
    APROCH_BASE_DATA_TYPE_CONVERTER(unsigned int, unsigned long long, ADC_UInt2ULongLong);

    APROCH_BASE_DATA_TYPE_CONVERTER(float, int, ADC_Float2Int);
    APROCH_BASE_DATA_TYPE_CONVERTER(float, double, ADC_Float2Double);
    APROCH_BASE_DATA_TYPE_CONVERTER(float, long, ADC_Float2Long);
    APROCH_BASE_DATA_TYPE_CONVERTER(float, long long, ADC_Float2LongLong);
    APROCH_BASE_DATA_TYPE_CONVERTER(float, unsigned long long, ADC_Float2ULongLong);

    APROCH_BASE_DATA_TYPE_CONVERTER(double, int, ADC_Double2Int);
    APROCH_BASE_DATA_TYPE_CONVERTER(double, float, ADC_Double2Float);
    APROCH_BASE_DATA_TYPE_CONVERTER(double, long, ADC_Double2Long);
    APROCH_BASE_DATA_TYPE_CONVERTER(double, long long, ADC_Double2LongLong);
    APROCH_BASE_DATA_TYPE_CONVERTER(double, unsigned long long, ADC_Double2ULongLong);

    APROCH_BASE_DATA_TYPE_CONVERTER(long, int, ADC_Long2Int);
    APROCH_BASE_DATA_TYPE_CONVERTER(long, unsigned int, ADC_Long2UInt);
    APROCH_BASE_DATA_TYPE_CONVERTER(long, double, ADC_Long2Double);
    APROCH_BASE_DATA_TYPE_CONVERTER(long, long long, ADC_Long2LongLong);
    APROCH_BASE_DATA_TYPE_CONVERTER(long, unsigned long long, ADC_Long2ULongLong);

    APROCH_BASE_DATA_TYPE_CONVERTER(long long, int, ADC_LongLong2Int);
    APROCH_BASE_DATA_TYPE_CONVERTER(long long, unsigned int, ADC_LongLong2UInt);
    APROCH_BASE_DATA_TYPE_CONVERTER(long long, double, ADC_LongLong2Double);
    APROCH_BASE_DATA_TYPE_CONVERTER(long long, unsigned long long, ADC_LongLong2ULongLong);

    APROCH_BASE_DATA_TYPE_CONVERTER(unsigned long long, int, ADC_ULongLong2Int);
    APROCH_BASE_DATA_TYPE_CONVERTER(unsigned long long, unsigned int, ADC_ULongLong2UInt);
    APROCH_BASE_DATA_TYPE_CONVERTER(unsigned long long, double, ADC_ULongLong2Double);
}
