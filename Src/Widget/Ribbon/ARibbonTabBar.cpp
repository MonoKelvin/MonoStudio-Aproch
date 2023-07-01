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
#include <QApplication>
#include <QPainter>
#include <QTime>
#include <QWidgetAction>
#include <QStylePainter>
#include <QStyleOption>
#include <QEvent>

#include "QtnRibbonBarPrivate.h"
#include "QtnRibbonTabBar.h"
#include "QtnRibbonPagePrivate.h"
#include "QtnRibbonBackstageView.h"

#include "QtnStyleHelperPrivate.h"
#include "QtnStyleOption.h"
#include "QtnCommonStylePrivate.h"
#include "QtnRibbonQuickAccessBar.h"
#include "QtnRibbonDef.h"
#ifdef QTN_MEMORY_DEBUG
#include "QtitanMSVSDebug.h"
#endif

QTITAN_USE_NAMESPACE

/* ContextualTab */
ContextualTab::ContextualTab(RibbonBar* bar, RibbonTab* tab)
    : ribbonBar(bar), firstTab(tab), lastTab(tab)
{
    Q_ASSERT(bar != Q_NULL && tab != Q_NULL);
    firstTab->setContextualTab(this);
}

ContextualTab::~ContextualTab()
{
    if (firstTab != Q_NULL)
        firstTab->setContextualTab(Q_NULL);
    if (lastTab != Q_NULL)
        lastTab->setContextualTab(Q_NULL);
}

const QColor& ContextualTab::color() const
{
    if (firstTab != Q_NULL)
        return firstTab->contextColor();
    static QColor empty = QColor();
    return empty;
}

const QString& ContextualTab::title() const
{
    if (firstTab != Q_NULL)
        return firstTab->contextTitle();
    static QString empty = QString();
    return empty;
}

const QString& ContextualTab::groupName() const
{
    if (firstTab != Q_NULL)
        return firstTab->contextGroupName();
    static QString empty = QString();
    return empty;
}

QRect ContextualTab::rect() const
{
    RibbonBarPrivate* ribbonBarPrivate = RibbonBarPrivate::_get(ribbonBar);

    int offset = 0;
    if (WindowTitleBar* titleBar = ribbonBarPrivate->m_ribbonTitleBarWidget->getWindowTitleBar())
        offset = ribbonBarPrivate->m_ribbonTitleBarWidget->geometry().left() - titleBar->borderThickness();

    QRect firstRect = firstTab->geometry();
    QRect lastRect = lastTab->geometry();
    firstRect.moveTo(firstTab->mapTo(ribbonBar, QPoint()));
    lastRect.moveTo(lastTab->mapTo(ribbonBar, QPoint()));
    QRect rect(QPoint(firstRect.x(), 0), QPoint(lastRect.right(), ribbonBarPrivate->m_ribbonTitleBarWidget->height()));
    rect.translate(-offset, 0);
    return rect;
}

QFont ContextualTab::font() const
{
    RibbonBarPrivate* ribbonBarPrivate = RibbonBarPrivate::_get(ribbonBar);
    return ribbonBarPrivate->m_ribbonTitleBarWidget->font();
}

QTITAN_BEGIN_NAMESPACE

class RibbonTabPrivate : public QObject
{
public:
    QTN_DECLARE_PUBLIC(RibbonTab)
public:
    explicit RibbonTabPrivate();
    static RibbonTabPrivate* get(RibbonTab* tab)
    { return &tab->qtn_d(); }
public:
    void init();
    void updateLabel();
public:
    Qt::Alignment m_align;
    ContextualTab* m_contextualTab;
    QAction* m_defaultAction;
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    QTime m_dblClickInterval;
#endif
    mutable uint m_isTextLabel : 1;
};

QTITAN_END_NAMESPACE

RibbonTabPrivate::RibbonTabPrivate()
{
}

void RibbonTabPrivate::init()
{
    QTN_P(RibbonTab);
    m_contextualTab = Q_NULL;
    m_isTextLabel   = false;
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    m_dblClickInterval.start();
#endif
    m_align = Qt::Alignment(Qt::AlignLeft | Qt::AlignVCenter /*| Qt::TextExpandTabs*/);
    m_defaultAction = new QWidgetAction(&p);
    p.setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred, QSizePolicy::Label));
}

void RibbonTabPrivate::updateLabel()
{
    QTN_P(RibbonTab);
    if (m_isTextLabel)
    {
        QSizePolicy policy = p.sizePolicy();
        const bool wrap = m_align & Qt::TextWordWrap;
        policy.setHeightForWidth(wrap);
        // should be replaced by WA_WState_OwnSizePolicy idiom
        if (policy != p.sizePolicy())  
            p.setSizePolicy(policy);
    }
    p.update(p.contentsRect());
}

/* RibbonTab */
RibbonTab::RibbonTab(RibbonTabBar* parent, const QString& text)
    : AbstractTab(parent, QIcon(), QString())
{
    QTN_INIT_PRIVATE(RibbonTab);
    QTN_D(RibbonTab);
    d.init();
    setText(text);
    setAlignment(Qt::AlignCenter);
}

RibbonTab::~RibbonTab()
{
    QTN_D(RibbonTab);
    if (d.m_contextualTab != Q_NULL)
    {
        if (d.m_contextualTab->firstTab == this)
            d.m_contextualTab->firstTab = Q_NULL;
        if (d.m_contextualTab->lastTab == this)
            d.m_contextualTab->lastTab  = Q_NULL;
    }
    QTN_FINI_PRIVATE();
}

RibbonTabBar* RibbonTab::tabBar() const
{ 
    return static_cast<RibbonTabBar*>(m_tabBar);
}

RibbonPage* RibbonTab::page() const
{ 
    return qobject_cast<RibbonPage*>(object());
}

Qt::Alignment RibbonTab::alignment() const
{
    QTN_D(const RibbonTab);
    return QFlag(d.m_align & (Qt::AlignVertical_Mask|Qt::AlignHorizontal_Mask));
}

void RibbonTab::setAlignment(Qt::Alignment alignment)
{
    QTN_D(RibbonTab);
    if (alignment == (d.m_align & (Qt::AlignVertical_Mask|Qt::AlignHorizontal_Mask)))
        return;
    d.m_align = (d.m_align & ~(Qt::AlignVertical_Mask|Qt::AlignHorizontal_Mask)) | 
        (alignment & (Qt::AlignVertical_Mask|Qt::AlignHorizontal_Mask));

    d.updateLabel();
}

void RibbonTab::setText(const QString& text)
{
    QTN_D(RibbonTab);
    d.m_isTextLabel = true;
    AbstractTab::setText(text);
    d.updateLabel();
}

const QString& RibbonTab::text() const
{
    return m_text;
}

const QString& RibbonTab::contextTitle() const
{
    if (page() != Q_NULL)
        return page()->contextTitle();
    static QString empty = QString();
    return empty;
}

const QString& RibbonTab::contextGroupName() const
{
    if (page() != Q_NULL)
        return page()->contextGroupName();
    static QString empty = QString();
    return empty;
}

const QColor& RibbonTab::contextColor() const
{
    if (page() != Q_NULL)
        return page()->contextColor();
    static QColor empty = QColor();
    return empty;
}

void RibbonTab::setContextualTab(ContextualTab* tab)
{
    QTN_D(RibbonTab);
    d.m_contextualTab = tab;
}

ContextualTab* RibbonTab::contextualTab() const
{
    QTN_D(const RibbonTab);
    return d.m_contextualTab;
}

QAction* RibbonTab::defaultAction() const
{
    QTN_D(const RibbonTab);
    return d.m_defaultAction;
}

QSize RibbonTab::sizeHint() const
{
    QSize sh = AbstractTab::sizeHint();
    if (contextualTab() != Q_NULL)
    {
        int spacing = style()->pixelMetric(QStyle::PM_TabBarTabVSpace, Q_NULL, tabBar());
        sh.rwidth() = qMax(qtn_horizontalAdvanceFont(QFontMetrics(contextualTab()->font()), contextualTab()->title()) + spacing * 2, sh.width());
    }
    return sh;
}
QSize RibbonTab::minimumSizeHint() const
{
    return AbstractTab::minimumSizeHint();
}

void RibbonTab::clicked()
{
    bool selectionChanged = isCurrent();
    AbstractTab::clicked();
    selectionChanged = selectionChanged != isCurrent();

    RibbonBar* _ribbonBar = tabBar()->ribbonBar();
    
    Q_ASSERT(_ribbonBar != Q_NULL);
    if (_ribbonBar == Q_NULL)
        return;

    if (_ribbonBar->isBackstageVisible())
    {
        if (RibbonSystemButton* button = _ribbonBar->systemButton())
        {
            if (RibbonBackstageView* backstage = qobject_cast<RibbonBackstageView*>(button->backstage()))
                backstage->close();
        }
    }

    if (_ribbonBar->isMinimized())
    {
        RibbonBarPrivate* barprivate = RibbonBarPrivate::_get(_ribbonBar);
        if (selectionChanged || !barprivate->isPagePopupVisible())
            barprivate->showPagePopup(page());
        else
            barprivate->hidePagePopup();
    }
}

void RibbonTab::dblClicked()
{
    AbstractTab::dblClicked();
    RibbonBar* _ribbonBar = tabBar()->ribbonBar();
    if (_ribbonBar != Q_NULL)
        RibbonBarPrivate::_get(_ribbonBar)->toggledMinimized();
}

/*! \reimp */
void RibbonTab::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QTN_D(RibbonTab);

    QPainter p(this);
    p.setClipRegion(event->region());

    RibbonOptionHeaderStyleOption opt;
    initStyleOption(&opt);
    opt.firstTab = contextualTab() != Q_NULL && contextualTab()->firstTab == this;
    opt.lastTab = contextualTab() != Q_NULL && contextualTab()->lastTab == this;
    opt.rcFirst = contextualTab() != Q_NULL ? contextualTab()->firstTab->rect() : QRect();
    opt.rcLast = contextualTab() != Q_NULL ? contextualTab()->lastTab->rect() : QRect();
    opt.textAlignment = d.m_align;
    opt.contextColor = contextColor();

    const bool isHighlight = opt.state & QStyle::State_MouseOver;

    bool minimized = false;
    bool backstageVisible = false;
    RibbonTabBar* _tabBar = this->tabBar();
    RibbonBar* _ribbonBar = _tabBar->ribbonBar();
    opt.tabBarPosition = _ribbonBar->tabBarPosition();

    minimized = _ribbonBar->isMinimized();
    opt.minimized = minimized;
    backstageVisible = _ribbonBar->isBackstageVisible();

    int max = m_maxSize;
    int w = opt.rect.width();
    if (w < max)
        opt.state |= QStyle::State_Small;
    else
        opt.state &= ~QStyle::State_Small;

    RibbonPage* _page = page();
    const bool selected = !backstageVisible && isCurrent() && _page != Q_NULL && _page->isVisible();
    if (selected)
    {
        opt.state |= QStyle::State_Selected;
        if (minimized || backstageVisible)
            opt.state |= QStyle::State_Sunken;
    }
    else
    {
        opt.state &= ~QStyle::State_Selected;
        if (minimized || backstageVisible)
            opt.state &= ~QStyle::State_Sunken;
    }

    bool drawControl = true;
    if (minimized)
    {
        RibbonBarPrivate* barprivate = RibbonBarPrivate::_get(_ribbonBar);
        if (!barprivate->isPagePopupVisible())
        {
            drawControl = false;
            opt.state &= ~QStyle::State_Selected;
        }
    }

    if (m_dragOffset != 0)
        opt.rect.moveLeft(opt.rect.x() + m_dragOffset);

    if (!_tabBar->m_startDrag)
    {
        if (isHighlight || drawControl)
            style()->drawControl(static_cast<QStyle::ControlElement>(CommonStyle::CE_RibbonTab), &opt, &p, this);
        style()->drawControl(static_cast<QStyle::ControlElement>(CommonStyle::CE_RibbonTabShapeLabel), &opt, &p, this);
    }
}

/* RibbonTabBar */
RibbonTabBar::RibbonTabBar(RibbonBar* ribbonBar)
    : AbstractTabBar(RibbonBarPrivate::_get(ribbonBar)->material())
{
    setScrollable(true);
//    setAlignmentRightLayout(Qt::AlignRight);
    setElideMode(Qt::ElideRight);
    m_ribbonBar = ribbonBar;
}

RibbonTabBar::~RibbonTabBar()
{
}

RibbonBar* RibbonTabBar::ribbonBar() const
{ return m_ribbonBar; }

RibbonTab* RibbonTabBar::getTab(int index)
{ return qobject_cast<RibbonTab*>(tabAt(index)); }

const RibbonTab* RibbonTabBar::getTab(int index) const
{ return qobject_cast<const RibbonTab*>(tabAt(index)); }

QAction* RibbonTabBar::addAction(const QIcon& icon, const QString& text, Qt::ToolButtonStyle style, QMenu* menu)
{
    QAction* action = new QAction(icon, text, this);
    if (menu != Q_NULL)
        action->setMenu(menu);
    return addAction(action, style);
}

QAction* RibbonTabBar::addAction(QAction* action, Qt::ToolButtonStyle style)
{
    if ( Qt::ToolButtonTextUnderIcon == style )
    {
        Q_ASSERT(false);
        style = Qt::ToolButtonTextBesideIcon;
    }
    QWidget::addAction(action);
    RibbonButton* button = new RibbonButton(this);

    button->setAutoRaise(true);
    button->setFocusPolicy(Qt::NoFocus);
    button->setToolButtonStyle(style);

    action->setIconText(action->text());
    button->setDefaultAction(action);
    button->setVisible(true);
    addWidgetToRightSide(button);
    return action;
}

QMenu* RibbonTabBar::addMenu(const QString& title)
{
    QMenu* menu = new QMenu(title, this);
    RibbonButton* button = new RibbonButton(this);
    button->setAutoRaise(true);
    button->setFocusPolicy(Qt::NoFocus);
    button->setPopupMode(QToolButton::InstantPopup);
    button->setMenu(menu);
    button->setToolButtonStyle(Qt::ToolButtonTextOnly);
    button->setDefaultAction(menu->menuAction());
    button->setVisible(true);
    addWidgetToRightSide(button);
    return menu;
}

void RibbonTabBar::paintTab(QPainter* painter, AbstractTab* tab) const
{
    RibbonOptionHeaderStyleOption opt;
    tab->initStyleOption(&opt);
    opt.textAlignment = RibbonTabPrivate::get(static_cast<RibbonTab *>(tab))->m_align;
    opt.position = QStyleOptionTab::OnlyOneTab;
    style()->drawControl(static_cast<QStyle::ControlElement>(CommonStyle::CE_RibbonTab), &opt, painter, tab);
    style()->drawControl(static_cast<QStyle::ControlElement>(CommonStyle::CE_RibbonTabShapeLabel), &opt, painter, tab);
}

void RibbonTabBar::paintTabBarFrame(QPainter* painter, QStyleOptionTabBarBase* optTabBase)
{
    Q_UNUSED(painter);
    Q_UNUSED(optTabBase);
}

/*! \reimp */
bool RibbonTabBar::event(QEvent* event)
{
    switch(event->type())
    {
        case QEvent::PaletteChange:
        {
        }
        break;
        case QEvent::Wheel:
            {
                if (ribbonBar()->isMinimized())
                {
                    event->ignore();
                    return true;
                }
                break;
            }
        default:
            break;
    }
    return AbstractTabBar::event(event);
}

/*! \reimp */
void RibbonTabBar::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
    QStyleOption opt;
    opt.initFrom(this);
    style()->drawControl(static_cast<QStyle::ControlElement>(CommonStyle::CE_RibbonTabBar), &opt, &p, this);
}

/*! \reimp */
void RibbonTabBar::mouseDoubleClickEvent(QMouseEvent* event)
{
    event->setAccepted(false);
    AbstractTabBar::mouseDoubleClickEvent(event);
}

AbstractTab* RibbonTabBar::createTab(const QIcon& icon, const QString& text, QObject* object)
{
    Q_UNUSED(icon);
    RibbonTab* tab = new RibbonTab(this, text);
    tab->setObject(object);
    RibbonPagePrivate::_get(tab->page())->setAssociativeTab(tab);
    return tab;
}

/*! \reimp */
QSize RibbonTabBar::sizeHint() const
{
    QSize sh = AbstractTabBar::sizeHint();
    return sh;
}
