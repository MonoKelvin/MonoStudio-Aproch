/****************************************************************************
 * @file    ADocFile.h
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

APROCH_NAMESPACE_BEGIN

/**
 * @brief 文档文件
 */
class APROCH_API ADocFile : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 操作文件时的类型
     */
    enum EHandleFileType
    {
        UsedRelease, // 使用完立即释放
        Occupy,      // 占用文件，直到主动释放。如果文件不可操作，则改变其属性为可读写（除非没有权限）
    };

    ADocFile(QObject *parent = nullptr);
    explicit ADocFile(const QString &filePath, EHandleFileType handleType = Occupy, QObject *parent = nullptr);
    virtual ~ADocFile();

    /**
     * @brief 保存文件
     * @param 要保存的数据
     * @return 是否成功
     */
    virtual bool save(const QByteArray &data);

    /**
     * @brief 加载文件
     * @return 字节数据
     */
    virtual QByteArray load();

    /**
     * @brief 设置文件处理类型
     * @param type 类型
     */
    void setHandleFileType(EHandleFileType type);

    /**
     * @brief 获取文件处理类型
     * @return 类型
     */
    inline EHandleFileType getHandleFileType() const noexcept
    {
        return mHandleType;
    }

    /**
     * @brief 设置文件路径
     * @param filePath 文件路径。文件可以不存在
     * @return
     */
    void setFilePath(const QString &filePath);

    /**
     * @brief 获取文件路径
     * @return 文件路径
     */
    QString getFilePath() const;

    /**
     * @brief 两个文件是否相等。文件地址一样则认识是同一个文件
     * @param rhs 另一个文档文件
     * @return 是否是同一个文件
     */
    bool operator==(const ADocFile &rhs) const;

    /**
     * @brief 是否this管理的文件和指定文件路径的文件相等
     * @param filePath 文件路径
     * @return 是否是同一个文件
     */
    bool operator==(const QString &filePath) const;

Q_SIGNALS:
    /**
     * @brief 信号：文件保存成功
     */
    void saved();

    /**
     * @brief 信号：文件加载成功
     */
    void loaded();

protected:
    /**
     * @brief 关闭文件
     */
    void closeFile();

protected:
    /** @brief 文件 */
    QFile mFile;

    /** @brief 文件处理类型 */
    EHandleFileType mHandleType;
};

APROCH_NAMESPACE_END
