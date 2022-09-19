/****************************************************************************
 * @file    AApplicationContext.h
 * @date    2021-9-30
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
#include "Common/IService.h"

namespace aproch
{
    /**
     * @brief 应用程序上下文
     */
    class APROCH_API AApplicationContext : public QObject
    {
        Q_OBJECT
        APROCH_OBJECT_SINGLETON(AApplicationContext);

    public:
        ~AApplicationContext();

        /**
         * @brief 运行应用
         * @param mainWidget 主窗口
         */
        void run(QWidget* mainWidget = nullptr);

        /**
         * @brief 注册服务。如果服务已存在则会被覆盖
         * @tparam ServiceName 服务类名
         */
        template <class ServiceName>
        IServicePtr registerService(const char* name)
        {
            APROCH_ASSERT_IS_DERIVED(aproch::IService, ServiceName);
            unregisterService(name);
            const IServicePtr pService = IServicePtr(new ServiceName);
            mServiceMap[name] = pService;
            return pService;
        }

        /**
         * @brief 取消注册服务
         * @tparam ServiceName 服务名称
         * @return 是否取消注册成功
         */
        void unregisterService(const char* name);

        /**
         * @brief 获取服务
         * @tparam ServiceName 服务名称
         * @return 服务
         */
        IServicePtr getService(const char* name) const
        {
            return mServiceMap.value(name);
        }

        /**
         * @brief 获取应用程序的主窗口
         * @return 应用程序的主窗口
         */
        static QWidget* MainWindow() noexcept;

        /**
         * @brief 获取程序所在的绝对路径，不包含文件名。如程序路径为 C:/Work/Dir/aproch.exe，则得到的绝对路径为 C:/Work/Dir
         * @see AppPath
         * @return 绝对路径
         */
        static QString AppDirectory();

        /**
         * @brief 获取程序所在的绝对路径文件名。如程序路径为 C:/Work/Dir/aproch.exe，则得到的绝对路径为 C:/Work/Dir/aproch.exe
         * @see AppDirectory
         * @return 绝对路径文件名
         */
        static QString AppPath();

        /**
         * @brief 应用程序的名称。如程序路径为 C:/Work/Dir/aproch.exe，则得到的程序名称为 aproch
         * @return 名称
         */
        static QString AppName();

    private:
        AApplicationContext(QObject* parent = nullptr);

    private:
        /** @brief 服务表 */
        QMap<QString, IServicePtr> mServiceMap;

        /** @brief 主窗口 */
        static QWidget* sMainWindow;
    };
}