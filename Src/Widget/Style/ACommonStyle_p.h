/****************************************************************************
 * @file    ACommonStyle_p.h
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
#include <QTime>
#include <QBasicTimer>
#include <QScreen>
#include <QColormap>
#include <QPainter>
#include <QApplication>
#include <QXmlStreamReader>
#include <QVariantAnimation>

#include "Platform/APlatform.h"
#include "AStyleOption.h"
#include "Widget/Style/ACommonStyle.h"
#include "ASegoeMDL2Assets.h"

#ifdef Q_OS_MAC
#define APROCH_MAC_TITLEBAR
#endif
#ifdef Q_OS_LINUX
#define APROCH_MAC_TITLEBAR
#endif

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

#define _aproch_PopupBar                  "PopupBar"
#define _aproch_SplitActionPopup          "SplitActionPopup_"
#define _aproch_StyleName                 "StyleName" 
#define _aproch_PopupButtonGallery        "PopupButtonGallery"
#define _aproch_ScrollUpButtonGallery     "ScrollUpButtonGallery"
#define _aproch_ScrollDownButtonGallery   "ScrollDownButtonGallery"
#define _aproch_SingleTheme               "Single"
#define _aproch_WidgetGallery             "WidgetGallery"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
#define APROCH_FALLTHROUGH() Q_FALLTHROUGH()
#else
#define APROCH_FALLTHROUGH()
#endif

#define APROCH_DEPRECATED_CODE 1

class APROCH_API AAnimation : public QObject
{
public:
    AAnimation() : m_widget(nullptr), m_running(true)
    {
    }
    virtual ~AAnimation();
public:
    QWidget* widget() const
    {
        return m_widget;
    }
    bool running() const
    {
        return m_running;
    }
    const QTime& startTime() const
    {
        return m_startTime;
    }
    void setRunning(bool val)
    {
        m_running = val;
    }
    void setWidget(QWidget* widget);
    void setStartTime(const QTime& startTime)
    {
        m_startTime = startTime;
    }
public:
    virtual void paint(QPainter* painter, const QStyleOption* option);
protected:
    void drawBlendedImage(QPainter* painter, QRect rect, qreal value);
    bool eventFilter(QObject* obj, QEvent* event) override;
protected:
    QTime m_startTime;
    QWidget* m_widget;
    QImage m_primaryImage;
    QImage m_secondaryImage;
    QImage m_tempImage;
    bool m_running;
};

class APROCH_API ATransition : public AAnimation
{
public:
    ATransition();
    ~ATransition() override;
public:
    void setDuration(int duration)
    {
        m_duration = duration;
    }
    void setStartImage(const QImage& image)
    {
        m_primaryImage = image;
    }
    void setEndImage(const QImage& image)
    {
        m_secondaryImage = image;
    }
    void paint(QPainter* painter, const QStyleOption* option) override;
    int duration() const
    {
        return m_duration;
    }
public:
    int m_duration;
};

class AClickLightAnimation : public QVariantAnimation
{
    Q_OBJECT
public:
    AClickLightAnimation(QWidget* w);
    ~AClickLightAnimation() override;
    void reset();
    static void startClickAnimation(QWidget* w);
public slots:
    void animationChanged(const QVariant& value);
    void animationFinished();
protected:
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    virtual void updateCurrentValue(const QVariant&)
    {
    }
#endif
private:
    QWidget* m_w;
};

typedef QHash<QPalette::ColorRole, QBrush> RoleBrushHash;
typedef QHash<QPalette::ColorGroup, RoleBrushHash> PaletteHash;
typedef QHash<QString, PaletteHash> SubControlHash;

class AbstractData
{
};

class APixelMetricData : public AbstractData
{
public:
    APixelMetricData();
public:
    uint value;
    bool scalableDpi;
};

class AWidgetData : public AbstractData
{
public:
    AWidgetData() : m_paletteHash(), m_subControlHash(), hover(-1), autoFillBackground(-1)
    {
    }
public:
    PaletteHash m_paletteHash;
    SubControlHash m_subControlHash;
    int hover;
    int autoFillBackground;
};

typedef QHash<uint, QColor> ColorHash;
typedef QHash<QString, int> StyleHintHash;
typedef QHash<QString, StyleHintHash> WidgetStyleHintHash;
typedef QHash<QString, QStringList> StandardPixmapHash;
typedef QHash<QString, StandardPixmapHash> WidgetPixmapHash;
typedef QHash<QString, APixelMetricData> PixelMetricHash;
typedef QHash<QString, PixelMetricHash> WidgetPixelMetricHash;
typedef QHash<QString, AWidgetData> WidgetDataHash;

class AStyleConfig;

class AStyleConfigReader
{
public:
    AStyleConfigReader(AStyleConfig* config, const QString& pathRes, const QString& themeName);
public:
    bool read(QIODevice* device);
    QString errorString() const;
protected:
    void readStyle();
    void readWidget();
    void readGlobal();

    void readStandardPixmap();
    void readWidgetPixmap(StandardPixmapHash& pixmapHash);
    void readPixelMetric(PixelMetricHash& pixelMetric);
    void readStyleHint(StyleHintHash& styleHint);
    void readSubControl(SubControlHash& subControlHash);

    void readPalette(PaletteHash& hashPalette);
    void readColors(PaletteHash& hashPalette);
protected:
    QString m_pathRes;
    QString m_themeName;
    QXmlStreamReader xml;
    AStyleConfig* m_config;
private:
    Q_DISABLE_COPY(AStyleConfigReader)
};

/* StyleConfig */
class APROCH_API AStyleConfig
{
public:
    AStyleConfig();
public:
    bool load(const QString& pathRes, const QString& themeName);
    const QByteArray getMetaObjectClassName(const QString& xmlClassName) const;
    QPalette getWidgetPalette(const QString& className) const;
    QPalette getWidgetPalette(QWidget* widget) const;
    void setWidgetAttribute(QWidget* widget, const AWidgetData& data);
#if 0
    void updateWidgetPalette(const QPalette& appPalette);
#endif
    void updateWidgetAttribute(QWidget* wd);
    void setPaletteFromGlobal(QPalette::ColorGroup gr, QPalette::ColorRole cr, QPalette& palette) const;

    int pixelMetric(const QWidget* widget, const QString& strWidget, const QString& numMetric, int defaultVal, bool* ok = nullptr) const;
    int styleHint(const QString& strWidget, const QString& numMetric, int defaultVal) const;

    QColor standardColorPixmap(uint satate) const;
    QString standardPixmapName(const QString& numPixmap, const QWidget* wd) const;
    QString widgetPixmapName(const QString& className, const QString& pixmapId) const;

    QBrush brushSubControl(const QString& className, const QString& subControlId,
                           QPalette::ColorGroup group, QPalette::ColorRole role) const;
    QColor colorSubControl(const QString& className, const QString& subControlId,
                           QPalette::ColorGroup group, QPalette::ColorRole role) const;
    void clear();
protected:
    PaletteHash m_hashGlobalPalette;
    ColorHash m_hashGlobalColorPixmap;
    StandardPixmapHash m_hashGlobalStandardPixmap;
    StyleHintHash m_hashGlobalStyleHint;
    PixelMetricHash m_hashGlobalMetricsData;

    WidgetPixmapHash m_hashWidgetPixmap;
    WidgetStyleHintHash m_hashWidgetStyleHint;
    WidgetPixelMetricHash m_hashWidgetMetricsData;
    WidgetDataHash m_hashWidgetsData;
private:
    Q_DISABLE_COPY(AStyleConfig)
        friend class AStyleConfigReader;
};

/* StandardSymbolIcon */
class AStandardSymbolIcon
{
public:
    AStandardSymbolIcon();
public:
    inline static SegoeMDL2Font::IconSymbol symbolStandardPixmap(QStyle::StandardPixmap sp)
    {
        switch (sp)
        {
        case QStyle::SP_TitleBarMinButton:    return SegoeMDL2Font::ChromeMinimize;
        case QStyle::SP_TitleBarMaxButton:    return SegoeMDL2Font::ChromeMaximize;
        case QStyle::SP_TitleBarCloseButton:  return SegoeMDL2Font::ChromeClose;
        case QStyle::SP_TitleBarNormalButton: return SegoeMDL2Font::ChromeRestore;
        case QStyle::SP_TitleBarShadeButton: return SegoeMDL2Font::ChromeRestore;
        case QStyle::SP_TitleBarUnshadeButton: return SegoeMDL2Font::ChromeMinimize;
        case QStyle::SP_TitleBarContextHelpButton: return SegoeMDL2Font::Help;
        case QStyle::SP_ArrowDown:            return SegoeMDL2Font::ChevronDown;
        default: break;
        }

        switch (static_cast<ACommonStyle::StandardPixmapEx>(sp))
        {
        case ACommonStyle::SP_TitleBarPinButton:        return SegoeMDL2Font::Pin;
        case ACommonStyle::SP_TitleBarPinFillButton:    return SegoeMDL2Font::PinFill;
        case ACommonStyle::SP_TitleBarPinnedButton:     return SegoeMDL2Font::Pinned;
        case ACommonStyle::SP_TitleBarPinnedFillButton: return SegoeMDL2Font::PinnedFill;
        case ACommonStyle::SP_NavigationBackButton:     return SegoeMDL2Font::ChromeBack;
        default: break;
        }
        return SegoeMDL2Font::None;
    }
public:
    QPixmap standardSymbolPixmap(QStyle::StandardPixmap sp, const QStyleOption* option, const QWidget* widget) const;
};

class ACommonPaintManager;

class APROCH_API ACommonStylePrivate : public QObject
{
    Q_OBJECT
public:
    A_DECLARE_PUBLIC(ACommonStyle);
    enum
    {
        InvalidMetric = -23576
    };
    enum
    {
        InvalidHint = -23576
    };
    enum ImageState
    {
        Black,
        Gray,
        LtGray,
        White,
        DkGray,
        Black2,
        Color0,
        Color1,
        Color2,
        Color3,
        InvalidState,
    };
public:
    explicit ACommonStylePrivate();
    ~ACommonStylePrivate() override;
public:
    virtual void init();
    void updateStyle();
    void updateStandardPixmap(const QString& indxIcon, const QWidget* wd);
    void setPaletteFromGlobal(QPalette::ColorGroup gr, QPalette::ColorRole cr, QPalette& palette) const;
    QPixmap standardPixmap(const QString& indxIcon, ImageState state, const QWidget* wd) const;
    QString widgetPixmapName(const QString& className, const QString& pixmapId) const;
    virtual void initStandardPalette(QPalette& palette) const;
public: // useful functions
    static bool isUse96Dpi();
    static int pixelMetricFromSystemDp(QStyle::PixelMetric pm, const QStyleOption* option = nullptr, const QWidget* widget = nullptr);
    static QPixmap loadIcon(const QString& name, const QWidget* wd);
    static bool buttonVisible(const QStyle::SubControl sc, const QStyleOptionTitleBar* tb);
    static int dpiScaled(int value, const QWidget* widget);
    static qreal dpiScaledF(qreal value, const QWidget* widget);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    //static qreal dpiScaled(qreal value, const QStyleOption* option);
#endif
    static QPixmap preparePixmap(const QIcon& icon, const QSize& iconSize, QIcon::Mode mode, QIcon::State state, const QWidget* wd);
    QPixmap loadIcon(const QString& icon, const QColor& color, const QWidget* wd) const;
    void beginUpdate(bool polishNeeded = true);
    void endUpdate(bool polishNeeded = true);
public:
    virtual bool isAprochStyle() const;
    virtual QPalette getWidgetPalette(QWidget* widget) const;
    virtual QFont getWidgetFont(const QString& className) const;
    QFont getWidgetFont(QWidget* widget) const;
    // draw PrimitiveElement
    virtual bool drawHighDpiDockMarker(const QStyleOption* opt, QPainter* painter, const QWidget* widget) const;
    virtual bool drawHighDpiBasisDockMarker(const QStyleOption* opt, QPainter* painter, const QWidget* widget) const;
    virtual bool drawHighDpiBasisCenterDockMarker(const QStyleOption* opt, QPainter* painter, const QWidget* widget) const;
    virtual bool drawHighDpiAreaDockMarker(const QStyleOption* opt, QPainter* painter, const QWidget* widget) const;
    virtual bool drawHighDpiWindowDockMarker(const QStyleOption* opt, QPainter* painter, const QWidget* widget) const;
    virtual bool drawHighDpiWindowClientDockMarker(const QStyleOption* opt, QPainter* painter, const QWidget* widget) const;
    virtual bool drawHighDpiArrowDockMarker(const QStyleOption* opt, QPainter* painter, const QWidget* widget) const;

    // for standard controls
    // draw PrimitiveElement
    virtual bool drawFrame(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawFrameWindow(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawPanelMenu(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawIndicatorArrow(QStyle::PrimitiveElement pe, const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawFrameGroupBox(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawIndicatorCheckBox(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawIndicatorItemCheck(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawIndicatorRadioButton(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawIndicatorMenuCheckMark(const QStyleOption*, QPainter*, const QWidget*) const;
    virtual bool drawIndicatorButtonDropDown(const QStyleOption*, QPainter*, const QWidget*) const;
    virtual bool drawIndicatorTabClose(const QStyleOption*, QPainter*, const QWidget*) const;
    virtual bool drawIndicatorToolBarHandle(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawIndicatorToolBarSeparator(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawIndicatorHeaderArrow(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawIndicatorBranch(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawDockMarker(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawDockWidgetPhantom(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawFrameFocusRect(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawFrameDockWidget(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawFrameMenu(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawFrameTabBarBase(const QStyleOption*, QPainter*, const QWidget*) const;
    virtual bool drawPanelItemViewItem(const QStyleOption* opt, QPainter* p, const QWidget* widget) const;
    virtual bool drawPanelTipLabel(const QStyleOption*, QPainter*, const QWidget*) const;
    // PE - ButtonTool
    virtual bool drawPanelButtonTool(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawGroupControlEntry(const QStyleOption*, QPainter*, const QWidget*) const;
    virtual bool drawPanelButtonCommand(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawFrameLineEdit(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawPanelLineEdit(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawFrameStatusBarItem(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawPanelStatusBar(const QStyleOption*, QPainter*, const QWidget*) const;
    // draw ComplexControl
    virtual bool drawTitleBar(const QStyleOptionComplex* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawMdiControls(const QStyleOptionComplex* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawComboBox(const QStyleOptionComplex* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawSlider(const QStyleOptionComplex* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawSpinBox(const QStyleOptionComplex* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawToolButton(const QStyleOptionComplex* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawScrollBar(const QStyleOptionComplex* opt, QPainter* painter, const QWidget* w) const;
    // draw ControlElement
    virtual bool drawMenuBarEmptyArea(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawShapedFrame(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawMenuBarItem(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawMenuItem(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawTabBarTabLabel(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawItemViewItem(const QStyleOption* opt, QPainter* p, const QWidget* widget) const;
    virtual bool drawSplitter(const QStyleOption* opt, QPainter* p, const QWidget* widget) const;
    virtual bool drawProgressBarContents(const QStyleOption* opt, QPainter* p, const QWidget* widget) const;
    virtual bool drawProgressBarLabel(const QStyleOption* opt, QPainter* p, const QWidget* widget) const;
    virtual bool drawGroupBox(const QStyleOption* opt, QPainter* p, const QWidget* widget) const;
    virtual bool drawToolBoxTabShape(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawToolButtonLabel(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawSizeGrip(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawToolBar(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual void drawMenuItemSeparator(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    // CE - draw components ScrollBar
    virtual bool drawScrollBarAddSubLine(QStyle::ControlElement ce, const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawScrollBarAddSubPage(QStyle::ControlElement ce, const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawScrollBarSlider(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawTabBarTabShape(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawHeader(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawHeaderSection(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawHeaderLabel(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    virtual bool drawDockWidgetTitle(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    // for RibbonBar
    // draw PrimitiveElement
    virtual bool drawRibbonBar(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonFrameGroups(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonFrameGroup(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonFrameGallery(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonSysFrameMenu(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonSysFramePagePopupList(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonSysFramePagePopup(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonPanelButton(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonOptionButton(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonQuickAccessButton(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonBackstageFrame(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonBackstagePanelButton(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonBackstageCloseButton(const QStyleOption* opt, QPainter* p, const QWidget* widget) const;
    virtual bool drawRibbonGroupScrollButton(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonPopupBarButton(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawWorkspace(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawPanelButtonBevel(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonSliderButton(const QStyleOption* opt, QPainter* p, const QWidget* w = nullptr) const;
    virtual bool drawKeyTip(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;

    // draw ControlElement
    virtual bool drawRibbonTabBar(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonTabShapeLabel(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonTab(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonGroupLabel(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonSysMenuItem(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonSysRecentFileItem(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonButtonLabel(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonGalleryItem(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonBackstageMenuItem(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonBackstageButtonLabel(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonReducedGroup(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawOfficePopupSizeGrip(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonTitleBarButton(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawNavigationBackButton(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;

    // draw ComplexControl
    virtual bool drawRibbonFileButton(const QStyleOptionComplex* opt, QPainter* painter, const QWidget* widget = nullptr) const;
    virtual bool drawRibbonButton(const QStyleOptionComplex* opt, QPainter* painter, const QWidget* widget = nullptr) const;
    virtual bool drawRibbonTitleBarWidget(const QStyleOptionComplex* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonGalleryButton(const QStyleOptionComplex* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawRibbonBackstageButton(const QStyleOptionComplex* opt, QPainter* painter, const QWidget* w = nullptr) const;
    virtual bool drawToggleSwitch(const QStyleOptionComplex* opt, QPainter* painter, const QWidget* w = nullptr) const;
    // subControlRect
    virtual QRect ribbonSubControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex* opt, QStyle::SubControl sc, const QWidget* widget = nullptr) const;
    virtual QSize ribbonSizeFromContents(QStyle::ContentsType ct, const QStyleOption* opt, const QSize& csz, const QWidget* widget = nullptr) const;
    // subControlRect
    virtual QRect ribbonSubElementRect(QStyle::SubElement se, const QStyleOption* option, const QWidget* widget = nullptr) const;
    virtual QRect navigationEdgeSubElementRect(QStyle::SubElement se, const QStyleOption* option, const QWidget* widget = nullptr) const;
    virtual QRect dockSubElementRect(QStyle::SubElement se, const QStyleOption* option, const QWidget* widget = nullptr) const;
    // pixelMetric
    virtual int basePixelMetric(QStyle::PixelMetric metric, const QStyleOption* option, const QWidget* widget = nullptr) const;
    virtual int ribbonPixelMetric(QStyle::PixelMetric metric, const QStyleOption* option, const QWidget* widget = nullptr) const;
    virtual int navigationEdgePixelMetric(QStyle::PixelMetric metric, const QStyleOption* option, const QWidget* widget = nullptr) const;
    virtual int gridPixelMetric(QStyle::PixelMetric metric, const QStyleOption* option, const QWidget* widget = nullptr) const;
    // pixelMetric
    virtual int ribbonStyleHint(QStyle::StyleHint stylehint, const QStyleOption* opt, const QWidget* widget, QStyleHintReturn* returnData) const;
    virtual int dockStyleHint(QStyle::StyleHint stylehint, const QStyleOption* opt, const QWidget* widget, QStyleHintReturn* returnData) const;
    // standardPixmap
    virtual QPixmap ribbonStandardPixmap(QStyle::StandardPixmap sp, const QStyleOption* opt, const QWidget* widget) const;
    // for Docking
    // draw PrimitiveElement
    virtual bool drawDockAutoHideFrameWindow(const QStyleOption* opt, QPainter* painter, const QWidget* w) const;
    // for Dock
    // standardPixmap
    virtual QPixmap dockStandardPixmap(QStyle::StandardPixmap sp, const QStyleOption* opt, const QWidget* widget) const;
    // for Navigation
    // standardPixmap
    virtual QPixmap navigationStandardPixmap(QStyle::StandardPixmap sp, const QStyleOption* opt, const QWidget* widget) const;
protected: // additional rendering functions
    void roundedAngle(QPainterPath& path, const QRectF& rect, qreal xRadius, qreal yRadius, bool drawTopPart) const;
    static void drawLineDPI(QPainter* painter, const QRect& rect, bool left, bool top, bool right, bool bottom);
    void drawIcon(const QPixmap& px, QPainter* painter, const QRect& rect) const;
    virtual void drawHighlightButton(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
    virtual void drawHighlightItemGallery(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
    void drawIconDp(const QPixmap& px, const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
    bool drawIndicatorArrowDp(QStyle::PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w = nullptr) const;
    bool drawToolButtonLabelDp(const QStyleOption* opt, QPainter* painter, const QWidget* w = nullptr) const;
    void drawfillTitleBar(const QStyleOption* option, QPainter* painter, const QWidget* widget, const QColor& frameColor, const QColor& windowColor) const;
protected:
    /*! \internal */
    enum TypePaint
    {
        tp_PrimitiveElement,
        tp_ControlElement,
        tp_ComplexControl,
    };
    bool paintAnimation(TypePaint, int nPrim, const QStyleOption*, QPainter*, const QWidget*, int iDurationOver = 40) const;
    void doTimer();
    void startAnimation(AAnimation*);
    void stopAnimation(const QWidget*);
    AAnimation* widgetAnimation(const QWidget* widget) const;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    void tabLayout(const QStyleOptionTab* opt, const QWidget* widget, QRect* textRect, QRect* iconRect) const;
#else
    void tabLayout(const QStyleOptionTabV3* opt, const QWidget* widget, QRect* textRect, QRect* iconRect) const;
#endif
protected:
    bool widgetStyleSupport(const QWidget* wd) const;
    bool showToolTip(const QPoint& pos, QWidget* w);
    bool canWordWrap(const QRect& rect, const QFontMetrics& fm, const QString& str, int* heightLine) const;
    virtual QBrush brushSubControl(const QString& className, const QString& subControlId, QPalette::ColorGroup group, QPalette::ColorRole role) const;
    virtual QColor colorSubControl(const QString& className, const QString& subControlId, QPalette::ColorGroup group, QPalette::ColorRole role) const;

    static QPixmap scale(int w, int h, const QPixmap& pixmap, const QMargins& margins);
    static QRect sourceRectImage(const QRect& rcSrc, int state, int count);

    QPixmap getCachedPixmap(const QString& className, const QString& image, const QSize& size, const QMargins& margins = QMargins(0, 0, 0, 0), const QColor& clrTransparent = QColor()) const;
    QPixmap getCachedPixmap(const QString& className, const QString& image, const QSize& size, int state, int count, const QMargins& margins = QMargins(0, 0, 0, 0), const QColor& clrTransparent = QColor()) const;
    bool drawWidgetPixmap(const QString& className, const QString& image, const QRect& rect, QPainter* painter, const QMargins& margins = QMargins(0, 0, 0, 0), const QColor& clrTransparent = QColor()) const;
    bool drawWidgetPixmap(const QString& className, const QString& image, const QRect& rect, QPainter* painter, int state, int count, const QMargins& margins = QMargins(0, 0, 0, 0), const QColor& clrTransparent = QColor()) const;
protected:
    QColor darkBackground(const QPalette& pal) const;
    virtual QColor darkBackgroundLight(const QPalette& pal) const;
    QColor outlineMidlight(const QPalette& pal) const;
    QColor outlineLight(const QPalette& pal) const;
    virtual QColor textGalleryItem(const QPalette& pal) const;
    virtual QColor buttonHighlightColor(const QPalette& pal, bool highlightDn, bool border = false, const QWidget* wd = nullptr) const;
    virtual QColor titleBarWindowColor(const QPalette& pal, bool isActive, const QWidget* wd) const;
    virtual QColor titleBarFrameColor(const QPalette& pal, bool isActive, const QWidget* wd) const;
    int colorBrightness(const QColor& clr) const;
    bool isDarkColor(const QColor& clr) const;
public:
    AStyleConfig m_config;
    QHash<QString, QPixmap> m_stdIconsBlackHash;
    QHash<QString, QPixmap> m_stdIconsDkGrayHash;
    QHash<QString, QPixmap> m_stdIconsGrayHash;
    QHash<QString, QPixmap> m_stdIconsLtGrayHash;
    QHash<QString, QPixmap> m_stdIconsWhiteHash;
    QHash<QString, QPixmap> m_stdIconsBlack2Hash;
    QHash<QString, QPixmap> m_stdIconsColor0Hash;
    QHash<QString, QPixmap> m_stdIconsColor1Hash;
    QHash<QString, QPixmap> m_stdIconsColor2Hash;
    QHash<QString, QPixmap> m_stdIconsColor3Hash;
protected:
    bool m_animationEnabled;
    bool m_ignoreDialogs;
    bool m_ignoreScrollBars;
    QColor m_colorIconBlack;
    QColor m_colorIconDkGray;
    QColor m_colorIconGray;
    QColor m_colorIconLtGray;
    QColor m_colorIconWhite;
    QColor m_colorIconBlack2;
    QColor m_colorIconColor0;
    QColor m_colorIconColor1;
    QColor m_colorIconColor2;
    QColor m_colorIconColor3;
    AStandardSymbolIcon m_standardSymbol;
    QList<AAnimation*> m_animations;
    QBasicTimer m_animationTimer;

    static QColor m_clrCloseButton;
    static QColor m_clrCloseButtonHighlight;
protected:
    QPalette m_systemPalette;
private:
    Q_DISABLE_COPY(ACommonStylePrivate)
        friend class AStandardSymbolIcon;
    friend class AStyleConfig;
};

class APen
{
public:
    APen(QPainter* p, QPen& pen)
        : m_painter(p)
    {
        m_savePen = m_painter->pen();
        m_painter->setPen(pen);
    }
    APen(QPainter* p, const QColor& clr, int width = 1)
        : m_painter(p)
    {
        m_savePen = m_painter->pen();
        m_painter->setPen(QPen(clr, width));
    }
    ~APen()
    {
        m_painter->setPen(m_savePen);
    }
protected:
    QPainter* m_painter;
    QPen m_savePen;
};

/* QtnBrush */
class ABrush
{
public:
    ABrush(QPainter* p, QBrush& brush)
        : m_painter(p)
    {
        m_saveBrush = m_painter->brush();
        m_painter->setBrush(brush);
    }
    ABrush(QPainter* p, const QColor& clr, Qt::BrushStyle bs = Qt::SolidPattern)
        : m_painter(p)
    {
        m_saveBrush = m_painter->brush();
        m_painter->setBrush(QBrush(clr, bs));
    }
    ~ABrush()
    {
        m_painter->setBrush(m_saveBrush);
    }
protected:
    QPainter* m_painter;
    QBrush m_saveBrush;
};

APROCH_API void aproch_paintLight(const QWidget* widget, QPainter* p, const QRect& r);
APROCH_API void aproch_paintLightFrame(const QWidget* widget, QPainter* p, const QRect& r);
APROCH_API void aproch_clickLight(const QWidget* widget);
APROCH_API void aproch_setShadecolor(QImage& image, const QColor& color, const QColor& origColor = Qt::black);
APROCH_API bool aproch_hasWindowTitleBar(const QWidget* widget);

inline QPixmap aproch_iconPixmap(const QIcon& icon, const QWidget* widget, const QSize& size, QIcon::Mode mode = QIcon::Normal, QIcon::State state = QIcon::Off)
{
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    qreal devicePixelRatio = widget != nullptr ? widget->devicePixelRatio() : qApp->devicePixelRatio();
    return icon.pixmap(size, devicePixelRatio, mode, state);
#elif (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    QWindow* w = widget != nullptr ? widget->window()->windowHandle() : nullptr;
    return icon.pixmap(w, size, mode, state);
#else
    Q_UNUSED(widget)
        return icon.pixmap(iconSize, mode, state);
#endif
}

inline bool aproch_isAncestor(const QObject* child, const char* strParent)
{
    while (child)
    {
        if (child && child->inherits(strParent))
            return true;
        child = child->parent();
    }
    return false;
}

inline const QObject* aproch_findChild(const QObject* object, const char* strParent)
{
    if (strParent == nullptr || object == nullptr)
        return nullptr;

    const QObjectList& children = object->children();
    for (int i = 0; i < children.size(); ++i)
    {
        QObject* obj = children.at(i);
        if (obj->inherits(strParent))
            return qobject_cast<QWidget*>(obj);
    }

    for (int i = 0; i < children.size(); ++i)
    {
        if (const QObject* obj = aproch_findChild(children.at(i), strParent))
            return obj;
    }

    return nullptr;
}

inline void aproch_setPaletteBrush(QPalette& palette, QPalette::ColorGroup group,
                                     QPalette::ColorRole role, const QBrush& brush)
{
#if 1
    palette.setBrush(group, role, brush);
#else
    if (palette.isBrushSet(group, role))
        return;
    uint mask = palette.resolve();
    palette.setBrush(group, role, brush);
    palette.resolve(mask);
#endif
}

inline QPixmap aproch_ColorizedImage(const QPixmap& px, const QColor& clrSrc, const QColor& clrDest)
{
    if (!clrDest.isValid())
        return px;

    QImage img = px.toImage();
    for (int y = 0; y < img.height(); ++y)
    {
        QRgb* scanLine = reinterpret_cast<QRgb*>(img.scanLine(y));
        for (int x = 0; x < img.width(); ++x)
        {
            QRgb pixel = *scanLine;
            if (pixel == clrSrc.rgb())
                *scanLine = clrDest.rgb();
            ++scanLine;
        }
    }
    return QPixmap::fromImage(img);
}

inline QLatin1String aproch_bestPngSize(const QWidget* widget)
{
    QLatin1String fileSuffix("9x9.png");
    QSize sz = QSize(10, 10);
    QSize size = aproch_DPIScaled(sz, widget);
    if (size.width() + 1 > 18)
        fileSuffix = QLatin1String("18x18.png");
    else if (size.width() + 1 > 13)
        fileSuffix = QLatin1String("13x13.png");
    else if (size.width() + 1 > 11)
        fileSuffix = QLatin1String("11x11.png");
    return fileSuffix;
}

inline QSize aproch_bestImageSize(const QWidget* widget)
{
    QSize sz = QSize(9, 9);
    QSize size = aproch_DPIScaled(sz, widget);
    if (size.width() + 1 > 18)
        sz = QSize(18, 18);
    else if (size.width() + 1 > 13)
        sz = QSize(13, 13);
    else if (size.width() + 1 > 11)
        sz = QSize(11, 11);
    return sz;
}

inline void aproch_draw3dRect(QPainter& p, int x, int y, int cx, int cy, const QColor& clrTopLeft, const QColor& clrBottomRight)
{
    p.fillRect(x, y, cx - 1, 1, clrTopLeft);
    p.fillRect(x, y, 1, cy - 1, clrTopLeft);
    p.fillRect(x + cx, y, -1, cy, clrBottomRight);
    p.fillRect(x, y + cy, cx, -1, clrBottomRight);
}

/*! \internal */
inline void aproch_drawArrow(const QStyle* style, const QStyleOptionToolButton* toolbutton,
                      const QRect& rect, QPainter* painter, const QWidget* widget = nullptr)
{
    QStyle::PrimitiveElement pe;

    switch (toolbutton->arrowType)
    {
    case Qt::LeftArrow:
        pe = QStyle::PE_IndicatorArrowLeft;
        break;
    case Qt::RightArrow:
        pe = QStyle::PE_IndicatorArrowRight;
        break;
    case Qt::UpArrow:
        pe = QStyle::PE_IndicatorArrowUp;
        break;
    case Qt::DownArrow:
        pe = QStyle::PE_IndicatorArrowDown;
        break;
    default:
        return;
    }

    QStyleOptionToolButton arrowOpt;
    arrowOpt.rect = rect;
    arrowOpt.palette = toolbutton->palette;
    arrowOpt.state = toolbutton->state;
    style->drawPrimitive(pe, &arrowOpt, painter, widget);
}

#ifdef A_MAC_TITLEBAR
inline QLinearGradient aproch_titleBarGradientActive()
{
    static QLinearGradient gradient;
    if (gradient == QLinearGradient())
    {
        gradient.setColorAt(0, QColor(235, 235, 235));
        gradient.setColorAt(0.5, QColor(210, 210, 210));
        gradient.setColorAt(0.75, QColor(195, 195, 195));
        gradient.setColorAt(1, QColor(180, 180, 180));
    }
    return gradient;
}

inline QLinearGradient aproch_titleBarGradientInactive()
{
    static QLinearGradient gradient;
    if (gradient == QLinearGradient())
    {
        gradient.setColorAt(0, QColor(250, 250, 250));
        gradient.setColorAt(1, QColor(225, 225, 225));
    }
    return gradient;
}
#endif

#ifdef Q_OS_MACX
bool set_style_mask_hook(QWidget* w);
bool unset_style_mask_hook(QWidget* w);
#endif

APROCH_NAMESPACE_END