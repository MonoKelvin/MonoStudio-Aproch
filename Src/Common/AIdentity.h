/****************************************************************************
 * @file    AIdentity.h
 * @date    2021-8-12
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
#include <QUuid>

namespace aproch
{
    /**
     * @brief 唯一标识模板类
     */
    template <class Type, class Generator>
    class AIdentity
    {
    public:
        using value_type = Type;

        /**
         * @brief 构造一个唯一标识
         */
        AIdentity()
            : mId(Generator::get())
        {
        }

        /**
         * @brief 拷贝构造一个唯一标识
         * @param rhs 另一个标识。<this>的标识将和<rhs>标识一样
         */
        AIdentity(const value_type &rhs)
            : mId(rhs)
        {
        }

        /**
         * @brief 获取ID数值
         * @return ID数值
         */
        inline const value_type &value(void) noexcept
        {
            return mId;
        }

        /**
         * @brief 获取ID数值
         * @return ID数值
         */
        inline value_type value(void) const noexcept
        {
            return mId;
        }

        /**
         * @brief 是否和另一个标识相等
         * @param rhs 另一个标识
         * @return 是否相等
         */
        inline bool operator==(const AIdentity &rhs) const
        {
            return mId == rhs.mId;
        }

        /**
         * @brief 是否和另一个标识相等
         * @param rhs 另一个标识
         * @return 是否相等
         */
        inline bool operator!=(const AIdentity &rhs) const
        {
            return mId != rhs.mId;
        }

        /**
         * @brief 是否和另一个标识相等
         * @param rhs 另一个标识
         * @return 是否相等
         */
        inline bool operator==(const value_type &rhs) const
        {
            return mId == rhs;
        }

        /**
         * @brief 是否和另一个标识不相等
         * @param rhs 另一个标识
         * @return 是否不相等
         */
        inline bool operator!=(const value_type &rhs) const
        {
            return mId != rhs;
        }

        /**
         * 将ID转换为字符串
         * @return 字符串信息
         */
        inline QString toString() const
        {
            return Generator::toString(mId);
        }

        /**
         * @brief 从字符串获取ID
         * @param val id列表
         */
        inline void fromString(const QString &val)
        {
            mId = Generator::fromString(val);
        }

        /**
         * @brief 转换为字符串操作符
         */
        inline operator QString() const
        {
            return toString();
        }

        /**
         * @brief 转换为装载的数据类型操作符
         */
        inline operator value_type() const
        {
            return mId;
        }

        /**
         * @brief 是否是空的ID
         * @return 是否是空的ID
         */
        inline bool isNull() const
        {
            return operator==(Null());
        }

        /**
         * @brief 获取一个唯一标识
         * @return 唯一标识
         */
        inline static value_type Identity()
        {
            return Generator::get();
        }

        /**
         * @brief 获取一个空的ID
         * @return 空ID
         */
        inline static value_type Null()
        {
            return Generator::null();
        }

    private:
        value_type mId;
    };

    /**
     * @brief 唯一整数生成器
     */
    class APROCH_API AIntegralNumberGenerator
    {
    public:
        using value_type = long long;

        /**
         * @brief 获取新的唯一整数，其值是上一次通过该方法得到的整数+1
         * @note 该方法使用原子操作，所以是线程安全的。
         * @return 唯一整数
         */
        inline static const value_type get()
        {
            return _InterlockedIncrement64(&sCurrentNumber);
        }

        /**
         * @brief 转换为字符串
         * @param val 整数
         * @return 字符串
         */
        inline static const QString toString(const value_type &val)
        {
            return QString::number(val);
        }

        /**
         * @brief 从字符串获取ID。
         * @note 同时更新全局ID为最大的那个ID数；该方法是线程安全的
         * @param val 字符串
         * @return ID
         */
        static value_type fromString(const QString &val);

        /**
         * @brief 设置当前值，此后每一次的数字在该数字的基础上递增
         * @param currentNum 当前数字，建议大于等于0
         */
        inline static void setCurrent(const value_type &currentNum)
        {
            sCurrentNumber = currentNum;
        }

        /**
         * @brief 获取一个空（无效）的ID
         * @return 空的（无效）ID
         */
        inline static value_type null()
        {
            return 0;
        }

    private:
        static volatile value_type sCurrentNumber;
    };

    /**
     * @brief UUID生成器
     */
    class APROCH_API AUuidGenerator
    {
    public:
        using value_type = QUuid;

        /**
         * @brief 获取新的UUID
         * @return 唯一整数
         */
        inline static value_type get()
        {
            return QUuid::createUuid();
        }

        /**
         * @brief 转换为字符串
         * @param val UUID
         * @return 字符串
         */
        inline static const QString toString(const value_type &val)
        {
            return val.toString();
        }

        /**
         * @brief 从字符串获取UUID。
         * @param val 字符串
         * @return ID
         */
        inline static value_type fromString(const QString &val)
        {
            return QUuid::fromString(val);
        }

        /**
         * @brief 获取一个空（无效）的UUID
         * @return 空的（无效）UUID
         */
        inline static value_type null()
        {
            return QUuid();
        }
    };

    typedef AIdentity<AIntegralNumberGenerator::value_type, AIntegralNumberGenerator> ANumID;
    typedef AIdentity<AUuidGenerator::value_type, AUuidGenerator> AUuid;

#ifdef A_USE_UUID_AS_IDENTITY
    /**
     * @brief 封装QUuid的唯一标识
     */
    typedef AUuid AID;
#else
    /**
     * @brief 唯一整数，每次使用的新整数都是上一次的整数+1
     */
    typedef ANumID AID;
#endif
}
