/****************************************************************************
 * @file    APrivateAppConfigService.cpp
 * @date    2022-06-12
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
#include "APrivateAppConfigService.h"

namespace aproch
{
    APrivateAppConfigService::APrivateAppConfigService()
    {
    }

    void APrivateAppConfigService::setFileName(const QString& filename)
    {
        QFile file(filename);
        if (!file.open(QFile::ReadOnly))
            return;

        file.readAll();

        auto jsonDoc = QJsonDocument::fromJson(QByteArray().fromBase64(file.readAll()));
        if (jsonDoc.isNull() || !jsonDoc.isObject())
            return;

        mConfigMap.clear();

        QJsonObject jsonObj = jsonDoc.object();
        mConfigMap = jsonObj.toVariantMap();
    }

    void APrivateAppConfigService::setValue(const QString& key, const QVariant& value)
    {
        mConfigMap[key] = value;
    }

    QVariant APrivateAppConfigService::getValue(const QString& key) const
    {
        auto iter = mConfigMap.find(key);
        if (iter == mConfigMap.end())
            return ANull_Variant;

        return iter.value();
    }

    QVariant APrivateAppConfigService::getDefault(const QString& key) const
    {
        return getValue(key);
    }

    bool APrivateAppConfigService::has(const QString& key) const
    {
        return mConfigMap.contains(key);
    }
}
