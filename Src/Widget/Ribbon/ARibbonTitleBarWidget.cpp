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
#include <QPainter>
#include <QLayout>
#include <QEvent>
#include <QStyleOption>
#include <QApplication>
#include <QMainWindow>
#include <QWidgetAction>

#include "QtnPlatform.h"
#include "ARibbonBar.h"
#include "ARibbonBarPrivate.h"
#include "ARibbonBackstageViewPrivate.h"
#include "QtnCommonStyle.h"
#include "QtnCommonStylePrivate.h"
#include "QtnStyleOption.h"
#include "QtnToggleSwitch.h"
#include "QtnStyleHelperPrivate.h"
#include "ARibbonBarPrivate.h"
#include "ARibbonQuickAccessBar.h"
#include "ARibbonTitleBarWidgetPrivate.h"

#ifdef A_MEMORY_DEBUG
#include "QtitanMSVSDebug.h"
#endif


static RibbonQuickAccessBar* aproch_findQuickAccessBar(QLayout* layout)
{
    for (int i = 0; i < layout->count(); ++i)
    {
        QLayoutItem* item = layout->itemAt(i);
        if (RibbonQuickAccessBar* quickAccessBar = qobject_cast<RibbonQuickAccessBar*>(item->widget()))
            return quickAccessBar;
    }
    return nullptr;
}

/*!
    \class RibbonTitleBarWidget
    \internal
*/
RibbonTitleBarWidget::RibbonTitleBarWidget(ARibbonBar* ribbonBar)
    : QWidget(ARibbonBarPrivate::_get(ribbonBar)->material())
    , m_ribbonBar(ribbonBar)
    , m_layout(nullptr)
    , m_searchBarAppearance(ARibbonBar::SearchBarCentral)
    , m_dirtyTextTitle(false)
{
    setAutoFillBackground(false);
    setMouseTracking(true);
    setAttribute(Qt::WA_NoSystemBackground);
    _pTopWidget = topWidget();
    if (_pTopWidget != nullptr)
    {
        connect(_pTopWidget, &QObject::destroyed, this, [&]() {_pTopWidget = nullptr; });
        topWidget()->installEventFilter(this);
    }
    ribbonTabBar()->scrollView()->installEventFilter(this);

    m_searchBar = new RibbonSearchBar(ribbonBar);

    m_layout = new QHBoxLayout(this);
    m_layout->setSpacing(1); 

    m_leftItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_layout->addSpacerItem(m_leftItem);
    
    m_layout->addWidget(m_searchBar, 0);

    m_rightItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_layout->addSpacerItem(m_rightItem);

    //Hide search bar by default.
    setSearchBarAppearance(ARibbonBar::SearchBarHidden);
}

RibbonTitleBarWidget::~RibbonTitleBarWidget()
{
    topWidget()->removeEventFilter(this);
    ribbonTabBar()->scrollView()->removeEventFilter(this);
    removeContextualTabs();
}

void RibbonTitleBarWidget::addWidget(QWidget* widget, int stretch)
{
    if (widget == nullptr)
        return;
    int index = lastLeftWidgetIndex();
    m_layout->insertWidget(index, widget, stretch);
    updateLayout();
}

void RibbonTitleBarWidget::insertWidget(int index, QWidget* widget, int stretch, Qt::Alignment alignment)
{
	if (widget == nullptr || m_layout == nullptr)
		return;
	m_layout->insertWidget(index, widget, stretch);
	updateLayout();
}

int RibbonTitleBarWidget::indexOf(QWidget* widget)
{
	if (widget == nullptr || m_layout == nullptr)
		return -1;

    return m_layout->indexOf(widget);
}

void RibbonTitleBarWidget::addWidgetToRightSide(QWidget* widget, int stretch)
{
    if (widget == nullptr)
        return;
    int index = m_layout->count();
    m_layout->insertWidget(index, widget, stretch);
    updateLayout();
}

void RibbonTitleBarWidget::removeWidget(QWidget* widget)
{
    if (widget == nullptr)
        return;
    m_layout->removeWidget(widget);
    updateLayout();
}

void RibbonTitleBarWidget::adjustSizeTitleBar()
{
}

ARibbonBar* RibbonTitleBarWidget::ribbonBar() const
{
    return m_ribbonBar;
}

RibbonTabBar* RibbonTitleBarWidget::ribbonTabBar() const
{
    return ARibbonBarPrivate::_get(ribbonBar())->m_ribbonTabBar;
}

RibbonSearchBar* RibbonTitleBarWidget::searchBar() const
{
    return m_searchBar;
}

WindowTitleBar* RibbonTitleBarWidget::getWindowTitleBar() const
{
    if (WindowTitleBar* titleBar = ARibbonBarPrivate::_get(m_ribbonBar)->findTitleBar())
    {
        if (titleBar->widget() == this)
            return titleBar;
    }
    return nullptr;
}

class RibbonTitleBarButton : public QToolButton
{
public:
    RibbonTitleBarButton(RibbonTitleBarWidget* titleBarWidget) 
        : QToolButton(), m_titleBarWidget(titleBarWidget) {
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        setFocusPolicy(Qt::WheelFocus);
    }

    ~RibbonTitleBarButton() override {
    }

    void initOption(QStyleOptionTitleBar& option) const
    {
        option.initFrom(this);
        option.titleBarFlags = Qt::WindowShadeButtonHint;
        option.subControls = QStyle::SC_TitleBarShadeButton;
        option.activeSubControls = QStyle::SC_TitleBarShadeButton;

        if (defaultAction() != nullptr)
        {
            option.icon = defaultAction()->icon();
            option.text = defaultAction()->text();
        }
    }

    QSize minimumSizeHint() const override
    {
        return sizeHint();
    }

    QSize sizeHint() const override {
        WindowTitleBar* titleBar = m_titleBarWidget->getWindowTitleBar();
        int h = titleBar != nullptr ? titleBar->titleHeight() : height();
        int w = h;

        const int margin = static_cast<int>(h * 0.25);
        QString text = defaultAction() != nullptr ? defaultAction()->text() : QString();
        if (!text.isEmpty())
        {
            QFontMetrics fm = this->fontMetrics();
            QRect rect = fm.boundingRect(text);
            w += rect.width();
            w += margin;
        }
        return QSize(w, h);
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        Q_UNUSED(event);
        QStyleOptionTitleBar option;
        initOption(option);
        QPainter painter(this);
        style()->drawControl((QStyle::ControlElement)CommonStyle::CE_RibbonTitleBarButton, &option, &painter, this);
    }

    void mousePressEvent(QMouseEvent* event) override {
        QToolButton::mousePressEvent(event);
    }
private:
    RibbonTitleBarWidget* m_titleBarWidget;
};

QAction* RibbonTitleBarWidget::addTitleButton(const QIcon& icon, const QString& help)
{
    QAction* result = new QAction(icon, QString(), nullptr);
    result->setToolTip(help);
    RibbonTitleBarButton* button = new RibbonTitleBarButton(this);
    button->setDefaultAction(result);
    addWidgetToRightSide(button, false);
    return result;
}

void RibbonTitleBarWidget::removeTitleButton(QAction* action)
{
    for (int i = 0; i < m_layout->count(); ++i)
    {
        QLayoutItem* item = m_layout->itemAt(i);
        if (QToolButton* button = qobject_cast<QToolButton*>(item->widget()))
        {
            if (button->defaultAction() == action)
            {
                m_layout->removeItem(item);
                delete button;
                return;
            }
        }
    }
}

void RibbonTitleBarWidget::setSearchBarAppearance(ARibbonBar::SearchBarAppearance appearance)
{
    if (m_searchBarAppearance == appearance)
        return;
    m_searchBarAppearance = appearance;
    if (appearance == ARibbonBar::SearchBarCentral)
    {
        m_rightItem->changeSize(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        m_searchBar->setCompact(false);
        m_searchBar->setVisible(true);
    }
    else if (appearance == ARibbonBar::SearchBarCompact)
    {
        m_rightItem->changeSize(0, 0, QSizePolicy::Minimum, QSizePolicy::Minimum);
        m_searchBar->setCompact(true);
        m_searchBar->setVisible(true);
    }
    else
    {
        m_rightItem->changeSize(0, 0, QSizePolicy::Minimum, QSizePolicy::Minimum);
        m_searchBar->setVisible(false);
    }
    layout()->invalidate();
}

ARibbonBar::SearchBarAppearance RibbonTitleBarWidget::searchBarAppearance() const
{
    return m_searchBarAppearance;
}

QSize RibbonTitleBarWidget::calcMinSize(QWidget* widget) const
{
    const QSize sizeHint = widget->sizeHint();
    const QSize minSizeHint = widget->minimumSizeHint();
    const QSize minSize = widget->minimumSize();
    const QSize maxSize = widget->maximumSize();
    const QSizePolicy sizePolicy = widget->sizePolicy();

    QSize size(0, 0);
    if (sizePolicy.horizontalPolicy() != QSizePolicy::Ignored)
    {
        if (sizePolicy.horizontalPolicy() & QSizePolicy::ShrinkFlag)
            size.setWidth(minSizeHint.width());
        else
            size.setWidth(qMax(sizeHint.width(), minSizeHint.width()));
    }

    if (sizePolicy.verticalPolicy() != QSizePolicy::Ignored)
    {
        if (sizePolicy.verticalPolicy() & QSizePolicy::ShrinkFlag)
            size.setHeight(minSizeHint.height());
        else
            size.setHeight(qMax(sizeHint.height(), minSizeHint.height()));
    }

    size = size.boundedTo(maxSize);
    if (minSize.width() > 0)
        size.setWidth(minSize.width());
    if (minSize.height() > 0)
        size.setHeight(minSize.height());
    return size.expandedTo(QSize(0, 0));
}

QRect RibbonTitleBarWidget::calcTextRect() const
{
    int left = 0;
    int right = width();
    left = qMax(left, m_leftItem->geometry().left());
    right = qMin(right, m_leftItem->geometry().right());

    QRect rcTitleText(left, 0, right - left, height());
    if (m_contextualTabs.count() > 0)
    {
        QRect cr = geometry();
        QRect rcHeaders(QPoint(m_contextualTabs[0]->rect().left(), cr.top()),
            QPoint(m_contextualTabs[m_contextualTabs.count() - 1]->rect().right(), cr.bottom()));

        int leftLength = rcHeaders.left() - rcTitleText.left();
        int rightLength = rcTitleText.right() - rcHeaders.right();
        if (leftLength < rightLength)
        {
            rcTitleText.setLeft(rcHeaders.right());
            rcTitleText.setRight(rcTitleText.right());
        }
        else
        {
            rcTitleText.setRight(rcHeaders.left());
        }
    }
    return rcTitleText;
}

QRect RibbonTitleBarWidget::calcContextualAreaRect() const
{
    int offset = 0;
    if (WindowTitleBar* titleBar = getWindowTitleBar())
        offset = geometry().left() - titleBar->borderThickness();
    int left = offset;
    int right = width();
    left = qMax(left, m_leftItem->geometry().left());
    right = qMin(right, /*m_rightItem*/m_leftItem->geometry().right());
    return QRect(QPoint(left, 0), QPoint(right, height() - 1));
}

int RibbonTitleBarWidget::lastLeftWidgetIndex() const
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
    return m_layout->indexOf(m_leftItem);
#else
    for (int i = 0; i < m_layout->count(); ++i)
        if (m_layout->itemAt(i) == m_leftItem)
            return i;
    return -1;
#endif
}

void RibbonTitleBarWidget::setVisible(bool visible)
{
    QWidget::setVisible(visible);
}

void RibbonTitleBarWidget::updateLayout()
{
    int spacing = 6;
    if (RibbonSystemButton* systemButton = m_ribbonBar->systemButton())
    {
        if (systemButton->toolButtonStyle() == Qt::ToolButtonIconOnly)
            spacing += systemButton->width();
    }
    m_layout->setContentsMargins(QMargins(spacing, 0, 0, 0));

    //m_layout->invalidate();
    for (int i = 0; i < m_layout->count(); ++i)
    {
        QLayoutItem* item = m_layout->itemAt(i);
        if (item->widget() != nullptr)
            item->widget()->updateGeometry(); //Update layout size cache.
    }

    updateContextualTabs();
    update();
}

/*! \internal */
QWidget* RibbonTitleBarWidget::titleBarWindow() const
{
    QWidget* ret = m_ribbonBar->parentWidget();
    if (ret == nullptr)
        return nullptr;
    if (ret->isWindow() || qobject_cast<QMainWindow*>(ret) != nullptr)
        return ret;
    ret = ret->parentWidget();

    if (ret->isWindow() || qobject_cast<QMainWindow*>(ret) != nullptr)
        return ret;
    return nullptr;
}

QWidget* RibbonTitleBarWidget::topWidget() const
{
#ifndef QTITAN_DESIGNER
    if (m_ribbonBar->parentWidget() && m_ribbonBar->parentWidget()->isWindow())
        return m_ribbonBar->parentWidget();
    else
        return m_ribbonBar;
#else
    return m_ribbonBar;
#endif
}

void RibbonTitleBarWidget::updateTextTitle()
{
    if (!m_dirtyTextTitle)
        return;

    const bool ribbonBarEmbeded = parentWidget() == ribbonBar();
    if (ribbonBarEmbeded)
        m_strTitle = QString();
    else
        m_strTitle = titleBarWindow() != nullptr ? titleBarWindow()->windowTitle() : QString();

    m_dirtyTextTitle = false;
}

void RibbonTitleBarWidget::windowTitleChanged(const QString& text)
{
    m_strTitle = text;
    m_dirtyTextTitle = true;
    updateLayout();
}

QFont RibbonTitleBarWidget::titleFont() const
{
    return QApplication::font("QMdiSubWindowTitleBar");
}

void RibbonTitleBarWidget::initTitleBarOption(TitleBarStyleOption* opt) const
{
    opt->initFrom(this);
    if (RibbonSystemButton* button = m_ribbonBar->systemButton())
    {
        if (const RibbonBackstageView* backstage = qobject_cast<const RibbonBackstageView*>(button->backstage()))
        {
            opt->isBackstageVisible = !backstage->isHidden();
            opt->menuWidth = RibbonBackstageViewPrivate::_get(backstage)->m_backstageMenu->width();
            opt->backstageFrameMenuPalette = RibbonBackstageViewPrivate::_get(backstage)->m_backstageMenu->palette();
        }
    }

    if (!opt->isBackstageVisible)
    {
        opt->contextualAreaRect = calcContextualAreaRect();
        //opt->contextualAreaRect.translate(-opt->contextualAreaRect.left(), 0);
        for (int i = 0, count = m_contextualTabs.count(); i < count; ++i)
        {
            ContextualTab* contextualTab = m_contextualTabs[i];
            opt->contextualTabs.append(TitleBarStyleOption::ContextualTabInfo(contextualTab->rect(), contextualTab->title(), contextualTab->color()));
        }
    }

    opt->text = m_strTitle;
    if (searchBarAppearance() == ARibbonBar::SearchBarCentral)
        opt->alignment = Qt::AlignVCenter | Qt::AlignLeft | Qt::TextSingleLine;
    else
        opt->alignment = Qt::AlignCenter | Qt::TextSingleLine;

    opt->titleRect = calcTextRect();
    opt->contextFont = font();
    opt->titleFont = titleFont();
    if (RibbonSystemButton* systemButton = m_ribbonBar->systemButton())
    {
        if (systemButton->toolButtonStyle() == Qt::ToolButtonIconOnly)
        {
            systemButton->initButtonStyleOption(&opt->optButton);
            const int halfHeight = opt->optButton.rect.height() / 2;
            opt->optButton.rect.translate(0, geometry().bottom() - halfHeight);
            if (opt->optButton.state & QStyle::State_Sunken)
                opt->optButton.state |= QStyle::State_Sunken;
        }
    }
}

ContextualTab* RibbonTitleBarWidget::hitContextHeaders(const QPoint& point) const
{
    QRect contextualAreaRect = calcContextualAreaRect();
    for (int i = 0, count = m_contextualTabs.count(); i < count; ++i)
    {
        ContextualTab* contextualTab = m_contextualTabs[i];
        if (contextualTab->rect().intersected(contextualAreaRect).contains(point))
            return contextualTab;
    }
    return nullptr;
}

void RibbonTitleBarWidget::updateContextualTabs()
{
    removeContextualTabs();
    if (!ribbonBar()->isContextualTabsVisible())
        return;
    ContextualTab* prevContextualTab = nullptr;
    RibbonTabBar* tabBar = ribbonTabBar();
    int count = tabBar->count();
    if (count == 0)
        return;

    for (int i = 0; i < count; ++i)
    {
        RibbonTab* tab = tabBar->getTab(i);
        if (tab->isHidden())
            continue;

        if (tab->contextTitle().isEmpty())
        {
            prevContextualTab = nullptr;
            continue;
        }

        if (prevContextualTab != nullptr &&
            !prevContextualTab->groupName().isEmpty() &&
            prevContextualTab->groupName() == tab->contextGroupName())
        {
            prevContextualTab->lastTab = tab;
            tab->setContextualTab(prevContextualTab);
        }
        else
        {
            prevContextualTab = new ContextualTab(ribbonBar(), tab);
            m_contextualTabs.append(prevContextualTab);
        }
    }
}

void RibbonTitleBarWidget::removeContextualTabs()
{
    for (int i = 0; i < m_contextualTabs.count(); i++)
        delete m_contextualTabs[i];
    m_contextualTabs.clear();
}

/*! \reimp */
bool RibbonTitleBarWidget::event(QEvent* event)
{
    switch (event->type())
    {
        case QEvent::ParentChange:
        {
            m_dirtyTextTitle = true;
            break;
        }
        case QEvent::Resize:
            updateLayout();
            break;
#if 0
        case QEvent::PaletteChange:
        {
            QColor c = palette().windowText().color();
            c = c;
        }
        break;
#endif
        case QEvent::StyleChange:
            break;
        default:
            break;
    }
    return QWidget::event(event);
}

bool RibbonTitleBarWidget::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ribbonBar() && event->type() == QEvent::ParentChange)
    {
        if (_pTopWidget != nullptr)
            _pTopWidget->removeEventFilter(this);

		_pTopWidget = topWidget();
        if (_pTopWidget != nullptr)
        {
			connect(_pTopWidget, &QObject::destroyed, this, [&]() {_pTopWidget = nullptr; });
            _pTopWidget->installEventFilter(this);
        }
    }

    if (watched == ribbonTabBar()->scrollView())
    {
        if (event->type() == QEvent::Move)
        {
            //Repaint contextual tabs on scroll the ribbonBar tabs.
            update();
        }
    }
    else if (watched == topWidget())
    {
        if (event->type() == QEvent::WindowTitleChange ||
            event->type() == QEvent::FontChange)
        {
            m_dirtyTextTitle = true;
            m_searchBar->setFont(titleFont());
            updateLayout();
        }
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        else if (event->type() == QEvent::WinIdChange)
        {
            if (QObject* wnd = (QObject*)(topWidget()->windowHandle()))
                QObject::connect(wnd, SIGNAL(windowTitleChanged(const QString&)), this, SLOT(windowTitleChanged(const QString&)));
        }
#endif
    }
    return QWidget::eventFilter(watched, event);
}

/*! \reimp */
void RibbonTitleBarWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    updateTextTitle();
    QPainter painter(this);

    TitleBarStyleOption opt;
    initTitleBarOption(&opt);
    style()->drawComplexControl(static_cast<QStyle::ComplexControl>(CommonStyle::CC_RibbonTitleBarWidget), &opt, &painter, this);

    if (RibbonQuickAccessBar* quickAccessBar = aproch_findQuickAccessBar(m_layout))
    {
        if (quickAccessBar->visibleCount() > 0)
        {
            painter.translate(quickAccessBar->geometry().topLeft());
            quickAccessBar->paintContent(&painter);
        #if 0 //remove me
            RibbonQuickAccessBarStyleOption opt;
            opt.initFrom(accessBar);
            const int hor = CommonStylePrivate::dpiScaled(2, accessBar);
            const int ver = CommonStylePrivate::dpiScaled(1, accessBar);
            opt.rect = accessBar->geometry().adjusted(-hor, -ver, hor, ver);
            opt.quickAccessBarPosition = ARibbonBar::TopPosition;
            RibbonSystemButton* systemButton = ribbonBar()->systemButton();
            opt.roundButton = systemButton && systemButton->toolButtonStyle() == Qt::ToolButtonIconOnly;
            QPalette_setColor(opt.palette, QPalette::Window, accessBar->parentWidget()->palette().color(QPalette::Window));
            style()->drawControl(QStyle::CE_ToolBar, &opt, &painter, accessBar);
        #endif
        }
    }
}

/*! \reimp */
void RibbonTitleBarWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (ContextualTab* contextualTab = hitContextHeaders(event->pos()))
        {
            int index = ribbonTabBar()->tabIndex(contextualTab->firstTab);
            if (index != -1)
            {
                m_ribbonBar->setCurrentPageIndex(index);
                return;
            }
        }
    }
    event->ignore();
}

