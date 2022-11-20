/****************************************************************************
 * @file    AAbstractDataManager.h
 * @date    2022-7-16
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
#include <QSet>

namespace aproch
{
    /** @brief ���ݰ󶨷��� */
    enum EDataBindingMethod
    {
        /** @brief �����޸�ʱ��֪ͨ�󶨸����ݵĿؼ����ؼ������޸�ʱ��֪ͨ�����޸� */
        DBM_DataToWidget,

        /** @brief �ؼ������޸�ʱ��֪ͨ�󶨵����ݣ������޸�ʱ��֪ͨ�ؼ� */
        DBM_WidgetToData,

        /** @brief ���ݻ�ؼ������޸�ʱ����֪ͨ�ؼ������� */
        DBM_TwoWay,
    };

    /**
     * @brief �������ݹ�������
     */
    class APROCH_API AAbstractDataManager : public QObject
    {
        Q_OBJECT
    private:
        friend class AData;
        Q_DISABLE_COPY_MOVE(AAbstractDataManager)

    public:
        explicit AAbstractDataManager(QObject* parent = nullptr);
        ~AAbstractDataManager();

        /** @brief ��ȡ���ݼ� */
        QSet<AData*> dataSet() const;

        /** @brief ɾ�����д��������ݣ��ͷ��ڴ棩 */
        void clear() const;

        /** @brief �Ӹ�������������� */
        AData* addData(const QString& name = QString());

    Q_SIGNALS:
        void dataInserted(AData* data, AData* parent, AData* after);
        void dataChanged(AData* data);
        void dataRemoved(AData* data, AData* parent);
        void dataDestroyed(AData* data);

    protected:
        virtual bool hasValue(const AData* data) const;
        virtual QIcon valueIcon(const AData* data) const;
        virtual QString valueText(const AData* data) const;

        /** @brief ����ʼ����������ʱ���ã�Ŀ���������ݹ�����֪�������Ѵ��� */
        virtual void initializeData(AData* data) = 0;

        /** @brief ������ָ������֮ǰ���ô˺�����Ŀ���������ݹ�����֪�������������ݣ��Ա�ɾ�����ݵ��������ԡ� */
        virtual void uninitializeData(AData* data);

        /** @brief �������� */
        virtual AData* createData();

    private:
        /**
         * @brief �������ݣ�ɾ���ڴ�
         * @param data ����
         */
        void destroyData(AData* data);

    private:
        QSet<AData*> m_dataSet;
    };
}
