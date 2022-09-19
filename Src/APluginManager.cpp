/****************************************************************************
 * @file    APluginManager.cpp
 * @date    2021-1-9
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
#include "APluginManager.h"

#include <QPluginLoader>
#include <qmessagebox.h>

using CreatePluginInstanceFunc = aproch::IPlugin* (*)(void);
using GetPluginConfigInfoFunc = QVariantMap(*)(void);

namespace aproch
{
    APROCH_INIT_SINGLETON(APluginManager);

    APluginManager::APluginManager()
    {
    }

    APluginManager::~APluginManager()
    {
        auto plugins = mPlugins.keys();
        for (auto&& pPlugin: plugins)
        {
            pPlugin->end();
            delete pPlugin;
            pPlugin = nullptr;
        }
        mPlugins.clear();
    }

    bool APluginManager::loadPlugin(const QString& dllFileName)
    {
        QLibrary mylib(dllFileName);
        if (!mylib.load())
        {
            // TODO: 日志
            qDebug() << mylib.errorString();
            return false;
        }

        CreatePluginInstanceFunc pFunc = (CreatePluginInstanceFunc)mylib.resolve("AprochCreatePluginInstance");
        if (nullptr == pFunc)
        {
            qDebug() << "No export function for library: " << dllFileName;
            return false;
        }

        IPlugin* pPlugin = pFunc();
        if (nullptr == pPlugin)
        {
            qDebug() << "Dynamic library instantiation failed：" << dllFileName;
            return false;
        }

        /*QPluginLoader loader(dllFileName);
        if (!loader.isLoaded())
        {
            // TODO: 日志
            qDebug() << loader.errorString();
            return false;
        }

        IPlugin* pPlugin = qobject_cast<IPlugin*>(loader.instance());
        if (nullptr == pPlugin)
            return false;*/

        SPluginInfo info;
        GetPluginConfigInfoFunc pGetPIFunc = (GetPluginConfigInfoFunc)mylib.resolve("AprochGetPluginConfigInfo");
        if (nullptr != pGetPIFunc)
        {
            auto dataMap = pGetPIFunc();
            info.author = dataMap.take(AStr("author")).toString();
            info.organization = dataMap.take(AStr("organization")).toString();
            info.version = dataMap.take(AStr("version")).toString();
            info.description = dataMap.take(AStr("description")).toString();
            info.config = dataMap;
        }

        if (!pPlugin->start())
            return true;

        mPlugins[pPlugin] = info;
        return true;
    }

    void APluginManager::loadPlugins()
    {
        QStringList dirList;
        dirList << AApplicationContext::getInstance()->AppDirectory();

        AAppConfigService configService;
        dirList << configService.getValue("Application/PluginsDirectory").toString();

        QStringList fileFilters;
        fileFilters << "*.dll" << "*.a" << "*.so" << "*.sl" << "*.dylib" << "*.bundle";

        for (auto&& path : dirList)
        {
            QDir dir(path);
            if (dir.isEmpty())  // TODO: 日志
                continue;

            const auto& fileInfoList = dir.entryInfoList(fileFilters, QDir::Files | QDir::Hidden);
            for (const QFileInfo& fileInfo : fileInfoList)
            {
                loadPlugin(fileInfo.absoluteFilePath());
            }
        }
    }
}
