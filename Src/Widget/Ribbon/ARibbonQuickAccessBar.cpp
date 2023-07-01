/****************************************************************************
 * @file    ARibbonQuickAccessBar.cpp
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
#include "ARibbonQuickAccessBar.h"
#include "ARibbonQuickAccessBar_p.h"
#include <QApplication>
#include <QStyleOption>
#include <QMenu>
#include <QPainter>
#include <QToolButton>
#include <QLayout>
#include <qevent.h>

#include "Widget/Style/ACommonStyle.h"
#include "ARibbonBar_p.h"
#include "Widget/Style/AStyleHelper.h"
#include "Widget/Style/ACommonStyle_p.h"
#include "ARibbonDef.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

/* ARibbonQuickAccessButton */
ARibbonQuickAccessButton::ARibbonQuickAccessButton(QWidget* parent)
    : QToolButton(parent)
{
    setObjectName(QStringLiteral("RibbonQuickAccessButton"));
}

ARibbonQuickAccessButton::~ARibbonQuickAccessButton()
{
}

QSize ARibbonQuickAccessButton::sizeHint() const
{
    QSize sz = QToolButton::sizeHint();
    return QSize(sz.width() / 2, sz.height());
}

void ARibbonQuickAccessButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter p(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    style()->drawPrimitive((QStyle::PrimitiveElement)ACommonStyle::PE_RibbonQuickAccessButton, &opt, &p, this);
}

/* ARibbonQuickAccessBarPrivate */
ARibbonQuickAccessBarPrivate::ARibbonQuickAccessBarPrivate()
    : m_menu(nullptr)
    , m_actionAccessPopup(nullptr)
    , m_customizeGroupAction(nullptr)
    , m_accessPopup(nullptr)
    , m_ribbonToolTipManager(nullptr)
    , m_removeAction(false)
    , m_customizeAction(false)
{
}

ARibbonQuickAccessBarPrivate::~ARibbonQuickAccessBarPrivate()
{
}

void ARibbonQuickAccessBarPrivate::init()
{
    A_P(ARibbonQuickAccessBar);
    p.setAutoFillBackground(false);
    p.setAttribute(Qt::WA_NoSystemBackground);
    p.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    static int sz = p.style()->pixelMetric(QStyle::PM_SmallIconSize, nullptr, &p);
    p.setIconSize(QSize(sz, sz));

    m_accessPopup = new ARibbonQuickAccessButton(&p);
    m_accessPopup->setPopupMode(QToolButton::InstantPopup);
    m_menu = new QMenu(&p);
    m_accessPopup->setMenu(m_menu);

    m_actionAccessPopup = p.addWidget(m_accessPopup);
    m_actionAccessPopup->setProperty(__aproch_Quick_Access_Button, QLatin1String(__aproch_Quick_Access_Button));
    m_actionAccessPopup->setToolTip(tr("Customize Quick Access Bar"));

    m_customizeGroupAction = new QActionGroup(&p);
    m_customizeGroupAction->setExclusive(false);
    QObject::connect(m_customizeGroupAction, SIGNAL(triggered(QAction*)), &p, SLOT(customizeAction(QAction*)));

    QObject::connect(m_menu, SIGNAL(aboutToShow()), &p, SLOT(aboutToShowCustomizeMenu()));
    QObject::connect(m_menu, SIGNAL(aboutToHide()), &p, SLOT(aboutToHideCustomizeMenu()));

    m_ribbonToolTipManager = new RibbonToolTipManager(&p);

    p.addAction(new AQuickAccessActionInvisible(&p, m_customizeGroupAction));
}

ARibbonBar* ARibbonQuickAccessBarPrivate::ribbonBar() const
{
    A_P(const ARibbonQuickAccessBar);
    return ARibbonBarPrivate::findRibbonBar(&p);
}

AQuickAccessAction* ARibbonQuickAccessBarPrivate::findQuickAccessAction(QAction* action) const
{
    if (m_customizeGroupAction == nullptr)
        return nullptr;

    QList<QAction*> list = m_customizeGroupAction->actions();
    for (int i = 0; i < list.count(); ++i)
    {
        AQuickAccessAction* act = qobject_cast<AQuickAccessAction*>(list[i]);
        if (act && action == act->m_srcAction)
            return act;
    }
    return nullptr;
}

QAction* ARibbonQuickAccessBarPrivate::findBeforeAction(QAction* action) const
{
    QList<QAction*> list = m_customizeGroupAction->actions();
    bool find = false;
    for (int i = 0, count = list.count(); i < count; ++i)
    {
        if (find)
        {
            if (AQuickAccessAction* beforeAct = qobject_cast<AQuickAccessAction*>(list[i]))
            {
                if (beforeAct->isChecked())
                    return beforeAct->m_srcAction;
            }
        }
        if (!find && action == list[i])
            find = true;
    }
    return nullptr;
}

void ARibbonQuickAccessBarPrivate::updateAction(QAction* action)
{
    if (AQuickAccessAction* wrapper = findQuickAccessAction(action))
        wrapper->update();
}

void ARibbonQuickAccessBarPrivate::setActionVisible(QAction* action, bool visible)
{
    A_P(ARibbonQuickAccessBar);
    if (AQuickAccessAction* wrapper = findQuickAccessAction(action))
    {
        if (visible)
        {
            if (m_customizeAction)
            {
                QAction* beforeAct = findBeforeAction(wrapper);
                p.insertAction(beforeAct, action);
            }
            else
                p.addAction(action);
        }
        else
        {
            p.removeAction(action);
            m_removeAction = false;
        }
        wrapper->update();
        p.adjustSize();
    }
}


/*!
\class ARibbonQuickAccessBar
\inmodule QtitanRibbon
\brief ARibbonQuickAccessBar class quick access ToolBar, which is located on ARibbonBar.
Quick access toolbar contains a set of controls that are always available to users regardless of which page is selected.
*/

/*!
Constructs ARibbonQuickAccessBar object with the given \a parent.
*/
ARibbonQuickAccessBar::ARibbonQuickAccessBar(QWidget* parent)
    : QToolBar(parent)
{
    A_INIT_PRIVATE(ARibbonQuickAccessBar);
    A_D(ARibbonQuickAccessBar);
    d.init();
}

/*!
Destructor of the ARibbonQuickAccessBar object.
*/
ARibbonQuickAccessBar::~ARibbonQuickAccessBar()
{
    A_FINI_PRIVATE();
}

/*!
\brief Returns a pointer to the QAction that is associated with the customize button of Ribbon's Quick Access Bar.
*/
QAction* ARibbonQuickAccessBar::actionCustomizeButton() const
{
    A_D(const ARibbonQuickAccessBar);
    return d.m_actionAccessPopup;
}

/*!
\brief Sets the visibility of the visual representation of action on the quick access toolbar. Parameter \a action is action, parameter \a visible is a visibility.
*/
void ARibbonQuickAccessBar::setActionVisible(QAction* action, bool visible)
{
    A_D(ARibbonQuickAccessBar);
    d.setActionVisible(action, visible);
}

/*!
\brief Returns the visibility of the visual representation for the given \a action.
*/
bool ARibbonQuickAccessBar::isActionVisible(QAction* action) const
{
    A_D(const ARibbonQuickAccessBar);
    if (AQuickAccessAction* wrapper = d.findQuickAccessAction(action))
        return wrapper->isChecked();
    return false;
}

/*!
\brief Returns the count of the elements visible on Ribbon's Quick Access Bar.
*/
int ARibbonQuickAccessBar::visibleCount() const
{
    A_D(const ARibbonQuickAccessBar);
    int visibleCount = 0;
    QList<QAction*> list = d.m_customizeGroupAction->actions();
    for (int i = 0, count = list.count(); i < count; ++i)
    {
        if (AQuickAccessAction* beforeAct = qobject_cast<AQuickAccessAction*>(list[i]))
            if (beforeAct->isChecked())
                visibleCount++;
    }
    return visibleCount;
}

/*!
\brief Returns the recommended size for Ribbon's Quick Access Bar.
\reimp
*/
QSize ARibbonQuickAccessBar::sizeHint() const
{
#if 0
    A_D(const ARibbonQuickAccessBar);
    ARibbonQuickAccessBarStyleOption opt;
    d.initStyleOption(&opt);
    ARibbonBar* ribbonBar = d.ribbonBar();
    if (ribbonBar != nullptr && opt.quickAccessBarPosition == ARibbonBar::QABottomPosition)
        return QSize(QToolBar::sizeHint().width(), ARibbonBarPrivate::_get(ribbonBar)->heightTabs() + 1);
#endif
    return QToolBar::sizeHint();
}

/*! \internal */
void ARibbonQuickAccessBar::customizeAction(QAction* action)
{
    A_D(ARibbonQuickAccessBar);
    d.m_customizeAction = true;
    if (AQuickAccessAction* act = qobject_cast<AQuickAccessAction*>(action))
    {
        setActionVisible(act->m_srcAction, !widgetForAction(act->m_srcAction));
        if (ARibbonBar* ribbonBar = d.ribbonBar())
        {
            RibbonCustomizeManager* manager = ribbonBar->customizeManager();
            manager->setEditMode();
            manager->aproch_d().m_customizeEngine->setQuickAccessBar();
            manager->setEditMode(false);
            ribbonBar->updateLayout();
        }
    }
    d.m_customizeAction = false;
}

/*! \internal */
void ARibbonQuickAccessBar::aboutToShowCustomizeMenu()
{
    A_D(ARibbonQuickAccessBar);
    d.m_menu->clear();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    if (style()->styleHint(QStyle::SH_Menu_SupportsSections))
    {
        d.m_menu->setSeparatorsCollapsible(false);
        d.m_menu->addSection(ARibbonBar::tr_compatible(ARibbonCustomizeQuickAccessToolBarString));
    }
#else
    d.m_menu->setSeparatorsCollapsible(false);
    d.m_menu->addSeparator()->setText(ARibbonBar::tr_compatible(ARibbonCustomizeQuickAccessToolBarString));
#endif
    QListIterator<QAction*> itAction(d.m_actionList);
    while (itAction.hasNext())
    {
        QAction* action = itAction.next();
        d.m_menu->addAction(action);
    }
    emit showCustomizeMenu(d.m_menu);
}

/*! \internal */
void ARibbonQuickAccessBar::aboutToHideCustomizeMenu()
{
    A_D(ARibbonQuickAccessBar);
    d.m_menu->clear();
}

/*! \reimp */
bool ARibbonQuickAccessBar::event(QEvent* event)
{
    if (parentWidget() && (event->type() == QEvent::Hide || event->type() == QEvent::Show))
    {
        adjustSize();
    }
    else if (event->type() == QEvent::StyleChange)
    {
        const int sz = style()->pixelMetric(QStyle::PM_SmallIconSize, nullptr, this);
        setIconSize(QSize(sz, sz));
    }
    return QToolBar::event(event);
}

/*! \reimp */
void ARibbonQuickAccessBar::actionEvent(QActionEvent* event)
{
    A_D(ARibbonQuickAccessBar);
    QToolBar::actionEvent(event);
    if (d.m_actionAccessPopup)
    {
        if (event->type() == QEvent::ActionAdded)
        {
            if (!d.m_removeAction)
                removeAction(d.m_actionAccessPopup);

            AQuickAccessAction* quickAccessAction = d.findQuickAccessAction(event->action());
            if (event->action() != d.m_actionAccessPopup && !quickAccessAction &&
                !qobject_cast<AQuickAccessActionInvisible*>(event->action()))
            {
                AQuickAccessAction* act = new AQuickAccessAction(this, event->action());

                bool addActionToMenu = true;
                if (ARibbonBar* ribbonBar = d.ribbonBar())
                {
                    RibbonCustomizeManager* manager = ribbonBar->customizeManager();
                    if (manager->isEditMode())
                    {
                        QList< QAction* > defaultActions = manager->aproch_d().m_customizeEngine->defaultToolBars().value(this);
                        if (!defaultActions.contains(event->action()))
                            addActionToMenu = false;
                    }
                }

                if (addActionToMenu)
                    d.m_actionList.append(act);

                d.m_customizeGroupAction->addAction(act);
            }
            else if (quickAccessAction)
                quickAccessAction->update();
        }
        else if (event->type() == QEvent::ActionRemoved)
        {
            if (event->action() == d.m_actionAccessPopup)
            {
                d.m_removeAction = true;
                addAction(d.m_actionAccessPopup);
                d.m_removeAction = false;
            }
            else if (AQuickAccessActionInvisible* actInvisible = qobject_cast<AQuickAccessActionInvisible*>(event->action()))
            {
                d.m_actionList.clear();
                QList<QAction*> actList = actInvisible->m_data->actions();
                for (int i = actList.size() - 1; i >= 0; i--)
                {
                    QAction* actionWrapper = actList[i];
                    d.m_customizeGroupAction->removeAction(actionWrapper);
                    delete actionWrapper;
                }
                delete actInvisible;
                addAction(new AQuickAccessActionInvisible(this, d.m_customizeGroupAction));
            }
            else
                d.updateAction(event->action());
        }
        else if (event->type() == QEvent::ActionChanged)
        {
            if (event->action() == d.m_actionAccessPopup)
                d.m_accessPopup->setDefaultAction(d.m_actionAccessPopup);
            else if (!qobject_cast<AQuickAccessAction*>(event->action()))
                d.updateAction(event->action());
        }
    }

    if (layout() != nullptr)
        layout()->invalidate();
}

void ARibbonQuickAccessBar::paintContent(QPainter* painter)
{
    A_D(ARibbonQuickAccessBar);
    ARibbonQuickAccessBarStyleOption opt;
    opt.initFrom(this);
    const int hor = ACommonStylePrivate::dpiScaled(2, this);
    const int ver = ACommonStylePrivate::dpiScaled(1, this);
    opt.rect = rect().adjusted(-hor, -ver, hor, ver);
    opt.quickAccessBarPosition = ARibbonBar::TopPosition;
    if (ARibbonBar* ribbonBar = d.ribbonBar())
    {
        opt.quickAccessBarPosition = ribbonBar->quickAccessBarPosition();
        ARibbonSystemButton* systemButton = ribbonBar->systemButton();
        opt.roundButton = systemButton != nullptr && systemButton->toolButtonStyle() == Qt::ToolButtonIconOnly;
    }
    style()->drawControl(QStyle::CE_ToolBar, &opt, painter, this);
}

/*! \reimp */
void ARibbonQuickAccessBar::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    A_D(ARibbonQuickAccessBar);
    if (d.ribbonBar() != nullptr)
        return;
    QPainter p(this);
    paintContent(&p);
}

/*! \reimp */
void ARibbonQuickAccessBar::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
}

APROCH_NAMESPACE_END