/****************************************************************************
 * @file    AAbstractObjectCreator.cpp
 * @date    2022-07-09
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
#include "AAbstractObjectCreator.h"

namespace aproch
{
    QString AAbstractObjectCreator::name(void) const
    {
        QString strName;
        for (int i = 0; i < m_nameList.size(); ++i)
            strName += m_nameList.at(i) + ADPWService::nameSplitterChar;
        return strName;
    }

    bool AAbstractObjectCreator::hasName(const QString& name) const
    {
        for (int i = 1; i < m_nameList.size(); ++i)
        {
            if (0 == m_nameList.at(i).compare(name.trimmed(), Qt::CaseInsensitive))
                return true;
        }
        return false;
    }

    bool AAbstractObjectCreator::addName(const QString& name, const QString& ns)
    {
        const QString newNS = ns.trimmed();
        QString newName = name.trimmed().toLower();
        if (newName.isEmpty())
            return false;

        if (!newNS.isEmpty())
            newName = newNS + ":" + newName;

        if (m_nameList.contains(newName))
            return false;

        m_nameList.push_back(newName);
        return true;
    }

    void AAbstractObjectCreator::setProperty(QObject* obj, const char* propName, const AttrMap& attrs) const
    {
        if (obj->property(propName).isValid())
        {
            const auto& iter = attrs.find(propName);
            if (iter != attrs.end())
                obj->setProperty(propName, iter.value());
        }
    }
}
