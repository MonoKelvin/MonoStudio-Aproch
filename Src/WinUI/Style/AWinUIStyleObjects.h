#pragma once
#include "Widget/Style/AUIStyleObject.h"

APROCH_NAMESPACE_BEGIN

/** @brief 亚克力画笔参数，https://learn.microsoft.com/zh-cn/windows/apps/design/style/acrylic */
struct SAcylicMaterial
{
    enum AcrylicBackgroundSource
    {
        /** @brief 应用窗口后面的内容的画笔示例。*/
        HostBackdrop,

        /** @brief 应用内容中的画笔示例。 */
        Backdrop,
    };

    QColor TintColor;
    QColor FallbackColor;
    qreal TintOpacity;
    qreal TintLuminosityOpacity;
    AcrylicBackgroundSource BackgroundSource;
};
//Q_DECLARE_METATYPE(SAcylicMaterial);

class APROCH_API AWinUIFrameStyleObject : public AUIStyleObject
{
public:
    virtual ~AWinUIFrameStyleObject() = default;

public:
    /** @brief 背景材质（Mica/Acylic/None） */
    static const char BackdropMaterial[];

protected:
    virtual void initProperties(QWidget* widget) override;
    void applyStyle(QWidget* widget);
};

// ----------------------------------------------------------------------------------------------------

class APROCH_API ATextBoxStyleObject : public AWinUIFrameStyleObject
{
public:
    static const char ClearButtonSize[];
    static const char ClearButtonMargins[];

protected:
    virtual void initProperties(QWidget* widget) override;
};


/* ----------------------------------------------------------------------------------------------------
    default style defines
---------------------------------------------------------------------------------------------------- */

extern APROCH_API const SCornerF FrameDefaultBorderRadius;

extern APROCH_API const SCornerF TextBoxDefaultBorderRadius;
extern APROCH_API const QMargins TextBoxDefaultContentMargins;


APROCH_NAMESPACE_END