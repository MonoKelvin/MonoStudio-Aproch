/****************************************************************************
 * @file    IDPOService.h
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
#define AServiceName_DPO "aproch.DPOService"

APROCH_NAMESPACE_BEGIN
/**
 * @brief ��̬�����ؼ���������
 */
class IDPOData
{
public:
    virtual ~IDPOData() = default;
};

/**
 * @brief ���󴴽���
 */
class IObjectCreator
{
public:
    virtual ~IObjectCreator() = default;

    /**
     * @brief �����ʶ���ƣ�����ͨ��"|"�ַ��ָ��Դ���ͬһ�����󴴽���
     * @note ע��ʱ�����ִ�Сд
     * @return ��ʶ����
     */
    virtual QString name(void) const = 0;

    /**
     * @brief ��������
     * @param name ��ʶ����
     * @param data DPO����
     * @param parent ������
     * @return �����Ķ���
     */
    virtual QObject *createObject(const QString &name, IDPOData &data, QObject *parent = nullptr) = 0;
};

/**
 * @brief ��̬��������DPO, Dynamically Parse Object������
 */
class IDPOService : public IService
{
public:
    virtual ~IDPOService() = default;

    /**
     * @brief ע����󴴽���
     * @param creator ������
     * @return �Ƿ�ע��ɹ�
     */
    virtual bool registerObjectCreator(const QSharedPointer<IObjectCreator> &creator) = 0;

    /**
     * @brief ɾ��ע��Ķ��󴴽���
     * @param name Ҫɾ���Ķ��󴴽���������
     * @return �Ƿ�ɾ���ɹ�
     */
    virtual bool unregisterObjectCreator(const QString &name) = 0;

    /**
     * @brief ��ȡ���󴴽���
     * @param name ����
     * @return ���󴴽���
     */
    virtual QSharedPointer<IObjectCreator> getObjectCreator(const QString &name) const = 0;

    /**
     * @brief ����
     * @param data ����
     * @param parent ������
     * @return
     */
    virtual QObject *prase(const QString &data, QObject *parent = nullptr) = 0;

    /**
     * @brief ���л�
     * @return QString ���л�����ַ���
     */
    virtual QString serialize(QObject *object) = 0;
};

APROCH_NAMESPACE_END
