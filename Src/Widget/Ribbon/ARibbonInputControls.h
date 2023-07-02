/****************************************************************************
 * @file    ARibbonInputControls.h
 * @date    2023-07-02 
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

#include <QComboBox>
#include <QFontComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QSlider>
#include <QDateTimeEdit>
#include <QTimeEdit>
#include <QDateEdit>

#include "ARibbonControls.h"
#include "ARibbonSliderPane.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

/* ARibbonFontComboBoxControl */
class APROCH_API ARibbonFontComboBoxControl : public ARibbonWidgetControl
{
public:
    explicit ARibbonFontComboBoxControl(ARibbonGroup* parent = nullptr);
    virtual ~ARibbonFontComboBoxControl();
public:
    QFontComboBox* widget() const;
private:
    Q_DISABLE_COPY(ARibbonFontComboBoxControl)
};


/* ARibbonLineEditControl */
class APROCH_API ARibbonLineEditControl : public ARibbonWidgetControl
{
public: 
    explicit ARibbonLineEditControl(ARibbonGroup* parent = nullptr);
    virtual ~ARibbonLineEditControl();
public:
    QLineEdit* widget() const;
private:
    Q_DISABLE_COPY(ARibbonLineEditControl)
};

/* ARibbonComboBoxControl */
class APROCH_API ARibbonComboBoxControl : public ARibbonWidgetControl
{
public: 
    explicit ARibbonComboBoxControl(ARibbonGroup* parent = nullptr);
    virtual ~ARibbonComboBoxControl();
public:
    QComboBox* widget() const;
private:
    Q_DISABLE_COPY(ARibbonComboBoxControl)
};

/* ARibbonSpinBoxControl */
class APROCH_API ARibbonSpinBoxControl : public ARibbonWidgetControl
{
public: 
    explicit ARibbonSpinBoxControl(ARibbonGroup* parent = nullptr);
    virtual ~ARibbonSpinBoxControl();
public:
    QSpinBox* widget() const;
private:
    Q_DISABLE_COPY(ARibbonSpinBoxControl)
};

/* ARibbonDoubleSpinBoxControl */
class APROCH_API ARibbonDoubleSpinBoxControl : public ARibbonWidgetControl
{
public: 
    explicit ARibbonDoubleSpinBoxControl(ARibbonGroup* parent = nullptr);
    virtual ~ARibbonDoubleSpinBoxControl();
public:
    QDoubleSpinBox* widget() const;
private:
    Q_DISABLE_COPY(ARibbonDoubleSpinBoxControl)
};

/* ARibbonSliderControl */
class APROCH_API ARibbonSliderControl : public ARibbonWidgetControl
{
public: 
    explicit ARibbonSliderControl(Qt::Orientation, ARibbonGroup* parent = nullptr);
    virtual ~ARibbonSliderControl();
public:
    QSlider* widget() const;
private:
    Q_DISABLE_COPY(ARibbonSliderControl)
};

/* ARibbonSliderPaneControl */
class APROCH_API ARibbonSliderPaneControl : public ARibbonWidgetControl
{
public: 
    explicit ARibbonSliderPaneControl(ARibbonGroup* parent = nullptr);
    virtual ~ARibbonSliderPaneControl();
public:
    ARibbonSliderPane* widget() const;
private:
    Q_DISABLE_COPY(ARibbonSliderPaneControl)
};

/* ARibbonDateTimeEditControl */
class APROCH_API ARibbonDateTimeEditControl : public ARibbonWidgetControl
{
public:
    explicit ARibbonDateTimeEditControl(ARibbonGroup* parent = nullptr);
    virtual ~ARibbonDateTimeEditControl();
public:
    QDateTimeEdit* widget() const;
    bool calendarPopup() const;
    void setCalendarPopup(bool enable);
private:
    Q_DISABLE_COPY(ARibbonDateTimeEditControl)
};

/* ARibbonTimeEditControl */
class APROCH_API ARibbonTimeEditControl : public ARibbonWidgetControl
{
public:
    explicit ARibbonTimeEditControl(ARibbonGroup* parent = nullptr);
    virtual ~ARibbonTimeEditControl();
public:
    QTimeEdit* widget() const;
private:
    Q_DISABLE_COPY(ARibbonTimeEditControl)
};

/* ARibbonDateEditControl */
class APROCH_API ARibbonDateEditControl : public ARibbonWidgetControl
{
public:
    explicit ARibbonDateEditControl(ARibbonGroup* parent = nullptr);
    virtual ~ARibbonDateEditControl();
public:
    QDateEdit* widget() const;
    bool calendarPopup() const;
    void setCalendarPopup(bool enable);
private:
    Q_DISABLE_COPY(ARibbonDateEditControl)
};

APROCH_NAMESPACE_END