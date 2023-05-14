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
#include "AData.h"

APROCH_NAMESPACE_BEGIN

class ADataContainer;
class AAbstractDataManagerPrivate;

/**
 * @brief 抽象数据管理器。
 */
class APROCH_API AAbstractDataManager : public QObject
{
    Q_OBJECT
private:
    friend class AData;
    friend class ADataContainer;
    friend class ADataContainerPrivate;
    Q_DISABLE_COPY_MOVE(AAbstractDataManager);
    Q_DECLARE_PRIVATE(AAbstractDataManager);

public:
    AAbstractDataManager(EMetaType type, ADataContainer* parent = nullptr);
    ~AAbstractDataManager();

    /** @brief 所管理的元数据类型 */
    EMetaType getType() const;

    /**
     * @brief 复制数据
     * @note 复制的数据没有指定父级对象；复制时不拷贝子集；复制后的对象没有进行数据绑定
     * @param srcData 源数据
     * @return 新数据
     */
    virtual AData* cloneData(AData* srcData);

    /** @brief 将数据转换为文本 */
    virtual QString toString(const AData* dt) const = 0;

    /** @brief 获取默认数据 */
    virtual QVariant getDefaultValue(AData* dt) const;

    /** @brief 重置为默认数据 */
    void resetValue(AData* dt);

    /** @brief 设置数据 */
    virtual bool setValue(AData* dt, const QVariant& val);

    /** @brief 获取数据集 */
    ADataSet* getDataSet() const;

    /** @brief 获取数据集 */
    const ADataSet& getDataSet();

    /** @brief 获取数据集，如果不存在则创建 */
    ADataSet* getOrCreateDataSet();

    /** @brief 获取管理器所在数据容器 */
    ADataContainer* getDataContainer() const;

    /** @brief 是否有效 */
    bool isValid() const;

    /** @brief 删除所有创建的数据（释放内存） */
    void clear();

    /** @brief 从给定名称添加数据 */
    AData* addData(const QString& name = QString());

    /**
     * @brief 从给定名称添加数据。
     * @param name 名称
     * @param defaultValue 默认值，数据类型必须和所管理的数据类型一致
     * @return 数据
     * @see type()
     */
    AData* addData(const QString& name, const QVariant& defaultValue);

protected:
    /** @brief 当重新设置数据容器时的初始化 */
    virtual void initialize(ADataContainer* dc);

    /** @brief 当初始化创建数据时调用，目的是让数据管理器知道数据已创建 */
    virtual void initializeData(AData* data);

    /**
     * @brief 创建数据。注意创建的数据类型如果不是所管理的数据类型，则无法添加成功.
     * @return 新数据
     * @see type(), addData()
     */
    virtual AData* createData();

private:
    /** @brief 初始化数据 */
    void init(ADataSet*);

    /** @brief 设置所管理的元数据类型 */
    void setType(EMetaType type);
};

#ifndef QT_NO_DEBUG_STREAM
APROCH_API QDebug operator<<(QDebug dbg, const AAbstractDataManager&);
#endif

APROCH_NAMESPACE_END