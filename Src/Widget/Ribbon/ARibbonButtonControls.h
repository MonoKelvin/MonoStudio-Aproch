/****************************************************************************
 * @file    ARibbonButtonControls.h
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
#include <QCheckBox>
#include <QToolButton>
#include <QRadioButton>
#include <QMenu>

#include "ARibbonControls.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class ARibbonButtonControlPrivate;
/* ARibbonButtonControl */
class APROCH_API ARibbonButtonControl : public ARibbonWidgetControl
{
    Q_OBJECT
public:
    explicit ARibbonButtonControl(ARibbonGroup* parent = nullptr);
    virtual ~ARibbonButtonControl();
public:
    const QIcon& largeIcon() const;
    void setLargeIcon(const QIcon& icon);

    const QIcon& smallIcon() const;
    void setSmallIcon(const QIcon& icon);

    const QString& label() const;
    void setLabel(const QString& label);

    void setMenu(QMenu* menu);
    QMenu* menu() const;

    void setPopupMode(QToolButton::ToolButtonPopupMode mode);
    QToolButton::ToolButtonPopupMode popupMode() const;

    void setToolButtonStyle(Qt::ToolButtonStyle style);
    Qt::ToolButtonStyle toolButtonStyle() const;

    QToolButton* widget() const;
public:
    virtual QSize sizeHint() const;
    virtual void sizeChanged(ARibbonControlSizeDefinition::GroupSize size);
    virtual void actionChanged();
protected:
    virtual bool event(QEvent* event);
private:
    A_DECLARE_PRIVATE(ARibbonButtonControl)
    Q_DISABLE_COPY(ARibbonButtonControl)
};


/* ARibbonCheckBoxControl */
class APROCH_API ARibbonCheckBoxControl : public ARibbonWidgetControl
{
public:
    explicit ARibbonCheckBoxControl(const QString& text = QString(), ARibbonGroup* parent = nullptr);
    virtual ~ARibbonCheckBoxControl();
public:
    QCheckBox* widget() const;
private:
    Q_DISABLE_COPY(ARibbonCheckBoxControl)
};

/* ARibbonRadioButtonControl */
class APROCH_API ARibbonRadioButtonControl : public ARibbonWidgetControl
{
public:
    explicit ARibbonRadioButtonControl(const QString& text = QString(), ARibbonGroup* parent = nullptr);
    virtual ~ARibbonRadioButtonControl();
public:
    QRadioButton* widget() const;
private:
    Q_DISABLE_COPY(ARibbonRadioButtonControl)
};

APROCH_NAMESPACE_END