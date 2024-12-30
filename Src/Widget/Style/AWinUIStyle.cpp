#include "stdafx.h"
#include "AWinUIStyle.h"
#include "AUIStyleObject.h"

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
    return QProxyStyle::pixelMetric(pm, opt, widget);
}

QPalette AWinUIStyle::standardPalette() const
{
    return QProxyStyle::standardPalette();
}

APROCH_NAMESPACE_END
