/****************************************************************************
 * @file    ADataManager.cpp
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
#include "ADataManager.h"
#include <QtCore/QDateTime>
#include <QtCore/QLocale>
#include <QtCore/QMap>
#include <QtCore/QTimer>
#include <QtGui/QIcon>
#include <QtCore/QMetaEnum>
#include <QtGui/QFontDatabase>
#include <QtWidgets/QStyleOption>
#include <QtWidgets/QStyle>
#include <QtWidgets/QApplication>
#include <QtGui/QPainter>
#include <QtWidgets/QLabel>

#include <limits>
#include <limits.h>
#include <float.h>

#if defined(Q_CC_MSVC)
#pragma warning(disable : 4786) /* MS VS 6: truncating debug info after 255 characters */
#endif

APROCH_NAMESPACE_BEGIN

template <class PrivateData, class Value>
static void setSimpleMinimumData(PrivateData* metaData, const Value& minVal)
{
    metaData->minVal = minVal;
    if (metaData->maxVal < metaData->minVal)
        metaData->maxVal = metaData->minVal;

    if (metaData->val < metaData->minVal)
        metaData->val = metaData->minVal;
}

template <class PrivateData, class Value>
static void setSimpleMaximumData(PrivateData* metaData, const Value& maxVal)
{
    metaData->maxVal = maxVal;
    if (metaData->minVal > metaData->maxVal)
        metaData->minVal = metaData->maxVal;

    if (metaData->val > metaData->maxVal)
        metaData->val = metaData->maxVal;
}

template <class PrivateData, class Value>
static void setSizeMinimumData(PrivateData* metaData, const Value& newMinVal)
{
    metaData->minVal = newMinVal;
    if (metaData->maxVal.width() < metaData->minVal.width())
        metaData->maxVal.setWidth(metaData->minVal.width());
    if (metaData->maxVal.height() < metaData->minVal.height())
        metaData->maxVal.setHeight(metaData->minVal.height());

    if (metaData->val.width() < metaData->minVal.width())
        metaData->val.setWidth(metaData->minVal.width());
    if (metaData->val.height() < metaData->minVal.height())
        metaData->val.setHeight(metaData->minVal.height());
}

template <class PrivateData, class Value>
static void setSizeMaximumData(PrivateData* metaData, const Value& newMaxVal)
{
    metaData->maxVal = newMaxVal;
    if (metaData->minVal.width() > metaData->maxVal.width())
        metaData->minVal.setWidth(metaData->maxVal.width());
    if (metaData->minVal.height() > metaData->maxVal.height())
        metaData->minVal.setHeight(metaData->maxVal.height());

    if (metaData->val.width() > metaData->maxVal.width())
        metaData->val.setWidth(metaData->maxVal.width());
    if (metaData->val.height() > metaData->maxVal.height())
        metaData->val.setHeight(metaData->maxVal.height());
}

namespace
{
    template <class Value>
    void orderBorders(Value& minVal, Value& maxVal)
    {
        if (minVal > maxVal)
            qSwap(minVal, maxVal);
    }

    template <class Value>
    static void orderSizeBorders(Value& minVal, Value& maxVal)
    {
        Value fromSize = minVal;
        Value toSize = maxVal;
        if (fromSize.width() > toSize.width())
        {
            fromSize.setWidth(maxVal.width());
            toSize.setWidth(minVal.width());
        }
        if (fromSize.height() > toSize.height())
        {
            fromSize.setHeight(maxVal.height());
            toSize.setHeight(minVal.height());
        }
        minVal = fromSize;
        maxVal = toSize;
    }

    void orderBorders(QSize& minVal, QSize& maxVal)
    {
        orderSizeBorders(minVal, maxVal);
    }

    void orderBorders(QSizeF& minVal, QSizeF& maxVal)
    {
        orderSizeBorders(minVal, maxVal);
    }
}
////////

template <class Value, class PrivateData>
static Value getData(const QMap<const AData*, PrivateData>& dataMap,
                     Value PrivateData::* metaData,
                     const AData* data, const Value& defaultValue = Value())
{
    const auto it = dataMap.constFind(data);
    if (it == dataMap.constEnd())
        return defaultValue;
    return it.value().*metaData;
}

template <class Value, class PrivateData>
static Value getValue(const QMap<const AData*, PrivateData>& dataMap,
                      const AData* data, const Value& defaultValue = Value())
{
    return getData<Value>(dataMap, &PrivateData::val, data, defaultValue);
}

template <class Value, class PrivateData>
static Value getMinimum(const QMap<const AData*, PrivateData>& dataMap,
                        const AData* data, const Value& defaultValue = Value())
{
    return getData<Value>(dataMap, &PrivateData::minVal, data, defaultValue);
}

template <class Value, class PrivateData>
static Value getMaximum(const QMap<const AData*, PrivateData>& dataMap,
                        const AData* data, const Value& defaultValue = Value())
{
    return getData<Value>(dataMap, &PrivateData::maxVal, data, defaultValue);
}

template <class ValueChangeParameter, class Value, class DataManager>
static void setSimpleValue(QMap<const AData*, Value>& dataMap,
                           DataManager* manager,
                           void (DataManager::* dataChangedSignal)(AData*),
                           void (DataManager::* valueChangedSignal)(AData*, ValueChangeParameter),
                           AData* data, const Value& val)
{
    const auto it = dataMap.find(data);
    if (it == dataMap.end())
        return;

    if (it.value() == val)
        return;

    it.value() = val;

    emit(manager->*dataChangedSignal)(data);
    emit(manager->*valueChangedSignal)(data, val);
}

template <class ValueChangeParameter, class DataManagerPrivate, class DataManager, class Value>
static void setValueInRange(DataManager* manager, DataManagerPrivate* managerPrivate,
                            void (DataManager::* dataChangedSignal)(AData*),
                            void (DataManager::* valueChangedSignal)(AData*, ValueChangeParameter),
                            AData* data, const Value& val,
                            void (DataManagerPrivate::* setSubDataValue)(AData*, ValueChangeParameter))
{
    const auto it = managerPrivate->m_values.find(data);
    if (it == managerPrivate->m_values.end())
        return;

    auto& metaData = it.value();

    if (metaData.val == val)
        return;

    const Value oldVal = metaData.val;

    metaData.val = qBound(metaData.minVal, val, metaData.maxVal);

    if (metaData.val == oldVal)
        return;

    if (setSubDataValue)
        (managerPrivate->*setSubDataValue)(data, metaData.val);

    emit(manager->*dataChangedSignal)(data);
    emit(manager->*valueChangedSignal)(data, metaData.val);
}

template <class ValueChangeParameter, class DataManagerPrivate, class DataManager, class Value>
static void setBorderValues(DataManager* manager, DataManagerPrivate* managerPrivate,
                            void (DataManager::* dataChangedSignal)(AData*),
                            void (DataManager::* valueChangedSignal)(AData*, ValueChangeParameter),
                            void (DataManager::* rangeChangedSignal)(AData*, ValueChangeParameter, ValueChangeParameter),
                            AData* data, const Value& minVal, const Value& maxVal,
                            void (DataManagerPrivate::* setSubDataRange)(AData*, ValueChangeParameter, ValueChangeParameter, ValueChangeParameter))
{
    const auto it = managerPrivate->m_values.find(data);
    if (it == managerPrivate->m_values.end())
        return;

    Value fromVal = minVal;
    Value toVal = maxVal;
    orderBorders(fromVal, toVal);

    auto& metaData = it.value();

    if (metaData.minVal == fromVal && metaData.maxVal == toVal)
        return;

    const Value oldVal = metaData.val;

    metaData.setMinimumValue(fromVal);
    metaData.setMaximumValue(toVal);

    emit(manager->*rangeChangedSignal)(data, metaData.minVal, metaData.maxVal);

    if (setSubDataRange)
        (managerPrivate->*setSubDataRange)(data, metaData.minVal, metaData.maxVal, metaData.val);

    if (metaData.val == oldVal)
        return;

    emit(manager->*dataChangedSignal)(data);
    emit(manager->*valueChangedSignal)(data, metaData.val);
}

template <class ValueChangeParameter, class DataManagerPrivate, class DataManager, class Value, class PrivateData>
static void setBorderValue(DataManager* manager, DataManagerPrivate* managerPrivate,
                           void (DataManager::* dataChangedSignal)(AData*),
                           void (DataManager::* valueChangedSignal)(AData*, ValueChangeParameter),
                           void (DataManager::* rangeChangedSignal)(AData*, ValueChangeParameter, ValueChangeParameter),
                           AData* data, Value(PrivateData::* getRangeVal)() const,
                           void (PrivateData::* setRangeVal)(ValueChangeParameter), 
                           const Value& borderVal,
                           void (DataManagerPrivate::* setSubDataRange)(AData*, ValueChangeParameter, ValueChangeParameter, ValueChangeParameter))
{
    const auto it = managerPrivate->m_values.find(data);
    if (it == managerPrivate->m_values.end())
        return;

    PrivateData& metaData = it.value();

    if ((metaData.*getRangeVal)() == borderVal)
        return;

    const Value oldVal = metaData.val;

    (metaData.*setRangeVal)(borderVal);

    emit(manager->*rangeChangedSignal)(data, metaData.minVal, metaData.maxVal);

    if (setSubDataRange)
        (managerPrivate->*setSubDataRange)(data, metaData.minVal, metaData.maxVal, metaData.val);

    if (metaData.val == oldVal)
        return;

    emit(manager->*dataChangedSignal)(data);
    emit(manager->*valueChangedSignal)(data, metaData.val);
}

template <class ValueChangeParameter, class DataManagerPrivate, class DataManager, class Value, class PrivateData>
static void setMinimumValue(DataManager* manager, DataManagerPrivate* managerPrivate,
                            void (DataManager::* dataChangedSignal)(AData*),
                            void (DataManager::* valueChangedSignal)(AData*, ValueChangeParameter),
                            void (DataManager::* rangeChangedSignal)(AData*, ValueChangeParameter, ValueChangeParameter),
                            AData* data, const Value& minVal)
{
    void (DataManagerPrivate:: * setSubDataRange)(AData*, ValueChangeParameter, ValueChangeParameter, ValueChangeParameter) = 0;
    setBorderValue<ValueChangeParameter, DataManagerPrivate, DataManager, Value, PrivateData>
        (manager, managerPrivate, dataChangedSignal, valueChangedSignal, rangeChangedSignal, data, 
         &DataManagerPrivate::Data::minimumValue, &DataManagerPrivate::Data::setMinimumValue, minVal, setSubDataRange);
}

template <class ValueChangeParameter, class DataManagerPrivate, class DataManager, class Value, class PrivateData>
static void setMaximumValue(DataManager* manager, DataManagerPrivate* managerPrivate,
                            void (DataManager::* dataChangedSignal)(AData*),
                            void (DataManager::* valueChangedSignal)(AData*, ValueChangeParameter),
                            void (DataManager::* rangeChangedSignal)(AData*, ValueChangeParameter, ValueChangeParameter),
                            AData* data, const Value& maxVal)
{
    void (DataManagerPrivate:: * setSubDataRange)(AData*, ValueChangeParameter, ValueChangeParameter, ValueChangeParameter) = 0;
    setBorderValue<ValueChangeParameter, DataManagerPrivate, DataManager, Value, PrivateData>
        (manager, managerPrivate, dataChangedSignal, valueChangedSignal, rangeChangedSignal, data,
         &DataManagerPrivate::Data::maximumValue, &DataManagerPrivate::Data::setMaximumValue, maxVal, setSubDataRange);
}

class AMetaEnumWrapper : public QObject
{
    Q_OBJECT;
    Q_PROPERTY(QSizePolicy::Policy policy READ policy)
public:
    QSizePolicy::Policy policy() const
    {
        return QSizePolicy::Ignored;
    }

private:
    AMetaEnumWrapper(QObject* parent) : QObject(parent)
    {
    }
};

class AMetaEnumProvider
{
public:
    AMetaEnumProvider();

    QStringList policyEnumNames() const
    {
        return m_policyEnumNames;
    }
    QStringList languageEnumNames() const
    {
        return m_languageEnumNames;
    }
    QStringList countryEnumNames(QLocale::Language language) const
    {
        return m_countryEnumNames.value(language);
    }

    QSizePolicy::Policy indexToSizePolicy(int index) const;
    int sizePolicyToIndex(QSizePolicy::Policy policy) const;

    void indexToLocale(int languageIndex, int countryIndex, QLocale::Language* language, QLocale::Country* country) const;
    void localeToIndex(QLocale::Language language, QLocale::Country country, int* languageIndex, int* countryIndex) const;

private:
    void initLocale();

    QStringList m_policyEnumNames;
    QStringList m_languageEnumNames;
    QMap<QLocale::Language, QStringList> m_countryEnumNames;
    QMap<int, QLocale::Language> m_indexToLanguage;
    QMap<QLocale::Language, int> m_languageToIndex;
    QMap<int, QMap<int, QLocale::Country>> m_indexToCountry;
    QMap<QLocale::Language, QMap<QLocale::Country, int>> m_countryToIndex;
    QMetaEnum m_policyEnum;
};

static QList<QLocale::Country> sortCountries(const QList<QLocale::Country>& countries)
{
    QMultiMap<QString, QLocale::Country> nameToCountry;
    for (QLocale::Country country : countries)
        nameToCountry.insert(QLocale::countryToString(country), country);
    return nameToCountry.values();
}

void AMetaEnumProvider::initLocale()
{
    QMultiMap<QString, QLocale::Language> nameToLanguage;
    for (int l = QLocale::C, last = QLocale::LastLanguage; l <= last; ++l)
    {
        const QLocale::Language language = static_cast<QLocale::Language>(l);
        QLocale locale(language);
        if (locale.language() == language)
            nameToLanguage.insert(QLocale::languageToString(language), language);
    }

    const QLocale system = QLocale::system();
    if (!nameToLanguage.contains(QLocale::languageToString(system.language())))
        nameToLanguage.insert(QLocale::languageToString(system.language()), system.language());

    const auto languages = nameToLanguage.values();
    for (QLocale::Language language : languages)
    {
        QList<QLocale::Country> countries;
        countries = QLocale::countriesForLanguage(language);
        if (countries.isEmpty() && language == system.language())
            countries << system.country();

        if (!countries.isEmpty() && !m_languageToIndex.contains(language))
        {
            countries = sortCountries(countries);
            int langIdx = m_languageEnumNames.count();
            m_indexToLanguage[langIdx] = language;
            m_languageToIndex[language] = langIdx;
            QStringList countryNames;
            int countryIdx = 0;
            for (QLocale::Country country : qAsConst(countries))
            {
                countryNames << QLocale::countryToString(country);
                m_indexToCountry[langIdx][countryIdx] = country;
                m_countryToIndex[language][country] = countryIdx;
                ++countryIdx;
            }
            m_languageEnumNames << QLocale::languageToString(language);
            m_countryEnumNames[language] = countryNames;
        }
    }
}

AMetaEnumProvider::AMetaEnumProvider()
{
    QMetaProperty p = AMetaEnumWrapper::staticMetaObject.property(AMetaEnumWrapper::staticMetaObject.propertyOffset() + 0);
    m_policyEnum = p.enumerator();
    const int keyCount = m_policyEnum.keyCount();
    for (int i = 0; i < keyCount; i++)
        m_policyEnumNames << QLatin1String(m_policyEnum.key(i));

    initLocale();
}

QSizePolicy::Policy AMetaEnumProvider::indexToSizePolicy(int index) const
{
    return static_cast<QSizePolicy::Policy>(m_policyEnum.value(index));
}

int AMetaEnumProvider::sizePolicyToIndex(QSizePolicy::Policy policy) const
{
    const int keyCount = m_policyEnum.keyCount();
    for (int i = 0; i < keyCount; i++)
        if (indexToSizePolicy(i) == policy)
            return i;
    return -1;
}

void AMetaEnumProvider::indexToLocale(int languageIndex, int countryIndex, QLocale::Language* language, QLocale::Country* country) const
{
    QLocale::Language l = QLocale::C;
    QLocale::Country c = QLocale::AnyCountry;
    if (m_indexToLanguage.contains(languageIndex))
    {
        l = m_indexToLanguage[languageIndex];
        if (m_indexToCountry.contains(languageIndex) && m_indexToCountry[languageIndex].contains(countryIndex))
            c = m_indexToCountry[languageIndex][countryIndex];
    }
    if (language)
        *language = l;
    if (country)
        *country = c;
}

void AMetaEnumProvider::localeToIndex(QLocale::Language language, QLocale::Country country, int* languageIndex, int* countryIndex) const
{
    int l = -1;
    int c = -1;
    if (m_languageToIndex.contains(language))
    {
        l = m_languageToIndex[language];
        if (m_countryToIndex.contains(language) && m_countryToIndex[language].contains(country))
            c = m_countryToIndex[language][country];
    }

    if (languageIndex)
        *languageIndex = l;
    if (countryIndex)
        *countryIndex = c;
}

Q_GLOBAL_STATIC(AMetaEnumProvider, metaEnumProvider);

// Numerical data manager
A_IMPLEMENTS_NUMBER_DATAMANAGER(AInt8DataManager, qint8);
A_IMPLEMENTS_NUMBER_DATAMANAGER(AUInt8DataManager, quint8);
A_IMPLEMENTS_NUMBER_DATAMANAGER(AInt16DataManager, qint16);
A_IMPLEMENTS_NUMBER_DATAMANAGER(AUInt16DataManager, quint16);
A_IMPLEMENTS_NUMBER_DATAMANAGER(AIntDataManager, qint32);
A_IMPLEMENTS_NUMBER_DATAMANAGER(AUIntDataManager, quint32);
A_IMPLEMENTS_NUMBER_DATAMANAGER(ALongLongDataManager, qint64);
A_IMPLEMENTS_NUMBER_DATAMANAGER(AULongLongDataManager, quint64);

// AFloatDataManager

class AFloatDataManagerPrivate
{
public:
    struct Data
    {
        float val{ 0 };
        float minVal{ -FLT_MAX };
        float maxVal{ FLT_MAX };
        float singleStep{ 1 };
        int decimals{ 2 };
        float minimumValue() const
        {
            return minVal;
        }
        float maximumValue() const
        {
            return maxVal;
        }
        void setMinimumValue(float newMinVal)
        {
            setSimpleMinimumData(this, newMinVal);
        }
        void setMaximumValue(float newMaxVal)
        {
            setSimpleMaximumData(this, newMaxVal);
        }
    };

    typedef QMap<const AData*, Data> DataValueMap;
    DataValueMap m_values;
};

AFloatDataManager::AFloatDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new AFloatDataManagerPrivate)
{
}

AFloatDataManager::~AFloatDataManager()
{
    clear();
}

float AFloatDataManager::value(const AData* data) const
{
    return getValue<float>(d_ptr->m_values, data, 0.0);
}

float AFloatDataManager::minimum(const AData* data) const
{
    return getMinimum<float>(d_ptr->m_values, data, 0.0);
}

float AFloatDataManager::maximum(const AData* data) const
{
    return getMaximum<float>(d_ptr->m_values, data, 0.0);
}

float AFloatDataManager::singleStep(const AData* data) const
{
    return getData<float>(d_ptr->m_values, &AFloatDataManagerPrivate::Data::singleStep, data, 0);
}

int AFloatDataManager::decimals(const AData* data) const
{
    return getData<int>(d_ptr->m_values, &AFloatDataManagerPrivate::Data::decimals, data, 0);
}

QString AFloatDataManager::toString(const AData* data) const
{
    const AFloatDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();
    return QString::number(it.value().val, 'f', it.value().decimals);
}

void AFloatDataManager::setValue(AData* data, float val)
{
    void (AFloatDataManagerPrivate:: * setSubDataValue)(AData*, float) = 0;
    setValueInRange<float, AFloatDataManagerPrivate, AFloatDataManager, float>(this, d_ptr.data(),
                                                                               &AFloatDataManager::dataChanged,
                                                                               &AFloatDataManager::valueChanged,
                                                                               data, val, setSubDataValue);
}

void AFloatDataManager::setSingleStep(AData* data, float step)
{
    const AFloatDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    AFloatDataManagerPrivate::Data metaData = it.value();
    if (step < 0)
        step = 0;

    if (metaData.singleStep == step)
        return;

    metaData.singleStep = step;
    it.value() = metaData;

    emit singleStepChanged(data, metaData.singleStep);
}

void AFloatDataManager::setDecimals(AData* data, int prec)
{
    const AFloatDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    AFloatDataManagerPrivate::Data metaData = it.value();

    if (prec > 13)
        prec = 13;
    else if (prec < 0)
        prec = 0;

    if (metaData.decimals == prec)
        return;

    metaData.decimals = prec;

    it.value() = metaData;

    emit decimalsChanged(data, metaData.decimals);
}

void AFloatDataManager::setMinimum(AData* data, float minVal)
{
    setMinimumValue<float, AFloatDataManagerPrivate, AFloatDataManager, float, AFloatDataManagerPrivate::Data>(this, d_ptr.data(),
                                                                                                               &AFloatDataManager::dataChanged,
                                                                                                               &AFloatDataManager::valueChanged,
                                                                                                               &AFloatDataManager::rangeChanged,
                                                                                                               data, minVal);
}

void AFloatDataManager::setMaximum(AData* data, float maxVal)
{
    setMaximumValue<float, AFloatDataManagerPrivate, AFloatDataManager, float, AFloatDataManagerPrivate::Data>(this, d_ptr.data(),
                                                                                                               &AFloatDataManager::dataChanged,
                                                                                                               &AFloatDataManager::valueChanged,
                                                                                                               &AFloatDataManager::rangeChanged,
                                                                                                               data, maxVal);
}

void AFloatDataManager::setRange(AData* data, float minVal, float maxVal)
{
    void (AFloatDataManagerPrivate:: * setSubDataRange)(AData*, float, float, float) = 0;
    setBorderValues<float, AFloatDataManagerPrivate, AFloatDataManager, float>(this, d_ptr.data(),
                                                                               &AFloatDataManager::dataChanged,
                                                                               &AFloatDataManager::valueChanged,
                                                                               &AFloatDataManager::rangeChanged,
                                                                               data, minVal, maxVal, setSubDataRange);
}

void AFloatDataManager::initializeData(AData* data)
{
    d_ptr->m_values[data] = AFloatDataManagerPrivate::Data();
}

void AFloatDataManager::uninitializeData(AData* data)
{
    d_ptr->m_values.remove(data);
}

// ADoubleDataManager

class ADoubleDataManagerPrivate
{
public:
    struct Data
    {
        double val{ 0 };
        double minVal{ -DBL_MAX };
        double maxVal{ DBL_MAX };
        double singleStep{ 1 };
        int decimals{ 2 };
        double minimumValue() const
        {
            return minVal;
        }
        double maximumValue() const
        {
            return maxVal;
        }
        void setMinimumValue(double newMinVal)
        {
            setSimpleMinimumData(this, newMinVal);
        }
        void setMaximumValue(double newMaxVal)
        {
            setSimpleMaximumData(this, newMaxVal);
        }
    };

    typedef QMap<const AData*, Data> DataValueMap;
    DataValueMap m_values;
};

ADoubleDataManager::ADoubleDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new ADoubleDataManagerPrivate)
{
}

ADoubleDataManager::~ADoubleDataManager()
{
    clear();
}

double ADoubleDataManager::value(const AData* data) const
{
    return getValue<double>(d_ptr->m_values, data, 0.0);
}

double ADoubleDataManager::minimum(const AData* data) const
{
    return getMinimum<double>(d_ptr->m_values, data, 0.0);
}

double ADoubleDataManager::maximum(const AData* data) const
{
    return getMaximum<double>(d_ptr->m_values, data, 0.0);
}

double ADoubleDataManager::singleStep(const AData* data) const
{
    return getData<double>(d_ptr->m_values, &ADoubleDataManagerPrivate::Data::singleStep, data, 0);
}

int ADoubleDataManager::decimals(const AData* data) const
{
    return getData<int>(d_ptr->m_values, &ADoubleDataManagerPrivate::Data::decimals, data, 0);
}

QString ADoubleDataManager::toString(const AData* data) const
{
    const ADoubleDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();
    return QString::number(it.value().val, 'f', it.value().decimals);
}

void ADoubleDataManager::setValue(AData* data, double val)
{
    void (ADoubleDataManagerPrivate:: * setSubDataValue)(AData*, double) = 0;
    setValueInRange<double, ADoubleDataManagerPrivate, ADoubleDataManager, double>(this, d_ptr.data(),
                                                                                   &ADoubleDataManager::dataChanged,
                                                                                   &ADoubleDataManager::valueChanged,
                                                                                   data, val, setSubDataValue);
}

void ADoubleDataManager::setSingleStep(AData* data, double step)
{
    const ADoubleDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    ADoubleDataManagerPrivate::Data metaData = it.value();

    if (step < 0)
        step = 0;

    if (metaData.singleStep == step)
        return;

    metaData.singleStep = step;

    it.value() = metaData;

    emit singleStepChanged(data, metaData.singleStep);
}

void ADoubleDataManager::setDecimals(AData* data, int prec)
{
    const ADoubleDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    ADoubleDataManagerPrivate::Data metaData = it.value();

    if (prec > 13)
        prec = 13;
    else if (prec < 0)
        prec = 0;

    if (metaData.decimals == prec)
        return;

    metaData.decimals = prec;

    it.value() = metaData;

    emit decimalsChanged(data, metaData.decimals);
}

void ADoubleDataManager::setMinimum(AData* data, double minVal)
{
    setMinimumValue<double, ADoubleDataManagerPrivate, ADoubleDataManager, double, ADoubleDataManagerPrivate::Data>(this, d_ptr.data(),
                                                                                                                    &ADoubleDataManager::dataChanged,
                                                                                                                    &ADoubleDataManager::valueChanged,
                                                                                                                    &ADoubleDataManager::rangeChanged,
                                                                                                                    data, minVal);
}

void ADoubleDataManager::setMaximum(AData* data, double maxVal)
{
    setMaximumValue<double, ADoubleDataManagerPrivate, ADoubleDataManager, double, ADoubleDataManagerPrivate::Data>(this, d_ptr.data(),
                                                                                                                    &ADoubleDataManager::dataChanged,
                                                                                                                    &ADoubleDataManager::valueChanged,
                                                                                                                    &ADoubleDataManager::rangeChanged,
                                                                                                                    data, maxVal);
}

void ADoubleDataManager::setRange(AData* data, double minVal, double maxVal)
{
    void (ADoubleDataManagerPrivate:: * setSubDataRange)(AData*, double, double, double) = 0;
    setBorderValues<double, ADoubleDataManagerPrivate, ADoubleDataManager, double>(this, d_ptr.data(),
                                                                                   &ADoubleDataManager::dataChanged,
                                                                                   &ADoubleDataManager::valueChanged,
                                                                                   &ADoubleDataManager::rangeChanged,
                                                                                   data, minVal, maxVal, setSubDataRange);
}

void ADoubleDataManager::initializeData(AData* data)
{
    d_ptr->m_values[data] = ADoubleDataManagerPrivate::Data();
}

void ADoubleDataManager::uninitializeData(AData* data)
{
    d_ptr->m_values.remove(data);
}

// AStringDataManager

class AStringDataManagerPrivate
{
public:
    struct Data
    {
        Data() : regExp(QString(QLatin1Char('*')), QRegularExpression::CaseInsensitiveOption)
        {
        }
        QString val;
        QRegularExpression regExp;
    };

    typedef QMap<const AData*, Data> DataValueMap;
    QMap<const AData*, Data> m_values;
};

AStringDataManager::AStringDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new AStringDataManagerPrivate)
{
}

AStringDataManager::~AStringDataManager()
{
    clear();
}

QString AStringDataManager::value(const AData* data) const
{
    return getValue<QString>(d_ptr->m_values, data);
}

QRegularExpression AStringDataManager::regExp(const AData* data) const
{
    return getData<QRegularExpression>(d_ptr->m_values, &AStringDataManagerPrivate::Data::regExp, data, QRegularExpression());
}

QString AStringDataManager::toString(const AData* data) const
{
    const AStringDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();
    return it.value().val;
}

void AStringDataManager::setValue(AData* data, const QString& val)
{
    const AStringDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    AStringDataManagerPrivate::Data metaData = it.value();

    if (metaData.val == val)
        return;

    if (metaData.regExp.isValid() && !metaData.regExp.match(val).hasMatch())
        return;

    metaData.val = val;

    it.value() = metaData;

    emit dataChanged(data);
    emit valueChanged(data, metaData.val);
}

void AStringDataManager::setRegExp(AData* data, const QRegularExpression& regExp)
{
    const AStringDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    AStringDataManagerPrivate::Data metaData = it.value();

    if (metaData.regExp == regExp)
        return;

    metaData.regExp = regExp;

    it.value() = metaData;

    emit regExpChanged(data, metaData.regExp);
}

void AStringDataManager::initializeData(AData* data)
{
    d_ptr->m_values[data] = AStringDataManagerPrivate::Data();
}

void AStringDataManager::uninitializeData(AData* data)
{
    d_ptr->m_values.remove(data);
}

// AStringListDataManager

class AStringListDataManagerPrivate
{
public:
    struct Data
    {
        QStringList val;
    };

    typedef QMap<const AData*, Data> DataValueMap;
    QMap<const AData*, Data> m_values;
};

AStringListDataManager::AStringListDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new AStringListDataManagerPrivate)
{
}

AStringListDataManager::~AStringListDataManager()
{
    clear();
}

QStringList AStringListDataManager::value(const AData* data) const
{
    return getValue<QStringList>(d_ptr->m_values, data);
}

QString AStringListDataManager::toString(const AData* data) const
{
    const AStringListDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();
    
    const QStringList& val = it.value().val;
    if (val.isEmpty())
        return QString("()");

    QString str('(');
    for (int i = 0; i < val.size() - 1; ++i)
        str += val.at(i) + AStr(", ");
    
    str += val.back() + ')';

    return str;
}

void AStringListDataManager::setValue(AData* data, const QStringList& val)
{
    const AStringListDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    AStringListDataManagerPrivate::Data metaData = it.value();

    if (metaData.val == val)
        return;

    metaData.val = val;

    it.value() = metaData;

    emit dataChanged(data);
    emit valueChanged(data, metaData.val);
}

void AStringListDataManager::initializeData(AData* data)
{
    d_ptr->m_values[data] = AStringListDataManagerPrivate::Data();
}

void AStringListDataManager::uninitializeData(AData* data)
{
    d_ptr->m_values.remove(data);
}

// ABoolDataManager
//     Return an icon containing a check box indicator
static QIcon drawCheckBox(bool value)
{
    QStyleOptionButton opt;
    opt.state |= value ? QStyle::State_On : QStyle::State_Off;
    opt.state |= QStyle::State_Enabled;
    const QStyle* style = QApplication::style();
    // Figure out size of an indicator and make sure it is not scaled down in a list view item
    // by making the pixmap as big as a list view icon and centering the indicator in it.
    // (if it is smaller, it can't be helped)
    const int indicatorWidth = style->pixelMetric(QStyle::PM_IndicatorWidth, &opt);
    const int indicatorHeight = style->pixelMetric(QStyle::PM_IndicatorHeight, &opt);
    const int listViewIconSize = indicatorWidth;
    const int pixmapWidth = indicatorWidth;
    const int pixmapHeight = qMax(indicatorHeight, listViewIconSize);

    opt.rect = QRect(0, 0, indicatorWidth, indicatorHeight);
    QPixmap pixmap = QPixmap(pixmapWidth, pixmapHeight);
    pixmap.fill(Qt::transparent);
    {
        // Center?
        const int xoff = (pixmapWidth > indicatorWidth) ? (pixmapWidth - indicatorWidth) / 2 : 0;
        const int yoff = (pixmapHeight > indicatorHeight) ? (pixmapHeight - indicatorHeight) / 2 : 0;
        QPainter painter(&pixmap);
        painter.translate(xoff, yoff);
        style->drawPrimitive(QStyle::PE_IndicatorCheckBox, &opt, &painter);
    }
    return QIcon(pixmap);
}

class ABoolDataManagerPrivate
{
    ABoolDataManager* q_ptr = nullptr;
    Q_DECLARE_PUBLIC(ABoolDataManager)
public:
    ABoolDataManagerPrivate();

    QMap<const AData*, bool> m_values;
    const QIcon m_checkedIcon;
    const QIcon m_uncheckedIcon;
};

ABoolDataManagerPrivate::ABoolDataManagerPrivate() : m_checkedIcon(drawCheckBox(true)),
m_uncheckedIcon(drawCheckBox(false))
{
}

ABoolDataManager::ABoolDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new ABoolDataManagerPrivate)
{
    d_ptr->q_ptr = this;
}

ABoolDataManager::~ABoolDataManager()
{
    clear();
}

bool ABoolDataManager::value(const AData* data) const
{
    return d_ptr->m_values.value(data, false);
}

QString ABoolDataManager::toString(const AData* data) const
{
    const QMap<const AData*, bool>::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();

    static const QString trueText = tr("True");
    static const QString falseText = tr("False");
    return it.value() ? trueText : falseText;
}

QIcon ABoolDataManager::valueIcon(const AData* data) const
{
    const QMap<const AData*, bool>::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QIcon();

    return it.value() ? d_ptr->m_checkedIcon : d_ptr->m_uncheckedIcon;
}

void ABoolDataManager::setValue(AData* data, bool val)
{
    setSimpleValue<bool, bool, ABoolDataManager>(d_ptr->m_values, this,
                                                 &ABoolDataManager::dataChanged,
                                                 &ABoolDataManager::valueChanged,
                                                 data, val);
}

void ABoolDataManager::initializeData(AData* data)
{
    d_ptr->m_values[data] = false;
}

void ABoolDataManager::uninitializeData(AData* data)
{
    d_ptr->m_values.remove(data);
}

// ADateDataManager

class ADateDataManagerPrivate
{
    ADateDataManager* q_ptr = nullptr;
    Q_DECLARE_PUBLIC(ADateDataManager)
public:
    explicit ADateDataManagerPrivate(ADateDataManager* q);

    struct Data
    {
        QDate val{ QDate::currentDate() };
        QDate minVal{ QDate(1752, 9, 14) };
        QDate maxVal{ QDate(9999, 12, 31) };
        QDate minimumValue() const
        {
            return minVal;
        }
        QDate maximumValue() const
        {
            return maxVal;
        }
        void setMinimumValue(const QDate& newMinVal)
        {
            setSimpleMinimumData(this, newMinVal);
        }
        void setMaximumValue(const QDate& newMaxVal)
        {
            setSimpleMaximumData(this, newMaxVal);
        }
    };

    QString m_format;

    typedef QMap<const AData*, Data> DataValueMap;
    QMap<const AData*, Data> m_values;
};

ADateDataManagerPrivate::ADateDataManagerPrivate(ADateDataManager* q) : q_ptr(q),
m_format(ATimeToolkit::defaultDateFormat())
{
}

ADateDataManager::ADateDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new ADateDataManagerPrivate(this))
{
}

ADateDataManager::~ADateDataManager()
{
    clear();
}

QDate ADateDataManager::value(const AData* data) const
{
    return getValue<QDate>(d_ptr->m_values, data);
}

QDate ADateDataManager::minimum(const AData* data) const
{
    return getMinimum<QDate>(d_ptr->m_values, data);
}

QDate ADateDataManager::maximum(const AData* data) const
{
    return getMaximum<QDate>(d_ptr->m_values, data);
}

QString ADateDataManager::toString(const AData* data) const
{
    const ADateDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();
    return it.value().val.toString(d_ptr->m_format);
}

void ADateDataManager::setValue(AData* data, const QDate& val)
{
    void (ADateDataManagerPrivate:: * setSubDataValue)(AData*, const QDate&) = 0;
    setValueInRange<const QDate&, ADateDataManagerPrivate, ADateDataManager, const QDate>(this, d_ptr.data(),
                                                                                          &ADateDataManager::dataChanged,
                                                                                          &ADateDataManager::valueChanged,
                                                                                          data, val, setSubDataValue);
}

void ADateDataManager::setMinimum(AData* data, const QDate& minVal)
{
    setMinimumValue<const QDate&, ADateDataManagerPrivate, ADateDataManager, QDate, ADateDataManagerPrivate::Data>(this, d_ptr.data(),
                                                                                                                   &ADateDataManager::dataChanged,
                                                                                                                   &ADateDataManager::valueChanged,
                                                                                                                   &ADateDataManager::rangeChanged,
                                                                                                                   data, minVal);
}

void ADateDataManager::setMaximum(AData* data, const QDate& maxVal)
{
    setMaximumValue<const QDate&, ADateDataManagerPrivate, ADateDataManager, QDate, ADateDataManagerPrivate::Data>(this, d_ptr.data(),
                                                                                                                   &ADateDataManager::dataChanged,
                                                                                                                   &ADateDataManager::valueChanged,
                                                                                                                   &ADateDataManager::rangeChanged,
                                                                                                                   data, maxVal);
}

void ADateDataManager::setRange(AData* data, const QDate& minVal, const QDate& maxVal)
{
    void (ADateDataManagerPrivate:: * setSubDataRange)(AData*, const QDate&,
                                                       const QDate&, const QDate&) = 0;
    setBorderValues<const QDate&, ADateDataManagerPrivate, ADateDataManager, QDate>(this, d_ptr.data(),
                                                                                    &ADateDataManager::dataChanged,
                                                                                    &ADateDataManager::valueChanged,
                                                                                    &ADateDataManager::rangeChanged,
                                                                                    data, minVal, maxVal, setSubDataRange);
}

void ADateDataManager::initializeData(AData* data)
{
    d_ptr->m_values[data] = ADateDataManagerPrivate::Data();
}

void ADateDataManager::uninitializeData(AData* data)
{
    d_ptr->m_values.remove(data);
}

// ATimeDataManager

class ATimeDataManagerPrivate
{
    ATimeDataManager* q_ptr = nullptr;
    Q_DECLARE_PUBLIC(ATimeDataManager)
public:
    explicit ATimeDataManagerPrivate(ATimeDataManager* q);

    const QString m_format;

    typedef QMap<const AData*, QTime> DataValueMap;
    DataValueMap m_values;
};

ATimeDataManagerPrivate::ATimeDataManagerPrivate(ATimeDataManager* q) : q_ptr(q),
m_format(ATimeToolkit::defaultTimeFormat())
{
}

ATimeDataManager::ATimeDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new ATimeDataManagerPrivate(this))
{
}

ATimeDataManager::~ATimeDataManager()
{
    clear();
}

QTime ATimeDataManager::value(const AData* data) const
{
    return d_ptr->m_values.value(data, QTime());
}

QString ATimeDataManager::toString(const AData* data) const
{
    const ATimeDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();
    return it.value().toString(d_ptr->m_format);
}

void ATimeDataManager::setValue(AData* data, const QTime& val)
{
    setSimpleValue<const QTime&, QTime, ATimeDataManager>(d_ptr->m_values, this,
                                                          &ATimeDataManager::dataChanged,
                                                          &ATimeDataManager::valueChanged,
                                                          data, val);
}

void ATimeDataManager::initializeData(AData* data)
{
    d_ptr->m_values[data] = QTime::currentTime();
}

void ATimeDataManager::uninitializeData(AData* data)
{
    d_ptr->m_values.remove(data);
}

// ADateTimeDataManager

class ADateTimeDataManagerPrivate
{
    ADateTimeDataManager* q_ptr = nullptr;
    Q_DECLARE_PUBLIC(ADateTimeDataManager)
public:
    explicit ADateTimeDataManagerPrivate(ADateTimeDataManager* q);

    const QString m_format;

    typedef QMap<const AData*, QDateTime> DataValueMap;
    DataValueMap m_values;
};

ADateTimeDataManagerPrivate::ADateTimeDataManagerPrivate(ADateTimeDataManager* q) : q_ptr(q),
m_format(ATimeToolkit::defaultDateTimeFormat())
{
}

ADateTimeDataManager::ADateTimeDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new ADateTimeDataManagerPrivate(this))
{
}

ADateTimeDataManager::~ADateTimeDataManager()
{
    clear();
}

QDateTime ADateTimeDataManager::value(const AData* data) const
{
    return d_ptr->m_values.value(data, QDateTime());
}

QString ADateTimeDataManager::toString(const AData* data) const
{
    const ADateTimeDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();
    return it.value().toString(d_ptr->m_format);
}

void ADateTimeDataManager::setValue(AData* data, const QDateTime& val)
{
    setSimpleValue<const QDateTime&, QDateTime, ADateTimeDataManager>(d_ptr->m_values, this,
                                                                      &ADateTimeDataManager::dataChanged,
                                                                      &ADateTimeDataManager::valueChanged,
                                                                      data, val);
}

void ADateTimeDataManager::initializeData(AData* data)
{
    d_ptr->m_values[data] = QDateTime::currentDateTime();
}

void ADateTimeDataManager::uninitializeData(AData* data)
{
    d_ptr->m_values.remove(data);
}

// AKeySequenceDataManager

class AKeySequenceDataManagerPrivate
{
    AKeySequenceDataManager* q_ptr = nullptr;
    Q_DECLARE_PUBLIC(AKeySequenceDataManager)
public:
    QString m_format;

    typedef QMap<const AData*, QKeySequence> DataValueMap;
    DataValueMap m_values;
};

AKeySequenceDataManager::AKeySequenceDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new AKeySequenceDataManagerPrivate)
{
    d_ptr->q_ptr = this;
}

AKeySequenceDataManager::~AKeySequenceDataManager()
{
    clear();
}

QKeySequence AKeySequenceDataManager::value(const AData* data) const
{
    return d_ptr->m_values.value(data, QKeySequence());
}

QString AKeySequenceDataManager::toString(const AData* data) const
{
    const AKeySequenceDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();
    return it.value().toString(QKeySequence::NativeText);
}

void AKeySequenceDataManager::setValue(AData* data, const QKeySequence& val)
{
    setSimpleValue<const QKeySequence&, QKeySequence, AKeySequenceDataManager>(d_ptr->m_values, this,
                                                                               &AKeySequenceDataManager::dataChanged,
                                                                               &AKeySequenceDataManager::valueChanged,
                                                                               data, val);
}

void AKeySequenceDataManager::initializeData(AData* data)
{
    d_ptr->m_values[data] = QKeySequence();
}

void AKeySequenceDataManager::uninitializeData(AData* data)
{
    d_ptr->m_values.remove(data);
}

// ACharDataManager

class ACharDataManagerPrivate
{
    ACharDataManager* q_ptr = nullptr;
    Q_DECLARE_PUBLIC(ACharDataManager)
public:
    typedef QMap<const AData*, QChar> DataValueMap;
    DataValueMap m_values;
};

ACharDataManager::ACharDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new ACharDataManagerPrivate)
{
    d_ptr->q_ptr = this;
}

ACharDataManager::~ACharDataManager()
{
    clear();
}

QChar ACharDataManager::value(const AData* data) const
{
    return d_ptr->m_values.value(data, QChar());
}

QString ACharDataManager::toString(const AData* data) const
{
    const ACharDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();
    const QChar c = it.value();
    return c.isNull() ? QString() : QString(c);
}

void ACharDataManager::setValue(AData* data, const QChar& val)
{
    setSimpleValue<const QChar&, QChar, ACharDataManager>(d_ptr->m_values, this,
                                                          &ACharDataManager::dataChanged,
                                                          &ACharDataManager::valueChanged,
                                                          data, val);
}

void ACharDataManager::initializeData(AData* data)
{
    d_ptr->m_values[data] = QChar();
}

void ACharDataManager::uninitializeData(AData* data)
{
    d_ptr->m_values.remove(data);
}

// ALocaleDataManager

class ALocaleDataManagerPrivate
{
    ALocaleDataManager* q_ptr = nullptr;
    Q_DECLARE_PUBLIC(ALocaleDataManager)
public:
    ALocaleDataManagerPrivate()
    {
    }

    typedef QMap<const AData*, QLocale> DataValueMap;
    DataValueMap m_values;

    AEnumDataManager* m_enumDataManager = nullptr;

    QMap<const AData*, AData*> m_dataToLanguage;
    QMap<const AData*, AData*> m_dataToCountry;

    QMap<const AData*, AData*> m_languageToData;
    QMap<const AData*, AData*> m_countryToData;
};


ALocaleDataManager::ALocaleDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new ALocaleDataManagerPrivate)
{
    d_ptr->q_ptr = this;

    d_ptr->m_enumDataManager = new AEnumDataManager(this);
    connect(d_ptr->m_enumDataManager, SIGNAL(valueChanged(AData*, int)),
            this, SLOT(onEnumChanged(AData*, int)));

    connect(d_ptr->m_enumDataManager, SIGNAL(dataDestroyed(AData*)),
            this, SLOT(onDataDestroyed(AData*)));
}

ALocaleDataManager::~ALocaleDataManager()
{
    clear();
}

AEnumDataManager* ALocaleDataManager::subEnumDataManager() const
{
    return d_ptr->m_enumDataManager;
}

QLocale ALocaleDataManager::value(const AData* data) const
{
    return d_ptr->m_values.value(data, QLocale());
}

QString ALocaleDataManager::toString(const AData* data) const
{
    const ALocaleDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();

    const QLocale loc = it.value();

    int langIdx = 0;
    int countryIdx = 0;
    const AMetaEnumProvider* me = metaEnumProvider();
    me->localeToIndex(loc.language(), loc.country(), &langIdx, &countryIdx);
    if (langIdx < 0)
    {
        qWarning("ALocaleDataManager::toString: Unknown language %d", loc.language());
        return tr("<Invalid>");
    }
    const QString languageName = me->languageEnumNames().at(langIdx);
    if (countryIdx < 0)
    {
        qWarning("ALocaleDataManager::toString: Unknown country %d for %s", loc.country(), qPrintable(languageName));
        return languageName;
    }
    const QString countryName = me->countryEnumNames(loc.language()).at(countryIdx);
    return tr("%1, %2").arg(languageName, countryName);
}

void ALocaleDataManager::setValue(AData* data, const QLocale& val)
{
    const ALocaleDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    const QLocale loc = it.value();
    if (loc == val)
        return;

    it.value() = val;

    int langIdx = 0;
    int countryIdx = 0;
    metaEnumProvider()->localeToIndex(val.language(), val.country(), &langIdx, &countryIdx);
    if (loc.language() != val.language())
    {
        d_ptr->m_enumDataManager->setValue(d_ptr->m_dataToLanguage.value(data), langIdx);
        d_ptr->m_enumDataManager->setEnumNames(d_ptr->m_dataToCountry.value(data),
                                               metaEnumProvider()->countryEnumNames(val.language()));
    }
    d_ptr->m_enumDataManager->setValue(d_ptr->m_dataToCountry.value(data), countryIdx);

    emit dataChanged(data);
    emit valueChanged(data, val);
}

void ALocaleDataManager::initializeData(AData* data)
{
    QLocale val;
    d_ptr->m_values[data] = val;

    int langIdx = 0;
    int countryIdx = 0;
    metaEnumProvider()->localeToIndex(val.language(), val.country(), &langIdx, &countryIdx);

    AData* languageProp = d_ptr->m_enumDataManager->addData();
    languageProp->setName(tr("Language"));
    d_ptr->m_enumDataManager->setEnumNames(languageProp, metaEnumProvider()->languageEnumNames());
    d_ptr->m_enumDataManager->setValue(languageProp, langIdx);
    d_ptr->m_dataToLanguage[data] = languageProp;
    d_ptr->m_languageToData[languageProp] = data;
    data->addSubData(languageProp);

    AData* countryProp = d_ptr->m_enumDataManager->addData();
    countryProp->setName(tr("Country"));
    d_ptr->m_enumDataManager->setEnumNames(countryProp, metaEnumProvider()->countryEnumNames(val.language()));
    d_ptr->m_enumDataManager->setValue(countryProp, countryIdx);
    d_ptr->m_dataToCountry[data] = countryProp;
    d_ptr->m_countryToData[countryProp] = data;
    data->addSubData(countryProp);
}

void ALocaleDataManager::uninitializeData(AData* data)
{
    AData* languageProp = d_ptr->m_dataToLanguage[data];
    if (languageProp)
    {
        d_ptr->m_languageToData.remove(languageProp);
        delete languageProp;
    }
    d_ptr->m_dataToLanguage.remove(data);

    AData* countryProp = d_ptr->m_dataToCountry[data];
    if (countryProp)
    {
        d_ptr->m_countryToData.remove(countryProp);
        delete countryProp;
    }
    d_ptr->m_dataToCountry.remove(data);

    d_ptr->m_values.remove(data);
}

void ALocaleDataManager::onEnumChanged(AData* data, int value)
{
    if (AData* prop = d_ptr->m_languageToData.value(data, 0))
    {
        const QLocale loc = d_ptr->m_values[prop];
        QLocale::Language newLanguage = loc.language();
        QLocale::Country newCountry = loc.country();
        metaEnumProvider()->indexToLocale(value, 0, &newLanguage, 0);
        QLocale newLoc(newLanguage, newCountry);
        setValue(prop, newLoc);
    }
    else if (AData* prop = d_ptr->m_countryToData.value(data, 0))
    {
        const QLocale loc = d_ptr->m_values[prop];
        QLocale::Language newLanguage = loc.language();
        QLocale::Country newCountry = loc.country();
        metaEnumProvider()->indexToLocale(d_ptr->m_enumDataManager->value(d_ptr->m_dataToLanguage.value(prop)),
                                          value, &newLanguage, &newCountry);
        QLocale newLoc(newLanguage, newCountry);
        setValue(prop, newLoc);
    }
}

void ALocaleDataManager::onDataDestroyed(AData* data)
{
    if (AData* subProp = d_ptr->m_languageToData.value(data, 0))
    {
        d_ptr->m_dataToLanguage[subProp] = 0;
        d_ptr->m_languageToData.remove(data);
    }
    else if (AData* subProp = d_ptr->m_countryToData.value(data, 0))
    {
        d_ptr->m_dataToCountry[subProp] = 0;
        d_ptr->m_countryToData.remove(data);
    }
}

// APointDataManager

class APointDataManagerPrivate
{
    APointDataManager* q_ptr = nullptr;
    Q_DECLARE_PUBLIC(APointDataManager)
public:
    typedef QMap<const AData*, QPoint> DataValueMap;
    DataValueMap m_values;

    AIntDataManager* m_intDataManager = nullptr;

    QMap<const AData*, AData*> m_dataToX;
    QMap<const AData*, AData*> m_dataToY;

    QMap<const AData*, AData*> m_xToData;
    QMap<const AData*, AData*> m_yToData;
};

APointDataManager::APointDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new APointDataManagerPrivate)
{
    d_ptr->q_ptr = this;

    d_ptr->m_intDataManager = new AIntDataManager(this);
    connect(d_ptr->m_intDataManager, SIGNAL(valueChanged(AData*, int)), this, SLOT(onIntChanged(AData*, int)));
    connect(d_ptr->m_intDataManager, SIGNAL(dataDestroyed(AData*)), this, SLOT(onDataDestroyed(AData*)));
}

APointDataManager::~APointDataManager()
{
    clear();
}

AIntDataManager* APointDataManager::subIntDataManager() const
{
    return d_ptr->m_intDataManager;
}

QPoint APointDataManager::value(const AData* data) const
{
    return d_ptr->m_values.value(data, QPoint());
}

QString APointDataManager::toString(const AData* data) const
{
    const APointDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();
    const QPoint v = it.value();
    return tr("(%1, %2)").arg(QString::number(v.x())).arg(QString::number(v.y()));
}

void APointDataManager::setValue(AData* data, const QPoint& val)
{
    const APointDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    if (it.value() == val)
        return;

    it.value() = val;
    d_ptr->m_intDataManager->setValue(d_ptr->m_dataToX[data], val.x());
    d_ptr->m_intDataManager->setValue(d_ptr->m_dataToY[data], val.y());

    emit dataChanged(data);
    emit valueChanged(data, val);
}

void APointDataManager::initializeData(AData* data)
{
    d_ptr->m_values[data] = QPoint(0, 0);

    AData* xProp = d_ptr->m_intDataManager->addData();
    xProp->setName(tr("X"));
    d_ptr->m_intDataManager->setValue(xProp, 0);
    d_ptr->m_dataToX[data] = xProp;
    d_ptr->m_xToData[xProp] = data;
    data->addSubData(xProp);

    AData* yProp = d_ptr->m_intDataManager->addData();
    yProp->setName(tr("Y"));
    d_ptr->m_intDataManager->setValue(yProp, 0);
    d_ptr->m_dataToY[data] = yProp;
    d_ptr->m_yToData[yProp] = data;
    data->addSubData(yProp);
}

void APointDataManager::uninitializeData(AData* data)
{
    AData* xProp = d_ptr->m_dataToX[data];
    if (xProp)
    {
        d_ptr->m_xToData.remove(xProp);
        delete xProp;
    }
    d_ptr->m_dataToX.remove(data);

    AData* yProp = d_ptr->m_dataToY[data];
    if (yProp)
    {
        d_ptr->m_yToData.remove(yProp);
        delete yProp;
    }
    d_ptr->m_dataToY.remove(data);

    d_ptr->m_values.remove(data);
}

void APointDataManager::onIntChanged(AData* data, int value)
{
    if (AData* xprop = d_ptr->m_xToData.value(data, 0))
    {
        QPoint p = d_ptr->m_values[xprop];
        p.setX(value);
        setValue(xprop, p);
    }
    else if (AData* yprop = d_ptr->m_yToData.value(data, 0))
    {
        QPoint p = d_ptr->m_values[yprop];
        p.setY(value);
        setValue(yprop, p);
    }
}

void APointDataManager::onDataDestroyed(AData* data)
{
    if (AData* pointProp = d_ptr->m_xToData.value(data, 0))
    {
        d_ptr->m_dataToX[pointProp] = 0;
        d_ptr->m_xToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_yToData.value(data, 0))
    {
        d_ptr->m_dataToY[pointProp] = 0;
        d_ptr->m_yToData.remove(data);
    }
}

// APointFDataManager

class APointFDataManagerPrivate
{
    APointFDataManager* q_ptr = nullptr;
    Q_DECLARE_PUBLIC(APointFDataManager)
public:
    struct Data
    {
        QPointF val;
        int decimals{ 2 };
    };

    typedef QMap<const AData*, Data> DataValueMap;
    DataValueMap m_values;

    ADoubleDataManager* m_doubleDataManager = nullptr;

    QMap<const AData*, AData*> m_dataToX;
    QMap<const AData*, AData*> m_dataToY;

    QMap<const AData*, AData*> m_xToData;
    QMap<const AData*, AData*> m_yToData;
};

APointFDataManager::APointFDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new APointFDataManagerPrivate)
{
    d_ptr->q_ptr = this;

    d_ptr->m_doubleDataManager = new ADoubleDataManager(this);
    connect(d_ptr->m_doubleDataManager, SIGNAL(valueChanged(AData*, double)),
            this, SLOT(onDoubleChanged(AData*, double)));
    connect(d_ptr->m_doubleDataManager, SIGNAL(dataDestroyed(AData*)),
            this, SLOT(onDataDestroyed(AData*)));
}

APointFDataManager::~APointFDataManager()
{
    clear();
}

ADoubleDataManager* APointFDataManager::subDoubleDataManager() const
{
    return d_ptr->m_doubleDataManager;
}

QPointF APointFDataManager::value(const AData* data) const
{
    return getValue<QPointF>(d_ptr->m_values, data);
}

int APointFDataManager::decimals(const AData* data) const
{
    return getData<int>(d_ptr->m_values, &APointFDataManagerPrivate::Data::decimals, data, 0);
}

QString APointFDataManager::toString(const AData* data) const
{
    const APointFDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();
    const QPointF v = it.value().val;
    const int dec = it.value().decimals;
    return tr("(%1, %2)").arg(QString::number(v.x(), 'f', dec)).arg(QString::number(v.y(), 'f', dec));
}

void APointFDataManager::setValue(AData* data, const QPointF& val)
{
    const APointFDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    if (it.value().val == val)
        return;

    it.value().val = val;
    d_ptr->m_doubleDataManager->setValue(d_ptr->m_dataToX[data], val.x());
    d_ptr->m_doubleDataManager->setValue(d_ptr->m_dataToY[data], val.y());

    emit dataChanged(data);
    emit valueChanged(data, val);
}

void APointFDataManager::setDecimals(AData* data, int prec)
{
    const APointFDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    APointFDataManagerPrivate::Data metaData = it.value();

    if (prec > 13)
        prec = 13;
    else if (prec < 0)
        prec = 0;

    if (metaData.decimals == prec)
        return;

    metaData.decimals = prec;
    d_ptr->m_doubleDataManager->setDecimals(d_ptr->m_dataToX[data], prec);
    d_ptr->m_doubleDataManager->setDecimals(d_ptr->m_dataToY[data], prec);

    it.value() = metaData;

    emit decimalsChanged(data, metaData.decimals);
}

void APointFDataManager::initializeData(AData* data)
{
    d_ptr->m_values[data] = APointFDataManagerPrivate::Data();

    AData* xProp = d_ptr->m_doubleDataManager->addData();
    xProp->setName(tr("X"));
    d_ptr->m_doubleDataManager->setDecimals(xProp, decimals(data));
    d_ptr->m_doubleDataManager->setValue(xProp, 0);
    d_ptr->m_dataToX[data] = xProp;
    d_ptr->m_xToData[xProp] = data;
    data->addSubData(xProp);

    AData* yProp = d_ptr->m_doubleDataManager->addData();
    yProp->setName(tr("Y"));
    d_ptr->m_doubleDataManager->setDecimals(yProp, decimals(data));
    d_ptr->m_doubleDataManager->setValue(yProp, 0);
    d_ptr->m_dataToY[data] = yProp;
    d_ptr->m_yToData[yProp] = data;
    data->addSubData(yProp);
}

void APointFDataManager::uninitializeData(AData* data)
{
    AData* xProp = d_ptr->m_dataToX[data];
    if (xProp)
    {
        d_ptr->m_xToData.remove(xProp);
        delete xProp;
    }
    d_ptr->m_dataToX.remove(data);

    AData* yProp = d_ptr->m_dataToY[data];
    if (yProp)
    {
        d_ptr->m_yToData.remove(yProp);
        delete yProp;
    }
    d_ptr->m_dataToY.remove(data);

    d_ptr->m_values.remove(data);
}

void APointFDataManager::onDoubleChanged(AData* data, double value)
{
    if (AData* prop = d_ptr->m_xToData.value(data, 0))
    {
        QPointF p = d_ptr->m_values[prop].val;
        p.setX(value);
        setValue(prop, p);
    }
    else if (AData* prop = d_ptr->m_yToData.value(data, 0))
    {
        QPointF p = d_ptr->m_values[prop].val;
        p.setY(value);
        setValue(prop, p);
    }
}

void APointFDataManager::onDataDestroyed(AData* data)
{
    if (AData* pointProp = d_ptr->m_xToData.value(data, 0))
    {
        d_ptr->m_dataToX[pointProp] = 0;
        d_ptr->m_xToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_yToData.value(data, 0))
    {
        d_ptr->m_dataToY[pointProp] = 0;
        d_ptr->m_yToData.remove(data);
    }
}

// ASizeDataManager

class ASizeDataManagerPrivate
{
    ASizeDataManager* q_ptr = nullptr;
    Q_DECLARE_PUBLIC(ASizeDataManager)
public:
    void setValue(AData* data, const QSize& val);
    void setRange(AData* data, const QSize& minVal, const QSize& maxVal, const QSize& val);

    struct Data
    {
        QSize val{ 0, 0 };
        QSize minVal{ 0, 0 };
        QSize maxVal{ INT_MAX, INT_MAX };
        QSize minimumValue() const
        {
            return minVal;
        }
        QSize maximumValue() const
        {
            return maxVal;
        }
        void setMinimumValue(const QSize& newMinVal)
        {
            setSizeMinimumData(this, newMinVal);
        }
        void setMaximumValue(const QSize& newMaxVal)
        {
            setSizeMaximumData(this, newMaxVal);
        }
    };

    typedef QMap<const AData*, Data> DataValueMap;
    DataValueMap m_values;

    AIntDataManager* m_intDataManager = nullptr;

    QMap<const AData*, AData*> m_dataToW;
    QMap<const AData*, AData*> m_dataToH;

    QMap<const AData*, AData*> m_wToData;
    QMap<const AData*, AData*> m_hToData;
};

void ASizeDataManagerPrivate::setValue(AData* data, const QSize& val)
{
    m_intDataManager->setValue(m_dataToW.value(data), val.width());
    m_intDataManager->setValue(m_dataToH.value(data), val.height());
}

void ASizeDataManagerPrivate::setRange(AData* data,
                                       const QSize& minVal, const QSize& maxVal, const QSize& val)
{
    AData* wData = m_dataToW.value(data);
    AData* hData = m_dataToH.value(data);
    m_intDataManager->setRange(wData, minVal.width(), maxVal.width());
    m_intDataManager->setValue(wData, val.width());
    m_intDataManager->setRange(hData, minVal.height(), maxVal.height());
    m_intDataManager->setValue(hData, val.height());
}

ASizeDataManager::ASizeDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new ASizeDataManagerPrivate)
{
    d_ptr->q_ptr = this;

    d_ptr->m_intDataManager = new AIntDataManager(this);
    connect(d_ptr->m_intDataManager, SIGNAL(valueChanged(AData*, int)),
            this, SLOT(onIntChanged(AData*, int)));
    connect(d_ptr->m_intDataManager, SIGNAL(dataDestroyed(AData*)),
            this, SLOT(onDataDestroyed(AData*)));
}

ASizeDataManager::~ASizeDataManager()
{
    clear();
}

AIntDataManager* ASizeDataManager::subIntDataManager() const
{
    return d_ptr->m_intDataManager;
}

QSize ASizeDataManager::value(const AData* data) const
{
    return getValue<QSize>(d_ptr->m_values, data);
}

QSize ASizeDataManager::minimum(const AData* data) const
{
    return getMinimum<QSize>(d_ptr->m_values, data);
}

QSize ASizeDataManager::maximum(const AData* data) const
{
    return getMaximum<QSize>(d_ptr->m_values, data);
}

QString ASizeDataManager::toString(const AData* data) const
{
    const ASizeDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();
    const QSize v = it.value().val;
    return tr("%1 x %2").arg(QString::number(v.width())).arg(QString::number(v.height()));
}

void ASizeDataManager::setValue(AData* data, const QSize& val)
{
    setValueInRange<const QSize&, ASizeDataManagerPrivate, ASizeDataManager, const QSize>(this, d_ptr.data(),
                                                                                          &ASizeDataManager::dataChanged,
                                                                                          &ASizeDataManager::valueChanged,
                                                                                          data, val, &ASizeDataManagerPrivate::setValue);
}

void ASizeDataManager::setMinimum(AData* data, const QSize& minVal)
{
    setBorderValue<const QSize&, ASizeDataManagerPrivate, ASizeDataManager, QSize, ASizeDataManagerPrivate::Data>(this, d_ptr.data(),
                                                                                                                  &ASizeDataManager::dataChanged,
                                                                                                                  &ASizeDataManager::valueChanged,
                                                                                                                  &ASizeDataManager::rangeChanged,
                                                                                                                  data,
                                                                                                                  &ASizeDataManagerPrivate::Data::minimumValue,
                                                                                                                  &ASizeDataManagerPrivate::Data::setMinimumValue,
                                                                                                                  minVal, &ASizeDataManagerPrivate::setRange);
}

void ASizeDataManager::setMaximum(AData* data, const QSize& maxVal)
{
    setBorderValue<const QSize&, ASizeDataManagerPrivate, ASizeDataManager, QSize, ASizeDataManagerPrivate::Data>(this, d_ptr.data(),
                                                                                                                  &ASizeDataManager::dataChanged,
                                                                                                                  &ASizeDataManager::valueChanged,
                                                                                                                  &ASizeDataManager::rangeChanged,
                                                                                                                  data,
                                                                                                                  &ASizeDataManagerPrivate::Data::maximumValue,
                                                                                                                  &ASizeDataManagerPrivate::Data::setMaximumValue,
                                                                                                                  maxVal, &ASizeDataManagerPrivate::setRange);
}

void ASizeDataManager::setRange(AData* data, const QSize& minVal, const QSize& maxVal)
{
    setBorderValues<const QSize&, ASizeDataManagerPrivate, ASizeDataManager, QSize>(this, d_ptr.data(),
                                                                                    &ASizeDataManager::dataChanged,
                                                                                    &ASizeDataManager::valueChanged,
                                                                                    &ASizeDataManager::rangeChanged,
                                                                                    data, minVal, maxVal, &ASizeDataManagerPrivate::setRange);
}

void ASizeDataManager::initializeData(AData* data)
{
    d_ptr->m_values[data] = ASizeDataManagerPrivate::Data();

    AData* wProp = d_ptr->m_intDataManager->addData();
    wProp->setName(tr("Width"));
    d_ptr->m_intDataManager->setValue(wProp, 0);
    d_ptr->m_intDataManager->setMinimum(wProp, 0);
    d_ptr->m_dataToW[data] = wProp;
    d_ptr->m_wToData[wProp] = data;
    data->addSubData(wProp);

    AData* hProp = d_ptr->m_intDataManager->addData();
    hProp->setName(tr("Height"));
    d_ptr->m_intDataManager->setValue(hProp, 0);
    d_ptr->m_intDataManager->setMinimum(hProp, 0);
    d_ptr->m_dataToH[data] = hProp;
    d_ptr->m_hToData[hProp] = data;
    data->addSubData(hProp);
}

void ASizeDataManager::uninitializeData(AData* data)
{
    AData* wProp = d_ptr->m_dataToW[data];
    if (wProp)
    {
        d_ptr->m_wToData.remove(wProp);
        delete wProp;
    }
    d_ptr->m_dataToW.remove(data);

    AData* hProp = d_ptr->m_dataToH[data];
    if (hProp)
    {
        d_ptr->m_hToData.remove(hProp);
        delete hProp;
    }
    d_ptr->m_dataToH.remove(data);

    d_ptr->m_values.remove(data);
}

void ASizeDataManager::onIntChanged(AData* data, int value)
{
    if (AData* prop = d_ptr->m_wToData.value(data, 0))
    {
        QSize s = d_ptr->m_values[prop].val;
        s.setWidth(value);
        setValue(prop, s);
    }
    else if (AData* prop = d_ptr->m_hToData.value(data, 0))
    {
        QSize s = d_ptr->m_values[prop].val;
        s.setHeight(value);
        setValue(prop, s);
    }
}

void ASizeDataManager::onDataDestroyed(AData* data)
{
    if (AData* pointProp = d_ptr->m_wToData.value(data, 0))
    {
        d_ptr->m_dataToW[pointProp] = 0;
        d_ptr->m_wToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_hToData.value(data, 0))
    {
        d_ptr->m_dataToH[pointProp] = 0;
        d_ptr->m_hToData.remove(data);
    }
}

// ASizeFDataManager

class ASizeFDataManagerPrivate
{
    ASizeFDataManager* q_ptr = nullptr;
    Q_DECLARE_PUBLIC(ASizeFDataManager)
public:
    void setValue(AData* data, const QSizeF& val);
    void setRange(AData* data,
                  const QSizeF& minVal, const QSizeF& maxVal, const QSizeF& val);

    struct Data
    {
        QSizeF val{ 0, 0 };
        QSizeF minVal{ 0, 0 };
        QSizeF maxVal{ (std::numeric_limits<qreal>::max)(), (std::numeric_limits<qreal>::max)() };
        int decimals{ 2 };
        QSizeF minimumValue() const
        {
            return minVal;
        }
        QSizeF maximumValue() const
        {
            return maxVal;
        }
        void setMinimumValue(const QSizeF& newMinVal)
        {
            setSizeMinimumData(this, newMinVal);
        }
        void setMaximumValue(const QSizeF& newMaxVal)
        {
            setSizeMaximumData(this, newMaxVal);
        }
    };

    typedef QMap<const AData*, Data> DataValueMap;
    DataValueMap m_values;

    ADoubleDataManager* m_doubleDataManager = nullptr;

    QMap<const AData*, AData*> m_dataToW;
    QMap<const AData*, AData*> m_dataToH;

    QMap<const AData*, AData*> m_wToData;
    QMap<const AData*, AData*> m_hToData;
};

void ASizeFDataManagerPrivate::setValue(AData* data, const QSizeF& val)
{
    m_doubleDataManager->setValue(m_dataToW.value(data), val.width());
    m_doubleDataManager->setValue(m_dataToH.value(data), val.height());
}

void ASizeFDataManagerPrivate::setRange(AData* data,
                                        const QSizeF& minVal, const QSizeF& maxVal, const QSizeF& val)
{
    m_doubleDataManager->setRange(m_dataToW[data], minVal.width(), maxVal.width());
    m_doubleDataManager->setValue(m_dataToW[data], val.width());
    m_doubleDataManager->setRange(m_dataToH[data], minVal.height(), maxVal.height());
    m_doubleDataManager->setValue(m_dataToH[data], val.height());
}

ASizeFDataManager::ASizeFDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new ASizeFDataManagerPrivate)
{
    d_ptr->q_ptr = this;

    d_ptr->m_doubleDataManager = new ADoubleDataManager(this);
    connect(d_ptr->m_doubleDataManager, SIGNAL(valueChanged(AData*, double)),
            this, SLOT(onDoubleChanged(AData*, double)));
    connect(d_ptr->m_doubleDataManager, SIGNAL(dataDestroyed(AData*)),
            this, SLOT(onDataDestroyed(AData*)));
}

ASizeFDataManager::~ASizeFDataManager()
{
    clear();
}

ADoubleDataManager* ASizeFDataManager::subDoubleDataManager() const
{
    return d_ptr->m_doubleDataManager;
}

QSizeF ASizeFDataManager::value(const AData* data) const
{
    return getValue<QSizeF>(d_ptr->m_values, data);
}

int ASizeFDataManager::decimals(const AData* data) const
{
    return getData<int>(d_ptr->m_values, &ASizeFDataManagerPrivate::Data::decimals, data, 0);
}

QSizeF ASizeFDataManager::minimum(const AData* data) const
{
    return getMinimum<QSizeF>(d_ptr->m_values, data);
}

QSizeF ASizeFDataManager::maximum(const AData* data) const
{
    return getMaximum<QSizeF>(d_ptr->m_values, data);
}

QString ASizeFDataManager::toString(const AData* data) const
{
    const ASizeFDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();
    const QSizeF v = it.value().val;
    const int dec = it.value().decimals;
    return tr("%1 x %2").arg(QString::number(v.width(), 'f', dec)).arg(QString::number(v.height(), 'f', dec));
}

void ASizeFDataManager::setValue(AData* data, const QSizeF& val)
{
    setValueInRange<const QSizeF&, ASizeFDataManagerPrivate, ASizeFDataManager, QSizeF>(this, d_ptr.data(),
                                                                                        &ASizeFDataManager::dataChanged,
                                                                                        &ASizeFDataManager::valueChanged,
                                                                                        data, val, &ASizeFDataManagerPrivate::setValue);
}

void ASizeFDataManager::setDecimals(AData* data, int prec)
{
    const ASizeFDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    ASizeFDataManagerPrivate::Data metaData = it.value();

    if (prec > 13)
        prec = 13;
    else if (prec < 0)
        prec = 0;

    if (metaData.decimals == prec)
        return;

    metaData.decimals = prec;
    d_ptr->m_doubleDataManager->setDecimals(d_ptr->m_dataToW[data], prec);
    d_ptr->m_doubleDataManager->setDecimals(d_ptr->m_dataToH[data], prec);

    it.value() = metaData;

    emit decimalsChanged(data, metaData.decimals);
}

void ASizeFDataManager::setMinimum(AData* data, const QSizeF& minVal)
{
    setBorderValue<const QSizeF&, ASizeFDataManagerPrivate, ASizeFDataManager, QSizeF, ASizeFDataManagerPrivate::Data>(this, d_ptr.data(),
                                                                                                                       &ASizeFDataManager::dataChanged,
                                                                                                                       &ASizeFDataManager::valueChanged,
                                                                                                                       &ASizeFDataManager::rangeChanged,
                                                                                                                       data,
                                                                                                                       &ASizeFDataManagerPrivate::Data::minimumValue,
                                                                                                                       &ASizeFDataManagerPrivate::Data::setMinimumValue,
                                                                                                                       minVal, &ASizeFDataManagerPrivate::setRange);
}

void ASizeFDataManager::setMaximum(AData* data, const QSizeF& maxVal)
{
    setBorderValue<const QSizeF&, ASizeFDataManagerPrivate, ASizeFDataManager, QSizeF, ASizeFDataManagerPrivate::Data>(this, d_ptr.data(),
                                                                                                                       &ASizeFDataManager::dataChanged,
                                                                                                                       &ASizeFDataManager::valueChanged,
                                                                                                                       &ASizeFDataManager::rangeChanged,
                                                                                                                       data,
                                                                                                                       &ASizeFDataManagerPrivate::Data::maximumValue,
                                                                                                                       &ASizeFDataManagerPrivate::Data::setMaximumValue,
                                                                                                                       maxVal, &ASizeFDataManagerPrivate::setRange);
}

void ASizeFDataManager::setRange(AData* data, const QSizeF& minVal, const QSizeF& maxVal)
{
    setBorderValues<const QSizeF&, ASizeFDataManagerPrivate, ASizeFDataManager, QSizeF>(this, d_ptr.data(),
                                                                                        &ASizeFDataManager::dataChanged,
                                                                                        &ASizeFDataManager::valueChanged,
                                                                                        &ASizeFDataManager::rangeChanged,
                                                                                        data, minVal, maxVal, &ASizeFDataManagerPrivate::setRange);
}

void ASizeFDataManager::initializeData(AData* data)
{
    d_ptr->m_values[data] = ASizeFDataManagerPrivate::Data();

    AData* wProp = d_ptr->m_doubleDataManager->addData();
    wProp->setName(tr("Width"));
    d_ptr->m_doubleDataManager->setDecimals(wProp, decimals(data));
    d_ptr->m_doubleDataManager->setValue(wProp, 0);
    d_ptr->m_doubleDataManager->setMinimum(wProp, 0);
    d_ptr->m_dataToW[data] = wProp;
    d_ptr->m_wToData[wProp] = data;
    data->addSubData(wProp);

    AData* hProp = d_ptr->m_doubleDataManager->addData();
    hProp->setName(tr("Height"));
    d_ptr->m_doubleDataManager->setDecimals(hProp, decimals(data));
    d_ptr->m_doubleDataManager->setValue(hProp, 0);
    d_ptr->m_doubleDataManager->setMinimum(hProp, 0);
    d_ptr->m_dataToH[data] = hProp;
    d_ptr->m_hToData[hProp] = data;
    data->addSubData(hProp);
}

void ASizeFDataManager::uninitializeData(AData* data)
{
    AData* wProp = d_ptr->m_dataToW[data];
    if (wProp)
    {
        d_ptr->m_wToData.remove(wProp);
        delete wProp;
    }
    d_ptr->m_dataToW.remove(data);

    AData* hProp = d_ptr->m_dataToH[data];
    if (hProp)
    {
        d_ptr->m_hToData.remove(hProp);
        delete hProp;
    }
    d_ptr->m_dataToH.remove(data);

    d_ptr->m_values.remove(data);
}

void ASizeFDataManager::onDoubleChanged(AData* data, double value)
{
    if (AData* prop = d_ptr->m_wToData.value(data, 0))
    {
        QSizeF s = d_ptr->m_values[prop].val;
        s.setWidth(value);
        setValue(prop, s);
    }
    else if (AData* prop = d_ptr->m_hToData.value(data, 0))
    {
        QSizeF s = d_ptr->m_values[prop].val;
        s.setHeight(value);
        setValue(prop, s);
    }
}

void ASizeFDataManager::onDataDestroyed(AData* data)
{
    if (AData* pointProp = d_ptr->m_wToData.value(data, 0))
    {
        d_ptr->m_dataToW[pointProp] = 0;
        d_ptr->m_wToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_hToData.value(data, 0))
    {
        d_ptr->m_dataToH[pointProp] = 0;
        d_ptr->m_hToData.remove(data);
    }
}

// ARectDataManager

class ARectDataManagerPrivate
{
    ARectDataManager* q_ptr = nullptr;
    Q_DECLARE_PUBLIC(ARectDataManager)
public:
    void setConstraint(AData* data, const QRect& constraint, const QRect& val);

    struct Data
    {
        QRect val{ 0, 0, 0, 0 };
        QRect constraint;
    };

    typedef QMap<const AData*, Data> DataValueMap;
    DataValueMap m_values;

    AIntDataManager* m_intDataManager = nullptr;

    QMap<const AData*, AData*> m_dataToX;
    QMap<const AData*, AData*> m_dataToY;
    QMap<const AData*, AData*> m_dataToW;
    QMap<const AData*, AData*> m_dataToH;

    QMap<const AData*, AData*> m_xToData;
    QMap<const AData*, AData*> m_yToData;
    QMap<const AData*, AData*> m_wToData;
    QMap<const AData*, AData*> m_hToData;
};

void ARectDataManagerPrivate::setConstraint(AData* data,
                                            const QRect& constraint, const QRect& val)
{
    const bool isNull = constraint.isNull();
    const int left = isNull ? INT_MIN : constraint.left();
    const int right = isNull ? INT_MAX : constraint.left() + constraint.width();
    const int top = isNull ? INT_MIN : constraint.top();
    const int bottom = isNull ? INT_MAX : constraint.top() + constraint.height();
    const int width = isNull ? INT_MAX : constraint.width();
    const int height = isNull ? INT_MAX : constraint.height();

    m_intDataManager->setRange(m_dataToX[data], left, right);
    m_intDataManager->setRange(m_dataToY[data], top, bottom);
    m_intDataManager->setRange(m_dataToW[data], 0, width);
    m_intDataManager->setRange(m_dataToH[data], 0, height);

    m_intDataManager->setValue(m_dataToX[data], val.x());
    m_intDataManager->setValue(m_dataToY[data], val.y());
    m_intDataManager->setValue(m_dataToW[data], val.width());
    m_intDataManager->setValue(m_dataToH[data], val.height());
}

ARectDataManager::ARectDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new ARectDataManagerPrivate)
{
    d_ptr->q_ptr = this;

    d_ptr->m_intDataManager = new AIntDataManager(this);
    connect(d_ptr->m_intDataManager, SIGNAL(valueChanged(AData*, int)),
            this, SLOT(onIntChanged(AData*, int)));
    connect(d_ptr->m_intDataManager, SIGNAL(dataDestroyed(AData*)),
            this, SLOT(onDataDestroyed(AData*)));
}

ARectDataManager::~ARectDataManager()
{
    clear();
}

AIntDataManager* ARectDataManager::subIntDataManager() const
{
    return d_ptr->m_intDataManager;
}

QRect ARectDataManager::value(const AData* data) const
{
    return getValue<QRect>(d_ptr->m_values, data);
}

QRect ARectDataManager::constraint(const AData* data) const
{
    return getData<QRect>(d_ptr->m_values, &ARectDataManagerPrivate::Data::constraint, data, QRect());
}

QString ARectDataManager::toString(const AData* data) const
{
    const ARectDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();
    const QRect v = it.value().val;
    return tr("[(%1, %2), %3 x %4]").arg(QString::number(v.x())).arg(QString::number(v.y())).arg(QString::number(v.width())).arg(QString::number(v.height()));
}

void ARectDataManager::setValue(AData* data, const QRect& val)
{
    const ARectDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    ARectDataManagerPrivate::Data metaData = it.value();

    QRect newRect = val.normalized();
    if (!metaData.constraint.isNull() && !metaData.constraint.contains(newRect))
    {
        const QRect r1 = metaData.constraint;
        const QRect r2 = newRect;
        newRect.setLeft(qMax(r1.left(), r2.left()));
        newRect.setRight(qMin(r1.right(), r2.right()));
        newRect.setTop(qMax(r1.top(), r2.top()));
        newRect.setBottom(qMin(r1.bottom(), r2.bottom()));
        if (newRect.width() < 0 || newRect.height() < 0)
            return;
    }

    if (metaData.val == newRect)
        return;

    metaData.val = newRect;

    it.value() = metaData;
    d_ptr->m_intDataManager->setValue(d_ptr->m_dataToX[data], newRect.x());
    d_ptr->m_intDataManager->setValue(d_ptr->m_dataToY[data], newRect.y());
    d_ptr->m_intDataManager->setValue(d_ptr->m_dataToW[data], newRect.width());
    d_ptr->m_intDataManager->setValue(d_ptr->m_dataToH[data], newRect.height());

    emit dataChanged(data);
    emit valueChanged(data, metaData.val);
}

void ARectDataManager::setConstraint(AData* data, const QRect& constraint)
{
    const ARectDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    ARectDataManagerPrivate::Data metaData = it.value();

    QRect newConstraint = constraint.normalized();
    if (metaData.constraint == newConstraint)
        return;

    const QRect oldVal = metaData.val;

    metaData.constraint = newConstraint;

    if (!metaData.constraint.isNull() && !metaData.constraint.contains(oldVal))
    {
        QRect r1 = metaData.constraint;
        QRect r2 = metaData.val;

        if (r2.width() > r1.width())
            r2.setWidth(r1.width());
        if (r2.height() > r1.height())
            r2.setHeight(r1.height());
        if (r2.left() < r1.left())
            r2.moveLeft(r1.left());
        else if (r2.right() > r1.right())
            r2.moveRight(r1.right());
        if (r2.top() < r1.top())
            r2.moveTop(r1.top());
        else if (r2.bottom() > r1.bottom())
            r2.moveBottom(r1.bottom());

        metaData.val = r2;
    }

    it.value() = metaData;

    emit constraintChanged(data, metaData.constraint);

    d_ptr->setConstraint(data, metaData.constraint, metaData.val);

    if (metaData.val == oldVal)
        return;

    emit dataChanged(data);
    emit valueChanged(data, metaData.val);
}

void ARectDataManager::initializeData(AData* data)
{
    d_ptr->m_values[data] = ARectDataManagerPrivate::Data();

    AData* xProp = d_ptr->m_intDataManager->addData();
    xProp->setName(tr("X"));
    d_ptr->m_intDataManager->setValue(xProp, 0);
    d_ptr->m_dataToX[data] = xProp;
    d_ptr->m_xToData[xProp] = data;
    data->addSubData(xProp);

    AData* yProp = d_ptr->m_intDataManager->addData();
    yProp->setName(tr("Y"));
    d_ptr->m_intDataManager->setValue(yProp, 0);
    d_ptr->m_dataToY[data] = yProp;
    d_ptr->m_yToData[yProp] = data;
    data->addSubData(yProp);

    AData* wProp = d_ptr->m_intDataManager->addData();
    wProp->setName(tr("Width"));
    d_ptr->m_intDataManager->setValue(wProp, 0);
    d_ptr->m_intDataManager->setMinimum(wProp, 0);
    d_ptr->m_dataToW[data] = wProp;
    d_ptr->m_wToData[wProp] = data;
    data->addSubData(wProp);

    AData* hProp = d_ptr->m_intDataManager->addData();
    hProp->setName(tr("Height"));
    d_ptr->m_intDataManager->setValue(hProp, 0);
    d_ptr->m_intDataManager->setMinimum(hProp, 0);
    d_ptr->m_dataToH[data] = hProp;
    d_ptr->m_hToData[hProp] = data;
    data->addSubData(hProp);
}

void ARectDataManager::uninitializeData(AData* data)
{
    AData* xProp = d_ptr->m_dataToX[data];
    if (xProp)
    {
        d_ptr->m_xToData.remove(xProp);
        delete xProp;
    }
    d_ptr->m_dataToX.remove(data);

    AData* yProp = d_ptr->m_dataToY[data];
    if (yProp)
    {
        d_ptr->m_yToData.remove(yProp);
        delete yProp;
    }
    d_ptr->m_dataToY.remove(data);

    AData* wProp = d_ptr->m_dataToW[data];
    if (wProp)
    {
        d_ptr->m_wToData.remove(wProp);
        delete wProp;
    }
    d_ptr->m_dataToW.remove(data);

    AData* hProp = d_ptr->m_dataToH[data];
    if (hProp)
    {
        d_ptr->m_hToData.remove(hProp);
        delete hProp;
    }
    d_ptr->m_dataToH.remove(data);

    d_ptr->m_values.remove(data);
}

void ARectDataManager::onIntChanged(AData* data, int value)
{
    if (AData* prop = d_ptr->m_xToData.value(data, 0))
    {
        QRect r = d_ptr->m_values[prop].val;
        r.moveLeft(value);
        setValue(prop, r);
    }
    else if (AData* prop = d_ptr->m_yToData.value(data))
    {
        QRect r = d_ptr->m_values[prop].val;
        r.moveTop(value);
        setValue(prop, r);
    }
    else if (AData* prop = d_ptr->m_wToData.value(data, 0))
    {
        ARectDataManagerPrivate::Data metaData = d_ptr->m_values[prop];
        QRect r = metaData.val;
        r.setWidth(value);
        if (!metaData.constraint.isNull() && metaData.constraint.x() + metaData.constraint.width() < r.x() + r.width())
        {
            r.moveLeft(metaData.constraint.left() + metaData.constraint.width() - r.width());
        }
        setValue(prop, r);
    }
    else if (AData* prop = d_ptr->m_hToData.value(data, 0))
    {
        ARectDataManagerPrivate::Data metaData = d_ptr->m_values[prop];
        QRect r = metaData.val;
        r.setHeight(value);
        if (!metaData.constraint.isNull() && metaData.constraint.y() + metaData.constraint.height() < r.y() + r.height())
        {
            r.moveTop(metaData.constraint.top() + metaData.constraint.height() - r.height());
        }
        setValue(prop, r);
    }
}

void ARectDataManager::onDataDestroyed(AData* data)
{
    if (AData* pointProp = d_ptr->m_xToData.value(data, 0))
    {
        d_ptr->m_dataToX[pointProp] = 0;
        d_ptr->m_xToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_yToData.value(data, 0))
    {
        d_ptr->m_dataToY[pointProp] = 0;
        d_ptr->m_yToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_wToData.value(data, 0))
    {
        d_ptr->m_dataToW[pointProp] = 0;
        d_ptr->m_wToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_hToData.value(data, 0))
    {
        d_ptr->m_dataToH[pointProp] = 0;
        d_ptr->m_hToData.remove(data);
    }
}

// ARectFDataManager

class ARectFDataManagerPrivate
{
    ARectFDataManager* q_ptr = nullptr;
    Q_DECLARE_PUBLIC(ARectFDataManager)
public:
    void setConstraint(AData* data, const QRectF& constraint, const QRectF& val);

    struct Data
    {
        QRectF val{ 0, 0, 0, 0 };
        QRectF constraint;
        int decimals{ 2 };
    };

    typedef QMap<const AData*, Data> DataValueMap;
    DataValueMap m_values;

    ADoubleDataManager* m_doubleDataManager = nullptr;

    QMap<const AData*, AData*> m_dataToX;
    QMap<const AData*, AData*> m_dataToY;
    QMap<const AData*, AData*> m_dataToW;
    QMap<const AData*, AData*> m_dataToH;

    QMap<const AData*, AData*> m_xToData;
    QMap<const AData*, AData*> m_yToData;
    QMap<const AData*, AData*> m_wToData;
    QMap<const AData*, AData*> m_hToData;
};

void ARectFDataManagerPrivate::setConstraint(AData* data,
                                             const QRectF& constraint, const QRectF& val)
{
    const bool isNull = constraint.isNull();
    const float left = isNull ? FLT_MIN : constraint.left();
    const float right = isNull ? FLT_MAX : constraint.left() + constraint.width();
    const float top = isNull ? FLT_MIN : constraint.top();
    const float bottom = isNull ? FLT_MAX : constraint.top() + constraint.height();
    const float width = isNull ? FLT_MAX : constraint.width();
    const float height = isNull ? FLT_MAX : constraint.height();

    m_doubleDataManager->setRange(m_dataToX[data], left, right);
    m_doubleDataManager->setRange(m_dataToY[data], top, bottom);
    m_doubleDataManager->setRange(m_dataToW[data], 0, width);
    m_doubleDataManager->setRange(m_dataToH[data], 0, height);

    m_doubleDataManager->setValue(m_dataToX[data], val.x());
    m_doubleDataManager->setValue(m_dataToY[data], val.y());
    m_doubleDataManager->setValue(m_dataToW[data], val.width());
    m_doubleDataManager->setValue(m_dataToH[data], val.height());
}

ARectFDataManager::ARectFDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new ARectFDataManagerPrivate)
{
    d_ptr->q_ptr = this;

    d_ptr->m_doubleDataManager = new ADoubleDataManager(this);
    connect(d_ptr->m_doubleDataManager, SIGNAL(valueChanged(AData*, double)),
            this, SLOT(onDoubleChanged(AData*, double)));
    connect(d_ptr->m_doubleDataManager, SIGNAL(dataDestroyed(AData*)),
            this, SLOT(onDataDestroyed(AData*)));
}

ARectFDataManager::~ARectFDataManager()
{
    clear();
}

ADoubleDataManager* ARectFDataManager::subDoubleDataManager() const
{
    return d_ptr->m_doubleDataManager;
}

QRectF ARectFDataManager::value(const AData* data) const
{
    return getValue<QRectF>(d_ptr->m_values, data);
}

int ARectFDataManager::decimals(const AData* data) const
{
    return getData<int>(d_ptr->m_values, &ARectFDataManagerPrivate::Data::decimals, data, 0);
}

QRectF ARectFDataManager::constraint(const AData* data) const
{
    return getData<QRectF>(d_ptr->m_values, &ARectFDataManagerPrivate::Data::constraint, data, QRect());
}

QString ARectFDataManager::toString(const AData* data) const
{
    const ARectFDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();
    const QRectF v = it.value().val;
    const int dec = it.value().decimals;
    return QString(tr("[(%1, %2), %3 x %4]").arg(QString::number(v.x(), 'f', dec)).arg(QString::number(v.y(), 'f', dec)).arg(QString::number(v.width(), 'f', dec)).arg(QString::number(v.height(), 'f', dec)));
}

void ARectFDataManager::setValue(AData* data, const QRectF& val)
{
    const ARectFDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    ARectFDataManagerPrivate::Data metaData = it.value();

    QRectF newRect = val.normalized();
    if (!metaData.constraint.isNull() && !metaData.constraint.contains(newRect))
    {
        const QRectF r1 = metaData.constraint;
        const QRectF r2 = newRect;
        newRect.setLeft(qMax(r1.left(), r2.left()));
        newRect.setRight(qMin(r1.right(), r2.right()));
        newRect.setTop(qMax(r1.top(), r2.top()));
        newRect.setBottom(qMin(r1.bottom(), r2.bottom()));
        if (newRect.width() < 0 || newRect.height() < 0)
            return;
    }

    if (metaData.val == newRect)
        return;

    metaData.val = newRect;

    it.value() = metaData;
    d_ptr->m_doubleDataManager->setValue(d_ptr->m_dataToX[data], newRect.x());
    d_ptr->m_doubleDataManager->setValue(d_ptr->m_dataToY[data], newRect.y());
    d_ptr->m_doubleDataManager->setValue(d_ptr->m_dataToW[data], newRect.width());
    d_ptr->m_doubleDataManager->setValue(d_ptr->m_dataToH[data], newRect.height());

    emit dataChanged(data);
    emit valueChanged(data, metaData.val);
}

void ARectFDataManager::setConstraint(AData* data, const QRectF& constraint)
{
    const ARectFDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    ARectFDataManagerPrivate::Data metaData = it.value();

    QRectF newConstraint = constraint.normalized();
    if (metaData.constraint == newConstraint)
        return;

    const QRectF oldVal = metaData.val;

    metaData.constraint = newConstraint;

    if (!metaData.constraint.isNull() && !metaData.constraint.contains(oldVal))
    {
        QRectF r1 = metaData.constraint;
        QRectF r2 = metaData.val;

        if (r2.width() > r1.width())
            r2.setWidth(r1.width());
        if (r2.height() > r1.height())
            r2.setHeight(r1.height());
        if (r2.left() < r1.left())
            r2.moveLeft(r1.left());
        else if (r2.right() > r1.right())
            r2.moveRight(r1.right());
        if (r2.top() < r1.top())
            r2.moveTop(r1.top());
        else if (r2.bottom() > r1.bottom())
            r2.moveBottom(r1.bottom());

        metaData.val = r2;
    }

    it.value() = metaData;

    emit constraintChanged(data, metaData.constraint);

    d_ptr->setConstraint(data, metaData.constraint, metaData.val);

    if (metaData.val == oldVal)
        return;

    emit dataChanged(data);
    emit valueChanged(data, metaData.val);
}

void ARectFDataManager::setDecimals(AData* data, int prec)
{
    const ARectFDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    ARectFDataManagerPrivate::Data metaData = it.value();

    if (prec > 13)
        prec = 13;
    else if (prec < 0)
        prec = 0;

    if (metaData.decimals == prec)
        return;

    metaData.decimals = prec;
    d_ptr->m_doubleDataManager->setDecimals(d_ptr->m_dataToX[data], prec);
    d_ptr->m_doubleDataManager->setDecimals(d_ptr->m_dataToY[data], prec);
    d_ptr->m_doubleDataManager->setDecimals(d_ptr->m_dataToW[data], prec);
    d_ptr->m_doubleDataManager->setDecimals(d_ptr->m_dataToH[data], prec);

    it.value() = metaData;

    emit decimalsChanged(data, metaData.decimals);
}

void ARectFDataManager::initializeData(AData* data)
{
    d_ptr->m_values[data] = ARectFDataManagerPrivate::Data();

    AData* xProp = d_ptr->m_doubleDataManager->addData();
    xProp->setName(tr("X"));
    d_ptr->m_doubleDataManager->setDecimals(xProp, decimals(data));
    d_ptr->m_doubleDataManager->setValue(xProp, 0);
    d_ptr->m_dataToX[data] = xProp;
    d_ptr->m_xToData[xProp] = data;
    data->addSubData(xProp);

    AData* yProp = d_ptr->m_doubleDataManager->addData();
    yProp->setName(tr("Y"));
    d_ptr->m_doubleDataManager->setDecimals(yProp, decimals(data));
    d_ptr->m_doubleDataManager->setValue(yProp, 0);
    d_ptr->m_dataToY[data] = yProp;
    d_ptr->m_yToData[yProp] = data;
    data->addSubData(yProp);

    AData* wProp = d_ptr->m_doubleDataManager->addData();
    wProp->setName(tr("Width"));
    d_ptr->m_doubleDataManager->setDecimals(wProp, decimals(data));
    d_ptr->m_doubleDataManager->setValue(wProp, 0);
    d_ptr->m_doubleDataManager->setMinimum(wProp, 0);
    d_ptr->m_dataToW[data] = wProp;
    d_ptr->m_wToData[wProp] = data;
    data->addSubData(wProp);

    AData* hProp = d_ptr->m_doubleDataManager->addData();
    hProp->setName(tr("Height"));
    d_ptr->m_doubleDataManager->setDecimals(hProp, decimals(data));
    d_ptr->m_doubleDataManager->setValue(hProp, 0);
    d_ptr->m_doubleDataManager->setMinimum(hProp, 0);
    d_ptr->m_dataToH[data] = hProp;
    d_ptr->m_hToData[hProp] = data;
    data->addSubData(hProp);
}

void ARectFDataManager::uninitializeData(AData* data)
{
    AData* xProp = d_ptr->m_dataToX[data];
    if (xProp)
    {
        d_ptr->m_xToData.remove(xProp);
        delete xProp;
    }
    d_ptr->m_dataToX.remove(data);

    AData* yProp = d_ptr->m_dataToY[data];
    if (yProp)
    {
        d_ptr->m_yToData.remove(yProp);
        delete yProp;
    }
    d_ptr->m_dataToY.remove(data);

    AData* wProp = d_ptr->m_dataToW[data];
    if (wProp)
    {
        d_ptr->m_wToData.remove(wProp);
        delete wProp;
    }
    d_ptr->m_dataToW.remove(data);

    AData* hProp = d_ptr->m_dataToH[data];
    if (hProp)
    {
        d_ptr->m_hToData.remove(hProp);
        delete hProp;
    }
    d_ptr->m_dataToH.remove(data);

    d_ptr->m_values.remove(data);
}

void ARectFDataManager::onDoubleChanged(AData* data, double value)
{
    if (AData* prop = d_ptr->m_xToData.value(data, 0))
    {
        QRectF r = d_ptr->m_values[prop].val;
        r.moveLeft(value);
        setValue(prop, r);
    }
    else if (AData* prop = d_ptr->m_yToData.value(data, 0))
    {
        QRectF r = d_ptr->m_values[prop].val;
        r.moveTop(value);
        setValue(prop, r);
    }
    else if (AData* prop = d_ptr->m_wToData.value(data, 0))
    {
        ARectFDataManagerPrivate::Data metaData = d_ptr->m_values[prop];
        QRectF r = metaData.val;
        r.setWidth(value);
        if (!metaData.constraint.isNull() && metaData.constraint.x() + metaData.constraint.width() < r.x() + r.width())
        {
            r.moveLeft(metaData.constraint.left() + metaData.constraint.width() - r.width());
        }
        setValue(prop, r);
    }
    else if (AData* prop = d_ptr->m_hToData.value(data, 0))
    {
        ARectFDataManagerPrivate::Data metaData = d_ptr->m_values[prop];
        QRectF r = metaData.val;
        r.setHeight(value);
        if (!metaData.constraint.isNull() && metaData.constraint.y() + metaData.constraint.height() < r.y() + r.height())
        {
            r.moveTop(metaData.constraint.top() + metaData.constraint.height() - r.height());
        }
        setValue(prop, r);
    }
}

void ARectFDataManager::onDataDestroyed(AData* data)
{
    if (AData* pointProp = d_ptr->m_xToData.value(data, 0))
    {
        d_ptr->m_dataToX[pointProp] = 0;
        d_ptr->m_xToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_yToData.value(data, 0))
    {
        d_ptr->m_dataToY[pointProp] = 0;
        d_ptr->m_yToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_wToData.value(data, 0))
    {
        d_ptr->m_dataToW[pointProp] = 0;
        d_ptr->m_wToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_hToData.value(data, 0))
    {
        d_ptr->m_dataToH[pointProp] = 0;
        d_ptr->m_hToData.remove(data);
    }
}

// AEnumDataManager

class AEnumDataManagerPrivate
{
    AEnumDataManager* q_ptr = nullptr;
    Q_DECLARE_PUBLIC(AEnumDataManager)
public:
    struct Data
    {
        int val{ -1 };
        QStringList enumNames;
        QMap<int, QIcon> enumIcons;
    };

    typedef QMap<const AData*, Data> DataValueMap;
    DataValueMap m_values;
};

AEnumDataManager::AEnumDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new AEnumDataManagerPrivate)
{
    d_ptr->q_ptr = this;
}

AEnumDataManager::~AEnumDataManager()
{
    clear();
}

int AEnumDataManager::value(const AData* data) const
{
    return getValue<int>(d_ptr->m_values, data, -1);
}

QStringList AEnumDataManager::enumNames(const AData* data) const
{
    return getData<QStringList>(d_ptr->m_values, &AEnumDataManagerPrivate::Data::enumNames, data, QStringList());
}

QMap<int, QIcon> AEnumDataManager::enumIcons(const AData* data) const
{
    return getData<QMap<int, QIcon>>(d_ptr->m_values, &AEnumDataManagerPrivate::Data::enumIcons, data, QMap<int, QIcon>());
}

QString AEnumDataManager::toString(const AData* data) const
{
    const AEnumDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();

    const AEnumDataManagerPrivate::Data& metaData = it.value();

    const int v = metaData.val;
    if (v >= 0 && v < metaData.enumNames.count())
        return metaData.enumNames.at(v);
    return QString();
}

QIcon AEnumDataManager::valueIcon(const AData* data) const
{
    const AEnumDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QIcon();

    const AEnumDataManagerPrivate::Data& metaData = it.value();

    const int v = metaData.val;
    return metaData.enumIcons.value(v);
}

void AEnumDataManager::setValue(AData* data, int val)
{
    const AEnumDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    AEnumDataManagerPrivate::Data metaData = it.value();

    if (val >= metaData.enumNames.count())
        return;

    if (val < 0 && metaData.enumNames.count() > 0)
        return;

    if (val < 0)
        val = -1;

    if (metaData.val == val)
        return;

    metaData.val = val;

    it.value() = metaData;

    emit dataChanged(data);
    emit valueChanged(data, metaData.val);
}

void AEnumDataManager::setEnumNames(AData* data, const QStringList& enumNames)
{
    const AEnumDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    AEnumDataManagerPrivate::Data metaData = it.value();

    if (metaData.enumNames == enumNames)
        return;

    metaData.enumNames = enumNames;

    metaData.val = -1;

    if (enumNames.count() > 0)
        metaData.val = 0;

    it.value() = metaData;

    emit enumNamesChanged(data, metaData.enumNames);

    emit dataChanged(data);
    emit valueChanged(data, metaData.val);
}

void AEnumDataManager::setEnumIcons(AData* data, const QMap<int, QIcon>& enumIcons)
{
    const AEnumDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    it.value().enumIcons = enumIcons;

    emit enumIconsChanged(data, it.value().enumIcons);

    emit dataChanged(data);
}

void AEnumDataManager::initializeData(AData* data)
{
    d_ptr->m_values[data] = AEnumDataManagerPrivate::Data();
}

void AEnumDataManager::uninitializeData(AData* data)
{
    d_ptr->m_values.remove(data);
}

// AFlagDataManager

class AFlagDataManagerPrivate
{
    AFlagDataManager* q_ptr = nullptr;
    Q_DECLARE_PUBLIC(AFlagDataManager)
public:
    struct Data
    {
        int val{ -1 };
        QStringList flagNames;
    };

    typedef QMap<const AData*, Data> DataValueMap;
    DataValueMap m_values;
    ABoolDataManager* m_boolDataManager = nullptr;
    QMap<const AData*, QList<AData*>> m_dataToFlags;
    QMap<const AData*, AData*> m_flagToData;
};

AFlagDataManager::AFlagDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new AFlagDataManagerPrivate)
{
    d_ptr->q_ptr = this;

    d_ptr->m_boolDataManager = new ABoolDataManager(this);
    connect(d_ptr->m_boolDataManager, SIGNAL(valueChanged(AData*, bool)), this, SLOT(onBoolChanged(AData*, bool)));
    connect(d_ptr->m_boolDataManager, SIGNAL(dataDestroyed(AData*)), this, SLOT(onDataDestroyed(AData*)));
}

AFlagDataManager::~AFlagDataManager()
{
    clear();
}

ABoolDataManager* AFlagDataManager::subBoolDataManager() const
{
    return d_ptr->m_boolDataManager;
}

int AFlagDataManager::value(const AData* data) const
{
    return getValue<int>(d_ptr->m_values, data, 0);
}

QStringList AFlagDataManager::flagNames(const AData* data) const
{
    return getData<QStringList>(d_ptr->m_values, &AFlagDataManagerPrivate::Data::flagNames, data, QStringList());
}

QString AFlagDataManager::toString(const AData* data) const
{
    const AFlagDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();

    const AFlagDataManagerPrivate::Data& metaData = it.value();

    QString str;
    int level = 0;
    const QChar bar = QLatin1Char('|');
    const QStringList::const_iterator fncend = metaData.flagNames.constEnd();
    for (QStringList::const_iterator it = metaData.flagNames.constBegin(); it != fncend; ++it)
    {
        if (metaData.val & (1 << level))
        {
            if (!str.isEmpty())
                str += bar;
            str += *it;
        }

        level++;
    }
    return str;
}

void AFlagDataManager::setValue(AData* data, int val)
{
    const AFlagDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    AFlagDataManagerPrivate::Data metaData = it.value();

    if (metaData.val == val)
        return;

    if (val > (1 << metaData.flagNames.count()) - 1)
        return;

    if (val < 0)
        return;

    metaData.val = val;

    it.value() = metaData;

    const auto pfit = d_ptr->m_dataToFlags.constFind(data);
    int level = 0;
    if (pfit != d_ptr->m_dataToFlags.constEnd())
    {
        for (AData* prop : pfit.value())
        {
            if (prop)
                d_ptr->m_boolDataManager->setValue(prop, val & (1 << level));
            level++;
        }
    }

    emit dataChanged(data);
    emit valueChanged(data, metaData.val);
}

void AFlagDataManager::setFlagNames(AData* data, const QStringList& flagNames)
{
    const AFlagDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    AFlagDataManagerPrivate::Data metaData = it.value();

    if (metaData.flagNames == flagNames)
        return;

    metaData.flagNames = flagNames;
    metaData.val = 0;

    it.value() = metaData;

    const auto pfit = d_ptr->m_dataToFlags.find(data);
    if (pfit != d_ptr->m_dataToFlags.end())
    {
        for (AData* prop : qAsConst(pfit.value()))
        {
            if (prop)
            {
                delete prop;
                d_ptr->m_flagToData.remove(prop);
            }
        }
        pfit.value().clear();
    }

    for (const QString& flagName : flagNames)
    {
        AData* prop = d_ptr->m_boolDataManager->addData();
        prop->setName(flagName);
        data->addSubData(prop);
        d_ptr->m_dataToFlags[data].append(prop);
        d_ptr->m_flagToData[prop] = data;
    }

    emit flagNamesChanged(data, metaData.flagNames);

    emit dataChanged(data);
    emit valueChanged(data, metaData.val);
}

void AFlagDataManager::initializeData(AData* data)
{
    d_ptr->m_values[data] = AFlagDataManagerPrivate::Data();

    d_ptr->m_dataToFlags[data] = QList<AData*>();
}

void AFlagDataManager::uninitializeData(AData* data)
{
    const auto it = d_ptr->m_dataToFlags.find(data);
    if (it != d_ptr->m_dataToFlags.end())
    {
        for (AData* prop : qAsConst(it.value()))
        {
            if (prop)
            {
                d_ptr->m_flagToData.remove(prop);
                delete prop;
            }
        }
    }
    d_ptr->m_dataToFlags.erase(it);

    d_ptr->m_values.remove(data);
}

void AFlagDataManager::onBoolChanged(AData* data, bool value)
{
    AData* prop = d_ptr->m_flagToData.value(data, 0);
    if (prop == 0)
        return;

    const auto pfit = d_ptr->m_dataToFlags.constFind(prop);
    if (pfit == d_ptr->m_dataToFlags.constEnd())
        return;
    int level = 0;
    for (AData* p : pfit.value())
    {
        if (p == data)
        {
            int v = d_ptr->m_values[prop].val;
            if (value)
            {
                v |= (1 << level);
            }
            else
            {
                v &= ~(1 << level);
            }
            setValue(prop, v);
            return;
        }
        level++;
    }
}

void AFlagDataManager::onDataDestroyed(AData* data)
{
    AData* flagData = d_ptr->m_flagToData.value(data, 0);
    if (flagData == 0)
        return;

    d_ptr->m_dataToFlags[flagData].replace(d_ptr->m_dataToFlags[flagData].indexOf(data), 0);
    d_ptr->m_flagToData.remove(data);
}

// ASizePolicyDataManager

class ASizePolicyDataManagerPrivate
{
    ASizePolicyDataManager* q_ptr = nullptr;
    Q_DECLARE_PUBLIC(ASizePolicyDataManager)
public:
    typedef QMap<const AData*, QSizePolicy> DataValueMap;
    DataValueMap m_values;

    AIntDataManager* m_intDataManager = nullptr;
    AEnumDataManager* m_enumDataManager = nullptr;

    QMap<const AData*, AData*> m_dataToHPolicy;
    QMap<const AData*, AData*> m_dataToVPolicy;
    QMap<const AData*, AData*> m_dataToHStretch;
    QMap<const AData*, AData*> m_dataToVStretch;

    QMap<const AData*, AData*> m_hPolicyToData;
    QMap<const AData*, AData*> m_vPolicyToData;
    QMap<const AData*, AData*> m_hStretchToData;
    QMap<const AData*, AData*> m_vStretchToData;
};

ASizePolicyDataManager::ASizePolicyDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new ASizePolicyDataManagerPrivate)
{
    d_ptr->q_ptr = this;

    d_ptr->m_intDataManager = new AIntDataManager(this);
    connect(d_ptr->m_intDataManager, SIGNAL(valueChanged(AData*, int)),
            this, SLOT(onIntChanged(AData*, int)));
    d_ptr->m_enumDataManager = new AEnumDataManager(this);
    connect(d_ptr->m_enumDataManager, SIGNAL(valueChanged(AData*, int)),
            this, SLOT(onEnumChanged(AData*, int)));

    connect(d_ptr->m_intDataManager, SIGNAL(dataDestroyed(AData*)),
            this, SLOT(onDataDestroyed(AData*)));
    connect(d_ptr->m_enumDataManager, SIGNAL(dataDestroyed(AData*)),
            this, SLOT(onDataDestroyed(AData*)));
}

ASizePolicyDataManager::~ASizePolicyDataManager()
{
    clear();
}

AIntDataManager* ASizePolicyDataManager::subIntDataManager() const
{
    return d_ptr->m_intDataManager;
}

AEnumDataManager* ASizePolicyDataManager::subEnumDataManager() const
{
    return d_ptr->m_enumDataManager;
}

QSizePolicy ASizePolicyDataManager::value(const AData* data) const
{
    return d_ptr->m_values.value(data, QSizePolicy());
}

QString ASizePolicyDataManager::toString(const AData* data) const
{
    const ASizePolicyDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();

    const QSizePolicy sp = it.value();
    const AMetaEnumProvider* mep = metaEnumProvider();
    const int hIndex = mep->sizePolicyToIndex(sp.horizontalPolicy());
    const int vIndex = mep->sizePolicyToIndex(sp.verticalPolicy());
    //! Unknown size policy on reading invalid uic3 files
    const QString hPolicy = hIndex != -1 ? mep->policyEnumNames().at(hIndex) : tr("<Invalid>");
    const QString vPolicy = vIndex != -1 ? mep->policyEnumNames().at(vIndex) : tr("<Invalid>");
    const QString str = tr("[%1, %2, %3, %4]").arg(hPolicy, vPolicy).arg(sp.horizontalStretch()).arg(sp.verticalStretch());
    return str;
}

void ASizePolicyDataManager::setValue(AData* data, const QSizePolicy& val)
{
    const ASizePolicyDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    if (it.value() == val)
        return;

    it.value() = val;

    d_ptr->m_enumDataManager->setValue(d_ptr->m_dataToHPolicy[data],
                                       metaEnumProvider()->sizePolicyToIndex(val.horizontalPolicy()));
    d_ptr->m_enumDataManager->setValue(d_ptr->m_dataToVPolicy[data],
                                       metaEnumProvider()->sizePolicyToIndex(val.verticalPolicy()));
    d_ptr->m_intDataManager->setValue(d_ptr->m_dataToHStretch[data],
                                      val.horizontalStretch());
    d_ptr->m_intDataManager->setValue(d_ptr->m_dataToVStretch[data],
                                      val.verticalStretch());

    emit dataChanged(data);
    emit valueChanged(data, val);
}

void ASizePolicyDataManager::initializeData(AData* data)
{
    QSizePolicy val;
    d_ptr->m_values[data] = val;

    AData* hPolicyProp = d_ptr->m_enumDataManager->addData();
    hPolicyProp->setName(tr("Horizontal Policy"));
    d_ptr->m_enumDataManager->setEnumNames(hPolicyProp, metaEnumProvider()->policyEnumNames());
    d_ptr->m_enumDataManager->setValue(hPolicyProp,
                                       metaEnumProvider()->sizePolicyToIndex(val.horizontalPolicy()));
    d_ptr->m_dataToHPolicy[data] = hPolicyProp;
    d_ptr->m_hPolicyToData[hPolicyProp] = data;
    data->addSubData(hPolicyProp);

    AData* vPolicyProp = d_ptr->m_enumDataManager->addData();
    vPolicyProp->setName(tr("Vertical Policy"));
    d_ptr->m_enumDataManager->setEnumNames(vPolicyProp, metaEnumProvider()->policyEnumNames());
    d_ptr->m_enumDataManager->setValue(vPolicyProp,
                                       metaEnumProvider()->sizePolicyToIndex(val.verticalPolicy()));
    d_ptr->m_dataToVPolicy[data] = vPolicyProp;
    d_ptr->m_vPolicyToData[vPolicyProp] = data;
    data->addSubData(vPolicyProp);

    AData* hStretchProp = d_ptr->m_intDataManager->addData();
    hStretchProp->setName(tr("Horizontal Stretch"));
    d_ptr->m_intDataManager->setValue(hStretchProp, val.horizontalStretch());
    d_ptr->m_intDataManager->setRange(hStretchProp, 0, 0xff);
    d_ptr->m_dataToHStretch[data] = hStretchProp;
    d_ptr->m_hStretchToData[hStretchProp] = data;
    data->addSubData(hStretchProp);

    AData* vStretchProp = d_ptr->m_intDataManager->addData();
    vStretchProp->setName(tr("Vertical Stretch"));
    d_ptr->m_intDataManager->setValue(vStretchProp, val.verticalStretch());
    d_ptr->m_intDataManager->setRange(vStretchProp, 0, 0xff);
    d_ptr->m_dataToVStretch[data] = vStretchProp;
    d_ptr->m_vStretchToData[vStretchProp] = data;
    data->addSubData(vStretchProp);
}

void ASizePolicyDataManager::uninitializeData(AData* data)
{
    AData* hPolicyProp = d_ptr->m_dataToHPolicy[data];
    if (hPolicyProp)
    {
        d_ptr->m_hPolicyToData.remove(hPolicyProp);
        delete hPolicyProp;
    }
    d_ptr->m_dataToHPolicy.remove(data);

    AData* vPolicyProp = d_ptr->m_dataToVPolicy[data];
    if (vPolicyProp)
    {
        d_ptr->m_vPolicyToData.remove(vPolicyProp);
        delete vPolicyProp;
    }
    d_ptr->m_dataToVPolicy.remove(data);

    AData* hStretchProp = d_ptr->m_dataToHStretch[data];
    if (hStretchProp)
    {
        d_ptr->m_hStretchToData.remove(hStretchProp);
        delete hStretchProp;
    }
    d_ptr->m_dataToHStretch.remove(data);

    AData* vStretchProp = d_ptr->m_dataToVStretch[data];
    if (vStretchProp)
    {
        d_ptr->m_vStretchToData.remove(vStretchProp);
        delete vStretchProp;
    }
    d_ptr->m_dataToVStretch.remove(data);

    d_ptr->m_values.remove(data);
}

void ASizePolicyDataManager::onIntChanged(AData* data, int value)
{
    if (AData* prop = d_ptr->m_hStretchToData.value(data, 0))
    {
        QSizePolicy sp = d_ptr->m_values[prop];
        sp.setHorizontalStretch(value);
        setValue(prop, sp);
    }
    else if (AData* prop = d_ptr->m_vStretchToData.value(data, 0))
    {
        QSizePolicy sp = d_ptr->m_values[prop];
        sp.setVerticalStretch(value);
        setValue(prop, sp);
    }
}

void ASizePolicyDataManager::onEnumChanged(AData* data, int value)
{
    if (AData* prop = d_ptr->m_hPolicyToData.value(data, 0))
    {
        QSizePolicy sp = d_ptr->m_values[prop];
        sp.setHorizontalPolicy(metaEnumProvider()->indexToSizePolicy(value));
        setValue(prop, sp);
    }
    else if (AData* prop = d_ptr->m_vPolicyToData.value(data, 0))
    {
        QSizePolicy sp = d_ptr->m_values[prop];
        sp.setVerticalPolicy(metaEnumProvider()->indexToSizePolicy(value));
        setValue(prop, sp);
    }
}

void ASizePolicyDataManager::onDataDestroyed(AData* data)
{
    if (AData* pointProp = d_ptr->m_hStretchToData.value(data, 0))
    {
        d_ptr->m_dataToHStretch[pointProp] = 0;
        d_ptr->m_hStretchToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_vStretchToData.value(data, 0))
    {
        d_ptr->m_dataToVStretch[pointProp] = 0;
        d_ptr->m_vStretchToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_hPolicyToData.value(data, 0))
    {
        d_ptr->m_dataToHPolicy[pointProp] = 0;
        d_ptr->m_hPolicyToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_vPolicyToData.value(data, 0))
    {
        d_ptr->m_dataToVPolicy[pointProp] = 0;
        d_ptr->m_vPolicyToData.remove(data);
    }
}

// AFontDataManager

Q_GLOBAL_STATIC(QFontDatabase, fontDatabase)

class AFontDataManagerPrivate
{
    AFontDataManager* q_ptr = nullptr;
    Q_DECLARE_PUBLIC(AFontDataManager)
public:
    QStringList m_familyNames;

    typedef QMap<const AData*, QFont> DataValueMap;
    DataValueMap m_values;

    AIntDataManager* m_intDataManager = nullptr;
    AEnumDataManager* m_enumDataManager = nullptr;
    ABoolDataManager* m_boolDataManager = nullptr;

    QMap<const AData*, AData*> m_dataToFamily;
    QMap<const AData*, AData*> m_dataToPointSize;
    QMap<const AData*, AData*> m_dataToBold;
    QMap<const AData*, AData*> m_dataToItalic;
    QMap<const AData*, AData*> m_dataToUnderline;
    QMap<const AData*, AData*> m_dataToStrikeOut;
    QMap<const AData*, AData*> m_dataToKerning;

    QMap<const AData*, AData*> m_familyToData;
    QMap<const AData*, AData*> m_pointSizeToData;
    QMap<const AData*, AData*> m_boldToData;
    QMap<const AData*, AData*> m_italicToData;
    QMap<const AData*, AData*> m_underlineToData;
    QMap<const AData*, AData*> m_strikeOutToData;
    QMap<const AData*, AData*> m_kerningToData;

    bool m_settingValue = false;
    QTimer* m_fontDatabaseChangeTimer = nullptr;
};

AFontDataManager::AFontDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new AFontDataManagerPrivate)
{
    d_ptr->q_ptr = this;
    QObject::connect(qApp, SIGNAL(fontDatabaseChanged()), this, SLOT(onFontDatabaseChanged()));

    d_ptr->m_intDataManager = new AIntDataManager(this);
    connect(d_ptr->m_intDataManager, SIGNAL(valueChanged(AData*, int)),
            this, SLOT(onIntChanged(AData*, int)));
    d_ptr->m_enumDataManager = new AEnumDataManager(this);
    connect(d_ptr->m_enumDataManager, SIGNAL(valueChanged(AData*, int)),
            this, SLOT(onEnumChanged(AData*, int)));
    d_ptr->m_boolDataManager = new ABoolDataManager(this);
    connect(d_ptr->m_boolDataManager, SIGNAL(valueChanged(AData*, bool)),
            this, SLOT(onBoolChanged(AData*, bool)));

    connect(d_ptr->m_intDataManager, SIGNAL(dataDestroyed(AData*)),
            this, SLOT(onDataDestroyed(AData*)));
    connect(d_ptr->m_enumDataManager, SIGNAL(dataDestroyed(AData*)),
            this, SLOT(onDataDestroyed(AData*)));
    connect(d_ptr->m_boolDataManager, SIGNAL(dataDestroyed(AData*)),
            this, SLOT(onDataDestroyed(AData*)));
}

AFontDataManager::~AFontDataManager()
{
    clear();
}

AIntDataManager* AFontDataManager::subIntDataManager() const
{
    return d_ptr->m_intDataManager;
}

AEnumDataManager* AFontDataManager::subEnumDataManager() const
{
    return d_ptr->m_enumDataManager;
}

ABoolDataManager* AFontDataManager::subBoolDataManager() const
{
    return d_ptr->m_boolDataManager;
}

QFont AFontDataManager::value(const AData* data) const
{
    return d_ptr->m_values.value(data, QFont());
}

QString AFontDataManager::toString(const AData* data) const
{
    const AFontDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();

    return AGraphicsToolkit::font2Text(it.value());
}

QIcon AFontDataManager::valueIcon(const AData* data) const
{
    const AFontDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QIcon();

    return AGraphicsToolkit::drawFontToPixmap(it.value());
}

void AFontDataManager::setValue(AData* data, const QFont& val)
{
    const AFontDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    const QFont oldVal = it.value();
    if (oldVal == val && oldVal.resolve(oldVal) == val.resolve(val))
        return;

    it.value() = val;

    int idx = d_ptr->m_familyNames.indexOf(val.family());
    if (idx == -1)
        idx = 0;
    bool settingValue = d_ptr->m_settingValue;
    d_ptr->m_settingValue = true;
    d_ptr->m_enumDataManager->setValue(d_ptr->m_dataToFamily[data], idx);
    d_ptr->m_intDataManager->setValue(d_ptr->m_dataToPointSize[data], val.pointSize());
    d_ptr->m_boolDataManager->setValue(d_ptr->m_dataToBold[data], val.bold());
    d_ptr->m_boolDataManager->setValue(d_ptr->m_dataToItalic[data], val.italic());
    d_ptr->m_boolDataManager->setValue(d_ptr->m_dataToUnderline[data], val.underline());
    d_ptr->m_boolDataManager->setValue(d_ptr->m_dataToStrikeOut[data], val.strikeOut());
    d_ptr->m_boolDataManager->setValue(d_ptr->m_dataToKerning[data], val.kerning());
    d_ptr->m_settingValue = settingValue;

    emit dataChanged(data);
    emit valueChanged(data, val);
}

void AFontDataManager::initializeData(AData* data)
{
    QFont val;
    d_ptr->m_values[data] = val;

    AData* familyProp = d_ptr->m_enumDataManager->addData();
    familyProp->setName(tr("Family"));
    if (d_ptr->m_familyNames.isEmpty())
        d_ptr->m_familyNames = fontDatabase()->families();
    d_ptr->m_enumDataManager->setEnumNames(familyProp, d_ptr->m_familyNames);
    int idx = d_ptr->m_familyNames.indexOf(val.family());
    if (idx == -1)
        idx = 0;
    d_ptr->m_enumDataManager->setValue(familyProp, idx);
    d_ptr->m_dataToFamily[data] = familyProp;
    d_ptr->m_familyToData[familyProp] = data;
    data->addSubData(familyProp);

    AData* pointSizeProp = d_ptr->m_intDataManager->addData();
    pointSizeProp->setName(tr("Point Size"));
    d_ptr->m_intDataManager->setValue(pointSizeProp, val.pointSize());
    d_ptr->m_intDataManager->setMinimum(pointSizeProp, 1);
    d_ptr->m_dataToPointSize[data] = pointSizeProp;
    d_ptr->m_pointSizeToData[pointSizeProp] = data;
    data->addSubData(pointSizeProp);

    AData* boldProp = d_ptr->m_boolDataManager->addData();
    boldProp->setName(tr("Bold"));
    d_ptr->m_boolDataManager->setValue(boldProp, val.bold());
    d_ptr->m_dataToBold[data] = boldProp;
    d_ptr->m_boldToData[boldProp] = data;
    data->addSubData(boldProp);

    AData* italicProp = d_ptr->m_boolDataManager->addData();
    italicProp->setName(tr("Italic"));
    d_ptr->m_boolDataManager->setValue(italicProp, val.italic());
    d_ptr->m_dataToItalic[data] = italicProp;
    d_ptr->m_italicToData[italicProp] = data;
    data->addSubData(italicProp);

    AData* underlineProp = d_ptr->m_boolDataManager->addData();
    underlineProp->setName(tr("Underline"));
    d_ptr->m_boolDataManager->setValue(underlineProp, val.underline());
    d_ptr->m_dataToUnderline[data] = underlineProp;
    d_ptr->m_underlineToData[underlineProp] = data;
    data->addSubData(underlineProp);

    AData* strikeOutProp = d_ptr->m_boolDataManager->addData();
    strikeOutProp->setName(tr("Strikeout"));
    d_ptr->m_boolDataManager->setValue(strikeOutProp, val.strikeOut());
    d_ptr->m_dataToStrikeOut[data] = strikeOutProp;
    d_ptr->m_strikeOutToData[strikeOutProp] = data;
    data->addSubData(strikeOutProp);

    AData* kerningProp = d_ptr->m_boolDataManager->addData();
    kerningProp->setName(tr("Kerning"));
    d_ptr->m_boolDataManager->setValue(kerningProp, val.kerning());
    d_ptr->m_dataToKerning[data] = kerningProp;
    d_ptr->m_kerningToData[kerningProp] = data;
    data->addSubData(kerningProp);
}

void AFontDataManager::uninitializeData(AData* data)
{
    AData* familyProp = d_ptr->m_dataToFamily[data];
    if (familyProp)
    {
        d_ptr->m_familyToData.remove(familyProp);
        delete familyProp;
    }
    d_ptr->m_dataToFamily.remove(data);

    AData* pointSizeProp = d_ptr->m_dataToPointSize[data];
    if (pointSizeProp)
    {
        d_ptr->m_pointSizeToData.remove(pointSizeProp);
        delete pointSizeProp;
    }
    d_ptr->m_dataToPointSize.remove(data);

    AData* boldProp = d_ptr->m_dataToBold[data];
    if (boldProp)
    {
        d_ptr->m_boldToData.remove(boldProp);
        delete boldProp;
    }
    d_ptr->m_dataToBold.remove(data);

    AData* italicProp = d_ptr->m_dataToItalic[data];
    if (italicProp)
    {
        d_ptr->m_italicToData.remove(italicProp);
        delete italicProp;
    }
    d_ptr->m_dataToItalic.remove(data);

    AData* underlineProp = d_ptr->m_dataToUnderline[data];
    if (underlineProp)
    {
        d_ptr->m_underlineToData.remove(underlineProp);
        delete underlineProp;
    }
    d_ptr->m_dataToUnderline.remove(data);

    AData* strikeOutProp = d_ptr->m_dataToStrikeOut[data];
    if (strikeOutProp)
    {
        d_ptr->m_strikeOutToData.remove(strikeOutProp);
        delete strikeOutProp;
    }
    d_ptr->m_dataToStrikeOut.remove(data);

    AData* kerningProp = d_ptr->m_dataToKerning[data];
    if (kerningProp)
    {
        d_ptr->m_kerningToData.remove(kerningProp);
        delete kerningProp;
    }
    d_ptr->m_dataToKerning.remove(data);

    d_ptr->m_values.remove(data);
}

void AFontDataManager::onIntChanged(AData* data, int value)
{
    if (d_ptr->m_settingValue)
        return;
    if (AData* prop = d_ptr->m_pointSizeToData.value(data, 0))
    {
        QFont f = d_ptr->m_values[prop];
        f.setPointSize(value);
        setValue(prop, f);
    }
}

void AFontDataManager::onEnumChanged(AData* data, int value)
{
    if (d_ptr->m_settingValue)
        return;
    if (AData* prop = d_ptr->m_familyToData.value(data, 0))
    {
        QFont f = d_ptr->m_values[prop];
        f.setFamily(d_ptr->m_familyNames.at(value));
        setValue(prop, f);
    }
}

void AFontDataManager::onBoolChanged(AData* data, bool value)
{
    if (d_ptr->m_settingValue)
        return;
    if (AData* prop = d_ptr->m_boldToData.value(data, 0))
    {
        QFont f = d_ptr->m_values[prop];
        f.setBold(value);
        setValue(prop, f);
    }
    else if (AData* prop = d_ptr->m_italicToData.value(data, 0))
    {
        QFont f = d_ptr->m_values[prop];
        f.setItalic(value);
        setValue(prop, f);
    }
    else if (AData* prop = d_ptr->m_underlineToData.value(data, 0))
    {
        QFont f = d_ptr->m_values[prop];
        f.setUnderline(value);
        setValue(prop, f);
    }
    else if (AData* prop = d_ptr->m_strikeOutToData.value(data, 0))
    {
        QFont f = d_ptr->m_values[prop];
        f.setStrikeOut(value);
        setValue(prop, f);
    }
    else if (AData* prop = d_ptr->m_kerningToData.value(data, 0))
    {
        QFont f = d_ptr->m_values[prop];
        f.setKerning(value);
        setValue(prop, f);
    }
}

void AFontDataManager::onDataDestroyed(AData* data)
{
    if (AData* pointProp = d_ptr->m_pointSizeToData.value(data, 0))
    {
        d_ptr->m_dataToPointSize[pointProp] = 0;
        d_ptr->m_pointSizeToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_familyToData.value(data, 0))
    {
        d_ptr->m_dataToFamily[pointProp] = 0;
        d_ptr->m_familyToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_boldToData.value(data, 0))
    {
        d_ptr->m_dataToBold[pointProp] = 0;
        d_ptr->m_boldToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_italicToData.value(data, 0))
    {
        d_ptr->m_dataToItalic[pointProp] = 0;
        d_ptr->m_italicToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_underlineToData.value(data, 0))
    {
        d_ptr->m_dataToUnderline[pointProp] = 0;
        d_ptr->m_underlineToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_strikeOutToData.value(data, 0))
    {
        d_ptr->m_dataToStrikeOut[pointProp] = 0;
        d_ptr->m_strikeOutToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_kerningToData.value(data, 0))
    {
        d_ptr->m_dataToKerning[pointProp] = 0;
        d_ptr->m_kerningToData.remove(data);
    }
}

void AFontDataManager::onFontDatabaseChanged()
{
    if (!d_ptr->m_fontDatabaseChangeTimer)
    {
        d_ptr->m_fontDatabaseChangeTimer = new QTimer(this);
        d_ptr->m_fontDatabaseChangeTimer->setInterval(0);
        d_ptr->m_fontDatabaseChangeTimer->setSingleShot(true);
        QObject::connect(d_ptr->m_fontDatabaseChangeTimer, SIGNAL(timeout()), this, SLOT(onFontDatabaseDelayedChange()));
    }
    if (!d_ptr->m_fontDatabaseChangeTimer->isActive())
        d_ptr->m_fontDatabaseChangeTimer->start();
}

void AFontDataManager::onFontDatabaseDelayedChange()
{
    typedef QMap<const AData*, AData*> DataDataMap;
    // rescan available font names
    const QStringList oldFamilies = d_ptr->m_familyNames;
    d_ptr->m_familyNames = fontDatabase()->families();

    // Adapt all existing properties
    if (!d_ptr->m_dataToFamily.isEmpty())
    {
        DataDataMap::const_iterator cend = d_ptr->m_dataToFamily.constEnd();
        for (DataDataMap::const_iterator it = d_ptr->m_dataToFamily.constBegin(); it != cend; ++it)
        {
            AData* familyProp = it.value();
            const int oldIdx = d_ptr->m_enumDataManager->value(familyProp);
            int newIdx = d_ptr->m_familyNames.indexOf(oldFamilies.at(oldIdx));
            if (newIdx < 0)
                newIdx = 0;
            d_ptr->m_enumDataManager->setEnumNames(familyProp, d_ptr->m_familyNames);
            d_ptr->m_enumDataManager->setValue(familyProp, newIdx);
        }
    }
}

// AColorDataManager

class AColorDataManagerPrivate
{
    AColorDataManager* q_ptr = nullptr;
    Q_DECLARE_PUBLIC(AColorDataManager)
public:
    typedef QMap<const AData*, QColor> DataValueMap;
    DataValueMap m_values;

    AIntDataManager* m_intDataManager = nullptr;

    QMap<const AData*, AData*> m_dataToR;
    QMap<const AData*, AData*> m_dataToG;
    QMap<const AData*, AData*> m_dataToB;
    QMap<const AData*, AData*> m_dataToA;

    QMap<const AData*, AData*> m_rToData;
    QMap<const AData*, AData*> m_gToData;
    QMap<const AData*, AData*> m_bToData;
    QMap<const AData*, AData*> m_aToData;
};

AColorDataManager::AColorDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new AColorDataManagerPrivate)
{
    d_ptr->q_ptr = this;

    d_ptr->m_intDataManager = new AIntDataManager(this);
    connect(d_ptr->m_intDataManager, SIGNAL(valueChanged(AData*, int)), this, SLOT(onIntChanged(AData*, int)));
    connect(d_ptr->m_intDataManager, SIGNAL(dataDestroyed(AData*)), this, SLOT(onDataDestroyed(AData*)));
}

AColorDataManager::~AColorDataManager()
{
    clear();
}

AIntDataManager* AColorDataManager::subIntDataManager() const
{
    return d_ptr->m_intDataManager;
}

QColor AColorDataManager::value(const AData* data) const
{
    return d_ptr->m_values.value(data, QColor());
}

QString AColorDataManager::toString(const AData* data) const
{
    const AColorDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();

    return AGraphicsToolkit::color2Text(it.value());
}

QIcon AColorDataManager::valueIcon(const AData* data) const
{
    const AColorDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QIcon();
    return AGraphicsToolkit::drawBrushToPixmap(QBrush(it.value()));
}

void AColorDataManager::setValue(AData* data, const QColor& val)
{
    const AColorDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    if (it.value() == val)
        return;

    it.value() = val;

    d_ptr->m_intDataManager->setValue(d_ptr->m_dataToR[data], val.red());
    d_ptr->m_intDataManager->setValue(d_ptr->m_dataToG[data], val.green());
    d_ptr->m_intDataManager->setValue(d_ptr->m_dataToB[data], val.blue());
    d_ptr->m_intDataManager->setValue(d_ptr->m_dataToA[data], val.alpha());

    emit dataChanged(data);
    emit valueChanged(data, val);
}

void AColorDataManager::initializeData(AData* data)
{
    QColor val;
    d_ptr->m_values[data] = val;

    AData* rProp = d_ptr->m_intDataManager->addData();
    rProp->setName(tr("Red"));
    d_ptr->m_intDataManager->setValue(rProp, val.red());
    d_ptr->m_intDataManager->setRange(rProp, 0, 0xFF);
    d_ptr->m_dataToR[data] = rProp;
    d_ptr->m_rToData[rProp] = data;
    data->addSubData(rProp);

    AData* gProp = d_ptr->m_intDataManager->addData();
    gProp->setName(tr("Green"));
    d_ptr->m_intDataManager->setValue(gProp, val.green());
    d_ptr->m_intDataManager->setRange(gProp, 0, 0xFF);
    d_ptr->m_dataToG[data] = gProp;
    d_ptr->m_gToData[gProp] = data;
    data->addSubData(gProp);

    AData* bProp = d_ptr->m_intDataManager->addData();
    bProp->setName(tr("Blue"));
    d_ptr->m_intDataManager->setValue(bProp, val.blue());
    d_ptr->m_intDataManager->setRange(bProp, 0, 0xFF);
    d_ptr->m_dataToB[data] = bProp;
    d_ptr->m_bToData[bProp] = data;
    data->addSubData(bProp);

    AData* aProp = d_ptr->m_intDataManager->addData();
    aProp->setName(tr("Alpha"));
    d_ptr->m_intDataManager->setValue(aProp, val.alpha());
    d_ptr->m_intDataManager->setRange(aProp, 0, 0xFF);
    d_ptr->m_dataToA[data] = aProp;
    d_ptr->m_aToData[aProp] = data;
    data->addSubData(aProp);
}

void AColorDataManager::uninitializeData(AData* data)
{
    AData* rProp = d_ptr->m_dataToR[data];
    if (rProp)
    {
        d_ptr->m_rToData.remove(rProp);
        delete rProp;
    }
    d_ptr->m_dataToR.remove(data);

    AData* gProp = d_ptr->m_dataToG[data];
    if (gProp)
    {
        d_ptr->m_gToData.remove(gProp);
        delete gProp;
    }
    d_ptr->m_dataToG.remove(data);

    AData* bProp = d_ptr->m_dataToB[data];
    if (bProp)
    {
        d_ptr->m_bToData.remove(bProp);
        delete bProp;
    }
    d_ptr->m_dataToB.remove(data);

    AData* aProp = d_ptr->m_dataToA[data];
    if (aProp)
    {
        d_ptr->m_aToData.remove(aProp);
        delete aProp;
    }
    d_ptr->m_dataToA.remove(data);

    d_ptr->m_values.remove(data);
}

void AColorDataManager::onIntChanged(AData* data, int value)
{
    if (AData* prop = d_ptr->m_rToData.value(data, 0))
    {
        QColor c = d_ptr->m_values[prop];
        c.setRed(value);
        setValue(prop, c);
    }
    else if (AData* prop = d_ptr->m_gToData.value(data, 0))
    {
        QColor c = d_ptr->m_values[prop];
        c.setGreen(value);
        setValue(prop, c);
    }
    else if (AData* prop = d_ptr->m_bToData.value(data, 0))
    {
        QColor c = d_ptr->m_values[prop];
        c.setBlue(value);
        setValue(prop, c);
    }
    else if (AData* prop = d_ptr->m_aToData.value(data, 0))
    {
        QColor c = d_ptr->m_values[prop];
        c.setAlpha(value);
        setValue(prop, c);
    }
}

void AColorDataManager::onDataDestroyed(AData* data)
{
    if (AData* pointProp = d_ptr->m_rToData.value(data, 0))
    {
        d_ptr->m_dataToR[pointProp] = 0;
        d_ptr->m_rToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_gToData.value(data, 0))
    {
        d_ptr->m_dataToG[pointProp] = 0;
        d_ptr->m_gToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_bToData.value(data, 0))
    {
        d_ptr->m_dataToB[pointProp] = 0;
        d_ptr->m_bToData.remove(data);
    }
    else if (AData* pointProp = d_ptr->m_aToData.value(data, 0))
    {
        d_ptr->m_dataToA[pointProp] = 0;
        d_ptr->m_aToData.remove(data);
    }
}

// Make sure icons are removed as soon as QApplication is destroyed, otherwise,
// handles are leaked on X11.
static void clearCursorDatabase();
namespace
{
    struct CursorDatabase : public ACursorDatabase
    {
        CursorDatabase()
        {
            qAddPostRoutine(clearCursorDatabase);
        }
    };
}
Q_GLOBAL_STATIC(ACursorDatabase, cursorDatabase)

static void clearCursorDatabase()
{
    cursorDatabase()->clear();
}

class ACursorDataManagerPrivate
{
    ACursorDataManager* q_ptr = nullptr;
    Q_DECLARE_PUBLIC(ACursorDataManager)
public:
    typedef QMap<const AData*, QCursor> DataValueMap;
    DataValueMap m_values;
};

ACursorDataManager::ACursorDataManager(QObject* parent)
    : AAbstractDataManager(parent), d_ptr(new ACursorDataManagerPrivate)
{
    d_ptr->q_ptr = this;
}

ACursorDataManager::~ACursorDataManager()
{
    clear();
}

#ifndef QT_NO_CURSOR
QCursor ACursorDataManager::value(const AData* data) const
{
    return d_ptr->m_values.value(data, QCursor());
}
#endif

QString ACursorDataManager::toString(const AData* data) const
{
    const ACursorDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QString();

    return cursorDatabase()->getCursorShapeName(it.value());
}

QIcon ACursorDataManager::valueIcon(const AData* data) const
{
    const ACursorDataManagerPrivate::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);
    if (it == d_ptr->m_values.constEnd())
        return QIcon();

    return cursorDatabase()->getCursorShapeIcon(it.value());
}

void ACursorDataManager::setValue(AData* data, const QCursor& value)
{
#ifndef QT_NO_CURSOR
    const ACursorDataManagerPrivate::DataValueMap::iterator it = d_ptr->m_values.find(data);
    if (it == d_ptr->m_values.end())
        return;

    if (it.value().shape() == value.shape() && value.shape() != Qt::BitmapCursor)
        return;

    it.value() = value;

    emit dataChanged(data);
    emit valueChanged(data, value);
#endif
}

void ACursorDataManager::initializeData(AData* data)
{
#ifndef QT_NO_CURSOR
    d_ptr->m_values[data] = QCursor();
#endif
}

void ACursorDataManager::uninitializeData(AData* data)
{
    d_ptr->m_values.remove(data);
}

// ----------------------------------------------------------------------------------------------------

A_DATA_MANAGER_REGISTER(AInt8DataManager, QMetaType::Char);
A_DATA_MANAGER_REGISTER(AUInt8DataManager, QMetaType::UChar);
A_DATA_MANAGER_REGISTER(AInt16DataManager, QMetaType::Short);
A_DATA_MANAGER_REGISTER(AUInt16DataManager, QMetaType::UShort);
A_DATA_MANAGER_REGISTER(AIntDataManager, QMetaType::Int);
A_DATA_MANAGER_REGISTER(AUIntDataManager, QMetaType::UInt);
A_DATA_MANAGER_REGISTER(ALongLongDataManager, QMetaType::LongLong);
A_DATA_MANAGER_REGISTER(AULongLongDataManager, QMetaType::ULongLong);
A_DATA_MANAGER_REGISTER(AFloatDataManager, QMetaType::Float);
A_DATA_MANAGER_REGISTER(ADoubleDataManager, QMetaType::Double);
A_DATA_MANAGER_REGISTER(ABoolDataManager, QMetaType::Bool);
A_DATA_MANAGER_REGISTER(AStringDataManager, QMetaType::QString);
A_DATA_MANAGER_REGISTER(AStringListDataManager, QMetaType::QStringList);
A_DATA_MANAGER_REGISTER(ADateDataManager, QMetaType::QDate);
A_DATA_MANAGER_REGISTER(ATimeDataManager, QMetaType::QTime);
A_DATA_MANAGER_REGISTER(ADateTimeDataManager, QMetaType::QDateTime);
A_DATA_MANAGER_REGISTER(AKeySequenceDataManager, QMetaType::QKeySequence);
A_DATA_MANAGER_REGISTER(ACharDataManager, QMetaType::QChar);
A_DATA_MANAGER_REGISTER(ALocaleDataManager, QMetaType::QLocale);
A_DATA_MANAGER_REGISTER(APointDataManager, QMetaType::QPoint);
A_DATA_MANAGER_REGISTER(APointFDataManager, QMetaType::QPointF);
A_DATA_MANAGER_REGISTER(ASizeDataManager, QMetaType::QSize);
A_DATA_MANAGER_REGISTER(ASizeFDataManager, QMetaType::QSizeF);
A_DATA_MANAGER_REGISTER(ARectDataManager, QMetaType::QRect);
A_DATA_MANAGER_REGISTER(ARectFDataManager, QMetaType::QRectF);
//A_DATA_MANAGER_REGISTER(AEnumDataManager, QMetaType::Enum);   // TODO
//A_DATA_MANAGER_REGISTER(AFlagDataManager, QMetaType::QFlag);
A_DATA_MANAGER_REGISTER(ASizePolicyDataManager, QMetaType::QSizePolicy);
A_DATA_MANAGER_REGISTER(AFontDataManager, QMetaType::QFont);
A_DATA_MANAGER_REGISTER(AColorDataManager, QMetaType::QColor);
A_DATA_MANAGER_REGISTER(ACursorDataManager, QMetaType::QCursor);


APROCH_NAMESPACE_END

#include "ADataManager.moc"