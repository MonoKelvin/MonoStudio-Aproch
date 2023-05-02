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

AData::AData(AAbstractDataManager* manager)
    : APrototype()
    , m_enabled(true)
    , m_manager(manager)
{
    Q_ASSERT_X(manager, Q_FUNC_INFO, "the manager is nullptr");
    m_value = QVariant(QVariant::Type(manager->getType()));
}

AData::AData(const AData& rhs, const FCopyOptions& op)
    : APrototype(rhs, op)
    , m_manager(rhs.m_manager)
    , m_name(rhs.m_name)
    , m_toolTip(rhs.m_toolTip)
    , m_description(rhs.m_description)
    , m_enabled(rhs.m_enabled)
{
    // 这里不处理父子级关系，而是由数据管理器构建
}

AData::~AData()
{
    if (m_manager->getDataContainer())
    {
        emit m_manager->getDataContainer()->dataDestroyed(this);

        for (AData* data : m_parentItems)
            emit data->m_manager->getDataContainer()->dataRemoved(this, data);
    }

    for (AData* data : m_subItems)
        data->m_parentItems.remove(this);

    for (AData* data : m_parentItems)
        data->m_subItems.removeAll(this);
}

ADataSet AData::getParentDataSet() const
{
    return m_parentItems;
}

QList<AData*> AData::getSubDataSet() const
{
    return m_subItems;
}

AAbstractDataManager* AData::getDataManager() const
{
    return m_manager;
}

APrototype* AData::clone(const FCopyOptions& op) const
{
    if (!m_manager)
        return nullptr;

    return m_manager->cloneData((AData*)(this));
}

const QVariant& AData::getValue() noexcept
{
    return m_value;
}

QVariant AData::getValue() const noexcept
{
    return m_value;
}

void AData::setValue(const QVariant& data)
{
    if (data == m_value)
        return;

    QVariant old = m_value;
    m_value = data;

    emit m_manager->getDataContainer()->valueChanged(this, old);
}

EMetaType AData::getType() const noexcept
{
    return EMetaType(m_value.type());
}

QString AData::getToolTip() const
{
    return m_toolTip;
}

QString AData::getDescription() const
{
    return m_description;
}

QString AData::getName() const
{
    return m_name;
}

bool AData::isEnabled() const
{
    return m_enabled;
}

QString AData::toText() const
{
    return m_manager->toText(this);
}

void AData::setToolTip(const QString& text)
{
    if (m_toolTip == text)
        return;

    m_toolTip = text;
    dataChanged();
}

void AData::setDescription(const QString& text)
{
    if (m_description == text)
        return;

    m_description = text;
    dataChanged();
}

void AData::setName(const QString& text)
{
    if (m_name == text)
        return;

    m_name = text;
    dataChanged();
}

void AData::setEnabled(bool enable)
{
    if (m_enabled == enable)
        return;

    m_enabled = enable;
    dataChanged();
}

void AData::addSubData(AData* data)
{
    AData* after = nullptr;

    if (m_subItems.count() > 0)
        after = m_subItems.last();

    insertSubData(data, after);
}

void AData::insertSubData(AData* data, AData* afterData)
{
    if (!data)
        return;

    if (data == this)
        return;

    // traverse all children of item. if this item is a child of item then cannot add.
    QList<AData*> pendingList = data->getSubDataSet();
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
        pendingList += i->getSubDataSet();
    }

    pendingList = getSubDataSet();
    int pos = 0;
    int newPos = 0;
    AData* properAfterData = 0;
    while (pos < pendingList.count())
    {
        AData* i = pendingList.at(pos);
        if (i == data)
            return; // if item is already inserted in this item then cannot add.
        if (i == afterData)
        {
            newPos = pos + 1;
            properAfterData = afterData;
        }
        pos++;
    }

    m_subItems.insert(newPos, data);
    data->m_parentItems.insert(this);

    emit m_manager->getDataContainer()->dataInserted(data, this, properAfterData);
}

void AData::insertSubData(AData* dt, int index)
{
    if (index > m_subItems.size())
        return;

    if (index > 0)
        insertSubData(dt, m_subItems.at(index - 1));
    else if (index == 0)
        insertSubData(dt, nullptr);
    else
        insertSubData(dt, m_subItems.back());
}

void AData::removeSubData(AData* data)
{
    if (!data)
        return;

    emit m_manager->getDataContainer()->dataRemoved(data, this);

    QList<AData*> pendingList = getSubDataSet();
    int pos = 0;
    while (pos < pendingList.count())
    {
        if (pendingList.at(pos) == data)
        {
            m_subItems.removeAt(pos);
            data->m_parentItems.remove(this);

            return;
        }
        pos++;
    }
}

void AData::dataChanged()
{
    emit m_manager->getDataContainer()->dataChanged(this);
}

bool AData::isValid() const
{
    return m_manager && m_value.isValid();
}

APROCH_NAMESPACE_END