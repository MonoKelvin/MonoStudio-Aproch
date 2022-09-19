/****************************************************************************
 * @file    AAbstractDataManager.h
 * @date    2022-7-16
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
#include <QSet>

namespace aproch
{
    /**
     * @brief 抽象数据管理器。
     */
    class APROCH_API AAbstractDataManager : public QObject
    {
        Q_OBJECT
    private:
        friend class AData;
        Q_DISABLE_COPY_MOVE(AAbstractDataManager)

    public:
        explicit AAbstractDataManager(QObject *parent = nullptr);
        ~AAbstractDataManager();

        /**
         * @brief 获取数据集
         * @return 数据集合
         */
        QSet<AData *> dataSet() const;

        /**
         * @brief 清楚所有创建的数据（释放内存）
         */
        void clear() const;

        /**
         * @brief 从给定名称添加数据
         * @param name 数据名称
         * @return 返回创建好的数据
         */
        AData *addData(const QString &name = QString());

    Q_SIGNALS:
        void dataInserted(AData *data, AData *parent, AData *after);
        void dataChanged(AData *data);
        void dataRemoved(AData *data, AData *parent);
        void dataDestroyed(AData *data);

    protected:
        virtual bool hasValue(const AData *data) const;
        virtual QIcon valueIcon(const AData *data) const;
        virtual QString valueText(const AData *data) const;

        /**
         * @brief 当初始化创建数据时调用，目的是让数据管理器知道数据已创建
         * @param data 数据
         */
        virtual void initializeData(AData *data) = 0;

        /**
         * @brief 在销毁指定数据之前调用此函数。目的是让数据管理器知道正在销毁数据，以便删除数据的其他属性。
         * @param data 数据
         */
        virtual void uninitializeData(AData *data);

        /**
         * @brief 创建数据
         * @return 数据
         */
        virtual AData *createData();

    private:
        /**
         * @brief 销毁数据，删除内存
         * @param data 数据
         */
        void destroyData(AData *data);

    private:
        QSet<AData *> m_dataSet;
    };
}
