/****************************************************************************
 * @file    ARibbonGallery.cpp
 * @date    2023-07-02 
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
#include <QApplication>
#include <QPainter>
#include <QToolButton>
#include <QAction>
#include <QStyleOption>
#include <QScrollBar>
#include <QToolTip>
#include <QBitmap>
#include <qevent.h>
#include <qstyleoption.h>
#include <QScreen>
#include <QWidgetAction>
#include <qmath.h>

#include "Widget/Style/ACommonStyle.h"
#include "Widget/Style/ACommonStyle_p.h"
#include "Widget/Style/AStyleOption.h"
#include "Widget/Style/AStyleHelper.h"
#include "ARibbonGallery.h"
#include "ARibbonGallery_p.h"
#include "ARibbonBar_p.h"
#include "AOfficePopupMenu.h"

// 
// The most of the following code is copied from Qtitan.
// 
// Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
// Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
// 

APROCH_NAMESPACE_BEGIN

ARibbonGalleryItemPrivate::ARibbonGalleryItemPrivate()
{
    m_index = -1;
    m_separator = false;
    m_enabled = true;
    m_visible = true;
}

void ARibbonGalleryItemPrivate::init()
{
    A_P(ARibbonGalleryItem);
    p.setSizeHint(QSize(0, 0));
}

ARibbonGalleryGroupPrivate::ARibbonGalleryGroupPrivate()
{
    m_sizeItem = QSize(0, 0);
    m_clipItems = true;
}

ARibbonGalleryGroupPrivate::~ARibbonGalleryGroupPrivate()
{
    for (int i = 0, count = m_viewWidgets.size(); count > i; ++i)
    {
        if (ARibbonGallery* gallery = qobject_cast<ARibbonGallery*>(m_viewWidgets.at(i)))
            gallery->aproch_d().m_items = nullptr; 
    }
}

void ARibbonGalleryGroupPrivate::init()
{
}

void ARibbonGalleryGroupPrivate::updateIndexes(int start /*= 0*/)
{
    A_P(ARibbonGalleryGroup);
    for (int i = start; i < m_arrItems.size(); ++i)
        p.item(i)->aproch_d().m_index = i;
}

void ARibbonGalleryGroupPrivate::itemsChanged()
{
    for (int i = 0; i < m_viewWidgets.size(); ++i)
    {
        if (ARibbonGallery* gallery = qobject_cast<ARibbonGallery*>(m_viewWidgets.at(i)))
        {
            gallery->aproch_d().m_checkedItem = nullptr; 
            gallery->aproch_d().m_selected  = -1;
            gallery->aproch_d().m_scrollPos = 0;
            gallery->aproch_d().layoutItems();

            if (gallery->aproch_d().m_preview)
                gallery->aproch_d().m_preview = false;
            gallery->update();
        }
    }
}

void ARibbonGalleryGroupPrivate::redrawWidget()
{
    for (int i = 0, count = m_viewWidgets.size(); count > i; ++i)
        m_viewWidgets.at(i)->update();
}

void ARibbonGalleryGroupPrivate::clear()
{
    ARibbonGalleryItem* item = nullptr;
    foreach (item, m_arrItems)
        delete item;
    m_arrItems.clear();
}

void ARibbonGalleryButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter p(this);
    QStyleOptionToolButton option;
    initStyleOption(&option);
    style()->drawComplexControl(static_cast<QStyle::ComplexControl>(ACommonStyle::CC_RibbonGalleryButton), &option, &p, this);
}

ARibbonGalleryPrivate::ARibbonGalleryPrivate()
{
    m_showBorders        = false;
    m_showLabels         = true;
    m_hideSelection      = false;
    m_transparent        = false;
    m_keyboardSelected   = false; 
    m_preview            = false;
    m_pressed            = false;
    m_animation          = false;
    m_autoWidth          = false;
    m_scrollPos          = 0;
    m_scrollPosTarget    = 0;
    m_totalHeight        = 0;
    m_selected           = -1;
    m_checkedItem        = nullptr; 
    m_items              = nullptr;
    m_ptPressed          = QPoint(0, 0);
    m_scrollBar          = nullptr; 
    m_menuBar            = nullptr;
    m_buttonPopup        = nullptr;
    m_buttonScrollUp     = nullptr;
    m_buttonScrollDown   = nullptr;
    m_animationStep      = 0.0;
    m_timerElapse        = 0;
    m_minimumColumnCount   = -1;
    m_maximumColumnCount   = -1;
    m_currentColumnCount   = -1;
    m_currentRowCount      = -1;
}

ARibbonGalleryPrivate::~ARibbonGalleryPrivate()
{
    A_P(ARibbonGallery);
    if (m_items && m_items->aproch_d().m_viewWidgets.size() > 0)
        m_items->aproch_d().m_viewWidgets.removeOne(&p);
}

void ARibbonGalleryPrivate::init()
{
    A_P(ARibbonGallery);
    p.setAttribute(Qt::WA_MouseTracking);
    p.setObjectName(QStringLiteral("ARibbonGallery"));
    p.setProperty(_aproch_WidgetGallery, true);
    setScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void ARibbonGalleryPrivate::layoutItems()
{
    A_P(ARibbonGallery);

    if (p.isShowAsButton())
        return;

    QRect rcItems = p.getItemsRect();

    if (p.autoWidth() && p.itemCount() > 0 && m_currentColumnCount != -1)
    {
        ARibbonGalleryItem* item = p.item(0);
        int width = m_currentColumnCount * item->sizeHint().width() + p.borders().width();
        rcItems.setWidth(width);
    }

    int x = rcItems.left();
    int y = rcItems.top();
    bool firstItem = true;

    int count = p.itemCount();

    m_arrRects.resize(count);
    int rowHeight = 0;

    for (int i = 0; i < count; ++i)
    {
        ARibbonGalleryItem* item = p.item(i);
        m_arrRects[i].item = item;
        m_arrRects[i].beginRow = false;

        if (!item->isVisible())
        {
            m_arrRects[i].rect = QRect(QPoint(0, 0), QPoint(0, 0));
            continue;
        }

        if (item->isSeparator())
        {
            if (!m_showLabels)
            {
                m_arrRects[i].rect = QRect(QPoint(0, 0), QPoint(0, 0));
                continue;
            }

            QFont font = p.font();
            if (qobject_cast<AOfficePopupMenu*>(p.parentWidget()))
                font.setBold(true);

            QFontMetrics qfm(font);
            const int marginH = p.style()->pixelMetric(QStyle::PM_FocusFrameHMargin) * 2;
            const int heightLabel = qfm.boundingRect(item->caption().isEmpty() ? QStringLiteral("XX") : item->caption()).height() + marginH;

            if (!firstItem)
                y += rowHeight;

            QRect rcItem(QPoint(rcItems.left(), y), QPoint(rcItems.right(), y + heightLabel));
            m_arrRects[i].rect = rcItem;
            m_arrRects[i].beginRow = true;

            y += heightLabel;
            rowHeight = 0;
            x = rcItems.left();
            firstItem = true;
        }
        else
        {
            QSize szItem = item->sizeHint();
            if (szItem.width() == 0)
                szItem.setWidth(rcItems.width());

            if (firstItem)
                m_arrRects[i].beginRow = true;

            if (!firstItem && x + szItem.width() > rcItems.right())
            {
                y += rowHeight;
                x = rcItems.left();
                m_arrRects[i].beginRow = true;
                rowHeight = 0;
            }

            QRect rcItem(QPoint(x, y), szItem);
            m_arrRects[i].rect = rcItem;

            rowHeight = qMax(rowHeight, szItem.height());

            firstItem = false;
            x += szItem.width();
        }
    }

    if (!firstItem)
        y += rowHeight;

    m_totalHeight = y - rcItems.top();

    if (m_scrollPos > m_totalHeight - rcItems.height())
        m_scrollPos = qMax(0, m_totalHeight - rcItems.height());

    setScrollBarValue();
}

void ARibbonGalleryPrivate::layoutScrollBar()
{
    A_P(ARibbonGallery);

    if (m_scrollBar)
    {
        QSize size = m_scrollBar->sizeHint();

        QRect rectScroll(p.rect());
        rectScroll.setLeft(rectScroll.right() - size.width());

        if (qobject_cast<AOfficePopupMenu*>(p.parentWidget()))
            rectScroll.adjust(0, 0, 0, 0);
        else if (m_showBorders )
            rectScroll.adjust(0, 1, -1, -1 );

        m_scrollBar->setGeometry(rectScroll);
    }
    else if (m_buttonPopup && m_buttonScrollUp && m_buttonScrollDown)
    {
        QRect rectScroll(p.rect());

        if (m_showBorders)
            rectScroll.adjust(1, 0, 0, 0);

        int width = p.style()->pixelMetric(QStyle::PM_ScrollBarExtent, nullptr, &p);
        rectScroll.setLeft(rectScroll.right() - width);
        
        int height = qRound(static_cast<qreal>(rectScroll.height()) / 3.0);
        QRect rectUp(QPoint(rectScroll.left(), rectScroll.top()), QPoint(rectScroll.right(), rectScroll.top() + height));
        m_buttonScrollUp->setGeometry(rectUp);
        QRect rectDown(QPoint(rectScroll.left(), rectUp.bottom() + 1), QPoint(rectScroll.right(), rectUp.bottom() + height));
        m_buttonScrollDown->setGeometry(rectDown);
        QRect rectScrollPopup(QPoint(rectScroll.left(), rectDown.bottom() + 1), QPoint(rectScroll.right(), rectScroll.bottom()));
        m_buttonPopup->setGeometry(rectScrollPopup);
    }
}

void ARibbonGalleryPrivate::setScrollBarValue()
{
    if (m_scrollBar)
    {
        A_P(ARibbonGallery);
        QRect rcItems = p.getItemsRect();

        int rangeMax = qMax(0, (m_totalHeight + 2) - m_scrollBar->rect().height());
        m_scrollBar->setRange(0, rangeMax);
        m_scrollBar->setPageStep(rcItems.height());
        m_scrollBar->setSingleStep(20);
        m_scrollBar->setValue(m_scrollPos);
    }
    else if (m_buttonPopup && m_buttonScrollUp && m_buttonScrollDown)
    {
        m_buttonScrollUp->setEnabled(isScrollButtonEnabled(true));
        m_buttonScrollDown->setEnabled(isScrollButtonEnabled(false));
    }
}

void ARibbonGalleryPrivate::setScrollPos(int scrollPos)
{
    A_P(ARibbonGallery);
    QRect rcItems = p.getItemsRect();

    if (scrollPos > m_totalHeight - rcItems.height())
        scrollPos = m_totalHeight - rcItems.height();

    if (scrollPos < 0)
        scrollPos = 0;

    if (m_animation)
    {
        m_animation = false;
        m_timerElapse = 0;
        m_scrollTimer.stop();
        m_scrollPos = -1;
    }

    if (m_scrollPos == scrollPos)
        return;

    m_scrollPos = scrollPos;
    layoutItems();

    repaintItems(nullptr, false);
}

void ARibbonGalleryPrivate::actionTriggered(int action)
{
    A_P(ARibbonGallery);
    int y = m_animation ? m_scrollPosTarget : m_scrollPos;
    QRect rcItems = p.getItemsRect();

    ARibbonGalleryGroup* items = p.galleryGroup();
    if (!items)
        return;

    switch(action)
    {
        case QAbstractSlider::SliderSingleStepAdd :
                y += items->size().height();
            break;
        case QAbstractSlider::SliderSingleStepSub :
                y -= items->size().height();
            break;
        case QAbstractSlider::SliderPageStepAdd :
                y += rcItems.height();
            break;
        case QAbstractSlider::SliderPageStepSub :
                y -= rcItems.height();
            break;
        case QAbstractSlider::SliderToMinimum :
                y = 0;
            break;
        case QAbstractSlider::SliderToMaximum :
                y = m_totalHeight;
            break;
        case QAbstractSlider::SliderMove :
                y = m_scrollBar->sliderPosition();
            break;
        default:
            break;
    }

    if (m_menuBar && (action == QAbstractSlider::SliderSingleStepSub || action == QAbstractSlider::SliderSingleStepAdd))
        startAnimation(y);
    else
        setScrollPos(y);
}

void ARibbonGalleryPrivate::pressedScrollUp()
{
    m_timerElapse = QApplication::doubleClickInterval() * 4 / 5;
    actionTriggered(QAbstractSlider::SliderSingleStepSub);
}

void ARibbonGalleryPrivate::pressedScrollDown()
{
    m_timerElapse = QApplication::doubleClickInterval() * 4 / 5;
    actionTriggered(QAbstractSlider::SliderSingleStepAdd);
}

void ARibbonGalleryPrivate::setScrollBarPolicy(Qt::ScrollBarPolicy policy)
{
    A_P(ARibbonGallery);

    if (policy == Qt::ScrollBarAlwaysOn)
    {
        A_DELETE_AND_NULL(m_buttonScrollUp);
        A_DELETE_AND_NULL(m_buttonScrollDown);
        A_DELETE_AND_NULL(m_buttonPopup);
        if (m_menuBar)
            m_menuBar->removeEventFilter(this);
        m_menuBar = nullptr;

        if (!m_scrollBar)
        {
            m_scrollBar = new QScrollBar(&p);
            setScrollBarValue();
            connect(m_scrollBar, SIGNAL(actionTriggered(int)), this, SLOT(actionTriggered(int)));
        }
    }
    else
    {
        if (m_scrollBar)
        {
            disconnect(m_scrollBar, SIGNAL(actionTriggered(int)), this, SLOT(actionTriggered(int)));
            delete m_scrollBar;
            m_scrollBar = nullptr;
        }
    }
}

Qt::ScrollBarPolicy ARibbonGalleryPrivate::scrollBarPolicy() const
{
    return m_scrollBar ? Qt::ScrollBarAlwaysOn : Qt::ScrollBarAlwaysOff;
}

QAction* ARibbonGalleryPrivate::setPopupMenu(AOfficePopupMenu* popup)
{
    A_P(ARibbonGallery);
    QAction* act = new QAction(&p);
    setScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_menuBar = popup;
    m_menuBar->installEventFilter(this);

    m_buttonScrollUp = new ARibbonGalleryButton(&p);
    m_buttonScrollUp->setProperty(_aproch_ScrollUpButtonGallery, true);
    m_buttonScrollDown = new ARibbonGalleryButton(&p);
    m_buttonScrollDown->setProperty(_aproch_ScrollDownButtonGallery, true);
    m_buttonPopup = new ARibbonGalleryButton(&p);
    m_buttonPopup->setProperty(_aproch_PopupButtonGallery, true);

    layoutScrollBar();
    m_buttonPopup->setPopupMode(QToolButton::InstantPopup);
    m_buttonPopup->setMenu(m_menuBar);
    m_menuBar->setWidgetBar(&p);
    m_menuBar->setDefaultAction(act);

    connect(m_buttonScrollUp, SIGNAL(pressed()), this, SLOT(pressedScrollUp()));
    connect(m_buttonScrollDown, SIGNAL(pressed()), this, SLOT(pressedScrollDown()));
    return act;
}

void ARibbonGalleryPrivate::drawGalleryItems(QPainter* painter)
{
    A_P(ARibbonGallery);

    ARibbonGalleryGroup* items = p.galleryGroup();
    if (!items)
        return;

    Q_ASSERT(m_arrRects.size() == items->itemCount());

    QRect rcItems = p.getItemsRect();
    painter->setClipRect(rcItems.adjusted(0, 1, 0, -1));

    int selected = m_hideSelection ? -1 : m_selected;
    for (int i = 0; i < m_arrRects.size(); ++i)
    {
        const SGalleryItemRect& pos = m_arrRects[i];

        QRect rcItem = pos.rect;
        rcItem.translate(0, -m_scrollPos);

        if (rcItem.bottom() < rcItems.top())
            continue;

        if (!rcItem.isEmpty())
        {
            bool enabled = pos.item->isEnabled() && p.isEnabled();
            pos.item->draw(painter, &p, rcItem, enabled, selected == i, selected == i && m_pressed, isItemChecked(pos.item));
        }

        if (rcItem.top() > rcItems.bottom())
            break;
    }
}

void ARibbonGalleryPrivate::repaintItems(QRect* pRect, bool bAnimate)
{
    Q_UNUSED(bAnimate)
    A_P(ARibbonGallery);
#ifdef Q_OS_MAC
    Q_UNUSED(pRect);
    p.update();
#else
    p.update(pRect ? *pRect : p.rect());
#endif
}

bool ARibbonGalleryPrivate::isItemChecked(ARibbonGalleryItem* pItem) const
{
    return m_checkedItem == pItem;
}

bool ARibbonGalleryPrivate::isScrollButtonEnabled(bool buttonUp)
{
    A_P(ARibbonGallery);

    if (!p.isEnabled())
        return false;
    return buttonUp ? m_scrollPos > 0 : m_scrollPos < m_totalHeight - p.getItemsRect().height();
}

int ARibbonGalleryPrivate::scrollWidth() const
{
    return m_scrollBar ? m_scrollBar->sizeHint().width() : m_buttonScrollUp ? m_buttonScrollUp->width() : 0;
}

void ARibbonGalleryPrivate::startAnimation(int scrollPos)
{
    A_P(ARibbonGallery);
    QRect rcItems = p.getItemsRect();

    if (scrollPos > m_totalHeight - rcItems.height())
        scrollPos = m_totalHeight - rcItems.height();

    if (scrollPos < 0)
        scrollPos = 0;

    if (m_animation && m_scrollPosTarget == scrollPos)
        return;

    if (!m_animation && m_scrollPos == scrollPos)
        return;

    m_animation = true;
    m_scrollPosTarget = scrollPos;
    m_animationStep = static_cast<qreal>(m_scrollPosTarget - m_scrollPos) / (m_timerElapse > 200 ? 8.0 : 3.0);

    if (m_animationStep > 0 && m_animationStep < 1) m_animationStep = 1;
    if (m_animationStep < 0 && m_animationStep > -1) m_animationStep = -1;

    int nms = 40;
    m_scrollTimer.start(nms, this);
    startAnimate();
}

void ARibbonGalleryPrivate::startAnimate()
{
    if (qAbs(m_scrollPos - m_scrollPosTarget) > qFabs(m_animationStep))
    {
        m_scrollPos = static_cast<int>(static_cast<qreal>(m_scrollPos) + m_animationStep);
    }
    else
    {
        m_animation = false;
        m_scrollPos = m_scrollPosTarget;
        m_timerElapse = 0;
        m_scrollTimer.stop();
    }
    layoutItems();
    setScrollBarValue();
    repaintItems(nullptr, false);
}

/*! \reimp */
bool ARibbonGalleryPrivate::event(QEvent* event)
{
    switch (event->type()) 
    {
        case QEvent::Timer:
            {
                QTimerEvent* timerEvent = static_cast<QTimerEvent*>(event);
                if (m_scrollTimer.timerId() == timerEvent->timerId()) 
                {
                    startAnimate();
                    event->accept();
                    return true;
                }
            }
            break;
        default:
            break;
    }
    return QObject::event(event);
}

/*! \reimp */
bool ARibbonGalleryPrivate::eventFilter(QObject* object, QEvent* event)
{
    bool bResult = QObject::eventFilter(object, event);

    if (event->type() == QEvent::Show)
    {
        if (AOfficePopupMenu* popup = qobject_cast<AOfficePopupMenu*>(object))
        {
            A_P(ARibbonGallery);
            QPoint pos;
            QRect screen = aproch_availableGeometry(m_buttonPopup);
            QSize sh = popup->sizeHint();
            QRect rect = p.rect();
            if (p.mapToGlobal(QPoint(0, rect.bottom())).y() + sh.height() <= screen.height())
                pos = p.mapToGlobal(rect.topLeft());
            else
                pos = p.mapToGlobal(rect.topLeft() - QPoint(0, sh.height()));
            popup->move(pos.x(), pos.y());
        }
    }
    else if (event->type() == QEvent::Hide)
    {
/*
        if (AOfficePopupMenu* popup = qobject_cast<AOfficePopupMenu*>(object))
        {
            popup->setTearOffEnabled(true);
            popup->setGeometry(QRect(QPoint(0,0), QSize(0, 0)));
            popup->resize(QSize(0,0));
        }
*/
    }
    return bResult;
}

/*!
\class ARibbonGalleryItem
\inmodule QtitanRibbon
\brief ARibbonGalleryItem class represents one element of the gallery.
*/

/*!
Constructs ARibbonGalleryItem.
*/
ARibbonGalleryItem::ARibbonGalleryItem()
{
    A_INIT_PRIVATE(ARibbonGalleryItem);
    A_D(ARibbonGalleryItem);
    d.init();
}

/*!
Destructor of the ARibbonGalleryGroup object.
*/
ARibbonGalleryItem::~ARibbonGalleryItem()
{
    A_DELETE_PRIVATE();
}

/*!
Returns the size hint for the item.
*/ 
QSize ARibbonGalleryItem::sizeHint() const
{
    A_D(const ARibbonGalleryItem);

    QSize size = qvariant_cast<QSize>(data(Qt::SizeHintRole));

    if (!size.isNull() && size.isValid())
        return size;

    return d.m_items->size();
}

/*!
Sets the \a size hint for the item.
*/ 
void ARibbonGalleryItem::setSizeHint(const QSize& size)
{
    setData(Qt::SizeHintRole, size);
}

/*!
Returns the icon of the item.
*/
QIcon ARibbonGalleryItem::icon() const
{
    return qvariant_cast<QIcon>(data(Qt::DecorationRole));
}

/*!
Sets the icon of the item.
*/
void ARibbonGalleryItem::setIcon(const QIcon& icon)
{
    setData(Qt::DecorationRole, icon);
}

/*!
Returns the \a caption of the item.
*/
QString ARibbonGalleryItem::caption() const
{ 
    return data(Qt::DisplayRole).toString(); 
}

/*!
Sets the caption of the item.
*/
void ARibbonGalleryItem::setCaption(const QString& caption)
{
    setData(Qt::DisplayRole, caption);
}

/*!
Returns the toolTip of the item.
*/
QString ARibbonGalleryItem::toolTip() const
{
    return data(Qt::ToolTipRole).toString();
}

/*!
Sets the \a toolTip of the item.
*/
void ARibbonGalleryItem::setToolTip(const QString& toolTip)
{
    setData(Qt::ToolTipRole, toolTip);
}

/*!
Returns the statusTip of the item.
*/
QString ARibbonGalleryItem::statusTip() const
{ 
    return data(Qt::StatusTipRole).toString(); 
}

/*!
Sets the \a statusTip of the item.
*/
void ARibbonGalleryItem::setStatusTip(const QString& statusTip)
{
    setData(Qt::StatusTipRole, statusTip);
}

/*!
Returns the index of this item in the gallery group.
*/
int ARibbonGalleryItem::getIndex() const
{
    A_D(const ARibbonGalleryItem);
    return d.m_index;
}

/*!
Returns true if the item is the separator.
*/ 
bool ARibbonGalleryItem::isSeparator() const
{
    A_D(const ARibbonGalleryItem);
    return d.m_separator;
}

/*!
Sets the representation for the item as a separator if parameter \a b is true.
*/ 
void ARibbonGalleryItem::setSeparator(bool b)
{
    A_D(ARibbonGalleryItem);
    d.m_separator = b;
}

void ARibbonGalleryItem::setEnabled(bool enabled)
{
    A_D(ARibbonGalleryItem);
    if (d.m_enabled == enabled)
        return;

    d.m_enabled = enabled;

    if (d.m_items)
        d.m_items->aproch_d().redrawWidget();
}

bool ARibbonGalleryItem::isEnabled() const
{
    A_D(const ARibbonGalleryItem);
    return d.m_enabled;
}

void ARibbonGalleryItem::setVisible(bool visible)
{
    A_D(ARibbonGalleryItem);
    if (d.m_visible == visible)
        return;

    d.m_visible = visible;

    if (d.m_items)
        d.m_items->aproch_d().itemsChanged();
}

bool ARibbonGalleryItem::isVisible() const
{
    A_D(const ARibbonGalleryItem);
    return d.m_visible;
}

void ARibbonGalleryItem::setData(int role, const QVariant& value)
{
    A_D(ARibbonGalleryItem);
    bool found = false;
    role = (role == Qt::EditRole ? Qt::DisplayRole : role);
    for (int i = 0; i < d.m_values.count(); ++i) 
    {
        if (d.m_values.at(i).role == role) 
        {
            if (d.m_values.at(i).value == value)
                return;
            d.m_values[i].value = value;
            found = true;
            break;
        }
    }
    if (!found)
        d.m_values.append(AWidgetItemData(role, value));

//    if (QListModel *model = (view ? qobject_cast<QListModel*>(view->model()) : 0))
//        model->itemChanged(this);
}

QVariant ARibbonGalleryItem::data(int role) const
{
    A_D(const ARibbonGalleryItem);
    role = (role == Qt::EditRole ? Qt::DisplayRole : role);
    for (int i = 0; i < d.m_values.count(); ++i)
        if (d.m_values.at(i).role == role)
            return d.m_values.at(i).value;
    return QVariant();
}

/*!
Draws the item on painter \a p.
*/
void ARibbonGalleryItem::draw(QPainter* p, ARibbonGallery* gallery, QRect rectItem, bool enabled, bool selected, bool pressed, bool checked)
{
    Q_UNUSED(gallery)
    ARibbonGalleryItemStyleOption option;
    option.initFrom(gallery);
    option.state = QStyle::State_None;
    if (enabled)
        option.state |= QStyle::State_Enabled;
    if (selected)
        option.state |= QStyle::State_MouseOver;
    if (pressed)
        option.state |= QStyle::State_Sunken;
    if (checked)
        option.state |= QStyle::State_On;
    else
        option.state |= QStyle::State_Off;

    option.icon = icon();
    option.caption = caption();
    option.separator = isSeparator();
    option.rectItem = rectItem;

    gallery->style()->drawControl(static_cast<QStyle::ControlElement>(ACommonStyle::CE_RibbonGalleryItem), &option, p, gallery);
}

/*!
\class ARibbonGalleryGroup
\inmodule QtitanRibbon
\brief ARibbonGalleryGroup class is a container for the ARibbonGalleryItem.
*/

/*!
Constructs ARibbonGalleryGroup object with the given \a parent.
*/ 
ARibbonGalleryGroup::ARibbonGalleryGroup(QObject* parent)
    : QObject(parent)
{
    setObjectName(QStringLiteral("ARibbonGalleryGroup"));
    A_INIT_PRIVATE(ARibbonGalleryGroup);
    A_D(ARibbonGalleryGroup);
    d.init();
}

/*!
Destructor of the ARibbonGalleryGroup object.
*/ 
ARibbonGalleryGroup::~ARibbonGalleryGroup()
{
    clear();
    A_DELETE_PRIVATE();
}

/*!
Creates and adds new item to the gallery with \a caption, \a pixmap. Additionaly it is possible to specify transparent color \a transparentColor in the \a pixmap.
*/ 
ARibbonGalleryItem* ARibbonGalleryGroup::addItem(const QString& caption, const QPixmap& pixmap, const QColor& transparentColor)
{
    ARibbonGalleryItem* item = new ARibbonGalleryItem();

    if (!pixmap.isNull())
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        QImage copyImage = pixmap.copy(0, 0, pixmap.width(), pixmap.height()).toImage();
        if (transparentColor.isValid())
            copyImage.setAlphaChannel(copyImage.createMaskFromColor(transparentColor.rgb(), Qt::MaskOutColor));
        item->setIcon(QPixmap::fromImage(copyImage));
#else
        QPixmap copyPix = pixmap.copy(0, 0, pixmap.width(), pixmap.height());
        if (transparentColor.isValid())
            copyPix.setAlphaChannel(QBitmap(copyPix.createMaskFromColor(transparentColor, Qt::MaskOutColor)));
        item->setIcon(QIcon(copyPix));
#endif
    }

    if (!caption.isEmpty())
    {
        item->setCaption(caption);
        item->setToolTip(caption);
    }
    appendItem(item);
    return item;
}

/*!
Creates and adds new item to the gallery with \a caption and pixmap from pixmap \a map. Parameter \a mapIndex is a index in the map. Parameter \a mapItemSize is a size of the one item in the map.
Additionaly it is possible to specify transparent color \a transparentColor in the pixmap.
*/ 
ARibbonGalleryItem* ARibbonGalleryGroup::addItemFromMap(const QString& caption, int mapIndex, const QPixmap& map, const QSize& mapSizeImage, const QColor& clrTransparent)
{
    QSize szImage = mapSizeImage;
    QRect rcImage(mapIndex * szImage.width(), 0, szImage.width(), szImage.height());
    QPixmap copyPix = map.copy(rcImage.left(), rcImage.top(), rcImage.width(), rcImage.height());

    Q_ASSERT(!copyPix.isNull());

    if (copyPix.isNull())
        return nullptr;

    return addItem(caption, copyPix, clrTransparent);
}

/*! Adds existing \a item to the gallery.
Note: ARibbonGalleryGroup object is assigned as a owner for the item.
*/ 
void ARibbonGalleryGroup::appendItem(ARibbonGalleryItem* item)
{
    insertItem(itemCount(), item);
}

/*! Inserts existing \a item to the gallery in pos \a index.
Note: ARibbonGalleryGroup object is assigned as a owner for the item.
*/ 
void ARibbonGalleryGroup::insertItem(int index, ARibbonGalleryItem* item)
{
    A_D(ARibbonGalleryGroup);

    if (!item)
    {
        Q_ASSERT(false);
        return;
    }

    if (index < 0 || index > itemCount())
        index = itemCount();

    d.m_arrItems.insert(index, item);
    d.updateIndexes(index);

    item->aproch_d().m_items = this;
    d.itemsChanged();
}

/*!
Adds separator to the group with given \a caption and returns reference to them.
*/ 
ARibbonGalleryItem* ARibbonGalleryGroup::addSeparator(const QString& caption)
{
    ARibbonGalleryItem* item = new ARibbonGalleryItem();
    appendItem(item);
    item->setCaption(caption);
    item->setSeparator(true);
    return item;
}

/*!
Clear the collection the item.
*/
void ARibbonGalleryGroup::clear()
{
    A_D(ARibbonGalleryGroup);
    d.clear();
    d.itemsChanged();
}

/*!
Removes the item from the gallery by given \a index.
*/
void ARibbonGalleryGroup::remove(int index)
{
    A_D(ARibbonGalleryGroup);

    ARibbonGalleryItem* galleryItem = item(index);
    if (!galleryItem)
        return;

    d.m_arrItems.remove(index);
    delete galleryItem;

    d.updateIndexes(index);
    d.itemsChanged();
}

/*!
Returns the count of the items.
*/ 
int ARibbonGalleryGroup::itemCount() const
{
    A_D(const ARibbonGalleryGroup);
    return d.m_arrItems.size();
}

/*!
Returns reference to the ARibbonGalleryItem by the given \a index.
*/ 
ARibbonGalleryItem* ARibbonGalleryGroup::item(int index) const
{
    A_D(const ARibbonGalleryGroup);
    return index >= 0 && index < itemCount() ? d.m_arrItems.at(index) : nullptr;
}

/*!
Removes item at index from item list without deleting it
*/ 
ARibbonGalleryItem* ARibbonGalleryGroup::takeItem(int index)
{
    A_D(ARibbonGalleryGroup);

    ARibbonGalleryItem* galleryItem = item(index);
    if (!galleryItem)
        return nullptr;

    d.m_arrItems.remove(index);    
    d.updateIndexes(0);

    d.itemsChanged();
    return galleryItem;
}

/*!
Returns size of the group.
*/ 
QSize ARibbonGalleryGroup::size() const
{
    A_D(const ARibbonGalleryGroup);

    if (!ACommonStylePrivate::isUse96Dpi())
    {
        QWidget* wd = nullptr;
        QList<QWidget*> lst = d.m_viewWidgets;
        if (lst.count() > 0)
            wd = lst.at(0);
        return QSize(ACommonStylePrivate::dpiScaled(d.m_sizeItem.width(), wd), ACommonStylePrivate::dpiScaled(d.m_sizeItem.height(), wd));
    }
    else
        return d.m_sizeItem;
}

/*!
Sets \a size of the group.
*/ 
void ARibbonGalleryGroup::setSize(const QSize& size)
{
    A_D(ARibbonGalleryGroup);
    d.m_sizeItem = size;
}

/*!
Sets clipping items if selection was changed. Paramter \a clipItems - true to enable clipping or false to disable. By default clipping is enabled.
*/
void ARibbonGalleryGroup::setClipItems(bool clipItems)
{
    A_D(ARibbonGalleryGroup);
    d.m_clipItems = clipItems;
}

/*!
    \class ARibbonGallery
    \inmodule QtitanRibbon
    \brief ARibbonGallery class represents the gallery control.
*/

/*!
    \fn void ARibbonGallery::itemPressed(ARibbonGalleryItem* item);
    Signal throws when you press mouse button on the \a item in the gallery.
*/

/*!
    \fn void ARibbonGallery::itemClicked(ARibbonGalleryItem* item);
    Signal throws when you click on the \a item in the gallery.
*/

/*!
    \fn void ARibbonGallery::itemClicking(ARibbonGalleryItem* item, bool& handled);
    Signal throws when user is clicking on the \a item in the gallery. \a item - the reference to teh clicking element.
    \a handled - the flag to prevent of closing the gallery popup. If handled is false then the gallery popup will not be closed.
*/

/*!
    \fn void ARibbonGallery::currentItemChanged(ARibbonGalleryItem* current, ARibbonGalleryItem* previous);
    Signal throws when the current item was changed. previous - reference to the previous selected item. current the reference to the current selected item.
*/

/*!
    \fn void ARibbonGallery::itemSelectionChanged();
    Signal throws when selected was changed.
*/

/*!
Constructs ARibbonGallery object with the given \a parent.
*/
ARibbonGallery::ARibbonGallery(QWidget* parent)
    : QWidget(parent)
{
    A_INIT_PRIVATE(ARibbonGallery);
    A_D(ARibbonGallery);
    d.init();
}

/*!
Destructor of the ARibbonGallery object.
*/
ARibbonGallery::~ARibbonGallery()
{
    A_DELETE_PRIVATE();
}

/*!
Sets group for the gallery. group - is a pointer to the elements group.
*/ 
void ARibbonGallery::setGalleryGroup(ARibbonGalleryGroup* items)
{
    A_D(ARibbonGallery);
    if (d.m_items)
    {
        d.m_arrRects.clear();
        d.m_items->aproch_d().m_viewWidgets.removeOne(this);
        d.m_items = nullptr;
    }

    if (items)
    {
        d.m_items = items;
        d.m_items->aproch_d().m_viewWidgets.append(this);
    }
    d.layoutItems();
    update();
}

/*!
Returns the visibility of the border around the gallery. By default the border is not visible.
*/
bool ARibbonGallery::isBorderVisible() const
{
    A_D(const ARibbonGallery);
    return d.m_showBorders;
}

/*!
Sets the visibility of the border around the gallery to \visible. 
*/
void ARibbonGallery::setBorderVisible(bool visible)
{
    A_D(ARibbonGallery);
    if (d.m_showBorders != visible)
    {
        d.m_showBorders = visible;
        update();
    }
}

bool ARibbonGallery::isTransparent() const
{
    A_D(const ARibbonGallery);
    return d.m_transparent;
}

void ARibbonGallery::setTransparent(bool transparent)
{
    A_D(ARibbonGallery);
    if (d.m_transparent != transparent)
    {
        d.m_transparent = transparent;
        update();
    }
}

void ARibbonGallery::setLabelsVisible(bool showLabels)
{
    A_D(ARibbonGallery);
    d.m_showLabels = showLabels;
}

bool ARibbonGallery::isLabelsVisible() const
{
    A_D(const ARibbonGallery);
    return d.m_showLabels;
}

/*!
Sets \a policy of the scrollbars.
*/
void ARibbonGallery::setScrollBarPolicy(Qt::ScrollBarPolicy policy)
{
    A_D(ARibbonGallery);
    d.setScrollBarPolicy(policy);
}

/*!
Returns policy of the scrollbars.
*/
Qt::ScrollBarPolicy ARibbonGallery::scrollBarPolicy() const
{
    A_D(const ARibbonGallery);
    return d.scrollBarPolicy();
}

void ARibbonGallery::ensureVisible(int index)
{
    A_D(ARibbonGallery);

    if (itemCount() == 0 || index == -1 || index >= itemCount() || 
        index >= d.m_arrRects.size())
        return;

    const SGalleryItemRect& pos = d.m_arrRects.at(index);

    QRect rcItem = pos.rect;
    rcItem.translate(0, -d.m_scrollPos);

    QRect rcItems = getItemsRect();

    if (rcItem.top() < rcItems.top())
    {
        int scrollPos = pos.rect.top() - rcItems.top();
        if (d.m_showLabels)
        {
            for (int i = index - 1; i >= 0; i--)
            {
                const SGalleryItemRect& posLabel = d.m_arrRects.at(i);
                if (posLabel.item->isSeparator() && pos.rect.top() - posLabel.rect.top() <= rcItems.height() - rcItem.height())
                {
                    scrollPos = posLabel.rect.top() - rcItems.top();
                    break;
                }
                if (pos.rect.top() - posLabel.rect.top() > rcItems.height()  - rcItem.height())
                    break;
            }
        }
        d.setScrollPos(scrollPos);
    }
    else if (rcItem.bottom() > rcItems.bottom())
    {
        d.setScrollPos(pos.rect.bottom() - rcItems.bottom());
    }
}

/*!
Sets \a popup menu for the gallery.
*/ 
QAction* ARibbonGallery::setPopupMenu(AOfficePopupMenu* popupMenu)
{
    A_D(ARibbonGallery);
    return d.setPopupMenu(popupMenu);
}

/*!
Returns gallery's popup menu.
*/ 
AOfficePopupMenu* ARibbonGallery::popupMenu() const
{
    A_D(const ARibbonGallery);
    return d.m_menuBar;
}

int ARibbonGallery::itemCount() const
{
    ARibbonGalleryGroup* pItems = galleryGroup();
    return pItems ? pItems->itemCount() : 0;
}

ARibbonGalleryItem* ARibbonGallery::item(int index) const
{
    ARibbonGalleryGroup* items = galleryGroup();
    return items ? items->item(index) : nullptr;
}

/*!
Returns reference to the ARibbonGalleryGroup object.
*/ 
ARibbonGalleryGroup* ARibbonGallery::galleryGroup() const
{
    A_D(const ARibbonGallery);
    return d.m_items;
}

/*!
Sets the selected element in the gallery by its \a index.
*/
void ARibbonGallery::setSelectedItem(int index)
{
    A_D(ARibbonGallery);

    int indSelected = d.m_selected;

    d.m_hideSelection = false;
    d.m_selected = index;
    d.m_pressed = false;
    d.m_keyboardSelected = true;

    ARibbonGalleryGroup* items = galleryGroup();

    if (items && items->aproch_d().m_clipItems && indSelected != -1)
    {
        QRect rect(getDrawItemRect(indSelected));
        d.repaintItems(&rect, true);
    }

    ensureVisible(d.m_selected);

    if (items && items->aproch_d().m_clipItems && d.m_selected != -1)
    {
        QRect rect(getDrawItemRect(d.m_selected));
        d.repaintItems(&rect, false);
    }

    if (!items || !items->aproch_d().m_clipItems)
        d.repaintItems();

    if (d.m_selected != -1)
    {
    }
    if (d.m_selected != -1 && !d.m_preview)
        d.m_preview = true;

    if (d.m_preview)
    {
    }

    if (d.m_selected == -1 && d.m_preview)
        d.m_preview = false;
    selectedItemChanged();
}

/*!
Returns the index of the selected element in the gallery.
*/
int ARibbonGallery::selectedItem() const
{
    A_D(const ARibbonGallery);
    return d.m_selected;
}

/*!
Sets checked item with the given \a index.
*/ 
void ARibbonGallery::setCheckedIndex(int index)
{
    A_D(ARibbonGallery);

    ARibbonGalleryItem* previous = nullptr;
    int previousInd = checkedIndex();
    if (previousInd != -1)
        previous = item(previousInd);

    ARibbonGalleryItem* currentItem = item(index);
    if (d.m_checkedItem != currentItem)
    {
        d.m_checkedItem = currentItem;
        d.repaintItems();
        emit currentItemChanged(currentItem, previous);
    }
    if (currentItem)
        ensureVisible(index);
}

/*!
Returns chcked item index in the gallery.
*/
int ARibbonGallery::checkedIndex() const
{
    A_D(const ARibbonGallery);
    if (d.m_checkedItem)
        return d.m_checkedItem->getIndex();
    return -1;
}

/*!
Sets checked \a item with the given reference to the ARibbonGalleryItem object.
See setCheckedIndex(int index).
*/ 
void ARibbonGallery::setCheckedItem(const ARibbonGalleryItem* item)
{
    Q_ASSERT(item != nullptr);
    if (item->getIndex() != -1)
        setCheckedIndex(item->getIndex());
}

/*!
Returs the reference to the ARibbonGalleryItem object of the chcked item in the gallery.
See int getCheckedIndex();
*/ 
ARibbonGalleryItem* ARibbonGallery::checkedItem() const
{
    A_D(const ARibbonGallery);
    return d.m_checkedItem;
}

/*!
Returns if the gallery needs to draw a normal button. Function is reserved.
*/
bool ARibbonGallery::isShowAsButton() const
{
    return false;
}

/*!
Returns whether selected items in the gallery.
*/
bool ARibbonGallery::isItemSelected() const
{
    A_D(const ARibbonGallery);
    return !d.m_hideSelection && selectedItem() != -1;
}

/*!
Returns the bounds of gallery in the rectangle form.
*/
QRect ARibbonGallery::borders() const
{
    A_D(const ARibbonGallery);
    QRect rcBorders(0, 0, 0, 0);

    if (d.m_showBorders)
    {
        rcBorders.setTopLeft(QPoint(1, 1));
        rcBorders.setBottomRight(QPoint(1, 1));
    }

    rcBorders.setRight(rcBorders.right() + d.scrollWidth());

    int left = 0, top = 0, right = 0, bottom = 0;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QMargins margins = contentsMargins();
    left = margins.left();
    top = margins.top();
    right = margins.right();
    bottom = margins.bottom();
#else
    getContentsMargins(&left, &top, &right, &bottom);
#endif
    rcBorders.adjust(left, top, right, bottom);

    return rcBorders;
}

/*!
Returns the bounded rectangle of the item by the given \a index.
*/ 
QRect ARibbonGallery::getDrawItemRect(int index)
{
    A_D(ARibbonGallery);
    if (index < 0 || index >= d.m_arrRects.size())
        return QRect(0, 0, 0, 0);

    QRect rc(d.m_arrRects[index].rect);
    rc.translate(0, -d.m_scrollPos);

    rc = rc.intersected(getItemsRect());
    return rc;
}

/*!
Returns possibility to resize.
*/
bool ARibbonGallery::isResizable() const
{
    bool isResizable = sizePolicy().horizontalPolicy() == QSizePolicy::Expanding || 
        sizePolicy().horizontalPolicy() == QSizePolicy::MinimumExpanding;
    return !isShowAsButton() && galleryGroup() ? isResizable : false;
}

bool ARibbonGallery::autoWidth() const
{
    A_D(const ARibbonGallery);
    return d.m_autoWidth;
}

void ARibbonGallery::setAutoWidth(bool width)
{
    A_D(ARibbonGallery);
    d.m_autoWidth = width;
}

/*!
Returns the index of the item by given mouse \a point. If rect is not nullptr it will contain the rectangle the gallery's item.
*/
int ARibbonGallery::hitTestItem(QPoint point, QRect* rect) const
{
    if (!isEnabled())
        return -1;

    if (isShowAsButton())
        return -1;

    A_D(const ARibbonGallery);
    Q_ASSERT(d.m_arrRects.size() == itemCount());

    QRect rcItems = getItemsRect();

    for (int i = 0; i < d.m_arrRects.size(); ++i)
    {
        const SGalleryItemRect& pos = d.m_arrRects[i];

        QRect rcItem = pos.rect;
        rcItem.translate(0, -d.m_scrollPos);

        if (!pos.item->isEnabled())
            continue;

        if (rcItem.top() >= rcItems.bottom())
            break;

        if (rcItem.contains(point))
        {
            if (rect != nullptr)
                *rect = rcItem;
            return pos.item->isSeparator() ? -1 : i;
        }
    }

    return -1;
}

/*!
Returns the united boundary of all elements within the galleries.
*/
QRect ARibbonGallery::getItemsRect() const
{
    A_D(const ARibbonGallery);
    QRect rc = rect();

    if (itemCount() > 0 && (minimumColumnCount() != -1 || maximumColumnCount() != -1))
    {
        ARibbonGalleryItem* it = item(0);
        int width = d.m_currentColumnCount * it->sizeHint().width() + borders().width();
        rc.setWidth(width);
    }

    if (rc.isNull())
    {
        rc.setWidth(32000);
        rc.setHeight(32000);
    }

    QRect rcBorders = borders();
    rc.adjust(rcBorders.left(), rcBorders.top(), -rcBorders.right(), -rcBorders.bottom());
    return rc;
}

/*!
Hide all selection of the gallery's item.
*/
void ARibbonGallery::hideSelection()
{
    A_D(ARibbonGallery);
    d.m_hideSelection = true;
    d.m_keyboardSelected = false;

    if (d.m_selected != -1)
    {
        ARibbonGalleryGroup* pItems = galleryGroup();
        if (pItems && pItems->aproch_d().m_clipItems)
        {
            QRect rect(getDrawItemRect(d.m_selected));
            d.repaintItems(&rect, false);
        }
        else
            d.repaintItems();
    }

    if (d.m_preview && d.m_selected != -1)
    {
    }

    if (d.m_preview)
        d.m_preview = false;
    selectedItemChanged();
}

void ARibbonGallery::updatelayout()
{
    A_D(ARibbonGallery);
    d.layoutItems();
    d.layoutScrollBar();
}

void ARibbonGallery::bestFit()
{
    if (itemCount() > 0)
    {
        ARibbonGalleryItem* galleryItem = item(0);
        QSize szItem = galleryItem->sizeHint();
        int totalWidth = width() - borders().width();
        if (szItem.width() > 0 &&  szItem.width() < totalWidth)
        {
            int columnCount =  totalWidth / szItem.width();
            setColumnCount(columnCount);
        }
        else
        {
            setColumnCount(-1);
        }
    }
}

void ARibbonGallery::selectedItemChanged()
{
}

/*!
Assigns a minimum \a count of column in the ribbon gallery.
*/
void ARibbonGallery::setMinimumColumnCount(int count)
{
    A_D(ARibbonGallery);
    if (d.m_minimumColumnCount != count)
    {
        d.m_minimumColumnCount = count;

        if (d.m_minimumColumnCount > d.m_currentColumnCount)
            d.m_currentColumnCount = d.m_minimumColumnCount;

        updatelayout();
    }
}

/*!
Returns a minimum count of column in the ribbon gallery.
*/
int ARibbonGallery::minimumColumnCount() const
{
    A_D(const ARibbonGallery);
    return d.m_minimumColumnCount;
}

/*!
Assigns a maximum \a count of column in the ribbon gallery.
*/
void ARibbonGallery::setMaximumColumnCount(int count)
{
    A_D(ARibbonGallery);
    if ( d.m_maximumColumnCount != count )
    {
        d.m_maximumColumnCount = count;

        if (d.m_maximumColumnCount < d.m_currentColumnCount || d.m_currentColumnCount == -1)
            d.m_currentColumnCount = d.m_maximumColumnCount;

        updatelayout();
    }
}

/*!
Returns a maximum count of column in the ribbon gallery.
*/
int ARibbonGallery::maximumColumnCount() const
{
    A_D(const ARibbonGallery);
    return d.m_maximumColumnCount;
}

/*!
Assigns a current \a count of column in the ribbon gallery.
*/
void ARibbonGallery::setColumnCount(int count)
{
    A_D(ARibbonGallery);

    if (d.m_maximumColumnCount != -1 && d.m_maximumColumnCount < count)
        count = d.m_maximumColumnCount;
    else if (d.m_minimumColumnCount != -1 && d.m_minimumColumnCount > count)
        count = d.m_minimumColumnCount;

    d.m_currentColumnCount = count;

    if (autoWidth())
        updatelayout();
}

/*!
Returns a current count of column in the ribbon gallery.
*/
int ARibbonGallery::columnCount() const
{
    A_D(const ARibbonGallery);
    return d.m_currentColumnCount;
}

/*!
Assigns a current \a count of row in the ribbon gallery.
*/
void ARibbonGallery::setRowCount(int row)
{
    A_D(ARibbonGallery);
    Q_ASSERT(row > 0);
    if (row > 0)
        d.m_currentRowCount = row;
}

/*!
Returns a current count of row in the ribbon gallery.
*/
int ARibbonGallery::rowCount() const
{
    A_D(const ARibbonGallery);
    return d.m_currentRowCount;
}

/*! \reimp */
QSize ARibbonGallery::sizeHint() const
{
    A_D(const ARibbonGallery);

    QSize size = minimumSizeHint();

    if (columnCount() != -1)
    {
        if (ARibbonGalleryGroup* group = galleryGroup())
        {
            size.setWidth(columnCount() * group->size().width());
            if (d.m_currentRowCount == -1)
                size.setHeight(d.m_totalHeight);
            return size;
        }
    }

    int widthGallery = qMax(isResizable() ? size.width() : 0, width());
    int heightGallery = height();
    return QSize(widthGallery, heightGallery);
}

/*! \reimp */
QSize ARibbonGallery::minimumSizeHint() const
{
    A_D(const ARibbonGallery);

    if (!parentWidget() || !galleryGroup())
        return QSize(0, 0);

    QRect rcBorders(borders());
    int width = qMax(16, galleryGroup()->aproch_d().m_sizeItem.width()) + rcBorders.left() + rcBorders.right();

    if (ARibbonGalleryGroup* group = galleryGroup())
        width = d.m_minimumColumnCount != -1 ? d.m_minimumColumnCount * group->size().width() : width;

    int height = 0;
    if (d.m_currentRowCount == -1)
        height = qMax(galleryGroup()->aproch_d().m_sizeItem.height() + rcBorders.top() + rcBorders.bottom(), 32);
    else if (itemCount() > 0)
        height = qMax((item(0)->sizeHint().height() * d.m_currentRowCount) + (rcBorders.top() + rcBorders.bottom()), 32);

    return QSize(width, height);
}

/*! \reimp */
bool ARibbonGallery::event(QEvent* event)
{
    if (event->type() == QEvent::ParentChange)
        updatelayout();
    return QWidget::event(event);
}

/*! \reimp */
void ARibbonGallery::paintEvent(QPaintEvent*)
{
    A_D(ARibbonGallery);
    QPainter p(this);
    QtStyleOptionFrame opt;
    opt.initFrom(this);
    opt.lineWidth = !parentWidget()->property(_aproch_PopupBar).toBool() && isBorderVisible() ? 1 : 0;
    opt.features |= isTransparent() ? QtStyleOptionFrame::None : QtStyleOptionFrame::Flat;
    style()->drawPrimitive(static_cast<QStyle::PrimitiveElement>(ACommonStyle::PE_RibbonFrameGallery), &opt, &p, this);
    d.drawGalleryItems(&p);
}

/*! \reimp */
void ARibbonGallery::mousePressEvent(QMouseEvent* event)
{
    A_D(ARibbonGallery);

    if (isShowAsButton())
    {
        QWidget::mousePressEvent(event);
        return;
    }

    if (d.m_selected != -1 && !d.m_keyboardSelected)
    {
        d.m_pressed = true;
        d.m_ptPressed = event->pos();
        d.repaintItems();

        int index = hitTestItem(event->pos());
        if (index != -1)
            emit itemPressed(item(index));
    }
}

/*! \reimp */
void ARibbonGallery::mouseReleaseEvent(QMouseEvent* event)
{
    A_D(ARibbonGallery);
    if (d.m_pressed || isItemSelected())
    {
        d.m_pressed = false;
        d.repaintItems();

        int index = hitTestItem(event->pos());
        if (index != -1)
        {
            d.repaintItems();
            emit itemSelectionChanged();
            ARibbonGalleryItem* galleryItem = item(index);

            emit itemClicked(galleryItem);

            bool handled = true;
            emit itemClicking(galleryItem, handled);

            if (handled)
            {
                if (AOfficePopupMenu* popupBar = qobject_cast<AOfficePopupMenu*>(parentWidget()))
                {
                    popupBar->close();
                    for (QWidget* widget = QApplication::activePopupWidget(); widget;) 
                    {
                        if (QMenu* qmenu = qobject_cast<QMenu*>(widget)) 
                        {
                            qmenu->hide();
                            break;
                        }
                    }
                }
            }
        }
    }
}

/*! \reimp */
void ARibbonGallery::mouseMoveEvent(QMouseEvent* event)
{
    A_D(ARibbonGallery);

    if (isShowAsButton())
    {
        QWidget::mouseMoveEvent(event);
        return;
    }

    int nItem = hitTestItem(event->pos());

    if (nItem == -1 && d.m_keyboardSelected)
        return;

    if (nItem == -1 && event->pos() == QPoint(-1, -1) && d.m_selected != -1)
    {
        hideSelection();
        return;
    }

    int nSelected = d.m_selected;
    if (nItem != d.m_selected || d.m_hideSelection || d.m_keyboardSelected)
    {
        d.m_selected = nItem;
        d.m_hideSelection = false;
        d.m_keyboardSelected = false;
        d.m_pressed = false;

        ARibbonGalleryGroup* pItems = galleryGroup();

        if (pItems && pItems->aproch_d().m_clipItems)
        {
            if (nSelected != -1)
            {
                QRect rect(getDrawItemRect(nSelected));
                d.repaintItems(&rect, true);
            }

            if (d.m_selected != -1)
            {
                QRect rect(getDrawItemRect(d.m_selected));
                d.repaintItems(&rect, false);
            }
        }
        else
            d.repaintItems();

        if (!d.m_preview && d.m_selected != -1)
        {
        }

        if (d.m_preview)
        {
        }

        if (d.m_selected == -1 && d.m_preview)
        {
            d.m_preview = false;
        }
        selectedItemChanged();
    }
    if (d.m_pressed && (d.m_ptPressed != QPoint(0, 0)) && (d.m_selected != -1) &&
        (abs(d.m_ptPressed.x() - event->pos().x()) > 4 || abs(d.m_ptPressed.y() - event->pos().y()) > 4))
    {
        d.m_ptPressed = QPoint(0, 0);
    }
}

/*! \reimp */
void ARibbonGallery::wheelEvent(QWheelEvent* event)
{
    QWidget::wheelEvent(event);
    A_D(ARibbonGallery);

    ARibbonGalleryGroup* group = galleryGroup();
    if (group == nullptr)
        return;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    int delta = (qAbs(event->angleDelta().x()) > qAbs(event->angleDelta().y()) ?
        event->angleDelta().x() : event->angleDelta().y());
#else
    int delta = event->delta();
#endif
    if (qobject_cast<AOfficePopupMenu*>(parentWidget()))
        d.setScrollPos(d.m_scrollPos + (delta < 0 ? 3 * group->size().height() : -3 * group->size().height()));
}

/*! \reimp */
void ARibbonGallery::leaveEvent(QEvent* event)
{
    QWidget::leaveEvent(event);

    A_D(ARibbonGallery);
    if (d.m_selected != -1)
        hideSelection();
}

/*! \reimp */
void ARibbonGallery::focusOutEvent(QFocusEvent* event)
{
    QWidget::focusOutEvent(event);
}

/*! \reimp */
void ARibbonGallery::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    if (autoWidth())
        bestFit();
    updatelayout();
}

APROCH_NAMESPACE_END