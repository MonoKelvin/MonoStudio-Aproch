/****************************************************************************
 * @file    ARibbonInputControls.cpp
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
#include "stdafx.h"
#include "ARibbonInputControls.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

/* ARibbonFontComboBoxControl */
ARibbonFontComboBoxControl::ARibbonFontComboBoxControl(ARibbonGroup* parentGroup)
    : ARibbonWidgetControl(parentGroup, false/*ignoreActionSettings*/)
{
    setContentWidget(new QFontComboBox(this));
}

ARibbonFontComboBoxControl::~ARibbonFontComboBoxControl()
{
}

QFontComboBox* ARibbonFontComboBoxControl::widget() const
{
    return qobject_cast<QFontComboBox*>(contentWidget());
}


/* ARibbonLineEditControl */
ARibbonLineEditControl::ARibbonLineEditControl(ARibbonGroup* parentGroup)
    : ARibbonWidgetControl(parentGroup, false/*ignoreActionSettings*/)
{
    setContentWidget(new QLineEdit(this));
}

ARibbonLineEditControl::~ARibbonLineEditControl()
{
}

QLineEdit* ARibbonLineEditControl::widget() const
{
    return qobject_cast<QLineEdit*>(contentWidget());
}

/* ARibbonComboBoxControl */
ARibbonComboBoxControl::ARibbonComboBoxControl(ARibbonGroup* parentGroup)
    : ARibbonWidgetControl(parentGroup, false/*ignoreActionSettings*/)
{
    setContentWidget(new QComboBox(this));
}

ARibbonComboBoxControl::~ARibbonComboBoxControl()
{
}

QComboBox* ARibbonComboBoxControl::widget() const
{
    return qobject_cast<QComboBox*>(contentWidget());
}

/* ARibbonSpinBoxControl */
ARibbonSpinBoxControl::ARibbonSpinBoxControl(ARibbonGroup* parentGroup)
    : ARibbonWidgetControl(parentGroup, false/*ignoreActionSettings*/)
{
    setContentWidget(new QSpinBox(this));
}

ARibbonSpinBoxControl::~ARibbonSpinBoxControl()
{
}

QSpinBox* ARibbonSpinBoxControl::widget() const
{
    return qobject_cast<QSpinBox*>(contentWidget());
}

/* ARibbonDoubleSpinBoxControl */
ARibbonDoubleSpinBoxControl::ARibbonDoubleSpinBoxControl(ARibbonGroup* parentGroup)
    : ARibbonWidgetControl(parentGroup, false/*ignoreActionSettings*/)
{
    setContentWidget(new QDoubleSpinBox(this));
}

ARibbonDoubleSpinBoxControl::~ARibbonDoubleSpinBoxControl()
{
}

QDoubleSpinBox* ARibbonDoubleSpinBoxControl::widget() const
{
    return qobject_cast<QDoubleSpinBox*>(contentWidget());
}


/* ARibbonSliderControl */
ARibbonSliderControl::ARibbonSliderControl(Qt::Orientation orientation, ARibbonGroup* parentGroup)
    : ARibbonWidgetControl(parentGroup, true/*ignoreActionSettings*/)
{
    setContentWidget(new QSlider(orientation, this));
}

ARibbonSliderControl::~ARibbonSliderControl()
{
}

QSlider* ARibbonSliderControl::widget() const
{
    return qobject_cast<QSlider*>(contentWidget());
}

/* ARibbonSliderPaneControl */
ARibbonSliderPaneControl::ARibbonSliderPaneControl(ARibbonGroup* parentGroup)
    : ARibbonWidgetControl(parentGroup, true/*ignoreActionSettings*/)
{
    setContentWidget(new ARibbonSliderPane(this));
}

ARibbonSliderPaneControl::~ARibbonSliderPaneControl()
{
}

ARibbonSliderPane* ARibbonSliderPaneControl::widget() const
{
    return qobject_cast<ARibbonSliderPane*>(contentWidget());
}


/* ARibbonDateTimeEditControl */
ARibbonDateTimeEditControl::ARibbonDateTimeEditControl(ARibbonGroup* parentGroup)
    : ARibbonWidgetControl(parentGroup, false/*ignoreActionSettings*/)
{
    setContentWidget(new QDateTimeEdit(this));
}

ARibbonDateTimeEditControl::~ARibbonDateTimeEditControl()
{
}

QDateTimeEdit* ARibbonDateTimeEditControl::widget() const
{
    return qobject_cast<QDateTimeEdit*>(contentWidget());
}

bool ARibbonDateTimeEditControl::calendarPopup() const
{
    QDateTimeEdit* dateTimeEdit = widget();
    Q_ASSERT(dateTimeEdit != nullptr);
    return dateTimeEdit->calendarPopup();
}

void ARibbonDateTimeEditControl::setCalendarPopup(bool enable)
{
    QDateTimeEdit* dateTimeEdit = widget();
    Q_ASSERT(dateTimeEdit != nullptr);
    dateTimeEdit->setCalendarPopup(enable);
}


/* ARibbonTimeEditControl */
ARibbonTimeEditControl::ARibbonTimeEditControl(ARibbonGroup* parentGroup)
    : ARibbonWidgetControl(parentGroup, false/*ignoreActionSettings*/)
{
    setContentWidget(new QTimeEdit(this));
}

ARibbonTimeEditControl::~ARibbonTimeEditControl()
{
}

QTimeEdit* ARibbonTimeEditControl::widget() const
{
    return qobject_cast<QTimeEdit*>(contentWidget());
}

/* ARibbonDateEditControl */
ARibbonDateEditControl::ARibbonDateEditControl(ARibbonGroup* parentGroup)
    : ARibbonWidgetControl(parentGroup, false/*ignoreActionSettings*/)
{
    setContentWidget(new QDateEdit(this));
}

ARibbonDateEditControl::~ARibbonDateEditControl()
{
}

QDateEdit* ARibbonDateEditControl::widget() const
{
    return qobject_cast<QDateEdit*>(contentWidget());
}

bool ARibbonDateEditControl::calendarPopup() const
{
    QDateEdit* dateEdit = widget();
    Q_ASSERT(dateEdit != nullptr);
    return dateEdit->calendarPopup();
}

void ARibbonDateEditControl::setCalendarPopup(bool enable)
{
    QDateEdit* dateEdit = widget();
    Q_ASSERT(dateEdit != nullptr);
    dateEdit->setCalendarPopup(enable);
}

APROCH_NAMESPACE_END