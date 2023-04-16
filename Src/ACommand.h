/****************************************************************************
 * @file    ACommand.h
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
#pragma once

#include <qobject.h>

APROCH_NAMESPACE_BEGIN

/** 命令参数 */
using CommandArgs = QVariantMap;

/** 命令ID */
using CommandId = QString;

/**
 * 简单命令抽象类
 */
class APROCH_API ACommand
{
public:
    virtual ~ACommand() = default;

    /**
     * 处理命令的函数
     */
    virtual void handle(const CommandArgs &args) = 0;

    /**
     * @brief 命令Id是否有效。正则匹配表达式为（不区分大小写）：^(([a-z])+([a-z0-9])*)(\.(([a-z])+([a-z0-9])*))*$
     * @note 命令Id格式为：xxx[.xxx][.xxx]... 如果不符合该格式，则被认为是无效的。如 cmd, app.close, app.menu.openFile 是有效的；
     * .closeWindow, app., app.close. 是无效的，其中只有"."可以做分隔符，出现非字母或数字（数字必须跟在字母后）
     * 的都是无效的，如：app.close/, app.&close, 2app, app.2close 也是无效的；
     * @param cmdId 命令Id
     * @return 是否有效
     */
    static bool IsValid(const CommandId &cmdId);

public:
    /** @brief 命令ID */
    CommandId mCommandId;

    /** @brief 命令描述 */
    QString mDescription;
};

APROCH_NAMESPACE_END
