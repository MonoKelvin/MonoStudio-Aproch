/****************************************************************************
 * @file    AStyleOption.h
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
#pragma once
#include <QStyleOption>
#include <QStyleOptionFrame>
#include "ACommonStyle.h"
#include "Widget/AToggleSwitch.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

class QTextDocument;

APROCH_NAMESPACE_BEGIN

// to support Qt4
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
typedef QStyleOptionFrame QtStyleOptionFrame;
#else
typedef QStyleOptionFrameV3 QtStyleOptionFrame;
#endif

#define A_OPT_VERSION 100

class APROCH_API AFrameStyleOption : public QStyleOptionFrame
{
public:
    enum StyleOptionVersion { Version = QStyleOptionFrame::Version + A_OPT_VERSION };
    AFrameStyleOption();
    AFrameStyleOption(const AFrameStyleOption& other);
    AFrameStyleOption& operator=(const AFrameStyleOption&) = default;
public:
    Qt::WindowFrameSection section;
};

class APROCH_API ABackstageButtonStyleOption : public QStyleOptionToolButton
{
public:
    enum StyleOptionVersion { Version = QStyleOptionToolButton::Version + A_OPT_VERSION };
    ABackstageButtonStyleOption();
    ABackstageButtonStyleOption(const ABackstageButtonStyleOption& other);
    ABackstageButtonStyleOption& operator=(const ABackstageButtonStyleOption&) = default;
public:
    bool tabStyle;
    QTextDocument* document;
};

class APROCH_API ARibbonBackstageStyleOption : public QStyleOption
{
public:
    enum StyleOptionVersion { Version = QStyleOption::Version + A_OPT_VERSION };
    ARibbonBackstageStyleOption();
    ARibbonBackstageStyleOption(const ARibbonBackstageStyleOption& other);
    ARibbonBackstageStyleOption& operator=(const ARibbonBackstageStyleOption&) = default;
public:
    int menuWidth;
};

class APROCH_API ARibbonStyleOption : public QStyleOption
{
public:
    enum StyleOptionVersion { Version = QStyleOption::Version + A_OPT_VERSION };
    ARibbonStyleOption();
    ARibbonStyleOption(const ARibbonStyleOption& other);
    ARibbonStyleOption& operator=(const ARibbonStyleOption&) = default;
public:
    QRect rectTabBar;
    QRect rectCurrentTab;
    int titleBarHeight;
    bool minimized;
    bool isBackstageVisible;
    uint tabBarPosition;
    uint quickAccessBarPosition;
    QColor contextColor;
};

class APROCH_API ASystemToolButtonStyleOption : public QStyleOptionToolButton
{
public:
    enum StyleOptionVersion { Version = QStyleOptionToolButton::Version + A_OPT_VERSION };
    ASystemToolButtonStyleOption();
    ASystemToolButtonStyleOption(const ASystemToolButtonStyleOption& other);
    ASystemToolButtonStyleOption& operator=(const ASystemToolButtonStyleOption&) = default;
};

class APROCH_API ATitleBarStyleOption : public QStyleOptionTitleBar
{
public:
    enum StyleOptionVersion { Version = QStyleOptionTitleBar::Version + A_OPT_VERSION };
    struct ContextualTabInfo
    {
        ContextualTabInfo() {}
        ContextualTabInfo(const QRect& _rect, const QString& _title, const QColor& _color)
            : rect(_rect)
            , title(_title)
            , color(_color) {}
        QRect rect;
        QString title;
        QColor color;
    };
    ATitleBarStyleOption();
    ATitleBarStyleOption(const ATitleBarStyleOption& other);
    ATitleBarStyleOption& operator=(const ATitleBarStyleOption&) = default;
public:
    QRect titleRect;
    bool isBackstageVisible;
    int menuWidth;
    QPalette backstageFrameMenuPalette;
    int alignment;
    QRect contextualAreaRect;
    QVector<ContextualTabInfo> contextualTabs;
    QFont contextFont;
    QFont titleFont;
    ASystemToolButtonStyleOption optButton;
protected:
};

class APROCH_API ACaptionButtonStyleOption : public QStyleOptionToolButton
{
public:
    enum StyleOptionVersion { Version = QStyleOptionToolButton::Version + A_OPT_VERSION };
    ACaptionButtonStyleOption();
};

class APROCH_API ARibbonQuickAccessBarStyleOption : public QStyleOption
{
public:
    enum StyleOptionVersion { Version = QStyleOption::Version + A_OPT_VERSION };
    ARibbonQuickAccessBarStyleOption();
    ARibbonQuickAccessBarStyleOption(const ARibbonQuickAccessBarStyleOption& other);
    ARibbonQuickAccessBarStyleOption& operator=(const ARibbonQuickAccessBarStyleOption&) = default;
public:
    uint quickAccessBarPosition;
    bool roundButton;
};

class APROCH_API ARibbonGroupStyleOption : public QStyleOption
{
public:
    enum StyleOptionVersion { Version = QStyleOption::Version + A_OPT_VERSION };
    ARibbonGroupStyleOption();
    ARibbonGroupStyleOption(const ARibbonGroupStyleOption& other);
    ARibbonGroupStyleOption& operator=(const ARibbonGroupStyleOption&) = default;
public:
    int widthOptButton;
    int heightCaption;
    QString text;
    QIcon icon;
    QSize iconSize;
    QStyle::SubControls subControls;
    QStyleOptionToolButton::ToolButtonFeatures features;
    Qt::Alignment textAlignment;
    Qt::TextElideMode textElideMode;
    QColor contextColor;
};

class APROCH_API ARibbonFrameMenuStyleOption : public QStyleOption
{
public:
    enum StyleOptionVersion { Version = QStyleOption::Version + A_OPT_VERSION };
    ARibbonFrameMenuStyleOption();
    ARibbonFrameMenuStyleOption(const ARibbonFrameMenuStyleOption& other);
    ARibbonFrameMenuStyleOption& operator=(const ARibbonFrameMenuStyleOption&) = default;
public:
    bool isAdditionalPages;
    int actionsWidth;
    int actionsHeight;
    QMargins margins;
};

class APROCH_API ARibbonGalleryItemStyleOption : public QStyleOption
{
public:
    enum StyleOptionVersion { Version = QStyleOption::Version + A_OPT_VERSION };
    ARibbonGalleryItemStyleOption();
    ARibbonGalleryItemStyleOption(const ARibbonGalleryItemStyleOption& other);
    ARibbonGalleryItemStyleOption& operator=(const ARibbonGalleryItemStyleOption&) = default;
public:
    bool separator;
    QRect rectItem;
    QString caption;
    QIcon icon;
};

class APROCH_API AStyleOptionMenuScrollItem : public QStyleOptionMenuItem
{
public:
    enum StyleOptionVersion { Version = QStyleOptionMenuItem::Version + A_OPT_VERSION };
    AStyleOptionMenuScrollItem();
    AStyleOptionMenuScrollItem(const AStyleOptionMenuScrollItem& other);
    AStyleOptionMenuScrollItem& operator=(const AStyleOptionMenuScrollItem&) = default;
public:
    QString toolTip;
};

class APROCH_API AHintReturnSysFrameMenuStyleOption : public QStyleHintReturn
{
public:
    AHintReturnSysFrameMenuStyleOption(const QMargins& mrgs);
public:
    QMargins margins;
};

class APROCH_API AAutoHideWindowStyleOption : public QStyleOption
{
public:
    enum StyleOptionVersion { Version = QStyleOption::Version + A_OPT_VERSION };
    AAutoHideWindowStyleOption();
    AAutoHideWindowStyleOption(const AAutoHideWindowStyleOption& other);
    AAutoHideWindowStyleOption& operator=(const AAutoHideWindowStyleOption&) = default;
public:
    ELayoutArea position;
};

class APROCH_API ADockTitleBarStyleOption : public QStyleOption
{
public:
    enum StyleOptionVersion { Version = QStyleOption::Version + A_OPT_VERSION };
    ADockTitleBarStyleOption();
    ADockTitleBarStyleOption(const ADockTitleBarStyleOption& other);
    ADockTitleBarStyleOption& operator=(const ADockTitleBarStyleOption&) = default;
public:
    QIcon icon;
    QString title;
    QRect rcText;
    QRect rcIcon;
    QRect lineRect;
    bool closable;
    bool movable;
    bool floatable;
    bool autohide;
    bool menu;
    bool verticalTitleBar;
};

class APROCH_API ADockMarkerStyleOption : public QStyleOption
{
public:
    enum StyleOptionVersion { Version = QStyleOption::Version + A_OPT_VERSION };
    ADockMarkerStyleOption();
    ADockMarkerStyleOption(const ADockMarkerStyleOption& other);
    ADockMarkerStyleOption& operator=(const ADockMarkerStyleOption&) = default;
public:
    QStyle::StandardPixmap sp;
    QPointF position;
    bool basis;
};

class APROCH_API ADockPhantomStyleOption : public QStyleOption
{
public:
    enum StyleOptionVersion { Version = QStyleOption::Version + A_OPT_VERSION };
    ADockPhantomStyleOption();
    ADockPhantomStyleOption(const ADockPhantomStyleOption& other);
    ADockPhantomStyleOption& operator=(const ADockPhantomStyleOption&) = default;
public:
    bool drawFrame;
    ELayoutArea posTab;
    int borderWidth;
    QRect rectTab;
};

class APROCH_API AToggleSwitchStyleOption : public QStyleOptionComplex
{
public:
    enum StyleOptionVersion { Version = QStyleOption::Version + A_OPT_VERSION };
    AToggleSwitchStyleOption();
    AToggleSwitchStyleOption(const AToggleSwitchStyleOption& other);
    AToggleSwitchStyleOption& operator=(const AToggleSwitchStyleOption&) = default;
public:
    bool isAnimated;
    bool dontDrawThumbOn;
    bool dontDrawThumbOff;
    QSize sizeIndicator;
    Qt::Orientation orientation;
    Qt::Alignment textAlignment;
    ACommonStyle::SubControlsEx subControls;
    AToggleSwitch::Shape shape;
    qreal animationOffset;
    qreal disabledOpacity;
    QString statusText;
    QString text;
};

class APROCH_API AStyleOptionTab
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    : public QStyleOptionTab
#else
    : public QStyleOptionTabV3
#endif // (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
{
public:
    enum StyleOptionVersion { Version = QStyleOptionTab::Version + A_OPT_VERSION };
    AStyleOptionTab();
public:
    QPixmap previewPixmap;
    QRect previewRect;
    int tabIndex;
    bool hoverClose;
protected:
    AStyleOptionTab(int version);
};

class APROCH_API ARibbonOptionHeaderStyleOption : public AStyleOptionTab
{
public:
    enum StyleOptionVersion { Version = AStyleOptionTab::Version + A_OPT_VERSION };
    ARibbonOptionHeaderStyleOption();
    ARibbonOptionHeaderStyleOption(const ARibbonOptionHeaderStyleOption& other);
    ARibbonOptionHeaderStyleOption& operator=(const ARibbonOptionHeaderStyleOption&) = default;
public:
    bool firstTab;
    bool lastTab;
    bool minimized;
    QRect rcFirst;
    QRect rcLast;
    QColor contextColor;
    uint tabBarPosition;
    Qt::Alignment textAlignment;
};

class APROCH_API AStyleOptionPopupTitleBar : public QStyleOptionTitleBar
{
public:
    enum StyleOptionVersion { Version = QStyleOptionTitleBar::Version + A_OPT_VERSION };
    AStyleOptionPopupTitleBar();
    AStyleOptionPopupTitleBar(const AStyleOptionPopupTitleBar& other);
    AStyleOptionPopupTitleBar& operator=(const AStyleOptionPopupTitleBar&) = default;
public:
    QPixmap pixmapCloseButton;
protected:
};

#define A_HEADER_OPTION_PROPERTY "aproch_header_style_option"
class APROCH_API AGridHeaderStyleOption : public QStyleOptionHeader
{
public:
    enum StyleOptionVersion { Version = QStyleOptionHeader::Version + A_OPT_VERSION };
    AGridHeaderStyleOption();
    AGridHeaderStyleOption(const QStyleOptionHeader& option);
    AGridHeaderStyleOption(const AGridHeaderStyleOption& other);
    AGridHeaderStyleOption& operator=(const AGridHeaderStyleOption&) = default;
public:
    QMargins margins;
    Qt::TextElideMode textElideMode;
    double scaleFactor;
    QColor decorationColor;
    QTextDocument* textDocument;
};

APROCH_NAMESPACE_END