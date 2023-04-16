/****************************************************************************
 * @file    IDocumentService.h
 * @date    2021-10-21
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
#include <QDir>

#define AServiceName_Document "aproch.DocumentService"

APROCH_NAMESPACE_BEGIN

/**
 * @brief 文档服务接口
 */
class IDocumentService : public IService
{
public:
    virtual ~IDocumentService() = default;

    /**
     * @brief 保存文档到指定路径
     * @param docPath 文档路径
     * @return 是否成功
     */
    virtual bool save(const QString &docPath) = 0;

    /**
     * @brief 从指定路径加载文档
     * @param docPath 文档路径
     * @return 是否成功
     */
    virtual bool load(const QString &docPath) = 0;

    /**
     * @brief 设置工作目录
     * @param dir 目录
     * @return 是否成功
     */
    virtual bool setWorkspace(const QDir &dir) = 0;

    /**
     * @brief 获取本地工作空间目录
     * @return 目录
     */
    virtual QDir getWorkspace() const = 0;

    /**
     * @brief 设置文档名称
     * @param docName 文档名称
     * @param 是否成功
     */
    virtual bool setDocumentName(const QString &docName) = 0;

    /**
     * @brief 获取文档名称
     * @return 文档名称
     */
    virtual QString getDocumentName() const = 0;
};

APROCH_NAMESPACE_END
