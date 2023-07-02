/****************************************************************************
 * @file    ARibbonControls.cpp
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
#include <QAction>
#include <QStyleOption>
#include <QPainter>
#include <QToolBar>
#include <QScreen>

#include "Platform/APlatform.h"
#include "ARibbonControls.h"
#include "ARibbonControls_p.h"
#include "ARibbonGroup_p.h"
#include "ARibbonBar_p.h"
#include "Widget/Style/ACommonStyle_p.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

ARibbonControlSizeDefinitionPrivate::ARibbonControlSizeDefinitionPrivate()
    : m_parentControl(nullptr)
{
}

void ARibbonControlSizeDefinitionPrivate::init(ARibbonControl* parentControl, ARibbonControlSizeDefinition::GroupSize size)
{
    m_parentControl = parentControl;

    m_imageSize = ARibbonControlSizeDefinition::ImageSmall;
    m_labelVisible = true;
    m_popup = size == ARibbonControlSizeDefinition::GroupPopup;
    m_showSeparator = true;
    m_visible = true;
    m_enabled = true;
    m_stretchable = true;
    m_wordWrap = true;
    m_visualIndex = -1;
    m_minItemCount = -1;
    m_maxItemCount = -1;

    if (size == ARibbonControlSizeDefinition::GroupSimplified || 
        size == ARibbonControlSizeDefinition::GroupSimplifiedOverflow)
    {
    }
    else if (size == ARibbonControlSizeDefinition::GroupLarge || size == ARibbonControlSizeDefinition::GroupPopup)
    {
        m_imageSize = ARibbonControlSizeDefinition::ImageLarge;
        m_labelVisible = true;
    }
    else
    {
        m_imageSize = ARibbonControlSizeDefinition::ImageSmall;
        if (size == ARibbonControlSizeDefinition::GroupMedium)
            m_labelVisible = true;
        else 
            m_labelVisible = false;
    }
}

void ARibbonControlSizeDefinitionPrivate::updateSizeDefinition()
{
    m_parentControl->sizeChanged(m_parentControl->currentSize());
}

ARibbonControlSizeDefinitionPrivate::~ARibbonControlSizeDefinitionPrivate()
{
}

/*!
\class ARibbonControlSizeDefinition
\inmodule QtitanRibbon
*/
ARibbonControlSizeDefinition::ARibbonControlSizeDefinition(ARibbonControl* parentControl, ARibbonControlSizeDefinition::GroupSize size)
    : QObject()
{
    A_INIT_PRIVATE(ARibbonControlSizeDefinition);
    A_D(ARibbonControlSizeDefinition);
    d.init(parentControl, size);
}

ARibbonControlSizeDefinition::~ARibbonControlSizeDefinition()
{
    A_DELETE_PRIVATE();
}

ARibbonControlSizeDefinition::ControlImageSize ARibbonControlSizeDefinition::imageSize() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_imageSize;
}

void ARibbonControlSizeDefinition::setImageSize(ARibbonControlSizeDefinition::ControlImageSize size)
{
    A_D(ARibbonControlSizeDefinition);
    if (d.m_imageSize != size)
    {
        d.m_imageSize = size;
        d.updateSizeDefinition();
    }
}

bool ARibbonControlSizeDefinition::isLabelVisible() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_labelVisible;
}

void ARibbonControlSizeDefinition::setLabelVisible(bool visible)
{
    A_D(ARibbonControlSizeDefinition);
    if (d.m_labelVisible != visible)
    {
        d.m_labelVisible = visible;
        d.updateSizeDefinition();
    }
}

bool ARibbonControlSizeDefinition::isPopup() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_popup;
}

void ARibbonControlSizeDefinition::setPopup(bool popup)
{
    A_D(ARibbonControlSizeDefinition);
    d.m_popup = popup;
}

int ARibbonControlSizeDefinition::visualIndex() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_visualIndex;
}

void ARibbonControlSizeDefinition::setVisualIndex(int index)
{
    A_D(ARibbonControlSizeDefinition);
    d.m_visualIndex = index;
}

bool ARibbonControlSizeDefinition::showSeparator() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_showSeparator;
}

bool ARibbonControlSizeDefinition::wordWrap() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_wordWrap;
}

void ARibbonControlSizeDefinition::setWordWrap(bool on)
{
    A_D(ARibbonControlSizeDefinition);
    if (d.m_wordWrap != on)
    {
        d.m_wordWrap = on;
        d.updateSizeDefinition();
    }
}

void ARibbonControlSizeDefinition::setShowSeparator(bool showSeparator)
{
    A_D(ARibbonControlSizeDefinition);
    d.m_showSeparator = showSeparator;
}

bool ARibbonControlSizeDefinition::isVisible() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_visible;
}

void ARibbonControlSizeDefinition::setVisible(bool visible)
{
    A_D(ARibbonControlSizeDefinition);
    d.m_visible = visible;
}

bool ARibbonControlSizeDefinition::isEnabled() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_enabled;
}

void ARibbonControlSizeDefinition::setEnabled(bool enabled)
{
    A_D(ARibbonControlSizeDefinition);
    d.m_enabled = enabled;
}

bool ARibbonControlSizeDefinition::isStretchable() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_stretchable;
}

void ARibbonControlSizeDefinition::setStretchable(bool stretchable)
{
    A_D(ARibbonControlSizeDefinition);
    d.m_stretchable = stretchable;
}

int ARibbonControlSizeDefinition::minimumItemCount() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_minItemCount;
}

void ARibbonControlSizeDefinition::setMinimumItemCount(int count)
{
    A_D(ARibbonControlSizeDefinition);
    d.m_minItemCount = count;
}

int ARibbonControlSizeDefinition::maximumItemCount() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_maxItemCount;
}

void ARibbonControlSizeDefinition::setMaximumItemCount(int count)
{
    A_D(ARibbonControlSizeDefinition);
    d.m_maxItemCount = count;
}

/* ARibbonControlPrivate */
ARibbonControlPrivate::ARibbonControlPrivate()
    : m_parentGroup(nullptr)
    , m_size(ARibbonControlSizeDefinition::GroupLarge)
    , m_action(nullptr)
{
}

ARibbonControlPrivate::~ARibbonControlPrivate()
{
    delete m_allSizes[0];
    delete m_allSizes[1];
    delete m_allSizes[2];
    delete m_allSizes[3];
    delete m_allSizes[4];
    delete m_allSizes[5];
    m_allSizes.clear();
}

void ARibbonControlPrivate::init(ARibbonGroup* parentGroup)
{
    A_P(ARibbonControl);
    if (m_parentGroup != nullptr)
    {
        m_parentGroup->removeControl(&p);

		for (auto size : m_allSizes)
		{
			delete size;
		}
		m_allSizes.clear();
    }

    m_parentGroup = parentGroup;
    m_allSizes.append(new ARibbonControlSizeDefinition(&p, ARibbonControlSizeDefinition::GroupLarge));  //Large
    m_allSizes.append(new ARibbonControlSizeDefinition(&p, ARibbonControlSizeDefinition::GroupMedium)); //Medium
    m_allSizes.append(new ARibbonControlSizeDefinition(&p, ARibbonControlSizeDefinition::GroupSmall));  //Small
    m_allSizes.append(new ARibbonControlSizeDefinition(&p, ARibbonControlSizeDefinition::GroupPopup));  //Popup
    m_allSizes.append(new ARibbonControlSizeDefinition(&p, ARibbonControlSizeDefinition::GroupSimplified));  //Simplified
    m_allSizes.append(new ARibbonControlSizeDefinition(&p, ARibbonControlSizeDefinition::GroupSimplifiedOverflow));  //Simplified Overflow
}

void ARibbonControlPrivate::changed()
{
}

/*!
\class ARibbonControl
\inmodule QtitanRibbon
*/
ARibbonControl::ARibbonControl(ARibbonGroup* parentGroup)
    : QWidget(nullptr)
{
    A_INIT_PRIVATE(ARibbonControl);
    A_D(ARibbonControl);
    d.init(parentGroup);
}

ARibbonControl::~ARibbonControl()
{
    if (parentGroup())
        parentGroup()->removeControl(this);
    A_DELETE_PRIVATE();
}

void ARibbonControl::setParentGroup(ARibbonGroup* parentGroup)
{
    A_D(ARibbonControl);
	d.init(parentGroup);
}

ARibbonGroup* ARibbonControl::parentGroup() const
{
    A_D(const ARibbonControl);
    return d.m_parentGroup;
}

QAction* ARibbonControl::defaultAction() const
{
    A_D(const ARibbonControl);
    return d.m_action;
}

void ARibbonControl::setDefaultAction(QAction* action)
{
    A_D(ARibbonControl);
    if (d.m_action == action)
        return;
    d.m_action = action;
    actionChanged();
}

void ARibbonControl::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
}

void ARibbonControl::setDirty()
{
    //Do nothing.
}

QSize ARibbonControl::getBoundedSizeHint(const QWidget* widget)
{
    QSize ret = widget->sizeHint();
    ret.rwidth() = qMin(widget->maximumWidth(), ret.width());
    ret.rheight() = qMin(widget->maximumHeight(), ret.height());
    ret.rwidth() = qMax(widget->minimumWidth(), ret.width());
    ret.rheight() = qMax(widget->minimumHeight(), ret.height());
    return ret;
}

ARibbonControlSizeDefinition* ARibbonControl::sizeDefinition(ARibbonControlSizeDefinition::GroupSize size) const
{
    A_D(const ARibbonControl);
    if ((int)size < 0 || (int)size >= d.m_allSizes.count())
    {
        Q_ASSERT(false);
        return d.m_allSizes[0];
    }
    return d.m_allSizes[(int)size];
}

bool ARibbonControl::adjustCurrentSize(bool expand)
{
    ARibbonControlSizeDefinition::GroupSize size = currentSize();
    if (size < ARibbonControlSizeDefinition::GroupSimplified)
        return false;

    if (expand)
    {
        if (size == ARibbonControlSizeDefinition::GroupSimplifiedOverflow)
            sizeChanged(ARibbonControlSizeDefinition::GroupSimplified);
    }
    else
    {
        if (size == ARibbonControlSizeDefinition::GroupSimplified)
            sizeChanged(ARibbonControlSizeDefinition::GroupSimplifiedOverflow);
    }
    return size != currentSize();
}

void ARibbonControl::sizeChanged(ARibbonControlSizeDefinition::GroupSize size)
{
    A_D(ARibbonControl);
    d.m_size = size;
}

void ARibbonControl::actionChanged()
{
    A_D(ARibbonControl);
    d.changed();
}

void ARibbonControl::updateLayout()
{
}

ARibbonControlSizeDefinition::GroupSize ARibbonControl::currentSize() const
{
    A_D(const ARibbonControl);
    ARibbonControlSizeDefinition::GroupSize old = d.m_size;
    aproch_get_groupsize(const_cast<ARibbonControlPrivate*>(&d)->m_size, parentGroup());
    if (old != d.m_size)
        const_cast<ARibbonControl*>(this)->sizeChanged(d.m_size);
    return d.m_size;
}

/* ARibbonWidgetControlPrivate */
ARibbonWidgetControlPrivate::ARibbonWidgetControlPrivate()
    : m_widget(nullptr)
    , m_visibleText(false)
    , m_ignoreActionSettings(false)
    , m_margin(4)
{
}

ARibbonWidgetControlPrivate::~ARibbonWidgetControlPrivate()
{
}

void ARibbonWidgetControlPrivate::init(bool ignoreActionSettings)
{
    A_P(ARibbonWidgetControl);
    m_ignoreActionSettings = ignoreActionSettings;
    p.sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setLabelVisible(true);
    p.sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setLabelVisible(false);
    p.sizeDefinition(ARibbonControlSizeDefinition::GroupSmall)->setLabelVisible(false);
    p.sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setLabelVisible(true);

    if (!ignoreActionSettings)
        p.setAttribute(Qt::WA_Hover);
}

void ARibbonWidgetControlPrivate::initStyleOption(QStyleOptionButton* option) const
{
    A_P(const ARibbonWidgetControl);
    option->initFrom(&p);
    QAction* action = p.defaultAction();
    if (action && !m_ignoreActionSettings)
    {
        option->icon = action->icon();
        option->text = m_visibleText ? action->text() : QString();
        QSize szIcon = option->icon.actualSize(option->rect.size());
        option->iconSize = szIcon;
    }
}

void ARibbonWidgetControlPrivate::updateFont()
{
    if (m_widget != nullptr)
        m_widget->setFont(QFont());
}

/*!
\class ARibbonWidgetControl
\inmodule QtitanRibbon
*/
ARibbonWidgetControl::ARibbonWidgetControl(ARibbonGroup* parentGroup, bool ignoreActionSettings)
    : ARibbonControl(parentGroup)
{
    A_INIT_PRIVATE(ARibbonWidgetControl);
    A_D(ARibbonWidgetControl);
    d.init(ignoreActionSettings);
}

ARibbonWidgetControl::~ARibbonWidgetControl()
{
    A_DELETE_PRIVATE();
}

int ARibbonWidgetControl::margin() const
{
    A_D(const ARibbonWidgetControl);
    return d.m_margin;
}

void ARibbonWidgetControl::setMargin(int margin)
{
    A_D(ARibbonWidgetControl);
    d.m_margin = margin;
}

QWidget* ARibbonWidgetControl::contentWidget() const
{
    A_D(const ARibbonWidgetControl);
    return d.m_widget;
}

void ARibbonWidgetControl::setContentWidget(QWidget* widget)
{
    A_D(ARibbonWidgetControl);
    if (d.m_widget != nullptr)
        delete d.m_widget;

    d.m_widget = widget;
    d.updateFont();

    if (d.m_widget != nullptr)
    {
        d.m_widget->setParent(this);
        d.m_widget->setVisible(true);
        d.m_widget->setAttribute(Qt::WA_MacSmallSize);
    }
}

void ARibbonWidgetControl::sizeChanged(ARibbonControlSizeDefinition::GroupSize size)
{
    A_D(ARibbonWidgetControl);
    ARibbonControl::sizeChanged(size);
    d.m_visibleText = sizeDefinition(size)->isLabelVisible();
}

QSize ARibbonWidgetControl::sizeHint() const
{
    A_D(const ARibbonWidgetControl);
    if (d.m_widget != nullptr)
    {
        QSize sz = getBoundedSizeHint(d.m_widget);

        QStyleOptionButton opt;
        d.initStyleOption(&opt);

        if (!opt.icon.isNull())
            sz.setWidth(sz.width() + opt.iconSize.width() + d.m_margin);

        if (!opt.text.isEmpty())
        {
            QFontMetrics fm = opt.fontMetrics;
            QSize szText = fm.boundingRect(opt.text).size();
            sz.setWidth(sz.width() + szText.width() + d.m_margin);
        }
        return sz;
    }
    return ARibbonControl::sizeHint();
}

void ARibbonWidgetControl::childEvent(QChildEvent* event)
{
    A_D(ARibbonWidgetControl);
    ARibbonControl::childEvent(event);

    if (event->type() == QEvent::StyleChange)
        d.updateFont();

    if (event->type() == QEvent::ChildRemoved)
    {
        if (ARibbonGroup* group = parentGroup())
        {
            if (QAction* action = defaultAction())
            {
                QChildEvent* childEvent = (QChildEvent*)event;
                if (childEvent->child()->isWidgetType())
                {
                    if (childEvent->child() == contentWidget())
                    {
                        d.m_widget = nullptr;
                        group->removeAction(action);
                    }
                }
            }
        }
    }
}

void ARibbonWidgetControl::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    A_D(ARibbonWidgetControl);
    if (!d.m_ignoreActionSettings && d.m_widget != nullptr)
    {
        QPainter painter(this);

        QStyleOptionButton opt;
        d.initStyleOption(&opt);

        bool enabled = d.m_widget->isEnabled();
        int posText = 0;

        if (!opt.icon.isNull())
        {
            QRect rcIcon = opt.rect;
            rcIcon.setWidth(opt.iconSize.width());
            opt.icon.paint(&painter, rcIcon, Qt::Alignment(Qt::AlignLeft|Qt::AlignVCenter), enabled ? QIcon::Normal : QIcon::Disabled);
            posText = rcIcon.width() + d.m_margin;
        }

        if (!opt.text.isEmpty())
        {
            QRect rectText = opt.rect;
            rectText.setLeft(posText);

            QFontMetrics fm = opt.fontMetrics;
            QSize szText = fm.boundingRect(opt.text).size();
            rectText.setWidth(szText.width());

            int flags = Qt::TextHideMnemonic | Qt::AlignVCenter;
            style()->drawItemText(&painter, rectText, flags, opt.palette, opt.state & QStyle::State_Enabled, opt.text, QPalette::WindowText);
        }
    }
}

void ARibbonWidgetControl::resizeEvent(QResizeEvent* event)
{
    A_D(ARibbonWidgetControl);
    if (d.m_widget != nullptr)
    {
        int x = 0;
        int y = 0;

        QSize sz = event->size();

        QStyleOptionButton opt;
        d.initStyleOption(&opt);

        if (!opt.icon.isNull())
        {
            x = opt.iconSize.width();
            x += d.m_margin;
        }
        if (!opt.text.isEmpty())
        {
            QFontMetrics fm = opt.fontMetrics;
            QSize szText = fm.boundingRect(opt.text).size();
            x += szText.width();
            x += d.m_margin;
        }

        sz.rwidth() -= x;
        QRect r = QRect(x, y, sz.width(), sz.height());
        sz.rwidth() = qMin(sz.width(), d.m_widget->maximumWidth());
        QRect controlRect = QStyle::alignedRect(layoutDirection(), Qt::AlignRight, sz, r);
        d.m_widget->setGeometry(controlRect);
    }

    ARibbonControl::resizeEvent(event);
}


/* ARibbonColumnBreakControlPrivate */
ARibbonColumnBreakControlPrivate::ARibbonColumnBreakControlPrivate()
    : m_textAlignment(Qt::AlignVCenter)
{
}

ARibbonColumnBreakControlPrivate::~ARibbonColumnBreakControlPrivate()
{
}

void ARibbonColumnBreakControlPrivate::init()
{
    A_P(ARibbonColumnBreakControl);
    QFont fnt = QApplication::font("QMenuBar");
    qreal pointSize = 0.8 * fnt.pointSizeF();
    fnt.setPointSizeF(pointSize);
    p.setFont(fnt);
}

void ARibbonColumnBreakControlPrivate::initStyleOption(QStyleOptionGroupBox* option) const
{
    A_P(const ARibbonColumnBreakControl);
    option->initFrom(&p);
    option->state |= QStyle::State_Horizontal;
    option->textAlignment = m_textAlignment;
    if (QAction* action = p.defaultAction())
        option->text = action->text();
}


/*!
\class ARibbonColumnBreakControl
\inmodule QtitanRibbon
*/
ARibbonColumnBreakControl::ARibbonColumnBreakControl(ARibbonGroup* parentGroup)
    : ARibbonWidgetControl(parentGroup, true)
{
    A_INIT_PRIVATE(ARibbonColumnBreakControl);
    A_D(ARibbonColumnBreakControl);
    d.init();
}

ARibbonColumnBreakControl::~ARibbonColumnBreakControl()
{
    A_DELETE_PRIVATE();
}

Qt::Alignment ARibbonColumnBreakControl::alignmentText() const
{
    A_D(const ARibbonColumnBreakControl);
    return d.m_textAlignment;
}

void ARibbonColumnBreakControl::setAlignmentText(Qt::Alignment alignment)
{
    A_D(ARibbonColumnBreakControl);
    if (d.m_textAlignment != alignment)
    {
        switch (alignment)
        {
            case Qt::AlignTop:
                    d.m_textAlignment = alignment;
                break;
            case Qt::AlignBottom:
                    d.m_textAlignment = alignment;
                break;
            case Qt::AlignVCenter:
                    d.m_textAlignment = alignment;
                break;
            default:
                Q_ASSERT(false);
               break;
        }
        update();
    }
}

void ARibbonColumnBreakControl::sizeChanged(ARibbonControlSizeDefinition::GroupSize size)
{
    ARibbonControl::sizeChanged(size);
    if (sizeDefinition(size)->showSeparator())
    {
    }
    else
    {
    }
}

QSize ARibbonColumnBreakControl::sizeHint() const
{
    A_D(const ARibbonColumnBreakControl);
    if (!parentGroup() || !parentGroup()->ribbonBar())
        return QWidget::sizeHint();

    QStyleOptionGroupBox opt;
    d.initStyleOption(&opt);
    const int height = parentGroup()->ribbonBar()->rowItemHeight() * parentGroup()->ribbonBar()->rowItemCount();
    int extent = style()->pixelMetric(QStyle::PM_ToolBarSeparatorExtent, &opt, parentGroup());

    if (!opt.text.isEmpty())
    {
        QFontMetrics fm = opt.fontMetrics;
        extent += fm.height();
    }

    return QSize(extent, height);
}

void ARibbonColumnBreakControl::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    A_D(ARibbonColumnBreakControl);
    QPainter p(this);
    QStyleOptionGroupBox opt;
    d.initStyleOption(&opt);
    style()->drawPrimitive(QStyle::PE_IndicatorToolBarSeparator, &opt, &p, parentGroup());
}

/*!
\class ARibbonLabelControl
\inmodule QtitanRibbon
*/
ARibbonLabelControl::ARibbonLabelControl(ARibbonGroup* parentGroup)
    : ARibbonWidgetControl(parentGroup, true/*ignoreActionSettings*/)
{
    setContentWidget(new QLabel(this));
}

ARibbonLabelControl::ARibbonLabelControl(const QString& text, ARibbonGroup* parentGroup)
    : ARibbonWidgetControl(parentGroup, true/*ignoreActionSettings*/)
{
    setContentWidget(new QLabel(text, this));
}

ARibbonLabelControl::~ARibbonLabelControl()
{
}

QLabel* ARibbonLabelControl::widget() const
{
    return qobject_cast<QLabel*>(contentWidget());
}


/* ARibbonToolBarSeparator */
ARibbonToolBarSeparator::ARibbonToolBarSeparator(ARibbonToolBarControl* parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

QSize ARibbonToolBarSeparator::sizeHint() const
{
    QSize sz = QWidget::sizeHint();
    QStyleOption opt;
    opt.initFrom(this);
    opt.state |= QStyle::State_Horizontal;
    const int extent = style()->pixelMetric(QStyle::PM_ToolBarSeparatorExtent, &opt, parentWidget());
    return QSize(extent, sz.width());
}

void ARibbonToolBarSeparator::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    QStyleOption opt;
    opt.initFrom(this);
    opt.state |= QStyle::State_Horizontal;
    style()->drawPrimitive(QStyle::PE_IndicatorToolBarSeparator, &opt, &p, parentWidget());
}

/* ARibbonToolBarOverflowMenu */
ARibbonToolBarOverflowMenu::ARibbonToolBarOverflowMenu(ARibbonToolBarControl* parent)
    : APopupMenu(parent)
    , m_parent(parent)
{
}

ARibbonToolBarOverflowMenu::~ARibbonToolBarOverflowMenu()
{
}

void ARibbonToolBarOverflowMenu::fillMenu()
{
    QAction* action = nullptr;
    ARibbonToolBarControlPrivate* priv = ARibbonToolBarControlPrivate::_get(m_parent);
    ARibbonToolBarControlPrivate::ARibbonToolBarActionGroupList::iterator it = priv->m_groups.begin();
    for (; it != priv->m_groups.end(); ++it)
    {
        if (it->row != -1)
            continue;
        ARibbonToolBarControlPrivate::ARibbonToolBarActionGroupItems::iterator jt = it->items.begin();
        for (; jt != it->items.end(); ++jt)
        {
            action = jt->action;
            if (action != nullptr && action->isVisible())
            {
                if (action->isSeparator())
                    addAction(action);
                else if (qobject_cast<QToolButton*>(jt->widget))
                    addAction(action);
                else if (jt->widget != nullptr)
                {
                    jt->rect = jt->widget->geometry();
                    action = new ARibbonOverflowAction(jt->widget, this);
                    addAction(action);
                }
            }
        }
    }
}

void ARibbonToolBarOverflowMenu::clearMenu()
{
    clear();
    ARibbonToolBarControlPrivate* priv = ARibbonToolBarControlPrivate::_get(m_parent);
    ARibbonToolBarControlPrivate::ARibbonToolBarActionGroupList::const_iterator it = priv->m_groups.constBegin();
    for (; it != priv->m_groups.constEnd(); ++it)
    {
        ARibbonToolBarControlPrivate::ARibbonToolBarActionGroupItems::const_iterator jt = it->items.constBegin();
        for (; jt != it->items.constEnd(); ++jt)
        {
            if (jt->action != nullptr && jt->widget != nullptr && jt->widget->parent() == nullptr)
            {
                jt->widget->setParent(m_parent);
                jt->widget->setGeometry(jt->rect);
                jt->widget->setVisible(true);
            }
        }
    }
}

bool ARibbonToolBarOverflowMenu::event(QEvent* event)
{
    if (event->type() == QEvent::Show)
        fillMenu();
    else if (event->type() == QEvent::Hide)
        clearMenu();
    return APopupMenu::event(event);
}
/* ARibbonColumnBreakControlPrivate */
ARibbonToolBarControlPrivate::ARibbonToolBarControlPrivate()
    : m_overflowMenu(nullptr)
    , m_margin(0)
    , m_rowsCount(2)
    , m_dirty(true)
    , m_calcRows(-1)
    , m_currColumnIndex(0)
    , m_fullWidth(0)
    , m_separatorWidth(0)
{
}

ARibbonToolBarControlPrivate::~ARibbonToolBarControlPrivate()
{
}

void ARibbonToolBarControlPrivate::init()
{
    A_P(ARibbonToolBarControl);
    m_overflowMenu = new ARibbonToolBarOverflowMenu(&p);
}

void ARibbonToolBarControlPrivate::recalcWidths(int height)
{
    A_P(ARibbonToolBarControl);
    ARibbonGroup* parentGroup = p.parentGroup();
    if (parentGroup == nullptr || parentGroup->isHidden())
        return;
    ARibbonBar* ribbonBar = parentGroup->ribbonBar();
    if (ribbonBar == nullptr)
        return;

    m_vecWidths.clear();
    m_mapNonOptWidths.clear();
    ARibbonGroupStyleOption opt;
    ARibbonGroupPrivate::_get(parentGroup)->initStyleOption(opt);
    m_separatorWidth = parentGroup->style()->pixelMetric(QStyle::PM_ToolBarSeparatorExtent, &opt, &p);
    
    int rowHeight = ribbonBar->rowItemHeight();
    int maxWidth = 0;
    const int minWidth = 0;
    m_currColumnIndex = 0;
    const int maxRows = 3;
    int lastWidth = -1;
    int dx = 16;

    //Recalculation default sizes for groups.
    for (ARibbonToolBarActionGroupList::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
    {
        if (maxWidth > 0)
            maxWidth += (m_separatorWidth + p.margin());
        ARibbonToolBarActionsGroup& actionGroup = *it;
        QSize groupSize = calcGroupSizeHint(actionGroup.items);
        groupSize.setHeight(groupSize.height() < rowHeight ? rowHeight : groupSize.height());
        actionGroup.defaultSize = groupSize;
        maxWidth += groupSize.width();
    }

    if (p.currentSize() == ARibbonControlSizeDefinition::GroupSimplified)
    {
        QRect rect(0, 0, maxWidth + dx, height);
        do
        {
            updateGroupsGeometry(rect, false);
            if (lastWidth == -1 || m_fullWidth < lastWidth)
            {
                lastWidth = m_fullWidth;
                if (lastWidth <= minWidth || (lastWidth <= minWidth && m_groups.size() == 1))
                {
                    if (m_vecWidths.size() == 0)
                    {
                        m_vecWidths.append(lastWidth);
                        m_mapNonOptWidths.insert(m_fullWidth + p.margin(), rect.width());
                        return;
                    }
                    break;
                }
                m_vecWidths.append(lastWidth);
                m_mapNonOptWidths.insert(m_fullWidth + p.margin(), rect.width());
                rect.setRight(lastWidth - dx);
            }
            else
                rect.setRight(rect.right() - dx);
        }
        while (rect.width() > 0);
    }
    else
    {
        for (int rows = 1; rows <= maxRows; rows++)
        {
            QRect rect(QPoint(0, 0), QSize(minWidth, height));

            for (;; rect.setRight(rect.right() + dx))
            {
                if (rect.width() >= maxWidth + dx)
                    break;

                updateGroupsGeometry(rect, false);

                if (lastWidth != -1 && m_fullWidth > lastWidth)
                    break;

                if (m_calcRows == rows && m_fullWidth > 0)
                {
                    if (m_calcRows == maxRows - 1)
                        m_vecWidths.clear();

                    m_vecWidths.append(m_fullWidth);
                    m_mapNonOptWidths.insert(m_fullWidth  + p.margin(), rect.width());
                    lastWidth = m_fullWidth;
                    break;
                }
            }
        }
    }

    if (m_vecWidths.count() == 0)
        m_vecWidths.append(minWidth);
    if (m_vecWidths.count() == 1)
        m_vecWidths.append(m_vecWidths[0]);

    m_vecWidths.append(minWidth);
}

void ARibbonToolBarControlPrivate::showDefaultButton()
{
    A_P(ARibbonToolBarControl);
    ARibbonGroup* parentGroup = p.parentGroup();
    if (parentGroup == nullptr || parentGroup->isHidden())
        return;
#if 0 
    ARibbonGroupStyleOption opt;
    ARibbonGroupPrivate::_get(parentGroup)->initStyleOption(opt);
    const int widthDefaultButton = p.parentGroup()->style()->pixelMetric((QStyle::PixelMetric)ACommonStyle::PM_RibbonGroupReducedWidth, &opt, parentGroup);
    currentRect.setRight(m_rect.left() + widthDefaultButton);
#endif
    m_calcRows = 0;
}

void ARibbonToolBarControlPrivate::addActionToGroup(QAction* action, QWidget* widget)
{
    bool endgroup = m_groups.size() == 0 || (m_groups.last().items.size() > 0 && m_groups.last().items.last().action->isSeparator());
    if (endgroup)
        m_groups.append(ARibbonToolBarActionsGroup());
    ARibbonToolBarActionGroupItems& lastGroup = m_groups.last().items;
    ARibbonToolBarActionGroupItem item(action, widget);
    lastGroup.append(item);
}

static int indexOfAction(const ARibbonToolBarControlPrivate::ARibbonToolBarActionGroupItems& group, QAction* action)
{
    int index = 0;
    for (ARibbonToolBarControlPrivate::ARibbonToolBarActionGroupItems::const_iterator it = group.constBegin(); it != group.constEnd(); ++it)
    {
        if (it->action == action)
            return index;
        index++;
    }
    return -1;
}

void ARibbonToolBarControlPrivate::removeActionFromGroup(QAction* action)
{
    for (ARibbonToolBarActionGroupList::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
    {
        ARibbonToolBarActionsGroup group = *it;
        ARibbonToolBarActionGroupItems& items = group.items;
        int index = indexOfAction(items, action);
        if (index != -1)
        {
            delete items.at(index).widget;
            items.removeAt(index);
            if (items.size() == 0)
            {
                m_groups.erase(it);
            }
            else if (action->isSeparator() && (it + 1) != m_groups.end())
            {
                ARibbonToolBarActionsGroup group2 = *(it + 1);
                items += group2.items;//*(it + 1);
//                updateGroupProperties(items);
                m_groups.erase(it + 1);
            }
            return;
        }
    }
}

#define MAX_ROW 1000

void ARibbonToolBarControlPrivate::updateGroupsGeometry(const QRect& rect, bool updateWidgets)
{
    A_P(ARibbonToolBarControl);
    ARibbonGroup* parentGroup = p.parentGroup();
    if (parentGroup == nullptr || parentGroup->isHidden())
        return;

    ARibbonBar* ribbonBar = parentGroup->ribbonBar();
    if (ribbonBar == nullptr)
        return;

    QRect currentRect = rect;
    m_fullWidth = 0;
    m_calcRows = 0;

    QFontMetrics fm(parentGroup->titleFont());
    const QSize sizeCaption = fm.boundingRect(parentGroup->title()).size();

    bool m_trancateCaption = false;
    if (!m_trancateCaption)
        currentRect.setRight(currentRect.left() + qMax(rect.width(), sizeCaption.width()));

    QSize size = rect.size();

    int rowHeight = ribbonBar->rowItemHeight();

    int x = 0, y = 0;
    int startX = x;
    bool isFullHeight = currentRect.bottom() == 32767;
    int maxWidth = size.width();

    if (updateWidgets && m_mapNonOptWidths.size() > 0 && m_mapNonOptWidths.contains(maxWidth))
        maxWidth = m_mapNonOptWidths.value(maxWidth); //TODO Remove me

    for (ARibbonToolBarActionGroupList::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
    {
        ARibbonToolBarActionsGroup& actionGroup = *it;
        actionGroup.row = -1;
        QSize groupSize = actionGroup.defaultSize;
        const bool isLargeButton = groupSize.height() > rowHeight;
        if (isLargeButton)
        {
            QRect groupRect(QPoint(currentRect.left() + x, currentRect.top() /*+ m_margin / 2*/),
                QSize(groupSize.width(), isFullHeight ? groupSize.height() : size.height()));
            actionGroup.rect = groupRect;
            actionGroup.row = MAX_ROW;
            x += groupSize.width();
            m_fullWidth = qMax(m_fullWidth, x);
            x += (p.margin() + m_separatorWidth);
            startX = x;
        }
    }

    int bottom = 0;
    QVector<int> vecRowWidths;
    for (ARibbonToolBarActionGroupList::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
    {
        ARibbonToolBarActionsGroup& actionGroup = *it;
        if (actionGroup.row != -1)
            continue;
        QSize groupSize = actionGroup.defaultSize;
        groupSize.setHeight(groupSize.height() < rowHeight ? rowHeight : groupSize.height());

        if (x + groupSize.width() > maxWidth)
        {
            if (x == startX)
            {
                showDefaultButton();
                return;
            }

            y += rowHeight;
            vecRowWidths.append(x);

            m_calcRows++;
            x = startX;
            rowHeight = 0;
        }

        if (y + groupSize.height() > size.height())
        {
            showDefaultButton();
            return;
        }

        if (x > startX)
            x += (p.margin() + m_separatorWidth);

        QRect groupRect(QPoint(currentRect.left() + x /*+ m_margin*/, currentRect.top() + y /*+ m_margin / 2*/), groupSize);

        actionGroup.rect = groupRect;
        actionGroup.row = m_calcRows;

        rowHeight = qMax(rowHeight, groupSize.height());

        x += groupSize.width();
        m_fullWidth = qMax(m_fullWidth, x);

        bottom = qMax(bottom, groupRect.bottom());
    }

    vecRowWidths.append(x);
    m_calcRows++;

    if (isFullHeight)
    {
        //TODO: remove me
        currentRect.setBottom(bottom + sizeCaption.height() + p.margin() / 2);
        size.setHeight(currentRect.height() - sizeCaption.height() - p.margin() / 2);
    }

    int i = 0;
    if (m_calcRows > 1)
    {
        bool recalcFullWidth = false;
        while (true)
        {
            int maxRowWidth = 0, maxRowIndex = -1;
            for (i = 0; i < vecRowWidths.size(); i++)
            {
                if (vecRowWidths[i] > maxRowWidth)
                {
                    maxRowWidth = vecRowWidths[i];
                    maxRowIndex = i;
                }
            }

            if (maxRowIndex < 0)
                break;

            int minWidth = 9999;
            ARibbonToolBarActionsGroup* minElem = nullptr;

            for (ARibbonToolBarActionGroupList::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
            {
                ARibbonToolBarActionsGroup& actionGroup = *it;
                if (actionGroup.row == maxRowIndex)
                {
                    QRect groupRect = actionGroup.rect;
                    if (!groupRect.isEmpty() && groupRect.width() < minWidth)
                    {
                        minWidth = groupRect.width();
                        minElem = &actionGroup;
                    }
                }
            }

            if (minElem == nullptr)
                break;

            bool moved = false;
            for (i = maxRowIndex + 1; i < vecRowWidths.size(); i++)
            {
                if (vecRowWidths[i] + minWidth < maxRowWidth)
                {
                    int x = 0, y = 0;
                    for (ARibbonToolBarActionGroupList::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
                    {
                        ARibbonToolBarActionsGroup& actionGroup = *it;

                        if (actionGroup.row == i)
                        {
                            x = qMax(actionGroup.rect.right() + (p.margin() + m_separatorWidth), x);
                            y = actionGroup.rect.top();
                        }
                        else if (actionGroup.row == maxRowIndex)
                        {
                            QRect groupRect = actionGroup.rect;

                            if (groupRect.left() > minElem->rect.left())
                            {
                                int dx = -(minWidth + (p.margin() + m_separatorWidth));
                                groupRect.adjust(dx, 0, dx, 0);
                                actionGroup.rect = groupRect;
                            }
                        }
                    }

                    minElem->rect = QRect(QPoint(x, y), minElem->rect.size());
                    minElem->row = i;

                    vecRowWidths[i] += minWidth;
                    vecRowWidths[maxRowIndex] -= minWidth;

                    recalcFullWidth = true;
                    moved = true;
                    break;
                }
            }
            if (!moved)
                break;
        }

        if (recalcFullWidth)
        {
            m_fullWidth = 0;
            for (ARibbonToolBarActionGroupList::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
                m_fullWidth = qMax(m_fullWidth, (*it).rect.right() + 1);
            //m_fullWidth -= currentRect.left() /*+ m_margin*/;
        }
    }

    if (!isFullHeight && m_calcRows > 1)
    {
        int yOffset = (size.height() - m_calcRows * rowHeight) / (m_calcRows + 1);
        if (yOffset > 0)
        {
            for (ARibbonToolBarActionGroupList::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
            {
                ARibbonToolBarActionsGroup& actionGroup = *it;

                int row = actionGroup.row;
                QRect groupRect = actionGroup.rect;

                if (row != MAX_ROW && !groupRect.isEmpty())
                {
                    int dy = (yOffset * (row + 1) - row);
                    groupRect.adjust(0, dy - 1, 0, dy);
                    actionGroup.rect = groupRect;
                }
            }
        }
    }

    if (updateWidgets)
        updateGeometryWidget();
}

void ARibbonToolBarControlPrivate::updateGeometryWidget()
{
    A_P(ARibbonToolBarControl);
    ARibbonGroup* parentGroup = p.parentGroup();
    if (parentGroup == nullptr || parentGroup->isHidden())
        return;
    ARibbonBar* ribbonBar = parentGroup->ribbonBar();
    if (ribbonBar == nullptr)
        return;

//    QRect groupRect = parentGroup->rect();

    int rowHeight = ribbonBar->rowItemHeight();
    QList<QWidget*> showWidgets, hideWidgets;

    int currentRow = 0;
    QWidget* lastWidget = nullptr;
    ARibbonToolBarSeparator* currentSeparator = nullptr;
    for (ARibbonToolBarActionGroupList::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
    {
        ARibbonToolBarActionsGroup& actionGroup = *it;
        if (currentRow != actionGroup.row && actionGroup.row != MAX_ROW)
        {
            currentRow = actionGroup.row;
            if (currentSeparator)
                hideWidgets << currentSeparator;
        }

        QRect rect = actionGroup.rect;
        int x = rect.left();
        QList<ARibbonToolBarActionGroupItem> items = actionGroup.items;
        for (QList<ARibbonToolBarActionGroupItem>::iterator itItem = items.begin(); itItem != items.end(); ++itItem)
        {
            ARibbonToolBarActionGroupItem& item = *itItem;
            if (item.widget != nullptr)
            {
                QSize sizeElem = ARibbonControl::getBoundedSizeHint(item.widget);
                if (lastWidget)
                {
                    if (ARibbonToolBarSeparator* separatorControl = qobject_cast<ARibbonToolBarSeparator*>(item.widget))
                    {
                        QSize sz = lastWidget->sizeHint();
                        sizeElem.setHeight(sz.height());
                        currentSeparator = separatorControl;
                        showWidgets << separatorControl;
                    }
                    else
                        sizeElem.setHeight(sizeElem.height() < rowHeight ? rowHeight : sizeElem.height());
                }
                else
                    sizeElem.setHeight(sizeElem.height() < rowHeight ? rowHeight : sizeElem.height());

                if (item.widget != nullptr)
                    lastWidget = item.widget;

                if (item.isEmpty())
                    hideWidgets << item.widget;
                else
                {
                    item.widget->setGeometry(QRect(QPoint(x, rect.top()), sizeElem));
                    showWidgets << item.widget;
                    x += sizeElem.width();
                }
            }
        }
    }

    if (currentSeparator != nullptr && currentSeparator == lastWidget)
        hideWidgets << currentSeparator;

    // we have to do the show/hide here, because it triggers more calls to setGeometry
    for (int i = 0, count = showWidgets.count(); i < count; ++i)
        showWidgets.at(i)->show();

    for (int i = 0, count = hideWidgets.count(); i < count; ++i)
        hideWidgets.at(i)->hide();
}

void ARibbonToolBarControlPrivate::updateGroupProperties(ARibbonToolBarActionGroupItems& group)
{
    QWidget* firstWidget = nullptr;
    QWidget* lastWidget = nullptr;
    for (ARibbonToolBarActionGroupItems::iterator it = group.begin(); it != group.end(); ++it)
    {
        if (it->isEmpty() || it->widget == nullptr || it->action->isSeparator())
            continue;

        if (firstWidget == nullptr)
            firstWidget = it->widget;

        lastWidget = it->widget;
        lastWidget->setProperty(_aproch_Begin_Group, firstWidget == it->widget);
        lastWidget->setProperty(_aproch_Group, true);
        lastWidget->setProperty(_aproch_End_Group, false);
    }

    if (lastWidget != nullptr)
        lastWidget->setProperty(_aproch_End_Group, true);
}

QSize ARibbonToolBarControlPrivate::calcGroupSizeHint(const ARibbonToolBarActionGroupItems& group) const
{
    A_P(const ARibbonToolBarControl);
    ARibbonGroup* parentGroup = p.parentGroup();
    if (parentGroup == nullptr || parentGroup->isHidden())
        return QSize();
    ARibbonBar* ribbonBar = parentGroup->ribbonBar();
    Q_ASSERT(ribbonBar != nullptr);

    QSize ret = QSize(0, 0);
    for (ARibbonToolBarControlPrivate::ARibbonToolBarActionGroupItems::const_iterator it = group.constBegin(); it != group.constEnd(); ++it)
    {
        if (it->action->isSeparator() && it->action->isVisible())
            continue;

        Q_ASSERT(it->widget != nullptr);
        if (it->widget == nullptr)
            continue;

        if (!it->isEmpty())
        {
            QSize s = ARibbonControl::getBoundedSizeHint(it->widget);
            s.setHeight(s.height() < ribbonBar->rowItemHeight() ? ribbonBar->rowItemHeight() : s.height());
            ret.rwidth() += s.width();
            ret.rheight() = qMax(ret.height(), s.height());
        }
    }
    return ret;
}

void ARibbonToolBarControlPrivate::updateGroupItems()
{
    A_P(ARibbonToolBarControl);
    ARibbonGroup* parentGroup = p.parentGroup();
    if (parentGroup == nullptr)
        return;
    ARibbonBar* ribbonBar = parentGroup->ribbonBar();
    if (ribbonBar == nullptr)
        return;

    recalcWidths(ribbonBar->rowItemHeight() * ribbonBar->rowItemCount());
    for (ARibbonToolBarActionGroupList::iterator itGroup = m_groups.begin(); itGroup != m_groups.end(); ++itGroup)
        updateGroupProperties((*itGroup).items);
}

void ARibbonToolBarControlPrivate::updateFont()
{
    for (ARibbonToolBarActionGroupList::const_iterator it = m_groups.begin(); it != m_groups.end(); ++it)
    {
        const ARibbonToolBarActionsGroup& actionGroup = *it;
        for (ARibbonToolBarControlPrivate::ARibbonToolBarActionGroupItems::const_iterator jt = actionGroup.items.constBegin(); 
            jt != actionGroup.items.constEnd(); ++jt)
        {
            const ARibbonToolBarActionGroupItem& item = *jt;
            item.widget->setFont(QFont());
        }
    }
}

/*!
\class ARibbonToolBarControl
\inmodule QtitanRibbon
*/
ARibbonToolBarControl::ARibbonToolBarControl(ARibbonGroup* parentGroup)
    : ARibbonControl(parentGroup)
{
    A_INIT_PRIVATE(ARibbonToolBarControl);
    A_D(ARibbonToolBarControl);
    d.init();
}

ARibbonToolBarControl::~ARibbonToolBarControl()
{
    A_DELETE_PRIVATE();
}

int ARibbonToolBarControl::rowsCount()
{
    A_D(ARibbonToolBarControl);
    return d.m_rowsCount;
}

void ARibbonToolBarControl::setRowsCount(int count)
{
    A_D(ARibbonToolBarControl);
    d.m_rowsCount = count;
}

void ARibbonToolBarControl::setDirty()
{
    A_D(ARibbonToolBarControl);
    d.m_dirty = true;
    for (ARibbonToolBarControlPrivate::ARibbonToolBarActionGroupList::iterator it = d.m_groups.begin(); it != d.m_groups.end(); ++it)
        for (ARibbonToolBarControlPrivate::ARibbonToolBarActionGroupItems::iterator jt = it->items.begin(); jt != it->items.end(); ++jt)
            if (ARibbonButton* button = qobject_cast<ARibbonButton*>(jt->widget))
                button->changed();
}

void ARibbonToolBarControl::clear()
{
    QList<QAction *> actions = this->actions();
    for (int i = 0; i < actions.size(); i++)
        removeAction(actions.at(i));
}

QAction* ARibbonToolBarControl::addWidget(QWidget* widget)
{
    QWidgetAction* action = new QWidgetAction(this);
    action->setDefaultWidget(widget);
    ARibbonControl::addAction(action);
    return action;
}

QAction* ARibbonToolBarControl::addWidget(const QIcon& icon, const QString& text, QWidget* widget)
{
    QWidgetAction* action = new QWidgetAction(this);
    action->setIcon(icon);
    action->setText(text);
    action->setDefaultWidget(widget);
    ARibbonControl::addAction(action);
    return action;
}

QAction* ARibbonToolBarControl::addMenu(const QIcon& icon, const QString& text, QMenu* menu, QToolButton::ToolButtonPopupMode mode)
{
    Q_ASSERT(menu != nullptr);
    Q_ASSERT_X(mode != QToolButton::DelayedPopup, "ARibbonToolBarControl::addMenu", "The use of mode is not valid");

    if (mode == QToolButton::DelayedPopup)
        return nullptr;

    QAction* action = menu->menuAction();
    action->setIcon(icon);
    menu->setTitle(text);

    ARibbonControl::addAction(action);

    ARibbonButton* button = qobject_cast<ARibbonButton *>(widgetByAction(action));
    if (button != nullptr)
    {
        button->setPopupMode(mode);
        button->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
    return action;
}

QAction* ARibbonToolBarControl::addAction(QAction* action, Qt::ToolButtonStyle style, QMenu* menu, QToolButton::ToolButtonPopupMode mode)
{
    addAction(action);
    ARibbonButton* button = qobject_cast<ARibbonButton *>(widgetByAction(action));
    Q_ASSERT(button != nullptr);
    button->setToolButtonStyle(style);
    if (menu || action->menu())
        button->setPopupMode(mode);
    if (menu)
        button->setMenu(menu);
    return action;
}

QAction* ARibbonToolBarControl::insertAction(QAction* before, QAction* action, Qt::ToolButtonStyle style, QMenu* menu, QToolButton::ToolButtonPopupMode mode)
{
    insertAction(before, action);
    ARibbonButton* button = qobject_cast<ARibbonButton *>(widgetByAction(action));
    Q_ASSERT(button != nullptr);
    button->setToolButtonStyle(style);
    if (menu || action->menu())
        button->setPopupMode(mode);
    if (menu)
        button->setMenu(menu);
    return action;
}

QAction* ARibbonToolBarControl::addAction(const QString& text)
{
    QAction *action = new QAction(text, this);
    ARibbonControl::addAction(action);
    return action;
}

QAction* ARibbonToolBarControl::addAction(const QIcon& icon, const QString& text)
{
    QAction *action = new QAction(icon, text, this);
    ARibbonControl::addAction(action);
    return action;
}

QAction* ARibbonToolBarControl::addAction(const QString& text, const QObject* receiver, const char* member)
{
    QAction *action = new QAction(text, this);
    QObject::connect(action, SIGNAL(triggered(bool)), receiver, member);
    ARibbonControl::addAction(action);
    return action;
}

QAction* ARibbonToolBarControl::addAction(const QIcon& icon, const QString& text, const QObject* receiver, const char* member)
{
    QAction* action = new QAction(icon, text, this);
    QObject::connect(action, SIGNAL(triggered(bool)), receiver, member);
    ARibbonControl::addAction(action);
    return action;
}

QAction* ARibbonToolBarControl::addAction(const QIcon& icon, const QString& text, Qt::ToolButtonStyle style, QMenu* menu, QToolButton::ToolButtonPopupMode mode)
{
    QAction* action = nullptr;
    if (mode == QToolButton::InstantPopup && menu)
    {
        action = menu->menuAction();
        action->setIcon(icon);
        menu->setTitle(text);
    }
    else
        action = new QAction(icon, text, this);

    ARibbonControl::addAction(action);

    if (ARibbonButton* button = qobject_cast<ARibbonButton *>(widgetByAction(action)))
    {
        button->setToolButtonStyle(style);
        button->setLargeIcon(style == Qt::ToolButtonTextUnderIcon);
    }
    return action;
}

QAction* ARibbonToolBarControl::addSeparator()
{
    QAction* action = new QAction(this);
    action->setSeparator(true);
    ARibbonControl::addAction(action);
    return action;
}

QMenu* ARibbonToolBarControl::overflowMenu()
{
    A_D(ARibbonToolBarControl);
    return d.m_overflowMenu;
}

bool ARibbonToolBarControl::isOverflowMenuIsEmpty() const
{
    A_D(const ARibbonToolBarControl);
    ARibbonToolBarControlPrivate::ARibbonToolBarActionGroupList::const_iterator it = d.m_groups.constBegin();
    for (; it != d.m_groups.constEnd(); ++it)
    {
        if (it->row == -1)
            return false;
    }
    return true;
}

QWidget* ARibbonToolBarControl::widgetByAction(QAction* action)
{
    A_D(ARibbonToolBarControl);
    for (ARibbonToolBarControlPrivate::ARibbonToolBarActionGroupList::iterator it = d.m_groups.begin(); it != d.m_groups.end(); ++it)
    {
        ARibbonToolBarControlPrivate::ARibbonToolBarActionsGroup& group = *it;
        ARibbonToolBarControlPrivate::ARibbonToolBarActionGroupItems& items = group.items;
        int index = indexOfAction(items, action);
        if (index != -1)
            return items[index].widget;
    }
    return nullptr;
}

void ARibbonToolBarControl::sizeChanged(ARibbonControlSizeDefinition::GroupSize size)
{
    ARibbonControl::sizeChanged(size);
    A_D(ARibbonToolBarControl);
    if (d.m_vecWidths.isEmpty())
        return;

    switch (size)
    {
        case ARibbonControlSizeDefinition::GroupLarge:
        case ARibbonControlSizeDefinition::GroupMedium:
            setRowsCount(2);
            break;
        case ARibbonControlSizeDefinition::GroupPopup:
            setRowsCount(2);
            break;
        case ARibbonControlSizeDefinition::GroupSmall:
            setRowsCount(3);
            break;
        case ARibbonControlSizeDefinition::GroupSimplified:
            setRowsCount(1);
            break;
        default:
            break;
    }

    for (ARibbonToolBarControlPrivate::ARibbonToolBarActionGroupList::iterator it = d.m_groups.begin(); it != d.m_groups.end(); ++it)
        for (ARibbonToolBarControlPrivate::ARibbonToolBarActionGroupItems::iterator jt = it->items.begin(); jt != it->items.end(); ++jt)
            if (ARibbonButton* button = qobject_cast<ARibbonButton*>(jt->widget))
                button->setSimplifiedMode(size == ARibbonControlSizeDefinition::GroupSimplified || size == ARibbonControlSizeDefinition::GroupSimplifiedOverflow);
}

bool ARibbonToolBarControl::adjustCurrentSize(bool expand)
{
    A_D(ARibbonToolBarControl);
    if (currentSize() == ARibbonControlSizeDefinition::GroupSimplified)
    {
        int current = d.m_currColumnIndex;
        const int old = current;

        if (expand)
            current--;
        else
            current++;

        if (current >= 0 && current < d.m_vecWidths.count())
            d.m_currColumnIndex = current;

        if (old != d.m_currColumnIndex)
            return true;
    }
    return ARibbonControl::adjustCurrentSize(expand);
}

void ARibbonToolBarControl::updateLayout()
{
    A_D(ARibbonToolBarControl);
    if (parentGroup() == nullptr)
        return;

    if (d.m_dirty)
    {
        d.m_dirty = false;
        d.updateGroupItems();
    }

    if (d.m_vecWidths.isEmpty())
        return;

    if (ARibbonBar* ribbonBar = parentGroup()->ribbonBar())
    {
        QSize sz;
        int currWidthIndex = rowsCount() == 2 ? 0 : 1;
        if (currentSize() == ARibbonControlSizeDefinition::GroupSimplified)
            currWidthIndex = d.m_currColumnIndex;

        const int currPanelWidth = d.m_vecWidths[currWidthIndex] /*+ 2 * margin()*/;
        sz.setWidth(currPanelWidth);
        const int vertMargin = ACommonStylePrivate::dpiScaled(3, this);
        const int height = (ribbonBar->rowItemHeight() * ribbonBar->rowItemCount());// /*+ 2 * (margin() / 2)*/ +vertMargin;
        sz.setHeight(height);
        d.updateGroupsGeometry(QRect(QPoint(margin(), 0/*margin() / 2*/), sz), true);
        //d.updateGeometryWidget();
        d.m_sizeHint = sz;
        d.m_sizeHint.setWidth(sz.width() + margin() * 2);
    }
}

/*! \reimp */
QSize ARibbonToolBarControl::sizeHint() const
{
    A_D(const ARibbonToolBarControl);
    return d.m_sizeHint;
}

/*! \reimp */
void ARibbonToolBarControl::paintEvent(QPaintEvent* event)
{
    ARibbonControl::paintEvent(event);
}

/*! \reimp */
void ARibbonToolBarControl::actionEvent(QActionEvent* event)
{
    A_D(ARibbonToolBarControl);
    QAction* action = event->action();
    switch (event->type())
    {
        case QEvent::ActionAdded:
            {
                QWidget* widget = nullptr;
                if (!action->isSeparator())
                {
                    QWidgetAction* widgetAction = qobject_cast<QWidgetAction *>(action);
                    if (widgetAction)
                        widget = widgetAction->requestWidget(this);

                    if (widget == nullptr)
                    {
                        ARibbonButton* button = new ARibbonButton(this);
                        button->setAutoRaise(true);
                        button->setFocusPolicy(Qt::NoFocus);
                        button->setDefaultAction(action);
                        button->setToolButtonStyle(Qt::ToolButtonIconOnly);
                        widget = button;
                    }
                    widget->hide();
                }
                else
                {
                    ARibbonToolBarSeparator* separator = new ARibbonToolBarSeparator(this);
                    widget = separator;
                }

                d.addActionToGroup(action, widget);
                widget->setFont(QFont());
                setDirty();
                break;
            }
        case QEvent::ActionChanged:
            {
                setDirty();
            }
            break;
        case QEvent::ActionRemoved:
            {
                d.removeActionFromGroup(action);
                setDirty();
            }
            break;
        default:
            break;
    }
}

/*! \reimp */
void ARibbonToolBarControl::changeEvent(QEvent* event)
{
    A_D(ARibbonToolBarControl);
    ARibbonControl::changeEvent(event);

    switch (event->type())
    {
        case QEvent::FontChange:
        case QEvent::StyleChange:
            {
                if (event->type() == QEvent::StyleChange)
                    d.updateFont();
                setDirty();
            }
            break;
        default:
            break;
    }
}

int ARibbonToolBarControl::margin() const
{
    A_D(const ARibbonToolBarControl);
    return d.m_margin;
}

void ARibbonToolBarControl::setMargin(int margin)
{
    A_D(ARibbonToolBarControl);
    d.m_margin = margin;
}

APROCH_NAMESPACE_END