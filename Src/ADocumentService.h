/****************************************************************************
 * @file    ADocumentService.h
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
#include "Common\IDocumentService.h"

namespace aproch
{
    /**
     * @brief 文档服务
     * @note 如果设计为简单文档，可以将文档名称<documentName>设置为目录名称，在该目录<workspace>下保存其他文档文件<ADocFile>
     */
    class APROCH_API ADocumentService : public QObject, public IDocumentService
    {
        Q_OBJECT
    public:
        explicit ADocumentService(QObject *parent);

        virtual bool save(const QString &docPath) override;
        virtual bool load(const QString &docPath) override;

        /**
         * @brief 设置工作目录。这会将工作目录下的文件全部移动到新的目录下
         * @param dir 目录。如果目录不存在，则递归创建目录
         * @return 是否成功
         */
        virtual bool setWorkspace(const QDir &dir) override;

        /**
         * @brief 获取本地工作空间目录
         * @return 目录
         */
        inline virtual QDir getWorkspace() const noexcept override
        {
            return mWorkspace;
        }

        /**
         * @brief 设置文档名称。
         * @note 如果文档内有任何一个文件正在使用中，都是无法设置成功的。所以该方法是初始化文档设定好的，一般不会二次更改；
         * @param docName 文档名称
         * @return 是否成功
         */
        virtual bool setDocumentName(const QString &docName) override;

        /**
         * @brief 获取文档名称
         * @return 文档名称
         */
        inline virtual QString getDocumentName() const noexcept override
        {
            return mDocName;
        }

        /**
         * @brief 添加文件
         * @param filePath 文件路径
         * @return 如果添加成功，将返回其指针，否则返回nullptr
         */
        ADocFile *addDocFile(const QString &filePath);

        /**
         * @brief 添加文件
         * @param docFile 文件
         * @return 如果添加成功，将返回true，否则返回false
         */
        bool addDocFile(ADocFile *docFile);

        /**
         * @brief 移除文档文件。如果该文件存在，则从磁盘和内存中删除
         * @param filePath 文件路径
         */
        void removeDocFile(const QString &filePath);

        /**
         * @brief 移除文件。如果该文件存在，则从磁盘和内存中删除
         * @param docFile 文件
         */
        void removeDocFile(ADocFile *docFile);

    protected:
        /** @brief 文档文件列表 */
        QList<ADocFile *> mDocFiles;

        /** @brief 工作目录 */
        QDir mWorkspace;

        /** @brief 文档名称 */
        QString mDocName;
    };
}
