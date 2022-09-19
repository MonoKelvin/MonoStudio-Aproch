/****************************************************************************
 * @file    AAppConfigService.cpp
 * @date    2021-10-20
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
#include "AAppConfigService.h"

const QString xPluginDirName = AStr("Application/PluginsDirectory");

namespace aproch
{
    AAppConfigService::AAppConfigService()
        : mSettings(configFilePath(), QSettings::IniFormat)
    {
        // 添加默认的配置设置
        if (!has(xPluginDirName))
            setValue(xPluginDirName, AApplicationContext::AppDirectory() + "/Plugins");
    }

    void AAppConfigService::setFileName(const QString& filename)
    {
        // 不支持更换路径
        qWarning("Can not changing the configuration directory of the application");

        // TODO: 日志
    }

    void AAppConfigService::setValue(const QString& key, const QVariant& value)
    {
        QString keyWithDef, keyWithoutDef;
        getNormalKeys(key, keyWithDef, keyWithoutDef);
        if (keyWithoutDef.isEmpty())
            return;

        mSettings.setValue(keyWithoutDef, value);
        if (!mSettings.contains(keyWithDef))
            mSettings.setValue(keyWithDef, value);
    }

    void AAppConfigService::setDefault(const QString& key, const QVariant& value)
    {
        QString keyWithDef, keyWithoutDef;
        getNormalKeys(key, keyWithDef, keyWithoutDef);
        if (keyWithoutDef.isEmpty())
            return;

        mSettings.setValue(keyWithDef, value);
    }

    QVariant AAppConfigService::getValue(const QString& key) const
    {
        QString keyWithDef, keyWithoutDef;
        getNormalKeys(key, keyWithDef, keyWithoutDef);

        return mSettings.value(keyWithoutDef);
    }

    QVariant AAppConfigService::getDefault(const QString& key) const
    {
        QString keyWithDef, keyWithoutDef;
        getNormalKeys(key, keyWithDef, keyWithoutDef);

        return mSettings.value(keyWithDef);
    }

    void AAppConfigService::reset()
    {
        const auto& allKeys = mSettings.allKeys();
        for (const auto& key : allKeys)
            reset(key);
    }

    void AAppConfigService::reset(const QString& key)
    {
        QString keyWithDef, keyWithoutDef;
        getNormalKeys(key, keyWithDef, keyWithoutDef);

        const auto& defVal = mSettings.value(keyWithDef);
        if (defVal.isValid())
            mSettings.setValue(keyWithoutDef, defVal);
    }

    bool AAppConfigService::has(const QString& key) const
    {
        QString keyWithDef, keyWithoutDef;
        getNormalKeys(key, keyWithDef, keyWithoutDef);

        return mSettings.contains(keyWithoutDef);
    }

    void AAppConfigService::getNormalKeys(const QString& key, QString& keyWithDef, QString& keyWithoutDef) const
    {
        keyWithoutDef = aproch::AStringToolkit::ReplacToSlash(key.trimmed());
        if (keyWithoutDef.isEmpty())
            return;

        // 移除开头和结尾的 "/" 符号
        if (keyWithoutDef.front() == AKey_Slash)
            keyWithoutDef.remove(0, 1);
        if (keyWithoutDef.size() > 0 && keyWithoutDef.back() == AKey_Slash)
            keyWithoutDef.remove(keyWithoutDef.size() - 1, 1);

        const QString defaultFlag = (AKey_Default + AKey_Slash).toUpper();
        if (keyWithoutDef.startsWith(defaultFlag, Qt::CaseInsensitive))
        {
            for (;;)
            {
                if (!keyWithoutDef.startsWith(defaultFlag, Qt::CaseInsensitive))
                    break;
                keyWithoutDef = keyWithoutDef.mid(defaultFlag.size());
            }
        }
        else
        {
            // 找到第一个 "/" 的位置，以此区分分组和键
            const int lastSlashIndex = keyWithoutDef.indexOf(AKey_Slash);
            if (lastSlashIndex != -1 && lastSlashIndex != keyWithoutDef.size() - 1)
            {
                keyWithoutDef = keyWithoutDef.left(lastSlashIndex).toUpper() + keyWithoutDef.mid(lastSlashIndex);
            }
        }

        if (!keyWithoutDef.isEmpty())
            keyWithDef = defaultFlag + keyWithoutDef;
    }

    const QString AAppConfigService::configFilePath() const
    {
        const QString configFileName = AApplicationContext::AppDirectory() + AStr("/Settings/") +
            AApplicationContext::AppName() + AStr(".ini");

        if (AFileToolkit::IsFileExists(configFileName))
            return configFileName;

        if (!AFileToolkit::CreateNewFile(configFileName))
        {
            // TODO: 日志
        }

        return configFileName;
    }
}
