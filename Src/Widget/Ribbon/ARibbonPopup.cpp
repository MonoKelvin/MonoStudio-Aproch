/****************************************************************************
 * @file    ARibbonPopup.cpp
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
#include "ARibbonPopup.h"
#include "Widget/Style/AStyleOption.h"
#include "Widget/Style/ACommonStyle_p.h"
#include "ARibbonBar_p.h"
#include "ARibbonGroup_p.h"
#include "ARibbonButtonControls.h"

 //
 // The most of the following code is copied from Qtitan.
 //
 // Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 //

APROCH_NAMESPACE_BEGIN

/* ARibbonPagePopup */
ARibbonPagePopup::ARibbonPagePopup(ARibbonBar* ribbonBar)
    : QWidget(ribbonBar, Qt::Popup), m_ribbonBar(ribbonBar)
{
    Q_ASSERT(ribbonBar != nullptr);
}

ARibbonBar* ARibbonPagePopup::ribbonBar() const
{
    return m_ribbonBar;
}

void ARibbonPagePopup::popup(const QRect& rect)
{
    QFont font = ribbonBar()->font();
    QPalette palette = ribbonBar()->palette();
    QFont_setResolveMask(font, QFont::AllPropertiesResolved);
    QPalette_setResolveMask(palette, 0xFFFFFF);
    setFont(font);
    setPalette(palette);

    QRect screen = aproch_availableGeometry(this);
    QPoint pos = rect.topLeft();

    if (pos.y() + rect.height() > screen.height())
        pos.setY(screen.height() - rect.height());
    if (pos.y() < 0)
        pos.setY(0);
    QRect r = QRect(pos, rect.size());
    setGeometry(r);
    show();
}


/*! \reimp */
void ARibbonPagePopup::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    ARibbonPage* page = ribbonBar()->page(ribbonBar()->currentPageIndex());
    if (page == nullptr)
        return;
    QPainter p(this);
    ARibbonStyleOption opt;
    opt.initFrom(this);
    opt.contextColor = page->contextColor();
    style()->drawPrimitive((QStyle::PrimitiveElement)ACommonStyle::PE_RibbonFrameGroups, &opt, &p, this);
}

/*! \reimp */
bool ARibbonPagePopup::event(QEvent* event)
{
    switch (event->type())
    {
    case QEvent::Show:
    {
    }
    break;
#ifdef Q_OS_MAC
    case QEvent::Close:
    {
        if (event->spontaneous())
        {
            event->ignore();
            return true;
        }
    }
    break;
#endif
    case QEvent::Hide:
    {
        ARibbonBar* _ribbonBar = ribbonBar();
        if (_ribbonBar == nullptr)
            break;
        _ribbonBar->update();
    }
    break;
    case QEvent::FontChange:
    {
    }
    break;
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseButtonPress:
    {
        ARibbonBar* _ribbonBar = ribbonBar();
        if (_ribbonBar == nullptr)
            break;
        if (_ribbonBar->isMinimized())
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QWidget* widget = QApplication::widgetAt(mouseEvent->globalPos());
            ARibbonTab* tab = qobject_cast<ARibbonTab*>(widget);
            if (tab != nullptr)
            {
                if (mouseEvent->type() == QEvent::MouseButtonDblClick)
                    QMetaObject::invokeMethod(tab, "dblClicked", Qt::QueuedConnection);
                else
                    QMetaObject::invokeMethod(tab, "clicked", Qt::QueuedConnection);
                return true;
            }
        }
    }
    break;
    default:
        break;
    }
    return QWidget::event(event);
}

/* ARibbonGroupPopup */
ARibbonGroupPopup::ARibbonGroupPopup(ARibbonGroup* group)
    : QWidget(group, Qt::Popup), m_group(group)
{
    Q_ASSERT(m_group != nullptr);
    setFocusPolicy(Qt::NoFocus);
}

ARibbonGroupPopup::~ARibbonGroupPopup()
{
}

void ARibbonGroupPopup::popup(const QPoint& pos)
{
    QFont font = m_group->font();
    QPalette palette = m_group->palette();
    QFont_setResolveMask(font, QFont::AllPropertiesResolved);
    QPalette_setResolveMask(palette, 0xFFFFFF);
    setFont(font);
    setPalette(palette);

    QRect rect = QRect(pos, sizeHint());
    QPoint newpos = rect.topLeft();
    QRect screen = aproch_availableGeometry(this);

    if (newpos.y() + rect.height() > screen.height())
        newpos.setY(screen.height() - rect.height());
    if (newpos.y() < 0)
        newpos.setY(0);

    const int desktopFrame = style()->pixelMetric(QStyle::PM_MenuDesktopFrameWidth, nullptr, m_group);
    if (newpos.x() + rect.width() - 1 > screen.right() - desktopFrame)
        newpos.setX(screen.right() - desktopFrame - rect.width() + 1);
    if (newpos.x() < screen.left() + desktopFrame)
        newpos.setX(screen.left() + desktopFrame);
    rect = QRect(newpos, rect.size());
    setGeometry(rect);
    show();
}

/*! \reimp */
QSize ARibbonGroupPopup::sizeHint() const
{
    QSize size = ARibbonGroupPrivate::_get(m_group)->sizeHint();
    return size;
}

/*! \reimp */
void ARibbonGroupPopup::hideEvent(QHideEvent* event)
{
    event->ignore();

    QRect rect = m_group->rect();
    rect.moveTopLeft(m_group->mapToGlobal(rect.topLeft()));
    m_group->setAttribute(Qt::WA_UnderMouse, rect.contains(QCursor::pos()));
    m_group->update();
}

/*! \reimp */
void ARibbonGroupPopup::paintEvent(QPaintEvent* event)
{
    event->accept();

    QPainter p(this);
    ARibbonStyleOption styleOption;
    styleOption.initFrom(this);
    style()->drawPrimitive((QStyle::PrimitiveElement)ACommonStyle::PE_RibbonFrameGroups, &styleOption, &p, this);

    ARibbonGroupStyleOption groupStyleOption;
    ARibbonGroupPrivate::_get(m_group)->initStyleOption(groupStyleOption);
    groupStyleOption.initFrom(this); //Init it again to change the base widget's options.
    style()->drawPrimitive((QStyle::PrimitiveElement)ACommonStyle::PE_RibbonFrameGroup, &groupStyleOption, &p, this);

    int flags = groupStyleOption.textAlignment;
    flags |= Qt::TextHideMnemonic;
    QRect captionRect = groupStyleOption.rect;
    captionRect.setTop(captionRect.bottom() - groupStyleOption.heightCaption);

    int indent = 0;
    if (m_group->isOptionButtonVisible())
        indent = ARibbonGroupPrivate::_get(m_group)->m_optionButton->width();

    captionRect.adjust(0, -1, -indent, -2);
    groupStyleOption.text = p.fontMetrics().elidedText(groupStyleOption.text, m_group->titleElideMode(), captionRect.adjusted(2, 0, -2, 0).width());
    style()->drawItemText(&p, captionRect, flags, groupStyleOption.palette,
                          groupStyleOption.state & QStyle::State_Enabled, groupStyleOption.text, QPalette::WindowText);
}

/*! \reimp */
void ARibbonGroupPopup::mousePressEvent(QMouseEvent* event)
{
    if (!rect().contains(event->pos()))
    {
        event->accept();
        QRect rect = m_group->rect();
        rect.moveTopLeft(m_group->mapToGlobal(rect.topLeft()));
        if (rect.contains(event->globalPos()))
            setAttribute(Qt::WA_NoMouseReplay);
        close();
        return;
    }
    QWidget::mousePressEvent(event);
}

/* ARibbonGroupOption */
ARibbonGroupOption::ARibbonGroupOption(QWidget* parent)
    : QToolButton(parent)
{
}

ARibbonGroupOption::~ARibbonGroupOption()
{
}

QString ARibbonGroupOption::text() const
{
    return QString();
}

/*! \reimp */
QSize ARibbonGroupOption::sizeHint() const
{
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    const int iconSize = style()->pixelMetric(static_cast<QStyle::PixelMetric>(ACommonStyle::PM_RibbonOptionButtonIconSize), nullptr, this);
    opt.iconSize = QSize(iconSize, iconSize);
    QSize sizeHint = style()->sizeFromContents(QStyle::CT_ToolButton, &opt, opt.iconSize, this);
    return sizeHint;
}

void ARibbonGroupOption::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    const int iconSize = style()->pixelMetric(static_cast<QStyle::PixelMetric>(ACommonStyle::PM_RibbonOptionButtonIconSize), nullptr, this);
    opt.iconSize = QSize(iconSize, iconSize);
    style()->drawPrimitive((QStyle::PrimitiveElement)ACommonStyle::PE_RibbonOptionButton, &opt, &p, this);
}

/*! \reimp */
void ARibbonGroupOption::actionEvent(QActionEvent* event)
{
    QToolButton::actionEvent(event);
    if (event->type() == QEvent::ActionChanged)
    {
        QAction* action = event->action();
        setPopupMode(action->menu() ? QToolButton::MenuButtonPopup : QToolButton::DelayedPopup);
    }
}


/*!
\class ARibbonGroupScrollButton
\internal
*/
ARibbonGroupScrollButton::ARibbonGroupScrollButton(QWidget* parent, bool scrollLeft)
    : QToolButton(parent)
    , m_scrollLeft(scrollLeft)
{
}

/*! \internal */
ARibbonGroupScrollButton::~ARibbonGroupScrollButton()
{
}

/*! \reimp */
void ARibbonGroupScrollButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    opt.arrowType = m_scrollLeft ? Qt::LeftArrow : Qt::RightArrow;
    style()->drawPrimitive((QStyle::PrimitiveElement)ACommonStyle::PE_RibbonGroupScrollButton, &opt, &p, this);
}

/*! \reimp */
void ARibbonGroupScrollButton::leaveEvent(QEvent* event)
{
    QToolButton::leaveEvent(event);
    emit stopScrollTimer();
}

/* ARibbonOverflowMenuButton */
ARibbonOverflowMenuButton::ARibbonOverflowMenuButton(QWidget* parent)
    : ARibbonButton(parent)
{
}

void ARibbonOverflowMenuButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    opt.features = QStyleOptionToolButton::None;
    int iconSize = qRound(qMin(opt.rect.width(), opt.rect.height()) / 2.5);
    ushort symbolIconCode = ASegoeMDL2Font::code(ASegoeMDL2Font::More);
    opt.icon = ASegoeMDL2Font::pixmap(symbolIconCode, QSize(iconSize, iconSize), palette().color(foregroundRole()));
    style()->drawComplexControl((QStyle::ComplexControl)ACommonStyle::CC_RibbonButton, &opt, &painter, this);
}

void ARibbonOverflowMenuButton::mousePressEvent(QMouseEvent* event)
{
    event->ignore();
    if (event->button() != Qt::LeftButton)
        return;

    ARibbonOverflowMenu* overflowMenu = qobject_cast<ARibbonOverflowMenu*>(this->menu());
    if (overflowMenu != nullptr)
        overflowMenu->fillMenu();
    QToolButton::mousePressEvent(event);
    if (overflowMenu != nullptr)
        overflowMenu->clear();
    event->accept();
}

/* ARibbonOverflowAction */
ARibbonOverflowAction::ARibbonOverflowAction(QWidget* control, QWidget* parent)
    : QWidgetAction(parent), m_control(control)
{
    Q_ASSERT(control != nullptr);
}

ARibbonOverflowAction::~ARibbonOverflowAction()
{
}

QWidget* ARibbonOverflowAction::createWidget(QWidget* parent)
{
    m_control->setParent(parent);
    return m_control;
}

void ARibbonOverflowAction::deleteWidget(QWidget* widget)
{
    if (widget == m_control)
        m_control->setParent(nullptr);
}

/* ARibbonOverflowMenu */
ARibbonOverflowMenu::ARibbonOverflowMenu(QWidget* parent)
    : APopupMenu(parent)
{
}

ARibbonOverflowMenu::~ARibbonOverflowMenu()
{
}

void ARibbonOverflowMenu::clearGroups()
{
    m_groups.clear();
}

void ARibbonOverflowMenu::addGroup(ARibbonGroup* group)
{
    Q_ASSERT(!m_groups.contains(group));
    m_groups.append(group);
}

void ARibbonOverflowMenu::fillMenu()
{
    clear();
    for (QList<ARibbonGroup*>::const_iterator it = m_groups.constBegin(); it != m_groups.constEnd(); ++it)
    {
        ARibbonGroup* group = *it;

        int visibleControls = 0;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        QAction* action = addSection(group->title());
#else
        QAction* action = addSeparator();
        action->setText(group->title());
#endif
        QAction* actionSection = action;
        for (int i = 0, count = group->controlCount(); i < count; ++i)
        {
            ARibbonControl* control = group->controlByIndex(i);

            ARibbonToolBarControl* toolBarControl = qobject_cast<ARibbonToolBarControl*>(control);
            bool proceed = toolBarControl && control->currentSize() == ARibbonControlSizeDefinition::GroupSimplified;
            if (!proceed && !control->sizeDefinition(ARibbonControlSizeDefinition::GroupSimplified)->isVisible())
                proceed = control->sizeDefinition(ARibbonControlSizeDefinition::GroupSimplifiedOverflow)->isVisible();

            if (!proceed && control->currentSize() != ARibbonControlSizeDefinition::GroupSimplifiedOverflow)
                continue;

            ARibbonControlSizeDefinition* sizeDefinition = control->sizeDefinition(ARibbonControlSizeDefinition::GroupSimplifiedOverflow);
            if (!sizeDefinition->isVisible())
                continue;

            if (control->defaultAction() != nullptr && control->defaultAction()->isSeparator())
            {
                action = new QAction(this);
                action->setSeparator(true);
            }
            else if (toolBarControl != nullptr)
            {
                if (!toolBarControl->isOverflowMenuIsEmpty())
                {
                    QAction* subMenuAction = new QAction(action->text(), this);
                    subMenuAction->setMenu(toolBarControl->overflowMenu());
                    action = subMenuAction;
                }
            }
            else if (ARibbonButtonControl* button = qobject_cast<ARibbonButtonControl*>(control))
            {
                Q_UNUSED(button);
                action = control->defaultAction();
            }
            else
            {
                action = new ARibbonOverflowAction(control, this);
                action->setEnabled(false);
                control->setEnabled(sizeDefinition->isEnabled());
            }
            addAction(action);
            visibleControls++;
        }
        if (visibleControls == 0)
            removeAction(actionSection);
    }
}

bool ARibbonOverflowMenu::isEmpty() const
{
    return m_groups.count() == 0;
}

void ARibbonOverflowMenu::paintMenuItem(QPainter* painter, QStyleOptionMenuItem* option)
{
    APopupMenu::paintMenuItem(painter, option);
}

APROCH_NAMESPACE_END
