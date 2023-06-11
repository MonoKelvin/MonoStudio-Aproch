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

class AAbstractDataManagerPrivate;

/**
 * @brief 抽象数据管理器
 */
class APROCH_API AAbstractDataManager : public QObject
{
    Q_OBJECT
public:
    explicit AAbstractDataManager(QObject* parent = nullptr);
    ~AAbstractDataManager();

    /** some default attribute names */

    static const QString constraintAttribute;       // "constraint"
    static const QString enumIconsAttribute;        // "enumIcons"
    static const QString enumNamesAttribute;        // "enumNames"
    static const QString flagNamesAttribute;        // "flagNames"
    static const QString maximumAttribute;          // "maximum"
    static const QString minimumAttribute;          // "minimum"
    static const QString singleStepAttribute;       // "singleStep"
    static const QString decimalsAttribute;         // "decimals"
    static const QString regExpAttribute;           // "regExp"

    /** @brief 获取所有数据的列表 */
    QSet<AData*> getDataList() const;

    /** @brief 清空管理的数据 */
    void clear() const;

    /** @brief 添加数据 */
    AData* addData(const QString& name = QString());

    /** @brief 获取数据类型 @see QMetaType */
    virtual int getType() const = 0;

    /** @brief 获取数据值 */
    virtual QVariant getValue(AData* data) const = 0;

    /** @brief 获取数据中的指定属性的值 */
    virtual QVariant getAttribute(AData* data, const QString& attribute) const;

public Q_SLOTS:
    /** @brief 设置数据值 */
    virtual void setValue(AData* data, const QVariant& value) = 0;

    /** @brief 设置数据中的属性值 */
    virtual void setAttribute(AData* data, const QString& attribute, const QVariant& value);

Q_SIGNALS:
    /** @brief 信号：当添加子数据时。after为空时插入到parent第一个子节点前 */
    void dataInserted(AData* data, AData* parent, AData* after);

    /** @brief 信号：数据修改 */
    void dataChanged(AData* data);

    /** @brief 信号：数据从父级移除 */
    void dataRemoved(AData* data, AData* parent);

    /** @brief 信号：数据被析构前 */
    void dataDestroyed(AData* data);

    /** @brief 信号：数据值修改 */
    void valueChanged(AData* data, const QVariant& val);

    /** @brief 信号：数据中属性值修改 */
    void attributeChanged(AData* data, const QString& attribute, const QVariant& val);

protected:
    /** @brief 获取指定数据是否被管理，或是否存在真实数据 */
    virtual bool hasValue(const AData* data) const;

    /** @brief 获取指定数据对应的图标 */
    virtual QIcon valueIcon(const AData* data) const;

    /** @brief 获取数据的文本形式 */
    virtual QString toString(const AData* data) const;

    /** @brief 创建数据对象 */
    virtual AData* createData();

    /** @brief 当数据创建后并加入到管理器中后的立即初始化动作，@see createData,addData */
    virtual void initializeData(AData* data) = 0;

    /** @brief 析构数据前并不再被该管理器管理时的动作，@see dataDestroyed */
    virtual void uninitializeData(AData* data);

private:
    /** @brief 释放数据内存 */
    void _destroyData(AData* data);

private:
    friend class AData;
    QScopedPointer<AAbstractDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(AAbstractDataManager);
    Q_DISABLE_COPY_MOVE(AAbstractDataManager);
};

APROCH_NAMESPACE_END
