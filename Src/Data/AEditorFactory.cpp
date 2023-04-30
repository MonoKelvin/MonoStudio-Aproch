/****************************************************************************
 * @file    AEditorFactory.cpp
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
#include "AEditorFactory.h"

APROCH_NAMESPACE_BEGIN

void ASpinBoxFactory::slotDataChanged(AData* data, int value)
{
    const auto it = m_helper.m_createdEditors.constFind(data);
    if (it == m_helper.m_createdEditors.cend())
        return;
    for (QSpinBox* editor : it.value())
    {
        if (editor->value() != value)
        {
            editor->blockSignals(true);
            editor->setValue(value);
            editor->blockSignals(false);
        }
    }
}

void ASpinBoxFactory::slotSetValue(int value)
{
    QObject* object = sender();
    const QMap<QSpinBox*, AData*>::ConstIterator ecend = m_helper.m_editorToData.constEnd();
    for (QMap<QSpinBox*, AData*>::ConstIterator itEditor = m_helper.m_editorToData.constBegin(); itEditor != ecend; ++itEditor)
    {
        if (itEditor.key() == object)
        {
            AData* data = itEditor.value();
            AIntegerDataManager* manager = dataManager(data);
            if (!manager)
                return;
            manager->setValue(data, value);
            return;
        }
    }
}

ASpinBoxFactory::ASpinBoxFactory(QObject* parent)
    : AAbstractEditorFactory<AIntegerDataManager>(parent)
{
}

ASpinBoxFactory::~ASpinBoxFactory()
{
    qDeleteAll(m_helper.m_editorToData.keys());
}

void ASpinBoxFactory::connectDataManager(AIntegerDataManager* manager)
{
    connect(manager, SIGNAL(valueChanged(AData*, int)), this, SLOT(slotDataChanged(AData*, int)));
}

QWidget* ASpinBoxFactory::createEditorImpl(AIntegerDataManager* manager, AData* data, QWidget* parent)
{
    QSpinBox* editor = m_helper.createEditorImpl(data, parent);
    editor->setValue(data->getValue().toInt());
    editor->setKeyboardTracking(false);

    connect(editor, SIGNAL(valueChanged(int)), this, SLOT(slotSetValue(int)));
    connect(editor, SIGNAL(destroyed(QObject*)), this, SLOT(slotEditorDestroyed(QObject*)));
    return editor;
}

void ASpinBoxFactory::disconnectDataManager(AIntegerDataManager* manager)
{
    disconnect(manager, SIGNAL(valueChanged(AData*, int)), this, SLOT(slotDataChanged(AData*, int)));
}

void ASpinBoxFactory::slotEditorDestroyed(QObject* obj)
{
    m_helper.slotEditorDestroyed(obj);
}

APROCH_NAMESPACE_END
