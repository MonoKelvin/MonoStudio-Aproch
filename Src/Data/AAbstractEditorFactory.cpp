/****************************************************************************
 * @file    AAbstractEditorFactory.cpp
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
#include "AAbstractEditorFactory.h"

#include <private/qobject_p.h>

APROCH_NAMESPACE_BEGIN

class AAbstractEditorFactoryBasePrivate : public QObjectPrivate
{
	Q_DECLARE_PUBLIC(AAbstractEditorFactoryBase);
public:
	AAbstractEditorFactoryBasePrivate(ADataContainer* _dc)
		: QObjectPrivate()
		, dc(_dc)
	{
	}

	ADataContainer* dc = nullptr;
};

// ----------------------------------------------------------------------------------------------------

AAbstractEditorFactoryBase::AAbstractEditorFactoryBase(ADataContainer* dc)
	: QObject(*(new AAbstractEditorFactoryBasePrivate(dc)), dc)
{
	Q_ASSERT_X(dc, Q_FUNC_INFO, "data container is nullptr");
}

AAbstractEditorFactoryBase::~AAbstractEditorFactoryBase()
{

}

QWidget* AAbstractEditorFactoryBase::createEditor(AData* data, QWidget* parent, const SBindParameter& param)
{
	Q_D(const AAbstractEditorFactoryBase);

    QWidget* widget = createEditorImpl(data, parent);
    if (!widget)
        return nullptr;

	bind(data, widget, param);
    connect(widget, &QWidget::destroyed, this, &AAbstractEditorFactoryBase::editorDestroyed);

    return widget;
}

ADataContainer* AAbstractEditorFactoryBase::getDataContainer() const
{
	Q_D(const AAbstractEditorFactoryBase);
	return d->dc;
}

void AAbstractEditorFactoryBase::editorDestroyed(QObject* obj)
{
	onEditorDestroyed(obj);
}

APROCH_NAMESPACE_END
