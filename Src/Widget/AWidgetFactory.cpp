/****************************************************************************
 * @file    AWidgetFactory.cpp
 * @date    2023-06-11 
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
#include "AWidgetFactory.h"

#include <QKeySequenceEdit>
#include <QCheckBox>

APROCH_NAMESPACE_BEGIN

class AFactoryMap
{
public:
    AFactoryMap()
    {
    }
    ~AFactoryMap()
    {
        qDeleteAll(factoryMap);
    }
    QMap<int, AWidgetFactory*> factoryMap;
};
Q_GLOBAL_STATIC(AFactoryMap, gFactoryMap);

AWidgetFactory::AWidgetFactory()
{
}

AWidgetFactory::~AWidgetFactory()
{
}

QWidget* AWidgetFactory::newWidget(int type, QWidget* parent)
{
    AWidgetFactory* widgetFactory = getFactory(type);
    if (!widgetFactory)
        return nullptr;
    return widgetFactory->createWidget(parent);
}

bool AWidgetFactory::registerFactory(int type, AWidgetFactory* factory)
{
    if (!factory)
        return false;

    AWidgetFactory* widgetFactory = gFactoryMap->factoryMap.value(type, nullptr);
    if (widgetFactory)
    {
        if (factory == widgetFactory)
            return true;

        delete widgetFactory;
        qWarning() << "the widget factory is overwritten";
    }

    gFactoryMap->factoryMap[type] = factory;

    return true;
}

void AWidgetFactory::unregister(int type)
{
    AWidgetFactory* widgetFactory = gFactoryMap->factoryMap.take(type);
    if (widgetFactory)
        delete widgetFactory;
}

bool AWidgetFactory::hasFactory(int type)
{
    return gFactoryMap->factoryMap.contains(type);
}

AWidgetFactory* AWidgetFactory::getFactory(int type)
{
    return gFactoryMap->factoryMap.value(type, nullptr);
}

// add default widget factory

A_WIDGET_FACTORY_REGISTER(QCheckBox, QMetaType::Bool, 1);

A_WIDGET_FACTORY_REGISTER(QSpinBox, QMetaType::Short, 1);
A_WIDGET_FACTORY_REGISTER(QSpinBox, QMetaType::UShort, 2);
A_WIDGET_FACTORY_REGISTER(QSpinBox, QMetaType::Int, 3);
A_WIDGET_FACTORY_REGISTER(QSpinBox, QMetaType::UInt, 4);

A_WIDGET_FACTORY_REGISTER(QDoubleSpinBox, QMetaType::Float, 1);
A_WIDGET_FACTORY_REGISTER(QDoubleSpinBox, QMetaType::Double, 2);
A_WIDGET_FACTORY_REGISTER(QDoubleSpinBox, QMetaType::QReal, 3);

A_WIDGET_FACTORY_REGISTER(QLineEdit, QMetaType::QChar, 1);
A_WIDGET_FACTORY_REGISTER(QLineEdit, QMetaType::Char, 2);
A_WIDGET_FACTORY_REGISTER(QLineEdit, QMetaType::UChar, 3);
A_WIDGET_FACTORY_REGISTER(QLineEdit, QMetaType::LongLong, 4);
A_WIDGET_FACTORY_REGISTER(QLineEdit, QMetaType::ULongLong, 5);
A_WIDGET_FACTORY_REGISTER(QLineEdit, QMetaType::QString, 6);
A_WIDGET_FACTORY_REGISTER(QLineEdit, QMetaType::QByteArray, 7);
A_WIDGET_FACTORY_REGISTER(QLineEdit, QMetaType::QRegularExpression, 8);

A_WIDGET_FACTORY_REGISTER(QComboBox, QMetaType::QStringList, 1);
A_WIDGET_FACTORY_REGISTER(QComboBox, QMetaType::QLocale, 2);
A_WIDGET_FACTORY_REGISTER(QComboBox, QMetaType::QSizePolicy, 3);

A_WIDGET_FACTORY_REGISTER(QDateEdit, QMetaType::QDate, 1);
A_WIDGET_FACTORY_REGISTER(QTimeEdit, QMetaType::QTime, 1);
A_WIDGET_FACTORY_REGISTER(QDateTimeEdit, QMetaType::QDateTime, 1);
A_WIDGET_FACTORY_REGISTER(QKeySequenceEdit, QMetaType::QKeySequence, 1);

APROCH_NAMESPACE_END
