/****************************************************************************
 * @file    AEditorFactory.h
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
#include "AAbstractEditorFactory.h"
#include "ADataManager.h"

#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>

APROCH_NAMESPACE_BEGIN

template <class Editor>
class EditorFactoryHelper
{
public:
    typedef QList<Editor*> EditorList;
    typedef QMap<AData*, EditorList> DataToEditorListMap;
    typedef QMap<Editor*, AData*> EditorToDataMap;

    Editor* createEditorImpl(AData* data, QWidget* parent);
    void initializeEditor(AData* data, Editor* e);
    void slotEditorDestroyed(QObject* object);

    DataToEditorListMap m_createdEditors;
    EditorToDataMap m_editorToData;
};

template <class Editor>
Editor* EditorFactoryHelper<Editor>::createEditorImpl(AData* data, QWidget* parent)
{
    Editor* editor = new Editor(parent);
    initializeEditor(data, editor);
    return editor;
}

template <class Editor>
void EditorFactoryHelper<Editor>::initializeEditor(AData* data, Editor* editor)
{
    typename DataToEditorListMap::iterator it = m_createdEditors.find(data);
    if (it == m_createdEditors.end())
        it = m_createdEditors.insert(data, EditorList());
    it.value().append(editor);
    m_editorToData.insert(editor, data);
}

template <class Editor>
void EditorFactoryHelper<Editor>::slotEditorDestroyed(QObject* object)
{
    const typename EditorToDataMap::iterator ecend = m_editorToData.end();
    for (typename EditorToDataMap::iterator itEditor = m_editorToData.begin(); itEditor != ecend; ++itEditor)
    {
        if (itEditor.key() == object)
        {
            Editor* editor = itEditor.key();
            AData* data = itEditor.value();
            const typename DataToEditorListMap::iterator pit = m_createdEditors.find(data);
            if (pit != m_createdEditors.end())
            {
                pit.value().removeAll(editor);
                if (pit.value().empty())
                    m_createdEditors.erase(pit);
            }
            m_editorToData.erase(itEditor);
            return;
        }
    }
}

/**
 * @brief 整数输入编辑器创建工厂
 */
class APROCH_API ASpinBoxFactory : public AAbstractEditorFactory<AIntegerDataManager>
{
    Q_OBJECT
public:
    ASpinBoxFactory(QObject* parent = nullptr);
    ~ASpinBoxFactory();

protected:
    void connectDataManager(AIntegerDataManager* manager) override;
    QWidget* createEditorImpl(AIntegerDataManager* manager, AData* data, QWidget* parent) override;
    void disconnectDataManager(AIntegerDataManager* manager) override;

private Q_SLOTS:
    void slotDataChanged(AData* data, int value);
    void slotSetValue(int value);
    void slotEditorDestroyed(QObject* obj);

private:
    EditorFactoryHelper<QSpinBox> m_helper;

private:
    Q_DISABLE_COPY_MOVE(ASpinBoxFactory)
};

APROCH_NAMESPACE_END
