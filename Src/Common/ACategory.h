/****************************************************************************
 * @file    ACategory.h
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
#pragma once

namespace aproch
{
    /**
     * @brief ���ࡣ�������ɲ��������Ƽ��Ϸָ��ַ���ɵ��ַ�����ʶ����ʽ�磺A/B/C������ACategory����ÿ�����඼�ǵ�����¼�ģ���Ϊ�ּ�<Level>
     * �ҷָ��ַ�<Splitter>���Զ��������ַ����綨���'|'����Ӧ�ķ�������Ϊ��A|B|C
     */
    class APROCH_API ACategory
    {
    public:
        /** @brief ��ַ�����Ϊѡ�� */
        enum ESplitBehavior
        {
            /** @brief û����Ϊ */
            NoBehavior = 0x000,

            /** @brief �����յķּ� */
            SkipEmptyLevel = 0x001,

            /** @brief �ָ��ַ������ִ�Сд */
            CaseInsensitive = 0x002,

            /** @brief ���ּ���ǰ��ո�ȥ�� */
            TrimmedLevel = 0x004,
        };

        ACategory();
        explicit ACategory(const QString& category, ESplitBehavior behavior = SkipEmptyLevel);
        explicit ACategory(const QStringList& stringList, ESplitBehavior behavior = SkipEmptyLevel);

        /** @brief ���ַ��������������� */
        void fromString(const QString& category, ESplitBehavior behavior = SkipEmptyLevel);

        /** @brief תΪ�ַ���ʽ���磺A/B/C. */
        QString toString() const;

        /** @brief ��ȡ���еķּ�����˳�� */
        const QStringList& getLevels() const;

        /** @brief ��ȡָ��λ�÷ּ������� */
        QString getLevel(int level) const;

        /** @brief ���÷ּ�λ�õ����� */
        void setLevel(int level, const QString& str);

        /** @brief ���׷�ӷּ� */
        ACategory& pushLevel(const QString& level, ESplitBehavior behavior = SkipEmptyLevel);

        /** @brief ��ָ��λ����ӷּ� */
        ACategory& insertLevel(int position, const QString& level, ESplitBehavior behavior = SkipEmptyLevel);

        /** @brief �Ƴ�ָ��λ�õķּ� */
        ACategory& removeLevel(int position);

        /** @brief �Ƴ����λ�õķּ� */
        ACategory& popLevel();

        /** @brief ���÷ָ��ַ� */
        void setSplitter(const QString& splitter);

        /** @brief ��ȡ�ָ��ַ� */
        QString getSplitter() const;

        /** @brief �����Ƿ�Ϊ�� */
        bool isVoid() const;

        /** @brief �������Ƿ��зּ� */
        bool hasLevel(const QString& level, Qt::CaseSensitivity cs = Qt::CaseInsensitive) const;

        /** @brief ��һ������ */
        ACategory previous() const;

        /** @brief תΪ�ַ�������ͬ�� toString */
        operator QString() const;

        /** @brief ����һ�������Ƿ���� */
        bool operator==(const ACategory& rhs) const;

        /** @brief ����һ�������Ƿ񲻵� */
        bool operator!=(const ACategory& rhs) const;

        /** @brief ���׷�ӷּ�����ͬ�� pushLevel(level, ESplitBehavior::SkipEmptyLevel) */
        ACategory& operator<<(const QString& level);

    protected:
        /** @brief ���������� */
        friend APROCH_API QDebug operator<<(QDebug ds, const ACategory& category);

    private:
        /** @brief ���� */
        QStringList m_category;

        /** @brief �ָ��ַ� */
        QString m_splitter;
    };
}