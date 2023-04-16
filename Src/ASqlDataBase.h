/****************************************************************************
 * @file    ASqlDataBase.h
 * @date    2022-06-05
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
#include "Common/IDataBaseService.h"

class QRecursiveMutex;

APROCH_NAMESPACE_BEGIN

/**
 * @brief ��װQtSqlDataBase�����ݿ����
 */
class APROCH_API ASqlDataBase : public IDataBaseService
{
public:
    ASqlDataBase();
    virtual ~ASqlDataBase();

    /**
     * @brief ������ݿ�
     * @param url ����·��ַ
     * @param username �˻���
     * @param password ����
     * @param connectionName �������ơ��������Ʋ��ܺ��������ӵ����ݿ����������ظ�
     * @return bool �Ƿ���ӳɹ�
     */
    virtual bool addDatabase(const QString &url, const QString &username, const QString &password, const QString &connectionName) override;

    /**
     * @brief �Ƴ����ӵ����ݿ�
     * @param connectionName ��������
     */
    virtual void removeDatabase(const QString &connectionName) override;

    /**
     * @brief ��ָ���������Ƶ����ݿ�
     * @param connectionName �������ƣ����Ϊ����򿪵�ǰ�����ӵ����ݿ�
     * @return �Ƿ�򿪳ɹ�
     */
    virtual bool open(const QString &connectionName = ANull_String) override;

    /**
     * @brief ��ָ���������Ƶ����ݿ�
     * @param username �˻���
     * @param password ����
     * @param connectionName �������ƣ����Ϊ����򿪵�ǰ�����ӵ����ݿ�
     * @return �Ƿ�򿪳ɹ�
     */
    virtual bool open(const QString &username, const QString &password, const QString &connectionName = ANull_String) override;

    /**
     * @brief �ر����ݿ�
     * @param connectionName �������ƣ����Ϊ����رյ�ǰ�����ӵ����ݿ�
     */
    virtual void close(const QString &connectionName = ANull_String) override;

    /**
     * @brief ִ�����
     * @param query sql���
     * @return ������
     */
    virtual QSqlError exec(const QString &query) override;

    /**
     * @brief ִ�в�ѯ���
     * @param sqlQuery ��ѯ���
     * @return ��ѯ���
     */
    virtual QSqlQuery query(const QString &sqlQuery) override;

    /**
     * @brief ��ȡ��ǰ��������ݿ�
     * @return SQL���ݿ�
     */
    virtual QSqlDatabase &dataBase() override;

protected:
    /** @brief ��ǰ���ݿ���� */
    QSharedPointer<QSqlDatabase> mSqlDB;

    /** @brief ������ */
    QRecursiveMutex *mMutex;
};

APROCH_NAMESPACE_END
