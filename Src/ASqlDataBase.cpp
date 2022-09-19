/****************************************************************************
 * @file    ASqlDataBase.cpp
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
#include "stdafx.h"
#include "ASqlDataBase.h"

#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QRecursiveMutex>

namespace aproch
{
    ASqlDataBase::ASqlDataBase()
    {
        mSqlDB = QSharedPointer<QSqlDatabase>(new QSqlDatabase());
        mMutex = new QRecursiveMutex();
    }

    ASqlDataBase::~ASqlDataBase()
    {
        mMutex->lock();
        const auto& connectinonNameList = QSqlDatabase::connectionNames();
        for (const auto& connectionName : connectinonNameList)
            QSqlDatabase::removeDatabase(connectionName);
        mMutex->unlock();

        delete mMutex;
        mMutex = nullptr;
    }

    bool ASqlDataBase::addDatabase(const QString& url, const QString& username, const QString& password,
                                    const QString& connectionName)
    {
        QMutexLocker locker(mMutex);

        // 是否已经存在连接名
        if (QSqlDatabase::connectionNames().contains(connectionName))
            return false;

        mSqlDB.reset(new QSqlDatabase(QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), connectionName)));
        mSqlDB->setDatabaseName(url);
        if (mSqlDB->password().isEmpty())
        {
            mSqlDB->setUserName(username);
            mSqlDB->setPassword(password);
        }

        const bool ok = mSqlDB->open(username, password);

        return ok;
    }

    void ASqlDataBase::removeDatabase(const QString& connectionName)
    {
        QMutexLocker locker(mMutex);
        QSqlDatabase::removeDatabase(connectionName);
    }

    bool ASqlDataBase::open(const QString& connectionName)
    {
        QMutexLocker locker(mMutex);

        if (!connectionName.isEmpty())
            mSqlDB.reset(new QSqlDatabase(QSqlDatabase::database(connectionName)));

        return mSqlDB->open();
    }

    bool ASqlDataBase::open(const QString& username, const QString& password, const QString& connectionName)
    {
        QMutexLocker locker(mMutex);

        if (!connectionName.isEmpty())
            mSqlDB.reset(new QSqlDatabase(QSqlDatabase::database(connectionName)));

        return mSqlDB->open(username, password);
    }

    void ASqlDataBase::close(const QString& connectionName)
    {
        QMutexLocker locker(mMutex);

        if (!connectionName.isEmpty())
            mSqlDB.reset(new QSqlDatabase(QSqlDatabase::database(connectionName)));

        mSqlDB->close();
    }

    QSqlError ASqlDataBase::exec(const QString& query)
    {
        QMutexLocker locker(mMutex);

        if (!mSqlDB->isOpen())
            open();

        QSqlQuery sqlQuery(*mSqlDB);
        sqlQuery.exec(query);
        return sqlQuery.lastError();
    }

    QSqlQuery ASqlDataBase::query(const QString& sqlQuery)
    {
        QMutexLocker locker(mMutex);

        if (!mSqlDB->isOpen())
            open();

        QSqlQuery query(*mSqlDB);
        query.exec(sqlQuery);
        return query;
    }

    QSqlDatabase& ASqlDataBase::dataBase()
    {
        return *mSqlDB;
    }
}
