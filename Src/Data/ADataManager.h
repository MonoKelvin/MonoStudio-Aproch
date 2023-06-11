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

/** The code written is based on QtPropertyManager.h from Qt Company.. */

class QDate;
class QTime;
class QDateTime;
class QLocale;

/** @brief 数值型数据管理器的类声明宏 */
#define A_DECLARE_NUMBER_DATAMANAGER(T, DataType)                               \
    class T##Private;                                                           \
    class APROCH_API T : public AAbstractDataManager                            \
    {                                                                           \
        Q_OBJECT                                                                \
    public:                                                                     \
        T(QObject *parent = nullptr);                                           \
        ~T();                                                                   \
        int getType() const override;                                           \
        QVariant getValue(AData *data) const override;                          \
        QVariant getAttribute(AData *data, const QString &name) const override; \
        DataType minimum(const AData *data) const;                              \
        DataType maximum(const AData *data) const;                              \
        DataType singleStep(const AData *data) const;                           \
    public Q_SLOTS:                                                             \
        void setValue(AData *data, const QVariant& val)                         \
        void setAttribute(AData *data, const QString &name,                     \
                          const QVariant &theVal);                              \
                                                                                \
    protected:                                                                  \
        QString toString(const AData *data) const;                              \
        virtual void initializeData(AData *data);                               \
        virtual void uninitializeData(AData *data);                             \
                                                                                \
    private:                                                                    \
        QScopedPointer<T##Private> d_ptr;                                       \
        Q_DECLARE_PRIVATE(T);                                                   \
        Q_DISABLE_COPY_MOVE(T);                                                 \
    }

/** @brief 数值型数据管理器的类实现宏 */
#define A_IMPLEMENTS_NUMBER_DATAMANAGER(T, DataType)                                                        \
    class T##Private                                                                                        \
    {                                                                                                       \
    public:                                                                                                 \
        struct Data                                                                                         \
        {                                                                                                   \
            DataType val{0};                                                                                \
            DataType minVal{(std::numeric_limits<DataType>::min)()};                                        \
            DataType maxVal{(std::numeric_limits<DataType>::max)()};                                        \
            DataType singleStep{1};                                                                         \
            DataType minimumValue() const { return minVal; }                                                \
            DataType maximumValue() const { return maxVal; }                                                \
            void setMinimumValue(DataType newMinVal) { setSimpleMinimumData(this, newMinVal); }             \
            void setMaximumValue(DataType newMaxVal) { setSimpleMaximumData(this, newMaxVal); }             \
        };                                                                                                  \
        typedef QMap<const AData *, Data> DataValueMap;                                                     \
        DataValueMap m_values;                                                                              \
    };                                                                                                      \
    T::T(QObject *parent) : AAbstractDataManager(parent), d_ptr(new T##Private) {}                          \
    T::~T() { clear(); }                                                                                    \
    QVariant T::getValue(AData *data) const                                                                 \
    {                                                                                                       \
        return ::getValue<DataType>(d_ptr->m_values, data, 0);                                              \
    }                                                                                                       \
    DataType T::minimum(const AData *data) const                                                            \
    {                                                                                                       \
        return getMinimum<DataType>(d_ptr->m_values, data, 0);                                              \
    }                                                                                                       \
    DataType T::maximum(const AData *data) const                                                            \
    {                                                                                                       \
        return getMaximum<DataType>(d_ptr->m_values, data, 0);                                              \
    }                                                                                                       \
    DataType T::singleStep(const AData *data) const                                                         \
    {                                                                                                       \
        return getData<DataType>(d_ptr->m_values, &T##Private::Data::singleStep, data, 0);                  \
    }                                                                                                       \
    QString T::toString(const AData *data) const                                                            \
    {                                                                                                       \
        const T##Private::DataValueMap::const_iterator it = d_ptr->m_values.constFind(data);                \
        if (it == d_ptr->m_values.constEnd())                                                               \
            return QString();                                                                               \
        return QString::number(it.value().val);                                                             \
    }                                                                                                       \
    void T::setValue(AData *data, const QVariant &val)                                                      \
    {                                                                                                       \
        void (T##Private::*setSubDataValue)(AData *, DataType) = 0;                                         \
        setValueInRange<DataType, T##Private, T, DataType>(this, d_ptr.data(),                              \
                                                           &T::dataChanged,                                 \
                                                           &T::valueChanged,                                \
                                                           data, val, setSubDataValue);                     \
    }                                                                                                       \
    void T::setAttribute(AData *data, const QString &attribute, const QVariant &value)                      \
    {                                                                                                       \
        if (attribute.compare(minimumAttribute, Qt::CaseInsensitive) == 0)                                  \
        {                                                                                                   \
            setMinimumValue<double, T##Private, T,                                                          \
                            double, T##Private::Data>(this, d_ptr.data(), &T::dataChanged,                  \
                                                      &T::attributeChanged, data, value.value<DataType>()); \
        }                                                                                                   \
        else if (attribute.compare(maximumAttribute, Qt::CaseInsensitive) == 0)                             \
        {                                                                                                   \
            setMaximumValue<double, T##Private, T,                                                          \
                            double, T##Private::Data>(this, d_ptr.data(), &T::dataChanged,                  \
                                                      &T::attributeChanged, data, value.value<DataType>()); \
        }                                                                                                   \
        else if (attribute.compare(singleStepAttribute, Qt::CaseInsensitive) == 0)                          \
        {                                                                                                   \
            const T##Private::DataValueMap::iterator it =                                                   \
                d_ptr->m_values.find(data);                                                                 \
            if (it == d_ptr->m_values.end())                                                                \
                return;                                                                                     \
            double step = value.value<DataType>();                                                          \
            T##Private::Data metaData = it.value();                                                         \
            if (step < 0)                                                                                   \
                step = 0;                                                                                   \
            if (metaData.singleStep == step)                                                                \
                return;                                                                                     \
            metaData.singleStep = step;                                                                     \
            it.value() = metaData;                                                                          \
            emit attributeChanged(data, singleStepAttribute, metaData.singleStep);                          \
        }                                                                                                   \
    }                                                                                                       \
    QVariant getAttribute(AData *data, const QString &name)                                                 \
    {                                                                                                       \
        if (attribute.compare(minimumAttribute, Qt::CaseInsensitive) == 0)                                  \
            return minimum(data);                                                                           \
        if (attribute.compare(maximumAttribute, Qt::CaseInsensitive) == 0)                                  \
            return maximum(data);                                                                           \
        if (attribute.compare(singleStepAttribute, Qt::CaseInsensitive) == 0)                               \
            return singleStep(data);                                                                        \
        return QVariant();                                                                                  \
    }                                                                                                       \
    void T::initializeData(AData *data)                                                                     \
    {                                                                                                       \
        d_ptr->m_values[data] = T##Private::Data();                                                         \
    }                                                                                                       \
    void T::uninitializeData(AData *data)                                                                   \
    {                                                                                                       \
        d_ptr->m_values.remove(data);                                                                       \
    }

APROCH_NAMESPACE_BEGIN

A_DECLARE_NUMBER_DATAMANAGER(AInt8DataManager, qint8);
A_DECLARE_NUMBER_DATAMANAGER(AUInt8DataManager, quint8);
A_DECLARE_NUMBER_DATAMANAGER(AInt16DataManager, qint16);
A_DECLARE_NUMBER_DATAMANAGER(AUInt16DataManager, quint16);
A_DECLARE_NUMBER_DATAMANAGER(AIntDataManager, qint32);
A_DECLARE_NUMBER_DATAMANAGER(AUIntDataManager, quint32);
A_DECLARE_NUMBER_DATAMANAGER(ALongLongDataManager, qint64);
A_DECLARE_NUMBER_DATAMANAGER(AULongLongDataManager, quint64);

class ABoolDataManagerPrivate;

class APROCH_API ABoolDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    ABoolDataManager(QObject *parent = nullptr);
    ~ABoolDataManager();

    virtual int getType() const override;
    virtual QVariant getValue(AData *data) const override;

public Q_SLOTS:
    virtual void setValue(AData *data, const QVariant &value) override;

protected:
    QString toString(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private:
    QScopedPointer<ABoolDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ABoolDataManager);
    Q_DISABLE_COPY_MOVE(ABoolDataManager);
};

class AFloatDataManagerPrivate;

class APROCH_API AFloatDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    AFloatDataManager(QObject *parent = nullptr);
    ~AFloatDataManager();

    float minimum(const AData *data) const;
    float maximum(const AData *data) const;
    float singleStep(const AData *data) const;
    int decimals(const AData *data) const;

    virtual int getType() const override;
    virtual QVariant getValue(AData *data) const override;
    virtual QVariant getAttribute(AData *data, const QString &attribute) const;

public Q_SLOTS:
    virtual void setValue(AData *data, const QVariant &value) override;
    virtual void setAttribute(AData *data, const QString &attribute, const QVariant &value) override;

protected:
    QString toString(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private:
    QScopedPointer<AFloatDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(AFloatDataManager);
    Q_DISABLE_COPY_MOVE(AFloatDataManager);
};

class ADoubleDataManagerPrivate;

class APROCH_API ADoubleDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    ADoubleDataManager(QObject *parent = nullptr);
    ~ADoubleDataManager();

    double minimum(const AData *data) const;
    double maximum(const AData *data) const;
    double singleStep(const AData *data) const;
    int decimals(const AData *data) const;

    virtual int getType() const override;
    virtual QVariant getValue(AData *data) const override;
    virtual QVariant getAttribute(AData *data, const QString &attribute) const;

public Q_SLOTS:
    virtual void setValue(AData *data, const QVariant &value) override;
    virtual void setAttribute(AData *data, const QString &attribute, const QVariant &value) override;

protected:
    QString toString(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private:
    QScopedPointer<ADoubleDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ADoubleDataManager);
    Q_DISABLE_COPY_MOVE(ADoubleDataManager);
};

class AStringDataManagerPrivate;

class APROCH_API AStringDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    AStringDataManager(QObject *parent = nullptr);
    ~AStringDataManager();

    virtual int getType() const override;
    virtual QVariant getValue(AData *data) const override;
    virtual QVariant getAttribute(AData *data, const QString &attribute) const;

public Q_SLOTS:
    virtual void setValue(AData *data, const QVariant &value) override;
    virtual void setAttribute(AData *data, const QString &attribute, const QVariant &value) override;

protected:
    QString toString(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private:
    QScopedPointer<AStringDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(AStringDataManager);
    Q_DISABLE_COPY_MOVE(AStringDataManager);
};

class AStringListDataManagerPrivate;

class APROCH_API AStringListDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    AStringListDataManager(QObject *parent = nullptr);
    ~AStringListDataManager();

    virtual int getType() const override;
    virtual QVariant getValue(AData *data) const override;

public Q_SLOTS:
    virtual void setValue(AData *data, const QVariant &value) override;

protected:
    QString toString(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private:
    QScopedPointer<AStringListDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(AStringListDataManager);
    Q_DISABLE_COPY_MOVE(AStringListDataManager);
};

class ADateDataManagerPrivate;

class APROCH_API ADateDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    ADateDataManager(QObject *parent = nullptr);
    ~ADateDataManager();

    QDate minimum(const AData *data) const;
    QDate maximum(const AData *data) const;

    virtual int getType() const override;
    virtual QVariant getValue(AData *data) const override;
    virtual QVariant getAttribute(AData *data, const QString &attribute) const;

public Q_SLOTS:
    virtual void setValue(AData *data, const QVariant &value) override;
    virtual void setAttribute(AData *data, const QString &attribute, const QVariant &value) override;

protected:
    QString toString(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private:
    QScopedPointer<ADateDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ADateDataManager);
    Q_DISABLE_COPY_MOVE(ADateDataManager);
};

class ATimeDataManagerPrivate;

class APROCH_API ATimeDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    ATimeDataManager(QObject *parent = nullptr);
    ~ATimeDataManager();

    QTime minimum(const AData *data) const;
    QTime maximum(const AData *data) const;

    virtual int getType() const override;
    virtual QVariant getValue(AData *data) const override;
    virtual QVariant getAttribute(AData *data, const QString &attribute) const;

public Q_SLOTS:
    virtual void setValue(AData *data, const QVariant &value) override;
    virtual void setAttribute(AData *data, const QString &attribute, const QVariant &value) override;

protected:
    QString toString(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private:
    QScopedPointer<ATimeDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ATimeDataManager);
    Q_DISABLE_COPY_MOVE(ATimeDataManager);
};

class ADateTimeDataManagerPrivate;

class APROCH_API ADateTimeDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    ADateTimeDataManager(QObject *parent = nullptr);
    ~ADateTimeDataManager();

    virtual int getType() const override;
    virtual QVariant getValue(AData *data) const override;

public Q_SLOTS:
    virtual void setValue(AData *data, const QVariant &value) override;

protected:
    QString toString(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private:
    QScopedPointer<ADateTimeDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ADateTimeDataManager);
    Q_DISABLE_COPY_MOVE(ADateTimeDataManager);
};

class AKeySequenceDataManagerPrivate;

class APROCH_API AKeySequenceDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    AKeySequenceDataManager(QObject *parent = nullptr);
    ~AKeySequenceDataManager();

    virtual int getType() const override;
    virtual QVariant getValue(AData *data) const override;

public Q_SLOTS:
    virtual void setValue(AData *data, const QVariant &value) override;

protected:
    QString toString(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private:
    QScopedPointer<AKeySequenceDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(AKeySequenceDataManager);
    Q_DISABLE_COPY_MOVE(AKeySequenceDataManager);
};

class ACharDataManagerPrivate;

class APROCH_API ACharDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    ACharDataManager(QObject *parent = nullptr);
    ~ACharDataManager();

    virtual int getType() const override;
    virtual QVariant getValue(AData *data) const override;

public Q_SLOTS:
    virtual void setValue(AData *data, const QVariant &value) override;

protected:
    QString toString(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private:
    QScopedPointer<ACharDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ACharDataManager);
    Q_DISABLE_COPY_MOVE(ACharDataManager);
};

class AEnumDataManager;
class ALocaleDataManagerPrivate;

class APROCH_API ALocaleDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    ALocaleDataManager(QObject *parent = nullptr);
    ~ALocaleDataManager();

    AEnumDataManager *subEnumDataManager() const;

    virtual int getType() const override;
    virtual QVariant getValue(AData *data) const override;

public Q_SLOTS:
    virtual void setValue(AData *data, const QVariant &value) override;

private Q_SLOTS:
    void onEnumChanged(AData *data, int val);
    void onDataDestroyed(AData *data);

protected:
    QString toString(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private:
    QScopedPointer<ALocaleDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ALocaleDataManager);
    Q_DISABLE_COPY_MOVE(ALocaleDataManager);
};

class APointDataManagerPrivate;

class APROCH_API APointDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    APointDataManager(QObject *parent = nullptr);
    ~APointDataManager();

    AIntDataManager *subIntDataManager() const;

    virtual int getType() const override;
    virtual QVariant getValue(AData *data) const override;

public Q_SLOTS:
    virtual void setValue(AData *data, const QVariant &value) override;

protected:
    QString toString(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private Q_SLOTS:
    void onIntChanged(AData *data, int val);
    void onDataDestroyed(AData *data);

private:
    QScopedPointer<APointDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(APointDataManager);
    Q_DISABLE_COPY_MOVE(APointDataManager);
};

class APointFDataManagerPrivate;

class APROCH_API APointFDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    APointFDataManager(QObject *parent = nullptr);
    ~APointFDataManager();

    ADoubleDataManager *subDoubleDataManager() const;

    int decimals(const AData *data) const;

    virtual int getType() const override;
    virtual QVariant getValue(AData *data) const override;
    virtual QVariant getAttribute(AData *data, const QString &attribute) const;

public Q_SLOTS:
    virtual void setValue(AData *data, const QVariant &value) override;
    virtual void setAttribute(AData *data, const QString &attribute, const QVariant &value) override;

protected:
    QString toString(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private Q_SLOTS:
    void onDoubleChanged(AData *data, double val);
    void onDataDestroyed(AData *data);

private:
    QScopedPointer<APointFDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(APointFDataManager);
    Q_DISABLE_COPY_MOVE(APointFDataManager);
};

class ASizeDataManagerPrivate;

class APROCH_API ASizeDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    ASizeDataManager(QObject *parent = nullptr);
    ~ASizeDataManager();

    AIntDataManager *subIntDataManager() const;

    QSize minimum(const AData *data) const;
    QSize maximum(const AData *data) const;
    void setRange(AData *data, const QSize &minVal, const QSize &maxVal);

    virtual int getType() const override;
    virtual QVariant getValue(AData *data) const override;
    virtual QVariant getAttribute(AData *data, const QString &attribute) const;

public Q_SLOTS:
    virtual void setValue(AData *data, const QVariant &value) override;
    virtual void setAttribute(AData *data, const QString &attribute, const QVariant &value) override;

protected:
    QString toString(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private Q_SLOTS:
    void onIntChanged(AData *data, int val);
    void onDataDestroyed(AData *data);

private:
    QScopedPointer<ASizeDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ASizeDataManager);
    Q_DISABLE_COPY_MOVE(ASizeDataManager);
};

class ASizeFDataManagerPrivate;

class APROCH_API ASizeFDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    ASizeFDataManager(QObject *parent = nullptr);
    ~ASizeFDataManager();

    ADoubleDataManager *subDoubleDataManager() const;

    QSizeF value(const AData *data) const;
    QSizeF minimum(const AData *data) const;
    QSizeF maximum(const AData *data) const;
    int decimals(const AData *data) const;

public Q_SLOTS:
    void setValue(AData *data, const QSizeF &val);
    void setMinimum(AData *data, const QSizeF &minVal);
    void setMaximum(AData *data, const QSizeF &maxVal);
    void setRange(AData *data, const QSizeF &minVal, const QSizeF &maxVal);
    void setDecimals(AData *data, int prec);

Q_SIGNALS:
    void valueChanged(AData *data, const QSizeF &val);
    void rangeChanged(AData *data, const QSizeF &minVal, const QSizeF &maxVal);
    void decimalsChanged(AData *data, int prec);

protected:
    QString toString(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private Q_SLOTS:
    void onDoubleChanged(AData *data, double val);
    void onDataDestroyed(AData *data);

private:
    QScopedPointer<ASizeFDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ASizeFDataManager);
    Q_DISABLE_COPY_MOVE(ASizeFDataManager);
};

class ARectDataManagerPrivate;

class APROCH_API ARectDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    ARectDataManager(QObject *parent = nullptr);
    ~ARectDataManager();

    AIntDataManager *subIntDataManager() const;

    QRect value(const AData *data) const;
    QRect constraint(const AData *data) const;

public Q_SLOTS:
    void setValue(AData *data, const QRect &val);
    void setConstraint(AData *data, const QRect &constraint);
Q_SIGNALS:
    void valueChanged(AData *data, const QRect &val);
    void constraintChanged(AData *data, const QRect &constraint);

protected:
    QString toString(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private Q_SLOTS:
    void onIntChanged(AData *data, int val);
    void onDataDestroyed(AData *data);

private:
    QScopedPointer<ARectDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ARectDataManager);
    Q_DISABLE_COPY_MOVE(ARectDataManager);
};

class ARectFDataManagerPrivate;

class APROCH_API ARectFDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    ARectFDataManager(QObject *parent = nullptr);
    ~ARectFDataManager();

    ADoubleDataManager *subDoubleDataManager() const;

    QRectF value(const AData *data) const;
    QRectF constraint(const AData *data) const;
    int decimals(const AData *data) const;

public Q_SLOTS:
    void setValue(AData *data, const QRectF &val);
    void setConstraint(AData *data, const QRectF &constraint);
    void setDecimals(AData *data, int prec);
Q_SIGNALS:
    void valueChanged(AData *data, const QRectF &val);
    void constraintChanged(AData *data, const QRectF &constraint);
    void decimalsChanged(AData *data, int prec);

protected:
    QString toString(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private Q_SLOTS:
    void onDoubleChanged(AData *data, double val);
    void onDataDestroyed(AData *data);

private:
    QScopedPointer<ARectFDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ARectFDataManager);
    Q_DISABLE_COPY_MOVE(ARectFDataManager);
};

class AEnumDataManagerPrivate;

class APROCH_API AEnumDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    AEnumDataManager(QObject *parent = nullptr);
    ~AEnumDataManager();

    int value(const AData *data) const;
    QStringList enumNames(const AData *data) const;
    QMap<int, QIcon> enumIcons(const AData *data) const;

public Q_SLOTS:
    void setValue(AData *data, int val);
    void setEnumNames(AData *data, const QStringList &names);
    void setEnumIcons(AData *data, const QMap<int, QIcon> &icons);

Q_SIGNALS:
    void valueChanged(AData *data, int val);
    void enumNamesChanged(AData *data, const QStringList &names);
    void enumIconsChanged(AData *data, const QMap<int, QIcon> &icons);

protected:
    QString toString(const AData *data) const;
    QIcon valueIcon(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private:
    QScopedPointer<AEnumDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(AEnumDataManager);
    Q_DISABLE_COPY_MOVE(AEnumDataManager);
};

class AFlagDataManagerPrivate;

class APROCH_API AFlagDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    AFlagDataManager(QObject *parent = nullptr);
    ~AFlagDataManager();

    ABoolDataManager *subBoolDataManager() const;

    int value(const AData *data) const;
    QStringList flagNames(const AData *data) const;

public Q_SLOTS:
    void setValue(AData *data, int val);
    void setFlagNames(AData *data, const QStringList &names);

Q_SIGNALS:
    void valueChanged(AData *data, int val);
    void flagNamesChanged(AData *data, const QStringList &names);

protected:
    QString toString(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private Q_SLOTS:
    void onBoolChanged(AData *data, bool val);
    void onDataDestroyed(AData *data);

private:
    QScopedPointer<AFlagDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(AFlagDataManager);
    Q_DISABLE_COPY_MOVE(AFlagDataManager);
};

class ASizePolicyDataManagerPrivate;

class APROCH_API ASizePolicyDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    ASizePolicyDataManager(QObject *parent = nullptr);
    ~ASizePolicyDataManager();

    AIntDataManager *subIntDataManager() const;
    AEnumDataManager *subEnumDataManager() const;

    QSizePolicy value(const AData *data) const;

public Q_SLOTS:
    void setValue(AData *data, const QSizePolicy &val);
Q_SIGNALS:
    void valueChanged(AData *data, const QSizePolicy &val);

protected:
    QString toString(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private Q_SLOTS:
    void onIntChanged(AData *data, int val);
    void onEnumChanged(AData *data, int val);
    void onDataDestroyed(AData *data);

private:
    QScopedPointer<ASizePolicyDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ASizePolicyDataManager);
    Q_DISABLE_COPY_MOVE(ASizePolicyDataManager);
};

class AFontDataManagerPrivate;

class APROCH_API AFontDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    AFontDataManager(QObject *parent = nullptr);
    ~AFontDataManager();

    AIntDataManager *subIntDataManager() const;
    AEnumDataManager *subEnumDataManager() const;
    ABoolDataManager *subBoolDataManager() const;

    QFont value(const AData *data) const;

public Q_SLOTS:
    void setValue(AData *data, const QFont &val);

Q_SIGNALS:
    void valueChanged(AData *data, const QFont &val);

protected:
    QString toString(const AData *data) const;
    QIcon valueIcon(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private Q_SLOTS:
    void onIntChanged(AData *, int);
    void onEnumChanged(AData *, int);
    void onBoolChanged(AData *, bool);
    void onDataDestroyed(AData *);
    void onFontDatabaseChanged();
    void onFontDatabaseDelayedChange();

private:
    QScopedPointer<AFontDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(AFontDataManager);
    Q_DISABLE_COPY_MOVE(AFontDataManager);
};

class AColorDataManagerPrivate;

class APROCH_API AColorDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    AColorDataManager(QObject *parent = nullptr);
    ~AColorDataManager();

    AIntDataManager *subIntDataManager() const;

    QColor value(const AData *data) const;

public Q_SLOTS:
    void setValue(AData *data, const QColor &val);

Q_SIGNALS:
    void valueChanged(AData *data, const QColor &val);

protected:
    QString toString(const AData *data) const;
    QIcon valueIcon(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private Q_SLOTS:
    void onIntChanged(AData *, int);
    void onDataDestroyed(AData *);

private:
    QScopedPointer<AColorDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(AColorDataManager);
    Q_DISABLE_COPY_MOVE(AColorDataManager);
};

class ACursorDataManagerPrivate;

class APROCH_API ACursorDataManager : public AAbstractDataManager
{
    Q_OBJECT
public:
    ACursorDataManager(QObject *parent = nullptr);
    ~ACursorDataManager();

#ifndef QT_NO_CURSOR
    QCursor value(const AData *data) const;
#endif

public Q_SLOTS:
    void setValue(AData *data, const QCursor &val);
Q_SIGNALS:
    void valueChanged(AData *data, const QCursor &val);

protected:
    QString toString(const AData *data) const;
    QIcon valueIcon(const AData *data) const;
    virtual void initializeData(AData *data);
    virtual void uninitializeData(AData *data);

private:
    QScopedPointer<ACursorDataManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ACursorDataManager);
    Q_DISABLE_COPY_MOVE(ACursorDataManager);
};

APROCH_NAMESPACE_END
