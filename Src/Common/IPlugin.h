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

namespace aproch
{
    /**
     * @brief ����ӿ�
     */
    class IPlugin
    {
    public:
        virtual ~IPlugin() = default;

        /**
         * @brief ������سɹ�������ʱ
         * @return �Ƿ���Լ��ز��
         */
        virtual bool start() = 0;

        /**
         * @brief �������ϵͳж��ʱ
         */
        virtual void end() = 0;

        /**
         * @brief ����Ĭ��ϵͳ���������ϣ����ҿ�������ʱ����
         */
        virtual void run()
        {
        }
    };

    /**
     * @brief �����Ϣ
     */
    struct SPluginInfo
    {
        /** @brief ���� */
        QString author;

        /** @brief ��֯ */
        QString organization;

        /** @brief �汾 */
        QString version;

        /** @brief ���� */
        QString description;

        /** @brief ������������ */
        QVariantMap config;
    };
}

/** @brief ������� */
#define APROCH_PLUGIN_NAME "Aproch.Plugin.v1.0.0"
Q_DECLARE_INTERFACE(aproch::IPlugin, APROCH_PLUGIN_NAME)

/** @brief �̳���<IPlugin>�Ĳ������Ҫ�����ĺ� */
#define APROCH_PLUGIN() \
Q_PLUGIN_METADATA(IID APROCH_PLUGIN_NAME FILE "AprochPlugin.json") \
Q_INTERFACES(aproch::IPlugin)

/** @brief �����������������д��.h�ļ��� */
#define APROCH_EXPORT_PLUGIN_DECLARE(_ExportAPIName_) \
extern "C" _ExportAPIName_ aproch::IPlugin* AprochCreatePluginInstance(void); \
extern "C" _ExportAPIName_ QVariantMap AprochGetPluginConfigInfo(void);

/** @brief ��������������壬д��.cpp�ļ��� */
#define APROCH_EXPORT_PLUGIN_DEFINE(_PluginName_) \
aproch::IPlugin* AprochCreatePluginInstance(void) \
{ \
    APROCH_ASSERT_IS_DERIVED(aproch::IPlugin, _PluginName_); \
    return static_cast<aproch::IPlugin*>(new _PluginName_); \
} \
QVariantMap AprochGetPluginConfigInfo(void) \
{ \
    QVariantMap configMap; \
    QFile file(QStringLiteral(":/plugin/AprochPlugin.json")); \
    if (!file.open(QIODevice::ReadOnly)) \
        return configMap; \
    const QByteArray data(file.readAll()); \
    file.close(); \
    QJsonParseError jError; \
    QJsonDocument jDoc = QJsonDocument::fromJson(data, &jError); \
    if (jError.error != QJsonParseError::NoError) \
        return configMap; \
    QJsonObject jObj = jDoc.object(); \
    return jObj.toVariantMap(); \
}
