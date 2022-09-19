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
     * 数据由 AAbstractDataManager 的子类对象创建，管理器可以创建给定类型的数据，其子数据生命周期由创建它的管理器管理，不是其父对象的数据管理
     * @note 代码来源于QtDesigner中的 QtProperty
     */
    class APROCH_API AData
    {
    private:
        friend class AAbstractDataManager;

    public:
        virtual ~AData();

        /**
         * @brief 获取创建该数据的数据管理器
         * @return 数据管理器
         */
        AAbstractDataManager *dataManager() const;

        /** @brief 获取数据相关参数 */

        QString toolTip() const;
        QString descriptionToolTip() const;
        QString statusTip() const;
        QString whatsThis() const;
        QString dataName() const;
        bool isEnabled() const;
        bool isModified() const;

        bool hasValue() const;
        QIcon valueIcon() const;
        QString valueText() const;

        /** @brief 设置数据相关参数 */

        void setToolTip(const QString &text);
        void setDescriptionToolTip(const QString &text);
        void setStatusTip(const QString &text);
        void setWhatsThis(const QString &text);
        void setDataName(const QString &text);
        void setEnabled(bool enable);
        void setModified(bool modified);

        /**
         * @brief 获取子数据集
         * @return 子数据集
         */
        QList<AData *> subDataSet() const;

        /** @brief 子数据 */

        void addSubData(AData *dt);
        void insertSubData(AData *dt, AData *afterDt);
        void removeSubData(AData *dt);

    protected:
        explicit AData(AAbstractDataManager *manager);
        void dataChanged();

    private:
        QSet<AData *> m_parentItems;
        QList<AData *> m_subItems;

        QString m_valueToolTip;
        QString m_descriptionToolTip;
        QString m_statusTip;
        QString m_whatsThis;
        QString m_name;
        bool m_enabled;
        bool m_modified;

        AAbstractDataManager *const m_manager;
    };
}
