/****************************************************************************
**
** Qtitan Library by Developer Machines (Microsoft-Ribbon implementation for Qt.C++)
** 
** Copyright (c) 2009-2022 Developer Machines (https://www.devmachines.com)
**           ALL RIGHTS RESERVED
** 
**  The entire contents of this file is protected by copyright law and
**  international treaties. Unauthorized reproduction, reverse-engineering
**  and distribution of all or any portion of the code contained in this
**  file is strictly prohibited and may result in severe civil and 
**  criminal penalties and will be prosecuted to the maximum extent 
**  possible under the law.
**
**  RESTRICTIONS
**
**  THE SOURCE CODE CONTAINED WITHIN THIS FILE AND ALL RELATED
**  FILES OR ANY PORTION OF ITS CONTENTS SHALL AT NO TIME BE
**  COPIED, TRANSFERRED, SOLD, DISTRIBUTED, OR OTHERWISE MADE
**  AVAILABLE TO OTHER INDIVIDUALS WITHOUT WRITTEN CONSENT
**  AND PERMISSION FROM DEVELOPER MACHINES
**
**  CONSULT THE END USER LICENSE AGREEMENT FOR INFORMATION ON
**  ADDITIONAL RESTRICTIONS.
**
****************************************************************************/

#include "QtnStyleOption.h"
#include "QtnRibbonPopup.h"
#include "QtnRibbonBarPrivate.h"
#include "QtnRibbonGroupPrivate.h"
#include "QtnRibbonButtonControls.h"
#include "QtnCommonStylePrivate.h"

#ifdef QTN_MEMORY_DEBUG
#include "QtitanMSVSDebug.h"
#endif

QTITAN_USE_NAMESPACE

/* RibbonPagePopup */
RibbonPagePopup::RibbonPagePopup(RibbonBar* ribbonBar)
    : QWidget(ribbonBar, Qt::Popup), m_ribbonBar(ribbonBar)
{
    Q_ASSERT(ribbonBar != Q_NULL);
}

RibbonBar* RibbonPagePopup::ribbonBar() const
{
    return m_ribbonBar;
}

void RibbonPagePopup::popup(const QRect& rect)
{
    QFont font = ribbonBar()->font();
    QPalette palette = ribbonBar()->palette();
    QFont_setResolveMask(font, QFont::AllPropertiesResolved);
    QPalette_setResolveMask(palette, 0xFFFFFF);
    setFont(font);
    setPalette(palette);

    QRect screen = qtn_availableGeometry(this);
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
void RibbonPagePopup::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    RibbonPage* page = ribbonBar()->page(ribbonBar()->currentPageIndex());
    if (page == Q_NULL)
        return;
    QPainter p(this);
    RibbonStyleOption opt;
    opt.initFrom(this);
    opt.contextColor = page->contextColor();
    style()->drawPrimitive((QStyle::PrimitiveElement)CommonStyle::PE_RibbonFrameGroups, &opt, &p, this);
}

/*! \reimp */
bool RibbonPagePopup::event(QEvent* event)
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
        RibbonBar* _ribbonBar = ribbonBar();
        if (_ribbonBar == Q_NULL)
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
        RibbonBar* _ribbonBar = ribbonBar();
        if (_ribbonBar == Q_NULL)
            break;
        if (_ribbonBar->isMinimized())
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QWidget* widget = QApplication::widgetAt(mouseEvent->globalPos());
            RibbonTab* tab = qobject_cast<RibbonTab*>(widget);
            if (tab != Q_NULL)
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

/* RibbonGroupPopup */
RibbonGroupPopup::RibbonGroupPopup(RibbonGroup* group)
    : QWidget(group, Qt::Popup), m_group(group)
{
    Q_ASSERT(m_group != Q_NULL);
    setFocusPolicy(Qt::NoFocus);
}

RibbonGroupPopup::~RibbonGroupPopup()
{
}

void RibbonGroupPopup::popup(const QPoint& pos)
{
    QFont font = m_group->font();
    QPalette palette = m_group->palette();
    QFont_setResolveMask(font, QFont::AllPropertiesResolved);
    QPalette_setResolveMask(palette, 0xFFFFFF);
    setFont(font);
    setPalette(palette);

    QRect rect = QRect(pos, sizeHint());
    QPoint newpos = rect.topLeft();
    QRect screen = qtn_availableGeometry(this);

    if (newpos.y() + rect.height() > screen.height())
        newpos.setY(screen.height() - rect.height());
    if (newpos.y() < 0)
        newpos.setY(0);

    const int desktopFrame = style()->pixelMetric(QStyle::PM_MenuDesktopFrameWidth, Q_NULL, m_group);
    if (newpos.x() + rect.width() - 1 > screen.right() - desktopFrame)
        newpos.setX(screen.right() - desktopFrame - rect.width() + 1);
    if (newpos.x() < screen.left() + desktopFrame)
        newpos.setX(screen.left() + desktopFrame);
    rect = QRect(newpos, rect.size());
    setGeometry(rect);
    show();
}

/*! \reimp */
QSize RibbonGroupPopup::sizeHint() const
{
    QSize size = RibbonGroupPrivate::_get(m_group)->sizeHint();
    return size;
}

/*! \reimp */
void RibbonGroupPopup::hideEvent(QHideEvent* event)
{
    event->ignore();

    QRect rect = m_group->rect();
    rect.moveTopLeft(m_group->mapToGlobal(rect.topLeft()));
    m_group->setAttribute(Qt::WA_UnderMouse, rect.contains(QCursor::pos()));
    m_group->update();
}

/*! \reimp */
void RibbonGroupPopup::paintEvent(QPaintEvent* event)
{
    event->accept();

    QPainter p(this);
    RibbonStyleOption styleOption;
    styleOption.initFrom(this);
    style()->drawPrimitive((QStyle::PrimitiveElement)CommonStyle::PE_RibbonFrameGroups, &styleOption, &p, this);

    RibbonGroupStyleOption groupStyleOption;
    RibbonGroupPrivate::_get(m_group)->initStyleOption(groupStyleOption);
    groupStyleOption.initFrom(this); //Init it again to change the base widget's options.
    style()->drawPrimitive((QStyle::PrimitiveElement)CommonStyle::PE_RibbonFrameGroup, &groupStyleOption, &p, this);
    
    int flags = groupStyleOption.textAlignment;
    flags |= Qt::TextHideMnemonic;
    QRect captionRect = groupStyleOption.rect;
    captionRect.setTop(captionRect.bottom() - groupStyleOption.heightCaption);

    int indent = 0;
    if (m_group->isOptionButtonVisible())
        indent = RibbonGroupPrivate::_get(m_group)->m_optionButton->width();

    captionRect.adjust(0, -1, -indent, -2);
    groupStyleOption.text = p.fontMetrics().elidedText(groupStyleOption.text, m_group->titleElideMode(), captionRect.adjusted(2, 0, -2, 0).width());
    style()->drawItemText(&p, captionRect, flags, groupStyleOption.palette,
        groupStyleOption.state & QStyle::State_Enabled, groupStyleOption.text, QPalette::WindowText);
}

/*! \reimp */
void RibbonGroupPopup::mousePressEvent(QMouseEvent* event)
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

/* RibbonGroupOption */
RibbonGroupOption::RibbonGroupOption(QWidget* parent)
    : QToolButton(parent)
{
}

RibbonGroupOption::~RibbonGroupOption()
{
}

QString RibbonGroupOption::text() const
{ return QString(); }

/*! \reimp */
QSize RibbonGroupOption::sizeHint() const
{
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    const int iconSize = style()->pixelMetric(static_cast<QStyle::PixelMetric>(CommonStyle::PM_RibbonOptionButtonIconSize), Q_NULL, this);
    opt.iconSize = QSize(iconSize, iconSize);
    QSize sizeHint = style()->sizeFromContents(QStyle::CT_ToolButton, &opt, opt.iconSize, this);
    return sizeHint;
}

void RibbonGroupOption::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    const int iconSize = style()->pixelMetric(static_cast<QStyle::PixelMetric>(CommonStyle::PM_RibbonOptionButtonIconSize), Q_NULL, this);
    opt.iconSize = QSize(iconSize, iconSize);
    style()->drawPrimitive((QStyle::PrimitiveElement)CommonStyle::PE_RibbonOptionButton, &opt, &p, this);
}

/*! \reimp */
void RibbonGroupOption::actionEvent(QActionEvent* event)
{
    QToolButton::actionEvent(event);
    if (event->type() == QEvent::ActionChanged)
    {
        QAction* action = event->action();
        setPopupMode(action->menu() ? QToolButton::MenuButtonPopup : QToolButton::DelayedPopup);
    }
}


/*!
\class RibbonGroupScrollButton
\internal
*/
RibbonGroupScrollButton::RibbonGroupScrollButton(QWidget* parent, bool scrollLeft)
    : QToolButton(parent)
    , m_scrollLeft( scrollLeft )
{
}

/*! \internal */
RibbonGroupScrollButton::~RibbonGroupScrollButton()
{
}

/*! \reimp */
void RibbonGroupScrollButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    opt.arrowType = m_scrollLeft ? Qt::LeftArrow : Qt::RightArrow;
    style()->drawPrimitive((QStyle::PrimitiveElement)CommonStyle::PE_RibbonGroupScrollButton, &opt, &p, this);
}

/*! \reimp */
void RibbonGroupScrollButton::leaveEvent(QEvent* event)
{
    QToolButton::leaveEvent(event);
    emit stopScrollTimer();
}

/* RibbonOverflowMenuButton */
RibbonOverflowMenuButton::RibbonOverflowMenuButton(QWidget* parent)
    : RibbonButton(parent)
{
}

void RibbonOverflowMenuButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    opt.features = QStyleOptionToolButton::None;
    int iconSize = qRound(qMin(opt.rect.width(), opt.rect.height()) / 2.5);
    ushort symbolIconCode = SegoeMDL2Font::code(SegoeMDL2Font::More);
    opt.icon = SegoeMDL2Font::pixmap(symbolIconCode, QSize(iconSize, iconSize), palette().color(foregroundRole()));
    style()->drawComplexControl((QStyle::ComplexControl)CommonStyle::CC_RibbonButton, &opt, &painter, this);
}

void RibbonOverflowMenuButton::mousePressEvent(QMouseEvent* event)
{
    event->ignore();
    if (event->button() != Qt::LeftButton)
        return;
    
    RibbonOverflowMenu* overflowMenu = qobject_cast<RibbonOverflowMenu*>(this->menu());
    if (overflowMenu != Q_NULL)
        overflowMenu->fillMenu();
    QToolButton::mousePressEvent(event);
    if (overflowMenu != Q_NULL)
        overflowMenu->clear();
    event->accept();
}

/* RibbonOverflowAction */
RibbonOverflowAction::RibbonOverflowAction(QWidget* control, QWidget* parent)
    : QWidgetAction(parent), m_control(control)
{
    Q_ASSERT(control != Q_NULL);
}

RibbonOverflowAction::~RibbonOverflowAction()
{
}

QWidget* RibbonOverflowAction::createWidget(QWidget* parent)
{
    m_control->setParent(parent);
    return m_control;
}

void RibbonOverflowAction::deleteWidget(QWidget* widget)
{
    if (widget == m_control)
        m_control->setParent(Q_NULL);
}

/* RibbonOverflowMenu */
RibbonOverflowMenu::RibbonOverflowMenu(QWidget* parent)
    : PopupMenu(parent)
{
}

RibbonOverflowMenu::~RibbonOverflowMenu()
{
}

void RibbonOverflowMenu::clearGroups()
{
    m_groups.clear();
}

void RibbonOverflowMenu::addGroup(RibbonGroup* group)
{
    Q_ASSERT(!m_groups.contains(group));
    m_groups.append(group);
}

void RibbonOverflowMenu::fillMenu()
{
    clear();
    for (QList<RibbonGroup*>::const_iterator it = m_groups.constBegin(); it != m_groups.constEnd(); ++it)
    {
        RibbonGroup* group = *it;

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
            RibbonControl* control = group->controlByIndex(i);

            RibbonToolBarControl* toolBarControl = qobject_cast<RibbonToolBarControl*>(control);
            bool proceed = toolBarControl && control->currentSize() == RibbonControlSizeDefinition::GroupSimplified;
            if (!proceed && !control->sizeDefinition(RibbonControlSizeDefinition::GroupSimplified)->isVisible())
                proceed = control->sizeDefinition(RibbonControlSizeDefinition::GroupSimplifiedOverflow)->isVisible();

            if (!proceed && control->currentSize() != RibbonControlSizeDefinition::GroupSimplifiedOverflow)
                continue;

            RibbonControlSizeDefinition* sizeDefinition = control->sizeDefinition(RibbonControlSizeDefinition::GroupSimplifiedOverflow);
            if (!sizeDefinition->isVisible())
                continue;

            if (control->defaultAction() != Q_NULL && control->defaultAction()->isSeparator())
            {
                action = new QAction(this);
                action->setSeparator(true);
            }
            else if (toolBarControl != Q_NULL)
            {
                if (!toolBarControl->isOverflowMenuIsEmpty())
                {
                    QAction* subMenuAction = new QAction(action->text(), this);
                    subMenuAction->setMenu(toolBarControl->overflowMenu());
                    action = subMenuAction;
                }
            }
            else if (RibbonButtonControl* button = qobject_cast<RibbonButtonControl*>(control))
            {
                Q_UNUSED(button);
                action = control->defaultAction();
            }
            else
            {
                action = new RibbonOverflowAction(control, this);
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

bool RibbonOverflowMenu::isEmpty() const
{
    return m_groups.count() == 0;
}

void RibbonOverflowMenu::paintMenuItem(QPainter* painter, QStyleOptionMenuItem* option)
{
    PopupMenu::paintMenuItem(painter, option);
}
