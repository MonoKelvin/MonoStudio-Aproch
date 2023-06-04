/****************************************************************************
 * @file    AData.cpp
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
#include "stdafx.h"
#include "AData.h"
#include "AAbstractDataManager.h"

APROCH_NAMESPACE_BEGIN

class ADataPrivate
{
public:
    ADataPrivate(AAbstractDataManager* manager) 
        : m_enabled(true)
        , m_modified(false)
        , m_manager(manager)
    {
        Q_ASSERT_X(manager, Q_FUNC_INFO, "the data manager is null");
    }

    QSet<AData*> m_parentItems;
    QList<AData*> m_subItems;

    QString m_toolTip;
    QString m_description;
    QString m_name;
    bool m_enabled = true;
    bool m_modified = false;

    AAbstractDataManager* const m_manager;
};

AData::AData(AAbstractDataManager* manager)
    : d_ptr(new ADataPrivate(manager))
{
}

AData::~AData()
{
    for (AData* data : qAsConst(d_ptr->m_parentItems))
        emit data->d_ptr->m_manager->dataRemoved(this, data);

    emit d_ptr->m_manager->_destroyData(this);

    for (AData* data : qAsConst(d_ptr->m_subItems))
        data->d_ptr->m_parentItems.remove(this);

    for (AData* data : qAsConst(d_ptr->m_parentItems))
        data->d_ptr->m_subItems.removeAll(this);
}

QList<AData*> AData::subDataList() const
{
    return d_ptr->m_subItems;
}

AAbstractDataManager* AData::getDataManager() const
{
    return d_ptr->m_manager;
}

QString AData::getToolTip() const
{
    return d_ptr->m_toolTip;
}

QString AData::getDescription() const
{
    return d_ptr->m_description;
}

QString AData::getName() const
{
    return d_ptr->m_name;
}

bool AData::isEnabled() const
{
    return d_ptr->m_enabled;
}

bool AData::isModified() const
{
    return d_ptr->m_modified;
}

bool AData::hasValue() const
{
    return d_ptr->m_manager->hasValue(this);
}

QIcon AData::valueIcon() const
{
    return d_ptr->m_manager->valueIcon(this);
}

QString AData::toString() const
{
    return d_ptr->m_manager->toString(this);
}

void AData::setToolTip(const QString& text)
{
    if (d_ptr->m_toolTip == text)
        return;

    d_ptr->m_toolTip = text;
    dataChanged();
}

void AData::setDescription(const QString& text)
{
    if (d_ptr->m_description == text)
        return;

    d_ptr->m_description = text;
    dataChanged();
}

void AData::setName(const QString& text)
{
    if (d_ptr->m_name == text)
        return;

    d_ptr->m_name = text;
    dataChanged();
}

void AData::setEnabled(bool enable)
{
    if (d_ptr->m_enabled == enable)
        return;

    d_ptr->m_enabled = enable;
    dataChanged();
}

void AData::setModified(bool modified)
{
    if (d_ptr->m_modified == modified)
        return;

    d_ptr->m_modified = modified;
    dataChanged();
}

void AData::addSubData(AData* data)
{
    AData* after = 0;
    if (d_ptr->m_subItems.count() > 0)
        after = d_ptr->m_subItems.last();
    insertSubData(data, after);
}

void AData::insertSubData(AData* data, AData* afterProperty)
{
    if (!data)
        return;

    if (data == this)
        return;

    // traverse all children of item. if this item is a child of item then cannot add.
    QList<AData*> pendingList = data->subDataList();
    QMap<AData*, bool> visited;
    while (!pendingList.isEmpty())
    {
        AData* i = pendingList.first();
        if (i == this)
            return;
        pendingList.removeFirst();
        if (visited.contains(i))
            continue;
        visited[i] = true;
        pendingList += i->subDataList();
    }

    pendingList = subDataList();
    int pos = 0;
    int newPos = 0;
    AData* properAfterProperty = 0;
    while (pos < pendingList.count())
    {
        AData* i = pendingList.at(pos);
        if (i == data)
            return; // if item is already inserted in this item then cannot add.
        if (i == afterProperty)
        {
            newPos = pos + 1;
            properAfterProperty = afterProperty;
        }
        pos++;
    }

    d_ptr->m_subItems.insert(newPos, data);
    data->d_ptr->m_parentItems.insert(this);

    emit d_ptr->m_manager->dataInserted(data, this, properAfterProperty);
}

void AData::removeSubData(AData* data)
{
    if (!data)
        return;

    emit d_ptr->m_manager->dataRemoved(data, this);

    QList<AData*> pendingList = subDataList();
    int pos = 0;
    while (pos < pendingList.count())
    {
        if (pendingList.at(pos) == data)
        {
            d_ptr->m_subItems.removeAt(pos);
            data->d_ptr->m_parentItems.remove(this);

            return;
        }
        pos++;
    }
}

void AData::dataChanged()
{
    emit d_ptr->m_manager->dataChanged(this);
}

APROCH_NAMESPACE_END