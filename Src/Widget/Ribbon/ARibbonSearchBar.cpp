/****************************************************************************
 * @file    ARibbonSearchBar.cpp
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
#include <QPainter>
#include <QApplication>
#include <QStyleOptionFrame>
#include <QLayout>

#include "ARibbonSearchBar.h"
#include "ARibbonSearchBar_p.h"
#include "Platform/APlatform.h"
#include "ARibbonQuickAccessBar_p.h"
#include "Widget/Style/ASegoeMDL2Assets.h"
#include "Widget/Style/ACommonStyle.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

 /* ARibbonSearchBarPrivate */
ARibbonSearchBarPrivate::ARibbonSearchBarPrivate()
    : QObject(), m_ribbonBar(nullptr), m_popup(nullptr), m_compact(false),
    m_showPopupNeeded(false), m_helpEnabled(false), m_maxSearchItemCount(40)
{
}

ARibbonSearchBarPrivate::~ARibbonSearchBarPrivate()
{
    A_DELETE_AND_NULL(m_actionsSep);
    A_DELETE_AND_NULL(m_helpSep);
    A_DELETE_AND_NULL(m_helpAction);
    A_DELETE_AND_NULL(m_suggestedSep);
    A_DELETE_AND_NULL(m_popup);
    A_DELETE_AND_NULL(m_animation);
}

void ARibbonSearchBarPrivate::showPopup(const QString& text)
{
    A_P(ARibbonSearchBar);
    m_showPopupNeeded = false;
    if (m_popup == nullptr)
    {
        m_popup = new ARibbonSearchBarPopup(&p);
        m_popup->addAction(m_actionsSep);
    }

    if (m_popup->isHidden())
    {
        buildIndex();
        m_popup->setFocusProxy(nullptr);
        m_popup->setFocusPolicy(Qt::NoFocus);
        m_popup->setFocusProxy(&p);
    }

    m_popup->setUpdateActions(true);
    const QList<QAction*> actions = m_popup->actions();
    const QList<QAction*> newActions = findActions(text);
    const QList<QAction*> suggestedActions = m_suggestedActions;

    for (QList<QAction*>::const_iterator it = actions.constBegin(); it != actions.constEnd(); ++it)
    {
        QAction* action = *it;
        if (newActions.contains(action))
            continue;
        if (action == m_actionsSep)
            continue;
        if (!text.isEmpty() && (action == m_helpSep || action == m_helpAction))
            continue;
        if (text.isEmpty() && (action == m_suggestedSep || suggestedActions.contains(action)))
            continue;
        m_popup->removeAction(action);
    }

    for (QList<QAction*>::const_iterator it = newActions.constBegin(); it != newActions.constEnd(); ++it)
    {
        QAction* action = *it;
        if (actions.contains(action))
            continue;
        m_popup->insertAction(m_helpSep, action);
    }

    bool hasHelpActions = p.isHelpEnabled() && !text.isEmpty();
    if (hasHelpActions)
    {
        if (!actions.contains(m_helpSep))
        {
            m_popup->addAction(m_helpSep);
            m_popup->addAction(m_helpAction);
            m_helpSep->setEnabled(true);
            m_helpSep->setVisible(true);
            m_helpAction->setEnabled(true);
            m_helpAction->setVisible(true);
        }

        QString s = QString(QLatin1String("%1 \"%2\"")).arg(ARibbonBar::tr_compatible(RibbonSearchBarGetHelpString)).arg(text);
        m_helpAction->setText(s);
        m_helpText = text;
    }
    else
    {
        m_popup->removeAction(m_helpSep);
        m_popup->removeAction(m_helpAction);
        m_helpText.clear();
    }

    if (text.isEmpty())
    {
        m_popup->addAction(m_suggestedSep);
        for (QList<QAction*>::const_iterator it = suggestedActions.constBegin(); it != suggestedActions.constEnd(); ++it)
        {
            QAction* action = *it;
            m_popup->addAction(action);
        }
    }

    m_popup->setUpdateActions(false);

    if (!hasHelpActions && newActions.isEmpty() && suggestedActions.isEmpty())
    {
        m_popup->hide();
        return;
    }

    m_popup->setMinimumWidth(p.width());
    QSize sh = m_popup->sizeHint();

    QPoint gpos = p.mapToGlobal(QPoint(0, 0));
    const QRect screen = aproch_screenGeometry(gpos, &p);
    QRect rect = QRect(gpos.x(), gpos.y() + p.height(), qMax(sh.width(), p.width()), m_popup->sizeHint().height());
    rect.setBottom(qMin(rect.bottom(), screen.bottom()));
    m_popup->setGeometry(rect);

    if (m_popup->isHidden())
    {
#ifndef QT_NO_EFFECTS
        if (QApplication::isEffectEnabled(Qt::UI_FadeMenu))
            qFadeEffect(m_popup, 200);
        else
#endif
            m_popup->show();
    }
    p.setFocus();
}

static void aproch_addAction(QAction* action, ActionSearchIndex& index)
{
    if (!action->property(__aproch_Quick_Access_Button).isNull())
        return;
    if (!action->property(__aproch_Action_Invisible).isNull())
        return;

    QString text = action->text();
    QString tip = action->toolTip();
    if (!text.isEmpty())
    {
        ActionSearchIndex::iterator sit = index.find(text);
        if (sit == index.end())
            sit = index.insert(text, QList<QAction*>());
        sit.value().append(action);
    }

    if (!tip.isEmpty())
    {
        ActionSearchIndex::iterator sit = index.find(tip);
        if (sit == index.end())
            sit = index.insert(tip, QList<QAction*>());
        sit.value().append(action);
    }
}

static void aproch_collectActions(ARibbonGroup* group, ActionSearchIndex& index)
{
    QList<QAction*> actions = group->actions();
    for (QList<QAction*>::const_iterator it = actions.constBegin(); it != actions.constEnd(); ++it)
        aproch_addAction(*it, index);
}

static void aproch_collectActions(ARibbonPage* page, ActionSearchIndex& index)
{
    for (int i = 0; i < page->groupCount(); ++i)
        aproch_collectActions(page->group(i), index);
}

static void aproch_collectActions(ARibbonBar* bar, ActionSearchIndex& index)
{
    if (ARibbonQuickAccessBar* quickBar = bar->quickAccessBar())
    {
        QList<QAction*> actions = quickBar->actions();
        for (QList<QAction*>::const_iterator it = actions.constBegin(); it != actions.constEnd(); ++it)
            aproch_addAction(*it, index);
    }

    for (int i = 0; i < bar->pageCount(); ++i)
        aproch_collectActions(bar->page(i), index);
}

void ARibbonSearchBarPrivate::buildIndex()
{
    m_searchIndex.clear();
    aproch_collectActions(m_ribbonBar, m_searchIndex);
}

QList<QAction*> ARibbonSearchBarPrivate::findActions(const QString& text)
{
    QList<QAction*> result;
    if (text.isEmpty())
        return result;

#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    QString wildcard = QRegularExpression::wildcardToRegularExpression(QStringLiteral("*%1*").arg(text));
    QRegularExpression rx(wildcard, QRegularExpression::CaseInsensitiveOption);
#else
    QRegExp rx(QStringLiteral("*%1*").arg(text));
    rx.setPatternSyntax(QRegExp::Wildcard);
    rx.setCaseSensitivity(Qt::CaseInsensitive);
#endif
    for (ActionSearchIndex::const_iterator it = m_searchIndex.constBegin(); it != m_searchIndex.constEnd(); ++it)
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
        QRegularExpressionMatch m = rx.match(it.key());
        if (m.hasMatch())
#else
        if (rx.exactMatch(it.key()))
#endif
        {
            const QList<QAction*>& list = it.value();
            for (QList<QAction*>::const_iterator jit = list.constBegin(); jit != list.constEnd(); ++jit)
            {
                if (!result.contains(*jit))
                    result.append(*jit);
            }
            if (m_maxSearchItemCount != -1 && result.count() >= m_maxSearchItemCount)
                break;
        }
    }
    return result;
}

void ARibbonSearchBarPrivate::init(ARibbonBar* ribbonBar)
{
    A_P(ARibbonSearchBar);
    m_ribbonBar = ribbonBar;

    p.setPlaceholderText(ARibbonBar::tr_compatible(RibbonSearchBarSearchString));
    m_icon = ASegoeMDL2Font::icon(ASegoeMDL2Font::Search);
    m_animation = new QVariantAnimation();
    connect(&p, SIGNAL(textEdited(const QString&)), &p, SLOT(showPopup(const QString&)));
    connect(m_animation, SIGNAL(finished()), &p, SLOT(animationFinished()));
    connect(m_animation, SIGNAL(valueChanged(const QVariant&)), &p, SLOT(animationValueChanged(const QVariant&)));

    m_actionsSep = new QAction(&p);
    m_actionsSep->setSeparator(true);
    m_actionsSep->setText(ARibbonBar::tr_compatible(RibbonSearchBarActionsString));

    m_helpSep = new QAction(&p);
    m_helpSep->setSeparator(true);
    m_helpSep->setText(ARibbonBar::tr_compatible(RibbonSearchBarHelpString));
    m_helpAction = new QAction(&p);
    m_helpAction->setIcon(ASegoeMDL2Font::icon(ASegoeMDL2Font::Unknown));
    connect(m_helpAction, SIGNAL(triggered()), &p, SLOT(helpClicked()));

    m_suggestedSep = new QAction(&p);
    m_suggestedSep->setSeparator(true);
    m_suggestedSep->setText(ARibbonBar::tr_compatible(RibbonSearchBarSuggestedActionsString));
}

void ARibbonSearchBarPrivate::updateGeometry(bool animated)
{
    A_P(ARibbonSearchBar);
    QSize sh = p.sizeHint();
    QRect expandRect = m_compact ? QRect(0, 0, sh.height() * 7, sh.height()) : QRect(0, 0, sh.height() * 20, sh.height());
    QRect minRect = QRect(0, 0, sh.height() * 1.5, sh.height());
    if (!m_compact || (m_popup != nullptr && !m_popup->isHidden()))
        minRect = expandRect;

    if (p.hasFocus())
    {
        m_animation->setStartValue(minRect);
        m_animation->setEndValue(expandRect);
        m_animation->setEasingCurve(QEasingCurve::OutExpo);
    }
    else
    {
        m_animation->setStartValue(expandRect);
        m_animation->setEndValue(minRect);
        m_animation->setEasingCurve(QEasingCurve::InExpo);
    }

    if (!animated || minRect == expandRect)
    {
        p.animationFinished();
        return;
    }

    m_animation->setDuration(250);
    m_animation->start();
}

/*!
\class ARibbonSearchBar
\inmodule QtitanRibbon
\brief ARibbonSearchBar class implements a search feature for all actions that are added in the internal structures of the ribbon bar.
*/

/*!
Constructs ARibbonSearchBar object with the given \a ribbonBar.
*/
ARibbonSearchBar::ARibbonSearchBar(ARibbonBar* ribbonBar)
    : QLineEdit()
{
    A_INIT_PRIVATE(ARibbonSearchBar);
    A_D(ARibbonSearchBar);
    d.init(ribbonBar);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setHelpEnabled(true);
}

ARibbonSearchBar::~ARibbonSearchBar()
{
}

/*! \reimp */
QSize ARibbonSearchBar::minimumSizeHint() const
{
    QSize result = QLineEdit::minimumSizeHint();
    result.setWidth(result.height());
    return result;
}

void ARibbonSearchBar::setCompact(bool compact)
{
    A_D(ARibbonSearchBar);
    d.m_compact = compact;
    if (compact)
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    else
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    d.updateGeometry(false);
}

const QIcon& ARibbonSearchBar::icon() const
{
    A_D(const ARibbonSearchBar);
    return d.m_icon;
}

bool ARibbonSearchBar::isHelpEnabled() const
{
    A_D(const ARibbonSearchBar);
    return d.m_helpEnabled;
}

void ARibbonSearchBar::setHelpEnabled(bool enabled)
{
    A_D(ARibbonSearchBar);
    d.m_helpEnabled = enabled;
}

const QList<QAction*>& ARibbonSearchBar::suggestedActions() const
{
    A_D(const ARibbonSearchBar);
    return d.m_suggestedActions;
}

void ARibbonSearchBar::addSuggestedAction(QAction* action)
{
    A_D(ARibbonSearchBar);
    if (d.m_suggestedActions.contains(action))
        return;
    d.m_suggestedActions.append(action);
}

void ARibbonSearchBar::removeSuggestedAction(QAction* action)
{
    A_D(ARibbonSearchBar);
    d.m_suggestedActions.removeAll(action);
}

int ARibbonSearchBar::maxSearchItemCount() const
{
    A_D(const ARibbonSearchBar);
    return d.m_maxSearchItemCount;
}

void ARibbonSearchBar::setMaxSearchItemCount(int count)
{
    A_D(ARibbonSearchBar);
    d.m_maxSearchItemCount = count;
}

void ARibbonSearchBar::animationFinished()
{
    A_D(ARibbonSearchBar);
    if (!hasFocus() && d.m_compact)
        setReadOnly(true); //Hides the mouse cursor.
    else
        setReadOnly(false);

    setMaximumWidth(d.m_animation->endValue().toRect().width());
    if (parentWidget() != nullptr && parentWidget()->layout() != nullptr)
        parentWidget()->layout()->invalidate();

    if (d.m_showPopupNeeded)
        QMetaObject::invokeMethod(this, "showPopup", Qt::QueuedConnection, Q_ARG(QString, QString()));
}

void ARibbonSearchBar::animationValueChanged(const QVariant& value)
{
    setMaximumWidth(value.toRect().width());
    if (parentWidget() != nullptr && parentWidget()->layout() != nullptr)
        parentWidget()->layout()->invalidate();
}

void ARibbonSearchBar::helpClicked()
{
    A_D(const ARibbonSearchBar);
    if (d.m_helpText.isEmpty())
        return;
    emit showHelp(d.m_helpText);
}

void ARibbonSearchBar::showPopup(const QString& text)
{
    A_D(ARibbonSearchBar);
    d.showPopup(text);
}

void ARibbonSearchBar::closePopup()
{
    A_D(ARibbonSearchBar);
    if (d.m_popup != nullptr)
        d.m_popup->hide();
}

/*! \reimp */
void ARibbonSearchBar::mousePressEvent(QMouseEvent* event)
{
    A_D(ARibbonSearchBar);
    QLineEdit::mousePressEvent(event);
    d.m_showPopupNeeded = true;
    if (d.m_animation->state() != QVariantAnimation::Running)
        animationFinished();
}

/*! \reimp */
void ARibbonSearchBar::keyPressEvent(QKeyEvent* event)
{
    A_D(ARibbonSearchBar);
    if (event->key() == Qt::Key_Escape)
    {
        if (d.m_popup != nullptr && !d.m_popup->isHidden())
            d.m_popup->hide();
        else if (d.m_compact)
            focusNextChild();
        event->accept();
        return;
    }
    QLineEdit::keyPressEvent(event);
}

/*! \reimp */
void ARibbonSearchBar::paintEvent(QPaintEvent* event)
{
    A_D(ARibbonSearchBar);
    QColor iconColor;
    QStyleOptionFrame option;
    initStyleOption(&option);
    if (!hasFocus() && d.m_compact && d.m_animation->state() != QVariantAnimation::Running)
    {
        iconColor = option.palette.windowText().color();
    }
    else
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
        iconColor = option.palette.placeholderText().color();
#else
        iconColor = option.palette.windowText().color();
#endif
        QLineEdit::paintEvent(event);
    }

    //Paint search icon.
    QPainter p(this);
    p.setPen(iconColor);
    QRect r = option.rect;
    r.adjust(option.lineWidth, option.lineWidth, -option.lineWidth, -option.lineWidth);
    r = QStyle::alignedRect(option.direction, Qt::Alignment(Qt::AlignRight | Qt::AlignVCenter),
                            QSize(r.height(), r.height()), r);
    r.adjust(option.lineWidth, option.lineWidth, -option.lineWidth, -option.lineWidth);
    icon().paint(&p, r, Qt::AlignCenter, isEnabled() ? QIcon::Normal : QIcon::Disabled);
}

/*! \reimp */
void ARibbonSearchBar::focusInEvent(QFocusEvent* event)
{
    A_D(ARibbonSearchBar);
    QLineEdit::focusInEvent(event);
    d.updateGeometry(true);
}

/*! \reimp */
void ARibbonSearchBar::focusOutEvent(QFocusEvent* event)
{
    A_D(ARibbonSearchBar);
    QLineEdit::focusOutEvent(event);
    d.updateGeometry(true);
}

/*! \reimp */
void ARibbonSearchBar::resizeEvent(QResizeEvent* event)
{
    QLineEdit::resizeEvent(event);
}

/* ARibbonSearchBarPopup */
ARibbonSearchBarPopup::ARibbonSearchBarPopup(ARibbonSearchBar* searchBar)
    : APopupMenu(searchBar), m_searchBar(searchBar), m_updateActions(false)
{
    setWindowFlags(Qt::WindowFlags(Qt::Popup
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#endif
#ifdef Q_OS_LINUX
                   | Qt::BypassWindowManagerHint
#endif
    ));
}

ARibbonSearchBarPopup::~ARibbonSearchBarPopup()
{
}

QSize ARibbonSearchBarPopup::sizeHint() const
{
    if (m_updateActions)
        return size(); //Prevent resizing.
    QSize result = APopupMenu::sizeHint();
    if (minimumWidth() > 0)
        result.rwidth() = qMin(result.width(), minimumWidth() * 3);
    return result;
}

void ARibbonSearchBarPopup::setUpdateActions(bool updateActions)
{
    if (updateActions == m_updateActions)
        return;
    m_updateActions = updateActions;
#if 0
    if (!updateActions && isVisible())
        resize(sizeHint());
#endif
}

bool ARibbonSearchBarPopup::event(QEvent* e)
{
    switch (e->type())
    {
    case QEvent::Hide:
    {
        setFocusProxy(nullptr);
        m_searchBar->setText(QString());
    }
    break;
    case QEvent::KeyPress:
    {
        QKeyEvent* ke = static_cast<QKeyEvent*>(e);
        const int key = ke->key();
        if ((key == Qt::Key_Up || key == Qt::Key_Down))
        {
            return true;
        }

        switch (key)
        {
        case Qt::Key_End:
        case Qt::Key_Home:
            if (ke->modifiers() & Qt::ControlModifier)
                return false;
            break;

        case Qt::Key_Up:
            break;

        case Qt::Key_Down:
            break;

        case Qt::Key_PageUp:
        case Qt::Key_PageDown:
            return false;
        }

        m_searchBar->event(ke);

        if (e->isAccepted() || !isVisible())
        {
            if ((!hasFocus()
#ifdef QT_KEYPAD_NAVIGATION
                || (QApplicationPrivate::keypadNavigationEnabled() && !hasEditFocus())
#endif
                ))
                hide();
            if (e->isAccepted())
                return true;
        }

        if (ke->matches(QKeySequence::Cancel))
        {
            hide();
            return true;
        }

        switch (key)
        {
#ifdef QT_KEYPAD_NAVIGATION
        case Qt::Key_Select:
            if (!QApplicationPrivate::keypadNavigationEnabled())
                break;
#endif
        case Qt::Key_Return:
        case Qt::Key_Enter:
        case Qt::Key_Tab:
            hide();
            break;

        case Qt::Key_F4:
            if (ke->modifiers() & Qt::AltModifier)
                hide();
            break;

        case Qt::Key_Backtab:
            hide();
            break;

        default:
            break;
        }

        return true;
    }
    default:
        break;
    }
    return APopupMenu::event(e);
}

void ARibbonSearchBarPopup::paintEvent(QPaintEvent* e)
{
    APopupMenu::paintEvent(e);
}

APROCH_NAMESPACE_END