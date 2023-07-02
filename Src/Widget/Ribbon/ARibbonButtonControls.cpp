/****************************************************************************
 * @file    ARibbonButtonControls.cpp
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
#include "ARibbonGroup.h"
#include "ARibbonControls_p.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

static Qt::ToolButtonStyle defineToolButtonStyle = Qt::ToolButtonTextUnderIcon;

ARibbonButtonControlPrivate::ARibbonButtonControlPrivate()
    : m_toolButton(nullptr)
{
}

ARibbonButtonControlPrivate::~ARibbonButtonControlPrivate()
{
}

void ARibbonButtonControlPrivate::init()
{
    A_P(ARibbonButtonControl);
    p.sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setStretchable(false);
    p.sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setStretchable(false);
    p.sizeDefinition(ARibbonControlSizeDefinition::GroupSmall)->setStretchable(false);
    p.sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setStretchable(false);

    m_toolButton = new ARibbonButton(&p);
    p.setContentWidget(m_toolButton);
}

/*!
\class ARibbonButtonControl
\inmodule QtitanRibbon
*/
ARibbonButtonControl::ARibbonButtonControl(ARibbonGroup* parentGroup)
    : ARibbonWidgetControl(parentGroup, true)
{
    A_INIT_PRIVATE(ARibbonButtonControl);
    A_D(ARibbonButtonControl);
    d.init();
}

ARibbonButtonControl::~ARibbonButtonControl()
{
    A_DELETE_PRIVATE();
}

const QIcon& ARibbonButtonControl::largeIcon() const
{
    A_D(const ARibbonButtonControl);
    return d.m_largeIcon;
}

void ARibbonButtonControl::setLargeIcon(const QIcon& icon)
{
    A_D(ARibbonButtonControl);
    d.m_largeIcon = icon;
}

const QIcon& ARibbonButtonControl::smallIcon() const
{
    A_D(const ARibbonButtonControl);
    return d.m_smallIcon;
}

void ARibbonButtonControl::setSmallIcon(const QIcon& icon)
{
    A_D(ARibbonButtonControl);
    d.m_smallIcon = icon;
}

const QString& ARibbonButtonControl::label() const
{
    A_D(const ARibbonButtonControl);
    return d.m_label;
}

void ARibbonButtonControl::setLabel(const QString& label)
{
    A_D(ARibbonButtonControl);
    d.m_label = label;
}

void ARibbonButtonControl::setMenu(QMenu* menu)
{
    A_D(ARibbonButtonControl);

     if (menu && d.m_toolButton->actions().contains(menu->menuAction()))
        d.m_toolButton->removeAction(menu->menuAction());

    d.m_toolButton->setMenu(menu);
}

QMenu* ARibbonButtonControl::menu() const
{
    A_D(const ARibbonButtonControl);
    return d.m_toolButton->menu();
}

void ARibbonButtonControl::setPopupMode(QToolButton::ToolButtonPopupMode mode)
{
    A_D(ARibbonButtonControl);
    d.m_toolButton->setPopupMode(mode);
}

QToolButton::ToolButtonPopupMode ARibbonButtonControl::popupMode() const
{
    A_D(const ARibbonButtonControl);
    return d.m_toolButton->popupMode();
}

void ARibbonButtonControl::setToolButtonStyle(Qt::ToolButtonStyle style)
{
    A_D(ARibbonButtonControl);
    if (style == Qt::ToolButtonFollowStyle)
        style = defineToolButtonStyle;

    d.m_toolButton->setToolButtonStyle(style);
    if (style == Qt::ToolButtonIconOnly)
    {
        sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
        sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setLabelVisible(false);
        sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
        sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setLabelVisible(false);
        sizeDefinition(ARibbonControlSizeDefinition::GroupSmall)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
        sizeDefinition(ARibbonControlSizeDefinition::GroupSmall)->setLabelVisible(false);
        sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
        sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setLabelVisible(false);
    }
    else if (style == Qt::ToolButtonTextOnly)
    {
        sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setImageSize(ARibbonControlSizeDefinition::ImageNone);
        sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setLabelVisible(true);
        sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setImageSize(ARibbonControlSizeDefinition::ImageNone);
        sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setLabelVisible(true);
        sizeDefinition(ARibbonControlSizeDefinition::GroupSmall)->setImageSize(ARibbonControlSizeDefinition::ImageNone);
        sizeDefinition(ARibbonControlSizeDefinition::GroupSmall)->setLabelVisible(true);
        sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setImageSize(ARibbonControlSizeDefinition::ImageNone);
        sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setLabelVisible(true);
    }
    else if (style == Qt::ToolButtonTextBesideIcon)
    {
        sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
        sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
        sizeDefinition(ARibbonControlSizeDefinition::GroupSmall)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
        sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
    }
    else if (style == Qt::ToolButtonTextUnderIcon)
    {
        sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setImageSize(ARibbonControlSizeDefinition::ImageLarge);
        sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setLabelVisible(true);
        sizeDefinition(ARibbonControlSizeDefinition::GroupSmall)->setLabelVisible(true);
        sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setLabelVisible(true);
    }
}

Qt::ToolButtonStyle ARibbonButtonControl::toolButtonStyle() const
{
    A_D(const ARibbonButtonControl);
    return d.m_toolButton->toolButtonStyle();
}

QToolButton* ARibbonButtonControl::widget() const
{
    A_D(const ARibbonButtonControl);
    return d.m_toolButton;
}

QSize ARibbonButtonControl::sizeHint() const
{
    A_D(const ARibbonButtonControl);
    QSize sz = ARibbonWidgetControl::sizeHint();

    // 菜单类型预留箭头宽度
	if (d.m_toolButton && menu())
	{
        Qt::ToolButtonStyle style = toolButtonStyle();
        if (style == Qt::ToolButtonTextUnderIcon)
        {
		    sz.setWidth(sz.width() + 12);
        }
	}

    if (d.m_toolButton && parentGroup() != nullptr)
    {
        if (ARibbonBar* ribbonBar = parentGroup()->ribbonBar())
        {
            Qt::ToolButtonStyle style = toolButtonStyle();
            if (d.m_toolButton->isLargeIcon() && (style == Qt::ToolButtonIconOnly || style == Qt::ToolButtonTextUnderIcon))
                sz.setHeight(ribbonBar->rowItemHeight() * ribbonBar->rowItemCount());
            else if (style == Qt::ToolButtonTextBesideIcon || style == Qt::ToolButtonIconOnly || style == Qt::ToolButtonTextOnly)
                sz.setHeight(ribbonBar->rowItemHeight());
        }
    }

    return sz;
}

void ARibbonButtonControl::sizeChanged(ARibbonControlSizeDefinition::GroupSize size)
{
    A_D(ARibbonButtonControl);
    ARibbonControl::sizeChanged(size);

    d.m_toolButton->setDefaultAction(defaultAction());

    if (sizeDefinition(size)->imageSize() == ARibbonControlSizeDefinition::ImageLarge)
    {
        d.m_toolButton->setLargeIcon(true);
        d.m_toolButton->setIcon(largeIcon());
        if (sizeDefinition(size)->isLabelVisible())
        {
            d.m_toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            d.m_toolButton->setText(label());
        }
        else
            d.m_toolButton->setToolButtonStyle(Qt::ToolButtonIconOnly);

        d.m_toolButton->setWordWrap(sizeDefinition(size)->wordWrap());
    }
    else if (sizeDefinition(size)->imageSize() == ARibbonControlSizeDefinition::ImageSmall)
    {
        d.m_toolButton->setLargeIcon(false);
        d.m_toolButton->setIcon(smallIcon());
        if (sizeDefinition(size)->isLabelVisible())
        {
            d.m_toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            d.m_toolButton->setText(label());
        }
        else
            d.m_toolButton->setToolButtonStyle(Qt::ToolButtonIconOnly);

        d.m_toolButton->setWordWrap(false);
    }
    else if (sizeDefinition(size)->imageSize() == ARibbonControlSizeDefinition::ImageNone)
    {
        d.m_toolButton->setLargeIcon(false);
        d.m_toolButton->setIcon(QIcon());
        d.m_toolButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
        d.m_toolButton->setText(label());
        d.m_toolButton->setWordWrap(false);
    }
}

void ARibbonButtonControl::actionChanged()
{
    if (defaultAction() != nullptr)
    {
        if (!defaultAction()->iconText().isEmpty())
            setLabel(defaultAction()->iconText());
        else 
            setLabel(defaultAction()->text());

        setLargeIcon(defaultAction()->icon());
        setSmallIcon(defaultAction()->icon());
    }

    ARibbonControl::actionChanged();
}

/*! \reimp */
bool ARibbonButtonControl::event(QEvent* event)
{
    bool result = ARibbonWidgetControl::event(event);

    QEvent::Type type = event->type();
    if (type == QEvent::FontChange || type == QEvent::StyleChange)
    {
        sizeChanged(currentSize());
    }
    return result;
}


/* ARibbonCheckBoxControl */
ARibbonCheckBoxControl::ARibbonCheckBoxControl(const QString& text, ARibbonGroup* parentGroup)
    : ARibbonWidgetControl(parentGroup, true/*ignoreActionSettings*/)
{
    QCheckBox* checkBox = new QCheckBox(text, this);
    setContentWidget(checkBox);
}

ARibbonCheckBoxControl::~ARibbonCheckBoxControl()
{
}

QCheckBox* ARibbonCheckBoxControl::widget() const
{
    return qobject_cast<QCheckBox*>(contentWidget());
}

/* ARibbonCheckBoxControl */
ARibbonRadioButtonControl::ARibbonRadioButtonControl(const QString& text, ARibbonGroup* parentGroup)
    : ARibbonWidgetControl(parentGroup, true/*ignoreActionSettings*/)
{
    QRadioButton* radioButton = new QRadioButton(text, this);
    setContentWidget(radioButton);
}

ARibbonRadioButtonControl::~ARibbonRadioButtonControl()
{
}

QRadioButton* ARibbonRadioButtonControl::widget() const
{
    return qobject_cast<QRadioButton*>(contentWidget());
}

APROCH_NAMESPACE_END