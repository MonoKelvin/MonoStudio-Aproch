/****************************************************************************
 * @file    AApplicationContext.cpp
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
#include "stdafx.h"
#include "AApplicationContext.h"

#include <QCoreApplication>

namespace aproch
{
    APROCH_OBJECT_INIT_SINGLETON(AApplicationContext)

    QWidget* AApplicationContext::sMainWindow = nullptr;

    AApplicationContext::AApplicationContext(QObject* parent)
        : QObject(parent)
    {
    }

    AApplicationContext::~AApplicationContext()
    {
    }

    void AApplicationContext::run(QWidget* mainWidget)
    {
        sMainWindow = mainWidget;

        // 加载插件
        APluginManager::getInstance()->loadPlugins();

        // 注册默认的服务
        registerService<AKeyExpressionService>(AServiceName_KeyExpression);
        registerService<AAppConfigService>(AServiceName_AppConfig);
        registerService<ASqlDataBase>(AServiceName_DataBase);

        // 注册默认的对象创建器
        auto pDPWService = qSharedPointerCast<ADPWService>(registerService<ADPWService>(AServiceName_DPW));
        pDPWService->registerObjectCreator(QSharedPointer<aproch::IObjectCreator>(new ABoxLayoutCreator()));
        pDPWService->registerObjectCreator(QSharedPointer<aproch::IObjectCreator>(new AEditCreator()));

        // 注册默认图标字体
        const AIconFont fontAwesome(AStr(":/font/Fonts/fontawesome-webfont.ttf"));
        AIconFont::AddIconFont(fontAwesome);

        // 运行所有插件
        const auto& plugins = APluginManager::getInstance()->getPlugins();
        for (auto plg = plugins.begin(); plg != plugins.end(); ++plg)
        {
            Q_ASSERT(nullptr != plg.key());
            plg.key()->run();
        }

        /// TEST
        /*QFile file("E:/Project/Code/Aproch/Test/test_dpw.xml");
        file.open(QFile::ReadOnly | QFile::Text);
        QString data = file.readAll();
        file.close();
        QDialog dialog;
        pDPWService->prase(data, &dialog);
        dialog.exec();*/
        ///
    }

    void AApplicationContext::unregisterService(const char* name)
    {
        IServicePtr service = mServiceMap.value(name);
        if (nullptr != service)
        {
            mServiceMap.remove(name);
        }
    }

    QWidget* AApplicationContext::MainWindow() noexcept
    {
        return sMainWindow;
    }

    QString AApplicationContext::AppDirectory()
    {
        return QCoreApplication::applicationDirPath();
    }

    QString AApplicationContext::AppPath()
    {
        return QCoreApplication::applicationFilePath();
    }

    QString AApplicationContext::AppName()
    {
        return QCoreApplication::applicationName();
    }
}