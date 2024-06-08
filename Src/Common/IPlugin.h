/****************************************************************************
 * @file    IPlugin.h
 * @date    2022-06-04
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

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>

APROCH_NAMESPACE_BEGIN
/**
 * @brief 插件接口
 */
class IPlugin
{
public:
    virtual ~IPlugin() = default;

    /**
     * @brief 插件加载成功并启动时
     * @return 是否可以加载插件
     */
    virtual bool start() = 0;

    /**
     * @brief 当插件从系统卸载时
     */
    virtual void end() = 0;

    /**
     * @brief 所有默认系统插件加载完毕，并且可以运行时调用
     */
    virtual void run()
    {
    }
};

/**
 * @brief 插件信息
 */
struct SPluginInfo
{
    /** @brief 作者 */
    QString author;

    /** @brief 组织 */
    QString organization;

    /** @brief 版本 */
    QString version;

    /** @brief 描述 */
    QString description;

    /** @brief 其他配置数据 */
    QVariantMap config;
};

APROCH_NAMESPACE_END

/** @brief 继承自<IPlugin>的插件类需要声明的宏 */
#define APROCH_PLUGIN(_Plugin_IID_) \
    Q_PLUGIN_METADATA(IID _Plugin_IID_ FILE "AprochPlugin.json") \
    Q_INTERFACES(APROCH_CLASSNAME(IPlugin))

/** @brief 导出插件方法声明，写在.h文件中 */
#define APROCH_EXPORT_PLUGIN_DECLARE(_ExportAPIName_)                                       \
    extern "C" _ExportAPIName_ APROCH_CLASSNAME(IPlugin) *AprochCreatePluginInstance(void); \
    extern "C" _ExportAPIName_ QVariantMap AprochGetPluginConfigInfo(void);

/** @brief 导出插件方法定义，写在.cpp文件中 */
#define APROCH_EXPORT_PLUGIN_DEFINE(_PluginName_)                          \
    APROCH_CLASSNAME(IPlugin) *AprochCreatePluginInstance(void)            \
    {                                                                      \
        APROCH_ASSERT_IS_DERIVED(APROCH_CLASSNAME(IPlugin), _PluginName_); \
        return static_cast<APROCH_CLASSNAME(IPlugin) *>(new _PluginName_); \
    }                                                                      \
    QVariantMap AprochGetPluginConfigInfo(void)                            \
    {                                                                      \
        QVariantMap configMap;                                             \
        QFile file(QStringLiteral(":/plugin/AprochPlugin.json"));          \
        if (!file.open(QIODevice::ReadOnly))                               \
            return configMap;                                              \
        const QByteArray data(file.readAll());                             \
        file.close();                                                      \
        QJsonParseError jError;                                            \
        QJsonDocument jDoc = QJsonDocument::fromJson(data, &jError);       \
        if (jError.error != QJsonParseError::NoError)                      \
            return configMap;                                              \
        QJsonObject jObj = jDoc.object();                                  \
        return jObj.toVariantMap();                                        \
    }
