/****************************************************************************
 * @file    AVariantData.h
 * @date    2022-7-20
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
#include "AData.h"
#include <QIcon>

typedef QMap<int, QIcon> AIconMap;

namespace aproch
{
    /**
     * @brief 变量数据
     */
    class APROCH_API AVariantData : public AData
    {
    private:
        friend class AVariantDataManager;

    public:
        ~AVariantData();

        QVariant value() const;
        QVariant subValue(const QString& valName) const;

        int valueType() const;
        int dataType() const;

        void setValue(const QVariant& value);
        void setSubValue(const QString& valName, const QVariant& value);

    protected:
        AVariantData(AVariantDataManager* manager);

    private:
        AVariantDataManager* manager;
    };

    /**
     * @brief 变量数据管理器
     */
    class APROCH_API AVariantDataManager : public AAbstractDataManager
    {
        Q_OBJECT
    public:
        AVariantDataManager(QObject* parent = nullptr);
        ~AVariantDataManager();

        virtual AVariantData* addData(int dataType, const QString& name = QString());

        int dataType(const AData* data) const;
        int valueType(const AData* data) const;
        AVariantData* variantData(const AData* data) const;

        virtual bool isDataTypeSupported(int dataType) const;
        virtual int valueType(int dataType) const;
        virtual QStringList subValues(int dataType) const;
        virtual int subValueType(int dataType, const QString& attribute) const;

        virtual QVariant value(const AData* data) const;
        virtual QVariant subValue(const AData* data, const QString& attribute) const;

        static int enumTypeId();
        static int flagTypeId();
        static int groupTypeId();
        static int iconMapTypeId();

    public Q_SLOTS:
        virtual void setValue(AData* data, const QVariant& val);
        virtual void setSubValue(AData* data, const QString& attribute, const QVariant& value);

    Q_SIGNALS:
        void valueChanged(AData* data, const QVariant& val);
        void subValueChanged(AData* data, const QString& attribute, const QVariant& val);

    protected:
        virtual bool hasValue(const AData* data) const;
        QString valueText(const AData* data) const;
        QIcon valueIcon(const AData* data) const;
        virtual void initializeData(AData* data);
        virtual void uninitializeData(AData* data);
        virtual AData* createData();

    protected Q_SLOTS:
        void slotValueChanged(AData* data, int val);
        void slotRangeChanged(AData* data, int min, int max);
        void slotSingleStepChanged(AData* data, int step);
        void slotValueChanged(AData* data, double val);
        void slotRangeChanged(AData* data, double min, double max);
        void slotSingleStepChanged(AData* data, double step);
        void slotDecimalsChanged(AData* data, int prec);
        void slotValueChanged(AData* data, bool val);
        void slotValueChanged(AData* data, const QString& val);
        void slotRegExpChanged(AData* data, const QRegExp& regExp);
        void slotValueChanged(AData* data, const QDate& val);
        void slotRangeChanged(AData* data, const QDate& min, const QDate& max);
        void slotValueChanged(AData* data, const QTime& val);
        void slotValueChanged(AData* data, const QDateTime& val);
        void slotValueChanged(AData* data, const QKeySequence& val);
        void slotValueChanged(AData* data, const QChar& val);
        void slotValueChanged(AData* data, const QLocale& val);
        void slotValueChanged(AData* data, const QPoint& val);
        void slotValueChanged(AData* data, const QPointF& val);
        void slotValueChanged(AData* data, const QSize& val);
        void slotRangeChanged(AData* data, const QSize& min, const QSize& max);
        void slotValueChanged(AData* data, const QSizeF& val);
        void slotRangeChanged(AData* data, const QSizeF& min, const QSizeF& max);
        void slotValueChanged(AData* data, const QRect& val);
        void slotConstraintChanged(AData* data, const QRect& val);
        void slotValueChanged(AData* data, const QRectF& val);
        void slotConstraintChanged(AData* data, const QRectF& val);
        void slotValueChanged(AData* data, const QColor& val);

        //void slotEnumChanged(AData* data, int val);
        void slotEnumNamesChanged(AData* data, const QStringList& enumNames);
        void slotEnumIconsChanged(AData* data, const QMap<int, QIcon>& enumIcons);
        void slotValueChanged(AData* data, const QSizePolicy& val);
        void slotValueChanged(AData* data, const QFont& val);
        void slotValueChanged(AData* data, const QCursor& val);

        //void slotFlagChanged(AData* data, int val);
        void slotFlagNamesChanged(AData* data, const QStringList& flagNames);
        void slotDataInserted(AData* data, AData* parent, AData* after);
        void slotDataRemoved(AData* data, AData* parent);

        void valueChangedHelper(AData* data, const QVariant& val);
        int internalDataToType(AData* data) const;
        AVariantData* createSubData(AVariantData* parent, AVariantData* after, AData* internal);
        void removeSubData(AVariantData* data);

    private:
        QMap<int, AAbstractDataManager*> m_typeToDataManager;
        QMap<int, QMap<QString, int> > m_typeToSubValueToSubValueType;
        QMap<const AData*, QPair<AVariantData*, int> > m_dataToType;
        QMap<int, int> m_typeToValueType;
        QMap<AData*, AVariantData*> m_internalToData;

        const QString m_constraintSubValue;
        const QString m_singleStepSubValue;
        const QString m_decimalsSubValue;
        const QString m_enumIconsSubValue;
        const QString m_enumNamesSubValue;
        const QString m_flagNamesSubValue;
        const QString m_maximumSubValue;
        const QString m_minimumSubValue;
        const QString m_regExpSubValue;

        bool m_creatingData;
        bool m_creatingSubDataSet;
        bool m_destroyingSubDataSet;
        int m_dataType;

    private:
        Q_DISABLE_COPY_MOVE(AVariantDataManager)
    };

    /**
     * @brief 创建变量编辑器工厂类
     */
    class APROCH_API AVariantEditorFactory : public AAbstractEditorFactory<AVariantDataManager>
    {
        Q_OBJECT
    public:
        AVariantEditorFactory(QObject* parent = nullptr);
        ~AVariantEditorFactory();

        using AAbstractEditorFactory<AVariantDataManager>::createEditorImpl;

    protected:
        virtual void connectDataManager(AVariantDataManager* manager) override;
        virtual QWidget* createEditorImpl(AVariantDataManager* manager, AData* data, QWidget* parent) override;
        virtual void disconnectDataManager(AVariantDataManager* manager) override;

    private:

        ASpinBoxFactory* m_spinBoxFactory;
        ADoubleSpinBoxFactory* m_doubleSpinBoxFactory;
        ACheckBoxFactory* m_checkBoxFactory;
        ALineEditFactory* m_lineEditFactory;
        /* ADateEditFactory* m_dateEditFactory;
         ATimeEditFactory* m_timeEditFactory;
         ADateTimeEditFactory* m_dateTimeEditFactory;
         AKeySequenceEditorFactory* m_keySequenceEditorFactory;
         ACharEditorFactory* m_charEditorFactory;*/
        AEnumEditorFactory* m_comboBoxFactory;

        //ACursorEditorFactory* m_cursorEditorFactory;
        AColorEditorFactory* m_colorEditorFactory;
        AFontEditorFactory* m_fontEditorFactory;

        QMap<AAbstractEditorFactoryBase*, int> m_factoryToType;
        QMap<int, AAbstractEditorFactoryBase*> m_typeToFactory;
    private:
        Q_DISABLE_COPY_MOVE(AVariantEditorFactory)
    };
}

Q_DECLARE_METATYPE(AIconMap)