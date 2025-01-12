#include "stdafx.h"
#include "AWinUIStyle.h"
#include "AUIStyleObject.h"
#include "Widget/Private/AToggleSwitch_p.h"

#include <qdrawutil.h>
#include <QStyleFactory>
#include <QPainterPath>

APROCH_NAMESPACE_BEGIN

class AWinUIStylePrivate
{
public:

};

AWinUIStyle::AWinUIStyle()
    : QProxyStyle(QStyleFactory::create("WinUI"))
    , d_ptr(new AWinUIStylePrivate())
{
}

AWinUIStyle::~AWinUIStyle()
{
}

void AWinUIStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option,
                                QPainter* painter, const QWidget* widget) const
{
    return QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void AWinUIStyle::drawControl(ControlElement element, const QStyleOption* option,
                              QPainter* painter, const QWidget* widget) const
{
    QProxyStyle::drawControl(element, option, painter, widget);
}

void AWinUIStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, 
                                     QPainter* painter, const QWidget* widget) const
{
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

int AWinUIStyle::pixelMetric(PixelMetric pm, const QStyleOption* opt, const QWidget* widget) const
{
    switch (pm)
    {
    case PM_CheckBoxLabelSpacing:
    case PM_RadioButtonLabelSpacing:
    {
        // 没有文字则不给间距
        if (const QStyleOptionButton* btnOpt = static_cast<const QStyleOptionButton*>(opt))
        {
            if (btnOpt->text.isEmpty())
                return 0;
        }
    }
        break;

    case PM_ExclusiveIndicatorWidth:
        if (qobject_cast<const AToggleSwitchIndicatorContainer*>(widget) ||
            qobject_cast<const AToggleSwitch*>(widget))
            return int(AGraphicsToolkit::dpiScaled(40, opt));
        break;
    case PM_ExclusiveIndicatorHeight:
        if (qobject_cast<const AToggleSwitchIndicatorContainer*>(widget) ||
            qobject_cast<const AToggleSwitch*>(widget))
            return int(AGraphicsToolkit::dpiScaled(20, opt));
    break;
    default:
        break;
    }

    return QProxyStyle::pixelMetric(pm, opt, widget);
}

QPalette AWinUIStyle::standardPalette() const
{
    return QProxyStyle::standardPalette();
}

QRect AWinUIStyle::subElementRect(SubElement element, const QStyleOption* opt, const QWidget* widget) const
{
    return QProxyStyle::subElementRect(element, opt, widget);
}

APROCH_NAMESPACE_END
