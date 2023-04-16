/****************************************************************************
 * @file    ADataManager.h
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
#include "AAbstractDataManager.h"

APROCH_NAMESPACE_BEGIN

namespace
{
    template <class Value, class Data>
    static Value getData(const QMap<const AData*, Data>& dataMap,
                         Value Data::* privateData,
                         const AData* data, const Value& defaultValue = Value())
    {
        const auto it = dataMap.constFind(data);
        if (it == dataMap.constEnd())
            return defaultValue;
        return it.value().*privateData;
    }

    template <class Value, class Data>
    static Value getValue(const QMap<const AData*, Data>& dataMap,
                          const AData* data, const Value& defaultValue = Value())
    {
        return getData<Value>(dataMap, &Data::val, data, defaultValue);
    }

    template <class Value, class Data>
    static Value getMinimum(const QMap<const AData*, Data>& dataMap,
                            const AData* data, const Value& defaultValue = Value())
    {
        return getData<Value>(dataMap, &Data::minVal, data, defaultValue);
    }

    template <class Value, class Data>
    static Value getMaximum(const QMap<const AData*, Data>& dataMap,
                            const AData* data, const Value& defaultValue = Value())
    {
        return getData<Value>(dataMap, &Data::maxVal, data, defaultValue);
    }

    template <class ValueChangeParameter, class Value, class DataManager>
    static void setSimpleValue(QMap<const AData*, Value>& dataMap, DataManager* manager,
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

    template <class Data, class Value>
    static void setSimpleMinimumData(Data* data, const Value& minVal)
    {
        data->minVal = minVal;
        if (data->maxVal < data->minVal)
            data->maxVal = data->minVal;

        if (data->val < data->minVal)
            data->val = data->minVal;
    }

    template <class Data, class Value>
    static void setSimpleMaximumData(Data* data, const Value& maxVal)
    {
        data->maxVal = maxVal;
        if (data->minVal > data->maxVal)
            data->minVal = data->maxVal;

        if (data->val > data->maxVal)
            data->val = data->maxVal;
    }

    template <class Data, class Value>
    static void setSizeMinimumData(Data* data, const Value& newMinVal)
    {
        data->minVal = newMinVal;
        if (data->maxVal.width() < data->minVal.width())
            data->maxVal.setWidth(data->minVal.width());
        if (data->maxVal.height() < data->minVal.height())
            data->maxVal.setHeight(data->minVal.height());

        if (data->val.width() < data->minVal.width())
            data->val.setWidth(data->minVal.width());
        if (data->val.height() < data->minVal.height())
            data->val.setHeight(data->minVal.height());
    }

    template <class Data, class Value>
    static void setSizeMaximumData(Data* data, const Value& newMaxVal)
    {
        data->maxVal = newMaxVal;
        if (data->minVal.width() > data->maxVal.width())
            data->minVal.setWidth(data->maxVal.width());
        if (data->minVal.height() > data->maxVal.height())
            data->minVal.setHeight(data->maxVal.height());

        if (data->val.width() > data->maxVal.width())
            data->val.setWidth(data->maxVal.width());
        if (data->val.height() > data->maxVal.height())
            data->val.setHeight(data->maxVal.height());
    }

    template <class Data, class Value>
    static void setV2DMinimumData(Data* data, const Value& newMinVal)
    {
        data->minVal = newMinVal;
        if (data->maxVal.x() < data->minVal.x())
            data->maxVal.setX(data->minVal.x());
        if (data->maxVal.y() < data->minVal.y())
            data->maxVal.setY(data->minVal.y());

        if (data->val.x() < data->minVal.x())
            data->val.setY(data->minVal.x());
        if (data->val.y() < data->minVal.y())
            data->val.setY(data->minVal.y());
    }

    template <class Data, class Value>
    static void setV2DMaximumData(Data* data, const Value& newMaxVal)
    {
        data->maxVal = newMaxVal;
        if (data->minVal.x() > data->maxVal.x())
            data->minVal.setX(data->maxVal.x());
        if (data->minVal.y() > data->maxVal.y())
            data->minVal.setY(data->maxVal.y());

        if (data->val.x() > data->maxVal.x())
            data->val.setX(data->maxVal.x());
        if (data->val.y() > data->maxVal.y())
            data->val.setY(data->maxVal.y());
    }

    template <class ValueChangeParameter, class DataManager, class Value, class Data>
    static void setMinimumValue(DataManager* manager,
                                void (DataManager::* dataChangedSignal)(AData*),
                                void (DataManager::* valueChangedSignal)(AData*, ValueChangeParameter),
                                void (DataManager::* rangeChangedSignal)(AData*, ValueChangeParameter, ValueChangeParameter),
                                AData* data, const Value& minVal)
    {
        void (DataManager:: * setSubDataRange)(AData*, ValueChangeParameter, ValueChangeParameter, ValueChangeParameter) = 0;
        setBorderValue<ValueChangeParameter, DataManager, Value, Data>(manager, dataChangedSignal, valueChangedSignal, rangeChangedSignal,
                                                                       data, &DataManager::Data::minimumValue,
                                                                       &DataManager::Data::setMinimumValue, minVal, setSubDataRange);
    }

    template <class ValueChangeParameter, class DataManager, class Value, class Data>
    static void setMaximumValue(DataManager* manager,
                                void (DataManager::* dataChangedSignal)(AData*),
                                void (DataManager::* valueChangedSignal)(AData*, ValueChangeParameter),
                                void (DataManager::* rangeChangedSignal)(AData*, ValueChangeParameter, ValueChangeParameter),
                                AData* data, const Value& maxVal)
    {
        void (DataManager:: * setSubDataRange)(AData*, ValueChangeParameter, ValueChangeParameter, ValueChangeParameter) = 0;
        setBorderValue<ValueChangeParameter, DataManager, Value, Data>(manager, dataChangedSignal, valueChangedSignal, rangeChangedSignal,
                                                                       data, &DataManager::Data::maximumValue,
                                                                       &DataManager::Data::setMaximumValue, maxVal, setSubDataRange);
    }

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

    template <class Value>
    static void orderV2DBorders(Value& minVal, Value& maxVal)
    {
        Value fromSize = minVal;
        Value toSize = maxVal;
        if (fromSize.x() > toSize.x())
        {
            fromSize.setX(maxVal.x());
            toSize.setX(minVal.x());
        }
        if (fromSize.y() > toSize.y())
        {
            fromSize.setY(maxVal.y());
            toSize.setY(minVal.y());
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

    void orderBorders(QPoint& minVal, QPoint& maxVal)
    {
        orderV2DBorders(minVal, maxVal);
    }

    void orderBorders(QPointF& minVal, QPointF& maxVal)
    {
        orderV2DBorders(minVal, maxVal);
    }

    template <class ValueChangeParameter, class DataManager, class Value>
    static void setValueInRange(DataManager* manager,
                                void (DataManager::* dataChangedSignal)(AData*),
                                void (DataManager::* valueChangedSignal)(AData*, ValueChangeParameter),
                                AData* data, const Value& val,
                                void (DataManager::* setSubDataValue)(AData*, ValueChangeParameter))
    {
        const auto it = manager->values().find(data);
        if (it == manager->values().end())
            return;

        auto& dataVal = it.value();

        if (dataVal.val == val)
            return;

        const Value oldVal = dataVal.val;

        dataVal.val = qBound(dataVal.minVal, val, dataVal.maxVal);

        if (dataVal.val == oldVal)
            return;

        if (setSubDataValue)
            (manager->*setSubDataValue)(data, dataVal.val);

        emit(manager->*dataChangedSignal)(data);
        emit(manager->*valueChangedSignal)(data, dataVal.val);
    }

    template <class ValueChangeParameter, class DataManager, class Value>
    static void setBorderValues(DataManager* manager,
                                void (DataManager::* dataChangedSignal)(AData*),
                                void (DataManager::* valueChangedSignal)(AData*, ValueChangeParameter),
                                void (DataManager::* rangeChangedSignal)(AData*, ValueChangeParameter, ValueChangeParameter),
                                AData* data, const Value& minVal, const Value& maxVal,
                                void (DataManager::* setSubDataRange)(AData*,
                                ValueChangeParameter, ValueChangeParameter, ValueChangeParameter))
    {
        const auto it = manager->values().find(data);
        if (it == manager->values().end())
            return;

        Value fromVal = minVal;
        Value toVal = maxVal;
        orderBorders(fromVal, toVal);

        auto& dataVal = it.value();

        if (dataVal.minVal == fromVal && dataVal.maxVal == toVal)
            return;

        const Value oldVal = dataVal.val;

        dataVal.setMinimumValue(fromVal);
        dataVal.setMaximumValue(toVal);

        emit(manager->*rangeChangedSignal)(data, dataVal.minVal, dataVal.maxVal);

        if (setSubDataRange)
            (manager->*setSubDataRange)(data, dataVal.minVal, dataVal.maxVal, dataVal.val);

        if (dataVal.val == oldVal)
            return;

        emit(manager->*dataChangedSignal)(data);
        emit(manager->*valueChangedSignal)(data, dataVal.val);
    }

    template <class ValueChangeParameter, class DataManager, class Value, class Data>
    static void setBorderValue(DataManager* manager,
                               void (DataManager::* dataChangedSignal)(AData*),
                               void (DataManager::* valueChangedSignal)(AData*, ValueChangeParameter),
                               void (DataManager::* rangeChangedSignal)(AData*, ValueChangeParameter, ValueChangeParameter),
                               AData* data,
                               Value(Data::* getRangeVal)() const,
                               void (Data::* setRangeVal)(ValueChangeParameter), const Value& borderVal,
                               void (DataManager::* setSubDataRange)(AData*, ValueChangeParameter, ValueChangeParameter, ValueChangeParameter))
    {
        const auto it = manager->values().find(data);
        if (it == manager->values().end())
            return;

        Data& dataVal = it.value();

        if ((dataVal.*getRangeVal)() == borderVal)
            return;

        const Value oldVal = dataVal.val;

        (dataVal.*setRangeVal)(borderVal);

        emit(manager->*rangeChangedSignal)(data, dataVal.minVal, dataVal.maxVal);

        if (setSubDataRange)
            (manager->*setSubDataRange)(data, dataVal.minVal, dataVal.maxVal, dataVal.val);

        if (dataVal.val == oldVal)
            return;

        emit(manager->*dataChangedSignal)(data);
        emit(manager->*valueChangedSignal)(data, dataVal.val);
    }
}

/**
 * @brief 整数型数据管理器
 */
class APROCH_API AIntDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    AIntDataManager(QObject* parent = nullptr);
    ~AIntDataManager();

    struct Data
    {
        int val{ 0 };
        int minVal{ -INT_MAX };
        int maxVal{ INT_MAX };
        int singleStep{ 1 };
        int minimumValue() const noexcept
        {
            return minVal;
        }
        int maximumValue() const noexcept
        {
            return maxVal;
        }
        void setMinimumValue(int newMinVal)
        {
            setSimpleMinimumData(this, newMinVal);
        }
        void setMaximumValue(int newMaxVal)
        {
            setSimpleMaximumData(this, newMaxVal);
        }
    };
    typedef QMap<const AData*, Data> DataValueMap;

    inline DataValueMap& values() noexcept
    {
        return m_values;
    }

    int value(const AData* data) const;
    int minimum(const AData* data) const;
    int maximum(const AData* data) const;
    int singleStep(const AData* data) const;

public Q_SLOTS:
    void setValue(AData* data, int val);
    void setMinimum(AData* data, int minVal);
    void setMaximum(AData* data, int maxVal);
    void setRange(AData* data, int minVal, int maxVal);
    void setSingleStep(AData* data, int step);

Q_SIGNALS:
    void valueChanged(AData* data, int val);
    void rangeChanged(AData* data, int minVal, int maxVal);
    void singleStepChanged(AData* data, int step);

protected:
    virtual QString toText(const AData* data) const override;
    virtual void initializeData(AData* data) override;
    virtual void uninitializeData(AData* data) override;

private:
    DataValueMap m_values;

private:
    Q_DISABLE_COPY_MOVE(AIntDataManager)
};

/**
 * @brief 双精度浮点型数据管理器
 */
class APROCH_API ADoubleDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    ADoubleDataManager(QObject* parent = nullptr);
    ~ADoubleDataManager();

    struct Data
    {
        double val{ 0 };
        double minVal{ -DBL_MAX };
        double maxVal{ DBL_MAX };
        double singleStep{ 1 };
        int decimals{ 2 };
        double minimumValue() const noexcept
        {
            return minVal;
        }
        double maximumValue() const noexcept
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

    inline DataValueMap& values() noexcept
    {
        return m_values;
    }

    double value(const AData* data) const;
    double minimum(const AData* data) const;
    double maximum(const AData* data) const;
    double singleStep(const AData* data) const;
    int decimals(const AData* data) const;

public Q_SLOTS:
    void setValue(AData* data, double val);
    void setMinimum(AData* data, double minVal);
    void setMaximum(AData* data, double maxVal);
    void setRange(AData* data, double minVal, double maxVal);
    void setSingleStep(AData* data, double step);
    void setDecimals(AData* data, int prec);

Q_SIGNALS:
    void valueChanged(AData* data, double val);
    void rangeChanged(AData* data, double minVal, double maxVal);
    void singleStepChanged(AData* data, double step);
    void decimalsChanged(AData* data, int prec);

protected:
    virtual QString toText(const AData* data) const override;
    virtual void initializeData(AData* data) override;
    virtual void uninitializeData(AData* data) override;

private:
    DataValueMap m_values;

private:
    Q_DISABLE_COPY_MOVE(ADoubleDataManager)
};

/**
 * @brief 字符串数据管理器
 */
class APROCH_API AStringDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    explicit AStringDataManager(QObject* parent = nullptr);
    ~AStringDataManager();

    struct Data
    {
        Data() : regExp(QString(QLatin1Char('*')), Qt::CaseSensitive, QRegExp::Wildcard)
        {
        }
        QString val;
        QRegExp regExp;
    };
    typedef QMap<const AData*, Data> DataValueMap;

    inline DataValueMap& values() noexcept
    {
        return m_values;
    }

    QString value(const AData* data) const;
    QRegExp regExp(const AData* data) const;

public Q_SLOTS:
    void setValue(AData* data, const QString& val);
    void setRegExp(AData* data, const QRegExp& regExp);

Q_SIGNALS:
    void valueChanged(AData* data, const QString& val);
    void regExpChanged(AData* data, const QRegExp& regExp);

protected:
    virtual QString toText(const AData* data) const override;
    virtual void initializeData(AData* data) override;
    virtual void uninitializeData(AData* data) override;

private:
    QMap<const AData*, Data> m_values;

private:
    Q_DISABLE_COPY_MOVE(AStringDataManager)
};

/**
 * @brief 布尔数据管理器
 */
class APROCH_API ABoolDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    ABoolDataManager(QObject* parent = nullptr);
    ~ABoolDataManager();

    inline QMap<const AData*, bool>& values() noexcept
    {
        return m_values;
    }

    bool value(const AData* data) const;

public Q_SLOTS:
    void setValue(AData* data, bool val);

Q_SIGNALS:
    void valueChanged(AData* data, bool val);

protected:
    virtual QString toText(const AData* data) const override;
    virtual void initializeData(AData* data) override;
    virtual void uninitializeData(AData* data) override;

private:
    QMap<const AData*, bool> m_values;
    const QIcon m_checkedIcon;
    const QIcon m_uncheckedIcon;

private:
    Q_DISABLE_COPY_MOVE(ABoolDataManager)
};

/**
 * @brief 尺寸数据管理器
 */
class APROCH_API ASizeDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    ASizeDataManager(QObject* parent = nullptr);
    ~ASizeDataManager();
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

    inline DataValueMap& values() noexcept
    {
        return m_values;
    }

    AIntDataManager* subIntDataManager() const;

    QSize value(const AData* data) const;
    QSize minimum(const AData* data) const;
    QSize maximum(const AData* data) const;

public Q_SLOTS:
    void setValue(AData* data, const QSize& val);
    void setMinimum(AData* data, const QSize& minVal);
    void setMaximum(AData* data, const QSize& maxVal);
    void setRange(AData* data, const QSize& minVal, const QSize& maxVal);

Q_SIGNALS:
    void valueChanged(AData* data, const QSize& val);
    void rangeChanged(AData* data, const QSize& minVal, const QSize& maxVal);

protected:
    QString toText(const AData* data) const override;
    virtual void initializeData(AData* data) override;
    virtual void uninitializeData(AData* data) override;

private Q_SLOTS:
    void slotIntChanged(AData*, int);
    void slotDataDestroyed(AData*);

private:
    void _setValue(AData* data, const QSize& val);
    void _setRange(AData* data, const QSize& minVal, const QSize& maxVal, const QSize& val);

private:
    DataValueMap m_values;

    AIntDataManager* m_intDataManager;

    QMap<const AData*, AData*> m_dataToW;
    QMap<const AData*, AData*> m_dataToH;

    QMap<const AData*, AData*> m_wToData;
    QMap<const AData*, AData*> m_hToData;

private:
    Q_DISABLE_COPY_MOVE(ASizeDataManager)
};

/**
 * @brief 二维整数点数据管理器
 */
class APROCH_API APointDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    APointDataManager(QObject* parent = nullptr);
    ~APointDataManager();
    struct Data
    {
        QPoint val{ 0, 0 };
        QPoint minVal{ -INT_MAX, -INT_MAX };
        QPoint maxVal{ INT_MAX, INT_MAX };
        QPoint minimumValue() const
        {
            return minVal;
        }
        QPoint maximumValue() const
        {
            return maxVal;
        }
        void setMinimumValue(const QPoint& newMinVal)
        {
            setV2DMinimumData(this, newMinVal);
        }
        void setMaximumValue(const QPoint& newMaxVal)
        {
            setV2DMaximumData(this, newMaxVal);
        }
    };
    typedef QMap<const AData*, Data> DataValueMap;

    inline DataValueMap& values() noexcept
    {
        return m_values;
    }

    AIntDataManager* subIntDataManager() const;

    QPoint value(const AData* data) const;
    QPoint minimum(const AData* data) const;
    QPoint maximum(const AData* data) const;

public Q_SLOTS:
    void setValue(AData* data, const QPoint& val);
    void setMinimum(AData* data, const QPoint& minVal);
    void setMaximum(AData* data, const QPoint& maxVal);
    void setRange(AData* data, const QPoint& minVal, const QPoint& maxVal);

Q_SIGNALS:
    void valueChanged(AData* data, const QPoint& val);
    void rangeChanged(AData* data, const QPoint& minVal, const QPoint& maxVal);

protected:
    QString toText(const AData* data) const override;
    virtual void initializeData(AData* data) override;
    virtual void uninitializeData(AData* data) override;

private Q_SLOTS:
    void slotIntChanged(AData*, int);
    void slotDataDestroyed(AData*);

private:
    void _setValue(AData* data, const QPoint& val);
    void _setRange(AData* data, const QPoint& minVal, const QPoint& maxVal, const QPoint& val);

private:
    DataValueMap m_values;

    AIntDataManager* m_intDataManager;

    QMap<const AData*, AData*> m_dataToX;
    QMap<const AData*, AData*> m_dataToY;

    QMap<const AData*, AData*> m_xToData;
    QMap<const AData*, AData*> m_yToData;

private:
    Q_DISABLE_COPY_MOVE(APointDataManager)
};

/**
 * @brief 枚举数据管理器
 */
class APROCH_API AEnumDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    AEnumDataManager(QObject* parent = nullptr);
    ~AEnumDataManager();

    struct Data
    {
        int val{ -1 };
        QStringList enumNames;
        QMap<int, QIcon> enumIcons;
    };
    typedef QMap<const AData*, Data> DataValueMap;

    inline DataValueMap& values() noexcept
    {
        return m_values;
    }

    int value(const AData* data) const;
    QStringList enumNames(const AData* data) const;
    QMap<int, QIcon> enumIcons(const AData* data) const;

public Q_SLOTS:
    void setValue(AData* data, int val);
    void setEnumNames(AData* data, const QStringList& names);
    void setEnumIcons(AData* data, const QMap<int, QIcon>& icons);

Q_SIGNALS:
    void valueChanged(AData* data, int val);
    void enumNamesChanged(AData* data, const QStringList& names);
    void enumIconsChanged(AData* data, const QMap<int, QIcon>& icons);

protected:
    QString toText(const AData* data) const override;
    QIcon valueIcon(const AData* data) const;
    virtual void initializeData(AData* data) override;
    virtual void uninitializeData(AData* data) override;

private:
    DataValueMap m_values;

private:
    Q_DISABLE_COPY_MOVE(AEnumDataManager)
};

/**
 * @brief 颜色数据管理器
 */
class APROCH_API AColorDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    AColorDataManager(QObject* parent = 0);
    ~AColorDataManager();

    AIntDataManager* subIntDataManager() const;

    QColor value(const AData* data) const;

public Q_SLOTS:
    void setValue(AData* data, const QColor& val);

Q_SIGNALS:
    void valueChanged(AData* data, const QColor& val);

protected:
    QString toText(const AData* data) const;
    QIcon valueIcon(const AData* data) const;
    virtual void initializeData(AData* data);
    virtual void uninitializeData(AData* data);

private Q_SLOTS:
    void slotIntChanged(AData* data, int value);
    void slotDataDestroyed(AData* data);

private:
    typedef QMap<const AData*, QColor> DataValueMap;
    DataValueMap m_values;

    AIntDataManager* m_intDataManager;

    QMap<const AData*, AData*> m_dataToR;
    QMap<const AData*, AData*> m_dataToG;
    QMap<const AData*, AData*> m_dataToB;
    QMap<const AData*, AData*> m_dataToA;

    QMap<const AData*, AData*> m_rToData;
    QMap<const AData*, AData*> m_gToData;
    QMap<const AData*, AData*> m_bToData;
    QMap<const AData*, AData*> m_aToData;

private:
    Q_DISABLE_COPY_MOVE(AColorDataManager)
};

/**
 * @brief 字体数据管理器
 */
class APROCH_API AFontDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    AFontDataManager(QObject* parent = 0);
    ~AFontDataManager();

    AIntDataManager* subIntDataManager() const;
    AEnumDataManager* subEnumDataManager() const;
    ABoolDataManager* subBoolDataManager() const;

    QFont value(const AData* data) const;

public Q_SLOTS:
    void setValue(AData* data, const QFont& val);

Q_SIGNALS:
    void valueChanged(AData* data, const QFont& val);

protected:
    QString toText(const AData* data) const;
    QIcon valueIcon(const AData* data) const;
    virtual void initializeData(AData* data);
    virtual void uninitializeData(AData* data);

private Q_SLOTS:
    void slotIntChanged(AData* data, int value);
    void slotEnumChanged(AData* data, int value);
    void slotBoolChanged(AData* data, bool value);
    void slotDataDestroyed(AData* data);
    void slotFontDatabaseChanged();
    void slotFontDatabaseDelayedChange();

private:
    QStringList m_familyNames;

    typedef QMap<const AData*, QFont> DataValueMap;
    DataValueMap m_values;

    AIntDataManager* m_intDataManager;
    AEnumDataManager* m_enumDataManager;
    ABoolDataManager* m_boolDataManager;

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

    bool m_settingValue;
    QTimer* m_fontDatabaseChangeTimer;

private:
    Q_DISABLE_COPY_MOVE(AFontDataManager)
};

APROCH_NAMESPACE_END