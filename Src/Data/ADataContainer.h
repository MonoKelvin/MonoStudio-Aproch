/****************************************************************************
 * @file    ADataContainer.h
 * @date    2023-05-14 
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

class AAbstractDataManager;
class ADataContainerPrivate;

typedef QList<AData*> ADataPath;
typedef QList<ADataPath> ADataPathList;

/**
 * @brief 数据容器
 */
class APROCH_API ADataContainer : public QObject
{
    Q_OBJECT
    friend class AAbstractDataManager;
public:
    explicit ADataContainer(QObject* parent = nullptr);
    explicit ADataContainer(const QList<AAbstractDataManager*>& dataMgrList, QObject* parent = nullptr);
    ~ADataContainer();

    /** @brief 添加数据管理器 */
    bool addDataManager(AAbstractDataManager* manager);

    template<class T>
    Q_ALWAYS_INLINE T* getDataManager(EMetaType type) const
    {
        return qobject_cast<T*>(getDataManager(type));
    }
    AAbstractDataManager* getDataManager(EMetaType type) const;

    /** @brief 设置数据的值 */
    bool setValue(AData* data, const QVariant& val);

    QVariant getDefaultValue(AData* dt) const;
    bool setDefaultValue(AData* dt, const QVariant& defaultVal);
    void resetValues(const ADataSet& dataSet);

    /** @brief 获取数据集，如果管理器器对应数据集不存在则创建 */
    ADataSet* getDataSet(AAbstractDataManager* manager, bool isCreateIfNull = true);
    ADataSet getAllData() const;

    /** @brief 添加指定类型的数据 */
    AData* addData(EMetaType type, const QString& name = QString());

    /** @brief 克隆数据，同时克隆所有子数据 */
    AData* cloneData(AData* srcData);

    /** @brief 删除数据集中的数据 */
    void deleteData(ADataSet& dataset);

    /**
     * @brief 获取给定数据的父子级路径.
     * 
     * @param data 数据
     * @param root 根数据。如果为空，则查找全路径，否则查找根节点为root的全部路径
     * @param isFindAll 是否查找全部路径，否则查找到一条则返回
     * @return 路径集合。存储顺序为父节点到子节点
     */
    static ADataPathList getDataPath(AData* data, AData* root = nullptr, bool isFindAll = true);

Q_SIGNALS:
    void dataInserted(AData* data, AData* parent, AData* after);
    void dataChanged(AData* data);
    void dataRemoved(AData* data, AData* parent);
    void dataDestroyed(AData* data);
    void defaultValueChanged(AData* data, const QVariant& oldValue);
    void valueChanged(AData* data, const QVariant& oldValue);

public Q_SLOTS:
    void resetValue(AData* dt);
    void deleteData(AData* dt);

private:
    /** @brief 添加数据管理器 */
    bool addDataManager(AAbstractDataManager* manager, EMetaType type);

    /** @brief 添加已有的数据 */
    bool addData(AAbstractDataManager* manager, AData* dt, const QVariant& defaultVal);

private:
    Q_DISABLE_COPY(ADataContainer);
    Q_DECLARE_PRIVATE(ADataContainer);
};

APROCH_NAMESPACE_END