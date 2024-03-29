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

APROCH_NAMESPACE_BEGIN

class AAbstractDataManager;
class ADataPrivate;

/**
 * @brief 支持控件、对象属性绑定的数据
 * @note 数据由 AAbstractDataManager 的子类对象创建，管理器可以创建给定类型的数据，
 *       其子数据生命周期由创建它的管理器管理，不是其父对象的数据管理
 */
class APROCH_API AData
{
public:
    virtual ~AData();

    QList<AData*> subDataList() const;

    AAbstractDataManager* getDataManager() const;

    QString getToolTip() const;
    QString getDescription() const;
    QString getName() const;
    bool isEnabled() const;
    bool isModified() const;

    bool hasValue() const;
    QIcon valueIcon() const;
    QString toString() const;

    void setToolTip(const QString& text);
    void setDescription(const QString& text);
    void setName(const QString& text);
    void setEnabled(bool enable);
    void setModified(bool modified);

    void addSubData(AData* data);
    void insertSubData(AData* data, AData* afterData);
    void removeSubData(AData* data);

protected:
    explicit AData(AAbstractDataManager* manager);
    void dataChanged();

private:
    friend class AAbstractDataManager;
    QScopedPointer<ADataPrivate> d_ptr;
};
APROCH_NAMESPACE_END