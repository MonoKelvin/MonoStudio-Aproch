﻿/****************************************************************************
 * @file    AActionManager.cpp
 * @date    2021-1-10
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
#include "AActionManager.h"

#include <QFile>

namespace aproch
{
    APROCH_INIT_SINGLETON(AActionManager);

    AActionManager::AActionManager()
    {
    }

    AActionManager::~AActionManager()
    {
    }

    void AActionManager::addAction(AAction* action)
    {
        if (nullptr == action || action->getActionId().isEmpty())
        {
            qDebug() << "Action Invalid";
            return;
        }

        if (!mActions.contains(action))
        {
            mActions.push_back(action);
        }
        else
        {
            qDebug() << "Action Exists";
        }
    }

    bool AActionManager::loadFile(const QString& jsonFilePath)
    {
        QFile jsonFile(jsonFilePath);
        if (!jsonFile.open(QFile::ReadOnly | QFile::Text))
        {
            return false;
        }

        const QByteArray& data = jsonFile.readAll();
        jsonFile.close();

        QJsonParseError error;
        const QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &error);
        if (error.error != QJsonParseError::NoError)
        {
            qDebug() << error.errorString();
            return false;
        }

        if (jsonDoc.isObject())
        {
            const QJsonObject jsonObj = jsonDoc.object();
        }

        return true;
    }

    AAction* AActionManager::getActionById(const ActionId& actionId)
    {
        for (const auto& action : mActions)
        {
            if (action->getActionId() == actionId)
            {
                return action;
            }
        }

        return nullptr;
    }

    void AActionManager::createActionFromJsonObject(const QJsonObject& jsonObj)
    {
        QPointer<AAction> newAction = QPointer<AAction>(new AAction(this));
        if (newAction->fromJson(jsonObj))
        {
            for (const auto& action : mActions)
            {
                if (newAction->getActionId() == action->getActionId())
                {
                    // TODO
                }
            }
        }
    }
}
