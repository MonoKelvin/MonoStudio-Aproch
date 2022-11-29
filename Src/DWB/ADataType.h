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
     * @brief ��������
     */
    class APROCH_API ADataType final
    {
    public:
        ADataType();
        ~ADataType();

        /** @brief ��ȡ����ID */
        TDataTypeId getId() const noexcept
        {
            return m_typeId;
        }

        /** @brief ��ȡ�������� */
        const QString& getName() const noexcept
        {
            return m_name;
        }

        /** @brief ��ȡ�������� */
        const QString& getDescription() const noexcept
        {
            return m_description;
        }

        /** @brief ��ȡ����ͼ�� */
        const QIcon& getIcon() const noexcept
        {
            return m_icon;
        }

        /** @brief �Ƿ�����������Ч */
        bool isValid() const;

        bool operator==(const ADataType& rhs) const;
        bool operator!=(const ADataType& rhs) const;
        bool operator>(const ADataType& rhs) const;
        bool operator>=(const ADataType& rhs) const;
        bool operator<(const ADataType& rhs) const;
        bool operator<=(const ADataType& rhs) const;

    public:
        /** @brief ����������ͣ����������ͬ<id>����и��ǡ�<name>Ҫ��Ϊ�� */
        static ADataType Add(TDataTypeId id, const QString& name, const QString& description = QString(), const QIcon& icon = QIcon());
        static void Remove(TDataTypeId id);
        static ADataType Get(TDataTypeId id);
        static const TDataTypeMap& GetAllType();

        /** @brief �����������ƣ�Ҫ��<newName>��Ϊ�գ����򷵻�false */
        static bool SetName(TDataTypeId id, const QString& newName);
        static bool SetDescription(TDataTypeId id, const QString& newDescription);
        static bool SetIcon(TDataTypeId id, const QIcon& newIcon);

    private:
        /** @brief ����ID */
        TDataTypeId m_typeId;

        /** @brief �������� */
        QString m_name;

        /** @brief ���� */
        QString m_description;

        /** @brief �������Ͷ�Ӧ��ͼ�� */
        QIcon m_icon;

        /** @brief ���ͱ� */
        static TDataTypeMap m_dataTypeMap;
    };

#ifndef QT_NO_DEBUG_STREAM
    APROCH_API QDebug operator<<(QDebug, const ADataType&);
#endif

}
