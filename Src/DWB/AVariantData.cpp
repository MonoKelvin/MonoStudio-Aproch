#include "stdafx.h"
#include "AVariantData.h"

class AEnumDataType
{
};

class AFlagDataType
{
};

class AGroupDataType
{
};

Q_DECLARE_METATYPE(AEnumDataType)
Q_DECLARE_METATYPE(AFlagDataType)
Q_DECLARE_METATYPE(AGroupDataType)

namespace aproch
{
    AVariantData::AVariantData(AVariantDataManager* manager)
        : AData(manager)
        , manager(manager)
    {
    }

    AVariantData::~AVariantData()
    {
    }

    QVariant AVariantData::value() const
    {
        return manager->value(this);
    }

    QVariant AVariantData::subValue(const QString& valName) const
    {
        return manager->subValue(this, valName);
    }

    int AVariantData::valueType() const
    {
        return manager->valueType(this);
    }

    int AVariantData::dataType() const
    {
        return manager->dataType(this);
    }

    void AVariantData::setValue(const QVariant& value)
    {
        manager->setValue(this, value);
    }

    void AVariantData::setSubValue(const QString& valName, const QVariant& value)
    {
        manager->setSubValue(this, valName, value);
    }

    // ----------------------------------------------------------------------------------------------------

    int AVariantDataManager::enumTypeId()
    {
        return qMetaTypeId<AEnumDataType>();
    }

    int AVariantDataManager::flagTypeId()
    {
        return qMetaTypeId<AFlagDataType>();
    }

    int AVariantDataManager::groupTypeId()
    {
        return qMetaTypeId<AGroupDataType>();
    }

    int AVariantDataManager::iconMapTypeId()
    {
        return qMetaTypeId<AIconMap>();
    }

    typedef QMap<const AData*, AData*> DataMap;
    Q_GLOBAL_STATIC(DataMap, dataToWrappedData)

        static AData* wrappedData(AData* data)
    {
        return dataToWrappedData()->value(data, 0);
    }

    int AVariantDataManager::internalDataToType(AData* data) const
    {
        int type = 0;
        AAbstractDataManager* internDataManager = data->dataManager();
        if (qobject_cast<AIntDataManager*>(internDataManager))
            type = QVariant::Int;
        else if (qobject_cast<AEnumDataManager*>(internDataManager))
            type = AVariantDataManager::enumTypeId();
        else if (qobject_cast<ABoolDataManager*>(internDataManager))
            type = QVariant::Bool;
        else if (qobject_cast<ADoubleDataManager*>(internDataManager))
            type = QVariant::Double;
        return type;
    }

    AVariantData* AVariantDataManager::createSubData(AVariantData* parent, AVariantData* after, AData* internal)
    {
        int type = internalDataToType(internal);
        if (!type)
            return 0;

        bool wasCreatingSubDataSet = m_creatingSubDataSet;
        m_creatingSubDataSet = true;

        AVariantData* varChild = addData(type, internal->dataName());

        m_creatingSubDataSet = wasCreatingSubDataSet;

        varChild->setDataName(internal->dataName());
        varChild->setToolTip(internal->toolTip());
        varChild->setStatusTip(internal->statusTip());
        varChild->setWhatsThis(internal->whatsThis());

        parent->insertSubData(varChild, after);

        m_internalToData[internal] = varChild;
        dataToWrappedData()->insert(varChild, internal);
        return varChild;
    }

    void AVariantDataManager::removeSubData(AVariantData* data)
    {
        AData* internChild = wrappedData(data);
        bool wasDestroyingSubDataSet = m_destroyingSubDataSet;
        m_destroyingSubDataSet = true;
        m_destroyingSubDataSet = wasDestroyingSubDataSet;
        m_internalToData.remove(internChild);
        dataToWrappedData()->remove(data);
        delete data;    // todo
    }

    void AVariantDataManager::slotDataInserted(AData* data, AData* parent, AData* after)
    {
        if (m_creatingData)
            return;

        AVariantData* varParent = m_internalToData.value(parent, 0);
        if (!varParent)
            return;

        AVariantData* varAfter = 0;
        if (after)
        {
            varAfter = m_internalToData.value(after, 0);
            if (!varAfter)
                return;
        }

        createSubData(varParent, varAfter, data);
    }

    void AVariantDataManager::slotDataRemoved(AData* data, AData* parent)
    {
        Q_UNUSED(parent);

        AVariantData* varData = m_internalToData.value(data, 0);
        if (!varData)
            return;

        removeSubData(varData);
    }

    void AVariantDataManager::valueChangedHelper(AData* data, const QVariant& val)
    {
        AVariantData* varProp = m_internalToData.value(data, 0);
        if (!varProp)
            return;
        emit valueChanged(varProp, val);
        emit dataChanged(varProp);
    }

    void AVariantDataManager::slotValueChanged(AData* data, int val)
    {
        valueChangedHelper(data, QVariant(val));
    }

    void AVariantDataManager::slotRangeChanged(AData* data, int min, int max)
    {
        if (AVariantData* varProp = m_internalToData.value(data, 0))
        {
            emit subValueChanged(varProp, m_minimumSubValue, QVariant(min));
            emit subValueChanged(varProp, m_maximumSubValue, QVariant(max));
        }
    }

    void AVariantDataManager::slotSingleStepChanged(AData* data, int step)
    {
        if (AVariantData* varProp = m_internalToData.value(data, 0))
            emit subValueChanged(varProp, m_singleStepSubValue, QVariant(step));
    }

    void AVariantDataManager::slotValueChanged(AData* data, double val)
    {
        valueChangedHelper(data, QVariant(val));
    }

    void AVariantDataManager::slotRangeChanged(AData* data, double min, double max)
    {
        if (AVariantData* varProp = m_internalToData.value(data, 0))
        {
            emit subValueChanged(varProp, m_minimumSubValue, QVariant(min));
            emit subValueChanged(varProp, m_maximumSubValue, QVariant(max));
        }
    }

    void AVariantDataManager::slotSingleStepChanged(AData* data, double step)
    {
        if (AVariantData* varProp = m_internalToData.value(data, 0))
            emit subValueChanged(varProp, m_singleStepSubValue, QVariant(step));
    }

    void AVariantDataManager::slotDecimalsChanged(AData* data, int prec)
    {
        if (AVariantData* varProp = m_internalToData.value(data, 0))
            emit subValueChanged(varProp, m_decimalsSubValue, QVariant(prec));
    }

    void AVariantDataManager::slotValueChanged(AData* data, bool val)
    {
        valueChangedHelper(data, QVariant(val));
    }

    void AVariantDataManager::slotValueChanged(AData* data, const QString& val)
    {
        valueChangedHelper(data, QVariant(val));
    }

    void AVariantDataManager::slotRegExpChanged(AData* data, const QRegExp& regExp)
    {
        if (AVariantData* varProp = m_internalToData.value(data, 0))
            emit subValueChanged(varProp, m_regExpSubValue, QVariant(regExp));
    }

    void AVariantDataManager::slotValueChanged(AData* data, const QDate& val)
    {
        valueChangedHelper(data, QVariant(val));
    }

    void AVariantDataManager::slotRangeChanged(AData* data, const QDate& min, const QDate& max)
    {
        if (AVariantData* varProp = m_internalToData.value(data, 0))
        {
            emit subValueChanged(varProp, m_minimumSubValue, QVariant(min));
            emit subValueChanged(varProp, m_maximumSubValue, QVariant(max));
        }
    }

    void AVariantDataManager::slotValueChanged(AData* data, const QTime& val)
    {
        valueChangedHelper(data, QVariant(val));
    }

    void AVariantDataManager::slotValueChanged(AData* data, const QDateTime& val)
    {
        valueChangedHelper(data, QVariant(val));
    }

    void AVariantDataManager::slotValueChanged(AData* data, const QKeySequence& val)
    {
        QVariant v;
        v.setValue(val);
        valueChangedHelper(data, v);
    }

    void AVariantDataManager::slotValueChanged(AData* data, const QChar& val)
    {
        valueChangedHelper(data, QVariant(val));
    }

    void AVariantDataManager::slotValueChanged(AData* data, const QLocale& val)
    {
        valueChangedHelper(data, QVariant(val));
    }

    void AVariantDataManager::slotValueChanged(AData* data, const QPoint& val)
    {
        valueChangedHelper(data, QVariant(val));
    }

    void AVariantDataManager::slotValueChanged(AData* data, const QPointF& val)
    {
        valueChangedHelper(data, QVariant(val));
    }

    void AVariantDataManager::slotValueChanged(AData* data, const QSize& val)
    {
        valueChangedHelper(data, QVariant(val));
    }

    void AVariantDataManager::slotRangeChanged(AData* data, const QSize& min, const QSize& max)
    {
        if (AVariantData* varProp = m_internalToData.value(data, 0))
        {
            emit subValueChanged(varProp, m_minimumSubValue, QVariant(min));
            emit subValueChanged(varProp, m_maximumSubValue, QVariant(max));
        }
    }

    void AVariantDataManager::slotValueChanged(AData* data, const QSizeF& val)
    {
        valueChangedHelper(data, QVariant(val));
    }

    void AVariantDataManager::slotRangeChanged(AData* data, const QSizeF& min, const QSizeF& max)
    {
        if (AVariantData* varProp = m_internalToData.value(data, 0))
        {
            emit subValueChanged(varProp, m_minimumSubValue, QVariant(min));
            emit subValueChanged(varProp, m_maximumSubValue, QVariant(max));
        }
    }

    void AVariantDataManager::slotValueChanged(AData* data, const QRect& val)
    {
        valueChangedHelper(data, QVariant(val));
    }

    void AVariantDataManager::slotConstraintChanged(AData* data, const QRect& constraint)
    {
        if (AVariantData* varProp = m_internalToData.value(data, 0))
            emit subValueChanged(varProp, m_constraintSubValue, QVariant(constraint));
    }

    void AVariantDataManager::slotValueChanged(AData* data, const QRectF& val)
    {
        valueChangedHelper(data, QVariant(val));
    }

    void AVariantDataManager::slotConstraintChanged(AData* data, const QRectF& constraint)
    {
        if (AVariantData* varProp = m_internalToData.value(data, 0))
            emit subValueChanged(varProp, m_constraintSubValue, QVariant(constraint));
    }

    void AVariantDataManager::slotValueChanged(AData* data, const QColor& val)
    {
        valueChangedHelper(data, QVariant(val));
    }

    void AVariantDataManager::slotEnumNamesChanged(AData* data, const QStringList& enumNames)
    {
        if (AVariantData* varProp = m_internalToData.value(data, 0))
            emit subValueChanged(varProp, m_enumNamesSubValue, QVariant(enumNames));
    }

    void AVariantDataManager::slotEnumIconsChanged(AData* data, const QMap<int, QIcon>& enumIcons)
    {
        if (AVariantData* varProp = m_internalToData.value(data, 0))
        {
            QVariant v;
            v.setValue(enumIcons);
            emit subValueChanged(varProp, m_enumIconsSubValue, v);
        }
    }

    void AVariantDataManager::slotValueChanged(AData* data, const QSizePolicy& val)
    {
        valueChangedHelper(data, QVariant(val));
    }

    void AVariantDataManager::slotValueChanged(AData* data, const QFont& val)
    {
        valueChangedHelper(data, QVariant(val));
    }

    void AVariantDataManager::slotValueChanged(AData* data, const QCursor& val)
    {
#ifndef QT_NO_CURSOR
        valueChangedHelper(data, QVariant(val));
#endif
    }

    void AVariantDataManager::slotFlagNamesChanged(AData* data, const QStringList& flagNames)
    {
        if (AVariantData* varProp = m_internalToData.value(data, 0))
            emit subValueChanged(varProp, m_flagNamesSubValue, QVariant(flagNames));
    }

    AVariantDataManager::AVariantDataManager(QObject* parent)
        : AAbstractDataManager(parent)
        , m_constraintSubValue(QLatin1String("constraint"))
        , m_singleStepSubValue(QLatin1String("singleStep"))
        , m_decimalsSubValue(QLatin1String("decimals"))
        , m_enumIconsSubValue(QLatin1String("enumIcons"))
        , m_enumNamesSubValue(QLatin1String("enumNames"))
        , m_flagNamesSubValue(QLatin1String("flagNames"))
        , m_maximumSubValue(QLatin1String("maximum"))
        , m_minimumSubValue(QLatin1String("minimum"))
        , m_regExpSubValue(QLatin1String("regExp"))
    {
        m_creatingData = false;
        m_creatingSubDataSet = false;
        m_destroyingSubDataSet = false;
        m_dataType = 0;

        // IntDataManager
        AIntDataManager* intDataManager = new AIntDataManager(this);
        m_typeToDataManager[QVariant::Int] = intDataManager;
        m_typeToSubValueToSubValueType[QVariant::Int][m_minimumSubValue] = QVariant::Int;
        m_typeToSubValueToSubValueType[QVariant::Int][m_maximumSubValue] = QVariant::Int;
        m_typeToSubValueToSubValueType[QVariant::Int][m_singleStepSubValue] = QVariant::Int;
        m_typeToValueType[QVariant::Int] = QVariant::Int;
        connect(intDataManager, SIGNAL(valueChanged(AData*, int)), this, SLOT(slotValueChanged(AData*, int)));
        connect(intDataManager, SIGNAL(rangeChanged(AData*, int, int)), this, SLOT(slotRangeChanged(AData*, int, int)));
        connect(intDataManager, SIGNAL(singleStepChanged(AData*, int)), this, SLOT(slotSingleStepChanged(AData*, int)));

        // DoubleDataManager
        ADoubleDataManager* doubleDataManager = new ADoubleDataManager(this);
        m_typeToDataManager[QVariant::Double] = doubleDataManager;
        m_typeToSubValueToSubValueType[QVariant::Double][m_minimumSubValue] = QVariant::Double;
        m_typeToSubValueToSubValueType[QVariant::Double][m_maximumSubValue] = QVariant::Double;
        m_typeToSubValueToSubValueType[QVariant::Double][m_singleStepSubValue] = QVariant::Double;
        m_typeToSubValueToSubValueType[QVariant::Double][m_decimalsSubValue] = QVariant::Int;
        m_typeToValueType[QVariant::Double] = QVariant::Double;
        connect(doubleDataManager, SIGNAL(valueChanged(AData*, double)), this, SLOT(slotValueChanged(AData*, double)));
        connect(doubleDataManager, SIGNAL(rangeChanged(AData*, double, double)), this, SLOT(slotRangeChanged(AData*, double, double)));
        connect(doubleDataManager, SIGNAL(singleStepChanged(AData*, double)), this, SLOT(slotSingleStepChanged(AData*, double)));
        connect(doubleDataManager, SIGNAL(decimalsChanged(AData*, int)), this, SLOT(slotDecimalsChanged(AData*, int)));

        // BoolDataManager
        ABoolDataManager* boolDataManager = new ABoolDataManager(this);
        m_typeToDataManager[QVariant::Bool] = boolDataManager;
        m_typeToValueType[QVariant::Bool] = QVariant::Bool;
        connect(boolDataManager, SIGNAL(valueChanged(AData*, bool)), this, SLOT(slotValueChanged(AData*, bool)));

        // StringDataManager
        AStringDataManager* stringDataManager = new AStringDataManager(this);
        m_typeToDataManager[QVariant::String] = stringDataManager;
        m_typeToValueType[QVariant::String] = QVariant::String;
        m_typeToSubValueToSubValueType[QVariant::String][m_regExpSubValue] = QVariant::RegExp;
        connect(stringDataManager, SIGNAL(valueChanged(AData*, QString)), this, SLOT(slotValueChanged(AData*, QString)));
        connect(stringDataManager, SIGNAL(regExpChanged(AData*, QRegExp)), this, SLOT(slotRegExpChanged(AData*, QRegExp)));

        // DateDataManager
        /*ADateDataManager* dateDataManager = new ADateDataManager(this);
        m_typeToDataManager[QVariant::Date] = dateDataManager;
        m_typeToValueType[QVariant::Date] = QVariant::Date;
        m_typeToSubValueToSubValueType[QVariant::Date][m_minimumSubValue] =
            QVariant::Date;
        m_typeToSubValueToSubValueType[QVariant::Date][m_maximumSubValue] =
            QVariant::Date;
        connect(dateDataManager, SIGNAL(valueChanged(AData*, QDate)),
                this, SLOT(slotValueChanged(AData*, QDate)));
        connect(dateDataManager, SIGNAL(rangeChanged(AData*, QDate, QDate)),
                this, SLOT(slotRangeChanged(AData*, QDate, QDate)));

        // TimeDataManager
        ATimeDataManager* timeDataManager = new ATimeDataManager(this);
        m_typeToDataManager[QVariant::Time] = timeDataManager;
        m_typeToValueType[QVariant::Time] = QVariant::Time;
        connect(timeDataManager, SIGNAL(valueChanged(AData*, QTime)),
                this, SLOT(slotValueChanged(AData*, QTime)));

        // DateTimeDataManager
        ADateTimeDataManager* dateTimeDataManager = new ADateTimeDataManager(this);
        m_typeToDataManager[QVariant::DateTime] = dateTimeDataManager;
        m_typeToValueType[QVariant::DateTime] = QVariant::DateTime;
        connect(dateTimeDataManager, SIGNAL(valueChanged(AData*, QDateTime)),
                this, SLOT(slotValueChanged(AData*, QDateTime)));

        // KeySequenceDataManager
        AKeySequenceDataManager* keySequenceDataManager = new AKeySequenceDataManager(this);
        m_typeToDataManager[QVariant::KeySequence] = keySequenceDataManager;
        m_typeToValueType[QVariant::KeySequence] = QVariant::KeySequence;
        connect(keySequenceDataManager, SIGNAL(valueChanged(AData*, QKeySequence)),
                this, SLOT(slotValueChanged(AData*, QKeySequence)));

        // CharDataManager
        ACharDataManager* charDataManager = new ACharDataManager(this);
        m_typeToDataManager[QVariant::Char] = charDataManager;
        m_typeToValueType[QVariant::Char] = QVariant::Char;
        connect(charDataManager, SIGNAL(valueChanged(AData*, QChar)),
                this, SLOT(slotValueChanged(AData*, QChar)));

        // LocaleDataManager
        ALocaleDataManager* localeDataManager = new ALocaleDataManager(this);
        m_typeToDataManager[QVariant::Locale] = localeDataManager;
        m_typeToValueType[QVariant::Locale] = QVariant::Locale;
        connect(localeDataManager, SIGNAL(valueChanged(AData*, QLocale)),
                this, SLOT(slotValueChanged(AData*, QLocale)));
        connect(localeDataManager->subEnumDataManager(), SIGNAL(valueChanged(AData*, int)),
                this, SLOT(slotValueChanged(AData*, int)));
        connect(localeDataManager, SIGNAL(dataInserted(AData*, AData*, AData*)),
                this, SLOT(slotDataInserted(AData*, AData*, AData*)));
        connect(localeDataManager, SIGNAL(dataRemoved(AData*, AData*)),
                this, SLOT(slotDataRemoved(AData*, AData*)));*/

                // PointDataManager
        APointDataManager* pointDataManager = new APointDataManager(this);
        m_typeToDataManager[QVariant::Point] = pointDataManager;
        m_typeToValueType[QVariant::Point] = QVariant::Point;
        m_typeToSubValueToSubValueType[QVariant::Point][m_minimumSubValue] = QVariant::Point;
        m_typeToSubValueToSubValueType[QVariant::Point][m_maximumSubValue] = QVariant::Point;
        connect(pointDataManager, SIGNAL(valueChanged(AData*, QPoint)), this, SLOT(slotValueChanged(AData*, QPoint)));
        connect(pointDataManager, SIGNAL(rangeChanged(AData*, QPoint, QPoint)), this, SLOT(slotRangeChanged(AData*, QPoint, QPoint)));
        connect(pointDataManager->subIntDataManager(), SIGNAL(valueChanged(AData*, int)), this, SLOT(slotValueChanged(AData*, int)));
        connect(pointDataManager->subIntDataManager(), SIGNAL(rangeChanged(AData*, int, int)), this, SLOT(slotRangeChanged(AData*, int, int)));
        connect(pointDataManager, SIGNAL(dataInserted(AData*, AData*, AData*)), this, SLOT(slotDataInserted(AData*, AData*, AData*)));
        connect(pointDataManager, SIGNAL(dataRemoved(AData*, AData*)), this, SLOT(slotDataRemoved(AData*, AData*)));

        // PointFDataManager
        /*APointFDataManager* pointFDataManager = new APointFDataManager(this);
        m_typeToDataManager[QVariant::PointF] = pointFDataManager;
        m_typeToValueType[QVariant::PointF] = QVariant::PointF;
        m_typeToSubValueToSubValueType[QVariant::PointF][m_decimalsSubValue] = QVariant::Int;
        connect(pointFDataManager, SIGNAL(valueChanged(AData*, QPointF)), this, SLOT(slotValueChanged(AData*, QPointF)));
        connect(pointFDataManager, SIGNAL(decimalsChanged(AData*, int)), this, SLOT(slotDecimalsChanged(AData*, int)));
        connect(pointFDataManager->subDoubleDataManager(), SIGNAL(valueChanged(AData*, double)), this, SLOT(slotValueChanged(AData*, double)));
        connect(pointFDataManager, SIGNAL(dataInserted(AData*, AData*, AData*)), this, SLOT(slotDataInserted(AData*, AData*, AData*)));
        connect(pointFDataManager, SIGNAL(dataRemoved(AData*, AData*)), this, SLOT(slotDataRemoved(AData*, AData*)));*/

        // SizeDataManager
        ASizeDataManager* sizeDataManager = new ASizeDataManager(this);
        m_typeToDataManager[QVariant::Size] = sizeDataManager;
        m_typeToValueType[QVariant::Size] = QVariant::Size;
        m_typeToSubValueToSubValueType[QVariant::Size][m_minimumSubValue] = QVariant::Size;
        m_typeToSubValueToSubValueType[QVariant::Size][m_maximumSubValue] = QVariant::Size;
        connect(sizeDataManager, SIGNAL(valueChanged(AData*, QSize)), this, SLOT(slotValueChanged(AData*, QSize)));
        connect(sizeDataManager, SIGNAL(rangeChanged(AData*, QSize, QSize)), this, SLOT(slotRangeChanged(AData*, QSize, QSize)));
        connect(sizeDataManager->subIntDataManager(), SIGNAL(valueChanged(AData*, int)), this, SLOT(slotValueChanged(AData*, int)));
        connect(sizeDataManager->subIntDataManager(), SIGNAL(rangeChanged(AData*, int, int)), this, SLOT(slotRangeChanged(AData*, int, int)));
        connect(sizeDataManager, SIGNAL(dataInserted(AData*, AData*, AData*)), this, SLOT(slotDataInserted(AData*, AData*, AData*)));
        connect(sizeDataManager, SIGNAL(dataRemoved(AData*, AData*)), this, SLOT(slotDataRemoved(AData*, AData*)));

        // SizeFDataManager
        /*ASizeFDataManager* sizeFDataManager = new ASizeFDataManager(this);
        m_typeToDataManager[QVariant::SizeF] = sizeFDataManager;
        m_typeToValueType[QVariant::SizeF] = QVariant::SizeF;
        m_typeToSubValueToSubValueType[QVariant::SizeF][m_minimumSubValue] = QVariant::SizeF;
        m_typeToSubValueToSubValueType[QVariant::SizeF][m_maximumSubValue] = QVariant::SizeF;
        m_typeToSubValueToSubValueType[QVariant::SizeF][m_decimalsSubValue] = QVariant::Int;
        connect(sizeFDataManager, SIGNAL(valueChanged(AData*, QSizeF)), this, SLOT(slotValueChanged(AData*, QSizeF)));
        connect(sizeFDataManager, SIGNAL(rangeChanged(AData*, QSizeF, QSizeF)), this, SLOT(slotRangeChanged(AData*, QSizeF, QSizeF)));
        connect(sizeFDataManager, SIGNAL(decimalsChanged(AData*, int)), this, SLOT(slotDecimalsChanged(AData*, int)));
        connect(sizeFDataManager->subDoubleDataManager(), SIGNAL(valueChanged(AData*, double)), this, SLOT(slotValueChanged(AData*, double)));
        connect(sizeFDataManager->subDoubleDataManager(), SIGNAL(rangeChanged(AData*, double, double)), this, SLOT(slotRangeChanged(AData*, double, double)));
        connect(sizeFDataManager, SIGNAL(dataInserted(AData*, AData*, AData*)), this, SLOT(slotDataInserted(AData*, AData*, AData*)));
        connect(sizeFDataManager, SIGNAL(dataRemoved(AData*, AData*)), this, SLOT(slotDataRemoved(AData*, AData*)));

        // RectDataManager
        ARectDataManager* rectDataManager = new ARectDataManager(this);
        m_typeToDataManager[QVariant::Rect] = rectDataManager;
        m_typeToValueType[QVariant::Rect] = QVariant::Rect;
        m_typeToSubValueToSubValueType[QVariant::Rect][m_constraintSubValue] = QVariant::Rect;
        connect(rectDataManager, SIGNAL(valueChanged(AData*, QRect)), this, SLOT(slotValueChanged(AData*, QRect)));
        connect(rectDataManager, SIGNAL(constraintChanged(AData*, QRect)), this, SLOT(slotConstraintChanged(AData*, QRect)));
        connect(rectDataManager->subIntDataManager(), SIGNAL(valueChanged(AData*, int)), this, SLOT(slotValueChanged(AData*, int)));
        connect(rectDataManager->subIntDataManager(), SIGNAL(rangeChanged(AData*, int, int)), this, SLOT(slotRangeChanged(AData*, int, int)));
        connect(rectDataManager, SIGNAL(dataInserted(AData*, AData*, AData*)), this, SLOT(slotDataInserted(AData*, AData*, AData*)));
        connect(rectDataManager, SIGNAL(dataRemoved(AData*, AData*)), this, SLOT(slotDataRemoved(AData*, AData*)));

        // RectFDataManager
        ARectFDataManager* rectFDataManager = new ARectFDataManager(this);
        m_typeToDataManager[QVariant::RectF] = rectFDataManager;
        m_typeToValueType[QVariant::RectF] = QVariant::RectF;
        m_typeToSubValueToSubValueType[QVariant::RectF][m_constraintSubValue] = QVariant::RectF;
        m_typeToSubValueToSubValueType[QVariant::RectF][m_decimalsSubValue] = QVariant::Int;
        connect(rectFDataManager, SIGNAL(valueChanged(AData*, QRectF)), this, SLOT(slotValueChanged(AData*, QRectF)));
        connect(rectFDataManager, SIGNAL(constraintChanged(AData*, QRectF)), this, SLOT(slotConstraintChanged(AData*, QRectF)));
        connect(rectFDataManager, SIGNAL(decimalsChanged(AData*, int)), this, SLOT(slotDecimalsChanged(AData*, int)));
        connect(rectFDataManager->subDoubleDataManager(), SIGNAL(valueChanged(AData*, double)), this, SLOT(slotValueChanged(AData*, double)));
        connect(rectFDataManager->subDoubleDataManager(), SIGNAL(rangeChanged(AData*, double, double)), this, SLOT(slotRangeChanged(AData*, double, double)));
        connect(rectFDataManager, SIGNAL(dataInserted(AData*, AData*, AData*)), this, SLOT(slotDataInserted(AData*, AData*, AData*)));
        connect(rectFDataManager, SIGNAL(dataRemoved(AData*, AData*)), this, SLOT(slotDataRemoved(AData*, AData*)));*/

        // ColorDataManager
        AColorDataManager* colorDataManager = new AColorDataManager(this);
        m_typeToDataManager[QVariant::Color] = colorDataManager;
        m_typeToValueType[QVariant::Color] = QVariant::Color;
        connect(colorDataManager, SIGNAL(valueChanged(AData*, QColor)), this, SLOT(slotValueChanged(AData*, QColor)));
        connect(colorDataManager->subIntDataManager(), SIGNAL(valueChanged(AData*, int)), this, SLOT(slotValueChanged(AData*, int)));
        connect(colorDataManager, SIGNAL(dataInserted(AData*, AData*, AData*)), this, SLOT(slotDataInserted(AData*, AData*, AData*)));
        connect(colorDataManager, SIGNAL(dataRemoved(AData*, AData*)), this, SLOT(slotDataRemoved(AData*, AData*)));

        // EnumDataManager
        int enumId = enumTypeId();
        AEnumDataManager* enumDataManager = new AEnumDataManager(this);
        m_typeToDataManager[enumId] = enumDataManager;
        m_typeToValueType[enumId] = QVariant::Int;
        m_typeToSubValueToSubValueType[enumId][m_enumNamesSubValue] = QVariant::StringList;
        m_typeToSubValueToSubValueType[enumId][m_enumIconsSubValue] = iconMapTypeId();
        connect(enumDataManager, SIGNAL(valueChanged(AData*, int)), this, SLOT(slotValueChanged(AData*, int)));
        connect(enumDataManager, SIGNAL(enumNamesChanged(AData*, QStringList)), this, SLOT(slotEnumNamesChanged(AData*, QStringList)));
        connect(enumDataManager, SIGNAL(enumIconsChanged(AData*, QMap<int, QIcon>)), this, SLOT(slotEnumIconsChanged(AData*, QMap<int, QIcon>)));

        // SizePolicyDataManager
        /*ASizePolicyDataManager* sizePolicyDataManager = new ASizePolicyDataManager(this);
        m_typeToDataManager[QVariant::SizePolicy] = sizePolicyDataManager;
        m_typeToValueType[QVariant::SizePolicy] = QVariant::SizePolicy;
        connect(sizePolicyDataManager, SIGNAL(valueChanged(AData*, QSizePolicy)),
                this, SLOT(slotValueChanged(AData*, QSizePolicy)));
        connect(sizePolicyDataManager->subIntDataManager(), SIGNAL(valueChanged(AData*, int)),
                this, SLOT(slotValueChanged(AData*, int)));
        connect(sizePolicyDataManager->subIntDataManager(), SIGNAL(rangeChanged(AData*, int, int)),
                this, SLOT(slotRangeChanged(AData*, int, int)));
        connect(sizePolicyDataManager->subEnumDataManager(), SIGNAL(valueChanged(AData*, int)),
                this, SLOT(slotValueChanged(AData*, int)));
        connect(sizePolicyDataManager->subEnumDataManager(),
                SIGNAL(enumNamesChanged(AData*, QStringList)),
                this, SLOT(slotEnumNamesChanged(AData*, QStringList)));
        connect(sizePolicyDataManager, SIGNAL(dataInserted(AData*, AData*, AData*)),
                this, SLOT(slotDataInserted(AData*, AData*, AData*)));
        connect(sizePolicyDataManager, SIGNAL(dataRemoved(AData*, AData*)),
                this, SLOT(slotDataRemoved(AData*, AData*)));*/

                // FontDataManager
        AFontDataManager* fontDataManager = new AFontDataManager(this);
        m_typeToDataManager[QVariant::Font] = fontDataManager;
        m_typeToValueType[QVariant::Font] = QVariant::Font;
        connect(fontDataManager, SIGNAL(valueChanged(AData*, QFont)), this, SLOT(slotValueChanged(AData*, QFont)));
        connect(fontDataManager->subIntDataManager(), SIGNAL(valueChanged(AData*, int)), this, SLOT(slotValueChanged(AData*, int)));
        connect(fontDataManager->subIntDataManager(), SIGNAL(rangeChanged(AData*, int, int)), this, SLOT(slotRangeChanged(AData*, int, int)));
        connect(fontDataManager->subEnumDataManager(), SIGNAL(valueChanged(AData*, int)), this, SLOT(slotValueChanged(AData*, int)));
        connect(fontDataManager->subEnumDataManager(), SIGNAL(enumNamesChanged(AData*, QStringList)), this, SLOT(slotEnumNamesChanged(AData*, QStringList)));
        connect(fontDataManager->subBoolDataManager(), SIGNAL(valueChanged(AData*, bool)), this, SLOT(slotValueChanged(AData*, bool)));
        connect(fontDataManager, SIGNAL(dataInserted(AData*, AData*, AData*)), this, SLOT(slotDataInserted(AData*, AData*, AData*)));
        connect(fontDataManager, SIGNAL(dataRemoved(AData*, AData*)), this, SLOT(slotDataRemoved(AData*, AData*)));

        // CursorDataManager
         /*ACursorDataManager* cursorDataManager = new ACursorDataManager(this);
        m_typeToDataManager[QVariant::Cursor] = cursorDataManager;
        m_typeToValueType[QVariant::Cursor] = QVariant::Cursor;
        connect(cursorDataManager, SIGNAL(valueChanged(AData*, QCursor)),
                this, SLOT(slotValueChanged(AData*, QCursor)));

        // FlagDataManager
        int flagId = flagTypeId();
        AFlagDataManager* flagDataManager = new AFlagDataManager(this);
        m_typeToDataManager[flagId] = flagDataManager;
        m_typeToValueType[flagId] = QVariant::Int;
        m_typeToSubValueToSubValueType[flagId][m_flagNamesSubValue] =
            QVariant::StringList;
        connect(flagDataManager, SIGNAL(valueChanged(AData*, int)),
                this, SLOT(slotValueChanged(AData*, int)));
        connect(flagDataManager, SIGNAL(flagNamesChanged(AData*, QStringList)),
                this, SLOT(slotFlagNamesChanged(AData*, QStringList)));
        connect(flagDataManager->subBoolDataManager(), SIGNAL(valueChanged(AData*, bool)),
                this, SLOT(slotValueChanged(AData*, bool)));
        connect(flagDataManager, SIGNAL(dataInserted(AData*, AData*, AData*)),
                this, SLOT(slotDataInserted(AData*, AData*, AData*)));
        connect(flagDataManager, SIGNAL(dataRemoved(AData*, AData*)),
                this, SLOT(slotDataRemoved(AData*, AData*)));

        // FlagDataManager
        int groupId = groupTypeId();
        AGroupDataManager* groupDataManager = new AGroupDataManager(this);
        m_typeToDataManager[groupId] = groupDataManager;
        m_typeToValueType[groupId] = QVariant::Invalid;*/
    }

    AVariantDataManager::~AVariantDataManager()
    {
        clear();
    }

    AVariantData* AVariantDataManager::variantData(const AData* data) const
    {
        const QMap<const AData*, QPair<AVariantData*, int> >::const_iterator it = m_dataToType.constFind(data);
        if (it == m_dataToType.cend())
            return nullptr;
        return it.value().first;
    }

    bool AVariantDataManager::isDataTypeSupported(int dataType) const
    {
        if (m_typeToValueType.contains(dataType))
            return true;
        return false;
    }

    AVariantData* AVariantDataManager::addData(int dataType, const QString& name)
    {
        if (!isDataTypeSupported(dataType))
            return 0;

        bool wasCreating = m_creatingData;
        m_creatingData = true;
        m_dataType = dataType;
        AData* data = AAbstractDataManager::addData(name);
        m_creatingData = wasCreating;
        m_dataType = 0;

        if (!data)
            return 0;

        return variantData(data);
    }

    QVariant AVariantDataManager::value(const AData* data) const
    {
        AData* internProp = dataToWrappedData()->value(data, 0);
        if (internProp == 0)
            return QVariant();

        AAbstractDataManager* manager = internProp->dataManager();
        if (AIntDataManager* intManager = qobject_cast<AIntDataManager*>(manager))
        {
            return intManager->value(internProp);
        }
        else if (ADoubleDataManager* doubleManager = qobject_cast<ADoubleDataManager*>(manager))
        {
            return doubleManager->value(internProp);
        }
        else if (ABoolDataManager* boolManager = qobject_cast<ABoolDataManager*>(manager))
        {
            return boolManager->value(internProp);
        }
        else if (AStringDataManager* stringManager = qobject_cast<AStringDataManager*>(manager))
        {
            return stringManager->value(internProp);
        }
        /*else if (ADateDataManager* dateManager = qobject_cast<ADateDataManager*>(manager))
        {
            return dateManager->value(internProp);
        }
        else if (ATimeDataManager* timeManager = qobject_cast<ATimeDataManager*>(manager))
        {
            return timeManager->value(internProp);
        }
        else if (ADateTimeDataManager* dateTimeManager = qobject_cast<ADateTimeDataManager*>(manager))
        {
            return dateTimeManager->value(internProp);
        }
        else if (AKeySequenceDataManager* keySequenceManager = qobject_cast<AKeySequenceDataManager*>(manager))
        {
            return QVariant::fromValue(keySequenceManager->value(internProp));
        }
        else if (ACharDataManager* charManager = qobject_cast<ACharDataManager*>(manager))
        {
            return charManager->value(internProp);
        }
        else if (ALocaleDataManager* localeManager = qobject_cast<ALocaleDataManager*>(manager))
        {
            return localeManager->value(internProp);
        }*/
        else if (APointDataManager* pointManager = qobject_cast<APointDataManager*>(manager))
        {
            return pointManager->value(internProp);
        }
        /*else if (APointFDataManager* pointFManager = qobject_cast<APointFDataManager*>(manager))
        {
            return pointFManager->value(internProp);
        }*/
        else if (ASizeDataManager* sizeManager = qobject_cast<ASizeDataManager*>(manager))
        {
            return sizeManager->value(internProp);
        }
        /*else if (ASizeFDataManager* sizeFManager = qobject_cast<ASizeFDataManager*>(manager))
        {
            return sizeFManager->value(internProp);
        }
        else if (ARectDataManager* rectManager = qobject_cast<ARectDataManager*>(manager))
        {
            return rectManager->value(internProp);
        }
        else if (ARectFDataManager* rectFManager = qobject_cast<ARectFDataManager*>(manager))
        {
            return rectFManager->value(internProp);
        }*/
        else if (AColorDataManager* colorManager = qobject_cast<AColorDataManager*>(manager))
        {
            return colorManager->value(internProp);
        }
        else if (AEnumDataManager* enumManager = qobject_cast<AEnumDataManager*>(manager))
        {
            return enumManager->value(internProp);
        }
        /*else if (AFontDataManager* fontManager = qobject_cast<AFontDataManager*>(manager))
        {
            return fontManager->value(internProp);
        }
        else if (ASizePolicyDataManager* sizePolicyManager =
                 qobject_cast<ASizePolicyDataManager*>(manager))
        {
            return sizePolicyManager->value(internProp);
#ifndef QT_NO_CURSOR
        }
        else if (ACursorDataManager* cursorManager = qobject_cast<ACursorDataManager*>(manager))
        {
            return cursorManager->value(internProp);
#endif
        }
        else if (AFlagDataManager* flagManager = qobject_cast<AFlagDataManager*>(manager))
        {
            return flagManager->value(internProp);
        }*/
        return QVariant();
    }

    int AVariantDataManager::valueType(const AData* data) const
    {
        int propType = dataType(data);
        return valueType(propType);
    }

    int AVariantDataManager::valueType(int dataType) const
    {
        if (m_typeToValueType.contains(dataType))
            return m_typeToValueType[dataType];
        return 0;
    }

    int AVariantDataManager::dataType(const AData* data) const
    {
        const QMap<const AData*, QPair<AVariantData*, int> >::const_iterator it = m_dataToType.constFind(data);
        if (it == m_dataToType.cend())
            return 0;
        return it.value().second;
    }

    QVariant AVariantDataManager::subValue(const AData* data, const QString& subValue) const
    {
        int propType = dataType(data);
        if (!propType)
            return QVariant();

        QMap<int, QMap<QString, int> >::ConstIterator it = m_typeToSubValueToSubValueType.find(propType);
        if (it == m_typeToSubValueToSubValueType.cend())
            return QVariant();

        QMap<QString, int> subValues = it.value();
        QMap<QString, int>::ConstIterator itAttr = subValues.find(subValue);
        if (itAttr == subValues.cend())
            return QVariant();

        AData* internProp = dataToWrappedData()->value(data, 0);
        if (internProp == 0)
            return QVariant();

        AAbstractDataManager* manager = internProp->dataManager();
        if (AIntDataManager* intManager = qobject_cast<AIntDataManager*>(manager))
        {
            if (subValue == m_maximumSubValue)
                return intManager->maximum(internProp);
            if (subValue == m_minimumSubValue)
                return intManager->minimum(internProp);
            if (subValue == m_singleStepSubValue)
                return intManager->singleStep(internProp);
            return QVariant();
        }
        else if (ADoubleDataManager* doubleManager = qobject_cast<ADoubleDataManager*>(manager))
        {
            if (subValue == m_maximumSubValue)
                return doubleManager->maximum(internProp);
            if (subValue == m_minimumSubValue)
                return doubleManager->minimum(internProp);
            if (subValue == m_singleStepSubValue)
                return doubleManager->singleStep(internProp);
            if (subValue == m_decimalsSubValue)
                return doubleManager->decimals(internProp);
            return QVariant();
        }
        else if (AStringDataManager* stringManager = qobject_cast<AStringDataManager*>(manager))
        {
            if (subValue == m_regExpSubValue)
                return stringManager->regExp(internProp);
            return QVariant();
        }
        /*else if (ADateDataManager* dateManager = qobject_cast<ADateDataManager*>(manager))
        {
            if (subValue == m_maximumSubValue)
                return dateManager->maximum(internProp);
            if (subValue == m_minimumSubValue)
                return dateManager->minimum(internProp);
            return QVariant();
        }
        else if (APointFDataManager* pointFManager = qobject_cast<APointFDataManager*>(manager))
        {
            if (subValue == m_decimalsSubValue)
                return pointFManager->decimals(internProp);
            return QVariant();
        }*/
        else if (ASizeDataManager* sizeManager = qobject_cast<ASizeDataManager*>(manager))
        {
            if (subValue == m_maximumSubValue)
                return sizeManager->maximum(internProp);
            if (subValue == m_minimumSubValue)
                return sizeManager->minimum(internProp);
            return QVariant();
        }
        /*else if (ASizeFDataManager* sizeFManager = qobject_cast<ASizeFDataManager*>(manager))
        {
            if (subValue == m_maximumSubValue)
                return sizeFManager->maximum(internProp);
            if (subValue == m_minimumSubValue)
                return sizeFManager->minimum(internProp);
            if (subValue == m_decimalsSubValue)
                return sizeFManager->decimals(internProp);
            return QVariant();
        }
        else if (ARectDataManager* rectManager = qobject_cast<ARectDataManager*>(manager))
        {
            if (subValue == m_constraintSubValue)
                return rectManager->constraint(internProp);
            return QVariant();
        }
        else if (ARectFDataManager* rectFManager = qobject_cast<ARectFDataManager*>(manager))
        {
            if (subValue == m_constraintSubValue)
                return rectFManager->constraint(internProp);
            if (subValue == m_decimalsSubValue)
                return rectFManager->decimals(internProp);
            return QVariant();
        }*/
        else if (AEnumDataManager* enumManager = qobject_cast<AEnumDataManager*>(manager))
        {
            if (subValue == m_enumNamesSubValue)
                return enumManager->enumNames(internProp);
            if (subValue == m_enumIconsSubValue)
            {
                QVariant v;
                v.setValue(enumManager->enumIcons(internProp));
                return v;
            }
            return QVariant();
        }
        /*else if (AFlagDataManager* flagManager = qobject_cast<AFlagDataManager*>(manager))
        {
            if (subValue == m_flagNamesSubValue)
                return flagManager->flagNames(internProp);
            return QVariant();
        }*/
        return QVariant();
    }

    QStringList AVariantDataManager::subValues(int dataType) const
    {
        auto it = m_typeToSubValueToSubValueType.find(dataType);
        if (it == m_typeToSubValueToSubValueType.cend())
            return QStringList();
        return it.value().keys();
    }

    int AVariantDataManager::subValueType(int dataType, const QString& subValue) const
    {
        auto it = m_typeToSubValueToSubValueType.find(dataType);
        if (it == m_typeToSubValueToSubValueType.cend())
            return 0;

        const QMap<QString, int>& subValues = it.value();
        QMap<QString, int>::ConstIterator itAttr = subValues.find(subValue);
        if (itAttr == subValues.cend())
            return 0;
        return itAttr.value();
    }

    void AVariantDataManager::setValue(AData* data, const QVariant& val)
    {
        int propType = val.userType();
        if (!propType)
            return;

        int valType = valueType(data);

        if (propType != valType && !val.canConvert(static_cast<QVariant::Type>(valType)))
            return;

        AData* internProp = dataToWrappedData()->value(data, 0);
        if (internProp == 0)
            return;

        AAbstractDataManager* manager = internProp->dataManager();
        if (AIntDataManager* intManager = qobject_cast<AIntDataManager*>(manager))
        {
            intManager->setValue(internProp, qvariant_cast<int>(val));
            return;
        }
        else if (ADoubleDataManager* doubleManager = qobject_cast<ADoubleDataManager*>(manager))
        {
            doubleManager->setValue(internProp, qvariant_cast<double>(val));
            return;
        }
        else if (ABoolDataManager* boolManager = qobject_cast<ABoolDataManager*>(manager))
        {
            boolManager->setValue(internProp, qvariant_cast<bool>(val));
            return;
        }
        else if (AStringDataManager* stringManager = qobject_cast<AStringDataManager*>(manager))
        {
            stringManager->setValue(internProp, qvariant_cast<QString>(val));
            return;
        }
        /*else if (ADateDataManager* dateManager = qobject_cast<ADateDataManager*>(manager))
        {
            dateManager->setValue(internProp, qvariant_cast<QDate>(val));
            return;
        }
        else if (ATimeDataManager* timeManager = qobject_cast<ATimeDataManager*>(manager))
        {
            timeManager->setValue(internProp, qvariant_cast<QTime>(val));
            return;
        }
        else if (ADateTimeDataManager* dateTimeManager = qobject_cast<ADateTimeDataManager*>(manager))
        {
            dateTimeManager->setValue(internProp, qvariant_cast<QDateTime>(val));
            return;
        }
        else if (AKeySequenceDataManager* keySequenceManager = qobject_cast<AKeySequenceDataManager*>(manager))
        {
            keySequenceManager->setValue(internProp, qvariant_cast<QKeySequence>(val));
            return;
        }
        else if (ACharDataManager* charManager = qobject_cast<ACharDataManager*>(manager))
        {
            charManager->setValue(internProp, qvariant_cast<QChar>(val));
            return;
        }
        else if (ALocaleDataManager* localeManager = qobject_cast<ALocaleDataManager*>(manager))
        {
            localeManager->setValue(internProp, qvariant_cast<QLocale>(val));
            return;
        }*/
        else if (APointDataManager* pointManager = qobject_cast<APointDataManager*>(manager))
        {
            pointManager->setValue(internProp, qvariant_cast<QPoint>(val));
            return;
        }
        /*else if (APointFDataManager* pointFManager = qobject_cast<APointFDataManager*>(manager))
        {
            pointFManager->setValue(internProp, qvariant_cast<QPointF>(val));
            return;
        }*/
        else if (ASizeDataManager* sizeManager = qobject_cast<ASizeDataManager*>(manager))
        {
            sizeManager->setValue(internProp, qvariant_cast<QSize>(val));
            return;
        }
        /*else if (ASizeFDataManager* sizeFManager = qobject_cast<ASizeFDataManager*>(manager))
        {
            sizeFManager->setValue(internProp, qvariant_cast<QSizeF>(val));
            return;
        }
        else if (ARectDataManager* rectManager = qobject_cast<ARectDataManager*>(manager))
        {
            rectManager->setValue(internProp, qvariant_cast<QRect>(val));
            return;
        }
        else if (ARectFDataManager* rectFManager = qobject_cast<ARectFDataManager*>(manager))
        {
            rectFManager->setValue(internProp, qvariant_cast<QRectF>(val));
            return;
        }*/
        else if (AEnumDataManager* enumManager = qobject_cast<AEnumDataManager*>(manager))
        {
            enumManager->setValue(internProp, qvariant_cast<int>(val));
            return;
        }
        else if (AColorDataManager* colorManager = qobject_cast<AColorDataManager*>(manager))
        {
            colorManager->setValue(internProp, qvariant_cast<QColor>(val));
            return;
        }
        else if (AFontDataManager* fontManager = qobject_cast<AFontDataManager*>(manager))
        {
            fontManager->setValue(internProp, qvariant_cast<QFont>(val));
            return;
        }
        /*else if (ASizePolicyDataManager* sizePolicyManager = qobject_cast<ASizePolicyDataManager*>(manager))
        {
            sizePolicyManager->setValue(internProp, qvariant_cast<QSizePolicy>(val));
            return;
#ifndef QT_NO_CURSOR
        }
        else if (ACursorDataManager* cursorManager = qobject_cast<ACursorDataManager*>(manager))
        {
            cursorManager->setValue(internProp, qvariant_cast<QCursor>(val));
            return;
#endif
        }
        else if (AFlagDataManager* flagManager = qobject_cast<AFlagDataManager*>(manager))
        {
            flagManager->setValue(internProp, qvariant_cast<int>(val));
            return;
        }*/
    }

    void AVariantDataManager::setSubValue(AData* data, const QString& subValName, const QVariant& value)
    {
        QVariant oldAttr = subValue(data, subValName);
        if (!oldAttr.isValid())
            return;

        int attrType = value.userType();
        if (!attrType)
            return;

        if (attrType != subValueType(dataType(data), subValName) &&
            !value.canConvert((QVariant::Type)attrType))
            return;

        AData* internProp = dataToWrappedData()->value(data, 0);
        if (internProp == 0)
            return;

        AAbstractDataManager* manager = internProp->dataManager();
        if (AIntDataManager* intManager = qobject_cast<AIntDataManager*>(manager))
        {
            if (subValName == m_maximumSubValue)
                intManager->setMaximum(internProp, qvariant_cast<int>(value));
            else if (subValName == m_minimumSubValue)
                intManager->setMinimum(internProp, qvariant_cast<int>(value));
            else if (subValName == m_singleStepSubValue)
                intManager->setSingleStep(internProp, qvariant_cast<int>(value));
            return;
        }
        else if (ADoubleDataManager* doubleManager = qobject_cast<ADoubleDataManager*>(manager))
        {
            if (subValName == m_maximumSubValue)
                doubleManager->setMaximum(internProp, qvariant_cast<double>(value));
            if (subValName == m_minimumSubValue)
                doubleManager->setMinimum(internProp, qvariant_cast<double>(value));
            if (subValName == m_singleStepSubValue)
                doubleManager->setSingleStep(internProp, qvariant_cast<double>(value));
            if (subValName == m_decimalsSubValue)
                doubleManager->setDecimals(internProp, qvariant_cast<int>(value));
            return;
        }
        else if (AStringDataManager* stringManager = qobject_cast<AStringDataManager*>(manager))
        {
            if (subValName == m_regExpSubValue)
                stringManager->setRegExp(internProp, qvariant_cast<QRegExp>(value));
            return;
        }
        else if (ASizeDataManager* sizeManager = qobject_cast<ASizeDataManager*>(manager))
        {
            if (subValName == m_maximumSubValue)
                sizeManager->setMaximum(internProp, qvariant_cast<QSize>(value));
            if (subValName == m_minimumSubValue)
                sizeManager->setMinimum(internProp, qvariant_cast<QSize>(value));
            return;
        }
        else if (AEnumDataManager* enumManager = qobject_cast<AEnumDataManager*>(manager))
        {
            if (subValName == m_enumNamesSubValue)
                enumManager->setEnumNames(internProp, qvariant_cast<QStringList>(value));
            if (subValName == m_enumIconsSubValue)
                enumManager->setEnumIcons(internProp, qvariant_cast<AIconMap>(value));
            return;
        }
        /*else if (ADateDataManager* dateManager = qobject_cast<ADateDataManager*>(manager))
        {
            if (subValName == m_maximumSubValue)
                dateManager->setMaximum(internProp, qvariant_cast<QDate>(value));
            if (subValName == m_minimumSubValue)
                dateManager->setMinimum(internProp, qvariant_cast<QDate>(value));
            return;
        }
        else if (APointFDataManager* pointFManager = qobject_cast<APointFDataManager*>(manager))
        {
            if (subValName == m_decimalsSubValue)
                pointFManager->setDecimals(internProp, qvariant_cast<int>(value));
            return;
        }
        else if (ASizeFDataManager* sizeFManager = qobject_cast<ASizeFDataManager*>(manager))
        {
            if (subValName == m_maximumSubValue)
                sizeFManager->setMaximum(internProp, qvariant_cast<QSizeF>(value));
            if (subValName == m_minimumSubValue)
                sizeFManager->setMinimum(internProp, qvariant_cast<QSizeF>(value));
            if (subValName == m_decimalsSubValue)
                sizeFManager->setDecimals(internProp, qvariant_cast<int>(value));
            return;
        }
        else if (ARectDataManager* rectManager = qobject_cast<ARectDataManager*>(manager))
        {
            if (subValName == m_constraintSubValue)
                rectManager->setConstraint(internProp, qvariant_cast<QRect>(value));
            return;
        }
        else if (ARectFDataManager* rectFManager = qobject_cast<ARectFDataManager*>(manager))
        {
            if (subValName == m_constraintSubValue)
                rectFManager->setConstraint(internProp, qvariant_cast<QRectF>(value));
            if (subValName == m_decimalsSubValue)
                rectFManager->setDecimals(internProp, qvariant_cast<int>(value));
            return;
        }
        else if (AFlagDataManager* flagManager = qobject_cast<AFlagDataManager*>(manager))
        {
            if (subValName == m_flagNamesSubValue)
                flagManager->setFlagNames(internProp, qvariant_cast<QStringList>(value));
            return;
        }*/
    }

    bool AVariantDataManager::hasValue(const AData* data) const
    {
        if (dataType(data) == groupTypeId())
            return false;
        return true;
    }

    QString AVariantDataManager::valueText(const AData* data) const
    {
        const AData* internProp = dataToWrappedData()->value(data, 0);
        return internProp ? internProp->valueText() : QString();
    }

    QIcon AVariantDataManager::valueIcon(const AData* data) const
    {
        const AData* internProp = dataToWrappedData()->value(data, 0);
        return internProp ? internProp->valueIcon() : QIcon();
    }

    void AVariantDataManager::initializeData(AData* data)
    {
        AVariantData* varProp = variantData(data);
        if (!varProp)
            return;

        QMap<int, AAbstractDataManager*>::ConstIterator it =
            m_typeToDataManager.find(m_dataType);
        if (it != m_typeToDataManager.cend())
        {
            AData* internProp = 0;
            if (!m_creatingSubDataSet)
            {
                AAbstractDataManager* manager = it.value();
                internProp = manager->addData();
                m_internalToData[internProp] = varProp;
            }
            dataToWrappedData()->insert(varProp, internProp);
            if (internProp)
            {
                const QList<AData*> children = internProp->subDataSet();
                AVariantData* lastData = 0;
                for (AData* child : children)
                {
                    AVariantData* prop = createSubData(varProp, lastData, child);
                    lastData = prop ? prop : lastData;
                }
            }
        }
    }

    void AVariantDataManager::uninitializeData(AData* data)
    {
        const QMap<const AData*, QPair<AVariantData*, int> >::iterator type_it = m_dataToType.find(data);
        if (type_it == m_dataToType.end())
            return;

        DataMap::iterator it = dataToWrappedData()->find(data);
        if (it != dataToWrappedData()->end())
        {
            AData* internProp = it.value();
            if (internProp)
            {
                m_internalToData.remove(internProp);
                if (!m_destroyingSubDataSet)
                {
                    delete internProp;
                }
            }
            dataToWrappedData()->erase(it);
        }
        m_dataToType.erase(type_it);
    }

    AData* AVariantDataManager::createData()
    {
        if (!m_creatingData)
            return 0;

        AVariantData* data = new AVariantData(this);
        m_dataToType.insert(data, qMakePair(data, m_dataType));

        return data;
    }

    // ----------------------------------------------------------------------------------------------------

    AVariantEditorFactory::AVariantEditorFactory(QObject* parent)
        : AAbstractEditorFactory<AVariantDataManager>(parent)
    {
        m_spinBoxFactory = new ASpinBoxFactory(this);
        m_factoryToType[m_spinBoxFactory] = QVariant::Int;
        m_typeToFactory[QVariant::Int] = m_spinBoxFactory;

        m_doubleSpinBoxFactory = new ADoubleSpinBoxFactory(this);
        m_factoryToType[m_doubleSpinBoxFactory] = QVariant::Double;
        m_typeToFactory[QVariant::Double] = m_doubleSpinBoxFactory;

        m_checkBoxFactory = new ACheckBoxFactory(this);
        m_factoryToType[m_checkBoxFactory] = QVariant::Bool;
        m_typeToFactory[QVariant::Bool] = m_checkBoxFactory;

        m_lineEditFactory = new ALineEditFactory(this);
        m_factoryToType[m_lineEditFactory] = QVariant::String;
        m_typeToFactory[QVariant::String] = m_lineEditFactory;

        /*m_dateEditFactory = new ADateEditFactory(this);
        m_factoryToType[m_dateEditFactory] = QVariant::Date;
        m_typeToFactory[QVariant::Date] = m_dateEditFactory;

        m_timeEditFactory = new ATimeEditFactory(this);
        m_factoryToType[m_timeEditFactory] = QVariant::Time;
        m_typeToFactory[QVariant::Time] = m_timeEditFactory;

        m_dateTimeEditFactory = new ADateTimeEditFactory(this);
        m_factoryToType[m_dateTimeEditFactory] = QVariant::DateTime;
        m_typeToFactory[QVariant::DateTime] = m_dateTimeEditFactory;

        m_keySequenceEditorFactory = new AKeySequenceEditorFactory(this);
        m_factoryToType[m_keySequenceEditorFactory] = QVariant::KeySequence;
        m_typeToFactory[QVariant::KeySequence] = m_keySequenceEditorFactory;

        m_charEditorFactory = new ACharEditorFactory(this);
        m_factoryToType[m_charEditorFactory] = QVariant::Char;
        m_typeToFactory[QVariant::Char] = m_charEditorFactory;

        m_cursorEditorFactory = new ACursorEditorFactory(this);
        m_factoryToType[m_cursorEditorFactory] = QVariant::Cursor;
        m_typeToFactory[QVariant::Cursor] = m_cursorEditorFactory;*/

        m_colorEditorFactory = new AColorEditorFactory(this);
        m_factoryToType[m_colorEditorFactory] = QVariant::Color;
        m_typeToFactory[QVariant::Color] = m_colorEditorFactory;

        m_fontEditorFactory = new AFontEditorFactory(this);
        m_factoryToType[m_fontEditorFactory] = QVariant::Font;
        m_typeToFactory[QVariant::Font] = m_fontEditorFactory;

        m_comboBoxFactory = new AEnumEditorFactory(this);
        const int enumId = AVariantDataManager::enumTypeId();
        m_factoryToType[m_comboBoxFactory] = enumId;
        m_typeToFactory[enumId] = m_comboBoxFactory;
    }

    AVariantEditorFactory::~AVariantEditorFactory()
    {
    }

    void AVariantEditorFactory::connectDataManager(AVariantDataManager* manager)
    {
        const QList<AIntDataManager*> intDataManagers = manager->findChildren<AIntDataManager*>();
        for (AIntDataManager* manager : intDataManagers)
            m_spinBoxFactory->addDataManager(manager);

        const QList<ADoubleDataManager*> doubleDataManagers = manager->findChildren<ADoubleDataManager*>();
        for (ADoubleDataManager* manager : doubleDataManagers)
            m_doubleSpinBoxFactory->addDataManager(manager);

        const QList<ABoolDataManager*> boolDataManagers = manager->findChildren<ABoolDataManager*>();
        for (ABoolDataManager* manager : boolDataManagers)
            m_checkBoxFactory->addDataManager(manager);

        const QList<AStringDataManager*> stringDataManagers = manager->findChildren<AStringDataManager*>();
        for (AStringDataManager* manager : stringDataManagers)
            m_lineEditFactory->addDataManager(manager);

        const QList<APointDataManager*> pointDataManagers = manager->findChildren<APointDataManager*>();
        for (APointDataManager* manager : pointDataManagers)
            m_spinBoxFactory->addDataManager(manager->subIntDataManager());

        const QList<ASizeDataManager*> sizeDataManagers = manager->findChildren<ASizeDataManager*>();
        for (ASizeDataManager* manager : sizeDataManagers)
            m_spinBoxFactory->addDataManager(manager->subIntDataManager());

        const QList<AEnumDataManager*> enumDataManagers = manager->findChildren<AEnumDataManager*>();
        for (AEnumDataManager* manager : enumDataManagers)
            m_comboBoxFactory->addDataManager(manager);

        const QList<AColorDataManager*> colorDataManagers = manager->findChildren<AColorDataManager*>();
        for (AColorDataManager* manager : colorDataManagers)
        {
            m_colorEditorFactory->addDataManager(manager);
            m_spinBoxFactory->addDataManager(manager->subIntDataManager());
        }

        const QList<AFontDataManager*> fontDataManagers = manager->findChildren<AFontDataManager*>();
        for (AFontDataManager* manager : fontDataManagers)
        {
            m_fontEditorFactory->addDataManager(manager);
            m_spinBoxFactory->addDataManager(manager->subIntDataManager());
            m_comboBoxFactory->addDataManager(manager->subEnumDataManager());
            m_checkBoxFactory->addDataManager(manager->subBoolDataManager());
        }

        /*const QList<ADateDataManager*> dateDataManagers = manager->findChildren<ADateDataManager*>();
        for (ADateDataManager* manager : dateDataManagers)
            m_dateEditFactory->addDataManager(manager);

        const QList<ATimeDataManager*> timeDataManagers = manager->findChildren<ATimeDataManager*>();
        for (ATimeDataManager* manager : timeDataManagers)
            m_timeEditFactory->addDataManager(manager);

        const QList<ADateTimeDataManager*> dateTimeDataManagers = manager->findChildren<ADateTimeDataManager*>();
        for (ADateTimeDataManager* manager : dateTimeDataManagers)
            m_dateTimeEditFactory->addDataManager(manager);

        const QList<AKeySequenceDataManager*> keySequenceDataManagers = manager->findChildren<AKeySequenceDataManager*>();
        for (AKeySequenceDataManager* manager : keySequenceDataManagers)
            m_keySequenceEditorFactory->addDataManager(manager);

        const QList<ACharDataManager*> charDataManagers = manager->findChildren<ACharDataManager*>();
        for (ACharDataManager* manager : charDataManagers)
            m_charEditorFactory->addDataManager(manager);

        const QList<ALocaleDataManager*> localeDataManagers = manager->findChildren<ALocaleDataManager*>();
        for (ALocaleDataManager* manager : localeDataManagers)
            m_comboBoxFactory->addDataManager(manager->subEnumDataManager());

        const QList<APointFDataManager*> pointFDataManagers = manager->findChildren<APointFDataManager*>();
        for (APointFDataManager* manager : pointFDataManagers)
            m_doubleSpinBoxFactory->addDataManager(manager->subDoubleDataManager());

        const QList<ASizeFDataManager*> sizeFDataManagers = manager->findChildren<ASizeFDataManager*>();
        for (ASizeFDataManager* manager : sizeFDataManagers)
            m_doubleSpinBoxFactory->addDataManager(manager->subDoubleDataManager());

        const QList<ARectDataManager*> rectDataManagers = manager->findChildren<ARectDataManager*>();
        for (ARectDataManager* manager : rectDataManagers)
            m_spinBoxFactory->addDataManager(manager->subIntDataManager());

        const QList<ARectFDataManager*> rectFDataManagers = manager->findChildren<ARectFDataManager*>();
        for (ARectFDataManager* manager : rectFDataManagers)
            m_doubleSpinBoxFactory->addDataManager(manager->subDoubleDataManager());

        const QList<ASizePolicyDataManager*> sizePolicyDataManagers = manager->findChildren<ASizePolicyDataManager*>();
        for (ASizePolicyDataManager* manager : sizePolicyDataManagers)
        {
            m_spinBoxFactory->addDataManager(manager->subIntDataManager());
            m_comboBoxFactory->addDataManager(manager->subEnumDataManager());
        }

        const QList<ACursorDataManager*> cursorDataManagers = manager->findChildren<ACursorDataManager*>();
        for (ACursorDataManager* manager : cursorDataManagers)
            m_cursorEditorFactory->addDataManager(manager);

        const QList<AFlagDataManager*> flagDataManagers = manager->findChildren<AFlagDataManager*>();
        for (AFlagDataManager* manager : flagDataManagers)
            m_checkBoxFactory->addDataManager(manager->subBoolDataManager());*/
    }

    QWidget* AVariantEditorFactory::createEditor(AVariantDataManager* manager, AData* data,
                                                 QWidget* parent)
    {
        const int propType = manager->dataType(data);
        AAbstractEditorFactoryBase* factory = m_typeToFactory.value(propType, 0);
        if (!factory)
            return 0;
        return factory->createEditor(wrappedData(data), parent);
    }

    void AVariantEditorFactory::disconnectDataManager(AVariantDataManager* manager)
    {
        const QList<AIntDataManager*> intDataManagers = manager->findChildren<AIntDataManager*>();
        for (AIntDataManager* manager : intDataManagers)
            m_spinBoxFactory->removeDataManager(manager);

        const QList<ADoubleDataManager*> doubleDataManagers = manager->findChildren<ADoubleDataManager*>();
        for (ADoubleDataManager* manager : doubleDataManagers)
            m_doubleSpinBoxFactory->removeDataManager(manager);

        const QList<ABoolDataManager*> boolDataManagers = manager->findChildren<ABoolDataManager*>();
        for (ABoolDataManager* manager : boolDataManagers)
            m_checkBoxFactory->removeDataManager(manager);

        const QList<AStringDataManager*> stringDataManagers = manager->findChildren<AStringDataManager*>();
        for (AStringDataManager* manager : stringDataManagers)
            m_lineEditFactory->removeDataManager(manager);

        const QList<AColorDataManager*> colorDataManagers = manager->findChildren<AColorDataManager*>();
        for (AColorDataManager* manager : colorDataManagers)
        {
            m_colorEditorFactory->removeDataManager(manager);
            m_spinBoxFactory->removeDataManager(manager->subIntDataManager());
        }

        const QList<ASizeDataManager*> sizeDataManagers = manager->findChildren<ASizeDataManager*>();
        for (ASizeDataManager* manager : sizeDataManagers)
            m_spinBoxFactory->removeDataManager(manager->subIntDataManager());

        const QList<AEnumDataManager*> enumDataManagers = manager->findChildren<AEnumDataManager*>();
        for (AEnumDataManager* manager : enumDataManagers)
            m_comboBoxFactory->removeDataManager(manager);

        const QList<AFontDataManager*> fontDataManagers = manager->findChildren<AFontDataManager*>();
        for (AFontDataManager* manager : fontDataManagers)
        {
            m_fontEditorFactory->removeDataManager(manager);
            m_spinBoxFactory->removeDataManager(manager->subIntDataManager());
            m_comboBoxFactory->removeDataManager(manager->subEnumDataManager());
            m_checkBoxFactory->removeDataManager(manager->subBoolDataManager());
        }

        /*const QList<ADateDataManager*> dateDataManagers = manager->findChildren<ADateDataManager*>();
        for (ADateDataManager* manager : dateDataManagers)
            m_dateEditFactory->removeDataManager(manager);

        const QList<ATimeDataManager*> timeDataManagers = manager->findChildren<ATimeDataManager*>();
        for (ATimeDataManager* manager : timeDataManagers)
            m_timeEditFactory->removeDataManager(manager);

        const QList<ADateTimeDataManager*> dateTimeDataManagers = manager->findChildren<ADateTimeDataManager*>();
        for (ADateTimeDataManager* manager : dateTimeDataManagers)
            m_dateTimeEditFactory->removeDataManager(manager);

        const QList<AKeySequenceDataManager*> keySequenceDataManagers = manager->findChildren<AKeySequenceDataManager*>();
        for (AKeySequenceDataManager* manager : keySequenceDataManagers)
            m_keySequenceEditorFactory->removeDataManager(manager);

        const QList<ACharDataManager*> charDataManagers = manager->findChildren<ACharDataManager*>();
        for (ACharDataManager* manager : charDataManagers)
            m_charEditorFactory->removeDataManager(manager);

        const QList<ALocaleDataManager*> localeDataManagers = manager->findChildren<ALocaleDataManager*>();
        for (ALocaleDataManager* manager : localeDataManagers)
            m_comboBoxFactory->removeDataManager(manager->subEnumDataManager());

        const QList<APointFDataManager*> pointFDataManagers = manager->findChildren<APointFDataManager*>();
        for (APointFDataManager* manager : pointFDataManagers)
            m_doubleSpinBoxFactory->removeDataManager(manager->subDoubleDataManager());

        const QList<ARectDataManager*> rectDataManagers = manager->findChildren<ARectDataManager*>();
        for (ARectDataManager* manager : rectDataManagers)
            m_spinBoxFactory->removeDataManager(manager->subIntDataManager());

        const QList<ARectFDataManager*> rectFDataManagers = manager->findChildren<ARectFDataManager*>();
        for (ARectFDataManager* manager : rectFDataManagers)
            m_doubleSpinBoxFactory->removeDataManager(manager->subDoubleDataManager());

        const QList<ASizePolicyDataManager*> sizePolicyDataManagers = manager->findChildren<ASizePolicyDataManager*>();
        for (ASizePolicyDataManager* manager : sizePolicyDataManagers)
        {
            m_spinBoxFactory->removeDataManager(manager->subIntDataManager());
            m_comboBoxFactory->removeDataManager(manager->subEnumDataManager());
        }

        const QList<ACursorDataManager*> cursorDataManagers = manager->findChildren<ACursorDataManager*>();
        for (ACursorDataManager* manager : cursorDataManagers)
            m_cursorEditorFactory->removeDataManager(manager);

        const QList<AFlagDataManager*> flagDataManagers = manager->findChildren<AFlagDataManager*>();
        for (AFlagDataManager* manager : flagDataManagers)
            m_checkBoxFactory->removeDataManager(manager->subBoolDataManager());*/
    }
}