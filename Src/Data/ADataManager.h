/****************************************************************************
 * @file    ADataManager.h
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
#include "AAbstractDataManager.h"

#include <QString>

APROCH_NAMESPACE_BEGIN

#define A_DECLARE_SIMPLE_DATAMANAGER(_ClassName_, _ValueType_, _MetaType_) \
class APROCH_API _ClassName_ : public AAbstractDataManager \
{ \
    Q_OBJECT \
public: \
    _ClassName_(ADataContainer* parent) : AAbstractDataManager(parent, _MetaType_) { } \
    virtual QString toText(const AData* dt) const override \
    { \
        if (!dt) \
            return QString(); \
        return AStr("%1").arg(dt->getValue().value<_ValueType_>()); \
    } \
}

A_DECLARE_SIMPLE_DATAMANAGER(ABoolDataManager, bool, EMetaType::Bool);
A_DECLARE_SIMPLE_DATAMANAGER(ACharDataManager, char, EMetaType::Char);
A_DECLARE_SIMPLE_DATAMANAGER(AUCharDataManager, uchar, EMetaType::UChar);
A_DECLARE_SIMPLE_DATAMANAGER(AShortDataManager, short, EMetaType::Short);
A_DECLARE_SIMPLE_DATAMANAGER(AUShortDataManager, ushort, EMetaType::UShort);
A_DECLARE_SIMPLE_DATAMANAGER(AIntegerDataManager, int, EMetaType::Int);
A_DECLARE_SIMPLE_DATAMANAGER(AUIntegerDataManager, uint, EMetaType::UInt);
A_DECLARE_SIMPLE_DATAMANAGER(ALongDataManager, long, EMetaType::Long);
A_DECLARE_SIMPLE_DATAMANAGER(AULongDataManager, ulong, EMetaType::ULong);
A_DECLARE_SIMPLE_DATAMANAGER(ALongLongDataManager, qlonglong, EMetaType::LongLong);
A_DECLARE_SIMPLE_DATAMANAGER(AULongLongDataManager, qulonglong, EMetaType::ULongLong);
A_DECLARE_SIMPLE_DATAMANAGER(AFloatDataManager, float, EMetaType::Float);
A_DECLARE_SIMPLE_DATAMANAGER(ADoubleDataManager, double, EMetaType::Double);
A_DECLARE_SIMPLE_DATAMANAGER(AStringDataManager, QString, EMetaType::QString);

class APROCH_API ASizeDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    ASizeDataManager(ADataContainer* parent);
    ~ASizeDataManager();

    virtual QString toText(const AData* dt) const override;

protected:
    virtual void initializeData(AData* data) override;
};

APROCH_NAMESPACE_END