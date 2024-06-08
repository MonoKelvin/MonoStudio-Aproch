#pragma once

APROCH_NAMESPACE_BEGIN

class AWinUIThemePrivate;

/**
 * @brief winui主题
 */
class APROCH_API AWinUITheme
{
public:
    ~AWinUITheme();

    /*
     *****************************************************************
     SYSTEM STYLES
     *****************************************************************
     */
    static QColor SystemAccentColor;
    static QColor SystemAccentColorLight1;
    static QColor SystemAccentColorLight2;
    static QColor SystemAccentColorLight3;
    static QColor SystemAltHighColor;
    static QColor SystemAltLowColor;
    static QColor SystemAltMediumColor;
    static QColor SystemAltMediumHighColor;
    static QColor SystemAltMediumLowColor;
    static QColor SystemBaseHighColor;
    static QColor SystemBaseLowColor;
    static QColor SystemBaseMediumColor;
    static QColor SystemBaseMediumHighColor;
    static QColor SystemBaseMediumLowColor;
    static QColor SystemChromeAltLowColor;
    static QColor SystemChromeBlackHighColor;
    static QColor SystemChromeBlackLowColor;
    static QColor SystemChromeBlackMediumLowColor;
    static QColor SystemChromeBlackMediumColor;
    static QColor SystemChromeDisabledHighColor;
    static QColor SystemChromeDisabledLowColor;
    static QColor SystemChromeHighColor;
    static QColor SystemChromeLowColor;
    static QColor SystemChromeMediumColor;
    static QColor SystemChromeMediumLowColor;
    static QColor SystemChromeWhiteColor;
    static QColor SystemChromeGrayColor;
    static QColor SystemListLowColor;
    static QColor SystemListMediumColor;
    static QColor SystemErrorTextColor;

    /** SolidColorBrush */
    static QColor SystemControlBackgroundAccentBrush;
    static QColor SystemControlBackgroundAltHighBrush;
    static QColor SystemControlBackgroundAltMediumHighBrush;
    static QColor SystemControlBackgroundAltMediumBrush;
    static QColor SystemControlBackgroundAltMediumLowBrush;
    static QColor SystemControlBackgroundBaseHighBrush;
    static QColor SystemControlBackgroundBaseLowBrush;
    static QColor SystemControlBackgroundBaseMediumBrush;
    static QColor SystemControlBackgroundBaseMediumHighBrush;
    static QColor SystemControlBackgroundBaseMediumLowBrush;
    static QColor SystemControlBackgroundChromeBlackHighBrush;
    static QColor SystemControlBackgroundChromeBlackMediumBrush;
    static QColor SystemControlBackgroundChromeBlackLowBrush;
    static QColor SystemControlBackgroundChromeBlackMediumLowBrush;
    static QColor SystemControlBackgroundChromeMediumBrush;
    static QColor SystemControlBackgroundChromeMediumLowBrush;
    static QColor SystemControlBackgroundChromeWhiteBrush;
    static QColor SystemControlBackgroundListLowBrush;
    static QColor SystemControlBackgroundListMediumBrush;
    static QColor SystemControlDisabledAccentBrush;
    static QColor SystemControlDisabledBaseHighBrush;
    static QColor SystemControlDisabledBaseLowBrush;
    static QColor SystemControlDisabledBaseMediumLowBrush;
    static QColor SystemControlDisabledChromeDisabledHighBrush;
    static QColor SystemControlDisabledChromeDisabledLowBrush;
    static QColor SystemControlDisabledChromeHighBrush;
    static QColor SystemControlDisabledChromeMediumLowBrush;
    static QColor SystemControlDisabledListMediumBrush;
    static QColor SystemControlDisabledTransparentBrush;
    static QColor SystemControlFocusVisualPrimaryBrush;
    static QColor SystemControlFocusVisualSecondaryBrush;
    static QColor SystemControlRevealFocusVisualBrush;
    static QColor SystemControlForegroundAccentBrush;
    static QColor SystemControlForegroundAltHighBrush;
    static QColor SystemControlForegroundAltMediumHighBrush;
    static QColor SystemControlForegroundBaseHighBrush;
    static QColor SystemControlForegroundBaseLowBrush;
    static QColor SystemControlForegroundBaseMediumBrush;
    static QColor SystemControlForegroundBaseMediumHighBrush;
    static QColor SystemControlForegroundBaseMediumLowBrush;
    static QColor SystemControlForegroundChromeBlackHighBrush;
    static QColor SystemControlForegroundChromeHighBrush;
    static QColor SystemControlForegroundChromeMediumBrush;
    static QColor SystemControlForegroundChromeWhiteBrush;
    static QColor SystemControlForegroundChromeDisabledLowBrush;
    static QColor SystemControlForegroundChromeGrayBrush;
    static QColor SystemControlForegroundListLowBrush;
    static QColor SystemControlForegroundListMediumBrush;
    static QColor SystemControlForegroundTransparentBrush;
    static QColor SystemControlForegroundChromeBlackMediumBrush;
    static QColor SystemControlForegroundChromeBlackMediumLowBrush;
    static QColor SystemControlHighlightAccentBrush;
    static QColor SystemControlHighlightAltAccentBrush;
    static QColor SystemControlHighlightAltAltHighBrush;
    static QColor SystemControlHighlightAltBaseHighBrush;
    static QColor SystemControlHighlightAltBaseLowBrush;
    static QColor SystemControlHighlightAltBaseMediumBrush;
    static QColor SystemControlHighlightAltBaseMediumHighBrush;
    static QColor SystemControlHighlightAltAltMediumHighBrush;
    static QColor SystemControlHighlightAltBaseMediumLowBrush;
    static QColor SystemControlHighlightAltListAccentHighBrush;
    static QColor SystemControlHighlightAltListAccentLowBrush;
    static QColor SystemControlHighlightAltListAccentMediumBrush;
    static QColor SystemControlHighlightAltChromeWhiteBrush;
    static QColor SystemControlHighlightAltTransparentBrush;
    static QColor SystemControlHighlightBaseHighBrush;
    static QColor SystemControlHighlightBaseLowBrush;
    static QColor SystemControlHighlightBaseMediumBrush;
    static QColor SystemControlHighlightBaseMediumHighBrush;
    static QColor SystemControlHighlightBaseMediumLowBrush;
    static QColor SystemControlHighlightChromeAltLowBrush;
    static QColor SystemControlHighlightChromeHighBrush;
    static QColor SystemControlHighlightListAccentHighBrush;
    static QColor SystemControlHighlightListAccentLowBrush;
    static QColor SystemControlHighlightListAccentMediumBrush;
    static QColor SystemControlHighlightListMediumBrush;
    static QColor SystemControlHighlightListLowBrush;
    static QColor SystemControlHighlightChromeWhiteBrush;
    static QColor SystemControlHighlightTransparentBrush;
    static QColor SystemControlHyperlinkTextBrush;
    static QColor SystemControlHyperlinkBaseHighBrush;
    static QColor SystemControlHyperlinkBaseMediumBrush;
    static QColor SystemControlHyperlinkBaseMediumHighBrush;
    static QColor SystemControlPageBackgroundAltMediumBrush;
    static QColor SystemControlPageBackgroundAltHighBrush;
    static QColor SystemControlPageBackgroundMediumAltMediumBrush;
    static QColor SystemControlPageBackgroundBaseLowBrush;
    static QColor SystemControlPageBackgroundBaseMediumBrush;
    static QColor SystemControlPageBackgroundListLowBrush;
    static QColor SystemControlPageBackgroundChromeLowBrush;
    static QColor SystemControlPageBackgroundChromeMediumLowBrush;
    static QColor SystemControlPageBackgroundTransparentBrush;
    static QColor SystemControlPageTextBaseHighBrush;
    static QColor SystemControlPageTextBaseMediumBrush;
    static QColor SystemControlPageTextChromeBlackMediumLowBrush;
    static QColor SystemControlTransparentBrush;
    static QColor SystemControlErrorTextForegroundBrush;

    /** 
     ******************************************************************
     DEFAULT COMMON CONTROL PROPERTIES
     ******************************************************************
     */

    static QColor DefaultControlBackgroundColor;
    static QColor DefaultControlBackgroundHighColor;
    static QColor DefaultControlBackgroundLowColor;
    static QColor DefaultControlBorderUnderLineColor;

    /** Thickness */
    static QMargins AppBarBottomBorderThemeThickness;
    static QMargins AppBarBottomThemePadding;
    static QMargins AppBarTopBorderThemeThickness;
    static QMargins AppBarTopThemePadding;
    static QMargins AppBarExpandButtonCircleInnerPadding;
    static QMargins AutoSuggestListBorderThemeThickness;
    static QMargins AutoSuggestListMargin;
    static QMargins AutoSuggestListPadding;
    static QMargins AutoSuggestListViewItemMargin;
    static QMargins ButtonBorderThemeThickness;
    static QMargins CalendarDatePickerBorderThemeThickness;
    static QMargins ComboBoxBorderThemeThickness;
    static QMargins ComboBoxDropdownBorderThickness;
    static QMargins ComboBoxDropdownBorderPadding;
    static QMargins ComboBoxDropdownContentMargin;
    static QMargins ComboBoxHeaderThemeMargin;
    static QMargins ComboBoxPopupBorderThemeThickness;
    static QMargins ComboBoxItemThemePadding;
    static QMargins ComboBoxItemThemeTouchPadding;
    static QMargins ComboBoxItemThemeGameControllerPadding;
    static QMargins ContentDialogBorderWidth;
    static QMargins ContentDialogButton1HostMargin;
    static QMargins ContentDialogButton2HostMargin;
    static QMargins ContentDialogContentMargin;
    static QMargins ContentDialogContentScrollViewerMargin;
    static QMargins ContentDialogCommandSpaceMargin;
    static QMargins ContentDialogTitleMargin;
    static QMargins ContentDialogPadding;
    static QMargins DatePickerHeaderThemeMargin;
    static QMargins DateTimeFlyoutBorderThickness;
    static QMargins DateTimeFlyoutBorderPadding;
    static QMargins DateTimeFlyoutButtonBorderThickness;
    static QMargins DateTimeFlyoutContentPanelPortraitThemeMargin;
    static QMargins DateTimeFlyoutContentPanelLandscapeThemeMargin;
    static QMargins DateTimeFlyoutTitleThemeMargin;
    static QMargins FlipViewButtonBorderThemeThickness;
    static QMargins FlyoutContentThemeMargin;
    static QMargins FlyoutContentThemePadding;
    static QMargins GridViewItemCompactSelectedBorderThemeThickness;
    static QMargins GridViewItemMultiselectBorderThickness;
    static QMargins HandwritingViewExpandedButtonMargin;
    static QMargins HubSectionHeaderThemeMargin;
    static QMargins HubSectionHeaderSeeMoreThemeMargin;
    static QMargins HyperlinkButtonBorderThemeThickness;
    static QMargins ListPickerFlyoutPresenterMultiselectCheckBoxMargin;
    static QMargins ListPickerFlyoutPresenterItemMargin;
    static QMargins PickerFlyoutContentPanelLandscapeThemeMargin;
    static QMargins PickerFlyoutContentPanelPortraitThemeMargin;
    static QMargins PickerFlyoutTitleThemeMargin;
    static QMargins PivotHeaderItemMargin;
    static QMargins PivotItemMargin;
    static QMargins PivotLandscapeThemePadding;
    static QMargins PivotNavButtonBorderThemeThickness;
    static QMargins PivotNavButtonMargin;
    static QMargins PivotPortraitThemePadding;
    static QMargins ProgressBarBorderThemeThickness;
    static QMargins RepeatButtonBorderThemeThickness;
    static QMargins ScrollBarPanningBorderThemeThickness;
    static QMargins SearchBoxQuerySuggestionThemeMargin;
    static QMargins SearchBoxResultSuggestionThemeMargin;
    static QMargins SearchBoxSeparatorSuggestionThemeMargin;
    static QMargins SearchBoxSuggestionSubcomponentThemeMargin;
    static QMargins SearchBoxThemePadding;
    static QMargins SearchBoxIMECandidateListSeparatorThemeThickness;
    static QMargins SearchBoxBorderThemeThickness;
    static QMargins SliderBorderThemeThickness;
    static QMargins SliderHeaderThemeMargin;
    static QMargins SplitViewLeftBorderThemeThickness;
    static QMargins SplitViewRightBorderThemeThickness;
    static QMargins TextControlBorderThemeThickness;
    static QMargins TextControlMarginThemeThickness;
    static QMargins TextControlThemePadding;
    static QMargins HelperButtonThemePadding;
    static QMargins TextControlPlaceholderThemePadding;
    static QMargins TimePickerHeaderThemeMargin;
    static QMargins TimePickerFirstHostThemeMargin;
    static QMargins TimePickerThirdHostThemeMargin;
    static QMargins ToggleButtonBorderThemeThickness;
    static QMargins KeyTipBorderThemeThickness;
    static QMargins KeyTipThemePadding;

    /** Border */
    static SCorner DefaultControlCornerRadius;

private:
    friend class WinUIThemeLoader;
    AWinUITheme();

private:
    Q_DISABLE_COPY_MOVE(AWinUITheme);
    QScopedPointer<AWinUIThemePrivate> d;
};

APROCH_NAMESPACE_END