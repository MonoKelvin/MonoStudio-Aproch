/****************************************************************************
 * @file    AAppContext.h
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

APROCH_NAMESPACE_BEGIN

class AAppContextPrivate;

/**
 * @brief 应用程序上下文
 */
class APROCH_API AAppContext : public QObject
{
    Q_OBJECT
    APROCH_OBJECT_SINGLETON(AAppContext);

public:
    ~AAppContext();

    /** @brief 运行应用 */
    static int run(int argc, char* argv[]);

    /** @brief 获取应用程序的主窗口 */
    static QWidget* getMainWindow();

    /** @brief 设置应用程序的主窗口，建议在 ready 信号中处理 */
    static bool setMainWindow(QWidget* mainWindow);

    /** 服务 */

    /** @brief 注册服务 */
    template <class ServiceName>
    static QSharedPointer<IService> registerService(const QString& name)
    {
        APROCH_ASSERT_IS_DERIVED(IService, ServiceName);
        const QSharedPointer<IService> pService = QSharedPointer<IService>(new ServiceName);
        if (!registerService(name, pService))
            return nullptr;
        return getService(name);
    }

    /** @brief 获取服务 */
    static QSharedPointer<IService> getService(const QString& name);

    /** @brief 注册服务 */
    static bool registerService(const QString& name, QSharedPointer<IService> service);

    /** @brief 取消注册服务 */
    static void unregisterService(const QString& name);

Q_SIGNALS:
    /** @brief 软件刚启动时信号。此时应用程序实例未创建 */
    void launch(bool& canContinue);

    /** @brief 软件加载组件、资源等进度信号。此时应用程序实例已创建 */
    void loading(int step, int totalStep, bool& canContinue);

    /** @brief 软件准备启动。此时应用程序实例已创建 */
    void ready(bool& canContinue);

    /** @brief 软件正式启动信号。正常情况下此时界面已经打开 */
    void start();

    /** @brief 软件退出信号 */
    void exit(int retCode);

public Q_SLOTS:
    /** @brief 重启应用程序 */
    void restart();

private:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    AAppContext(QObject *parent = nullptr);

private:
    friend class AAppContextPrivate;
    Q_DISABLE_COPY_MOVE(AAppContext);
    QScopedPointer<AAppContextPrivate> d_ptr;
};

APROCH_NAMESPACE_END
