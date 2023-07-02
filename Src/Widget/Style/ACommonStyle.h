/****************************************************************************
 * @file    AACommonStyle.h
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
#include <QProxyStyle>
#include <QStyleOption>

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

Q_ALWAYS_INLINE void initBaseResource()
{
    Q_INIT_RESOURCE(RC_BaseResources);
}

#ifndef QT_NO_EFFECTS
struct QEffects
{
    enum Direction
    {
        LeftScroll = 0x0001,
        RightScroll = 0x0002,
        UpScroll = 0x0004,
        DownScroll = 0x0008
    };

    typedef uint DirFlags;
};

extern void Q_GUI_EXPORT qScrollEffect(QWidget*, QEffects::DirFlags dir = QEffects::DownScroll, int time = -1);
extern void Q_GUI_EXPORT qFadeEffect(QWidget*, int time = -1);
#endif

APROCH_NAMESPACE_BEGIN

class ACommonStylePrivate;

/* ACommonStyle */
class APROCH_API ACommonStyle : public QProxyStyle
{
    Q_OBJECT
        Q_PROPERTY(bool animationEnabled READ isAnimationEnabled WRITE setAnimationEnabled)
        Q_PROPERTY(bool ignoreDialogs READ isDialogsIgnored WRITE setDialogsIgnored)
        Q_PROPERTY(bool ignoreScrollBars READ isScrollBarsIgnored WRITE setScrollBarsIgnored)
public:
    /*! \internal */
    enum PixelMetricEx
    {
        PM_BaseScrollButtonItem = PM_CustomBase + 1,
        PM_RibbonGroupReducedWidth,
        PM_RibbonVMarginCaptionSysMenu,
        PM_RibbonHMarginCaptionSysMenu,
        PM_RibbonPopupMenuGripHeight,
        PM_RibbonQuickAccessBarMargin,
        PM_RibbonGroupContentMargin,
        PM_RibbonLayoutHorizontalSpacing,
        PM_RibbonOptionButtonIconSize,
        PM_GridGroupPanelHeight,
        PM_GridSmallToolButtonSize,
        PM_ToggleSwitchThumbSize,
        PM_ToggleSwitchMargin,
        PM_ToggleSwitchLabelSpacing,
    };

    /*! \internal */
    enum PrimitiveElementEx
    {
        // PE_CustomBase = 0xf0000000 - base
        PE_RibbonBarPanel = PE_CustomBase + 1,
        PE_RibbonFrameGroups,
        PE_RibbonFrameGroup,
        PE_RibbonFrameGallery,
        PE_RibbonSysFrameMenu,
        PE_RibbonSysFramePagePopupList,
        PE_RibbonSysFramePagePopup,
        PE_RibbonPanelButton,
        PE_RibbonOptionButton,
        PE_RibbonQuickAccessButton,
        PE_RibbonBackstageFrame,
        PE_RibbonBackstagePanelButton,
        PE_RibbonBackstageCloseButton,
        PE_RibbonGroupScrollButton,
        PE_RibbonPopupBarButton,
        PE_RibbonSliderButton,
        PE_RibbonKeyTip,
        PE_DockAutoHideFrameWindow,
        PE_DockMarker,
        PE_DockWidgetPhantom,
        PE_Workspace,
    };

    /*! \internal */
    enum ContentsTypeEx
    {
        CT_RibbonSliderButton = CT_CustomBase + 1,
        CT_RibbonGroupButton,
    };

    /*! \internal */
    enum ControlElementEx
    {
        CE_RibbonTabShapeLabel = CE_CustomBase + 1,
        CE_RibbonTab,
        CE_RibbonTabBar,
        CE_RibbonGroupLabel,
        CE_RibbonSysMenuItem,
        CE_RibbonBackstageMenuItem,
        CE_RibbonBackstageButtonLabel,
        CE_RibbonSysRecentFileItem,
        CE_RibbonButtonLabel,
        CE_RibbonGalleryItem,
        CE_RibbonReducedGroup,
        CE_RibbonGripBar,
        CE_RibbonTitleBarButton,
        CE_NavigationBackButton,
    };

    /*! \internal */
    enum ComplexControlEx
    {
        CC_RibbonFileButton = CC_CustomBase + 1,
        CC_RibbonTitleBarWidget,
        CC_RibbonGalleryButton,
        CC_RibbonBackstageButton,
        CC_RibbonButton,
        CC_ToggleSwitch,
    };

    /*! \internal */
    enum SubElementEx
    {
        SE_RibbonSysHeaderLabelPopupList = SE_CustomBase + 1,
    };

    /*! \internal */
    enum SubControlEx
    {
        SC_TitleBarFrame = (int)SC_CustomBase + 1,
        SC_ToggleSwitchGroove,
        SC_ToggleSwitchThumb,
        SC_ToggleSwitchText,
    };
    Q_DECLARE_FLAGS(SubControlsEx, SubControlEx)

        /*! \internal */
        enum StyleHintEx
    {
        SH_AprochBegin = SH_CustomBase + 1,
        SH_RibbonBegin = SH_AprochBegin,
        SH_RibbonStyledFrame = SH_RibbonBegin,
        SH_RibbonsSysMenuButtonVisible,
        SH_RecentFileListBackground,
        SH_RibbonItemUpperCase,
        SH_RibbonBackstageHideTabs,
        SH_WindowTitleBarPalette,
        SH_RibbonEnd = SH_WindowTitleBarPalette,
        SH_DockStyledFrame,
        SH_DockSysMenuButtonVisible,
    };

    /*! \internal */
    enum StandardPixmapEx
    {
        SP_RibbonButtonPopupMenu = SP_CustomBase + 1,
        SP_RibbonButtonScrollUp,
        SP_RibbonButtonScrollDown,
        SP_RibbonButtonGroupOption,
        SP_RibbonShadedArrowUp,
        SP_RibbonShadedArrowDown,
        SP_TitleBarPinButton,
        SP_TitleBarPinFillButton,
        SP_TitleBarPinnedButton,
        SP_TitleBarPinnedFillButton,
        SP_NavigationBackButton,

        SP_DockMarkerBasis,
        SP_DockMarkerBasisEx,
        SP_DockMarkerLeft,
        SP_DockMarkerBasisLeft,

        SP_DockMarkerRight,
        SP_DockMarkerBasisRight,

        SP_DockMarkerCenterLeft,
        SP_DockMarkerCenterSplitLeft,

        SP_DockMarkerCenterRight,
        SP_DockMarkerCenterSplitRight,

        SP_DockMarkerCenterTop,
        SP_DockMarkerCenterSplitTop,

        SP_DockMarkerCenterBottom,
        SP_DockMarkerCenterSplitBottom,

        SP_DockMarkerTop,
        SP_DockMarkerBasisTop,

        SP_DockMarkerBottom,
        SP_DockMarkerBasisBottom,
        SP_DockMarkerCenter
    };
public:
    ACommonStyle(QStyle* style = nullptr);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    ACommonStyle(const QString& key);
#endif
    ~ACommonStyle() override;
    static ACommonStyle* ensureStyle();
public:
    bool isAnimationEnabled() const;
    void setAnimationEnabled(bool enabled = true);
    void setDialogsIgnored(bool ignore = false);
    bool isDialogsIgnored() const;

    void setScrollBarsIgnored(bool ignore = false);
    bool isScrollBarsIgnored() const;
public:
    QPalette standardPalette() const override;
    void polish(QPalette& palette) override;
    void polish(QApplication* app) override;
    void unpolish(QApplication* app) override;
    void polish(QWidget* widget) override;
    void unpolish(QWidget* widget) override;
public:
    QRect itemTextRect(const QFontMetrics& fm, const QRect& r, int flags, bool enabled, const QString& text) const override;
    QRect itemPixmapRect(const QRect& r, int flags, const QPixmap& pixmap) const override;

    void drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* widget = nullptr) const override;
    void drawControl(QStyle::ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* widget = nullptr) const override;
    void drawComplexControl(QStyle::ComplexControl cc, const QStyleOptionComplex* opt, QPainter* p, const QWidget* widget = nullptr) const override;
    QRect subElementRect(QStyle::SubElement r, const QStyleOption* opt, const QWidget* widget = nullptr) const override;
    SubControl hitTestComplexControl(QStyle::ComplexControl cc, const QStyleOptionComplex* opt, const QPoint& pt, const QWidget* widget = nullptr) const override;
    QRect subControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc, const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(QStyle::ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* widget = nullptr) const override;

    int pixelMetric(QStyle::PixelMetric m, const QStyleOption* opt = nullptr, const QWidget* widget = nullptr) const override;
    int styleHint(QStyle::StyleHint sh, const QStyleOption* opt = nullptr, const QWidget* w = nullptr, QStyleHintReturn* shret = nullptr) const override;
    QIcon standardIcon(QStyle::StandardPixmap standardIcon, const QStyleOption* opt = nullptr, const QWidget* widget = nullptr) const override;
    QPixmap standardPixmap(QStyle::StandardPixmap sp, const QStyleOption* opt = nullptr, const QWidget* widget = nullptr) const override;
    QPixmap generatedIconPixmap(QIcon::Mode iconMode, const QPixmap& pixmap, const QStyleOption* opt) const override;

    int layoutSpacing(QSizePolicy::ControlType control1, QSizePolicy::ControlType control2, Qt::Orientation orientation,
                      const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;
public:
    void setStyleConfig(const QString& fileName, const QString& themeName);
    static QRgb lightColor(const QRgb& rgb, int tint);
    static QRgb darkColor(const QRgb& rgb, int tint);
    static QColor contextColorToColor(EContextColor color);
    static QImage addaptColors(const QImage& imageSrc, QRgb clrBase, QRgb clrTone, bool clampHue = true);
    static QImage invertColors(const QImage& pxSrc);
Q_SIGNALS:
    void widgetPolished(QWidget* w);
protected:
    ACommonStyle(ACommonStylePrivate& dd);
    void reset();
protected:
    bool event(QEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
protected:
    A_DECLARE_PRIVATE(ACommonStyle);
private:
    Q_DISABLE_COPY(ACommonStyle);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ACommonStyle::SubControlsEx)

APROCH_API int aproch_DPIScaled(int value, const QWidget* widget);
APROCH_API qreal aproch_DPIScaledF(qreal value, const QWidget* widget);
APROCH_API QPointF aproch_DPIScaledF(const QPointF& point, const QWidget* widget);
APROCH_API QSize aproch_DPIScaled(const QSize& size, const QWidget* widget);
APROCH_API QSizeF aproch_DPIScaledF(const QSizeF& size, const QWidget* widget);
APROCH_API int aproch_getPixelMetric(int metric, const QWidget* widget);
APROCH_API QRect aproch_paint_icon_and_text(QPainter& painter, Qt::LayoutDirection direction,
                                            const QPalette& palette, const QRect& rect, const QIcon& icon, const QSize& iconSize,
                                            bool enabled, Qt::Alignment iconAlignment, const QString& text, Qt::Alignment textAlignment,
                                            Qt::TextElideMode textElideMode = Qt::ElideRight,
                                            int margin = 5, bool* textFullyVisible = nullptr,
                                            const QString& selectedText = QString(), Qt::CaseSensitivity selectCaseSensitivity = Qt::CaseInsensitive);
APROCH_API void aproch_draw_pixmap_texture(QPainter* painter, const QRect& rect, const QPixmap& texture, int left, int top, int right, int bottom);
APROCH_API void aproch_paint_keyboard_focus(QPainter* painter, const QRect& rect, const QColor& color);
APROCH_API bool aproch_isColorLight(const QColor& color);
APROCH_API QPen aproch_createPen(const QColor& color, int thickness = 1);
APROCH_API QRect aproch_adjustRect(const QPen& pen, const QRect& rect);
APROCH_API QRectF aproch_adjustRectF(const QPen& pen, const QRectF& rect);
APROCH_API void aproch_drawRect(QPainter* painter, const QRect& rect);

//#define _DEBUG_PALETTE
#ifdef _DEBUG_PALETTE
APROCH_API void dump_palette(const QPalette& p, bool _out = false);
APROCH_API void show_palette_dialog(const QPalette& p);
#endif

APROCH_NAMESPACE_END
