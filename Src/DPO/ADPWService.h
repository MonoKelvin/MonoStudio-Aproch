/****************************************************************************
 * @file    ADPWService.h
 * @date    2022-07-06
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
#include "DPO/IDPOService.h"
#include <QtXml/QDomElement>

#define AServiceName_DPW "aproch.DPWService"

namespace aproch
{
    /**
     * @brief DPW����
     */
    class APROCH_API ADPWData : public aproch::IDPOData
    {
    public:
        /**
         * @brief ��ȡ���Զ�Ӧ��ֵ
         * @param attrName ��������
         * @return ���Զ�Ӧ��ֵ��û�����Է��ؿ�
         */
        QString value(const QString &attrName) const;

        /**
         * @brief ��ȡ���е�����
         * @param normalize �Ƿ��׼����ȥ���������Ƶ�ǰ��ո�(QString::trimmed)��ȫ��ת��ΪСд(QString::toLower)
         * @return ���Ա�
         * @see properties
         */
        QMap<QString, QString> attributes(bool normalize = false) const;

        /** @brief XMLԪ�ؽڵ� */
        QDomElement domElement;

        /** @brief ��ֵ���� */
        static const QString key;
    };

    /**
     * @brief ��̬�����ؼ���DPW, Dynamically Parse Widget������֧�ִ��������ڵ�Ϊ���� QLayout ����
     * @note ��������Ϊxml����
     */
    class APROCH_API ADPWService : public aproch::IDPOService
    {
    public:
        ~ADPWService();

        /** @brief ���󴴽������ַ��ָ���� */
        static QString nameSplitterChar;

        virtual bool registerObjectCreator(const QSharedPointer<aproch::IObjectCreator> &creator) override;
        virtual bool unregisterObjectCreator(const QString &name) override;
        virtual QSharedPointer<aproch::IObjectCreator> getObjectCreator(const QString &name) const override;
        virtual QObject *prase(const QString &data, QObject *parent = nullptr) override;
        virtual QString serialize(QObject *object) override;

    protected:
        /**
         * @brief ����XML�ڵ㴴������
         * @param domElement XML�ڵ�
         * @param parent ������
         * @return �ؼ����򲼾֣�����
         */
        QObject *createObjectByDomElement(const QDomElement &domElement, QObject *parent);

    private:
        const QString _key(const QString &str) const;

    private:
        /** @brief ���󴴽����� */
        QMap<QString, QSharedPointer<aproch::IObjectCreator>> mCreatorMap;
    };
}
