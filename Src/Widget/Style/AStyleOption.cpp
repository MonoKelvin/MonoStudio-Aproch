/****************************************************************************
 * @file    AStyleOption.cpp
 * @date    2023-07-01
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
#include "AStyleOption.h"
#include <QApplication>

//
// The most of the following code is copied from Qtitan.
//
// Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
// Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
//

APROCH_NAMESPACE_BEGIN

/* AFrameStyleOption */
AFrameStyleOption::AFrameStyleOption()
    : QStyleOptionFrame(AFrameStyleOption::Version), section(Qt::NoSection)
{
}

AFrameStyleOption::AFrameStyleOption(const AFrameStyleOption &other) : QStyleOptionFrame(AFrameStyleOption::Version)
{
    *this = other;
}

/* ABackstageButtonStyleOption */
ABackstageButtonStyleOption::ABackstageButtonStyleOption()
    : QStyleOptionToolButton(ABackstageButtonStyleOption::Version), tabStyle(false), document(nullptr)
{
}

ABackstageButtonStyleOption::ABackstageButtonStyleOption(const ABackstageButtonStyleOption &other) : QStyleOptionToolButton(ABackstageButtonStyleOption::Version)
{
    *this = other;
}

/* ARibbonBackstageStyleOption */
ARibbonBackstageStyleOption::ARibbonBackstageStyleOption()
    : QStyleOption(ARibbonBackstageStyleOption::Version), menuWidth(-1)
{
}

ARibbonBackstageStyleOption::ARibbonBackstageStyleOption(const ARibbonBackstageStyleOption &other) : QStyleOption(ARibbonBackstageStyleOption::Version)
{
    *this = other;
}

/* ARibbonStyleOption */
ARibbonStyleOption::ARibbonStyleOption()
    : QStyleOption(ARibbonStyleOption::Version), titleBarHeight(0), minimized(false), isBackstageVisible(false), tabBarPosition(1), quickAccessBarPosition(1)
{
}

ARibbonStyleOption::ARibbonStyleOption(const ARibbonStyleOption &other) : QStyleOption(ARibbonStyleOption::Version)
{
    *this = other;
}

/* ASystemToolButtonStyleOption */
ASystemToolButtonStyleOption::ASystemToolButtonStyleOption()
    : QStyleOptionToolButton(ASystemToolButtonStyleOption::Version)
{
}

ASystemToolButtonStyleOption::ASystemToolButtonStyleOption(const ASystemToolButtonStyleOption &other)
    : QStyleOptionToolButton(ASystemToolButtonStyleOption::Version)
{
    *this = other;
}

/* ATitleBarStyleOption */
ATitleBarStyleOption::ATitleBarStyleOption()
    : QStyleOptionTitleBar(ATitleBarStyleOption::Version), isBackstageVisible(false), menuWidth(0), alignment(0)
{
}

ATitleBarStyleOption::ATitleBarStyleOption(const ATitleBarStyleOption &other)
    : QStyleOptionTitleBar(ATitleBarStyleOption::Version)
{
    *this = other;
}

/* ACaptionButtonStyleOption */
ACaptionButtonStyleOption::ACaptionButtonStyleOption()
    : QStyleOptionToolButton(ACaptionButtonStyleOption::Version)
{
}

/*! ARibbonQuickAccessBarStyleOption */
ARibbonQuickAccessBarStyleOption::ARibbonQuickAccessBarStyleOption()
    : QStyleOption(ARibbonQuickAccessBarStyleOption::Version), quickAccessBarPosition(1), roundButton(false)
{
}

ARibbonQuickAccessBarStyleOption::ARibbonQuickAccessBarStyleOption(const ARibbonQuickAccessBarStyleOption &other)
    : QStyleOption(ARibbonQuickAccessBarStyleOption::Version)
{
    *this = other;
}

/* ARibbonGroupStyleOption */
ARibbonGroupStyleOption::ARibbonGroupStyleOption()
    : QStyleOption(ARibbonGroupStyleOption::Version), widthOptButton(0), heightCaption(0), text(QString()), icon(QIcon()), iconSize(QSize(32, 32)), subControls(QStyle::SC_ToolButton), features(QStyleOptionToolButton::HasMenu), textAlignment(Qt::AlignCenter), textElideMode(Qt::ElideNone)
{
}

ARibbonGroupStyleOption::ARibbonGroupStyleOption(const ARibbonGroupStyleOption &other) : QStyleOption(ARibbonGroupStyleOption::Version)
{
    *this = other;
}

/* ARibbonFrameMenuStyleOption */
ARibbonFrameMenuStyleOption::ARibbonFrameMenuStyleOption()
    : QStyleOption(ARibbonFrameMenuStyleOption::Version), isAdditionalPages(false), actionsWidth(0), actionsHeight(0), margins()
{
}

ARibbonFrameMenuStyleOption::ARibbonFrameMenuStyleOption(const ARibbonFrameMenuStyleOption &other)
    : QStyleOption(ARibbonFrameMenuStyleOption::Version)
{
    *this = other;
}

/* ARibbonGalleryItemStyleOption */
ARibbonGalleryItemStyleOption::ARibbonGalleryItemStyleOption()
    : QStyleOption(ARibbonGalleryItemStyleOption::Version), separator(false), rectItem(QRect()), caption(), icon()
{
}

ARibbonGalleryItemStyleOption::ARibbonGalleryItemStyleOption(const ARibbonGalleryItemStyleOption &other)
    : QStyleOption(ARibbonGalleryItemStyleOption::Version)
{
    *this = other;
}

/* AStyleOptionMenuScrollItem */
AStyleOptionMenuScrollItem::AStyleOptionMenuScrollItem()
    : QStyleOptionMenuItem(AStyleOptionMenuScrollItem::Version)
{
}

AStyleOptionMenuScrollItem::AStyleOptionMenuScrollItem(const AStyleOptionMenuScrollItem &other)
    : QStyleOptionMenuItem(AStyleOptionMenuScrollItem::Version)
{
    *this = other;
}

/* AHintReturnSysFrameMenuStyleOption */
AHintReturnSysFrameMenuStyleOption::AHintReturnSysFrameMenuStyleOption(const QMargins &mrgs)
    : QStyleHintReturn(), margins(mrgs)
{
}

/* AAutoHideWindowStyleOption */
AAutoHideWindowStyleOption::AAutoHideWindowStyleOption()
    : QStyleOption(AAutoHideWindowStyleOption::Version), position(ELayoutArea::LeftArea)
{
}

AAutoHideWindowStyleOption::AAutoHideWindowStyleOption(const AAutoHideWindowStyleOption &other)
    : QStyleOption(AAutoHideWindowStyleOption::Version)
{
    *this = other;
}

/* ADockTitleBarStyleOption */
ADockTitleBarStyleOption::ADockTitleBarStyleOption()
    : QStyleOption(ADockTitleBarStyleOption::Version), lineRect(), closable(false), movable(false), floatable(false), autohide(false), menu(false), verticalTitleBar(false)
{
}

ADockTitleBarStyleOption::ADockTitleBarStyleOption(const ADockTitleBarStyleOption& other)
    : QStyleOption(ADockTitleBarStyleOption::Version)
{
    *this = other;
}

/* ADockMarkerStyleOption */
ADockMarkerStyleOption::ADockMarkerStyleOption()
    : QStyleOption(ADockMarkerStyleOption::Version), sp(static_cast<QStyle::StandardPixmap>(ACommonStyle::SP_DockMarkerBasis)), position(), basis(false)
{
}

ADockMarkerStyleOption::ADockMarkerStyleOption(const ADockMarkerStyleOption &other) : QStyleOption(ADockMarkerStyleOption::Version)
{
    *this = other;
}

/* ADockPhantomStyleOption */
ADockPhantomStyleOption::ADockPhantomStyleOption()
    : QStyleOption(ADockPhantomStyleOption::Version), drawFrame(false), posTab(ELayoutArea::DefaultArea), borderWidth(7)
{
}

ADockPhantomStyleOption::ADockPhantomStyleOption(const ADockPhantomStyleOption &other)
    : QStyleOption(ADockPhantomStyleOption::Version)
{
    *this = other;
}

/* AToggleSwitchStyleOption */
AToggleSwitchStyleOption::AToggleSwitchStyleOption()
    : QStyleOptionComplex(AToggleSwitchStyleOption::Version), isAnimated(false), dontDrawThumbOn(false), dontDrawThumbOff(false), sizeIndicator(), orientation(Qt::Horizontal), textAlignment(Qt::AlignLeft), subControls(static_cast<ACommonStyle::SubControlEx>(QStyle::SC_None)), shape(AToggleSwitch::Circle), animationOffset(0.0), disabledOpacity(0.4), statusText(), text()
{
}

AToggleSwitchStyleOption::AToggleSwitchStyleOption(const AToggleSwitchStyleOption &other)
    : QStyleOptionComplex(AToggleSwitchStyleOption::Version)
{
    *this = other;
}

/* AStyleOptionTab */
AStyleOptionTab::AStyleOptionTab()
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    : QStyleOptionTab(AStyleOptionTab::Version)
#else
    : QStyleOptionTabV3(AStyleOptionTab::Version)
#endif // (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
      , tabIndex(-1), hoverClose(false)
{
}

AStyleOptionTab::AStyleOptionTab(int version)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    : QStyleOptionTab(version)
#else
    : QStyleOptionTabV3(version)
#endif // (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
      , tabIndex(-1), hoverClose(false)
{
}

/* ARibbonOptionHeaderStyleOption */
ARibbonOptionHeaderStyleOption::ARibbonOptionHeaderStyleOption()
    : AStyleOptionTab(ARibbonOptionHeaderStyleOption::Version), firstTab(false), lastTab(false), minimized(false), tabBarPosition(1), textAlignment(Qt::AlignLeft)
{
}

ARibbonOptionHeaderStyleOption::ARibbonOptionHeaderStyleOption(const ARibbonOptionHeaderStyleOption &other)
    : AStyleOptionTab(ARibbonOptionHeaderStyleOption::Version)
{
    *this = other;
}

/* AStyleOptionPopupTitleBar */
AStyleOptionPopupTitleBar::AStyleOptionPopupTitleBar()
    : QStyleOptionTitleBar(AStyleOptionPopupTitleBar::Version)
{
}

AStyleOptionPopupTitleBar::AStyleOptionPopupTitleBar(const AStyleOptionPopupTitleBar &other)
    : QStyleOptionTitleBar(AStyleOptionPopupTitleBar::Version)
{
    *this = other;
}

/* AGridHeaderStyleOption */
AGridHeaderStyleOption::AGridHeaderStyleOption()
    : QStyleOptionHeader(AGridHeaderStyleOption::Version),
    margins(QMargins()), textElideMode(Qt::ElideRight),
    scaleFactor(1.0), decorationColor(Qt::transparent), textDocument(nullptr)
{
}

AGridHeaderStyleOption::AGridHeaderStyleOption(const QStyleOptionHeader& option)
    : QStyleOptionHeader(option),
    margins(QMargins()), textElideMode(Qt::ElideRight),
    scaleFactor(1.0), decorationColor(Qt::transparent), textDocument(nullptr)
{
    version = AGridHeaderStyleOption::Version;
}

AGridHeaderStyleOption::AGridHeaderStyleOption(const AGridHeaderStyleOption& other)
    : QStyleOptionHeader(AGridHeaderStyleOption::Version)
{
    *this = other;
}

APROCH_NAMESPACE_END
