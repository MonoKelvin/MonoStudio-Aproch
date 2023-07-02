/****************************************************************************
 * @file    AAbstractTabBar.cpp
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
#include <QToolTip>
#include <QWhatsThis>
#include <QStylePainter>
#include <QMouseEvent>
#include <QApplication>
#include <QWidgetAction>

#include "Style/ACommonStyle.h"
#include "Style/ACommonStyle_p.h"
#include "AAbstractTabBar.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

#define A_ANIMATION_DURATION 250

static void _aproch_initStyleBaseOption(QStyleOptionTabBarBase* optTabBase, AAbstractTabBar* tabbar, const QSize& size)
{
    QStyleOptionTab tabOverlap;
    tabOverlap.shape = static_cast<QTabBar::Shape>(QTabBar::RoundedNorth);
    int overlap = tabbar->style()->pixelMetric(QStyle::PM_TabBarBaseOverlap, &tabOverlap, tabbar);
    QWidget* theParent = tabbar->parentWidget();
    optTabBase->initFrom(tabbar);
    optTabBase->shape = static_cast<QTabBar::Shape>(QTabBar::RoundedNorth);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    optTabBase->documentMode = false;
#endif
    if (theParent && overlap > 0)
    {
        QRect rect;
        rect.setRect(0, size.height() - overlap, size.width(), overlap);
        optTabBase->rect = rect;
    }
}

class ADragTabAnimation : public QVariantAnimation
{
public:
    ADragTabAnimation(AAbstractTab* tab, AAbstractTabBar* tabBar) : m_tab(tab), m_privatTabBar(tabBar) {
        setEasingCurve(QEasingCurve::InOutQuad);
    }
    ~ADragTabAnimation() override {}
    void updateCurrentValue(const QVariant& current) override { 
        if (state() == QVariantAnimation::Running)
            m_privatTabBar->moveTabOffset(m_privatTabBar->m_tabList.indexOf(m_tab), current.toInt()); 
    }
    void updateState(State newState, State) override{ 
        if (newState == Stopped)
            m_privatTabBar->moveTabStopped(m_privatTabBar->m_tabList.indexOf(m_tab));
    }
private:
    AAbstractTab* m_tab;
    AAbstractTabBar* m_privatTabBar;
};

/* AMovableTabWidget */
class AMovableTabWidget : public QWidget
{
public:
    explicit AMovableTabWidget(QWidget* parent = nullptr);
    void initialize(AAbstractTab* tab);
protected:
    virtual void paintEvent(QPaintEvent* event);
private:
    QPixmap m_pixmap;
private:
    Q_DISABLE_COPY(AMovableTabWidget)
};

/* AMovableTabWidget */
AMovableTabWidget::AMovableTabWidget(QWidget* parent)
    : QWidget(parent)
{
    setFont(parent->font());
}

void AMovableTabWidget::initialize(AAbstractTab* tab)
{
    if (tab == nullptr)
    {
        Q_ASSERT(false);
        return;
    }
    QRect rect = tab->tabBar()->tabRect(tab->index());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    QPixmap pixmap(rect.size() * devicePixelRatioF());
    pixmap.setDevicePixelRatio(devicePixelRatioF());
#else
    QPixmap pixmap(grabRect.size());
#endif
    pixmap.fill(Qt::transparent);

    QStylePainter painter(&pixmap, tab);
    painter.setFont(tab->font());
    tab->tabBar()->paintTab(&painter, tab);
    m_pixmap = pixmap;
    setGeometry(rect);
    update();
}

void AMovableTabWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.drawPixmap(0, 0, m_pixmap);
}

class AScrollContainer : public AAbstractScrollWidgetBar
{
public:
    explicit AScrollContainer(AAbstractTabBar* parent);
public:
    QPoint viewOffset() const;
    int getColumnCount() const override;
    QSize getColumnSize(int column) const override;
    void updateItemsLayout() override;
protected:
    void wheelEvent(QWheelEvent* event) override;
protected:
#if 0
    virtual void paintEvent(QPaintEvent* event)
    {
        Q_UNUSED(event);
        QPainter p(this);
        p.setPen(Qt::blue);
        p.drawRect(rect().adjusted(0, 0, -1, -1));
    }
#endif
    AAbstractTabBar* m_tabBar;
private:
    friend class AAbstractTabBar;
    Q_DISABLE_COPY(AScrollContainer);
};

AScrollContainer::AScrollContainer(AAbstractTabBar* parent)
    : AAbstractScrollWidgetBar(parent)
    , m_tabBar(parent)
{
    setFocusPolicy(Qt::NoFocus);
    setPageIndent(0);
    setItemIndent(0);
    setMargins(QMargins(0, 0, 0, 0));
    setMoveType(MoveByItem);
}

QPoint AScrollContainer::viewOffset() const
{ 
    return getViewOffset();
}

int AScrollContainer::getColumnCount() const
{ 
    return m_tabBar->visibleTabs().count();
}

QSize AScrollContainer::getColumnSize(int column) const
{
    if (column < 0 || column >= getColumnCount())
        return QSize();
    const AAbstractTab* tab = m_tabBar->visibleTabs()[column];
    QSize ret = tab->size();
    return ret;
}

void AScrollContainer::updateItemsLayout()
{
}

void AScrollContainer::wheelEvent(QWheelEvent* event)
{
    QRect rect = geometry();
    QRect rectTab = m_tabBar->tabRect(m_tabBar->currentIndex());
    QPoint pnt = mapToParent(rectTab.topLeft());
    rectTab.moveTo(pnt);
    rectTab.adjust(0, 1, 0, -1);
    if (!rect.contains(rectTab))
        AAbstractScrollWidgetBar::wheelEvent(event);
    else
        event->ignore();
}

/* AAbstractTab */
AAbstractTab::AAbstractTab(AAbstractTabBar* bar, const QIcon& icon, const QString& txt)
    : QWidget(bar)
    , m_shortcutId(0)
    , m_text(txt)
    , m_icon(icon)
    , m_hoverClose(false)
    , m_pressClose(false)
    , m_lastTab(-1)
    , m_dragOffset(0)
    , m_animation(nullptr)
    , m_tabBar(bar)
    , m_object(nullptr)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
    updateShortcut();
}

AAbstractTab::~AAbstractTab()
{
    A_DELETE_AND_NULL(m_animation);
}

void AAbstractTab::startAnimation(AAbstractTabBar* tabBar, int duration)
{
    if (!tabBar->isAnimated())
    {
        tabBar->moveTabStopped(tabBar->m_tabList.indexOf(this));
        return;
    }
    if (m_animation == nullptr)
        m_animation = new ADragTabAnimation(this, tabBar);
    m_animation->setStartValue(m_dragOffset);
    m_animation->setEndValue(0);
    m_animation->setDuration(duration);
    m_animation->start();
}

AAbstractTabBar* AAbstractTab::tabBar() const
{ 
    return m_tabBar;
}

int AAbstractTab::index() const
{
    return tabBar()->tabIndex(this);
}

QObject* AAbstractTab::object() const 
{ 
    return m_object;
}

void AAbstractTab::setObject(QObject* obj) 
{
    m_object = obj;
}

void AAbstractTab::setText(const QString& text)
{
    if (m_text == text)
        return;
    m_text = text;
    updateShortcut();

    AAbstractTabBar* _tabBar = tabBar();
    if (_tabBar != nullptr)
        _tabBar->updateLayout(true);
}

void AAbstractTab::clicked()
{
    AAbstractTabBar* _tabBar = tabBar();
    if (_tabBar != nullptr)
        _tabBar->setCurrentIndex(_tabBar->tabIndex(this));
}

void AAbstractTab::dblClicked() {}

void AAbstractTab::initStyleOption(AStyleOptionTab* option) const
{
    initBasicStyleOption(option);
    AAbstractTabBar* _tabBar = tabBar();
    QRect textRect = style()->subElementRect(QStyle::SE_TabBarTabText, option, this);
    QString tmp3 = option->text;
    QString tmp = option->text;
    tmp.remove(QChar(L'&'));
    QString tmp2 = fontMetrics().elidedText(tmp, _tabBar->m_elideMode, textRect.width(), Qt::TextShowMnemonic);
    if (tmp2 != tmp)
    {
        option->text = fontMetrics().elidedText(option->text, _tabBar->m_elideMode, textRect.width(), Qt::TextShowMnemonic);
    }
}

void AAbstractTab::initBasicStyleOption(AStyleOptionTab* option) const
{
    AAbstractTabBar* _tabBar = tabBar();
    int tabIndex = _tabBar->tabIndex(this);
    const int count = _tabBar->count();
    if (!option || (tabIndex < 0 || tabIndex >= count))
        return;
    option->initFrom(this);
    option->state &= ~(QStyle::State_HasFocus/* | QStyle::State_MouseOver*/);
    const bool isCurrent = tabIndex == _tabBar->m_currentIndex;
    option->row = 0;
    if (tabIndex == _tabBar->m_pressedIndex)
        option->state |= QStyle::State_Sunken;
    if (isCurrent)
        option->state |= QStyle::State_Selected;
    if (isCurrent && hasFocus())
        option->state |= QStyle::State_HasFocus;
    if (!isEnabled())
        option->state &= ~QStyle::State_Enabled;
    if (isActiveWindow())
        option->state |= QStyle::State_Active;
    option->shape = static_cast<QTabBar::Shape>(QTabBar::RoundedNorth);
    option->text = m_text;

    if (m_textColor.isValid())
        QPalette_setColor(option->palette, foregroundRole(), m_textColor);
    option->icon = m_icon;
    option->iconSize = _tabBar->iconSize();

    if (_tabBar->m_tabsClosable)
    {
        option->hoverClose = m_hoverClose;
        int width = style()->pixelMetric(QStyle::PM_TabCloseIndicatorWidth, nullptr, this);
        int height = style()->pixelMetric(QStyle::PM_TabCloseIndicatorHeight, nullptr, this);
        option->rightButtonSize = QSize(width, height);
    }
    else
        option->rightButtonSize = QSize();

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    option->documentMode = false;
#endif
    if (tabIndex > 0 && tabIndex - 1 == _tabBar->m_currentIndex)
        option->selectedPosition = QStyleOptionTab::PreviousIsSelected;
    else if (tabIndex + 1 < count && tabIndex + 1 == _tabBar->m_currentIndex)
        option->selectedPosition = QStyleOptionTab::NextIsSelected;
    else
        option->selectedPosition = QStyleOptionTab::NotAdjacent;

    const bool paintBeginning = (tabIndex == _tabBar->m_beginVisible) || (_tabBar->m_startDrag && tabIndex == _tabBar->m_pressedIndex + 1);
    const bool paintEnd = (tabIndex == _tabBar->m_endVisible) || (_tabBar->m_startDrag && tabIndex == _tabBar->m_pressedIndex - 1);
    if (paintBeginning)
    {
        if (paintEnd)
            option->position = QStyleOptionTab::OnlyOneTab;
        else
            option->position = QStyleOptionTab::Beginning;
    }
    else if (paintEnd)
        option->position = QStyleOptionTab::End;
    else
        option->position = QStyleOptionTab::Middle;
    option->tabIndex = tabIndex;
}

/*! \reimp */
QSize AAbstractTab::sizeHint() const
{
    AAbstractTabBar* _tabBar = tabBar();
    AStyleOptionTab opt;
    initBasicStyleOption(&opt);
    opt.text = m_text;
    QSize iconSize = m_icon.isNull() ? QSize(0, 0) : opt.iconSize;
    int hframe = style()->pixelMetric(QStyle::PM_TabBarTabHSpace, &opt, _tabBar);
    int vframe = style()->pixelMetric(QStyle::PM_TabBarTabVSpace, &opt, _tabBar);
    const QFontMetrics fm = _tabBar->fontMetrics();
    int maxWidgetHeight = qMax(opt.leftButtonSize.height(), opt.rightButtonSize.height());
    int widgetWidth = 0;
    int widgetHeight = 0;
    int padding = 0;
    if (!opt.rightButtonSize.isEmpty())
    {
        padding += 4;
        widgetWidth += opt.rightButtonSize.width();
        widgetHeight += opt.rightButtonSize.height();
    }
    if (!opt.icon.isNull())
        padding += 4;

    const int textWidth = fm.size(Qt::TextShowMnemonic, m_text).width();
    QSize size = QSize(textWidth + iconSize.width() + hframe + widgetWidth + padding,
        qMax(maxWidgetHeight, qMax(fm.height(), iconSize.height())) + vframe);
    return style()->sizeFromContents(QStyle::CT_TabBarTab, &opt, size, _tabBar);
}

QSize AAbstractTab::minimumSizeHint() const
{
    QString saveText = m_text;
    AAbstractTab* tab = const_cast<AAbstractTab*>(this);
    tab->m_text = QStringLiteral("XX...");
    QSize size = tab->AAbstractTab::sizeHint();
    tab->m_text = saveText;
    return size;
}

bool AAbstractTab::isCurrent() const
{
    return this == tabBar()->currentTab();
}


/*! \reimp */
void AAbstractTab::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    AAbstractTabBar* _tabBar = tabBar();
    QStylePainter p(this);
    AStyleOptionTab tab;
    initStyleOption(&tab);

    const bool isCurrent = (_tabBar->m_currentIndex == _tabBar->tabIndex(this));
    if (!isCurrent)
    {
        if (_tabBar->m_startDrag)
        {
            tab.rect.moveLeft(tab.rect.x() + m_dragOffset);
            tab.previewRect.moveLeft(tab.previewRect.x() + m_dragOffset);
        }
        p.drawControl(QStyle::CE_TabBarTab, tab);
    }
    else
    {
        if (!_tabBar->m_startDrag)
            p.drawControl(QStyle::CE_TabBarTab, tab);
    }
}

/*! \reimp */
void AAbstractTab::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        clicked();

    if (m_hoverClose)
    {
        m_pressClose = true;
        event->accept();
        return;
    }
    else
        QWidget::mousePressEvent(event);
}

void AAbstractTab::mouseMoveEvent(QMouseEvent* event)
{
    AAbstractTabBar* _tabBar = tabBar();
    if (_tabBar->m_tabsClosable)
    {
        AStyleOptionTab opt;
        initStyleOption(&opt);
        bool saveHoverClose = m_hoverClose;
        m_hoverClose = false;
        QRect rect = style()->subElementRect(QStyle::SE_TabBarTabRightButton, &opt, this);
        if (rect.contains(event->pos()))
        {
            m_hoverClose = true;
            update(rect);
            event->ignore();
            return;
        }
        if (m_hoverClose != saveHoverClose)
            update(rect);
    }
    QWidget::mouseMoveEvent(event);
}

/*! \reimp */
void AAbstractTab::mouseReleaseEvent(QMouseEvent* event)
{
    QWidget::mouseReleaseEvent(event);
    if (event->button() != Qt::LeftButton)
        return;
    update();
    if (m_pressClose && m_hoverClose)
    {
        m_hoverClose = false;
        AAbstractTabBar* _tabBar = tabBar();
        _tabBar->closeButtonPressed(_tabBar->tabIndex(this));
        event->accept();
    }
}

/*! \reimp */
void AAbstractTab::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        dblClicked();
        event->accept();
        return;
    }
    QWidget::mousePressEvent(event);
}

/*! \reimp */
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
void AAbstractTab::enterEvent(QEnterEvent* event)
{
    QWidget::enterEvent(event);
    update();
}
#else
void AAbstractTab::enterEvent(QEvent* event)
{
    QWidget::enterEvent(event);
}
#endif

/*! \reimp */
void AAbstractTab::leaveEvent(QEvent* event)
{
    QWidget::leaveEvent(event);
    m_hoverClose = false;
    update();
}

/*! \reimp */
bool AAbstractTab::event(QEvent* event)
{
    if (event->type() == QEvent::HoverMove)
        update();
    return QWidget::event(event);
}

int AAbstractTab::shortcutId() const 
{
    return m_shortcutId;
}

void AAbstractTab::updateShortcut()
{
    if (tabBar() == nullptr)
        return;
    if (m_shortcutId != 0)
        tabBar()->releaseShortcut(m_shortcutId);
    m_shortcutId = tabBar()->grabShortcut(QKeySequence::mnemonic(m_text));
    if (m_shortcutId != 0)
        tabBar()->setShortcutEnabled(m_shortcutId, isEnabled());
}

/* ALayoutWidget */
class ALayoutWidget : public QWidget
{
public:
    explicit ALayoutWidget(QWidget* parent);
    virtual ~ALayoutWidget();
public:
    void setAlignmentLayout(Qt::Alignment alignment);
    void addWidget(QWidget* widget);
    void removeWidget(QWidget* widget);
public:
    virtual QSize sizeHint() const;
protected:
    virtual void paintEvent(QPaintEvent* event)
    {
        Q_UNUSED(event);
#if 0
        QPainter painter(this);
        painter.fillRect(rect(), QColor(255, 0, 0));
        painter.drawRect(rect().adjusted(0, 0, -1, -1));
#endif
    }
private:
    QHBoxLayout* m_layout;
private:
    Q_DISABLE_COPY(ALayoutWidget)
};

/* ALayoutWidget */
ALayoutWidget::ALayoutWidget(QWidget* parent) 
    : QWidget(parent)
{
    m_layout = new QHBoxLayout(this);
    m_layout->setDirection(QBoxLayout::RightToLeft);
    m_layout->setSpacing(0);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->addStretch(0);
}

ALayoutWidget::~ALayoutWidget()
{
}

void ALayoutWidget::setAlignmentLayout(Qt::Alignment alignment)
{
    Q_ASSERT(alignment == Qt::AlignLeft || alignment == Qt::AlignRight);
    if (alignment == Qt::AlignLeft)
        m_layout->setDirection(QBoxLayout::RightToLeft);
    else if (alignment == Qt::AlignRight)
        m_layout->setDirection(QBoxLayout::LeftToRight);
}

void ALayoutWidget::addWidget(QWidget* widget)
{
    m_layout->addWidget(widget);
}

void ALayoutWidget::removeWidget(QWidget* widget)
{
    m_layout->removeWidget(widget);
}

QSize ALayoutWidget::sizeHint() const { return  QWidget::sizeHint(); }

void AAbstractTabBar::init()
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_elideMode = Qt::TextElideMode(style()->styleHint(QStyle::SH_TabBar_ElideMode, nullptr, this));
}

void AAbstractTabBar::calcFirstLastVisible(int index, bool visible, bool remove)
{
    if (visible)
    {
        m_beginVisible = qMin(index, m_beginVisible);
        m_endVisible = qMax(index, m_endVisible);
    }
    else
    {
        if (remove || (index == m_beginVisible))
        {
            m_beginVisible = -1;
            for (int i = 0; i < m_tabList.count(); ++i)
            {
                if (!m_tabList.at(i)->isHidden())
                {
                    m_beginVisible = i;
                    break;
                }
            }
            if (m_beginVisible < 0)
                m_beginVisible = 0;
        }
        if (remove || (index == m_endVisible))
        {
            m_endVisible = -1;
            for (int i = m_tabList.count() - 1; i >= 0; --i)
            {
                if (!m_tabList.at(i)->isHidden())
                {
                    m_endVisible = i;
                    break;
                }
            }
        }
    }
}

int AAbstractTabBar::selectNewCurrentIndexFrom(int fromIndex)
{
    int newindex = -1;
    for (int i = fromIndex; i < m_tabList.count(); ++i)
    {
        if (!tabAt(i)->isHidden() && tabAt(i)->isEnabled())
        {
            newindex = i;
            break;
        }
    }
    if (newindex < 0)
    {
        for (int i = fromIndex - 1; i > -1; --i)
        {
            if (!tabAt(i)->isHidden() && tabAt(i)->isEnabled())
            {
                newindex = i;
                break;
            }
        }
    }
    return newindex;
}

int AAbstractTabBar::calcPosition(int from, int to, int index) const
{
    if (index == from)
        return to;
    int start = qMin(from, to);
    int end = qMax(from, to);
    if (index >= start && index <= end)
        index += (from < to) ? -1 : 1;
    return index;
}

void AAbstractTabBar::moveTabPosition(int from, int to)
{
    if (from == to || !isIndexValid(from) || !isIndexValid(to))
        return;
    int preLocation = tabRect(from).x();
    int oldPressedPosition = 0;
    if (m_pressedIndex != -1)
        oldPressedPosition = m_tabList[m_pressedIndex]->geometry().x();

    int start = qMin(from, to);
    int end = qMax(from, to);
    int width = m_tabList[from]->geometry().width();
    if (from < to)
        width *= -1;

    bool rtl = isRightToLeft();
    for (int i = start; i <= end; ++i)
    {
        if (i == from)
            continue;

        QRect rcMove = m_tabList[i]->geometry();
        rcMove.moveLeft(m_tabList[i]->geometry().x() + width);
        m_tabList[i]->setGeometry(rcMove);

        int direction = -1;
        if (rtl)
            direction *= -1;
        if (m_tabList[i]->m_dragOffset != 0)
            m_tabList[i]->m_dragOffset += (direction * width);
    }

    QRect rcMoveFrom = m_tabList[from]->geometry();
    if (from < to)
        rcMoveFrom.moveLeft(m_tabList[to]->geometry().right() + 1);
    else
        rcMoveFrom.moveLeft(m_tabList[to]->geometry().left() - width);

    m_tabList[from]->setGeometry(rcMoveFrom);

    m_tabList.move(from, to);

    for (int i = 0; i < m_tabList.count(); ++i)
        m_tabList[i]->m_lastTab = calcPosition(from, to, m_tabList[i]->m_lastTab);

    m_currentIndex = calcPosition(from, to, m_currentIndex);
    if (m_pressedIndex != -1)
    {
        m_pressedIndex = calcPosition(from, to, m_pressedIndex);
        int newPressedPosition = m_tabList[m_pressedIndex]->geometry().left();
        int diff = oldPressedPosition - newPressedPosition;
        if (isRightToLeft())
            diff *= -1;
        m_pntStartDrag.setX(m_pntStartDrag.x() - diff);
    }
    int postLocation = tabRect(to).x();
    int length = postLocation - preLocation;
    m_tabList[to]->m_dragOffset -= length;
    m_tabList[to]->startAnimation(this, A_ANIMATION_DURATION);
}

AAbstractTab* AAbstractTabBar::tabAt(int index)
{ return isIndexValid(index) ? m_tabList[index] : nullptr; }

const AAbstractTab* AAbstractTabBar::tabAt(int index) const
{ return isIndexValid(index) ? m_tabList[index] : nullptr; }

int AAbstractTabBar::tabIndex(const AAbstractTab* tab) const
{
    Q_ASSERT(tab != nullptr);
    for (int i = 0, count = m_tabList.count(); count > i; i++)
    {
        if (m_tabList[i] == tab)
            return i;
    }
    return -1;
}

QRect AAbstractTabBar::tabRect(int index) const
{
    if (const AAbstractTab* tab = tabAt(index))
    {
        Q_ASSERT(!m_layoutDirty);
        if (tab->isHidden())
            return QRect();
        QRect rc = tab->geometry();
        rc = QStyle::visualRect(layoutDirection(), rect(), rc);
        if (m_scrollContainer != nullptr)
            rc.translate(m_scrollContainer->viewOffset());
        return rc;
    }
    return QRect();
}

int AAbstractTabBar::indexAtPos(const QPoint& pnt) const
{
    if (tabRect(m_currentIndex).contains(pnt))
        return m_currentIndex;
    for (int i = 0; i < m_tabList.count(); ++i)
        if (m_tabList.at(i)->isEnabled() && tabRect(i).contains(pnt))
            return i;
    return -1;
}

int AAbstractTabBar::tabAt(const QPoint& position) const
{
    if (isIndexValid(m_currentIndex) && tabRect(m_currentIndex).contains(position))
        return m_currentIndex;
    const int max = m_tabList.size();
    for (int i = 0; i < max; ++i)
    {
        if (tabRect(i).contains(position))
            return i;
    }
    return -1;
}

AAbstractTabBar::SelectionBehavior AAbstractTabBar::selectionBehaviorOnRemove() const
{ 
    return m_selectionBehaviorOnRemove; 
}

void AAbstractTabBar::setSelectionBehaviorOnRemove(AAbstractTabBar::SelectionBehavior behavior)
{ 
    m_selectionBehaviorOnRemove = behavior;
}

const QList<AAbstractTab*>& AAbstractTabBar::visibleTabs() const
{
    return m_visibleTabList;
}

void AAbstractTabBar::updateLayout(bool force)
{
    if (!force && !m_layoutDirty)
        return;
    m_layoutDirty = false;
    m_visibleTabList.clear();
    int sumWidth = 0, sumCount = 0;
    QSize sh = sizeHint();
    for (int i = 0; i < m_tabList.count(); ++i)
    {
        AAbstractTab* tab = m_tabList[i];
        if (tab->isHidden())
            continue;
        m_visibleTabList.append(tab);
        QSize size = tab->sizeHint();
        tab->m_maxSize = size.width();
        sumWidth += size.width();
        size = elideMode() != Qt::ElideNone ? tab->minimumSizeHint() : QSize(tab->m_maxSize, 0);
        tab->m_minSize = size.width();
        sumCount++;
    }

    QRect rc = this->rect();
    if (m_customWidgetLayout != nullptr)
    {
        if (m_layoutDirection == QBoxLayout::LeftToRight)
        {
            int widthItem = 0;
            for (int i = 0; i < m_items.size(); ++i)
            {
                STabBarItem* item = m_items.at(i);
                if (item == nullptr)
                    break;
                if (!item->widget->isHidden())
                    widthItem += item->widget->sizeHint().width();
            }
            if (rc.width() > sumWidth + widthItem)
                rc.setWidth(sumWidth + widthItem);
            m_customWidgetLayout->setGeometry(rc);
        }
        else if (m_layoutDirection == QBoxLayout::RightToLeft)
            m_customWidgetLayout->setGeometry(rc);
    }

    QRect rect = calcCenterRect();
    int newWidth = 0;
    int available = rect.width();
    if (sumCount > 0 && available < sumWidth)
        newWidth = qRound(double(sumWidth - available) / sumCount) + 1;

    int leftOffset = 0;
    for (int i = 0; i < m_visibleTabList.count(); ++i)
    {
        AAbstractTab* tab = m_visibleTabList[i];
        QRect tabRect = QRect(QPoint(leftOffset, 0), QSize(tab->m_maxSize, sh.height()));
        if (newWidth > 0)
            tabRect.setWidth(qMax(tab->m_minSize, tabRect.width() - newWidth));
        tab->setGeometry(tabRect);
        leftOffset += tabRect.width();
    }

    if (m_scrollContainer != nullptr)
    {
        if (m_scrollContainer->geometry() != rect)
            m_scrollContainer->setGeometry(rect);
        else
            m_scrollContainer->updateLayout();
    }
}

QWidget* AAbstractTabBar::scrollView() const
{
    return m_scrollContainer == nullptr ? const_cast<AAbstractTabBar*>(this) : m_scrollContainer->viewWidget();
}

QSize AAbstractTabBar::calcMinSize(QWidget* widget) const
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

QRect AAbstractTabBar::calcCenterRect() const
{
    int left = 0;
    for (int i = 0; i < m_items.size(); ++i)
    {
        STabBarItem* item = m_items.at(i);
        if (item == nullptr)
            break;
        if (!item->right && !item->widget->isHidden())
            left += item->widget->geometry().right();
    }

    int right = width();
    for (int i = 0; i < m_items.size(); ++i)
    {
        STabBarItem* item = m_items.at(i);
        if (item == nullptr)
            break;
        if (item->right && !item->widget->isHidden())
        {
            right = item->widget->geometry().left();
            break;
        }
    }
    return QRect(left, 0, right - left, height());
}

void AAbstractTabBar::updateALayoutWidgets()
{
    m_layoutDirty = true;
    A_DELETE_AND_NULL(m_customWidgetLayout);
    m_customWidgetLayout = new QHBoxLayout(nullptr);
    m_customWidgetLayout->setContentsMargins(QMargins());

    int i = 0;
    STabBarItem* item = nullptr;
    for (i = 0, item = nullptr; i < m_items.size(); ++i)
    {
        item = m_items.at(i);
        if (!item || item->right)
            break;
        m_customWidgetLayout->addWidget(item->widget, item->stretch, item->alignment);
    }

    m_customWidgetLayout->addStretch(0);

    for (item = nullptr; i < m_items.size(); ++i)
    {
        item = m_items.at(i);
        if (!item)
            break;
        m_customWidgetLayout->addWidget(item->widget, item->stretch, item->alignment);
    }
}

int AAbstractTabBar::indexToLastRightWidget() const
{
    int i = m_items.size() - 1;
    for (; i >= 0; --i)
    {
        STabBarItem* item = m_items.at(i);
        if (!(item && item->right))
            break;
    }
    return i;
}

void AAbstractTabBar::closeButtonPressed(int index)
{
    if (index != -1)
        emit tabCloseRequested(index);
}

void AAbstractTabBar::createMovableTab()
{
    if (m_movableTabWidget == nullptr)
        m_movableTabWidget = new AMovableTabWidget(this);
    m_movableTabWidget->initialize(tabAt(m_pressedIndex));
    m_movableTabWidget->raise();
    m_movableTabWidget->show();
}

void AAbstractTabBar::paintTab(QPainter* painter, AAbstractTab* tab) const
{
    AStyleOptionTab optTab;
    tab->initStyleOption(&optTab);
    optTab.position = QStyleOptionTab::OnlyOneTab;
    style()->drawControl(QStyle::CE_TabBarTab, &optTab, painter, tab);
}

void AAbstractTabBar::refresh()
{
    if (m_pressedIndex != -1 && m_movable &&
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        QGuiApplication::mouseButtons() == Qt::NoButton)
#else
        QApplication::mouseButtons() == Qt::NoButton)
#endif
    {
        moveTabStopped(m_pressedIndex);
        if (!isIndexValid(m_pressedIndex))
            m_pressedIndex = -1;
    }
    m_layoutDirty = true;
    if (isVisible()) 
    {
        updateLayout();
        update();
    }
}

AAbstractTabBar::AAbstractTabBar(QWidget* parent)
    : QWidget(parent)
    , m_currentIndex(-1)
    , m_pressedIndex(-1)
    , m_previousIndex(-1)
    , m_beginVisible(0)
    , m_endVisible(-1)
    , m_layoutDirty(false)
    , m_tabsClosable(false)
    , m_movable(false)
    , m_startDrag(false)
    , m_selectionBehaviorOnRemove(AAbstractTabBar::SelectRightTab)
    , m_movableTabWidget(nullptr)
    , m_scrollContainer(nullptr)
    , m_layoutDirection(QBoxLayout::RightToLeft)
    , m_customWidgetLayout(nullptr)
{
    init();
}

AAbstractTabBar::~AAbstractTabBar()
{
    A_DELETE_AND_NULL(m_customWidgetLayout);
    while (!m_items.isEmpty())
    {
        STabBarItem* item = m_items.takeFirst();
        if (QWidgetAction* widgetAction = qobject_cast<QWidgetAction*>(item->action))
        {
            if (item->customWidget)
                widgetAction->releaseWidget(item->widget);
        }
        A_DELETE_AND_NULL(item);
    }
}

int AAbstractTabBar::addTab(const QString& text, QObject* object)
{
    return insertTab(-1, text, object);
}

int AAbstractTabBar::addTab(const QIcon& icon, const QString& text, QObject* object)
{
    return insertTab(-1, icon, text, object);
}

int AAbstractTabBar::insertTab(int index, const QString &text, QObject* object)
{
    return insertTab(index, QIcon(), text, object);
}

int AAbstractTabBar::insertTab(int index, const QIcon& icon, const QString& text, QObject* object)
{
    QIcon iconTab = icon;
    if (iconTab.isNull())
    {
        QPixmap px = ASegoeMDL2Font::pixmap(ASegoeMDL2Font::PreviewLink, iconSize());
        if (!px.isNull())
            iconTab.addPixmap(px);
    }

    AAbstractTab* tab = createTab(iconTab, text, object);
    tab->setParent(scrollView());
    tab->show();

    if (!isIndexValid(index))
    {
        index = m_tabList.count();
        m_tabList.append(tab);
    }
    else 
    {
        m_tabList.insert(index, tab);
    }

    m_beginVisible = qMax(qMin(index, m_beginVisible), 0);
    m_endVisible = qMax(index, m_endVisible);

    if (m_tabList.count() == 1)
        setCurrentIndex(index);
    else if (index <= m_currentIndex) 
    {
        ++m_currentIndex;
        ++m_endVisible;
    }

    for (int i = 0; i < m_tabList.count(); ++i) 
    {
        if (m_tabList[i]->m_lastTab >= index)
            ++m_tabList[i]->m_lastTab;
    }
    refresh();
    return index;
}

void AAbstractTabBar::removeTab(int index)
{
    if (!isIndexValid(index))
        return;

    if (m_startDrag)
        moveTabStopped(m_pressedIndex);

    AAbstractTab* tab = m_tabList[index];
    m_visibleTabList.removeAll(tab);
    releaseShortcut(tab->m_shortcutId);
    int newIndex = tab->m_lastTab;
    m_tabList.removeAt(index);
    delete tab;

    for (int i = 0; i < m_tabList.count(); ++i) 
    {
        if (m_tabList[i]->m_lastTab == index)
            m_tabList[i]->m_lastTab = -1;
        if (m_tabList[i]->m_lastTab > index)
            --m_tabList[i]->m_lastTab;
    }

    calcFirstLastVisible(index, false, true);

    if (index == m_currentIndex) 
    {
        m_currentIndex = -1; //Reset current index, we will have a new.
        if (m_tabList.size() > 0) 
        {
            switch (m_selectionBehaviorOnRemove) 
            {
                case SelectPreviousTab:
                    if (newIndex > index)
                        newIndex--;
                    if (isIndexValid(newIndex) && !m_tabList.at(newIndex)->isHidden())
                        break;
                    A_FALLTHROUGH();
                case SelectRightTab:
                    newIndex = qBound(m_beginVisible, index, m_endVisible);
                    break;
                case SelectLeftTab:
                    newIndex = qBound(m_beginVisible, index - 1, m_endVisible);
                    if (newIndex < 0)
                        newIndex = 0;
                    break;
                default:
                    break;
            }

            if (isIndexValid(newIndex))
            {
                int bump = m_tabList[newIndex]->m_lastTab;
                setCurrentIndex(newIndex);
                m_tabList[newIndex]->m_lastTab = bump;
            }
        }
        else 
        {
            emit currentChanged(-1);
        }
    }
    else if (index < m_currentIndex) 
        setCurrentIndex(m_currentIndex - 1);

    refresh();
}

bool AAbstractTabBar::isTabEnabled(int index) const
{
    if (const AAbstractTab* tab = tabAt(index))
        return tab->isEnabled();
    return false;
}

void AAbstractTabBar::setTabEnabled(int index, bool enabled)
{
    if (AAbstractTab* tab = tabAt(index))
    {
        if (enabled == tab->isEnabled())
            return;
        tab->setEnabled(enabled);
        setShortcutEnabled(tab->m_shortcutId, enabled);
        if (!enabled && index == m_currentIndex)
            setCurrentIndex(selectNewCurrentIndexFrom(index + 1));
        else if (enabled && !isTabVisible(m_currentIndex))
            setCurrentIndex(selectNewCurrentIndexFrom(index));
    }
}

bool AAbstractTabBar::isTabVisible(int index) const
{
    if (const AAbstractTab* tab = tabAt(index))
        return !tab->isHidden();
    return false;
}

void AAbstractTabBar::setTabVisible(int index, bool visible)
{
    if (AAbstractTab* tab = tabAt(index))
    {
        if (visible == !tab->isHidden())
            return;
        tab->setVisible(visible);
        setShortcutEnabled(tab->m_shortcutId, visible);
        calcFirstLastVisible(index, visible, false);
        if (!visible && index == m_currentIndex) 
        {
            const int newindex = selectNewCurrentIndexFrom(index + 1);
            setCurrentIndex(newindex);
        }
        refresh();
    }
}

void AAbstractTabBar::setTabToolTip(int index, const QString& tip)
{
    if (AAbstractTab* tab = tabAt(index))
        tab->m_toolTip = tip;
}

QString AAbstractTabBar::tabToolTip(int index) const
{
    if (const AAbstractTab* tab = tabAt(index))
        return tab->m_toolTip;
    return QString();
}

int AAbstractTabBar::currentIndex() const
{
    if (isIndexValid(m_currentIndex))
        return m_currentIndex;
    return -1;
}

const AAbstractTab* AAbstractTabBar::currentTab() const
{
    int index = currentIndex();
    if (index != -1)
        return tabAt(index);
    return nullptr;
}

void AAbstractTabBar::setCurrentIndex(int index)
{
    if (m_currentIndex == index)
        return;

    if (m_startDrag && m_pressedIndex != -1)
        return;

    bool changed = true;
    emit currentAboutToBeChanged(index, changed);
    if (!changed)
        return;

    update();

    int oldIndex = m_currentIndex;
    if (isIndexValid(index))
    {
        m_currentIndex = index;
        m_tabList[index]->m_lastTab = oldIndex;
        emit currentChanged(index);
    }

    if (m_scrollContainer != nullptr)
        m_scrollContainer->ensureVisible(m_currentIndex);
}

QSize AAbstractTabBar::iconSize() const
{
    if (m_iconSize.isValid())
        return m_iconSize;
    int iconExtent = style()->pixelMetric(QStyle::PM_TabBarIconSize, nullptr, this);
    return QSize(iconExtent, iconExtent);

}

void AAbstractTabBar::setIconSize(const QSize &size)
{
    m_iconSize = size;
    updateLayout(true);
    update();
}

int AAbstractTabBar::count() const
{
    return m_tabList.count();
}

/*! \reimp */
bool AAbstractTabBar::event(QEvent* event)
{
    if (event->type() == QEvent::ToolTip) 
    {
        if (const AAbstractTab* tab = tabAt(tabAt(static_cast<QHelpEvent*>(event)->pos())))
        {
            if (!tab->m_toolTip.isEmpty()) 
            {
                QToolTip::showText(static_cast<QHelpEvent*>(event)->globalPos(), tab->m_toolTip, this);
                return true;
            }
        }
    }
    else if (event->type() == QEvent::Shortcut) 
    {
        QShortcutEvent* se = static_cast<QShortcutEvent *>(event);
        for (int i = 0; i < m_tabList.count(); ++i) 
        {
            const AAbstractTab* tab = m_tabList.at(i);
            if (tab->shortcutId() == se->shortcutId())
            {
                setCurrentIndex(i);
                event->accept();
                return true;
            }
        }
    }
    return QWidget::event(event);
}

/*! \reimp */
void AAbstractTabBar::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    updateLayout(true);
    if (m_scrollContainer != nullptr)
        m_scrollContainer->moveTo(currentIndex());
}

/*! \reimp */
void AAbstractTabBar::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);
#if 0
    if (m_layoutDirty)
        refresh();
    if (!isIndexValid(m_currentIndex))
        setCurrentIndex(0);
#endif
}

/*! \reimp */
void AAbstractTabBar::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QStyleOptionTabBarBase option;
    _aproch_initStyleBaseOption(&option, this, size());

    QPainter p(this);
    int selected = m_currentIndex;
    if (m_startDrag)
        selected = m_pressedIndex;
    for (int i = 0; i < m_tabList.count(); ++i)
        option.tabBarRect |= tabRect(i);
    option.selectedTabRect = tabRect(selected);

    paintTabBarFrame(&p, &option);

    if (selected >= 0)
    {
        QRect tabRect = this->tabRect(selected);
        if (m_tabList[selected]->m_dragOffset != 0)
            tabRect.moveLeft(tabRect.x() + m_tabList[selected]->m_dragOffset);
        if (m_startDrag)
        {
            int taboverlap = style()->pixelMetric(QStyle::PM_TabBarTabOverlap, nullptr, this);
            m_movableTabWidget->setGeometry(tabRect.adjusted(-taboverlap, 0, taboverlap, 0));
        }
    }
}

void AAbstractTabBar::moveTab(int from, int to)
{
    if (from == to || !isIndexValid(from) || !isIndexValid(to))
        return;
    m_tabList.move(from, to);
    int previousIndex = m_currentIndex;
    m_currentIndex = calcPosition(from, to, m_currentIndex);
    update();
    emit tabMoved(from, to);
    if (previousIndex != m_currentIndex)
        emit currentChanged(m_currentIndex);
}

void AAbstractTabBar::moveTabOffset(int index, int offset)
{
    if (!isIndexValid(index))
        return;
    m_tabList[index]->m_dragOffset = offset;
    update();
}

/*! \reimp */
void AAbstractTabBar::mousePressEvent(QMouseEvent* event)
{
    event->ignore();
    if (event->button() != Qt::LeftButton) 
        return;

    const QPoint pos = event->pos();
    Q_UNUSED(pos);
    if (m_pressedIndex != -1 && m_movable)
        moveTabStopped(m_pressedIndex);

    const int pressedIndex = indexAtPos(event->pos());
    if (!isIndexValid(pressedIndex))
        return;

    event->accept();
    m_pressedIndex = pressedIndex;
    if (m_movable)
    {
        m_pntStartDrag = event->pos();
        m_previousIndex = m_pressedIndex;
    }
}

/*! \reimp */
void AAbstractTabBar::mouseMoveEvent(QMouseEvent* event)
{
    if (m_movable) 
    {
        if (m_pressedIndex != -1 && event->buttons() == Qt::NoButton)
            moveTabStopped(m_pressedIndex);
        if (!m_startDrag && m_pressedIndex != -1)
        {
            if ((event->pos() - m_pntStartDrag).manhattanLength() > QApplication::startDragDistance()) 
            {
                m_startDrag = true;
                createMovableTab();
            }
        }

        if (event->buttons() == Qt::LeftButton && m_startDrag && isIndexValid(m_pressedIndex))
        {
            int dragOffset = (event->pos().x() - m_pntStartDrag.x());
            m_tabList[m_pressedIndex]->m_dragOffset = dragOffset;

            QRect startingRect = tabRect(m_pressedIndex);
            startingRect.moveLeft(startingRect.x() + dragOffset);

            int overIndex;
            if (dragOffset < 0)
                overIndex = tabAt(startingRect.topLeft());
            else
                overIndex = tabAt(startingRect.topRight());

            if (overIndex != m_pressedIndex && overIndex != -1) 
            {
                int offset = 1;
                if (isRightToLeft())
                    offset *= -1;
                if (dragOffset < 0)
                {
                    dragOffset *= -1;
                    offset *= -1;
                }
                for (int i = m_pressedIndex; offset > 0 ? i < overIndex : i > overIndex; i += offset) 
                {
                    QRect overIndexRect = tabRect(overIndex);
                    int needsToBeOver = overIndexRect.width() / 2;
                    if (dragOffset > needsToBeOver)
                        moveTabPosition(i + offset, m_pressedIndex);
                }
            }
            update();
        }
    }

    if (event->buttons() != Qt::LeftButton) 
    {
        event->ignore();
        return;
    }
}

/*! \reimp */
void AAbstractTabBar::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) 
    {
        event->ignore();
        return;
    }

    event->accept();

    if (stopAnimation())
    {
        if (m_previousIndex != m_pressedIndex)
            emit tabMoved(m_previousIndex, m_pressedIndex);
    }
    m_pressedIndex = -1;
    m_previousIndex = -1;
}

/*! \reimp */
void AAbstractTabBar::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape && stopAnimation())
    {
        moveTabPosition(m_pressedIndex, m_previousIndex);
        m_pressedIndex = -1;
        m_previousIndex = -1;
        event->ignore();
        return;
    }
    if (event->key() != Qt::Key_Left && event->key() != Qt::Key_Right) 
    {
        event->ignore();
        return;
    }

    event->accept();

    int offset = event->key() == (isRightToLeft() ? Qt::Key_Right : Qt::Key_Left) ? -1 : 1;
    setCurrentNextEnabledIndex(offset);
}

/*! \reimp */
void AAbstractTabBar::wheelEvent(QWheelEvent* event)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
    int delta = qAbs(event->angleDelta().x()) > qAbs(event->angleDelta().y()) ?
        event->angleDelta().x() : event->angleDelta().y();
#else
    int delta = event->delta();
#endif
    int offset = delta > 0 ? -1 : 1;
    setCurrentNextEnabledIndex(offset);
    event->accept();
}

bool AAbstractTabBar::isAnimated() const 
{ 
#if (QT_VERSION >= QT_VERSION_CHECK(5, 11, 0))
    return style()->styleHint(QStyle::SH_Widget_Animation_Duration, nullptr, this) > 0;
#elif (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    return style()->styleHint(QStyle::SH_Widget_Animate, nullptr, this) > 0;
#else
    return false;
#endif
}

bool AAbstractTabBar::stopAnimation()
{
    if (m_movable && m_startDrag && isIndexValid(m_pressedIndex))
    {
        int length = m_tabList[m_pressedIndex]->m_dragOffset;
        int width = tabRect(m_pressedIndex).width();
        int duration = qMin(A_ANIMATION_DURATION, (qAbs(length) * A_ANIMATION_DURATION) / width);
        m_tabList[m_pressedIndex]->startAnimation(this, duration);
        m_startDrag = false;
        m_movableTabWidget->hide();
        m_pntStartDrag = QPoint();
        return true;
    }
    return false;
}

void AAbstractTabBar::setCurrentNextEnabledIndex(int offset)
{
    for (int index = m_currentIndex + offset; isIndexValid(index); index += offset)
    {
        AAbstractTab* tab = m_tabList.at(index);
        if (!tab->isHidden() && tab->isEnabled())
        {
            setCurrentIndex(index);
            break;
        }
    }
}

void AAbstractTabBar::moveTabStopped(int index)
{
    bool cleanup = (m_pressedIndex == index) || (m_pressedIndex == -1) || !isIndexValid(index);
    bool allAnimationsFinished = true;

    for (int i = 0; allAnimationsFinished && i < m_tabList.count(); ++i)
    {
        const AAbstractTab& tab = *m_tabList.at(i);
        if (tab.m_animation && tab.m_animation->state() == QAbstractAnimation::Running)
            allAnimationsFinished = false;
    }

    if (allAnimationsFinished && cleanup)
    {
        if (m_movableTabWidget != nullptr)
            m_movableTabWidget->hide();

        for (int i = 0; i < m_tabList.count(); ++i)
            m_tabList[i]->m_dragOffset = 0;

        if (m_pressedIndex != -1 && m_movable)
        {
            m_pressedIndex = -1;
            m_startDrag = false;
            m_pntStartDrag = QPoint();
        }
    }
    else
    {
        if (!isIndexValid(index))
            return;
        m_tabList[index]->m_dragOffset = 0;
    }
    update();
}

/*! \reimp */
void AAbstractTabBar::changeEvent(QEvent* event)
{
    switch (event->type()) 
    {
        case QEvent::StyleChange:
        case QEvent::FontChange:
            {
                updateLayout(true);
            }
            break;
        default:
            break;
    }
    QWidget::changeEvent(event);
}

Qt::TextElideMode AAbstractTabBar::elideMode() const
{ return m_elideMode; }

void AAbstractTabBar::setElideMode(Qt::TextElideMode mode)
{
    m_elideMode = mode;
    refresh();
}

bool AAbstractTabBar::tabsClosable() const
{ return m_tabsClosable; }

void AAbstractTabBar::setTabsClosable(bool closable)
{
    if (m_tabsClosable == closable)
        return;
    m_tabsClosable = closable;
    updateLayout(true);
    update();
}

bool AAbstractTabBar::isMovable() const
{ 
    return m_movable;
}

void AAbstractTabBar::setMovable(bool movable)
{ 
    m_movable = movable;
}

bool AAbstractTabBar::isScrollable() const
{ 
    return m_scrollContainer != nullptr;
}

void AAbstractTabBar::setScrollable(bool scrollable)
{
    if (isScrollable() == scrollable)
        return;

    if (scrollable)
        m_scrollContainer = new AScrollContainer(this);
    else
        A_DELETE_AND_NULL(m_scrollContainer);

    for (int i = 0; i < m_tabList.count(); ++i)
        m_tabList.at(i)->setParent(scrollView());
}

bool AAbstractTabBar::scrollButtonsBeside() const
{
    if (!isScrollable())
        return false;
    return m_scrollContainer->scrollButtonsBeside();
}

void AAbstractTabBar::setScrollButtonsBeside(bool beside)
{
    if (!isScrollable())
        return;
    return m_scrollContainer->setScrollButtonsBeside(beside);
}

void AAbstractTabBar::addWidgetToLeftSide(QWidget* widget, int stretch, Qt::Alignment alignment)
{
    if (widget == nullptr)
        return;
    insertWidgetToLeftSide(0, widget, stretch, alignment);
}

int AAbstractTabBar::insertWidgetToLeftSide(int index, QWidget* widget, int stretch, Qt::Alignment alignment)
{
    if (widget == nullptr)
        return -1;
    STabBarItem* item = new STabBarItem(widget, stretch, alignment, false);
    int idx = indexToLastRightWidget();
    if (index < 0 || index > m_items.size() || (idx >= 0 && index > idx + 1))
    {
        qWarning("AAbstractTabBar::insertWidgetToLeftSide: Index out of range (%d), appending widget", index);
        index = m_items.size();
    }
    m_items.insert(index, item);
    updateALayoutWidgets();
    return index;
}

void AAbstractTabBar::setDirectionRightSide(QBoxLayout::Direction direction)
{
    if (m_layoutDirection == direction)
        return;
    m_layoutDirection = direction;
    updateLayout(true);
}

void AAbstractTabBar::addWidgetToRightSide(QWidget *widget, int stretch, Qt::Alignment alignment)
{
    if (widget == nullptr)
        return;
    insertWidgetToRightSide(indexToLastRightWidget() + 1, widget, stretch, alignment);
}

int AAbstractTabBar::insertWidgetToRightSide(int index, QWidget *widget, int stretch, Qt::Alignment alignment)
{
    if (widget == nullptr)
        return -1;
    STabBarItem* item = new STabBarItem(widget, stretch, alignment, true);
    int idx = indexToLastRightWidget();
    if (index < 0 || index > m_items.size() || (idx >= 0 && index <= idx))
    {
        qWarning("AAbstractTabBar::insertWidgetToRightSide: Index out of range (%d), appending widget", index);
        index = idx + 1;
    }
    m_items.insert(index, item);
    updateALayoutWidgets();
    return index;
}

void AAbstractTabBar::removeWidget(QWidget* widget)
{
    if (widget == nullptr)
        return;
    bool found = false;
    for (int i = 0; i < m_items.size(); ++i)
    {
        STabBarItem* item = m_items.at(i);
        if (item == nullptr)
            break;

        if (item->widget == widget)
        {
            m_items.removeAt(i);
            item->widget->hide();

            QWidgetAction* widgetAction = qobject_cast<QWidgetAction*>(item->action);
            if (widgetAction != nullptr && item->customWidget)
                widgetAction->releaseWidget(item->widget);
            A_DELETE_AND_NULL(item);
            found = true;
            break;
        }
    }
    if (found)
        updateLayout(true);
}

/*! \reimp */
QSize AAbstractTabBar::sizeHint() const
{
    QSize size;
    if (m_customWidgetLayout != nullptr)
    {
        m_customWidgetLayout->invalidate();
        size = m_customWidgetLayout->sizeHint();
    }
    AStyleOptionTab opt;
    int vframe = style()->pixelMetric(QStyle::PM_TabBarTabVSpace, &opt, this);
    const QFontMetrics fm = this->fontMetrics();

    int height = fm.height() + vframe;
    size.setHeight(qMax(height, size.height()));
    return size;
}

APROCH_NAMESPACE_END