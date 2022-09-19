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

namespace aproch
{
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

    void ASpinBoxFactory::slotRangeChanged(AData* data, int min, int max)
    {
        const auto it = m_helper.m_createdEditors.constFind(data);
        if (it == m_helper.m_createdEditors.cend())
            return;

        AIntDataManager* manager = dataManager(data);
        if (!manager)
            return;

        for (QSpinBox* editor : it.value())
        {
            editor->blockSignals(true);
            editor->setRange(min, max);
            editor->setValue(manager->value(data));
            editor->blockSignals(false);
        }
    }

    void ASpinBoxFactory::slotSingleStepChanged(AData* data, int step)
    {
        const auto it = m_helper.m_createdEditors.constFind(data);
        if (it == m_helper.m_createdEditors.cend())
            return;
        for (QSpinBox* editor : it.value())
        {
            editor->blockSignals(true);
            editor->setSingleStep(step);
            editor->blockSignals(false);
        }
    }

    void ASpinBoxFactory::slotSetValue(int value)
    {
        QObject* object = sender();
        const QMap<QSpinBox*, AData*>::ConstIterator  ecend = m_helper.m_editorToData.constEnd();
        for (QMap<QSpinBox*, AData*>::ConstIterator itEditor = m_helper.m_editorToData.constBegin(); itEditor != ecend; ++itEditor)
        {
            if (itEditor.key() == object)
            {
                AData* data = itEditor.value();
                AIntDataManager* manager = dataManager(data);
                if (!manager)
                    return;
                manager->setValue(data, value);
                return;
            }
        }
    }

    ASpinBoxFactory::ASpinBoxFactory(QObject* parent)
        : AAbstractEditorFactory<AIntDataManager>(parent)
    {
    }

    ASpinBoxFactory::~ASpinBoxFactory()
    {
        qDeleteAll(m_helper.m_editorToData.keys());
    }

    void ASpinBoxFactory::connectDataManager(AIntDataManager* manager)
    {
        connect(manager, SIGNAL(valueChanged(AData*, int)), this, SLOT(slotDataChanged(AData*, int)));
        connect(manager, SIGNAL(rangeChanged(AData*, int, int)), this, SLOT(slotRangeChanged(AData*, int, int)));
        connect(manager, SIGNAL(singleStepChanged(AData*, int)), this, SLOT(slotSingleStepChanged(AData*, int)));
    }

    QWidget* ASpinBoxFactory::createEditor(AIntDataManager* manager, AData* data, QWidget* parent)
    {
        QSpinBox* editor = m_helper.createEditor(data, parent);
        editor->setSingleStep(manager->singleStep(data));
        editor->setRange(manager->minimum(data), manager->maximum(data));
        editor->setValue(manager->value(data));
        editor->setKeyboardTracking(false);

        connect(editor, SIGNAL(valueChanged(int)), this, SLOT(slotSetValue(int)));
        connect(editor, SIGNAL(destroyed(QObject*)), this, SLOT(slotEditorDestroyed(QObject*)));
        return editor;
    }

    void ASpinBoxFactory::disconnectDataManager(AIntDataManager* manager)
    {
        disconnect(manager, SIGNAL(valueChanged(AData*, int)), this, SLOT(slotDataChanged(AData*, int)));
        disconnect(manager, SIGNAL(rangeChanged(AData*, int, int)), this, SLOT(slotRangeChanged(AData*, int, int)));
        disconnect(manager, SIGNAL(singleStepChanged(AData*, int)), this, SLOT(slotSingleStepChanged(AData*, int)));
    }

    void ASpinBoxFactory::slotEditorDestroyed(QObject* obj)
    {
        m_helper.slotEditorDestroyed(obj);
    }

    // ----------------------------------------------------------------------------------------------------

    void ADoubleSpinBoxFactory::slotDataChanged(AData* data, double value)
    {
        const auto it = m_helper.m_createdEditors.constFind(data);
        if (it == m_helper.m_createdEditors.cend())
            return;
        for (QDoubleSpinBox* editor : it.value())
        {
            if (editor->value() != value)
            {
                editor->blockSignals(true);
                editor->setValue(value);
                editor->blockSignals(false);
            }
        }
    }

    void ADoubleSpinBoxFactory::slotRangeChanged(AData* data, double min, double max)
    {
        const auto it = m_helper.m_createdEditors.constFind(data);
        if (it == m_helper.m_createdEditors.cend())
            return;

        ADoubleDataManager* manager = dataManager(data);
        if (!manager)
            return;

        for (QDoubleSpinBox* editor : it.value())
        {
            editor->blockSignals(true);
            editor->setRange(min, max);
            editor->setValue(manager->value(data));
            editor->blockSignals(false);
        }
    }

    void ADoubleSpinBoxFactory::slotSingleStepChanged(AData* data, double step)
    {
        const auto it = m_helper.m_createdEditors.constFind(data);
        if (it == m_helper.m_createdEditors.cend())
            return;

        ADoubleDataManager* manager = dataManager(data);
        if (!manager)
            return;

        for (QDoubleSpinBox* editor : it.value())
        {
            editor->blockSignals(true);
            editor->setSingleStep(step);
            editor->blockSignals(false);
        }
    }

    void ADoubleSpinBoxFactory::slotDecimalsChanged(AData* data, int prec)
    {
        const auto it = m_helper.m_createdEditors.constFind(data);
        if (it == m_helper.m_createdEditors.constEnd())
            return;

        ADoubleDataManager* manager = dataManager(data);
        if (!manager)
            return;

        for (QDoubleSpinBox* editor : it.value())
        {
            editor->blockSignals(true);
            editor->setDecimals(prec);
            editor->setValue(manager->value(data));
            editor->blockSignals(false);
        }
    }

    void ADoubleSpinBoxFactory::slotSetValue(double value)
    {
        QObject* object = sender();
        const QMap<QDoubleSpinBox*, AData*>::ConstIterator itcend = m_helper.m_editorToData.constEnd();
        for (QMap<QDoubleSpinBox*, AData*>::ConstIterator itEditor = m_helper.m_editorToData.constBegin(); itEditor != itcend; ++itEditor)
        {
            if (itEditor.key() == object)
            {
                AData* data = itEditor.value();
                ADoubleDataManager* manager = dataManager(data);
                if (!manager)
                    return;
                manager->setValue(data, value);
                return;
            }
        }
    }

    ADoubleSpinBoxFactory::ADoubleSpinBoxFactory(QObject* parent)
        : AAbstractEditorFactory<ADoubleDataManager>(parent)
    {
    }

    ADoubleSpinBoxFactory::~ADoubleSpinBoxFactory()
    {
        qDeleteAll(m_helper.m_editorToData.keys());
    }

    void ADoubleSpinBoxFactory::connectDataManager(ADoubleDataManager* manager)
    {
        connect(manager, SIGNAL(valueChanged(AData*, double)), this, SLOT(slotDataChanged(AData*, double)));
        connect(manager, SIGNAL(rangeChanged(AData*, double, double)), this, SLOT(slotRangeChanged(AData*, double, double)));
        connect(manager, SIGNAL(singleStepChanged(AData*, double)), this, SLOT(slotSingleStepChanged(AData*, double)));
        connect(manager, SIGNAL(decimalsChanged(AData*, int)), this, SLOT(slotDecimalsChanged(AData*, int)));
    }

    QWidget* ADoubleSpinBoxFactory::createEditor(ADoubleDataManager* manager, AData* data, QWidget* parent)
    {
        QDoubleSpinBox* editor = m_helper.createEditor(data, parent);
        editor->setSingleStep(manager->singleStep(data));
        editor->setDecimals(manager->decimals(data));
        editor->setRange(manager->minimum(data), manager->maximum(data));
        editor->setValue(manager->value(data));
        editor->setKeyboardTracking(false);

        connect(editor, SIGNAL(valueChanged(double)), this, SLOT(slotSetValue(double)));
        connect(editor, SIGNAL(destroyed(QObject*)), this, SLOT(slotEditorDestroyed(QObject*)));
        return editor;
    }

    void ADoubleSpinBoxFactory::disconnectDataManager(ADoubleDataManager* manager)
    {
        disconnect(manager, SIGNAL(valueChanged(AData*, double)), this, SLOT(slotDataChanged(AData*, double)));
        disconnect(manager, SIGNAL(rangeChanged(AData*, double, double)), this, SLOT(slotRangeChanged(AData*, double, double)));
        disconnect(manager, SIGNAL(singleStepChanged(AData*, double)), this, SLOT(slotSingleStepChanged(AData*, double)));
        disconnect(manager, SIGNAL(decimalsChanged(AData*, int)), this, SLOT(slotDecimalsChanged(AData*, int)));
    }

    void ADoubleSpinBoxFactory::slotEditorDestroyed(QObject* obj)
    {
        m_helper.slotEditorDestroyed(obj);
    }

    // ----------------------------------------------------------------------------------------------------

    void ASliderFactory::slotDataChanged(AData* data, int value)
    {
        const auto it = m_helper.m_createdEditors.constFind(data);
        if (it == m_helper.m_createdEditors.cend())
            return;
        for (QSlider* editor : it.value())
        {
            editor->blockSignals(true);
            editor->setValue(value);
            editor->blockSignals(false);
        }
    }

    void ASliderFactory::slotRangeChanged(AData* data, int min, int max)
    {
        const auto it = m_helper.m_createdEditors.constFind(data);
        if (it == m_helper.m_createdEditors.cend())
            return;

        AIntDataManager* manager = dataManager(data);
        if (!manager)
            return;

        for (QSlider* editor : it.value())
        {
            editor->blockSignals(true);
            editor->setRange(min, max);
            editor->setValue(manager->value(data));
            editor->blockSignals(false);
        }
    }

    void ASliderFactory::slotSingleStepChanged(AData* data, int step)
    {
        const auto it = m_helper.m_createdEditors.constFind(data);
        if (it == m_helper.m_createdEditors.cend())
            return;
        for (QSlider* editor : it.value())
        {
            editor->blockSignals(true);
            editor->setSingleStep(step);
            editor->blockSignals(false);
        }
    }

    void ASliderFactory::slotSetValue(int value)
    {
        QObject* object = sender();
        const QMap<QSlider*, AData*>::ConstIterator ecend = m_helper.m_editorToData.constEnd();
        for (QMap<QSlider*, AData*>::ConstIterator itEditor = m_helper.m_editorToData.constBegin(); itEditor != ecend; ++itEditor)
        {
            if (itEditor.key() == object)
            {
                AData* data = itEditor.value();
                AIntDataManager* manager = dataManager(data);
                if (!manager)
                    return;
                manager->setValue(data, value);
                return;
            }
        }
    }

    ASliderFactory::ASliderFactory(QObject* parent)
        : AAbstractEditorFactory<AIntDataManager>(parent)
    {
    }

    ASliderFactory::~ASliderFactory()
    {
        qDeleteAll(m_helper.m_editorToData.keys());
    }

    void ASliderFactory::connectDataManager(AIntDataManager* manager)
    {
        connect(manager, SIGNAL(valueChanged(AData*, int)), this, SLOT(slotDataChanged(AData*, int)));
        connect(manager, SIGNAL(rangeChanged(AData*, int, in t)), this, SLOT(slotRangeChanged(AData*, int, int)));
        connect(manager, SIGNAL(singleStepChanged(AData*, int)), this, SLOT(slotSingleStepChanged(AData*, int)));
    }

    QWidget* ASliderFactory::createEditor(AIntDataManager* manager, AData* data, QWidget* parent)
    {
        QSlider* editor = new QSlider(Qt::Horizontal, parent);
        m_helper.initializeEditor(data, editor);
        editor->setSingleStep(manager->singleStep(data));
        editor->setRange(manager->minimum(data), manager->maximum(data));
        editor->setValue(manager->value(data));

        connect(editor, SIGNAL(valueChanged(int)), this, SLOT(slotSetValue(int)));
        connect(editor, SIGNAL(destroyed(QObject*)), this, SLOT(slotEditorDestroyed(QObject*)));
        return editor;
    }

    void ASliderFactory::disconnectDataManager(AIntDataManager* manager)
    {
        disconnect(manager, SIGNAL(valueChanged(AData*, int)), this, SLOT(slotDataChanged(AData*, int)));
        disconnect(manager, SIGNAL(rangeChanged(AData*, int, int)), this, SLOT(slotRangeChanged(AData*, int, int)));
        disconnect(manager, SIGNAL(singleStepChanged(AData*, int)), this, SLOT(slotSingleStepChanged(AData*, int)));
    }

    void ASliderFactory::slotEditorDestroyed(QObject* obj)
    {
        m_helper.slotEditorDestroyed(obj);
    }

    // ----------------------------------------------------------------------------------------------------

    ALineEditFactory::ALineEditFactory(QObject* parent)
        : AAbstractEditorFactory<AStringDataManager>(parent)
    {
    }

    ALineEditFactory::~ALineEditFactory()
    {
        qDeleteAll(m_helper.m_editorToData.keys());
    }

    void ALineEditFactory::connectDataManager(AStringDataManager* manager)
    {
        connect(manager, SIGNAL(valueChanged(AData*, QString)), this, SLOT(slotDataChanged(AData*, QString)));
        connect(manager, SIGNAL(regExpChanged(AData*, QRegExp)), this, SLOT(slotRegExpChanged(AData*, QRegExp)));
    }

    QWidget* ALineEditFactory::createEditor(AStringDataManager* manager, AData* data, QWidget* parent)
    {
        QLineEdit* editor = m_helper.createEditor(data, parent);
        if (!editor)
            return nullptr;

        QRegExp regExp = manager->regExp(data);
        if (regExp.isValid())
        {
            QValidator* validator = new QRegExpValidator(regExp, editor);
            editor->setValidator(validator);
        }
        editor->setText(manager->value(data));

        connect(editor, SIGNAL(textEdited(QString)), this, SLOT(slotSetValue(QString)));
        connect(editor, SIGNAL(destroyed(QObject*)), this, SLOT(slotEditorDestroyed(QObject*)));
        return editor;
    }

    void ALineEditFactory::disconnectDataManager(AStringDataManager* manager)
    {
        disconnect(manager, SIGNAL(valueChanged(AData*, QString)), this, SLOT(slotDataChanged(AData*, QString)));
        disconnect(manager, SIGNAL(regExpChanged(AData*, QRegExp)), this, SLOT(slotRegExpChanged(AData*, QRegExp)));
    }

    void ALineEditFactory::slotDataChanged(AData* data, const QString& value)
    {
        const auto it = m_helper.m_createdEditors.constFind(data);
        if (it == m_helper.m_createdEditors.constEnd())
            return;

        for (QLineEdit* editor : it.value())
        {
            if (editor->text() != value)
                editor->setText(value);
        }
    }

    void ALineEditFactory::slotRegExpChanged(AData* data, const QRegExp& regExp)
    {
        const auto it = m_helper.m_createdEditors.constFind(data);
        if (it == m_helper.m_createdEditors.constEnd())
            return;

        AStringDataManager* manager = dataManager(data);
        if (!manager)
            return;

        for (QLineEdit* editor : it.value())
        {
            editor->blockSignals(true);
            const QValidator* oldValidator = editor->validator();
            QValidator* newValidator = 0;
            if (regExp.isValid())
            {
                newValidator = new QRegExpValidator(regExp, editor);
            }
            editor->setValidator(newValidator);
            if (oldValidator)
                delete oldValidator;
            editor->blockSignals(false);
        }
    }

    void ALineEditFactory::slotSetValue(const QString& value)
    {
        QObject* object = sender();
        const QMap<QLineEdit*, AData*>::ConstIterator ecend = m_helper.m_editorToData.constEnd();
        for (QMap<QLineEdit*, AData*>::ConstIterator itEditor = m_helper.m_editorToData.constBegin(); itEditor != ecend; ++itEditor)
        {
            if (itEditor.key() == object)
            {
                AData* data = itEditor.value();
                AStringDataManager* manager = dataManager(data);
                if (!manager)
                    return;
                manager->setValue(data, value);
                return;
            }
        }
    }

    void ALineEditFactory::slotEditorDestroyed(QObject* obj)
    {
        m_helper.slotEditorDestroyed(obj);
    }

    // ----------------------------------------------------------------------------------------------------

    ACheckBoxFactory::ACheckBoxFactory(QObject* parent)
        : AAbstractEditorFactory<ABoolDataManager>(parent)
    {
    }

    ACheckBoxFactory::~ACheckBoxFactory()
    {
        qDeleteAll(m_helper.m_editorToData.keys());
    }

    void ACheckBoxFactory::connectDataManager(ABoolDataManager* manager)
    {
        connect(manager, SIGNAL(valueChanged(AData*, bool)), this, SLOT(slotDataChanged(AData*, bool)));
    }

    QWidget* ACheckBoxFactory::createEditor(ABoolDataManager* manager, AData* data, QWidget* parent)
    {
        QCheckBox* editor = m_helper.createEditor(data, parent);
        editor->setChecked(manager->value(data));

        connect(editor, SIGNAL(toggled(bool)), this, SLOT(slotSetValue(bool)));
        connect(editor, SIGNAL(destroyed(QObject*)), this, SLOT(slotEditorDestroyed(QObject*)));
        return editor;
    }

    void ACheckBoxFactory::disconnectDataManager(ABoolDataManager* manager)
    {
        disconnect(manager, SIGNAL(valueChanged(AData*, bool)), this, SLOT(slotDataChanged(AData*, bool)));
    }

    void ACheckBoxFactory::slotDataChanged(AData* data, bool val)
    {
        const auto it = m_helper.m_createdEditors.constFind(data);
        if (it == m_helper.m_createdEditors.cend())
            return;

        for (QCheckBox* editor : it.value())
        {
            editor->blockSignals(true);
            editor->setChecked(val);
            editor->blockSignals(false);
        }
    }

    void ACheckBoxFactory::slotSetValue(bool value)
    {
        QObject* object = sender();

        const auto ecend = m_helper.m_editorToData.constEnd();
        for (auto itEditor = m_helper.m_editorToData.constBegin(); itEditor != ecend; ++itEditor)
        {
            if (itEditor.key() == object)
            {
                AData* data = itEditor.value();
                ABoolDataManager* manager = dataManager(data);
                if (!manager)
                    return;
                manager->setValue(data, value);
                return;
            }
        }
    }

    void ACheckBoxFactory::slotEditorDestroyed(QObject* obj)
    {
        m_helper.slotEditorDestroyed(obj);
    }

    // ----------------------------------------------------------------------------------------------------

    AEnumEditorFactory::AEnumEditorFactory(QObject* parent)
        : AAbstractEditorFactory<AEnumDataManager>(parent)
    {
    }

    AEnumEditorFactory::~AEnumEditorFactory()
    {
        qDeleteAll(m_helper.m_editorToData.keys());
    }

    void AEnumEditorFactory::connectDataManager(AEnumDataManager* manager)
    {
        connect(manager, SIGNAL(valueChanged(AData*, int)), this, SLOT(slotDataChanged(AData*, int)));
        connect(manager, SIGNAL(enumNamesChanged(AData*, QStringList)), this, SLOT(slotEnumNamesChanged(AData*, QStringList)));
    }

    QWidget* AEnumEditorFactory::createEditor(AEnumDataManager* manager, AData* data, QWidget* parent)
    {
        QComboBox* editor = m_helper.createEditor(data, parent);
        editor->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
        editor->view()->setTextElideMode(Qt::ElideRight);

        QStringList enumNames = manager->enumNames(data);
        editor->addItems(enumNames);

        QMap<int, QIcon> enumIcons = manager->enumIcons(data);
        const int enumNamesCount = enumNames.count();
        for (int i = 0; i < enumNamesCount; i++)
            editor->setItemIcon(i, enumIcons.value(i));
        editor->setCurrentIndex(manager->value(data));

        connect(editor, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSetValue(int)));
        connect(editor, SIGNAL(destroyed(QObject*)), this, SLOT(slotEditorDestroyed(QObject*)));
        return editor;
    }

    void AEnumEditorFactory::disconnectDataManager(AEnumDataManager* manager)
    {
        disconnect(manager, SIGNAL(valueChanged(AData*, int)), this, SLOT(slotDataChanged(AData*, int)));
        disconnect(manager, SIGNAL(enumNamesChanged(AData*, QStringList)), this, SLOT(slotEnumNamesChanged(AData*, QStringList)));
    }

    void AEnumEditorFactory::slotDataChanged(AData* data, int value)
    {
        const auto it = m_helper.m_createdEditors.constFind(data);
        if (it == m_helper.m_createdEditors.constEnd())
            return;

        for (QComboBox* editor : it.value())
        {
            editor->blockSignals(true);
            editor->setCurrentIndex(value);
            editor->blockSignals(false);
        }
    }

    void AEnumEditorFactory::slotEnumNamesChanged(AData* data, const QStringList& enumNames)
    {
        const auto it = m_helper.m_createdEditors.constFind(data);
        if (it == m_helper.m_createdEditors.constEnd())
            return;

        AEnumDataManager* manager = dataManager(data);
        if (!manager)
            return;

        QMap<int, QIcon> enumIcons = manager->enumIcons(data);

        for (QComboBox* editor : it.value())
        {
            editor->blockSignals(true);
            editor->clear();
            editor->addItems(enumNames);
            const int nameCount = enumNames.count();
            for (int i = 0; i < nameCount; i++)
                editor->setItemIcon(i, enumIcons.value(i));
            editor->setCurrentIndex(manager->value(data));
            editor->blockSignals(false);
        }
    }

    void AEnumEditorFactory::slotEnumIconsChanged(AData* data, const QMap<int, QIcon>& enumIcons)
    {
        const auto it = m_helper.m_createdEditors.constFind(data);
        if (it == m_helper.m_createdEditors.constEnd())
            return;

        AEnumDataManager* manager = dataManager(data);
        if (!manager)
            return;

        const QStringList enumNames = manager->enumNames(data);
        for (QComboBox* editor : it.value())
        {
            editor->blockSignals(true);
            const int nameCount = enumNames.count();
            for (int i = 0; i < nameCount; i++)
                editor->setItemIcon(i, enumIcons.value(i));
            editor->setCurrentIndex(manager->value(data));
            editor->blockSignals(false);
        }
    }

    void AEnumEditorFactory::slotSetValue(int value)
    {
        QObject* object = sender();
        const auto ecend = m_helper.m_editorToData.constEnd();
        for (auto itEditor = m_helper.m_editorToData.constBegin(); itEditor != ecend; ++itEditor)
        {
            if (itEditor.key() == object)
            {
                AData* data = itEditor.value();
                AEnumDataManager* manager = dataManager(data);
                if (!manager)
                    return;
                manager->setValue(data, value);
                return;
            }
        }
    }

    void AEnumEditorFactory::slotEditorDestroyed(QObject* obj)
    {
        m_helper.slotEditorDestroyed(obj);
    }

    // ----------------------------------------------------------------------------------------------------

    void AColorEditorFactory::slotDataChanged(AData* data, const QColor& value)
    {
        const auto it = m_helper.m_createdEditors.constFind(data);
        if (it == m_helper.m_createdEditors.constEnd())
            return;

        //for (AColorEditWidget* e : it.value()) // todo
            //e->setValue(value);
        for (QLabel* e : it.value())
            e->setText(value.name());
    }

    void AColorEditorFactory::slotSetValue(const QColor& value)
    {
        QObject* object = sender();
        const auto ecend = m_helper.m_editorToData.constEnd();
        for (auto itEditor = m_helper.m_editorToData.constBegin(); itEditor != ecend; ++itEditor)
        {
            if (itEditor.key() == object)
            {
                AData* data = itEditor.value();
                AColorDataManager* manager = dataManager(data);
                if (!manager)
                    return;
                manager->setValue(data, value);
                return;
            }
        }
    }

    AColorEditorFactory::AColorEditorFactory(QObject* parent)
        : AAbstractEditorFactory<AColorDataManager>(parent)
    {
    }

    AColorEditorFactory::~AColorEditorFactory()
    {
        qDeleteAll(m_helper.m_editorToData.keys());
    }

    void AColorEditorFactory::connectDataManager(AColorDataManager* manager)
    {
        connect(manager, SIGNAL(valueChanged(AData*, QColor)), this, SLOT(slotDataChanged(AData*, QColor)));
    }

    QWidget* AColorEditorFactory::createEditor(AColorDataManager* manager, AData* data, QWidget* parent)
    {
        //AColorEditWidget* editor = createEditor(data, parent); // todo
        //editor->setValue(manager->value(data));
        //connect(editor, SIGNAL(valueChanged(QColor)), this, SLOT(slotSetValue(QColor))); // todo
        QLabel* editor = m_helper.createEditor(data, parent);
        editor->setText(manager->value(data).name());
        connect(editor, SIGNAL(destroyed(QObject*)), this, SLOT(slotEditorDestroyed(QObject*)));
        return editor;
    }

    void AColorEditorFactory::disconnectDataManager(AColorDataManager* manager)
    {
        disconnect(manager, SIGNAL(valueChanged(AData*, QColor)), this, SLOT(slotDataChanged(AData*, QColor)));
    }

    void AColorEditorFactory::slotEditorDestroyed(QObject* obj)
    {
        m_helper.slotEditorDestroyed(obj);
    }

    // ----------------------------------------------------------------------------------------------------

    void AFontEditorFactory::slotDataChanged(AData* data, const QFont& value)
    {
        const auto it = m_helper.m_createdEditors.constFind(data);
        if (it == m_helper.m_createdEditors.constEnd())
            return;

        /*for (AFontEditWidget* e : it.value()) // todo
            e->setValue(value);*/
        for (QLabel* e : it.value())
            e->setText(value.family());
    }

    void AFontEditorFactory::slotSetValue(const QFont& value)
    {
        QObject* object = sender();
        const auto ecend = m_helper.m_editorToData.constEnd();
        for (auto itEditor = m_helper.m_editorToData.constBegin(); itEditor != ecend; ++itEditor)
        {
            if (itEditor.key() == object)
            {
                AData* data = itEditor.value();
                AFontDataManager* manager = dataManager(data);
                if (!manager)
                    return;
                manager->setValue(data, value);
                return;
            }
        }
    }

    AFontEditorFactory::AFontEditorFactory(QObject* parent)
        : AAbstractEditorFactory<AFontDataManager>(parent)
    {
    }

    AFontEditorFactory::~AFontEditorFactory()
    {
        qDeleteAll(m_helper.m_editorToData.keys());
    }

    void AFontEditorFactory::connectDataManager(AFontDataManager* manager)
    {
        connect(manager, SIGNAL(valueChanged(AData*, QFont)), this, SLOT(slotDataChanged(AData*, QFont)));
    }

    QWidget* AFontEditorFactory::createEditor(AFontDataManager* manager, AData* data, QWidget* parent)
    {
        /*AFontEditWidget* editor = createEditor(data, parent);
        editor->setValue(manager->value(data));
        connect(editor, SIGNAL(valueChanged(QFont)), this, SLOT(slotSetValue(QFont)));*/ // todo

        QLabel* editor = m_helper.createEditor(data, parent);
        editor->setText(ADPWUtils::FontText(manager->value(data)));
        connect(editor, SIGNAL(destroyed(QObject*)), this, SLOT(slotEditorDestroyed(QObject*)));
        return editor;
    }

    void AFontEditorFactory::disconnectDataManager(AFontDataManager* manager)
    {
        disconnect(manager, SIGNAL(valueChanged(AData*, QFont)), this, SLOT(slotDataChanged(AData*, QFont))); // todo
    }

    void AFontEditorFactory::slotEditorDestroyed(QObject* obj)
    {
        m_helper.slotEditorDestroyed(obj);
    }
}