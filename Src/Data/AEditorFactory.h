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
    typedef QList<Editor *> EditorList;
    typedef QMap<AData *, EditorList> DataToEditorListMap;
    typedef QMap<Editor *, AData *> EditorToDataMap;

    Editor *createEditorImpl(AData *data, QWidget *parent);
    void initializeEditor(AData *data, Editor *e);
    void slotEditorDestroyed(QObject *object);

    DataToEditorListMap m_createdEditors;
    EditorToDataMap m_editorToData;
};

template <class Editor>
Editor *EditorFactoryHelper<Editor>::createEditorImpl(AData *data, QWidget *parent)
{
    Editor *editor = new Editor(parent);
    initializeEditor(data, editor);
    return editor;
}

template <class Editor>
void EditorFactoryHelper<Editor>::initializeEditor(AData *data, Editor *editor)
{
    typename DataToEditorListMap::iterator it = m_createdEditors.find(data);
    if (it == m_createdEditors.end())
        it = m_createdEditors.insert(data, EditorList());
    it.value().append(editor);
    m_editorToData.insert(editor, data);
}

template <class Editor>
void EditorFactoryHelper<Editor>::slotEditorDestroyed(QObject *object)
{
    const typename EditorToDataMap::iterator ecend = m_editorToData.end();
    for (typename EditorToDataMap::iterator itEditor = m_editorToData.begin(); itEditor != ecend; ++itEditor)
    {
        if (itEditor.key() == object)
        {
            Editor *editor = itEditor.key();
            AData *data = itEditor.value();
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
 * @brief 单行文本编辑器创建工厂
 */
class APROCH_API ALineEditFactory : public AAbstractEditorFactory<AStringDataManager>
{
    Q_OBJECT
public:
    explicit ALineEditFactory(QObject *parent = nullptr);
    ~ALineEditFactory();

protected:
    virtual void connectDataManager(AStringDataManager *manager) override;
    virtual QWidget *createEditorImpl(AStringDataManager *manager, AData *data, QWidget *parent) override;
    virtual void disconnectDataManager(AStringDataManager *manager) override;

private Q_SLOTS:
    void slotDataChanged(AData *data, const QString &value);
    void slotRegExpChanged(AData *data, const QRegExp &regExp);
    void slotSetValue(const QString &value);
    void slotEditorDestroyed(QObject *obj);

private:
    EditorFactoryHelper<QLineEdit> m_helper;

private:
    Q_DISABLE_COPY_MOVE(ALineEditFactory)
};

/**
 * @brief 整数输入编辑器创建工厂
 */
class APROCH_API ASpinBoxFactory : public AAbstractEditorFactory<AIntDataManager>
{
    Q_OBJECT
public:
    ASpinBoxFactory(QObject *parent = nullptr);
    ~ASpinBoxFactory();

protected:
    void connectDataManager(AIntDataManager *manager);
    QWidget *createEditorImpl(AIntDataManager *manager, AData *data, QWidget *parent);
    void disconnectDataManager(AIntDataManager *manager);

private Q_SLOTS:
    void slotDataChanged(AData *data, int value);
    void slotRangeChanged(AData *data, int min, int max);
    void slotSingleStepChanged(AData *data, int step);
    void slotSetValue(int value);
    void slotEditorDestroyed(QObject *obj);

private:
    EditorFactoryHelper<QSpinBox> m_helper;

private:
    Q_DISABLE_COPY_MOVE(ASpinBoxFactory)
};

/**
 * @brief 双精度浮点数输入编辑器创建工厂
 */
class APROCH_API ADoubleSpinBoxFactory : public AAbstractEditorFactory<ADoubleDataManager>
{
    Q_OBJECT
public:
    ADoubleSpinBoxFactory(QObject *parent = nullptr);
    ~ADoubleSpinBoxFactory();

protected:
    void connectDataManager(ADoubleDataManager *manager);
    QWidget *createEditorImpl(ADoubleDataManager *manager, AData *data, QWidget *parent);
    void disconnectDataManager(ADoubleDataManager *manager);

private Q_SLOTS:
    void slotDataChanged(AData *data, double value);
    void slotRangeChanged(AData *data, double min, double max);
    void slotSingleStepChanged(AData *data, double step);
    void slotDecimalsChanged(AData *data, int prec);
    void slotSetValue(double value);
    void slotEditorDestroyed(QObject *obj);

private:
    EditorFactoryHelper<QDoubleSpinBox> m_helper;

private:
    Q_DISABLE_COPY_MOVE(ADoubleSpinBoxFactory)
};

/**
 * @brief 滑动条编辑器创建工厂
 */
class APROCH_API ASliderFactory : public AAbstractEditorFactory<AIntDataManager>
{
    Q_OBJECT
public:
    ASliderFactory(QObject *parent = nullptr);
    ~ASliderFactory();

protected:
    void connectDataManager(AIntDataManager *manager);
    QWidget *createEditorImpl(AIntDataManager *manager, AData *data, QWidget *parent);
    void disconnectDataManager(AIntDataManager *manager);

private Q_SLOTS:
    void slotDataChanged(AData *data, int value);
    void slotRangeChanged(AData *data, int min, int max);
    void slotSingleStepChanged(AData *data, int step);
    void slotSetValue(int value);
    void slotEditorDestroyed(QObject *obj);

private:
    EditorFactoryHelper<QSlider> m_helper;

private:
    Q_DISABLE_COPY_MOVE(ASliderFactory)
};

/**
 * @brief 勾选框编辑器创建工厂
 */
class APROCH_API ACheckBoxFactory : public AAbstractEditorFactory<ABoolDataManager>
{
    Q_OBJECT
public:
    ACheckBoxFactory(QObject *parent = nullptr);
    ~ACheckBoxFactory();

protected:
    virtual void connectDataManager(ABoolDataManager *manager) override;
    virtual QWidget *createEditorImpl(ABoolDataManager *manager, AData *data, QWidget *parent) override;
    virtual void disconnectDataManager(ABoolDataManager *manager) override;

private Q_SLOTS:
    void slotDataChanged(AData *, bool);
    void slotSetValue(bool);
    void slotEditorDestroyed(QObject *);

private:
    EditorFactoryHelper<QCheckBox> m_helper;

private:
    Q_DISABLE_COPY_MOVE(ACheckBoxFactory)
};

/**
 * @brief 枚举编辑器创建工厂
 */
class APROCH_API AEnumEditorFactory : public AAbstractEditorFactory<AEnumDataManager>
{
    Q_OBJECT
public:
    AEnumEditorFactory(QObject *parent = nullptr);
    ~AEnumEditorFactory();

protected:
    void connectDataManager(AEnumDataManager *manager);
    QWidget *createEditorImpl(AEnumDataManager *manager, AData *data, QWidget *parent);
    void disconnectDataManager(AEnumDataManager *manager);

private Q_SLOTS:
    void slotDataChanged(AData *data, int val);
    void slotEnumNamesChanged(AData *data, const QStringList &enumNames);
    void slotEnumIconsChanged(AData *data, const QMap<int, QIcon> &enumIcons);
    void slotSetValue(int val);
    void slotEditorDestroyed(QObject *);

private:
    EditorFactoryHelper<QComboBox> m_helper;

private:
    Q_DISABLE_COPY_MOVE(AEnumEditorFactory)
};

/**
 * @brief 颜色编辑器创建工厂
 */
class APROCH_API AColorEditorFactory : public AAbstractEditorFactory<AColorDataManager>
{
    Q_OBJECT
public:
    AColorEditorFactory(QObject *parent = nullptr);
    ~AColorEditorFactory();

protected:
    void connectDataManager(AColorDataManager *manager);
    QWidget *createEditorImpl(AColorDataManager *manager, AData *data, QWidget *parent);
    void disconnectDataManager(AColorDataManager *manager);

private Q_SLOTS:
    void slotDataChanged(AData *data, const QColor &value);
    void slotSetValue(const QColor &value);
    void slotEditorDestroyed(QObject *obj);

private:
    EditorFactoryHelper<QLabel> m_helper;

private:
    Q_DISABLE_COPY_MOVE(AColorEditorFactory)
};

/**
 * @brief 字体编辑器创建工厂
 */
class APROCH_API AFontEditorFactory : public AAbstractEditorFactory<AFontDataManager>
{
    Q_OBJECT
public:
    AFontEditorFactory(QObject *parent = nullptr);
    ~AFontEditorFactory();

protected:
    void connectDataManager(AFontDataManager *manager);
    QWidget *createEditorImpl(AFontDataManager *manager, AData *data, QWidget *parent);
    void disconnectDataManager(AFontDataManager *manager);

private Q_SLOTS:
    void slotDataChanged(AData *data, const QFont &value);
    void slotSetValue(const QFont &value);
    void slotEditorDestroyed(QObject *obj);

private:
    EditorFactoryHelper<QLabel> m_helper; // todo: change widget

private:
    Q_DISABLE_COPY_MOVE(AFontEditorFactory)
};

APROCH_NAMESPACE_END
