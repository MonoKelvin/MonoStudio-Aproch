/****************************************************************************
 * @file    AData.h
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
     * @brief 支持控件、对象属性绑定的数据
     * @note 数据由 AAbstractDataManager 的子类对象创建，管理器可以创建给定类型的数据，
     *       其子数据生命周期由创建它的管理器管理，不是其父对象的数据管理
     */
    class APROCH_API AData
    {
        friend class AAbstractDataManager;
    public:
        virtual ~AData();

        /**
         * @brief 获取创建该数据的数据管理器
         * @return 数据管理器
         */
        AAbstractDataManager *dataManager() const;

        /** @brief 获取数据相关参数 */

        QString getToolTip() const;
        QString getDescription() const;
        QString getDataName() const;
        bool isEnabled() const;
        bool isModified() const;

        /** @brief 设置数据相关参数 */

        void setToolTip(const QString &text);
        void setDescription(const QString &text);
        void setDataName(const QString &text);
        void setEnabled(bool enable);

        /** @brief 数据的文本形式 */
        QString toText() const;

        /**
         * @brief 获取子数据集
         * @return 子数据集
         */
        QList<AData *> subDataSet() const;
        void addSubData(AData *dt);
        void insertSubData(AData *dt, AData *afterDt);
        void removeSubData(AData *dt);

        /** @brief 是否有效 */
        virtual bool isValid() const;

    protected:
        //explicit AData(const AData& rhs, const FCopyOptions& op = A_Default_Copy_Option);
        explicit AData(AAbstractDataManager* manager);
        void dataChanged();

    private:
        /** @brief 父对象数据集 */
        QSet<AData *> m_parentItems;

        /** @brief 子对象数据集 */
        QList<AData *> m_subItems;

        /** @brief 管理该数据的数据管理器对象 */
        AAbstractDataManager* const m_manager;

        /** @brief 数据名称 */
        QString m_name;

        /** @brief 数据提示内容 */
        QString m_toolTip;

        /** @brief 数据描述内容 */
        QString m_description;

        /** @brief 数据是否可用 */
        bool m_enabled;
    };
}
