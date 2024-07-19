#include "stdafx.h"
#include "ASplitterHandle.h"
#include "Common/AGraphicsToolkit.h"

#include <QPainterPath>

APROCH_NAMESPACE_BEGIN

ASplitterHandle::ASplitterHandle(QSplitter* parent)
    : ASplitterHandle(Qt::Orientation::Vertical, parent)
{
}

ASplitterHandle::ASplitterHandle(Qt::Orientation o, QSplitter* parent)
    : QSplitterHandle(o, parent)
    , m_barColor(128, 128, 128, 180)
{
#ifndef QT_NO_CURSOR
    setCursor(o == Qt::Horizontal ? Qt::SizeHorCursor : Qt::SizeVerCursor);
#endif
}

ASplitterHandle::~ASplitterHandle()
{
}

void ASplitterHandle::moveSplitterEx(int pos)
{
    moveSplitter(pos);
}

QSize ASplitterHandle::sizeHint() const
{
    const QSize defSh = QSplitterHandle::sizeHint();
    return orientation() == Qt::Horizontal ? QSize(12, defSh.height()) : QSize(defSh.width(), 12);

}

void ASplitterHandle::paintEvent(QPaintEvent* evt)
{
    APROCH_USE_STYLE_SHEET();

    QSplitterHandle::paintEvent(evt);

    const bool isHor = orientation() == Qt::Horizontal;

    QPainter painter(this);

    // draw bar
    const QPoint c = (rect().topLeft() + rect().bottomRight()) / 2;
    constexpr int barWidth = 4;
    constexpr int barHeight = 30;
    constexpr int barBorderRadius = 3;

    QRect barRect;
    if (isHor)
        barRect = QRect(c.x() - barWidth / 2, c.y() - barHeight / 2, barWidth, barHeight);
    else
        barRect = QRect(c.x() - barHeight / 2, c.y() - barWidth / 2, barHeight, barWidth);

    QPainterPath path;
    AGraphicsToolkit::drawRoundedRect(path, barRect, barBorderRadius);
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_barColor);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPath(path);
}

APROCH_NAMESPACE_END
