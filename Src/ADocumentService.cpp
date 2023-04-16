/****************************************************************************
 * @file    ADocumentService.cpp
 * @date    2021-10-22
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
#include "ADocumentService.h"

APROCH_NAMESPACE_BEGIN

ADocumentService::ADocumentService(QObject *parent)
    : QObject(parent), mWorkspace(AApplicationContext::AppDirectory() + AStr("/Workspace"))
{
}

bool ADocumentService::save(const QString &docPath)
{
    return false;
}

bool ADocumentService::load(const QString &docPath)
{
    return false;
}

bool ADocumentService::setWorkspace(const QDir &dir)
{
    if (mWorkspace == dir)
        return true;

    if (!dir.exists() && !AFileToolkit::CreateDir(dir.absolutePath()))
        return false;

    if (!AFileToolkit::MoveDirectory(mWorkspace.absolutePath(), dir.absolutePath()))
        return false;

    mWorkspace = dir;
    return true;
}

bool ADocumentService::setDocumentName(const QString &docName)
{
    QDir newWorksapce = mWorkspace;
    if (!newWorksapce.cdUp())
        return false;

    const QFileInfo fileInfo(newWorksapce.absolutePath() + AKey_Slash + docName);

    // 如果是文档是目录
    if (fileInfo.isDir())
    {
        mWorkspace.rename(mDocName, fileInfo.absolutePath());
    }
    else if (fileInfo.isFile())
    {
        // 如果文档是文件
        mWorkspace.setPath(fileInfo.absoluteFilePath());
    }
    mDocName = docName;
    return true;
}

ADocFile *ADocumentService::addDocFile(const QString &filePath)
{
    for (const auto &f : mDocFiles)
    {
        if (*f == filePath)
            return nullptr;
    }

    ADocFile *newDocFile = new ADocFile(filePath, ADocFile::UsedRelease, this);
    mDocFiles.append(newDocFile);
    return newDocFile;
}

bool ADocumentService::addDocFile(ADocFile *docFile)
{
    if (nullptr == docFile)
        return false;

    for (const auto &f : mDocFiles)
    {
        if (f == docFile || *f == *docFile)
            return false;
    }

    docFile->setParent(this);
    mDocFiles.append(docFile);
    return true;
}

void ADocumentService::removeDocFile(const QString &filePath)
{
}

void ADocumentService::removeDocFile(ADocFile *docFile)
{
}

APROCH_NAMESPACE_END
