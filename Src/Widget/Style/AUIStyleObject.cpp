/****************************************************************************
 * @file    AUIStyleObject.cpp
 * @date    2023-04-08 
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
#include "AUIStyleObject.h"

APROCH_NAMESPACE_BEGIN

// aproch property names
const char AUIStyleObject::BorderRadius[] = "_aprop_borderradius";
const char AUIStyleObject::BorderThickness[] = "_aprop_borderthickness";
const char AUIStyleObject::BorderColor[] = "_aprop_bordercolor";
const char AUIStyleObject::Margins[] = "_aprop_margins";
const char AUIStyleObject::Paddings[] = "_aprop_paddings";
const char AUIStyleObject::Spacing[] = "_aprop_spacing";
const char AUIStyleObject::TextSize[] = "_aprop_textsize";
const char AUIStyleObject::TextSpcing[] = "_aprop_textspcing";
const char AUIStyleObject::TextLineSpcing[] = "_aprop_textlinespcing";
const char AUIStyleObject::Shadow[] = "_aprop_shadow";
const char AUIStyleObject::Transition[] = "_aprop_transition";

// default styles
SCornerF AprochDefaultBorderRadius(8);
SBorderThicknessF AprochDefaultBorderThickness(0);
QColor AprochDefaultBorderColor(Qt::transparent);
QMargins AprochDefaultMargins(10, 10, 10, 10);
Paddings AprochDefaultPaddings(10, 10, 10, 10);
Spacing AprochDefaultSpacing(10);
//QColor AprochDefaultBackground;
QColor AprochDefaultBackgroundColor(Qt::white);
QString AprochDefaultFontFamily(QString::fromLatin1("Microsoft Yahei"));
QColor AprochDefaultTextColor(Qt::black);
int AprochDefaultTextSize(9);
Spacing AprochDefaultTextSpcing(2);
Spacing AprochDefaultTextLineSpcing(5);
SShadowParam AprochDefaultShadow;
//SAnimationParam AprochDefaultTransition;

class AUIStyleObjectPrivate
{
public:
    AUIStyleObjectPrivate();
    ~AUIStyleObjectPrivate();

#ifdef Q_OS_WIN
    //class ThemeWatcher : public QObject
    //{
    //    Q_OBJECT
    //public:
    //};
#endif // Q_OS_WIN

    QList<QByteArray> styleProperties;

    QWidget* widget = nullptr;
};

AUIStyleObjectPrivate::AUIStyleObjectPrivate()
{
}

AUIStyleObjectPrivate::~AUIStyleObjectPrivate()
{
}

// ----------------------------------------------------------------------------------------------------

AUIStyleObject::AUIStyleObject()
    : _uso_d_ptr(new AUIStyleObjectPrivate)
{
}

AUIStyleObject::~AUIStyleObject()
{
}

QVariant AUIStyleObject::getStyleValue(const char* name) const
{
    if (!_uso_d_ptr->styleProperties.contains(QByteArray(name)))
        return QVariant(QVariant::Invalid);
    return _uso_d_ptr->widget->property(name);
}

QVariant AUIStyleObject::getStyleValue(const char* name, const QVariant& defaultValue) const
{
    const QVariant val = getStyleValue(name);
    if (!val.isValid())
        return defaultValue;
    return val;
}

const QList<QByteArray>& AUIStyleObject::getStyleNameList() const
{
    return _uso_d_ptr->styleProperties;
}

void AUIStyleObject::bindStyleObject(QWidget* widget)
{
    _uso_d_ptr->widget = widget;
    Q_ASSERT(nullptr != _uso_d_ptr->widget);

    initProperties(widget);
}

void AUIStyleObject::addStyle(const char* name, const QVariant& val)
{
    Q_ASSERT_X(_uso_d_ptr->widget, Q_FUNC_INFO, "widget not initialized with bindStyleObject()");

    _uso_d_ptr->widget->setProperty(name, val);

    const QByteArray Name(name);
    if (!_uso_d_ptr->styleProperties.contains(Name))
        _uso_d_ptr->styleProperties.push_back(Name);
}

void AUIStyleObject::initProperties(QWidget* widget)
{
    addStyle(BorderRadius, AprochDefaultBorderRadius);
    addStyle(BorderThickness, AprochDefaultBorderThickness);
    addStyle(BorderColor, AprochDefaultBorderColor);
    addStyle(Margins, AprochDefaultMargins);
    addStyle(Paddings, AprochDefaultPaddings);
    addStyle(Spacing, AprochDefaultSpacing);
    addStyle(TextSize, AprochDefaultTextSize);
    addStyle(TextSpcing, AprochDefaultTextSpcing);
    addStyle(TextLineSpcing, AprochDefaultTextLineSpcing);
    addStyle(Shadow, AprochDefaultShadow);
    //addStyle(Transition, AprochDefaultTransition);
}

APROCH_NAMESPACE_END
