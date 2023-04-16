/****************************************************************************
 * @file    ACommandManager.cpp
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
#include "ACommandManager.h"

APROCH_NAMESPACE_BEGIN

APROCH_INIT_SINGLETON(ACommandManager);

ACommandManager::ACommandManager()
{
}

ACommandManager::~ACommandManager()
{
}

void ACommandManager::executeCommand(const CommandId &commandId)
{
    CommandArgs args;
    executeCommand(commandId, args);
}

void ACommandManager::executeCommand(const CommandId &commandId, const CommandArgs &args)
{
    for (const auto &cmd : mCommandList)
    {
        if (nullptr != cmd && cmd->mCommandId == commandId)
        {
            cmd->handle(args);
            // QMetaObject::invokeMethod(cmd, "handle", Qt::AutoConnection, Q_ARG(CommandArgs, args));
            break;
        }
    }
}

void ACommandManager::registerCommand(const QSharedPointer<ACommand> &command)
{
    if (nullptr == command || !ACommand::IsValid(command->mCommandId))
    {
        qDebug() << "Invalid Command";
        return;
    }

    int i = 0;
    for (; i < mCommandList.size(); ++i)
    {
        if (mCommandList.at(i)->mCommandId == command->mCommandId)
        {
            break;
        }
    }

    if (i != mCommandList.size())
    {
        qDebug() << "Command Exists";
        mCommandList.replace(i, command);
    }
    else
    {
        mCommandList.push_back(command);
    }
}

QSharedPointer<ACommand> ACommandManager::getCommand(const CommandId &commandId)
{
    for (const auto &command : mCommandList)
    {
        if (command->mCommandId == commandId)
        {
            return command;
        }
    }

    return nullptr;
}

APROCH_NAMESPACE_END
