/****************************************************************************
 * @file    AScrollWidgetBar.cpp
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
#include "stdafx.h"
#include "AScrollWidgetBar.h"
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QStyle>
#include "Style/ACommonStyle_p.h"

// 
// The most of the following code is copied from Qtitan.
// 
// Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
// Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
// 

APROCH_NAMESPACE_BEGIN

static const int ScrollWidgetBarPageIndent = 25;
static const int ScrollWidgetBarItemIndent = 20;

AAbstractScrollWidgetBar::AAbstractScrollWidgetBar(QWidget* parent, Qt::Orientation orientation)
    : QWidget(parent), m_orientation(orientation), m_moveType(MoveByPage),
      m_columnNum(0), m_columnFirst(0), m_columnLast(0)
    , m_viewWidget(nullptr), m_pageIndent(-1)
    , m_margins(0, 0, 0, 0)
    , m_itemIndent(ScrollWidgetBarItemIndent)
    , m_maxViewItemCount(0)
    , m_scrollButtonsBeside(false)
{
    setFocusPolicy(Qt::WheelFocus);

    m_prevButton = new AScrollWidgetBarButton(this);
    connect(m_prevButton, SIGNAL(clicked()), this, SLOT(movePrev()));
    m_nextButton = new AScrollWidgetBarButton(this);
    connect(m_nextButton, SIGNAL(clicked()), this, SLOT(moveNext()));

    m_animation = new QPropertyAnimation(nullptr, "geometry");
    m_animation->setDuration(150);
    m_animation->setEasingCurve(QEasingCurve::InOutExpo /* QEasingCurve::OutCubic */);
    connect(m_animation, SIGNAL(finished()), this, SLOT(scrollAnimationFinished()));
}

AAbstractScrollWidgetBar::~AAbstractScrollWidgetBar()
{
    delete m_animation;
    m_animation = nullptr;
    delete m_viewWidget;
    m_viewWidget = nullptr;
    delete m_prevButton;
    m_prevButton = nullptr;
    delete m_nextButton;
    m_nextButton = nullptr;
}

Qt::Orientation AAbstractScrollWidgetBar::orientation() const
{
    return m_orientation;
}

void AAbstractScrollWidgetBar::setOrientation(Qt::Orientation orientation)
{
    m_orientation = orientation;
}

bool AAbstractScrollWidgetBar::scrollButtonsBeside() const
{
    return m_scrollButtonsBeside;
}

void AAbstractScrollWidgetBar::setScrollButtonsBeside(bool beside)
{
    if (m_scrollButtonsBeside == beside)
        return;
    m_scrollButtonsBeside = beside;
    updateLayout();
}

QWidget* AAbstractScrollWidgetBar::createViewWidget()
{
    QWidget* ret = new QWidget(this);
    ret->setAttribute(Qt::WA_NoSystemBackground, true);
    return ret;
}

QWidget* AAbstractScrollWidgetBar::viewWidget() const
{
    if (m_viewWidget == nullptr)
    {
        AAbstractScrollWidgetBar* bar = const_cast<AAbstractScrollWidgetBar *>(this);
        bar->m_viewWidget = bar->createViewWidget();
        m_animation->setTargetObject(bar->m_viewWidget);
    }
    return m_viewWidget;
}

int AAbstractScrollWidgetBar::pageIndent() const
{
    if (m_pageIndent != -1)
        return m_pageIndent;
    QSize sh = m_prevButton->sizeHint();
    return sh.height();
}

void AAbstractScrollWidgetBar::setPageIndent(int indent)
{
    m_pageIndent = indent;
}

const QMargins& AAbstractScrollWidgetBar::margins() const
{
    return m_margins;
}

void AAbstractScrollWidgetBar::setMargins(const QMargins& margins)
{
    m_margins = margins;
}

/*!
* Returns indent between items.
*/
int AAbstractScrollWidgetBar::itemIndent() const
{
    return m_itemIndent;
}

/*!
* Sets \a indent between items.
*/
void AAbstractScrollWidgetBar::setItemIndent(int indent)
{
    if (m_itemIndent == indent)
        return;
    m_itemIndent = indent;
}

void AAbstractScrollWidgetBar::setAmimationSettings(int duration, const QEasingCurve& curve)
{
    m_animation->setDuration(duration);
    m_animation->setEasingCurve(curve);
}

int AAbstractScrollWidgetBar::maxViewItemCount() const
{
    return m_maxViewItemCount;
}

void AAbstractScrollWidgetBar::setMaxViewItemCount(int count)
{
    m_maxViewItemCount = count;
}

QSize AAbstractScrollWidgetBar::sizeHint() const
{
    QSize ret = QSize(0, 0);

    int count = 0;
    for (int i = 0; i < getColumnCount(); ++i)
    {
        /*QWidget* widget = *it;
        if (widget->isHidden())
            continue;*/

        QSize s = getColumnSize(i);

        if (orientation() == Qt::Horizontal)
        {
            ret.rheight() = qMax(ret.height(), s.height());
            if (maxViewItemCount() <= 0 || maxViewItemCount() > count)
            {
                if (ret.width() > 0)
                    ret.rwidth() += itemIndent();
                ret.rwidth() += s.width();
            }
        }
        else
        {
            ret.rwidth() = qMax(ret.width(), s.width());
            if (maxViewItemCount() <= 0 || maxViewItemCount() > count)
            {
                if (ret.height() > 0)
                    ret.rheight() += itemIndent();
                ret.rheight() += s.height();
            }
        }
        count++;
    }

    if (orientation() == Qt::Horizontal)
    {
        ret.rwidth() += pageIndent() * 2;
    }
    else
    {
        ret.rheight() += pageIndent() * 2;
    }

    ret.rwidth() += margins().left() + margins().right();
    ret.rheight() += margins().top() + margins().bottom();

    return ret;
}

/*! \reimp */
void AAbstractScrollWidgetBar::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    updateLayout();
}

/*! \reimp */
void AAbstractScrollWidgetBar::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
#if 0
    QPainter painter(this);
    QRect r = QRect(0, 0, width(), height());
    painter.fillRect(r, /*palette().color(backgroundRole())*/QColor(255, 0, 0, 20));
#endif
}

/*! \reimp */
void AAbstractScrollWidgetBar::keyPressEvent(QKeyEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        int key = event->key();
        bool _prev = (orientation() == Qt::Horizontal && key == Qt::Key_Left) || (orientation() == Qt::Vertical && key == Qt::Key_Up);
        bool _next = (orientation() == Qt::Horizontal && key == Qt::Key_Right) || (orientation() == Qt::Vertical && key == Qt::Key_Down);
        if (_prev)
            movePrev();
        else if (_next)
            moveNext();

        if (!_prev && !_next)
            event->ignore();
    }
}

/*! \reimp */
#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
void AAbstractScrollWidgetBar::wheelEvent(QWheelEvent* event)
{
    bool horizontal = qAbs(event->angleDelta().x()) > qAbs(event->angleDelta().y());
    if (!horizontal && event->angleDelta().x() != 0 && orientation() == Qt::Horizontal)
        return;
    int delta = horizontal ? event->angleDelta().x() : event->angleDelta().y();
    if (delta < 0)
        moveNext();
    else
        movePrev();
    if (!m_prevButton->isHidden() || !m_nextButton->isHidden())
        event->accept();
}
#else
void AAbstractScrollWidgetBar::wheelEvent(QWheelEvent* event)
{
    if (event->delta() < 0)
        moveNext();
    else
        movePrev();
    if (!m_prevButton->isHidden() || !m_nextButton->isHidden())
        event->accept();
}
#endif

void AAbstractScrollWidgetBar::setTogetherButtonsVisible(bool visible, bool prevEnabled, bool nextEnabled)
{
    setPrevButtonVisible(visible, prevEnabled);
    setNextButtonVisible(visible, nextEnabled);
}

void AAbstractScrollWidgetBar::setPrevButtonVisible(bool visible, bool enabled)
{
    if (visible)
    {
        QRect r;
        QSize s = m_prevButton->sizeHint();
        if (orientation() == Qt::Horizontal)
        {
            if (m_scrollButtonsBeside)
            {
                s = m_nextButton->sizeHint();
                s.rheight() = height();
                s.rwidth() = s.width();
                r = QStyle::alignedRect(Qt::LeftToRight, Qt::Alignment(Qt::AlignRight | Qt::AlignTop), s,
                                        QRect(-s.width(), 0, this->width(), this->height()));
            } 
            else
            {
                s.rheight() = height();
                r = QStyle::alignedRect(Qt::LeftToRight, Qt::Alignment(Qt::AlignLeft | Qt::AlignTop), s, 
                                        QRect(0, 0, this->width(), this->height()));
            }
        }
        else
        {
            if (m_scrollButtonsBeside)
            {
                s = m_nextButton->sizeHint();
                s.rheight() = s.height();
                s.rwidth() = width();
                r = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignBottom, s, QRect(0, -s.height(), 
                                        this->width(), this->height()));
            }
            else
            {
                s.rwidth() = width();
                r = QStyle::alignedRect(Qt::LeftToRight, Qt::Alignment(Qt::AlignTop | Qt::AlignLeft), s,
                                        QRect(0, 0, this->width(), this->height()));
            }
        }

        m_prevButton->setGeometry(r);
        m_prevButton->raise();
    }


    if (visible && m_scrollButtonsBeside)
    {
        m_prevButton->setEnabled(enabled);
        m_prevButton->setVisible(true);
    }
    else
        m_prevButton->setVisible(visible);
}

void AAbstractScrollWidgetBar::setNextButtonVisible(bool visible, bool enabled)
{
    if (visible)
    {
        QRect r;
        QSize s = m_nextButton->sizeHint();
        if (orientation() == Qt::Horizontal)
        {
            s.rheight() = height();
            r = QStyle::alignedRect(Qt::LeftToRight, Qt::Alignment(Qt::AlignRight | Qt::AlignTop), s,
                                    QRect(0, 0, this->width(), this->height()));
        }
        else
        {
            s.rwidth() = width();
            r = QStyle::alignedRect(Qt::LeftToRight, Qt::Alignment(Qt::AlignBottom | Qt::AlignLeft), s, 
                                    QRect(0, 0, this->width(), this->height()));
        }
        m_nextButton->setGeometry(r);
        m_nextButton->raise();
    }
    
    if (visible && m_scrollButtonsBeside)
    {
        m_nextButton->setEnabled(enabled);
        m_nextButton->setVisible(true);
    }
    else
        m_nextButton->setVisible(visible);
}

void AAbstractScrollWidgetBar::scrollAnimationFinished()
{
    updateLayout();
}

void AAbstractScrollWidgetBar::movePrev()
{
    if (m_prevButton->isHidden() || m_animation->state() == QAbstractAnimation::Running)
        return;

    m_columnNum = nextColumn(m_columnNum, true);
    startScrollAnimation();
}

void AAbstractScrollWidgetBar::moveNext()
{
    if (m_nextButton->isHidden() || m_animation->state() == QAbstractAnimation::Running)
        return;

    m_columnNum = nextColumn(m_columnNum, false);
    startScrollAnimation();
}

void AAbstractScrollWidgetBar::moveTo(int column)
{
    if (column < 0 || column >= getColumnCount())
        return;

    int first = 0;

    while (true)
    {
        int last = nextPageColumn(first, false);
        if (first <= column && last > column)
        {
            if (m_columnNum != first)
            {
                m_columnNum = first;
                startScrollAnimation();
            }
            break;
        }
        if (first == last)
            break;
        first = nextColumn(first, false);
    }
}

int AAbstractScrollWidgetBar::getColumnLength(int column) const
{
    if (orientation() == Qt::Horizontal)
        return getColumnSize(column).width();
    else
        return getColumnSize(column).height();
}

QColor AAbstractScrollWidgetBar::getButtonColor() const
{
    return QColor();
}

int AAbstractScrollWidgetBar::calcColumnsLength(int start, int end) const
{
    int ret = 0;
    for (int i = start; i <= end; ++i)
        ret += getColumnLength(i);
    return ret;
}

bool AAbstractScrollWidgetBar::isColumnFirst(int column) const
{
    return m_columnNum > column;
}

bool AAbstractScrollWidgetBar::isColumnLast(int column) const
{
    return m_columnLast < column;
}

void AAbstractScrollWidgetBar::ensureVisible(int column)
{
    int last = m_columnLast;
    if (last > m_columnFirst)
        --last;
    if (m_columnFirst <= column && column <= last)
        return;
    moveTo(column);
}

int AAbstractScrollWidgetBar::nextColumn(int column, bool back) const
{
    if (m_moveType == MoveByPage)
        return nextPageColumn(column, back);
    if (back)
    {
        if (column > 0)
            column--;
    }
    else
    {
        if (column < getColumnCount() - 1)
            column++;
    }
    return column;
}

int AAbstractScrollWidgetBar::nextPageColumn(int column, bool back) const
{
    int w = orientation() == Qt::Horizontal ? width() - pageIndent() * 2 : height() - pageIndent() * 2;
    bool first = true;
    while (w > 0)
    {
        if (back)
        {
            if (column == 0)
                break;
        }
        else
        {
            if (column == getColumnCount())
                break;
        }
         
        w -= (getColumnLength(column) + itemIndent());

        if (w < 0 && !first)
            break;

        first = false;

        if (back)
            column--;
        else
            column++;
    }
    return column;
}

QPoint AAbstractScrollWidgetBar::getViewOffset() const
{
    int columnsLen = calcColumnsLength(0, m_columnNum - 1) + itemIndent() * m_columnNum;
    QPoint offset = orientation() == Qt::Horizontal ? QPoint(-columnsLen + pageIndent(), 0) : 
        QPoint(0, -columnsLen + pageIndent());
    return offset;
}

void AAbstractScrollWidgetBar::startScrollAnimation()
{
    QRect rect = viewWidget()->geometry();
    m_animation->setStartValue(rect);
    rect.moveTo(getViewOffset());
    m_animation->setEndValue(rect);
    m_animation->start();
    updateLayout();
}

void AAbstractScrollWidgetBar::updateLayout()
{
    if (m_columnNum < 0 || m_columnNum >= getColumnCount())
        m_columnNum = 0;

    bool prevVisible = m_columnNum > 0;
    int columnNum = nextPageColumn(m_columnNum, false);
    m_columnFirst = m_columnNum;
    m_columnLast = columnNum;

    bool nextVisible = columnNum < getColumnCount();

    QPoint offset = getViewOffset();

    int columnsLength = calcColumnsLength(0, getColumnCount() - 1);

    if (m_animation->state() != QAbstractAnimation::Running)
    {
        QSize s = sizeHint();
        QRect viewRect = orientation() == Qt::Horizontal ?
            QRect(offset, QSize(columnsLength + itemIndent() * (getColumnCount() - 1) + 
                  margins().left() + margins().right(), s.height())) :
            QRect(offset, QSize(s.width(), columnsLength + itemIndent() * (getColumnCount() - 1) + 
                  margins().top() + margins().bottom()));
        viewWidget()->setGeometry(viewRect);
    }

    if (m_scrollButtonsBeside)
        setTogetherButtonsVisible(prevVisible || nextVisible, prevVisible, nextVisible);
    else
    {
        setPrevButtonVisible(prevVisible, true);
        setNextButtonVisible(nextVisible, true);
    }
}

/* AScrollWidgetBar */
AScrollWidgetBar::AScrollWidgetBar(QWidget* parent, Qt::Orientation orientation)
    : AAbstractScrollWidgetBar(parent, orientation), m_updateCount(0)
{
}

AScrollWidgetBar::~AScrollWidgetBar()
{
}

void AScrollWidgetBar::addWidget(QWidget* widget)
{
    beginUpdate();
    widget->setParent(viewWidget());
    m_widgets.push_back(widget);
    endUpdate();
}

void AScrollWidgetBar::removeWidget(QWidget* widget)
{
    beginUpdate();
    int i = m_widgets.indexOf(widget);
    if (i >= 0)
        m_widgets.remove(i);
    endUpdate();
}

void AScrollWidgetBar::beginUpdate()
{
    m_updateCount++;
}

void AScrollWidgetBar::endUpdate()
{
    Q_ASSERT(m_updateCount > 0);
    m_updateCount--;
    if (m_updateCount == 0)
        updateItemsLayout();
}

bool AScrollWidgetBar::isUpdating() const
{
    return m_updateCount > 0;
}

QWidget* AScrollWidgetBar::getWidget(int index) const
{
    return m_widgets[index];
}

int AScrollWidgetBar::getWidgetCount() const
{
    return m_widgets.count();
}

int AScrollWidgetBar::getColumnCount() const
{
    return m_widgets.count();
}

QSize AScrollWidgetBar::getColumnSize(int column) const
{
    return m_widgets[column]->sizeHint();
}

int AScrollWidgetBar::getColumnIndex(QWidget* widget) const
{
    return m_widgets.indexOf(widget);
}

void AScrollWidgetBar::updateItemsLayout()
{
    if (m_updateCount > 0)
        return;

    QPoint topLeft = QPoint(0, 0);
    QPoint offset = QPoint(0, 0);

    offset += QPoint(margins().left(), margins().top());

    for (QVector<QWidget *>::iterator it = m_widgets.begin(); it != m_widgets.end(); ++it)
    {
        QWidget* widget = *it;
        if (widget->isHidden())
            continue;

        QSize s = widget->sizeHint();

        QRect rect = QRect(offset + topLeft, s);
        widget->setGeometry(rect);

        if (orientation() == Qt::Horizontal)
            topLeft.rx() += itemIndent() + s.width();
        else
            topLeft.ry() += itemIndent() + s.height();
    }
}

bool AScrollWidgetBar::focusNextPrevChild(bool next)
{
    QWidget* w = focusWidget();
    int index = m_widgets.indexOf(w);
    if (index == -1)
    {
        index = next ? 0 : getWidgetCount() - 1;
    }
    else
    {
        index = next ? index + 1 : index - 1;
    }
 
    if (index >= 0 && index < getWidgetCount())
    {
        w = m_widgets[index];
        w->setFocus(next ? Qt::TabFocusReason : Qt::BacktabFocusReason);
        ensureVisible(getColumnIndex(w));
        return true;
    }

    return AAbstractScrollWidgetBar::focusNextPrevChild(next);


}

/* AScrollWidgetBarButton */
AScrollWidgetBarButton::AScrollWidgetBarButton(AAbstractScrollWidgetBar* bar)
    : QAbstractButton(bar)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::NoFocus);
}

AScrollWidgetBarButton::~AScrollWidgetBarButton()
{
}

QSize AScrollWidgetBarButton::sizeHint() const
{
#if 0
    AAbstractScrollWidgetBar* bar = qobject_cast<AAbstractScrollWidgetBar *>(parentWidget());
    if (bar != nullptr)
        return QSize(bar->pageIndent(), bar->pageIndent());
#endif
    int size = ScrollWidgetBarPageIndent;
    int val = style()->pixelMetric((QStyle::PixelMetric)ACommonStyle::PM_BaseScrollButtonItem, nullptr, this);
    if (val > 0)
        size = val;
    return QSize(size, size);
}

void AScrollWidgetBarButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    AAbstractScrollWidgetBar* bar = qobject_cast<AAbstractScrollWidgetBar *>(parentWidget());
    if (bar == nullptr)
        return;
    QPainter painter(this);
    QStyleOptionToolButton opt;
    opt.initFrom(this);
    opt.subControls = QStyle::SC_ToolButton;
    opt.activeSubControls = QStyle::SC_None;
    opt.features = QStyleOptionToolButton::Arrow;

    if (isDown())
    {
        opt.state |= QStyle::State_Sunken;
        opt.activeSubControls |= QStyle::SC_ToolButton;
    }
    else
        opt.state |= QStyle::State_Raised;

    QColor color = bar->getButtonColor();
    if (color.isValid())
    {
        opt.palette.setColor(QPalette::Window, color);
        opt.palette.setColor(QPalette::Highlight, color.darker());
    }

    if (bar->orientation() == Qt::Horizontal)
        opt.arrowType = bar->m_prevButton == this ? Qt::LeftArrow : Qt::RightArrow;
    else
        opt.arrowType = bar->m_prevButton == this ? Qt::UpArrow : Qt::DownArrow;
    style()->drawComplexControl(QStyle::CC_ToolButton, &opt, &painter, this);
}

void AScrollWidgetBarButton::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    update();
}

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
void AScrollWidgetBarButton::enterEvent(QEnterEvent *event)
#else
void AScrollWidgetBarButton::enterEvent(QEvent *event)
#endif
{
    Q_UNUSED(event);
    update();
}

void AScrollWidgetBarButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    update();
}

APROCH_NAMESPACE_END