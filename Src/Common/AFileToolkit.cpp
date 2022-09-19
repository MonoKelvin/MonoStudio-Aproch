/****************************************************************************
 * @file    AFileToolkit.cpp
 * @date    2021-1-17
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
#include "AFileToolkit.h"

#include <regex>

#include <QDir>
#include <QCoreApplication>

namespace aproch
{
    bool AFileToolkit::CreateDir(const QString& path, bool recursion/* = true*/)
    {
        const QDir dir(path);
        if (dir.exists())
        {
            return true;
        }

        if (recursion)
        {
            return dir.mkpath(path); // 创建多级目录
        }

        return dir.mkdir(path); // 只创建一级子目录，即必须保证上级目录存在
    }

    QString AFileToolkit::GetAppDirectory()
    {
        return QCoreApplication::applicationDirPath();
    }

    bool AFileToolkit::CreateNewFile(const QString& fileName, bool recursion)
    {
        const QFileInfo fileInfo(fileName);
        CreateDir(fileInfo.path(), recursion);

        QFile file(fileName);
        const bool ok = file.open(QFile::WriteOnly);
        file.close();

        return ok;
    }

    bool AFileToolkit::CopyAFile(const QString& sourceFile, const QString& newFile, bool isOverWrite)
    {
        QFile file(sourceFile);
        if (!file.exists())
        {
            return false;
        }

        if (QDir(sourceFile).absolutePath() == QDir(newFile).absolutePath())
            return true;

        const QFileInfo fileInfo(newFile);
        QDir().mkpath(fileInfo.path());

        if (isOverWrite)
        {
            QFile(newFile).remove(newFile);
        }

        return file.copy(newFile);
    }

    bool AFileToolkit::CopyDirectory(const QString& fromDir, const QString& toDir, bool isOverWrite)
    {
        QDir sourceDir(fromDir);
        QDir targetDir(toDir);

        if (!targetDir.exists() && !targetDir.mkdir(targetDir.absolutePath()))
            return false;

        QFileInfoList fileInfoList = sourceDir.entryInfoList();
        for (const auto& fileInfo : fileInfoList)
        {
            if (fileInfo.fileName() == AStr(".") || fileInfo.fileName() == AStr("..") ||
                fileInfo.fileName() == targetDir.dirName())
                continue;

            if (fileInfo.isDir())
            {
                CopyDirectory(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName()), isOverWrite);
            }
            else
            {
                if (isOverWrite && targetDir.exists(fileInfo.fileName()))
                    targetDir.remove(fileInfo.fileName());

                QFile::copy(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName()));
            }
        }

        return true;
    }

    bool AFileToolkit::MoveDirectory(const QString& fromDir, const QString& toDir, bool isOverWrite)
    {
        QDir sourceDir(fromDir);
        QDir targetDir(toDir);

        if (!targetDir.exists() && !targetDir.mkdir(targetDir.absolutePath()))
            return false;

        QFileInfoList fileInfoList = sourceDir.entryInfoList();
        for (auto fileInfo = fileInfoList.begin(); fileInfo != fileInfoList.end();)
        {
            if (fileInfo->fileName() == AStr(".") || fileInfo->fileName() == AStr("..") ||
                fileInfo->fileName() == targetDir.dirName())
            {
                ++fileInfo;
                continue;
            }

            const QString tarFilePath = targetDir.filePath(fileInfo->fileName());
            if (fileInfo->isDir())
            {
                CopyDirectory(fileInfo->filePath(), tarFilePath, isOverWrite);
                QDir(fileInfo->filePath()).removeRecursively();
            }
            else
            {
                if (isOverWrite && targetDir.exists(fileInfo->fileName()))
                    targetDir.remove(fileInfo->fileName());

                QFile::copy(fileInfo->filePath(), tarFilePath);
                QFile(fileInfo->filePath()).remove();
            }

            fileInfo = fileInfoList.erase(fileInfo);
        }

        return true;
    }

    bool AFileToolkit::RemoveFileOrDirectory(const QString& path)
    {
        QFile theFile(path);
        if (theFile.exists())
            return theFile.remove();

        QDir theDir(path);
        if (theDir.exists())
            return theDir.removeRecursively();

        return false;
    }

    bool AFileToolkit::HasIllegalChar(const QString& str, bool checkEmpty)
    {
        if (checkEmpty && str.isEmpty())
        {
            return false;
        }

        const std::regex reg("[\\s\\\\/:\\*\\?\\\"<>\\|]");
        return std::regex_search(str.toStdString(), reg);
    }

    bool AFileToolkit::IsFileExists(const QString& filePath)
    {
        const QFile file(filePath);
        return file.exists();
    }

    bool AFileToolkit::IsPathExists(const QString& path)
    {
        if (path.isEmpty())
            return false;

        const QDir dir(path);
        return dir.exists();
    }
}