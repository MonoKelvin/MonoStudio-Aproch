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
    return orientation() == Qt::Vertical ? QSize(12, defSh.height()) : QSize(defSh.width(), 12);

}

void ASplitterHandle::paintEvent(QPaintEvent* evt)
{
    APROCH_USE_STYLE_SHEET();

    QSplitterHandle::paintEvent(evt);

    QPainter painter(this);

    // draw line
    const QPoint c = rect().center();
    constexpr int barWidth = 6;
    constexpr int barHeight = 30;
    constexpr int barBorderRadius = 3;
    QRect barRect(c.x() - barWidth / 2, c.y() - barHeight / 2, barWidth, barHeight);

    QPainterPath path;
    AGraphicsToolkit::drawRoundedRect(path, barRect, barBorderRadius);
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::gray);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPath(path);
}

APROCH_NAMESPACE_END
