/****************************************************************************
 * @file    ARibbonBackstageView.cpp
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
#include <QStyleOption>
#include <QWidgetAction>
#include <QPainter>
#include <QStatusBar>
#include <QBitmap>
#include <QLayout>

#include <QTextDocument>

#include <QTextFrameFormat>
#include <QAbstractTextDocumentLayout>
#include <QTextFrame>
#include <QMainWindow>

#include <qevent.h>

#include "Widget/Style/AStyleHelper.h"
#include "Widget/Style/ACommonStyle.h"
#include "Widget/Style/ACommonStyle_p.h"
#include "Widget/Style/AStyleOption.h"
#include "ARibbonBackstageView.h"
#include "ARibbonBackstageView_p.h"
#include "ARibbonBar.h"
#include "ARibbonBar_p.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

const int wheelDelta = 120; // Default value for rolling one notch
const int commandGap = 6;
const int textGap = 15;

static ARibbonBar* aproch_findRibbonBar(const ARibbonBackstageView* view)
{
    if (view == nullptr || qobject_cast<QMainWindow*>(view->parentWidget()) == nullptr)
        return nullptr;
    ARibbonBar* ribbonBar = qobject_cast<ARibbonBar*>(static_cast<QMainWindow*>(view->parentWidget())->menuWidget());
    if (ribbonBar != nullptr && ribbonBar->systemButton()->backstage() == view)
        return ribbonBar;
    return nullptr;
}

static int aproch_backstageViewTop(const ARibbonBackstageView* view)
{
    ARibbonBar* ribbonBar = aproch_findRibbonBar(view);
    if (ribbonBar != nullptr && ribbonBar->isVisible())
        return ARibbonBarPrivate::_get(ribbonBar)->backstageViewTop();
    return 0;
}

static void aproch_setBackstageViewVisible(const ARibbonBackstageView* view, bool visible)
{
    ARibbonBar* ribbonBar = aproch_findRibbonBar(view);
    if (ribbonBar != nullptr)
        ARibbonBarPrivate::_get(ribbonBar)->setBackstageViewVisible(visible);
}

/* ARibbonBackstageCloseButton */
ARibbonBackstageCloseButton::ARibbonBackstageCloseButton(QWidget* parent)
    : QToolButton(parent)
{
}

/*! \reimp */
QSize ARibbonBackstageCloseButton::sizeHint() const
{
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    QList<QSize> sz = opt.icon.availableSizes();
    Q_ASSERT(sz.size() > 0);
    QSize size = sz[0];
    size = QSize(static_cast<int>(ACommonStylePrivate::dpiScaled(static_cast<qreal>(size.height()), this)),
                 static_cast<int>(ACommonStylePrivate::dpiScaled(static_cast<qreal>(size.width()), this)));
    return size;
}

/*! \reimp */
void ARibbonBackstageCloseButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter p(this);

    QStyleOptionToolButton opt;
    initStyleOption(&opt);

    QList<QSize> sz = opt.icon.availableSizes();
    opt.iconSize = sz.at(0);
    opt.iconSize = QSize(static_cast<int>(ACommonStylePrivate::dpiScaled(static_cast<qreal>(opt.iconSize.height()), this)),
                         static_cast<int>(ACommonStylePrivate::dpiScaled(static_cast<qreal>(opt.iconSize.width()), this)));
    style()->drawPrimitive(static_cast<QStyle::PrimitiveElement>(ACommonStyle::PE_RibbonBackstageCloseButton), &opt, &p, this);
}

/*! \reimp */
void ARibbonBackstageCloseButton::mousePressEvent(QMouseEvent* event)
{
    if (testAttribute(Qt::WA_UnderMouse))
        QToolButton::mousePressEvent(event);
}


/* ARibbonBackstageViewMenu */
ARibbonBackstageViewMenu::ARibbonBackstageViewMenu(ARibbonBackstageView* backstageView)
    : QWidget(backstageView)
    , m_backstageView(backstageView)
    , m_backStageCloseButton(nullptr)
    , m_totalHeight(0)
    , m_offsetScroll(0)
    , m_mouseDown(false)
    , m_itemsDirty(true)
    , m_menuWidth(132) 
    , m_maxIconWidth(0)
{
    setMouseTracking(true);
}

QSize ARibbonBackstageViewMenu::sizeHint() const
{
    m_itemsDirty = true;
    updateActionRects();
    return QSize(m_menuWidth, m_backstageView->sizeHint().width());
}

void ARibbonBackstageViewMenu::createBackstageCloseButton()
{
    if (static_cast<bool>(style()->styleHint(static_cast<QStyle::StyleHint>(ACommonStyle::SH_RibbonBackstageHideTabs))))
    {
        if (m_backStageCloseButton == nullptr)
        {
            m_backStageCloseButton = new ARibbonBackstageCloseButton(this);
            QAction* actBackStageCloseButton = new QAction(this);
            actBackStageCloseButton->setIcon(QIcon(QStringLiteral(":/res/ribbonbackstageback_icon.png")));
            m_backStageCloseButton->setDefaultAction(actBackStageCloseButton);
            QSize sz = m_backStageCloseButton->sizeHint();
            m_backStageCloseButton->setGeometry(QRect(QPoint(sz.width() / 2, sz.height()), sz));
            m_backStageCloseButton->setVisible(true);
            QObject::connect(m_backStageCloseButton, SIGNAL(pressed()), m_backstageView, SLOT(close()));
        }
    }
    else
        A_DELETE_AND_NULL(m_backStageCloseButton);

    resetItemsDirty();
}

QAction* ARibbonBackstageViewMenu::actionAt(const QPoint& pt) const
{
    QList<QAction*> actions = m_backstageView->actions();
    for(int i = 0; i < m_actionRects.count(); ++i) 
    {
        if (m_actionRects.at(i).contains(pt))
            return actions.at(i);
    }
    return nullptr;
}

QRect ARibbonBackstageViewMenu::actionRect(QAction* act) const
{
    QList<QAction*> actions = m_backstageView->actions();
    int index = actions.indexOf(act);
    if (index == -1)
        return QRect();

    updateActionRects();
    // we found the action
    return m_actionRects.at(index);
}

void ARibbonBackstageViewMenu::resetItemsDirty()
{
    m_itemsDirty = true;
    update();
}

void ARibbonBackstageViewMenu::setOffsetScroll(int offset)
{
    if (m_offsetScroll != offset)
    {
        m_offsetScroll = offset;
        resetItemsDirty();
        updateActionRects();
        update();
    }
}

int ARibbonBackstageViewMenu::totalHeight() const
{
    return m_totalHeight;
}

void ARibbonBackstageViewMenu::initStyleOption(ARibbonBackstageStyleOption* opt) const
{
    opt->initFrom(this);
    opt->menuWidth = m_menuWidth;
    opt->rect.adjust(0, 0, -1, -1);
}

void ARibbonBackstageViewMenu::initStyleOption(QStyleOptionMenuItem* option, const QAction* action) const
{
    if (!option || !action)
        return;

    option->initFrom(this);
    option->palette = palette();
    option->state = QStyle::State_None;

    if (m_backstageView->window()->isActiveWindow())
        option->state |= QStyle::State_Active;
    if (m_backstageView->isEnabled() && action->isEnabled()
        && (!action->menu() || action->menu()->isEnabled()))
        option->state |= QStyle::State_Enabled;
    
    if (option->state & QStyle::State_Enabled)
        option->palette.setCurrentColorGroup(option->state & QStyle::State_Active ? QPalette::Active : QPalette::Inactive);
    else
        option->palette.setCurrentColorGroup(QPalette::Disabled);

    option->font = action->font().resolve(font());
    option->fontMetrics = QFontMetrics(option->font, const_cast<ARibbonBackstageViewMenu*>(this));

    if (m_backstageView->aproch_d().m_currentAction && m_backstageView->aproch_d().m_currentAction == action && !m_backstageView->aproch_d().m_currentAction->isSeparator()) 
        option->state |= QStyle::State_Selected | (m_mouseDown ? QStyle::State_Sunken : QStyle::State_None);

    option->menuItemType = QStyleOptionMenuItem::Normal;
    if (const QWidgetAction* pageAct = qobject_cast<const QWidgetAction*>(action))
    {
        option->menuItemType = QStyleOptionMenuItem::SubMenu;
        if (pageAct->defaultWidget() == m_backstageView->activePage())
            option->state |= QStyle::State_HasFocus;
    }
    else if (action->isSeparator())
        option->menuItemType = QStyleOptionMenuItem::Separator;

    if (action->isIconVisibleInMenu())
        option->icon = action->icon();
    QString textAndAccel = action->text();

#ifndef QT_NO_SHORTCUT
    if (textAndAccel.indexOf(QLatin1Char('\t')) == -1) {
        QKeySequence seq = action->shortcut();
        if (!seq.isEmpty())
            textAndAccel += QLatin1Char('\t') + seq.toString();
    }
#endif
    option->text = textAndAccel;
    option->maxIconWidth = m_maxIconWidth;
    option->menuRect = rect();
}

void ARibbonBackstageViewMenu::updateActionRects() const
{
    if (!m_itemsDirty)
        return;

    ARibbonBackstageViewMenu* menuThis = const_cast<ARibbonBackstageViewMenu*>(this);
    menuThis->m_totalHeight = 0;

    ensurePolished();
    QList<QAction*> actions = m_backstageView->actions();
    //let's reinitialize the buffer
    m_actionRects.resize(actions.count());
    m_actionRects.fill(QRect());

    QStyle* style = this->style();
    QStyleOption opt;
    opt.initFrom(this);

    const int icone = style->pixelMetric(QStyle::PM_SmallIconSize, &opt, this);

    QFontMetrics qfm = fontMetrics();

    m_maxIconWidth = 0;
    int actionHeight = 26;
    int pageHeight = qMax(37, qfm.height() + 10);

    for (int i = 0, count = actions.count(); i < count; i++)
    {
        QAction *action = actions.at(i);
        if (action->isSeparator() || !action->isVisible() )
            continue;

        QIcon is = action->icon();
        if (!is.isNull())
            m_maxIconWidth = qMax<int>(m_maxIconWidth, icone + 4);
    }

    for (int i = 0, count = actions.count(); i < count; i++)
    {
        QAction* action = actions.at(i);
        if (!action->isVisible() ||  action->isSeparator())
            continue; // we continue, this action will get an empty QRect

        // let the style modify the above size..
        QStyleOptionMenuItem opt;
        initStyleOption(&opt, action);

        QIcon is = action->icon();
        const QFontMetrics& fm = opt.fontMetrics;

        QRect br(fm.boundingRect(action->text()));
        int width = br.width() + commandGap * 2 + textGap;
        if (!is.isNull())
            width = br.width() + commandGap * 2 + textGap * 2 + textGap;

        actionHeight = qMax(actionHeight, qMax(fm.height() + fm.descent() * 2, qfm.height() + qfm.descent() * 2));

        if (!is.isNull() && actionHeight < icone)
            actionHeight = icone;

        width += m_maxIconWidth;
        m_menuWidth = qMax(m_menuWidth, width);
    }

    const int vSpacing = style->pixelMetric(QStyle::PM_LayoutVerticalSpacing, nullptr, this);
    int top = m_backStageCloseButton ? m_backStageCloseButton->geometry().bottom() + vSpacing : vSpacing / 2;
    top -= m_offsetScroll;
    for (int i = 0, count = actions.count(); i < count; i++)
    {
        QAction* action = actions.at(i);
        if (!action->isVisible() /*||  action->isSeparator()*/)
            continue; // we continue, this action will get an empty QRect

        if (qobject_cast<QWidgetAction*>(action))
        {
            m_actionRects[i] = QRect(QPoint(0, top), QPoint(m_menuWidth, top + pageHeight));
            top += pageHeight + 2;
        }
        else if (action->isSeparator())
        {
            m_actionRects[i] = QRect(QPoint(-commandGap, top), QPoint(m_menuWidth - commandGap * 2, top + actionHeight - 3));
            top += actionHeight - 3;
        }
        else
        {
            m_actionRects[i] = QRect(QPoint(commandGap, top), QPoint(m_menuWidth - commandGap, top + actionHeight));
            top += actionHeight + 2;
        }
    }

    if (m_backstageView->activePage() == nullptr)
    {
        QWidget* firstPage = nullptr;
        for (int i = 0, count = actions.count(); i < count; i++)
        {
            if (QWidgetAction* action = qobject_cast<QWidgetAction*>(actions.at(i)))
            {
                if (firstPage == nullptr)
                    firstPage = action->defaultWidget();

                if (action->isEnabled())
                {
                    m_backstageView->setActivePage(action->defaultWidget());
                    break;
                }
            }
        }
        if (m_backstageView->activePage() == nullptr && firstPage != nullptr)
            m_backstageView->setActivePage(firstPage);
    }

    menuThis->m_totalHeight = top + m_offsetScroll;
    m_itemsDirty = false;
}

void ARibbonBackstageViewMenu::paintEvent(QPaintEvent* event)
{
    updateActionRects();
    QPainter painter(this);

    QRect rc = rect();
    QRegion emptyArea = QRegion(rc);
    QRegion emptyArea1 = emptyArea;

    ARibbonBackstageStyleOption optBackstage;
    if (m_backStageCloseButton == nullptr)
    {
        optBackstage.initFrom(m_backstageView);
        optBackstage.menuWidth = m_menuWidth;
    }
    else
        initStyleOption(&optBackstage);
                                                                                                        
    style()->drawPrimitive(static_cast<QStyle::PrimitiveElement>(ACommonStyle::PE_RibbonBackstageFrame), &optBackstage, &painter, m_backstageView);

    QRect rectEvent = event->rect();
    QList<QAction*> acts = m_backstageView->actions();
    // draw the items that need updating..
    for (int i = 0, count = acts.count(); i < count; ++i) 
    {
        QAction* action = acts.at(i);
        QRect adjustedActionRect = m_actionRects.at(i);
        if (!rectEvent.intersects(adjustedActionRect))
            continue;

        // set the clip region to be extra safe (and adjust for the scrollers)
        QRegion adjustedActionReg(adjustedActionRect);
        emptyArea -= adjustedActionReg;

        QStyleOptionMenuItem opt;
        initStyleOption(&opt, action);
        opt.rect = adjustedActionRect;
        opt.text = action->text();

        if (QWidgetAction* pageAct = qobject_cast<QWidgetAction*>(action))
        {
            opt.menuItemType = QStyleOptionMenuItem::SubMenu;
            if (pageAct->defaultWidget() == m_backstageView->activePage())
                opt.state |= QStyle::State_HasFocus;
        }
        style()->drawControl(static_cast<QStyle::ControlElement>(ACommonStyle::CE_RibbonBackstageMenuItem), &opt, &painter, m_backstageView);
    }
}

/*! \reimp */
void ARibbonBackstageViewMenu::keyPressEvent(QKeyEvent* event)
{
    QWidget::keyPressEvent(event);
    switch(event->key())
    {
        case Qt::Key_Up:
            break;
        case Qt::Key_Down:
            break;
        default:
            break;
    }
}

/*! \reimp */
void ARibbonBackstageViewMenu::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton)
        return;
    if (rect().contains(event->pos()))
    {
        if (QWidgetAction* act = qobject_cast<QWidgetAction*>(actionAt(event->pos())))
        {
            if (act && (act->isSeparator() || !act->isEnabled()))
                return;
            if (QWidget* w = act->defaultWidget())
            {
                m_backstageView->setActivePage(w);
            }
        }
    }
}

void ARibbonBackstageViewMenu::mouseMoveEvent(QMouseEvent* event)
{
    if (rect().contains(event->pos()))
    {
        QAction* action = actionAt(event->pos());
        m_backstageView->aproch_d().setCurrentAction(action ? action : nullptr);
        setFocus();
    }
}

/*! \reimp */
void ARibbonBackstageViewMenu::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton)
        return;

    if (rect().contains(event->pos()))
    {
        QAction* action = actionAt(event->pos());

        if (action && (action->isSeparator() || !action->isEnabled() || m_backstageView->aproch_d().m_widgetItems.contains(action)))
            return;

        if (action && action == m_backstageView->aproch_d().m_currentAction) 
        {
            if (!m_backstageView->isClosePrevented())
                m_backstageView->close();
            action->activate(QAction::Trigger);
        }
    }
}

/*! \reimp */
void ARibbonBackstageViewMenu::leaveEvent(QEvent* event)
{
    QWidget::leaveEvent(event);
    m_backstageView->aproch_d().setCurrentAction(nullptr);
}

/*! \reimp */
void ARibbonBackstageViewMenu::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    if (m_backStageCloseButton)
        m_backStageCloseButton->show();
}

/*! \reimp */
void ARibbonBackstageViewMenu::hideEvent(QHideEvent* event)
{
    QWidget::hideEvent(event);
    if (m_backStageCloseButton)
        m_backStageCloseButton->hide();
}

/*!
\class ARibbonBackstageSeparator
\inmodule QtitanRibbon
\brief Class ARibbonBackstageSeparator used to display separators for both vertical and horizontal forms in ARibbonBackstagePage.
*/

/*!
Constructs ARibbonBackstageSeparator object with the given \a parent.
*/
ARibbonBackstageSeparator::ARibbonBackstageSeparator(QWidget* parent)
    : QFrame(parent)
{
    setAttribute(Qt::WA_MouseNoMask); 
    setFrameStyle(HLine | Sunken);
}

/*!
Destructor of the ARibbonBackstageSeparator object.
*/
ARibbonBackstageSeparator::~ARibbonBackstageSeparator()
{
}

/*!
Sets the \a orientation of the separator.
*/
void ARibbonBackstageSeparator::setOrientation(Qt::Orientation orient)
{ 
    setFrameShape(orient == Qt::Horizontal ? HLine : VLine); 
}

/*!
Returns the orientation of the separator.
*/
Qt::Orientation ARibbonBackstageSeparator::orientation() const
{ 
    return frameShape() == HLine ? Qt::Horizontal : Qt::Vertical; 
}

class ARibbonBackstageButtonPrivate : public QObject
{
public:
    A_DECLARE_PUBLIC(ARibbonBackstageButton)
public:
    explicit ARibbonBackstageButtonPrivate();
    virtual ~ARibbonBackstageButtonPrivate();
    void init();
    void ensureTextLayouted();
    int rowCount() const;
public:
    QSize sizeWordWrap();
public:
    bool m_tabStyle;
    bool m_textLayoutDirty;
    QTextDocument* m_document;
};

ARibbonBackstageButtonPrivate::ARibbonBackstageButtonPrivate()
    : m_tabStyle(false)
    , m_textLayoutDirty(true)
    , m_document(nullptr)
{
}

ARibbonBackstageButtonPrivate::~ARibbonBackstageButtonPrivate()
{
    delete m_document;
}

void ARibbonBackstageButtonPrivate::init()
{
    A_P(ARibbonBackstageButton);
    m_document = new QTextDocument(&p);
}

void ARibbonBackstageButtonPrivate::ensureTextLayouted()
{
    A_P(ARibbonBackstageButton);
    if (!m_textLayoutDirty)
        return;

    QStyleOptionToolButton opt;
    p.initStyleOption(&opt);

    int flags = Qt::TextShowMnemonic;
    flags |= Qt::AlignHCenter | Qt::TextWordWrap;

    m_document->setPlainText(opt.text);
    m_document->setDefaultFont(opt.font);

    QTextOption optText = m_document->defaultTextOption();
    if (flags & Qt::TextWordWrap)
        optText.setWrapMode(QTextOption::WordWrap);
    else
        optText.setWrapMode(QTextOption::ManualWrap);

    Qt::Alignment alignment = optText.alignment();
    alignment &= ~Qt::AlignLeft;
    alignment |= Qt::Alignment(Qt::AlignHCenter | Qt::AlignTop);
    optText.setAlignment(alignment);
    m_document->setDefaultTextOption(optText);
    m_document->setDocumentMargin(0);
//    m_document->setTextWidth(static_cast<qreal>(opt.rect.width()));
    m_textLayoutDirty = false;
}

int ARibbonBackstageButtonPrivate::rowCount() const
{
    A_P(const ARibbonBackstageButton);
    QSize sizeWordWrap = m_document->size().toSize();
    QFontMetrics fm(p.font());
    return sizeWordWrap.height() / fm.height();
}

QSize ARibbonBackstageButtonPrivate::sizeWordWrap()
{
    ensureTextLayouted();
    return m_document->size().toSize();
}


/*!
\class ARibbonBackstageButton
\inmodule QtitanRibbon
\brief Class ARibbonBackstageButton used to display buttons onto backstage page. Buttons can be presented as tabs, ordinary buttons or flat buttons. The button has apropriate style and look-and-feel for the backstageview concept.
*/

/*!
Constructs ARibbonBackstageButton object with the given \a parent.
*/
ARibbonBackstageButton::ARibbonBackstageButton(QWidget* parent)
    : QToolButton(parent)
{
    A_INIT_PRIVATE(ARibbonBackstageButton);
    A_D(ARibbonBackstageButton);
    d.init();
}

/*!
Destructor of the ARibbonBackstageButton object.
*/
ARibbonBackstageButton::~ARibbonBackstageButton()
{ 
    A_DELETE_PRIVATE();
}

/*!
Returns true if the button is used as a tab.
*/
bool ARibbonBackstageButton::tabStyle() const
{
    A_D(const ARibbonBackstageButton);
    return d.m_tabStyle;
}

/*!
Sets the \a style for a button that allows you to use button as a tab.
*/
void ARibbonBackstageButton::setTabStyle(bool style)
{
    A_D(ARibbonBackstageButton);
    d.m_tabStyle = style;
    update();
}

QSize ARibbonBackstageButton::minimumSizeHint() const
{
    return sizeHint();
}

/*! \reimp */
QSize ARibbonBackstageButton::sizeHint() const
{
    A_D(const ARibbonBackstageButton);
    ARibbonBackstageButton* thisButton = const_cast<ARibbonBackstageButton*>(this);

    QSize sz;
    QStyleOptionToolButton opt;
    initStyleOption(&opt);

    if (opt.toolButtonStyle != Qt::ToolButtonIconOnly)
    {
        if (opt.toolButtonStyle == Qt::ToolButtonTextUnderIcon)
        {
            if (!opt.icon.isNull())
            {
                const int actualSize = opt.icon.actualSize(opt.iconSize, QIcon::Normal, QIcon::Off).width();
                const int ret = ACommonStylePrivate::dpiScaled(actualSize, this);
                sz = QSize(ret, ret);
            }
            else
            {
                const int sizeIcon = style()->pixelMetric(QStyle::PM_LargeIconSize, &opt, this);
                sz = QSize(sizeIcon, sizeIcon);
            }

            QSize sizeWordWrap = thisButton->aproch_d().sizeWordWrap();
            sizeWordWrap.setHeight(sizeWordWrap.height() * (d.rowCount() == 1 ? 2 : 1));
            sz.setHeight(sz.height() + sizeWordWrap.height() + (opt.fontMetrics.height() / 2));
            sz.setWidth(qMax(sizeWordWrap.width(), sz.width()));
            return sz;
        }
        else if (opt.toolButtonStyle == Qt::ToolButtonTextOnly)
        {
            const int sizeIcon = style()->pixelMetric(QStyle::PM_LargeIconSize, &opt, this);
            QSize sizeWordWrap = thisButton->aproch_d().sizeWordWrap();
            sizeWordWrap.setHeight(sizeWordWrap.height() * (d.rowCount() == 1 ? 2 : 1));
            sz.setHeight(sizeIcon + sizeWordWrap.height() + (opt.fontMetrics.height() / 2));
            sz.setWidth(qMax(sizeWordWrap.width(), sz.width()));
            return sz;
        }
    }
    return QToolButton::sizeHint();
}

/*! \reimp */
bool ARibbonBackstageButton::event(QEvent* event)
{
    A_D(ARibbonBackstageButton);
    QEvent::Type type = event->type();
    if (type == QEvent::StyleChange || type == QEvent::FontChange || 
        type == QEvent::Show || type == QEvent::Resize)
        d.m_textLayoutDirty = true;
    return QToolButton::event(event);
}

/*! \reimp */
void ARibbonBackstageButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    A_D(ARibbonBackstageButton);
    QPainter p(this);
    ABackstageButtonStyleOption option;
    initStyleOption(&option);

    if (!option.icon.isNull())
    {
        const int actualSize = option.icon.actualSize(option.iconSize, QIcon::Normal, QIcon::Off).width();
        const int ret = ACommonStylePrivate::dpiScaled(actualSize, this);
        option.iconSize = QSize(ret, ret);
    }
    else
    {
        const int sizeIcon = style()->pixelMetric(QStyle::PM_LargeIconSize, &option, this);
        option.iconSize = QSize(sizeIcon, sizeIcon);
    }
    option.tabStyle = d.m_tabStyle;
    option.document = d.m_document;
    style()->drawComplexControl(static_cast<QStyle::ComplexControl>(ACommonStyle::CC_RibbonBackstageButton), &option, &p, this);
}

/*!
\class ARibbonBackstagePage
\inmodule QtitanRibbon
\brief The class ARibbonBackstagePage is used for rendering pages for the backstage view concept.
*/

/*!
Constructs ARibbonBackstagePage object with the given \a parent.
*/
ARibbonBackstagePage::ARibbonBackstagePage(QWidget* parent)
    : QWidget(parent)
{
}

/*!
Destructor of the ARibbonBackstagePage object.
*/
ARibbonBackstagePage::~ARibbonBackstagePage()
{
}

/*! \reimp */
void ARibbonBackstagePage::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
    QStyleOptionFrame opt;
    opt.initFrom(this);
    style()->drawPrimitive(QStyle::PE_FrameWindow, &opt, &p, this);
}

/*!
\class ARibbonBackstageViewPrivate
\internal
*/
ARibbonBackstageViewPrivate::ARibbonBackstageViewPrivate()
    : m_closePrevented(false)
    , m_scrollOffset(0)
    , m_scrollOffsetHorz(0)
    , m_totalHeight(0)
    , m_scrollBarVert(nullptr)
    , m_scrollBarHorz(nullptr)
    , m_backstageMenu(nullptr)
    , m_activePage(nullptr)
    , m_currentAction(nullptr)
    , m_sizeRightPage(QSize())
{
}

ARibbonBackstageViewPrivate::~ARibbonBackstageViewPrivate()
{
    A_P(ARibbonBackstageView);
    QWidget* parent =  p.parentWidget();
    if (parent != nullptr)
        parent->removeEventFilter(&p);
}

void ARibbonBackstageViewPrivate::init()
{
    A_P(ARibbonBackstageView);
    p.setAttribute(Qt::WA_NoMousePropagation);

    QWidget* parent = p.parentWidget();
    if (parent != nullptr)
        parent->installEventFilter(&p);

    m_backstageMenu = new ARibbonBackstageViewMenu(&p);
    m_backstageMenu->setAttribute(Qt::WA_NoMousePropagation);
    m_scrollBarVert = new QScrollBar(Qt::Vertical, &p);
    m_scrollBarVert->setGeometry(QRect());
    connect(m_scrollBarVert, SIGNAL(actionTriggered(int)), this, SLOT(scrollVertTriggered(int)));
    m_scrollBarHorz = new QScrollBar(Qt::Horizontal, &p);
    m_scrollBarHorz->setGeometry(QRect());
    connect(m_scrollBarHorz, SIGNAL(actionTriggered(int)), this, SLOT(scrollHorzTriggered(int)));
    p.setVisible(false);
}

void ARibbonBackstageViewPrivate::adjustLocations()
{
    m_backstageMenu->setOffsetScroll(m_scrollOffset);
    updateLayoutInternal();
    m_scrollBarVert->raise();
    m_scrollBarHorz->raise();
}

void ARibbonBackstageViewPrivate::adjustScrollBars()
{
    A_P(ARibbonBackstageView);
    int scrollOffsetOld = m_scrollOffset;
    int scrollOffsetHorzOld = m_scrollOffsetHorz;

    const int scrollWidth = p.style()->pixelMetric(QStyle::PM_ScrollBarExtent, nullptr, &p);
    const int scrollHeight = scrollWidth;

    QRect rect = p.rect();
    int totalWidth = m_sizeRightPage.width();
    totalWidth += m_backstageMenu->sizeHint().width();

    int totalHeight = qMax(m_totalHeight, m_sizeRightPage.height());

    bool hasVertScrollBar = false;
    bool hasHorzScrollBar = totalWidth > rect.width();

    if (totalHeight <= rect.height())
    {
        m_scrollBarVert->setVisible(false);
        m_scrollOffset = 0;
    }
    else
    {
        int vertScrollHeight = rect.height();
        if (hasHorzScrollBar)
            vertScrollHeight -= scrollHeight;

        m_scrollBarVert->setGeometry(QRect(QPoint((rect.right() - scrollWidth), rect.top()), QSize(scrollWidth, vertScrollHeight)));
        m_scrollBarVert->setVisible(true);
        m_scrollBarVert->raise();

        m_scrollBarVert->setMinimum(0);
        m_scrollBarVert->setMaximum(totalHeight - rect.height());
        m_scrollBarVert->setPageStep(rect.height());

        if (m_scrollOffset > totalHeight)
            m_scrollOffset = totalHeight;

        hasVertScrollBar = true;
    }

    if (totalWidth <= rect.width())
    {
        m_scrollBarHorz->setVisible(false);
        m_scrollOffsetHorz = 0;
    }
    else
    {
        int horzScrollWidth = rect.width();
        if (hasVertScrollBar)
            horzScrollWidth -= scrollWidth;

        m_scrollBarHorz->setGeometry(QRect(QPoint(rect.left(), (rect.bottom() - scrollHeight) + 1), QSize(horzScrollWidth, scrollHeight)));
        m_scrollBarHorz->setVisible(true);
        m_scrollBarHorz->raise();

        m_scrollBarHorz->setMinimum(0);
        m_scrollBarHorz->setMaximum(totalWidth - rect.width());
        m_scrollBarHorz->setPageStep(rect.width());

        if (m_scrollOffsetHorz > totalWidth)
            m_scrollOffsetHorz = totalWidth;
    }

    if (((scrollOffsetOld != m_scrollOffset) || (scrollOffsetHorzOld != m_scrollOffsetHorz)) && m_backstageMenu != nullptr)
        adjustLocations();
}

void ARibbonBackstageViewPrivate::updateLayoutInternal()
{
    A_P(ARibbonBackstageView);
    int backstageViewTop = 0;
    m_backstageMenu->createBackstageCloseButton();

    QRect rect = p.rect();
    
    QWidget* parent = p.parentWidget();
    if (parent != nullptr)
        rect = parent->rect();

    QRect rectMenu = rect;
    rectMenu.setLeft(rectMenu.left() - m_scrollOffsetHorz);
    QSize sz = m_backstageMenu->sizeHint();
    m_backstageMenu->setGeometry(QRect(QPoint(rectMenu.topLeft()), QPoint(rectMenu.left() + sz.width(), rectMenu.bottom())));
    m_backstageMenu->raise();

    if (m_backstageMenu->closeButton())
        m_backstageMenu->closeButton()->raise();

    rect.setTop(rect.top() + backstageViewTop);

    if (!static_cast<bool>(p.style()->styleHint(static_cast<QStyle::StyleHint>(ACommonStyle::SH_RibbonBackstageHideTabs))))
    {
        backstageViewTop = aproch_backstageViewTop(&p);
        rect.setTop(rect.top() + backstageViewTop);
    }

    rect.setLeft(rect.left() - m_scrollOffsetHorz);
    if (parent != nullptr)
    {
        p.setGeometry(rect);
        p.raise();
    }
    updateGeometryPage(m_activePage);
    m_totalHeight = qMax(m_totalHeight, m_backstageMenu->totalHeight());
}

void ARibbonBackstageViewPrivate::updateLayout()
{
    m_backstageMenu->resetItemsDirty();
    updateLayoutInternal();
    adjustScrollBars();
}

void ARibbonBackstageViewPrivate::updateGeometryPage(QWidget* widget)
{
    if (widget == nullptr)
        return;

    A_P(const ARibbonBackstageView);
    m_totalHeight = 0;
    QSize minimumSize = widget->minimumSize();
    int top = static_cast<bool>(p.style()->styleHint(static_cast<QStyle::StyleHint>(ACommonStyle::SH_RibbonBackstageHideTabs))) ? 
        aproch_backstageViewTop(&p) : 2;
    int width = m_backstageMenu->sizeHint().width();
    QRect rcContent(QPoint(width, top - m_scrollOffset), QPoint(p.rect().right(), p.rect().bottom()));
    rcContent.setRight(qMax(rcContent.right(), rcContent.left() + minimumSize.width()));
    rcContent.setBottom(qMax(rcContent.bottom(), rcContent.top() + minimumSize.height()));

    widget->setGeometry(rcContent);
    m_sizeRightPage = m_mapSize[widget];
}

void ARibbonBackstageViewPrivate::setCurrentAction(QAction* action)
{
    if (m_currentAction)
        m_backstageMenu->update(m_backstageMenu->actionRect(m_currentAction));

    m_currentAction = action;

    if (action) 
    {
        if (!action->isSeparator()) 
            m_backstageMenu->update(m_backstageMenu->actionRect(action));
    }
}

QAction* ARibbonBackstageViewPrivate::currentAction() const
{
    return m_currentAction;
}

QWidgetAction* ARibbonBackstageViewPrivate::getAction(QWidget* w) const
{
    A_P(const ARibbonBackstageView);
    QList<QAction*> actions = p.actions();
    for (int i = 0, count = actions.count(); i < count; i++)
    {
        QWidgetAction* action = qobject_cast<QWidgetAction*>(actions.at(i));
        if (action && action->defaultWidget() == w)
            return action;
    }
    return nullptr;
}

void ARibbonBackstageViewPrivate::scrollVertTriggered(int action)
{
    int iOffset = m_scrollOffset;
    int pageStep = m_scrollBarVert->pageStep();
    int totalHeight = qMax(m_totalHeight, m_sizeRightPage.height());

    if (totalHeight - pageStep <= 1)
        return;

    switch (action)
    {
        case QAbstractSlider::SliderMove:
            iOffset = m_scrollBarVert->sliderPosition();
            break;
        case QAbstractSlider::SliderSingleStepSub:
            iOffset -= pageStep;
            break;
        case QAbstractSlider::SliderSingleStepAdd:
            iOffset += pageStep;
            break;
        default:
            break;
    }

    iOffset = qMin(qMax(0, iOffset), totalHeight - pageStep);

    if (iOffset == m_scrollOffset)
        return;

    m_scrollOffset = iOffset;
    adjustLocations();
    m_scrollBarVert->setSliderPosition(iOffset);
}

void ARibbonBackstageViewPrivate::scrollHorzTriggered(int action)
{
    int iOffset = m_scrollOffsetHorz;
    int pageStep = m_scrollBarVert->pageStep();
    int totalWidth = m_sizeRightPage.width();
    totalWidth += m_backstageMenu->sizeHint().width();

    if (totalWidth - pageStep <= 1)
        return;

    switch (action)
    {
        case QAbstractSlider::SliderMove:
            iOffset = m_scrollBarHorz->sliderPosition();
            break;
        case QAbstractSlider::SliderSingleStepSub:
            iOffset -= pageStep;
            break;
        case QAbstractSlider::SliderSingleStepAdd:
            iOffset += pageStep;
            break;
        default:
            break;
    }

    iOffset = qMin(qMax(0, iOffset), totalWidth - pageStep);

    if (iOffset == m_scrollOffsetHorz)
        return;

    m_scrollOffsetHorz = iOffset;
    adjustLocations();
    m_scrollBarHorz->setSliderPosition(iOffset);
}

/*!
    \class ARibbonBackstageView
    \inmodule QtitanRibbon
    \brief The class for rendering backstage view window in ribbon concept. Can be top level window or placed on other windows.
*/

/*!
\fn void ARibbonBackstageView::aboutToShow();
Signal is emitted just before the ARibbonBackstageView is shown to the user.
*/

/*!
\fn void ARibbonBackstageView::aboutToHide();
Signal is emitted just before the ARibbonBackstageView is shown to the user.
*/

/*!
Constructs ARibbonBackstageView object with the given \a parent.
*/
ARibbonBackstageView::ARibbonBackstageView(QWidget* parent)
    : QWidget(parent)
{
    A_INIT_PRIVATE(ARibbonBackstageView);
    A_D(ARibbonBackstageView);
    d.init();
}

/*!
Destructor of the ARibbonBackstageView object.
*/
ARibbonBackstageView::~ARibbonBackstageView()
{
    A_DELETE_PRIVATE();
}

/*!
\reimp
*/
void ARibbonBackstageView::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);
    A_D(ARibbonBackstageView);
//    d.m_tabBarPosition = d.m_ribbon->tabBarPosition();
//    d.m_ribbon->setTabBarPosition(ARibbonBar::TopPosition);
    aproch_setBackstageViewVisible(this, true);
    d.updateLayout();
    setFocus();
    emit aboutToShow();
}

/*!
\reimp
*/
void ARibbonBackstageView::hideEvent(QHideEvent * event)
{
    Q_UNUSED(event);
    aproch_setBackstageViewVisible(this, false);
    emit aboutToHide();
}

/*!
\reimp
*/
void ARibbonBackstageView::wheelEvent(QWheelEvent* event)
{
    A_D(ARibbonBackstageView);
    if (!d.m_scrollBarVert->isVisible())
        return;
    if (QApplication::activePopupWidget() != nullptr)
        return;

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    const int delta = (qAbs(event->angleDelta().x()) > qAbs(event->angleDelta().y()) ?
        event->angleDelta().x() : event->angleDelta().y());
    const int steps = abs(delta) / wheelDelta;
    for (int i = 0; i < steps; i++)
        d.scrollVertTriggered(delta < 0 ? QAbstractSlider::SliderSingleStepAdd : QAbstractSlider::SliderSingleStepSub);
#else
    const int steps = abs(event->delta()) / wheelDelta;
    for (int i = 0; i < steps; i++)
        d.scrollVertTriggered(event->delta() < 0 ? QAbstractSlider::SliderSingleStepAdd : QAbstractSlider::SliderSingleStepSub);
#endif
}

bool ARibbonBackstageView::isClosePrevented() const
{
    A_D(const ARibbonBackstageView);
    return d.m_closePrevented;
}

void ARibbonBackstageView::setClosePrevented(bool prevent)
{
    A_D(ARibbonBackstageView);
    d.m_closePrevented = prevent;
}

/*!
Creates a new action with an \a icon and some \a text. The function adds the newly created action to the view's list of actions, and returns it.
*/
QAction* ARibbonBackstageView::addAction(const QIcon& icon, const QString& text)
{
    QAction* action = new QAction(icon, text, this);
    addAction(action);
    return action;
}

/*!
Adds page separator to the ribbon backstage view.
*/
QAction* ARibbonBackstageView::addSeparator()
{
    QAction* action = new QAction(this);
    action->setSeparator(true);
    addAction(action);
    return action;
}

/*!
Adding the new page to the view. Parameter \a widget is a pointer to the QWidget which will be placed onto page. 
Adding a page leads to the causing of tab on the left side of the view.
To set the title of the page please use the fucntion QWidget::setWindowTitle()
*/
QAction* ARibbonBackstageView::addPage(QWidget* widget)
{
    A_D(ARibbonBackstageView);
    QWidgetAction* action = new QWidgetAction(this);
    action->setDefaultWidget(widget);
    widget->setFont(font());
    action->setText(widget->windowTitle());
    d.m_mapSize[widget] = widget->sizeHint();
    addAction(action);
    return action;
}

/*!
Returns an active widget from the current view.
*/
QWidget* ARibbonBackstageView::activePage() const
{
    A_D(const ARibbonBackstageView);
    return d.m_activePage;
}

/*!
Sets an active \a widget for the current view.
*/
void ARibbonBackstageView::setActivePage(QWidget* widget)
{
    A_D(ARibbonBackstageView);
    bool find = false;
    QList<QAction*> acts = actions();
    for (int i = 0, count = acts.count(); i < count && !find; i++)
    {
        if (QWidgetAction* action = qobject_cast<QWidgetAction*>(acts.at(i)))
        {
            if (action->defaultWidget() == widget)
            {
                if (!action->isVisible())
                {
                    Q_ASSERT(false);
                    return;
                }
                find = true;
            }
        }
    }

    if (!find)
    {
        Q_ASSERT(false);
        return;
    }

    if (d.m_activePage)
    {
        if (QWidgetAction* act = d.getAction(d.m_activePage))
            d.m_backstageMenu->update(d.m_backstageMenu->actionRect(act));
    }

    for (int i = 0, count = acts.count(); i < count; i++)
    {
        if (QWidgetAction* action = qobject_cast<QWidgetAction*>(acts.at(i)))
        {
            QWidget* w = action->defaultWidget();
            if (w == widget)
            {
                d.m_activePage = widget;
                d.updateLayout();
                w->setVisible(true);
            }
            else
                w->setVisible(false);
        }
    }

    if (d.m_activePage)
    {
        if (QWidgetAction* act = d.getAction(d.m_activePage))
            d.m_backstageMenu->update(d.m_backstageMenu->actionRect(act));
    }
}

/*!
Returns the rectangle that contains the given \a action.
*/
QRect ARibbonBackstageView::actionGeometry(QAction* action) const
{
    A_D(const ARibbonBackstageView);
    return d.m_backstageMenu->actionRect(action);
}

/*!
Returns the rectangle that contains the menu.
*/
QRect ARibbonBackstageView::menuGeometry() const
{
    A_D(const ARibbonBackstageView);
    return d.m_backstageMenu->geometry();
}

/*!
Opens the backstage view window.
*/
void ARibbonBackstageView::open()
{
    ensurePolished(); // Get the right font
    setVisible(true);
}

/*! \reimp */
bool ARibbonBackstageView::event(QEvent* event)
{
    switch (event->type())
    {
        case QEvent::KeyPress:
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Escape)
                close();
        }
        break;
        case QEvent::ParentChange :
            {
                if (QWidget* parent = parentWidget())
                    parent->installEventFilter(this);
            }
            break;
        case QEvent::ParentAboutToChange : 
            {
                if (QWidget* parent = parentWidget())
                    parent->removeEventFilter(this);
            }
            break;
        default:
            break;
    }
    return QWidget::event(event);
}

/*! \reimp */
void ARibbonBackstageView::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
    ARibbonBackstageStyleOption opt;
    opt.initFrom(this);
    style()->drawPrimitive(static_cast<QStyle::PrimitiveElement>(ACommonStyle::PE_RibbonBackstageFrame), &opt, &p, this);
}

/*! \reimp */
void ARibbonBackstageView::resizeEvent(QResizeEvent* event)
{
    A_D(ARibbonBackstageView);
    if (parentWidget() == nullptr)
        d.updateLayout();
    QWidget::resizeEvent(event);
}

/*! \reimp */
bool ARibbonBackstageView::eventFilter(QObject* object, QEvent* event)
{
    A_D(ARibbonBackstageView);
    if (parentWidget() == object && isVisible())
    {
        switch (event->type())
        {
            case QEvent::Resize : 
                d.updateLayout();
            break;
            default:
            break;
        }
    }
    return QWidget::eventFilter(object, event);
}

/*! \reimp */
void ARibbonBackstageView::actionEvent(QActionEvent* event)
{
    QWidget::actionEvent(event);

    A_D(ARibbonBackstageView);
    if (event->type() == QEvent::ActionAdded)
    {
        if (QWidgetAction* wa = qobject_cast<QWidgetAction*>(event->action())) 
        {
            QWidget* widget = wa->requestWidget(this);
            if (widget != nullptr)
                d.m_widgetItems.insert(wa, widget);
        }
    } 
    else if (event->type() == QEvent::ActionRemoved)
    {
        if (QWidgetAction* wa = qobject_cast<QWidgetAction*>(event->action())) 
        {
            if (QWidget *widget = d.m_widgetItems.value(wa))
                wa->releaseWidget(widget);
        }
        d.m_widgetItems.remove(event->action());
    } 
    else if (event->type() == QEvent::ActionChanged)
    {
        if (QWidgetAction* action = qobject_cast<QWidgetAction*>(event->action()))
        {
            if (!action->isVisible() && action->defaultWidget())
            {
                action->defaultWidget()->setVisible(false);

                if (activePage() == action->defaultWidget())
                {
                    bool activatePage = false;
                    QList<QAction*> acts = actions();
                    for (int i = 0, count = acts.count(); i < count && !activatePage; i++)
                    {
                        if (QWidgetAction* action = qobject_cast<QWidgetAction*>(acts.at(i)))
                        {
                            if (action->isVisible())
                            {
                                if (QWidget* widget = action->defaultWidget())
                                {
                                    setActivePage(widget);
                                    activatePage = true;
                                }
                            }
                        }
                    }
                }
            }
        }
        d.m_backstageMenu->resetItemsDirty();
    }
}

APROCH_NAMESPACE_END