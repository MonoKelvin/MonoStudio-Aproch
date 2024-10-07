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
    switch (element)
    {
    case PE_FrameMenu: //整个菜单widget的边框色
        //qDrawShadeRect(painter, option->rect, option->palette, option->state & State_Sunken, 1);
    {
        painter->save();

        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(45, 45, 45));

        QPainterPath path;
        AGraphicsToolkit::drawRoundedRect(path, option->rect, AprochDefaultBorderRadius);

        painter->drawPath(path);

        painter->restore();
    }
        break;
    case PE_PanelMenu: //整个菜单widget的背景色
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(45, 45, 45));

        QPainterPath path;
        AGraphicsToolkit::drawRoundedRect(path, option->rect, AprochDefaultBorderRadius);

        painter->drawPath(path);

        painter->restore();
    }
    break;
    default:
        QProxyStyle::drawPrimitive(element, option, painter, widget);
        break;
    }
}

void AWinUIStyle::drawControl(ControlElement element, const QStyleOption* option,
                              QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case CE_MenuEmptyArea: //空白区域不处理，看源码可看见最后恢得成了整个菜单区域，有需要的可以处理如蒙上一层半透明
        break;
    //case CE_MenuItem: //画菜单项
    //    if (const QStyleOptionMenuItem* menuItem = qstyleoption_cast<const QStyleOptionMenuItem*>(option))
    //    {
    //        painter->save();
    //        bool hover = menuItem->state & State_Selected && menuItem->state & State_Enabled;
    //        if (hover)
    //        { //鼠标滑过，先画一个矩形，使后面的文字不会被覆盖
    //            painter->setBrush(menuItem->palette.brush(QPalette::Text));
    //            painter->drawRect(menuItem->rect);
    //        }
    //        constexpr int leftW = qRound(25.2);
    //        if (menuItem->menuItemType == QStyleOptionMenuItem::Separator)
    //        { //画分隔线
    //            painter->setPen(QColor(Qt::white));
    //            painter->drawLine(leftW, menuItem->rect.center().y(), menuItem->rect.right(), menuItem->rect.center().y());
    //        }
    //        else if (menuItem->menuItemType == QStyleOptionMenuItem::SubMenu)
    //        { //画子菜单
    //            QRect textRect = menuItem->rect;
    //            textRect.moveLeft(leftW);
    //            painter->setPen(menuItem->palette.text().color());
    //            painter->drawText(textRect, Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextSingleLine | Qt::TextDontClip, menuItem->text);
    //            int x, y, w, h;
    //            menuItem->rect.getRect(&x, &y, &w, &h);
    //            int dim = (h - 2 * 2) / 2;
    //            int xpos = x + w - 2 - 1 - dim;
    //            QRect  vSubMenuRect = visualRect(option->direction, menuItem->rect, QRect(xpos, y + h / 2 - dim / 2, dim, dim));
    //            painter->setBrush(menuItem->palette.brush(QPalette::Text));
    //            painter->drawEllipse(vSubMenuRect);
    //        }
    //        else
    //        { //文字菜单项
    //            painter->setPen(menuItem->palette.color(QPalette::Text));
    //            QRect textRect = menuItem->rect;
    //            QFont font = menuItem->font;
    //            painter->setFont(font);
    //            constexpr int text_flags = Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine;
    //            QString s = menuItem->text;
    //            QString shortcutText = s.mid(s.indexOf('\t') + 1);
    //            s = s.left(s.indexOf('\t'));
    //            if (!shortcutText.isEmpty())
    //            { //画快捷键文字
    //                int fw = menuItem->fontMetrics.horizontalAdvance(shortcutText);
    //                textRect.moveLeft(menuItem->rect.right() - fw);
    //                textRect = visualRect(menuItem->direction, menuItem->rect, textRect);
    //                painter->drawText(textRect, text_flags, shortcutText);
    //            }
    //            textRect.moveLeft(leftW);
    //            textRect = visualRect(menuItem->direction, menuItem->rect, textRect);
    //            painter->drawText(textRect, text_flags, s);
    //        }
    //        painter->restore();
    //    }
    //    break;
    default:
        QProxyStyle::drawControl(element, option, painter, widget);
        break;
    }
}

void AWinUIStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, 
                                     QPainter* painter, const QWidget* widget) const
{
    // TODO

    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

int AWinUIStyle::pixelMetric(PixelMetric pm, const QStyleOption* opt, const QWidget* widget) const
{
    if (pm == PM_MenuPanelWidth)
        return 12;        // 调整菜单边框宽度, 以绘制阴影

    return QProxyStyle::pixelMetric(pm, opt, widget);
}

QPalette AWinUIStyle::standardPalette() const
{
    return QProxyStyle::standardPalette();
}

APROCH_NAMESPACE_END
