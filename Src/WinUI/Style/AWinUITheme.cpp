#include "stdafx.h"
#include "AWinUITheme.h"

#include <winrt/Windows.UI.ViewManagement.h>
using namespace winrt;
using namespace Windows::UI::ViewManagement;

APROCH_NAMESPACE_BEGIN

QColor AWinUITheme::SystemAccentColor;
QColor AWinUITheme::SystemAccentColorLight1;
QColor AWinUITheme::SystemAccentColorLight2;
QColor AWinUITheme::SystemAccentColorLight3;
QColor AWinUITheme::SystemAltHighColor = AStr("#FF000000");
QColor AWinUITheme::SystemAltLowColor = AStr("#33000000");
QColor AWinUITheme::SystemAltMediumColor = AStr("#99000000");
QColor AWinUITheme::SystemAltMediumHighColor = AStr("#CC000000");
QColor AWinUITheme::SystemAltMediumLowColor = AStr("#66000000");
QColor AWinUITheme::SystemBaseHighColor = AStr("#FFFFFFFF");
QColor AWinUITheme::SystemBaseLowColor = AStr("#33FFFFFF");
QColor AWinUITheme::SystemBaseMediumColor = AStr("#99FFFFFF");
QColor AWinUITheme::SystemBaseMediumHighColor = AStr("#CCFFFFFF");
QColor AWinUITheme::SystemBaseMediumLowColor = AStr("#66FFFFFF");
QColor AWinUITheme::SystemChromeAltLowColor = AStr("#FFF2F2F2");
QColor AWinUITheme::SystemChromeBlackHighColor = AStr("#FF000000");
QColor AWinUITheme::SystemChromeBlackLowColor = AStr("#33000000");
QColor AWinUITheme::SystemChromeBlackMediumLowColor = AStr("#66000000");
QColor AWinUITheme::SystemChromeBlackMediumColor = AStr("#CC000000");
QColor AWinUITheme::SystemChromeDisabledHighColor = AStr("#FF333333");
QColor AWinUITheme::SystemChromeDisabledLowColor = AStr("#FF858585");
QColor AWinUITheme::SystemChromeHighColor = AStr("#FF767676");
QColor AWinUITheme::SystemChromeLowColor = AStr("#FF171717");
QColor AWinUITheme::SystemChromeMediumColor = AStr("#FF1F1F1F");
QColor AWinUITheme::SystemChromeMediumLowColor = AStr("#FF2B2B2B");
QColor AWinUITheme::SystemChromeWhiteColor = AStr("#FFFFFFFF");
QColor AWinUITheme::SystemChromeGrayColor = AStr("#FF767676");
QColor AWinUITheme::SystemListLowColor = AStr("#19FFFFFF");
QColor AWinUITheme::SystemListMediumColor = AStr("#33FFFFFF");
QColor AWinUITheme::SystemErrorTextColor = AStr("#FFF00000");

QColor AWinUITheme::SystemControlBackgroundAccentBrush;
QColor AWinUITheme::SystemControlBackgroundAltHighBrush;
QColor AWinUITheme::SystemControlBackgroundAltMediumHighBrush;
QColor AWinUITheme::SystemControlBackgroundAltMediumBrush;
QColor AWinUITheme::SystemControlBackgroundAltMediumLowBrush;
QColor AWinUITheme::SystemControlBackgroundBaseHighBrush;
QColor AWinUITheme::SystemControlBackgroundBaseLowBrush;
QColor AWinUITheme::SystemControlBackgroundBaseMediumBrush;
QColor AWinUITheme::SystemControlBackgroundBaseMediumHighBrush;
QColor AWinUITheme::SystemControlBackgroundBaseMediumLowBrush;
QColor AWinUITheme::SystemControlBackgroundChromeBlackHighBrush;
QColor AWinUITheme::SystemControlBackgroundChromeBlackMediumBrush;
QColor AWinUITheme::SystemControlBackgroundChromeBlackLowBrush;
QColor AWinUITheme::SystemControlBackgroundChromeBlackMediumLowBrush;
QColor AWinUITheme::SystemControlBackgroundChromeMediumBrush;
QColor AWinUITheme::SystemControlBackgroundChromeMediumLowBrush;
QColor AWinUITheme::SystemControlBackgroundChromeWhiteBrush;
QColor AWinUITheme::SystemControlBackgroundListLowBrush;
QColor AWinUITheme::SystemControlBackgroundListMediumBrush;
QColor AWinUITheme::SystemControlDisabledAccentBrush;
QColor AWinUITheme::SystemControlDisabledBaseHighBrush;
QColor AWinUITheme::SystemControlDisabledBaseLowBrush;
QColor AWinUITheme::SystemControlDisabledBaseMediumLowBrush;
QColor AWinUITheme::SystemControlDisabledChromeDisabledHighBrush;
QColor AWinUITheme::SystemControlDisabledChromeDisabledLowBrush;
QColor AWinUITheme::SystemControlDisabledChromeHighBrush;
QColor AWinUITheme::SystemControlDisabledChromeMediumLowBrush;
QColor AWinUITheme::SystemControlDisabledListMediumBrush;
QColor AWinUITheme::SystemControlDisabledTransparentBrush;
QColor AWinUITheme::SystemControlFocusVisualPrimaryBrush;
QColor AWinUITheme::SystemControlFocusVisualSecondaryBrush;
QColor AWinUITheme::SystemControlRevealFocusVisualBrush;
QColor AWinUITheme::SystemControlForegroundAccentBrush;
QColor AWinUITheme::SystemControlForegroundAltHighBrush;
QColor AWinUITheme::SystemControlForegroundAltMediumHighBrush;
QColor AWinUITheme::SystemControlForegroundBaseHighBrush;
QColor AWinUITheme::SystemControlForegroundBaseLowBrush;
QColor AWinUITheme::SystemControlForegroundBaseMediumBrush;
QColor AWinUITheme::SystemControlForegroundBaseMediumHighBrush;
QColor AWinUITheme::SystemControlForegroundBaseMediumLowBrush;
QColor AWinUITheme::SystemControlForegroundChromeBlackHighBrush;
QColor AWinUITheme::SystemControlForegroundChromeHighBrush;
QColor AWinUITheme::SystemControlForegroundChromeMediumBrush;
QColor AWinUITheme::SystemControlForegroundChromeWhiteBrush;
QColor AWinUITheme::SystemControlForegroundChromeDisabledLowBrush;
QColor AWinUITheme::SystemControlForegroundChromeGrayBrush;
QColor AWinUITheme::SystemControlForegroundListLowBrush;
QColor AWinUITheme::SystemControlForegroundListMediumBrush;
QColor AWinUITheme::SystemControlForegroundTransparentBrush;
QColor AWinUITheme::SystemControlForegroundChromeBlackMediumBrush;
QColor AWinUITheme::SystemControlForegroundChromeBlackMediumLowBrush;
QColor AWinUITheme::SystemControlHighlightAccentBrush;
QColor AWinUITheme::SystemControlHighlightAltAccentBrush;
QColor AWinUITheme::SystemControlHighlightAltAltHighBrush;
QColor AWinUITheme::SystemControlHighlightAltBaseHighBrush;
QColor AWinUITheme::SystemControlHighlightAltBaseLowBrush;
QColor AWinUITheme::SystemControlHighlightAltBaseMediumBrush;
QColor AWinUITheme::SystemControlHighlightAltBaseMediumHighBrush;
QColor AWinUITheme::SystemControlHighlightAltAltMediumHighBrush;
QColor AWinUITheme::SystemControlHighlightAltBaseMediumLowBrush;
QColor AWinUITheme::SystemControlHighlightAltListAccentHighBrush;
QColor AWinUITheme::SystemControlHighlightAltListAccentLowBrush;
QColor AWinUITheme::SystemControlHighlightAltListAccentMediumBrush;
QColor AWinUITheme::SystemControlHighlightAltChromeWhiteBrush;
QColor AWinUITheme::SystemControlHighlightAltTransparentBrush;
QColor AWinUITheme::SystemControlHighlightBaseHighBrush;
QColor AWinUITheme::SystemControlHighlightBaseLowBrush;
QColor AWinUITheme::SystemControlHighlightBaseMediumBrush;
QColor AWinUITheme::SystemControlHighlightBaseMediumHighBrush;
QColor AWinUITheme::SystemControlHighlightBaseMediumLowBrush;
QColor AWinUITheme::SystemControlHighlightChromeAltLowBrush;
QColor AWinUITheme::SystemControlHighlightChromeHighBrush;
QColor AWinUITheme::SystemControlHighlightListAccentHighBrush;
QColor AWinUITheme::SystemControlHighlightListAccentLowBrush;
QColor AWinUITheme::SystemControlHighlightListAccentMediumBrush;
QColor AWinUITheme::SystemControlHighlightListMediumBrush;
QColor AWinUITheme::SystemControlHighlightListLowBrush;
QColor AWinUITheme::SystemControlHighlightChromeWhiteBrush;
QColor AWinUITheme::SystemControlHighlightTransparentBrush;
QColor AWinUITheme::SystemControlHyperlinkTextBrush;
QColor AWinUITheme::SystemControlHyperlinkBaseHighBrush;
QColor AWinUITheme::SystemControlHyperlinkBaseMediumBrush;
QColor AWinUITheme::SystemControlHyperlinkBaseMediumHighBrush;
QColor AWinUITheme::SystemControlPageBackgroundAltMediumBrush;
QColor AWinUITheme::SystemControlPageBackgroundAltHighBrush;
QColor AWinUITheme::SystemControlPageBackgroundMediumAltMediumBrush;
QColor AWinUITheme::SystemControlPageBackgroundBaseLowBrush;
QColor AWinUITheme::SystemControlPageBackgroundBaseMediumBrush;
QColor AWinUITheme::SystemControlPageBackgroundListLowBrush;
QColor AWinUITheme::SystemControlPageBackgroundChromeLowBrush;
QColor AWinUITheme::SystemControlPageBackgroundChromeMediumLowBrush;
QColor AWinUITheme::SystemControlPageBackgroundTransparentBrush;
QColor AWinUITheme::SystemControlPageTextBaseHighBrush;
QColor AWinUITheme::SystemControlPageTextBaseMediumBrush;
QColor AWinUITheme::SystemControlPageTextChromeBlackMediumLowBrush;
QColor AWinUITheme::SystemControlTransparentBrush;
QColor AWinUITheme::SystemControlErrorTextForegroundBrush;

QColor AWinUITheme::DefaultControlBackgroundColor("#FF343434");
QColor AWinUITheme::DefaultControlBackgroundHighColor("#FF393939");
QColor AWinUITheme::DefaultControlBackgroundLowColor("#FF212121");
QColor AWinUITheme::DefaultControlBorderUnderLineColor;

QMargins AWinUITheme::AppBarBottomBorderThemeThickness(0, 0, 0, 0);
QMargins AWinUITheme::AppBarBottomThemePadding(0, 0, 0, 0);
QMargins AWinUITheme::AppBarTopBorderThemeThickness(0, 0, 0, 0);
QMargins AWinUITheme::AppBarTopThemePadding(0, 0, 0, 0);
QMargins AWinUITheme::AppBarExpandButtonCircleInnerPadding(3, 0, 3, 0);
QMargins AWinUITheme::AutoSuggestListBorderThemeThickness(1, 1, 1, 1);
QMargins AWinUITheme::AutoSuggestListMargin(0, 2, 0, 2);
QMargins AWinUITheme::AutoSuggestListPadding(-1, 0, -1, 0);
QMargins AWinUITheme::AutoSuggestListViewItemMargin(12, 11, 0, 13);
QMargins AWinUITheme::ButtonBorderThemeThickness(2, 2, 2, 2);
QMargins AWinUITheme::CalendarDatePickerBorderThemeThickness(2, 2, 2, 2);
QMargins AWinUITheme::ComboBoxBorderThemeThickness(2, 2, 2, 2);
QMargins AWinUITheme::ComboBoxDropdownBorderThickness(1, 1, 1, 1);
QMargins AWinUITheme::ComboBoxDropdownBorderPadding(0, 0, 0, 0);
QMargins AWinUITheme::ComboBoxDropdownContentMargin(0, 4, 0, 4);
QMargins AWinUITheme::ComboBoxHeaderThemeMargin(0, 0, 0, 4);
QMargins AWinUITheme::ComboBoxPopupBorderThemeThickness(2, 2, 2, 2);
QMargins AWinUITheme::ComboBoxItemThemePadding(11, 5, 11, 7);
QMargins AWinUITheme::ComboBoxItemThemeTouchPadding(11, 11, 11, 13);
QMargins AWinUITheme::ComboBoxItemThemeGameControllerPadding(11, 11, 11, 13);
QMargins AWinUITheme::ContentDialogBorderWidth(1, 1, 1, 1);
QMargins AWinUITheme::ContentDialogButton1HostMargin(0, 0, 4, 0);
QMargins AWinUITheme::ContentDialogButton2HostMargin(0, 0, 0, 0);
QMargins AWinUITheme::ContentDialogContentMargin(0, 0, 0, 0);
QMargins AWinUITheme::ContentDialogContentScrollViewerMargin(0, 0, 0, 0);
QMargins AWinUITheme::ContentDialogCommandSpaceMargin(0, 24, 0, 0);
QMargins AWinUITheme::ContentDialogTitleMargin(0, 0, 0, 12);
QMargins AWinUITheme::ContentDialogPadding(24, 18, 24, 24);
QMargins AWinUITheme::DatePickerHeaderThemeMargin(0, 0, 0, 4);
QMargins AWinUITheme::DateTimeFlyoutBorderThickness(1, 1, 1, 1);
QMargins AWinUITheme::DateTimeFlyoutBorderPadding(0, 0, 0, 0);
QMargins AWinUITheme::DateTimeFlyoutButtonBorderThickness(0, 0, 0, 0);
QMargins AWinUITheme::DateTimeFlyoutContentPanelPortraitThemeMargin(0, 37, 0, 0);
QMargins AWinUITheme::DateTimeFlyoutContentPanelLandscapeThemeMargin(0, 19, 0, 0);
QMargins AWinUITheme::DateTimeFlyoutTitleThemeMargin(19, 0, 19, 17.5);
QMargins AWinUITheme::FlipViewButtonBorderThemeThickness(0, 0, 0, 0);
QMargins AWinUITheme::FlyoutContentThemeMargin(0, 0, 0, 0);
QMargins AWinUITheme::FlyoutContentThemePadding(12, 11, 12, 12);
QMargins AWinUITheme::GridViewItemCompactSelectedBorderThemeThickness(4, 4, 4, 4);
QMargins AWinUITheme::GridViewItemMultiselectBorderThickness(2.5, 2.5, 2.5, 2.5);
QMargins AWinUITheme::HandwritingViewExpandedButtonMargin(5, 6, 5, 6);
QMargins AWinUITheme::HubSectionHeaderThemeMargin(0, 0, 0, 9);
QMargins AWinUITheme::HubSectionHeaderSeeMoreThemeMargin(24, 0, 0, 11);
QMargins AWinUITheme::HyperlinkButtonBorderThemeThickness(0, 0, 0, 0);
QMargins AWinUITheme::ListPickerFlyoutPresenterMultiselectCheckBoxMargin(0, 9.5, 0, 0);
QMargins AWinUITheme::ListPickerFlyoutPresenterItemMargin(0, 0, 0, 19);
QMargins AWinUITheme::PickerFlyoutContentPanelLandscapeThemeMargin(19, 19, 19, 0);
QMargins AWinUITheme::PickerFlyoutContentPanelPortraitThemeMargin(19, 37, 19, 0);
QMargins AWinUITheme::PickerFlyoutTitleThemeMargin(0, 0, 0, 32.5);
QMargins AWinUITheme::PivotHeaderItemMargin(12, 0, 12, 0);
QMargins AWinUITheme::PivotItemMargin(12, 0, 12, 0);
QMargins AWinUITheme::PivotLandscapeThemePadding(12, 14, 0, 13);
QMargins AWinUITheme::PivotNavButtonBorderThemeThickness(0, 0, 0, 0);
QMargins AWinUITheme::PivotNavButtonMargin(0, 6, 0, 0);
QMargins AWinUITheme::PivotPortraitThemePadding(12, 14, 0, 13);
QMargins AWinUITheme::ProgressBarBorderThemeThickness(0, 0, 0, 0);
QMargins AWinUITheme::RepeatButtonBorderThemeThickness(2, 2, 2, 2);
QMargins AWinUITheme::ScrollBarPanningBorderThemeThickness(1, 1, 1, 1);
QMargins AWinUITheme::SearchBoxQuerySuggestionThemeMargin(12, 11, 8, 13);
QMargins AWinUITheme::SearchBoxResultSuggestionThemeMargin(12, 11, 8, 13);
QMargins AWinUITheme::SearchBoxSeparatorSuggestionThemeMargin(12, 11, 8, 13);
QMargins AWinUITheme::SearchBoxSuggestionSubcomponentThemeMargin(0, 0, 12, 0);
QMargins AWinUITheme::SearchBoxThemePadding(12, 4, 8, 4);
QMargins AWinUITheme::SearchBoxIMECandidateListSeparatorThemeThickness(0, 2, 0, 0);
QMargins AWinUITheme::SearchBoxBorderThemeThickness(2, 2, 2, 2);
QMargins AWinUITheme::SliderBorderThemeThickness(0, 0, 0, 0);
QMargins AWinUITheme::SliderHeaderThemeMargin(0, 0, 0, 4);
QMargins AWinUITheme::SplitViewLeftBorderThemeThickness(0, 0, 1, 0);
QMargins AWinUITheme::SplitViewRightBorderThemeThickness(1, 0, 0, 0);
QMargins AWinUITheme::TextControlBorderThemeThickness(2, 2, 2, 2);
QMargins AWinUITheme::TextControlMarginThemeThickness(0, 9.5, 0, 9.5);
QMargins AWinUITheme::TextControlThemePadding(10, 3, 6, 6);
QMargins AWinUITheme::HelperButtonThemePadding(0, 0, -2, 0);
QMargins AWinUITheme::TextControlPlaceholderThemePadding(12, 5, 10, 5);
QMargins AWinUITheme::TimePickerHeaderThemeMargin(0, 0, 0, 4);
QMargins AWinUITheme::TimePickerFirstHostThemeMargin(0, 0, 20, 0);
QMargins AWinUITheme::TimePickerThirdHostThemeMargin(20, 0, 0, 0);
QMargins AWinUITheme::ToggleButtonBorderThemeThickness(2, 2, 2, 2);
QMargins AWinUITheme::KeyTipBorderThemeThickness(1, 1, 1, 1);
QMargins AWinUITheme::KeyTipThemePadding(4, 4, 4, 4);

SCorner AWinUITheme::DefaultControlCornerRadius(4);

class AWinUIThemePrivate
{
public:
    AWinUIThemePrivate(AWinUITheme* q_ptr)
        : q(q_ptr)
    {
        reloadSystemStyle();
    }

    void getSystemColor(UIColorType t, QColor& color)
    {
        UISettings const ui_settings{};
        auto const accent_color{ ui_settings.GetColorValue(t) };
        color.setRgba(qRgba(accent_color.R, accent_color.G, accent_color.B, accent_color.A));
    }

    void reloadSystemStyle()
    {
        getSystemColor(UIColorType::Accent, AWinUITheme::SystemAccentColor);
        getSystemColor(UIColorType::AccentLight1, AWinUITheme::SystemAccentColorLight1);
        getSystemColor(UIColorType::AccentLight2, AWinUITheme::SystemAccentColorLight2);
        getSystemColor(UIColorType::AccentLight3, AWinUITheme::SystemAccentColorLight3);

        AWinUITheme::SystemControlBackgroundAccentBrush = AWinUITheme::SystemAccentColor;
        AWinUITheme::SystemControlBackgroundAltHighBrush = AWinUITheme::SystemAltHighColor;
        AWinUITheme::SystemControlBackgroundAltMediumHighBrush = AWinUITheme::SystemAltMediumHighColor;
        AWinUITheme::SystemControlBackgroundAltMediumBrush = AWinUITheme::SystemAltMediumColor;
        AWinUITheme::SystemControlBackgroundAltMediumLowBrush = AWinUITheme::SystemAltMediumLowColor;
        AWinUITheme::SystemControlBackgroundBaseHighBrush = AWinUITheme::SystemBaseHighColor;
        AWinUITheme::SystemControlBackgroundBaseLowBrush = AWinUITheme::SystemBaseLowColor;
        AWinUITheme::SystemControlBackgroundBaseMediumBrush = AWinUITheme::SystemBaseMediumColor;
        AWinUITheme::SystemControlBackgroundBaseMediumHighBrush = AWinUITheme::SystemBaseMediumHighColor;
        AWinUITheme::SystemControlBackgroundBaseMediumLowBrush = AWinUITheme::SystemBaseMediumLowColor;
        AWinUITheme::SystemControlBackgroundChromeBlackHighBrush = AWinUITheme::SystemChromeBlackHighColor;
        AWinUITheme::SystemControlBackgroundChromeBlackMediumBrush = AWinUITheme::SystemChromeBlackMediumColor;
        AWinUITheme::SystemControlBackgroundChromeBlackLowBrush = AWinUITheme::SystemChromeBlackLowColor;
        AWinUITheme::SystemControlBackgroundChromeBlackMediumLowBrush = AWinUITheme::SystemChromeBlackMediumLowColor;
        AWinUITheme::SystemControlBackgroundChromeMediumBrush = AWinUITheme::SystemChromeMediumColor;
        AWinUITheme::SystemControlBackgroundChromeMediumLowBrush = AWinUITheme::SystemChromeMediumLowColor;
        AWinUITheme::SystemControlBackgroundChromeWhiteBrush = AWinUITheme::SystemChromeWhiteColor;
        AWinUITheme::SystemControlBackgroundListLowBrush = AWinUITheme::SystemListLowColor;
        AWinUITheme::SystemControlBackgroundListMediumBrush = AWinUITheme::SystemListMediumColor;
        AWinUITheme::SystemControlDisabledAccentBrush = AWinUITheme::SystemAccentColor;
        AWinUITheme::SystemControlDisabledBaseHighBrush = AWinUITheme::SystemBaseHighColor;
        AWinUITheme::SystemControlDisabledBaseLowBrush = AWinUITheme::SystemBaseLowColor;
        AWinUITheme::SystemControlDisabledBaseMediumLowBrush = AWinUITheme::SystemBaseMediumLowColor;
        AWinUITheme::SystemControlDisabledChromeDisabledHighBrush = AWinUITheme::SystemChromeDisabledHighColor;
        AWinUITheme::SystemControlDisabledChromeDisabledLowBrush = AWinUITheme::SystemChromeDisabledLowColor;
        AWinUITheme::SystemControlDisabledChromeHighBrush = AWinUITheme::SystemChromeHighColor;
        AWinUITheme::SystemControlDisabledChromeMediumLowBrush = AWinUITheme::SystemChromeMediumLowColor;
        AWinUITheme::SystemControlDisabledListMediumBrush = AWinUITheme::SystemListMediumColor;
        AWinUITheme::SystemControlDisabledTransparentBrush = Qt::transparent;
        AWinUITheme::SystemControlFocusVisualPrimaryBrush = AWinUITheme::SystemBaseHighColor;
        AWinUITheme::SystemControlFocusVisualSecondaryBrush = AWinUITheme::SystemAltMediumColor;
        AWinUITheme::SystemControlRevealFocusVisualBrush = AWinUITheme::SystemAccentColor;
        AWinUITheme::SystemControlForegroundAccentBrush = AWinUITheme::SystemAccentColor;
        AWinUITheme::SystemControlForegroundAltHighBrush = AWinUITheme::SystemAltHighColor;
        AWinUITheme::SystemControlForegroundAltMediumHighBrush = AWinUITheme::SystemAltMediumHighColor;
        AWinUITheme::SystemControlForegroundBaseHighBrush = AWinUITheme::SystemBaseHighColor;
        AWinUITheme::SystemControlForegroundBaseLowBrush = AWinUITheme::SystemBaseLowColor;
        AWinUITheme::SystemControlForegroundBaseMediumBrush = AWinUITheme::SystemBaseMediumColor;
        AWinUITheme::SystemControlForegroundBaseMediumHighBrush = AWinUITheme::SystemBaseMediumHighColor;
        AWinUITheme::SystemControlForegroundBaseMediumLowBrush = AWinUITheme::SystemBaseMediumLowColor;
        AWinUITheme::SystemControlForegroundChromeBlackHighBrush = AWinUITheme::SystemChromeBlackHighColor;
        AWinUITheme::SystemControlForegroundChromeHighBrush = AWinUITheme::SystemChromeHighColor;
        AWinUITheme::SystemControlForegroundChromeMediumBrush = AWinUITheme::SystemChromeMediumColor;
        AWinUITheme::SystemControlForegroundChromeWhiteBrush = AWinUITheme::SystemChromeWhiteColor;
        AWinUITheme::SystemControlForegroundChromeDisabledLowBrush = AWinUITheme::SystemChromeDisabledLowColor;
        AWinUITheme::SystemControlForegroundChromeGrayBrush = AWinUITheme::SystemChromeGrayColor;
        AWinUITheme::SystemControlForegroundListLowBrush = AWinUITheme::SystemListLowColor;
        AWinUITheme::SystemControlForegroundListMediumBrush = AWinUITheme::SystemListMediumColor;
        AWinUITheme::SystemControlForegroundTransparentBrush = Qt::transparent;
        AWinUITheme::SystemControlForegroundChromeBlackMediumBrush = AWinUITheme::SystemChromeBlackMediumColor;
        AWinUITheme::SystemControlForegroundChromeBlackMediumLowBrush = AWinUITheme::SystemChromeBlackMediumLowColor;
        AWinUITheme::SystemControlHighlightAccentBrush = AWinUITheme::SystemAccentColor;
        AWinUITheme::SystemControlHighlightAltAccentBrush = AWinUITheme::SystemAccentColor;
        AWinUITheme::SystemControlHighlightAltAltHighBrush = AWinUITheme::SystemAltHighColor;
        AWinUITheme::SystemControlHighlightAltBaseHighBrush = AWinUITheme::SystemBaseHighColor;
        AWinUITheme::SystemControlHighlightAltBaseLowBrush = AWinUITheme::SystemBaseLowColor;
        AWinUITheme::SystemControlHighlightAltBaseMediumBrush = AWinUITheme::SystemBaseMediumColor;
        AWinUITheme::SystemControlHighlightAltBaseMediumHighBrush = AWinUITheme::SystemBaseMediumHighColor;
        AWinUITheme::SystemControlHighlightAltAltMediumHighBrush = AWinUITheme::SystemAltMediumHighColor;
        AWinUITheme::SystemControlHighlightAltBaseMediumLowBrush = AWinUITheme::SystemBaseMediumLowColor;
        AWinUITheme::SystemControlHighlightAltListAccentHighBrush = AWinUITheme::SystemAccentColor;
        AWinUITheme::SystemControlHighlightAltListAccentHighBrush.setAlphaF(0.9);
        AWinUITheme::SystemControlHighlightAltListAccentLowBrush = AWinUITheme::SystemAccentColor;
        AWinUITheme::SystemControlHighlightAltListAccentLowBrush .setAlphaF(0.6);
        AWinUITheme::SystemControlHighlightAltListAccentMediumBrush = AWinUITheme::SystemAccentColor;
        AWinUITheme::SystemControlHighlightAltListAccentMediumBrush.setAlphaF(0.8);
        AWinUITheme::SystemControlHighlightAltChromeWhiteBrush = AWinUITheme::SystemChromeWhiteColor;
        AWinUITheme::SystemControlHighlightAltTransparentBrush = Qt::transparent;
        AWinUITheme::SystemControlHighlightBaseHighBrush = AWinUITheme::SystemBaseHighColor;
        AWinUITheme::SystemControlHighlightBaseLowBrush = AWinUITheme::SystemBaseLowColor;
        AWinUITheme::SystemControlHighlightBaseMediumBrush = AWinUITheme::SystemBaseMediumColor;
        AWinUITheme::SystemControlHighlightBaseMediumHighBrush = AWinUITheme::SystemBaseMediumHighColor;
        AWinUITheme::SystemControlHighlightBaseMediumLowBrush = AWinUITheme::SystemBaseMediumLowColor;
        AWinUITheme::SystemControlHighlightChromeAltLowBrush = AWinUITheme::SystemChromeAltLowColor;
        AWinUITheme::SystemControlHighlightChromeHighBrush = AWinUITheme::SystemChromeHighColor;
        AWinUITheme::SystemControlHighlightListAccentHighBrush = AWinUITheme::SystemAccentColor;
        AWinUITheme::SystemControlHighlightListAccentHighBrush.setAlphaF(0.9);
        AWinUITheme::SystemControlHighlightListAccentLowBrush = AWinUITheme::SystemAccentColor;
        AWinUITheme::SystemControlHighlightListAccentLowBrush.setAlphaF(0.6);
        AWinUITheme::SystemControlHighlightListAccentMediumBrush = AWinUITheme::SystemAccentColor;
        AWinUITheme::SystemControlHighlightListAccentMediumBrush.setAlphaF(0.8);
        AWinUITheme::SystemControlHighlightListMediumBrush = AWinUITheme::SystemListMediumColor;
        AWinUITheme::SystemControlHighlightListLowBrush = AWinUITheme::SystemListLowColor;
        AWinUITheme::SystemControlHighlightChromeWhiteBrush = AWinUITheme::SystemChromeWhiteColor;
        AWinUITheme::SystemControlHighlightTransparentBrush = Qt::transparent;
        AWinUITheme::SystemControlHyperlinkTextBrush = AWinUITheme::SystemAccentColor;
        AWinUITheme::SystemControlHyperlinkBaseHighBrush = AWinUITheme::SystemBaseHighColor;
        AWinUITheme::SystemControlHyperlinkBaseMediumBrush = AWinUITheme::SystemBaseMediumColor;
        AWinUITheme::SystemControlHyperlinkBaseMediumHighBrush = AWinUITheme::SystemBaseMediumHighColor;
        AWinUITheme::SystemControlPageBackgroundAltMediumBrush = AWinUITheme::SystemAltMediumColor;
        AWinUITheme::SystemControlPageBackgroundAltHighBrush = AWinUITheme::SystemAltHighColor;
        AWinUITheme::SystemControlPageBackgroundMediumAltMediumBrush = AWinUITheme::SystemAltMediumColor;
        AWinUITheme::SystemControlPageBackgroundBaseLowBrush = AWinUITheme::SystemBaseLowColor;
        AWinUITheme::SystemControlPageBackgroundBaseMediumBrush = AWinUITheme::SystemBaseMediumColor;
        AWinUITheme::SystemControlPageBackgroundListLowBrush = AWinUITheme::SystemListLowColor;
        AWinUITheme::SystemControlPageBackgroundChromeLowBrush = AWinUITheme::SystemChromeLowColor;
        AWinUITheme::SystemControlPageBackgroundChromeMediumLowBrush = AWinUITheme::SystemChromeMediumLowColor;
        AWinUITheme::SystemControlPageBackgroundTransparentBrush = Qt::transparent;
        AWinUITheme::SystemControlPageTextBaseHighBrush = AWinUITheme::SystemBaseHighColor;
        AWinUITheme::SystemControlPageTextBaseMediumBrush = AWinUITheme::SystemBaseMediumColor;
        AWinUITheme::SystemControlPageTextChromeBlackMediumLowBrush = AWinUITheme::SystemChromeBlackMediumLowColor;
        AWinUITheme::SystemControlTransparentBrush = Qt::transparent;
        AWinUITheme::SystemControlErrorTextForegroundBrush = AWinUITheme::SystemErrorTextColor;

        AWinUITheme::DefaultControlBorderUnderLineColor = AWinUITheme::SystemAccentColorLight2;
    }

    AWinUITheme* q = nullptr;
};

AWinUITheme::AWinUITheme()
    : d(new AWinUIThemePrivate(this))
{
}

AWinUITheme::~AWinUITheme()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

class WinUIThemeLoader
{
public:
    WinUIThemeLoader()
    {
        AWinUITheme temp;
    }
}_loader;

APROCH_NAMESPACE_END
