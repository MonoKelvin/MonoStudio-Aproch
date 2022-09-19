/****************************************************************************
 * @file    IDataBaseService.h
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
class QSqlDatabase;
class QSqlQuery;
class QSqlError;

#define AServiceName_DataBase "aproch.DataBaseService"

namespace aproch
{
    /**
     * @brief 数据库服务接口
     */
    class IDataBaseService : public IService
    {
    public:
        virtual ~IDataBaseService() = default;

        /**
         * @brief 添加数据库
         * @param url 数据路地址
         * @param username 账户名
         * @param password 密码
         * @param connectionName 连接名称。连接名称不能和现有连接的数据库连接名称重复
         * @return bool 是否添加成功
         */
        virtual bool addDatabase(const QString &url, const QString &username, const QString &password, const QString &connectionName) = 0;

        /**
         * @brief 移除连接的数据库
         * @param connectionName 连接名称
         */
        virtual void removeDatabase(const QString &connectionName) = 0;

        /**
         * @brief 打开指定连接名称的数据库
         * @param connectionName 连接名称，如果为空则打开当前正连接的数据库
         * @return 是否打开成功
         */
        virtual bool open(const QString &connectionName = ANull_String) = 0;

        /**
         * @brief 打开指定连接名称的数据库
         * @param username 账户名
         * @param password 密码
         * @param connectionName 连接名称，如果为空则打开当前正连接的数据库
         * @return 是否打开成功
         */
        virtual bool open(const QString &username, const QString &password, const QString &connectionName = ANull_String) = 0;

        /**
         * @brief 关闭数据库
         * @param connectionName 连接名称，如果为空则关闭当前正连接的数据库
         */
        virtual void close(const QString &connectionName = ANull_String) = 0;

        /**
         * @brief 执行语句
         * @param query sql语句
         * @return 错误结果
         */
        virtual QSqlError exec(const QString &query) = 0;

        /**
         * @brief 执行查询语句
         * @param sqlQuery 查询语句
         * @return 查询结果
         */
        virtual QSqlQuery query(const QString &sqlQuery) = 0;

        /**
         * @brief 获取当前开打的数据库
         * @return SQL数据库
         */
        virtual QSqlDatabase &dataBase() = 0;
    };
}
