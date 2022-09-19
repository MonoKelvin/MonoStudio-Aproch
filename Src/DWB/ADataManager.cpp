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

#include <QCoreApplication>

namespace aproch
{
    Q_GLOBAL_STATIC(QFontDatabase, fontDatabase)

        AIntDataManager::AIntDataManager(QObject* parent)
        : AAbstractDataManager(parent)
    {
    }

    AIntDataManager::~AIntDataManager()
    {
        clear();
    }

    int AIntDataManager::value(const AData* data) const
    {
        return getValue<int>(m_values, data, 0);
    }

    int AIntDataManager::minimum(const AData* data) const
    {
        return getMinimum<int>(m_values, data, 0);
    }

    int AIntDataManager::maximum(const AData* data) const
    {
        return getMaximum<int>(m_values, data, 0);
    }

    int AIntDataManager::singleStep(const AData* data) const
    {
        return getData<int>(m_values, &Data::singleStep, data, 1);
    }

    void AIntDataManager::setValue(AData* data, int val)
    {
        void (AIntDataManager:: * setSubDataValue)(AData*, int) = 0;
        setValueInRange<int, AIntDataManager, int>(this,
                                                   &AIntDataManager::dataChanged,
                                                   &AIntDataManager::valueChanged,
                                                   data, val,
                                                   setSubDataValue);
    }

    void AIntDataManager::setMinimum(AData* data, int minVal)
    {
        setMinimumValue<int, AIntDataManager, int, AIntDataManager::Data>(this,
                                                                          &AIntDataManager::dataChanged,
                                                                          &AIntDataManager::valueChanged,
                                                                          &AIntDataManager::rangeChanged,
                                                                          data, minVal);
    }

    void AIntDataManager::setMaximum(AData* data, int maxVal)
    {
        setMaximumValue<int, AIntDataManager, int, AIntDataManager::Data>(this,
                                                                          &AIntDataManager::dataChanged,
                                                                          &AIntDataManager::valueChanged,
                                                                          &AIntDataManager::rangeChanged,
                                                                          data, maxVal);
    }

    void AIntDataManager::setRange(AData* data, int minVal, int maxVal)
    {
        void (AIntDataManager:: * setSubDataRange)(AData*, int, int, int) = 0;
        setBorderValues<int, AIntDataManager, int>(this,
                                                   &AIntDataManager::dataChanged,
                                                   &AIntDataManager::valueChanged,
                                                   &AIntDataManager::rangeChanged,
                                                   data, minVal, maxVal, setSubDataRange);
    }

    void AIntDataManager::setSingleStep(AData* data, int step)
    {
        const AIntDataManager::DataValueMap::iterator it = m_values.find(data);
        if (it == m_values.end())
            return;

        AIntDataManager::Data dataVal = it.value();

        if (step < 0)
            step = 0;

        if (dataVal.singleStep == step)
            return;

        dataVal.singleStep = step;

        it.value() = dataVal;

        emit singleStepChanged(data, dataVal.singleStep);
    }

    QString AIntDataManager::valueText(const AData* data) const
    {
        const AIntDataManager::DataValueMap::const_iterator it = m_values.constFind(data);
        if (it == m_values.constEnd())
            return QString();
        return QString::number(it.value().val);
    }

    void AIntDataManager::initializeData(AData* data)
    {
        m_values[data] = AIntDataManager::Data();
    }

    void AIntDataManager::uninitializeData(AData* data)
    {
        m_values.remove(data);
    }

    // ----------------------------------------------------------------------------------------------------

    ADoubleDataManager::ADoubleDataManager(QObject* parent)
        : AAbstractDataManager(parent)
    {
    }

    ADoubleDataManager::~ADoubleDataManager()
    {
        clear();
    }

    double ADoubleDataManager::value(const AData* data) const
    {
        return getValue<double>(m_values, data, 0);
    }

    double ADoubleDataManager::minimum(const AData* data) const
    {
        return getMinimum<double>(m_values, data, 0);
    }

    double ADoubleDataManager::maximum(const AData* data) const
    {
        return getMaximum<double>(m_values, data, 0);
    }

    double ADoubleDataManager::singleStep(const AData* data) const
    {
        return getData<double>(m_values, &Data::singleStep, data, 1);
    }

    int ADoubleDataManager::decimals(const AData* data) const
    {
        return getData<int>(m_values, &Data::decimals, data, 1);
    }

    void ADoubleDataManager::setValue(AData* data, double val)
    {
        void (ADoubleDataManager:: * setSubDataValue)(AData*, double) = 0;
        setValueInRange<double, ADoubleDataManager, double>(this,
                                                            &ADoubleDataManager::dataChanged,
                                                            &ADoubleDataManager::valueChanged,
                                                            data, val,
                                                            setSubDataValue);
    }

    void ADoubleDataManager::setMinimum(AData* data, double minVal)
    {
        setMinimumValue<double, ADoubleDataManager, double, ADoubleDataManager::Data>(this,
                                                                                      &ADoubleDataManager::dataChanged,
                                                                                      &ADoubleDataManager::valueChanged,
                                                                                      &ADoubleDataManager::rangeChanged,
                                                                                      data, minVal);
    }

    void ADoubleDataManager::setMaximum(AData* data, double maxVal)
    {
        setMaximumValue<double, ADoubleDataManager, double, ADoubleDataManager::Data>(this,
                                                                                      &ADoubleDataManager::dataChanged,
                                                                                      &ADoubleDataManager::valueChanged,
                                                                                      &ADoubleDataManager::rangeChanged,
                                                                                      data, maxVal);
    }

    void ADoubleDataManager::setRange(AData* data, double minVal, double maxVal)
    {
        void (ADoubleDataManager:: * setSubDataRange)(AData*, double, double, double) = 0;
        setBorderValues<double, ADoubleDataManager, double>(this,
                                                            &ADoubleDataManager::dataChanged,
                                                            &ADoubleDataManager::valueChanged,
                                                            &ADoubleDataManager::rangeChanged,
                                                            data, minVal, maxVal, setSubDataRange);
    }

    void ADoubleDataManager::setSingleStep(AData* data, double step)
    {
        const ADoubleDataManager::DataValueMap::iterator it = m_values.find(data);
        if (it == m_values.end())
            return;

        ADoubleDataManager::Data dataVal = it.value();

        if (step < 0)
            step = 0;

        if (dataVal.singleStep == step)
            return;

        dataVal.singleStep = step;

        it.value() = dataVal;

        emit singleStepChanged(data, dataVal.singleStep);
    }

    void ADoubleDataManager::setDecimals(AData* data, int prec)
    {
        const DataValueMap::iterator it = m_values.find(data);
        if (it == m_values.end())
            return;

        Data dataVal = it.value();

        if (prec > 13)
            prec = 13;
        else if (prec < 0)
            prec = 0;

        if (dataVal.decimals == prec)
            return;

        dataVal.decimals = prec;

        it.value() = dataVal;

        emit decimalsChanged(data, dataVal.decimals);
    }

    QString ADoubleDataManager::valueText(const AData* data) const
    {
        const ADoubleDataManager::DataValueMap::const_iterator it = m_values.constFind(data);
        if (it == m_values.constEnd())
            return QString();
        return QString::number(it.value().val, 'f', it.value().decimals);
    }

    void ADoubleDataManager::initializeData(AData* data)
    {
        m_values[data] = ADoubleDataManager::Data();
    }

    void ADoubleDataManager::uninitializeData(AData* data)
    {
        m_values.remove(data);
    }

    // ----------------------------------------------------------------------------------------------------

    AStringDataManager::AStringDataManager(QObject* parent)
        : AAbstractDataManager(parent)
    {
    }

    AStringDataManager::~AStringDataManager()
    {
        clear();
    }

    QString AStringDataManager::value(const AData* data) const
    {
        return getValue<QString>(m_values, data);
    }

    QRegExp AStringDataManager::regExp(const AData* data) const
    {
        return getData<QRegExp>(m_values, &Data::regExp, data, QRegExp());
    }

    void AStringDataManager::setValue(AData* data, const QString& val)
    {
        const auto it = m_values.find(data);
        if (it == m_values.end())
            return;

        Data dataVal = it.value();

        if (dataVal.val == val)
            return;

        if (dataVal.regExp.isValid() && !dataVal.regExp.exactMatch(val))
            return;

        dataVal.val = val;

        it.value() = dataVal;

        emit dataChanged(data);
        emit valueChanged(data, dataVal.val);
    }

    void AStringDataManager::setRegExp(AData* data, const QRegExp& regExp)
    {
        const auto it = m_values.find(data);
        if (it == m_values.end())
            return;

        Data dataVal = it.value();

        if (dataVal.regExp == regExp)
            return;

        dataVal.regExp = regExp;

        it.value() = dataVal;

        emit regExpChanged(data, dataVal.regExp);
    }

    QString AStringDataManager::valueText(const AData* data) const
    {
        auto it = m_values.constFind(data);
        if (it == m_values.constEnd())
            return QString();
        return it.value().val;
    }

    void AStringDataManager::initializeData(AData* data)
    {
        m_values[data] = Data();
    }

    void AStringDataManager::uninitializeData(AData* data)
    {
        m_values.remove(data);
    }

    // ----------------------------------------------------------------------------------------------------

    ABoolDataManager::ABoolDataManager(QObject* parent)
        : AAbstractDataManager(parent)
    {
    }

    ABoolDataManager::~ABoolDataManager()
    {
        clear();
    }

    bool ABoolDataManager::value(const AData* data) const
    {
        return m_values.value(data, false);
    }

    QString ABoolDataManager::valueText(const AData* data) const
    {
        const QMap<const AData*, bool>::const_iterator it = m_values.constFind(data);
        if (it == m_values.constEnd())
            return QString();

        static const QString trueText = tr("True");
        static const QString falseText = tr("False");
        return it.value() ? trueText : falseText;
    }

    QIcon ABoolDataManager::valueIcon(const AData* data) const
    {
        const QMap<const AData*, bool>::const_iterator it = m_values.constFind(data);
        if (it == m_values.constEnd())
            return QIcon();

        return it.value() ? m_checkedIcon : m_uncheckedIcon;
    }

    void ABoolDataManager::setValue(AData* data, bool val)
    {
        setSimpleValue<bool, bool, ABoolDataManager>(m_values, this,
                                                     &ABoolDataManager::dataChanged,
                                                     &ABoolDataManager::valueChanged,
                                                     data, val);
    }

    void ABoolDataManager::initializeData(AData* data)
    {
        m_values[data] = false;
    }

    void ABoolDataManager::uninitializeData(AData* data)
    {
        m_values.remove(data);
    }

    // ----------------------------------------------------------------------------------------------------

    ASizeDataManager::ASizeDataManager(QObject* parent)
        : AAbstractDataManager(parent)
    {
        m_intDataManager = new AIntDataManager(this);
        connect(m_intDataManager, SIGNAL(valueChanged(AData*, int)), this, SLOT(slotIntChanged(AData*, int)));
        connect(m_intDataManager, SIGNAL(dataDestroyed(AData*)), this, SLOT(slotDataDestroyed(AData*)));
    }

    ASizeDataManager::~ASizeDataManager()
    {
        clear();
    }

    AIntDataManager* ASizeDataManager::subIntDataManager() const
    {
        return m_intDataManager;
    }

    QSize ASizeDataManager::value(const AData* data) const
    {
        return getValue<QSize>(m_values, data);
    }

    QSize ASizeDataManager::minimum(const AData* data) const
    {
        return getMinimum<QSize>(m_values, data);
    }

    QSize ASizeDataManager::maximum(const AData* data) const
    {
        return getMaximum<QSize>(m_values, data);
    }

    QString ASizeDataManager::valueText(const AData* data) const
    {
        const ASizeDataManager::DataValueMap::const_iterator it = m_values.constFind(data);
        if (it == m_values.constEnd())
            return QString();
        const QSize v = it.value().val;
        return tr("%1 x %2").arg(QString::number(v.width())).arg(QString::number(v.height()));
    }

    void ASizeDataManager::setValue(AData* data, const QSize& val)
    {
        setValueInRange<const QSize&, ASizeDataManager, const QSize>(this,
                                                                     &ASizeDataManager::dataChanged,
                                                                     &ASizeDataManager::valueChanged,
                                                                     data, val, &ASizeDataManager::_setValue);
    }

    void ASizeDataManager::setMinimum(AData* data, const QSize& minVal)
    {
        setBorderValue<const QSize&, ASizeDataManager, QSize, ASizeDataManager::Data>(this,
                                                                                      &ASizeDataManager::dataChanged,
                                                                                      &ASizeDataManager::valueChanged,
                                                                                      &ASizeDataManager::rangeChanged,
                                                                                      data,
                                                                                      &ASizeDataManager::Data::minimumValue,
                                                                                      &ASizeDataManager::Data::setMinimumValue,
                                                                                      minVal, &ASizeDataManager::_setRange);
    }

    void ASizeDataManager::setMaximum(AData* data, const QSize& maxVal)
    {
        setBorderValue<const QSize&, ASizeDataManager, QSize, ASizeDataManager::Data>(this,
                                                                                      &ASizeDataManager::dataChanged,
                                                                                      &ASizeDataManager::valueChanged,
                                                                                      &ASizeDataManager::rangeChanged,
                                                                                      data,
                                                                                      &ASizeDataManager::Data::maximumValue,
                                                                                      &ASizeDataManager::Data::setMaximumValue,
                                                                                      maxVal, &ASizeDataManager::_setRange);
    }

    void ASizeDataManager::setRange(AData* data, const QSize& minVal, const QSize& maxVal)
    {
        setBorderValues<const QSize&, ASizeDataManager, QSize>(this,
                                                               &ASizeDataManager::dataChanged,
                                                               &ASizeDataManager::valueChanged,
                                                               &ASizeDataManager::rangeChanged,
                                                               data, minVal, maxVal,
                                                               &ASizeDataManager::_setRange);
    }

    void ASizeDataManager::initializeData(AData* data)
    {
        m_values[data] = ASizeDataManager::Data();

        AData* wProp = m_intDataManager->addData();
        wProp->setDataName(tr("Width"));
        m_intDataManager->setValue(wProp, 0);
        m_intDataManager->setMinimum(wProp, 0);
        m_dataToW[data] = wProp;
        m_wToData[wProp] = data;
        data->addSubData(wProp);

        AData* hProp = m_intDataManager->addData();
        hProp->setDataName(tr("Height"));
        m_intDataManager->setValue(hProp, 0);
        m_intDataManager->setMinimum(hProp, 0);
        m_dataToH[data] = hProp;
        m_hToData[hProp] = data;
        data->addSubData(hProp);
    }

    void ASizeDataManager::uninitializeData(AData* data)
    {
        AData* wProp = m_dataToW[data];
        if (wProp)
        {
            m_wToData.remove(wProp);
            delete wProp;
        }
        m_dataToW.remove(data);

        AData* hProp = m_dataToH[data];
        if (hProp)
        {
            m_hToData.remove(hProp);
            delete hProp;
        }
        m_dataToH.remove(data);

        m_values.remove(data);
    }

    void ASizeDataManager::slotIntChanged(AData* data, int val)
    {
        if (AData* prop = m_wToData.value(data, 0))
        {
            QSize s = m_values[prop].val;
            s.setWidth(val);
            setValue(prop, s);
        }
        else if (AData* prop = m_hToData.value(data, 0))
        {
            QSize s = m_values[prop].val;
            s.setHeight(val);
            setValue(prop, s);
        }
    }

    void ASizeDataManager::slotDataDestroyed(AData* data)
    {
        if (AData* pointProp = m_wToData.value(data, 0))
        {
            m_dataToW[pointProp] = 0;
            m_wToData.remove(data);
        }
        else if (AData* pointProp = m_hToData.value(data, 0))
        {
            m_dataToH[pointProp] = 0;
            m_hToData.remove(data);
        }
    }

    void ASizeDataManager::_setValue(AData* data, const QSize& val)
    {
        m_intDataManager->setValue(m_dataToW.value(data), val.width());
        m_intDataManager->setValue(m_dataToH.value(data), val.height());
    }

    void ASizeDataManager::_setRange(AData* data, const QSize& minVal, const QSize& maxVal, const QSize& val)
    {
        AData* wData = m_dataToW.value(data);
        AData* hData = m_dataToH.value(data);
        m_intDataManager->setRange(wData, minVal.width(), maxVal.width());
        m_intDataManager->setValue(wData, val.width());
        m_intDataManager->setRange(hData, minVal.height(), maxVal.height());
        m_intDataManager->setValue(hData, val.height());
    }

    // ----------------------------------------------------------------------------------------------------

    APointDataManager::APointDataManager(QObject* parent)
        : AAbstractDataManager(parent)
    {
        m_intDataManager = new AIntDataManager(this);
        connect(m_intDataManager, SIGNAL(valueChanged(AData*, int)), this, SLOT(slotIntChanged(AData*, int)));
        connect(m_intDataManager, SIGNAL(dataDestroyed(AData*)), this, SLOT(slotDataDestroyed(AData*)));
    }

    APointDataManager::~APointDataManager()
    {
        clear();
    }

    AIntDataManager* APointDataManager::subIntDataManager() const
    {
        return m_intDataManager;
    }

    QPoint APointDataManager::value(const AData* data) const
    {
        return getValue<QPoint>(m_values, data);
    }

    QPoint APointDataManager::minimum(const AData* data) const
    {
        return getMinimum<QPoint>(m_values, data);
    }

    QPoint APointDataManager::maximum(const AData* data) const
    {
        return getMaximum<QPoint>(m_values, data);
    }

    QString APointDataManager::valueText(const AData* data) const
    {
        const APointDataManager::DataValueMap::const_iterator it = m_values.constFind(data);
        if (it == m_values.constEnd())
            return QString();
        const QPoint v = it.value().val;
        return tr("(%1, %2)").arg(QString::number(v.x())).arg(QString::number(v.y()));
    }

    void APointDataManager::setValue(AData* data, const QPoint& val)
    {
        setValueInRange<const QPoint&, APointDataManager, const QPoint>(this,
                                                                        &APointDataManager::dataChanged,
                                                                        &APointDataManager::valueChanged,
                                                                        data, val, &APointDataManager::_setValue);
    }

    void APointDataManager::setMinimum(AData* data, const QPoint& minVal)
    {
        setBorderValue<const QPoint&, APointDataManager, QPoint, APointDataManager::Data>(this,
                                                                                          &APointDataManager::dataChanged,
                                                                                          &APointDataManager::valueChanged,
                                                                                          &APointDataManager::rangeChanged,
                                                                                          data,
                                                                                          &APointDataManager::Data::minimumValue,
                                                                                          &APointDataManager::Data::setMinimumValue,
                                                                                          minVal, &APointDataManager::_setRange);
    }

    void APointDataManager::setMaximum(AData* data, const QPoint& maxVal)
    {
        setBorderValue<const QPoint&, APointDataManager, QPoint, APointDataManager::Data>(this,
                                                                                          &APointDataManager::dataChanged,
                                                                                          &APointDataManager::valueChanged,
                                                                                          &APointDataManager::rangeChanged,
                                                                                          data,
                                                                                          &APointDataManager::Data::maximumValue,
                                                                                          &APointDataManager::Data::setMaximumValue,
                                                                                          maxVal, &APointDataManager::_setRange);
    }

    void APointDataManager::setRange(AData* data, const QPoint& minVal, const QPoint& maxVal)
    {
        setBorderValues<const QPoint&, APointDataManager, QPoint>(this,
                                                                  &APointDataManager::dataChanged,
                                                                  &APointDataManager::valueChanged,
                                                                  &APointDataManager::rangeChanged,
                                                                  data, minVal, maxVal,
                                                                  &APointDataManager::_setRange);
    }

    void APointDataManager::initializeData(AData* data)
    {
        m_values[data] = APointDataManager::Data();

        AData* wProp = m_intDataManager->addData();
        wProp->setDataName(tr("Width"));
        m_intDataManager->setValue(wProp, 0);
        m_intDataManager->setMinimum(wProp, 0);
        m_dataToX[data] = wProp;
        m_xToData[wProp] = data;
        data->addSubData(wProp);

        AData* hProp = m_intDataManager->addData();
        hProp->setDataName(tr("Height"));
        m_intDataManager->setValue(hProp, 0);
        m_intDataManager->setMinimum(hProp, 0);
        m_dataToY[data] = hProp;
        m_yToData[hProp] = data;
        data->addSubData(hProp);
    }

    void APointDataManager::uninitializeData(AData* data)
    {
        AData* wProp = m_dataToX[data];
        if (wProp)
        {
            m_xToData.remove(wProp);
            delete wProp;
        }
        m_dataToX.remove(data);

        AData* hProp = m_dataToY[data];
        if (hProp)
        {
            m_yToData.remove(hProp);
            delete hProp;
        }
        m_dataToY.remove(data);

        m_values.remove(data);
    }

    void APointDataManager::slotIntChanged(AData* data, int val)
    {
        if (AData* prop = m_xToData.value(data, 0))
        {
            QPoint s = m_values[prop].val;
            s.setX(val);
            setValue(prop, s);
        }
        else if (AData* prop = m_yToData.value(data, 0))
        {
            QPoint s = m_values[prop].val;
            s.setY(val);
            setValue(prop, s);
        }
    }

    void APointDataManager::slotDataDestroyed(AData* data)
    {
        if (AData* pointProp = m_xToData.value(data, 0))
        {
            m_dataToX[pointProp] = 0;
            m_xToData.remove(data);
        }
        else if (AData* pointProp = m_yToData.value(data, 0))
        {
            m_dataToY[pointProp] = 0;
            m_yToData.remove(data);
        }
    }

    void APointDataManager::_setValue(AData* data, const QPoint& val)
    {
        m_intDataManager->setValue(m_dataToX.value(data), val.x());
        m_intDataManager->setValue(m_dataToY.value(data), val.y());
    }

    void APointDataManager::_setRange(AData* data, const QPoint& minVal, const QPoint& maxVal, const QPoint& val)
    {
        AData* wData = m_dataToX.value(data);
        AData* hData = m_dataToY.value(data);
        m_intDataManager->setRange(wData, minVal.x(), maxVal.x());
        m_intDataManager->setValue(wData, val.x());
        m_intDataManager->setRange(hData, minVal.y(), maxVal.y());
        m_intDataManager->setValue(hData, val.y());
    }

    // ----------------------------------------------------------------------------------------------------

    AEnumDataManager::AEnumDataManager(QObject* parent)
        : AAbstractDataManager(parent)
    {
    }

    AEnumDataManager::~AEnumDataManager()
    {
        clear();
    }

    int AEnumDataManager::value(const AData* data) const
    {
        return getValue<int>(m_values, data, -1);
    }

    QStringList AEnumDataManager::enumNames(const AData* data) const
    {
        return getData<QStringList>(m_values, &Data::enumNames, data, QStringList());
    }

    QMap<int, QIcon> AEnumDataManager::enumIcons(const AData* data) const
    {
        return getData<QMap<int, QIcon> >(m_values, &Data::enumIcons, data, QMap<int, QIcon>());
    }

    QString AEnumDataManager::valueText(const AData* data) const
    {
        const auto it = m_values.constFind(data);
        if (it == m_values.constEnd())
            return QString();

        const AEnumDataManager::Data& dataVal = it.value();

        const int v = dataVal.val;
        if (v >= 0 && v < dataVal.enumNames.count())
            return dataVal.enumNames.at(v);
        return QString();
    }

    QIcon AEnumDataManager::valueIcon(const AData* data) const
    {
        const auto it = m_values.constFind(data);
        if (it == m_values.constEnd())
            return QIcon();

        const AEnumDataManager::Data& dataVal = it.value();

        const int v = dataVal.val;
        return dataVal.enumIcons.value(v);
    }

    void AEnumDataManager::setValue(AData* data, int val)
    {
        const auto it = m_values.find(data);
        if (it == m_values.end())
            return;

        AEnumDataManager::Data dataVal = it.value();

        if (val >= dataVal.enumNames.count())
            return;

        if (val < 0 && dataVal.enumNames.count() > 0)
            return;

        if (val < 0)
            val = -1;

        if (dataVal.val == val)
            return;

        dataVal.val = val;

        it.value() = dataVal;

        emit dataChanged(data);
        emit valueChanged(data, dataVal.val);
    }

    void AEnumDataManager::setEnumNames(AData* data, const QStringList& enumNames)
    {
        const auto it = m_values.find(data);
        if (it == m_values.end())
            return;

        AEnumDataManager::Data dataVal = it.value();

        if (dataVal.enumNames == enumNames)
            return;

        dataVal.enumNames = enumNames;
        dataVal.val = -1;

        if (enumNames.count() > 0)
            dataVal.val = 0;

        it.value() = dataVal;

        emit enumNamesChanged(data, dataVal.enumNames);
        emit dataChanged(data);
        emit valueChanged(data, dataVal.val);
    }

    void AEnumDataManager::setEnumIcons(AData* data, const QMap<int, QIcon>& enumIcons)
    {
        const auto it = m_values.find(data);
        if (it == m_values.end())
            return;

        it.value().enumIcons = enumIcons;

        emit enumIconsChanged(data, it.value().enumIcons);
        emit dataChanged(data);
    }

    void AEnumDataManager::initializeData(AData* data)
    {
        m_values[data] = AEnumDataManager::Data();
    }

    void AEnumDataManager::uninitializeData(AData* data)
    {
        m_values.remove(data);
    }

    // ----------------------------------------------------------------------------------------------------

    void AColorDataManager::slotIntChanged(AData* data, int value)
    {
        if (AData* prop = m_rToData.value(data, 0))
        {
            QColor c = m_values[prop];
            c.setRed(value);
            setValue(prop, c);
        }
        else if (AData* prop = m_gToData.value(data, 0))
        {
            QColor c = m_values[prop];
            c.setGreen(value);
            setValue(prop, c);
        }
        else if (AData* prop = m_bToData.value(data, 0))
        {
            QColor c = m_values[prop];
            c.setBlue(value);
            setValue(prop, c);
        }
        else if (AData* prop = m_aToData.value(data, 0))
        {
            QColor c = m_values[prop];
            c.setAlpha(value);
            setValue(prop, c);
        }
    }

    void AColorDataManager::slotDataDestroyed(AData* data)
    {
        if (AData* pointProp = m_rToData.value(data, 0))
        {
            m_dataToR[pointProp] = 0;
            m_rToData.remove(data);
        }
        else if (AData* pointProp = m_gToData.value(data, 0))
        {
            m_dataToG[pointProp] = 0;
            m_gToData.remove(data);
        }
        else if (AData* pointProp = m_bToData.value(data, 0))
        {
            m_dataToB[pointProp] = 0;
            m_bToData.remove(data);
        }
        else if (AData* pointProp = m_aToData.value(data, 0))
        {
            m_dataToA[pointProp] = 0;
            m_aToData.remove(data);
        }
    }

    AColorDataManager::AColorDataManager(QObject* parent)
        : AAbstractDataManager(parent)
    {
        m_intDataManager = new AIntDataManager(this);
        connect(m_intDataManager, SIGNAL(valueChanged(AData*, int)), this, SLOT(slotIntChanged(AData*, int)));
        connect(m_intDataManager, SIGNAL(dataDestroyed(AData*)), this, SLOT(slotDataDestroyed(AData*)));
    }

    AColorDataManager::~AColorDataManager()
    {
        clear();
    }

    AIntDataManager* AColorDataManager::subIntDataManager() const
    {
        return m_intDataManager;
    }

    QColor AColorDataManager::value(const AData* data) const
    {
        return m_values.value(data, QColor());
    }

    QString AColorDataManager::valueText(const AData* data) const
    {
        const AColorDataManager::DataValueMap::const_iterator it = m_values.constFind(data);
        if (it == m_values.constEnd())
            return QString();

        return ADPWUtils::ColorText(it.value());
    }

    QIcon AColorDataManager::valueIcon(const AData* data) const
    {
        const AColorDataManager::DataValueMap::const_iterator it = m_values.constFind(data);
        if (it == m_values.constEnd())
            return QIcon();

        return ADPWUtils::DrawBrushToPixmap(QBrush(it.value()));
    }

    void AColorDataManager::setValue(AData* data, const QColor& val)
    {
        const AColorDataManager::DataValueMap::iterator it = m_values.find(data);
        if (it == m_values.end())
            return;

        if (it.value() == val)
            return;

        it.value() = val;

        m_intDataManager->setValue(m_dataToR[data], val.red());
        m_intDataManager->setValue(m_dataToG[data], val.green());
        m_intDataManager->setValue(m_dataToB[data], val.blue());
        m_intDataManager->setValue(m_dataToA[data], val.alpha());

        emit dataChanged(data);
        emit valueChanged(data, val);
    }

    void AColorDataManager::initializeData(AData* data)
    {
        QColor val;
        m_values[data] = val;

        AData* rProp = m_intDataManager->addData();
        rProp->setDataName(tr("Red"));
        m_intDataManager->setValue(rProp, val.red());
        m_intDataManager->setRange(rProp, 0, 0xFF);
        m_dataToR[data] = rProp;
        m_rToData[rProp] = data;
        data->addSubData(rProp);

        AData* gProp = m_intDataManager->addData();
        gProp->setDataName(tr("Green"));
        m_intDataManager->setValue(gProp, val.green());
        m_intDataManager->setRange(gProp, 0, 0xFF);
        m_dataToG[data] = gProp;
        m_gToData[gProp] = data;
        data->addSubData(gProp);

        AData* bProp = m_intDataManager->addData();
        bProp->setDataName(tr("Blue"));
        m_intDataManager->setValue(bProp, val.blue());
        m_intDataManager->setRange(bProp, 0, 0xFF);
        m_dataToB[data] = bProp;
        m_bToData[bProp] = data;
        data->addSubData(bProp);

        AData* aProp = m_intDataManager->addData();
        aProp->setDataName(tr("Alpha"));
        m_intDataManager->setValue(aProp, val.alpha());
        m_intDataManager->setRange(aProp, 0, 0xFF);
        m_dataToA[data] = aProp;
        m_aToData[aProp] = data;
        data->addSubData(aProp);
    }

    void AColorDataManager::uninitializeData(AData* data)
    {
        AData* rProp = m_dataToR[data];
        if (rProp)
        {
            m_rToData.remove(rProp);
            delete rProp;
        }
        m_dataToR.remove(data);

        AData* gProp = m_dataToG[data];
        if (gProp)
        {
            m_gToData.remove(gProp);
            delete gProp;
        }
        m_dataToG.remove(data);

        AData* bProp = m_dataToB[data];
        if (bProp)
        {
            m_bToData.remove(bProp);
            delete bProp;
        }
        m_dataToB.remove(data);

        AData* aProp = m_dataToA[data];
        if (aProp)
        {
            m_aToData.remove(aProp);
            delete aProp;
        }
        m_dataToA.remove(data);

        m_values.remove(data);
    }

    // ----------------------------------------------------------------------------------------------------

    AFontDataManager::AFontDataManager(QObject* parent)
        : AAbstractDataManager(parent)
        , m_settingValue(false)
        , m_fontDatabaseChangeTimer(0)
    {
        QObject::connect(qApp, SIGNAL(fontDatabaseChanged()), this, SLOT(slotFontDatabaseChanged()));

        m_intDataManager = new AIntDataManager(this);
        connect(m_intDataManager, SIGNAL(valueChanged(AData*, int)), this, SLOT(slotIntChanged(AData*, int)));
        m_enumDataManager = new AEnumDataManager(this);
        connect(m_enumDataManager, SIGNAL(valueChanged(AData*, int)), this, SLOT(slotEnumChanged(AData*, int)));
        m_boolDataManager = new ABoolDataManager(this);
        connect(m_boolDataManager, SIGNAL(valueChanged(AData*, bool)), this, SLOT(slotBoolChanged(AData*, bool)));

        connect(m_intDataManager, SIGNAL(dataDestroyed(AData*)), this, SLOT(slotDataDestroyed(AData*)));
        connect(m_enumDataManager, SIGNAL(dataDestroyed(AData*)), this, SLOT(slotDataDestroyed(AData*)));
        connect(m_boolDataManager, SIGNAL(dataDestroyed(AData*)), this, SLOT(slotDataDestroyed(AData*)));
    }

    void AFontDataManager::slotIntChanged(AData* data, int value)
    {
        if (m_settingValue)
            return;
        if (AData* prop = m_pointSizeToData.value(data, 0))
        {
            QFont f = m_values[prop];
            f.setPointSize(value);
            setValue(prop, f);
        }
    }

    void AFontDataManager::slotEnumChanged(AData* data, int value)
    {
        if (m_settingValue)
            return;
        if (AData* prop = m_familyToData.value(data, 0))
        {
            QFont f = m_values[prop];
            f.setFamily(m_familyNames.at(value));
            setValue(prop, f);
        }
    }

    void AFontDataManager::slotBoolChanged(AData* data, bool value)
    {
        if (m_settingValue)
            return;
        if (AData* prop = m_boldToData.value(data, 0))
        {
            QFont f = m_values[prop];
            f.setBold(value);
            setValue(prop, f);
        }
        else if (AData* prop = m_italicToData.value(data, 0))
        {
            QFont f = m_values[prop];
            f.setItalic(value);
            setValue(prop, f);
        }
        else if (AData* prop = m_underlineToData.value(data, 0))
        {
            QFont f = m_values[prop];
            f.setUnderline(value);
            setValue(prop, f);
        }
        else if (AData* prop = m_strikeOutToData.value(data, 0))
        {
            QFont f = m_values[prop];
            f.setStrikeOut(value);
            setValue(prop, f);
        }
        else if (AData* prop = m_kerningToData.value(data, 0))
        {
            QFont f = m_values[prop];
            f.setKerning(value);
            setValue(prop, f);
        }
    }

    void AFontDataManager::slotDataDestroyed(AData* data)
    {
        if (AData* pointProp = m_pointSizeToData.value(data, 0))
        {
            m_dataToPointSize[pointProp] = 0;
            m_pointSizeToData.remove(data);
        }
        else if (AData* pointProp = m_familyToData.value(data, 0))
        {
            m_dataToFamily[pointProp] = 0;
            m_familyToData.remove(data);
        }
        else if (AData* pointProp = m_boldToData.value(data, 0))
        {
            m_dataToBold[pointProp] = 0;
            m_boldToData.remove(data);
        }
        else if (AData* pointProp = m_italicToData.value(data, 0))
        {
            m_dataToItalic[pointProp] = 0;
            m_italicToData.remove(data);
        }
        else if (AData* pointProp = m_underlineToData.value(data, 0))
        {
            m_dataToUnderline[pointProp] = 0;
            m_underlineToData.remove(data);
        }
        else if (AData* pointProp = m_strikeOutToData.value(data, 0))
        {
            m_dataToStrikeOut[pointProp] = 0;
            m_strikeOutToData.remove(data);
        }
        else if (AData* pointProp = m_kerningToData.value(data, 0))
        {
            m_dataToKerning[pointProp] = 0;
            m_kerningToData.remove(data);
        }
    }

    void  AFontDataManager::slotFontDatabaseChanged()
    {
        if (!m_fontDatabaseChangeTimer)
        {
            m_fontDatabaseChangeTimer = new QTimer(this);
            m_fontDatabaseChangeTimer->setInterval(0);
            m_fontDatabaseChangeTimer->setSingleShot(true);
            QObject::connect(m_fontDatabaseChangeTimer, SIGNAL(timeout()), this, SLOT(slotFontDatabaseDelayedChange()));
        }
        if (!m_fontDatabaseChangeTimer->isActive())
            m_fontDatabaseChangeTimer->start();
    }

    void AFontDataManager::slotFontDatabaseDelayedChange()
    {
        typedef QMap<const AData*, AData*> DataDataMap;

        // rescan available font names
        const QStringList oldFamilies = m_familyNames;
        m_familyNames = fontDatabase()->families();

        // Adapt all existing properties
        if (!m_dataToFamily.empty())
        {
            DataDataMap::const_iterator cend = m_dataToFamily.constEnd();
            for (DataDataMap::const_iterator it = m_dataToFamily.constBegin(); it != cend; ++it)
            {
                AData* familyProp = it.value();
                const int oldIdx = m_enumDataManager->value(familyProp);
                int newIdx = m_familyNames.indexOf(oldFamilies.at(oldIdx));
                if (newIdx < 0)
                    newIdx = 0;
                m_enumDataManager->setEnumNames(familyProp, m_familyNames);
                m_enumDataManager->setValue(familyProp, newIdx);
            }
        }
    }

    AFontDataManager::~AFontDataManager()
    {
        clear();
    }

    AIntDataManager* AFontDataManager::subIntDataManager() const
    {
        return m_intDataManager;
    }

    AEnumDataManager* AFontDataManager::subEnumDataManager() const
    {
        return m_enumDataManager;
    }

    ABoolDataManager* AFontDataManager::subBoolDataManager() const
    {
        return m_boolDataManager;
    }

    QFont AFontDataManager::value(const AData* data) const
    {
        return m_values.value(data, QFont());
    }

    QString AFontDataManager::valueText(const AData* data) const
    {
        const AFontDataManager::DataValueMap::const_iterator it = m_values.constFind(data);
        if (it == m_values.constEnd())
            return QString();

        return ADPWUtils::FontText(it.value());
    }

    QIcon AFontDataManager::valueIcon(const AData* data) const
    {
        const AFontDataManager::DataValueMap::const_iterator it = m_values.constFind(data);
        if (it == m_values.constEnd())
            return QIcon();

        return ADPWUtils::DrawFontToPixmap(it.value());
    }

    void AFontDataManager::setValue(AData* data, const QFont& val)
    {
        const AFontDataManager::DataValueMap::iterator it = m_values.find(data);
        if (it == m_values.end())
            return;

        const QFont oldVal = it.value();
        if (oldVal == val && oldVal.resolve() == val.resolve())
            return;

        it.value() = val;

        int idx = m_familyNames.indexOf(val.family());
        if (idx == -1)
            idx = 0;
        bool settingValue = m_settingValue;
        m_settingValue = true;
        m_enumDataManager->setValue(m_dataToFamily[data], idx);
        m_intDataManager->setValue(m_dataToPointSize[data], val.pointSize());
        m_boolDataManager->setValue(m_dataToBold[data], val.bold());
        m_boolDataManager->setValue(m_dataToItalic[data], val.italic());
        m_boolDataManager->setValue(m_dataToUnderline[data], val.underline());
        m_boolDataManager->setValue(m_dataToStrikeOut[data], val.strikeOut());
        m_boolDataManager->setValue(m_dataToKerning[data], val.kerning());
        m_settingValue = settingValue;

        emit dataChanged(data);
        emit valueChanged(data, val);
    }

    void AFontDataManager::initializeData(AData* data)
    {
        QFont val;
        m_values[data] = val;

        AData* familyProp = m_enumDataManager->addData();
        familyProp->setDataName(tr("Family"));
        if (m_familyNames.empty())
            m_familyNames = fontDatabase()->families();
        m_enumDataManager->setEnumNames(familyProp, m_familyNames);
        int idx = m_familyNames.indexOf(val.family());
        if (idx == -1)
            idx = 0;
        m_enumDataManager->setValue(familyProp, idx);
        m_dataToFamily[data] = familyProp;
        m_familyToData[familyProp] = data;
        data->addSubData(familyProp);

        AData* pointSizeProp = m_intDataManager->addData();
        pointSizeProp->setDataName(tr("Point Size"));
        m_intDataManager->setValue(pointSizeProp, val.pointSize());
        m_intDataManager->setMinimum(pointSizeProp, 1);
        m_dataToPointSize[data] = pointSizeProp;
        m_pointSizeToData[pointSizeProp] = data;
        data->addSubData(pointSizeProp);

        AData* boldProp = m_boolDataManager->addData();
        boldProp->setDataName(tr("Bold"));
        m_boolDataManager->setValue(boldProp, val.bold());
        m_dataToBold[data] = boldProp;
        m_boldToData[boldProp] = data;
        data->addSubData(boldProp);

        AData* italicProp = m_boolDataManager->addData();
        italicProp->setDataName(tr("Italic"));
        m_boolDataManager->setValue(italicProp, val.italic());
        m_dataToItalic[data] = italicProp;
        m_italicToData[italicProp] = data;
        data->addSubData(italicProp);

        AData* underlineProp = m_boolDataManager->addData();
        underlineProp->setDataName(tr("Underline"));
        m_boolDataManager->setValue(underlineProp, val.underline());
        m_dataToUnderline[data] = underlineProp;
        m_underlineToData[underlineProp] = data;
        data->addSubData(underlineProp);

        AData* strikeOutProp = m_boolDataManager->addData();
        strikeOutProp->setDataName(tr("Strikeout"));
        m_boolDataManager->setValue(strikeOutProp, val.strikeOut());
        m_dataToStrikeOut[data] = strikeOutProp;
        m_strikeOutToData[strikeOutProp] = data;
        data->addSubData(strikeOutProp);

        AData* kerningProp = m_boolDataManager->addData();
        kerningProp->setDataName(tr("Kerning"));
        m_boolDataManager->setValue(kerningProp, val.kerning());
        m_dataToKerning[data] = kerningProp;
        m_kerningToData[kerningProp] = data;
        data->addSubData(kerningProp);
    }

    void AFontDataManager::uninitializeData(AData* data)
    {
        AData* familyProp = m_dataToFamily[data];
        if (familyProp)
        {
            m_familyToData.remove(familyProp);
            delete familyProp;
        }
        m_dataToFamily.remove(data);

        AData* pointSizeProp = m_dataToPointSize[data];
        if (pointSizeProp)
        {
            m_pointSizeToData.remove(pointSizeProp);
            delete pointSizeProp;
        }
        m_dataToPointSize.remove(data);

        AData* boldProp = m_dataToBold[data];
        if (boldProp)
        {
            m_boldToData.remove(boldProp);
            delete boldProp;
        }
        m_dataToBold.remove(data);

        AData* italicProp = m_dataToItalic[data];
        if (italicProp)
        {
            m_italicToData.remove(italicProp);
            delete italicProp;
        }
        m_dataToItalic.remove(data);

        AData* underlineProp = m_dataToUnderline[data];
        if (underlineProp)
        {
            m_underlineToData.remove(underlineProp);
            delete underlineProp;
        }
        m_dataToUnderline.remove(data);

        AData* strikeOutProp = m_dataToStrikeOut[data];
        if (strikeOutProp)
        {
            m_strikeOutToData.remove(strikeOutProp);
            delete strikeOutProp;
        }
        m_dataToStrikeOut.remove(data);

        AData* kerningProp = m_dataToKerning[data];
        if (kerningProp)
        {
            m_kerningToData.remove(kerningProp);
            delete kerningProp;
        }
        m_dataToKerning.remove(data);

        m_values.remove(data);
    }

    // ----------------------------------------------------------------------------------------------------
}