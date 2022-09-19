/****************************************************************************
 * @file    ADocFile.cpp
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
#include "stdafx.h"
#include "ADocFile.h"

namespace aproch
{
    ADocFile::ADocFile(QObject* parent)
        : QObject(parent)
        , mHandleType(UsedRelease)
    {
    }

    ADocFile::ADocFile(const QString& filePath, ADocFile::EHandleFileType handleType, QObject* parent)
        : QObject(parent)
        , mHandleType(handleType)
    {
        setFilePath(filePath);
    }

    ADocFile::~ADocFile()
    {
        closeFile();
    }

    bool ADocFile::save(const QByteArray& data)
    {
        closeFile();
        mFile.remove();
        if (mFile.open(QIODevice::WriteOnly))
        {
            const qint64 writtenData = mFile.write(data);
            if (mHandleType == UsedRelease)
                mFile.close();

            if (writtenData >= 0)
            {
                emit saved();
                return true;
            }
        }
        else
        {
            qDebug() << "File open failed.";
        }

        return false;
    }

    QByteArray ADocFile::load()
    {
        if (!mFile.isOpen())
            mFile.open(QIODevice::ReadOnly | QIODevice::Text);

        const QByteArray data = mFile.readAll();
        if (mHandleType == UsedRelease)
            mFile.close();

        emit loaded();
        return data;
    }

    void ADocFile::setHandleFileType(EHandleFileType type)
    {
        mHandleType = type;
        if (mHandleType == UsedRelease)
        {
            closeFile();
        }
        else
        {
            mFile.open(QIODevice::ReadOnly);
        }
    }

    void ADocFile::setFilePath(const QString& filePath)
    {
        if (QFile(filePath).fileName() == mFile.fileName())
            return;

        // 关闭之前的文件
        closeFile();

        mFile.setFileName(filePath);
        if (mFile.exists() && mHandleType == Occupy)
        {
            mFile.open(QIODevice::ReadWrite);
        }
    }

    QString ADocFile::getFilePath() const
    {
        return mFile.fileName();
    }

    bool ADocFile::operator==(const ADocFile& rhs) const
    {
        return rhs.mFile.fileName() == mFile.fileName();
    }

    bool ADocFile::operator==(const QString& filePath) const
    {
        return QFile(filePath).fileName() == mFile.fileName();
    }

    void ADocFile::closeFile()
    {
        if (mFile.isOpen())
            mFile.close();
    }
}
