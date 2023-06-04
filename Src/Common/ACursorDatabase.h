/****************************************************************************
 * @file    ACursorDatabase.h
 * @date    2023-06-03
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

class ACursorDatabasePrivate;

/**
 * @brief 鼠标指针形状数据库
 */
class APROCH_API ACursorDatabase
{
public:
    ACursorDatabase();
    ~ACursorDatabase();

    /** @brief 清除所有鼠标指针 */
    void clear();

    /** @brief 获取所有鼠标指针名称 */
    QStringList getCursorShapeNames() const;

    /** @brief 获取鼠标指针图标表，模板参数一为索引 */
    QMap<int, QIcon> getCursorShapeIcons() const;

    /** @brief 给定鼠标指针获取其名称 */
    QString getCursorShapeName(const QCursor& cursor) const;

    /** @brief 给定鼠标指针获取其图标 */
    QIcon getCursorShapeIcon(const QCursor& cursor) const;

    /** @brief 给定鼠标指针获取其索引 */
    int getCursorIndex(const QCursor& cursor) const;

#ifndef QT_NO_CURSOR
    /** @brief 给定鼠标指针枚举类型获取对应鼠标指针数据 */
    QCursor getCursor(int value) const;
#endif

    /** @brief 添加鼠标指针，已有的数据将覆盖 */
    void appendCursor(Qt::CursorShape shape, const QString& name, const QIcon& icon);

private:
    Q_DISABLE_COPY(ACursorDatabase);
    QScopedPointer<ACursorDatabasePrivate> d_ptr;
};

APROCH_NAMESPACE_END
