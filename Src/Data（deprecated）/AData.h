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

APROCH_NAMESPACE_BEGIN

class AData;
using EMetaType = QMetaType::Type;
using ADataSet = QSet<AData*>;

/**
 * @brief 支持控件、对象属性绑定的数据
 * @note 数据由 AAbstractDataManager 的子类对象创建，管理器可以创建给定类型的数据，
 *       其子数据生命周期由创建它的管理器管理，不是其父对象的数据管理
 */
class APROCH_API AData : public APrototype
{
    friend class AAbstractDataManager;
public:
    virtual ~AData();

    /**
     * @brief 获取创建该数据的数据管理器
     * @return 数据管理器
     */
    AAbstractDataManager* getDataManager() const;

    /** @brief 克隆数据，并将数据加入到当前数据所在的数据管理器中 */
    virtual APrototype* clone(const FCopyOptions& op = DeepCopy) const override;

    const QVariant& getValue() noexcept;
    QVariant getValue() const noexcept;
    bool setValue(const QVariant& val);
    bool resetValue();

    EMetaType getType() const noexcept;

    QString getName() const;
    void setName(const QString& text);

    QString getDescription() const;
    void setDescription(const QString& text);

    /** @brief 数据是否有效 */
    virtual bool isValid() const override;

    /** @brief 数据的文本形式 */
    QString toString() const;

    ADataSet getParentDataSet() const;
    const ADataSet& getParentDataSetRef() const;
    QList<AData*> getSubDataSet() const;
    const QList<AData*>& getSubDataSetRef() const;
    void addSubData(AData* dt);

    /** @brief 插入子数据，afterDt为空则在子列表首部插入 */
    void insertSubData(AData* dt, AData* afterDt);
    void insertSubData(AData* dt, int index = -1);
    void removeSubData(AData* dt);

protected:
    AData(AAbstractDataManager* manager);
    explicit AData(const AData& rhs, const FCopyOptions& op = A_DEFAULT_COPY_OPTION);

    /** @brief 数据修改 */
    void dataChanged();

private:
    void setValueInternal(const QVariant& data);

private:
    /** @brief 父对象数据集 */
    ADataSet m_parentItems;

    /** @brief 子对象数据集 */
    QList<AData*> m_subItems;

    /** @brief 管理该数据的数据管理器对象 */
    AAbstractDataManager* const m_manager;

    /** @brief 数据内容 */
    QVariant m_value;

    /** @brief 数据名称 */
    QString m_name;

    /** @brief 数据描述内容 */
    QString m_description;
};

APROCH_NAMESPACE_END