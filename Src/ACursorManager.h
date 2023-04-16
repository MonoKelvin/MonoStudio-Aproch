/****************************************************************************
 * @file    ACursorManager.h
 * @date    2021-9-26
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

#include <QStack>
#include <QCursor>

APROCH_NAMESPACE_BEGIN

/**
 * @brief 鼠标指针管理器
 */
class APROCH_API ACursorManager
{
    APROCH_SINGLETON(ACursorManager);

public:
    /**
     * @brief 设置默认鼠标指针
     */
    void setDefault(const QCursor &cursor);

    /**
     * @brief 获取默认鼠标指针
     * @return 默认鼠标指针
     */
    QCursor getDefault(void) const;

    /**
     * @brief 设置当前鼠标指针
     * @param isNoPushIfSameShape 如果为true，则当前鼠标指针和上一次设置的鼠标指针形状 @see Qt::CursorShape 一样，就不记录到栈中
     * @param cursor
     */
    void setCurrent(const QCursor &cursor, bool isNoPushIfSameShape = true);

    /**
     * @brief 获取当前鼠标指针
     * @return 当前鼠标指针
     */
    QCursor getCurrent(void) const;

    /**
     * @brief 删除当前鼠标指针，返回到上一个鼠标指针并返回。如果没有通过@see setCurrent 设置上一个鼠标指针，则返回默认的@see getDefault
     * @return 上一个设置的鼠标指针
     */
    QCursor pop();

    /**
     * @brief 获取上一次的鼠标指针，不会删除当前的
     * @return 上一个设置的鼠标指针
     */
    QCursor getLast(void) const;

    /**
     * @brief 设置全局鼠标
     * @param cursor 鼠标指针
     */
    static void SetGlobalCursor(const QCursor &cursor);

    /**
     * @brief 还原到上一次的鼠标指针
     */
    static void RestoreGlobalCursor();

    /**
     * @brief 动态修改鼠标指针
     * @note 该方法一般放在频繁调用的函数内部，如鼠标移动、悬停移动等事件中。此外，需要确保condition可以覆盖到true和false两种情况，也可以
     * 在不同地方分别调用该方法，但同样要确保condition覆盖到true和false
     * @param condition 条件
     * @param cursor 当前鼠标
     * @example
     * 1. 单个状态切换
     *
     *      ACursorManager::DynamicCursor(condition, Qt::CrossCursor);
     *
     * condition需要覆盖true和false，为true时鼠标指针为十字光标，为false时还原上一次的图标
     *
     * 2. 在两个状态之间切换
     *
     *      ACursorManager::DynamicCursor(false);
     *      ACursorManager::DynamicCursor(true, condition ?  Qt::ForbiddenCursor : Qt::CrossCursor);
     *
     * 当condition为真时，鼠标指针变为<ForbiddenCursor>禁用图标，否则变为<CrossCursor>十字光标
     *
     */
    static void DynamicCursor(bool condition, const QCursor &cursor = QCursor());

private:
    Q_DISABLE_COPY_MOVE(ACursorManager)
    ACursorManager();

private:
    /** @brief 鼠标指针堆栈 */
    QStack<QCursor> mCursorStack;

    /** @brief 默认鼠标指针 */
    QCursor mDefaultCursor;
};

APROCH_NAMESPACE_END
