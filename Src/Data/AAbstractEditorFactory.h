/****************************************************************************
 * @file    AAbstractEditorFactory.h
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

#include <QMap>
#include <QList>

APROCH_NAMESPACE_BEGIN

class AAbstractEditorFactoryBasePrivate;

/**
 * @brief 用于创建数据编辑控件的抽象工厂基类
 */
class APROCH_API AAbstractEditorFactoryBase : public QObject, public ADataWidgetBindMethod
{
    Q_OBJECT
public:
    explicit AAbstractEditorFactoryBase(ADataContainer* dc);
    ~AAbstractEditorFactoryBase();

    /** @brief 创建绑定给定数据的控件 */
    virtual QWidget *createEditor(AData *data, QWidget *parent, const SBindParameter& param = SBindParameter());

    /** @brief 获取数据容器 */
    ADataContainer* getDataContainer() const;

protected:
    virtual QWidget* createEditorImpl(AData* data, QWidget* parent) = 0;
    virtual void onEditorDestroyed(QObject* obj) = 0;

private Q_SLOTS:
    void editorDestroyed(QObject* obj);

private:
    Q_DISABLE_COPY_MOVE(AAbstractEditorFactoryBase);
    Q_DECLARE_PRIVATE(AAbstractEditorFactoryBase);
};

/**
 * @brief 用于创建数据编辑控件的抽象工厂类
 */
template <class Editor>
class AAbstractEditorFactory : public AAbstractEditorFactoryBase
{
public:
    explicit AAbstractEditorFactory(ADataContainer *dc)
        : AAbstractEditorFactoryBase(dc)
    {
    }

    typedef QList<Editor*> EditorList;
    typedef QMap<AData*, EditorList> DataToEditorListMap;
    typedef QMap<Editor*, AData*> EditorToDataMap;

    class EditorFactoryHelper
    {
    public:
        Editor* newEditor(AData* data, QWidget* parent)
        {
            Editor* editor = new Editor(parent);
            typename DataToEditorListMap::iterator it = m_createdEditors.find(data);
            if (it == m_createdEditors.end())
                it = m_createdEditors.insert(data, EditorList());
            it.value().append(editor);
            m_editorToData.insert(editor, data);
            return editor;
        }

        void initializeEditor(AData* data, Editor* editor)
        {
            typename DataToEditorListMap::iterator it = m_createdEditors.find(data);
            if (it == m_createdEditors.end())
                it = m_createdEditors.insert(data, EditorList());
            it.value().append(editor);
            m_editorToData.insert(editor, data);
        }

        void onEditorDestroyed(QObject* object)
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

        DataToEditorListMap m_createdEditors;
        EditorToDataMap m_editorToData;
    };

    /** @brief 获取数据-控件的绑定方法 */
    ADataWidgetBindMethod* getBindMethod() const
    {
        return ADataWidgetBinding::getBindMethod<Editor>();
    }

protected:
    virtual void onEditorDestroyed(QObject* obj) override
    {
        m_editorSet.onEditorDestroyed(obj);
    }

protected:
    virtual void valueChanged(AData* data, const QVariant& old)
    {
        ADataWidgetBindMethod* method = getBindMethod();
        if (!method)
            return;

        const auto it = m_editorSet.m_createdEditors.constFind(data);
        if (it == m_editorSet.m_createdEditors.cend())
            return;

        for (Editor* editor : it.value())
        {
            method->onValueChanged(qAsConst<AData*>(data), editor, old);
        }
    }

    virtual void editorValueChanged(const QVariant& val)
    {
        ADataWidgetBindMethod* method = getBindMethod();
        if (!method)
            return;

        QObject* obj = sender();
        auto ecend = m_editorSet.m_editorToData.constEnd();
        for (auto itEditor = m_editorSet.m_editorToData.constBegin(); itEditor != ecend; ++itEditor)
        {
            if (itEditor.key() == obj)
            {
                AData* data = itEditor.value();
                if (!data)
                    return;

                method->onWidgetValueChanged(data, qAsConst<QWidget*>(itEditor.key()));
                return;
            }
        }
    }

protected:
    EditorFactoryHelper m_editorSet;

private:
    Q_DISABLE_COPY_MOVE(AAbstractEditorFactory);
};

APROCH_NAMESPACE_END
