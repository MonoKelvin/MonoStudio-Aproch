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

namespace aproch
{
    class ADataType;
    typedef unsigned int TDataTypeId;
    typedef QMap<TDataTypeId, ADataType> TDataTypeMap;

    /**
     * @brief 数据类型
     */
    class APROCH_API ADataType final
    {
    public:
        ADataType();
        ~ADataType();

        /** @brief 获取类型ID */
        TDataTypeId getId() const noexcept
        {
            return m_typeId;
        }

        /** @brief 获取类型名称 */
        const QString& getName() const noexcept
        {
            return m_name;
        }

        /** @brief 获取类型描述 */
        const QString& getDescription() const noexcept
        {
            return m_description;
        }

        /** @brief 获取类型图标 */
        const QIcon& getIcon() const noexcept
        {
            return m_icon;
        }

        /** @brief 是否数据类型有效 */
        bool isValid() const;

        bool operator==(const ADataType& rhs) const;
        bool operator!=(const ADataType& rhs) const;
        bool operator>(const ADataType& rhs) const;
        bool operator>=(const ADataType& rhs) const;
        bool operator<(const ADataType& rhs) const;
        bool operator<=(const ADataType& rhs) const;

    public:
        /** @brief 添加数据类型，如果存在相同<id>则进行覆盖。<name>要求不为空 */
        static ADataType Add(TDataTypeId id, const QString& name, const QString& description = QString(), const QIcon& icon = QIcon());
        static void Remove(TDataTypeId id);
        static ADataType Get(TDataTypeId id);
        static const TDataTypeMap& GetAllType();

        /** @brief 设置类型名称，要求<newName>不为空，否则返回false */
        static bool SetName(TDataTypeId id, const QString& newName);
        static bool SetDescription(TDataTypeId id, const QString& newDescription);
        static bool SetIcon(TDataTypeId id, const QIcon& newIcon);

    private:
        /** @brief 类型ID */
        TDataTypeId m_typeId;

        /** @brief 类型名称 */
        QString m_name;

        /** @brief 描述 */
        QString m_description;

        /** @brief 数据类型对应的图标 */
        QIcon m_icon;

        /** @brief 类型表 */
        static TDataTypeMap m_dataTypeMap;
    };

#ifndef QT_NO_DEBUG_STREAM
    APROCH_API QDebug operator<<(QDebug, const ADataType&);
#endif

}
