/****************************************************************************
 * @file    AARibbonBar.cpp
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
#include "ARibbonBar.h"

#include <QScreen>
#include <QMainWindow>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QWindow>
#endif
#include <QStyleOption>
#include <QPainter>
#include <QCheckBox>
#include <QRadioButton>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QWidgetAction>
#include <QMdiSubWindow>
#include <QTranslator>

#include "Platform/APlatform.h"
#include "Widget/Style/AStyleOption.h"
#include "Widget/Style/ACommonStyle_p.h"
#include "Widget/AToolTip.h"
#include "Widget/AWindowTitleBar.h"
#include "ARibbonBar_p.h"
#include "ARibbonPage_p.h"
#include "AOfficePopupMenu.h"
#include "ARibbonSystemMenu.h"
#include "ARibbonBackstageView_p.h"
#include "ARibbonKeyTip_p.h"
#include "ARibbonGroup_p.h"
#include "ARibbonGallery.h"
#include "ARibbonCustomizeManager.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

const QString strCustomizeQAToolBar = AStr("customizeQAToolBar");
const QString strCustomizeARibbonBar = AStr("customizeARibbonBar");

static void aproch_fixupQtMainLayout(ARibbonBar* ribbonBar)
{
    Q_UNUSED(ribbonBar);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    QMainWindow* mainWindow = qobject_cast<QMainWindow*>(ribbonBar->parentWidget());
    if (mainWindow == nullptr)
        return;
    if (AWindowTitleBar::find(mainWindow) == nullptr || !AWindowTitleBar::find(mainWindow)->isVisible())
        return;

    QWidgetData* data = qt_qwidget_data(ribbonBar);
    Q_ASSERT(data != nullptr);
    data->crect.setRect(mainWindow->contentsMargins().left(), data->crect.top(),
                        mainWindow->width() - mainWindow->contentsMargins().left() - mainWindow->contentsMargins().right(),
                        data->crect.height());
#endif
}


/* ARibbonBarBanner */
ARibbonBarBanner::ARibbonBarBanner(QWidget* parent)
    : QLabel( parent )
    , m_alignment(Qt::AlignRight)
{
}

/*! \reimp */
void ARibbonBarBanner::paintEvent(QPaintEvent* event)
{
    QLabel::paintEvent(event);
}


/*!
    \enum ARibbonBar::QuickAccessBarPosition
    This enum describes the position of quick access bar widget on ribbon.
    \inmodule QtitanARibbon
    \value QATopPosition quick access bar is placed on the title (or the top) of the ribbon.
    \value QABottomPosition quick access bar is placed on the bottom of the ribbon.    
*/

/*!
\property ARibbonBar::currentPageIndex
\inmodule QtitanARibbon
Holds the index of the current page.
*/

/*!
\property ARibbonBar::minimized
\inmodule QtitanARibbon
Allows  to switch the ARibbon Bar to the minimized mode and back.
*/

/*!
\class ARibbonToolTipManager
\internal
*/
ARibbonToolTipManager::ARibbonToolTipManager(QObject* parent)
    : QObject(parent)
{
    qApp->installEventFilter(this);
}

ARibbonToolTipManager::~ARibbonToolTipManager()
{
    qApp->removeEventFilter(this);
}

/*! \internal */
bool ARibbonToolTipManager::showToolTip(const QPoint& pos, QWidget* w)
{
    if (!qobject_cast<ARibbonGallery*>(w))
    {
        if (QtnPrivate::aproch_isAncestor(w, QTITAN_META_CLASSNAME("ARibbonSystemMenu")))
            return true;
        if (QtnPrivate::aproch_isAncestor(w, "QDialog"))
            return false;
        if (!QtnPrivate::aproch_isAncestor(w, QTITAN_META_CLASSNAME("ARibbonQuickAccessBar")) && 
            !QtnPrivate::aproch_isAncestor(w, QTITAN_META_CLASSNAME("ARibbonBar")) &&
            !QtnPrivate::aproch_isAncestor(w, QTITAN_META_CLASSNAME("ARibbonStatusBar")) &&
            !QtnPrivate::aproch_isAncestor(w, QTITAN_META_CLASSNAME("ARibbonTitleBarWidget")))
            return false;
    }

    QPoint p = pos;
    p += QPoint(2,
#ifdef Q_WS_WIN
        21
#else
        16
#endif
    );

    int posX = p.x();
    int posY = p.y();

    if (const ARibbonGroup* group = aproch_getParentWidget<ARibbonGroup>(w))
    {
        Q_UNUSED(group);
        posX = w->mapToGlobal(w->rect().topLeft()).x();

        if (QWidget* prevWidget = aproch_getPrevParentWidget<ARibbonGroup>(w))
        {
            if (ARibbonGallery* gallery = qobject_cast<ARibbonGallery*>(w))
            {
                QRect rect = gallery->getDrawItemRect(gallery->selectedItem());
                posX = prevWidget->mapToGlobal(rect.topLeft()).x();
            }
            else
            {
                posX = prevWidget->mapToGlobal(prevWidget->rect().topLeft()).x();
                if (qobject_cast<ARibbonToolBarControl*>(prevWidget))
                    posX = w->mapToGlobal(w->rect().topLeft()).x();
            }
        }
        if (group->windowFlags() & Qt::Popup)
            posY = group->mapToGlobal(group->rect().bottomRight()).y() + int(ACommonStylePrivate::dpiScaled(2., w));
        else if (const ARibbonPage* page = aproch_getParentWidget<ARibbonPage>(w))
            posY = page->mapToGlobal(page->rect().bottomRight()).y() + int(ACommonStylePrivate::dpiScaled(2., w));
    }

    QIcon tipIcon;
    QString tipTitleText;
    QString tipText;

    if (ARibbonGallery* gallery = qobject_cast<ARibbonGallery*>(w))
    {
        tipText = gallery->toolTip();
        if (ARibbonGalleryItem* item = gallery->item(gallery->selectedItem()))
        {
            QString str = item->toolTip();
            if (tipText != str)
                tipText = str;
        }
    }
    else if (QToolButton* button = qobject_cast<QToolButton*>(w))
    {
        tipText = button->toolTip();
        tipIcon = button->icon();
        QString str = button->text();
        if (tipText != str)
            tipTitleText = str;

        if (QAction * defAction = button->defaultAction())
        {
            QString strShortcuts;
            QKeySequence keySequence;
            QList<QKeySequence> lstShortcuts = defAction->shortcuts();

            foreach(keySequence, lstShortcuts)
            {
                QString strShortcutString = keySequence.toString(QKeySequence::NativeText);
                if (strShortcuts != QString())
                    strShortcuts += QStringLiteral(", ");
                strShortcuts += strShortcutString;
            }
            if (strShortcuts != QString())
            {
                if (tipTitleText != QString())
                    tipTitleText += QStringLiteral(" (") + strShortcuts + QStringLiteral(")");
                else
                    tipText += QStringLiteral(" (") + strShortcuts + QStringLiteral(")");
            }
        }
    }
    else
    {
        QWidget* parentWidget = w->parentWidget();

        if (!qobject_cast<ARibbonGroup*>(parentWidget))
        {
            if (ARibbonPageSystemRecentFileList* recentFile = qobject_cast<ARibbonPageSystemRecentFileList*>(w))
            {
                if (QAction* currentAction = recentFile->getCurrentAction())
                {
                    tipText = currentAction->data().toString();
                    tipIcon = currentAction->icon();
                    QString str = button->text();
                    if (tipText != str)
                        tipTitleText = str;
                }
            }
            else
            {
#if 0
                while (parentWidget)
                {
                    tipText = parentWidget->toolTip();
                    if (!tipText.isEmpty())
                        break;
                    parentWidget = parentWidget->parentWidget();
                    if (qobject_cast<ARibbonGroup*>(parentWidget))
                        break;
                }
#endif
            }
        }
    }

    if (!tipTitleText.isEmpty() || !tipText.isEmpty() || !tipIcon.isNull())
    {
        QPoint globalPos(posX, posY);
        ToolTip::showToolTip(globalPos, tipTitleText, tipText, tipIcon, w);
        return true;
    }
    return false;
}

/*! \reimp */
bool ARibbonToolTipManager::eventFilter(QObject* watched, QEvent* event)
{
    switch (event->type())
    {
        case QEvent::ToolTip:
            if (QWidget* widget = qobject_cast<QWidget*>(watched))
            {
                if (showToolTip(static_cast<QHelpEvent*>(event)->globalPos(), widget))
                    return true;
            }
            break;
        default:
            break;
    }
    return QObject::eventFilter(watched, event);
}

/* ARibbonBarView */
ARibbonBarView::ARibbonBarView(ARibbonBar* ribbonBar)
    : MaterialWidget(ribbonBar), m_ribbonBar(ribbonBar)
{
    setBlendType(MaterialWidget::DisabledBlend);
}

void ARibbonBarView::paintEvent(QPaintEvent* event)
{
    MaterialWidget::paintEvent(event);
    if (m_ribbonBar->titleBackground().isNull())
        return;
    QPainter painter(this);
    painter.setOpacity(0.5);
    QRect paintRect = QStyle::alignedRect(Qt::LeftToRight, Qt::Alignment(Qt::AlignRight | Qt::AlignTop), m_ribbonBar->titleBackground().size(), rect());
    painter.drawPixmap(paintRect, m_ribbonBar->titleBackground());
}


/* ARibbonBarPrivate */
ARibbonBarPrivate::ARibbonBarPrivate()
    : m_metricsDirty(true)
    , m_view(nullptr)
    , m_ribbonItemHeight(-1)
    , m_ribbonToolTipManager(nullptr)
    , m_ribbonTitleBarWidget(nullptr)
    , m_simplifiedAction(nullptr)
    , m_ribbonTabBar(nullptr)
    , m_pagePopup(nullptr)
    , m_quickAccessBar(nullptr)
    , m_systemButton(nullptr)
    , m_banner(nullptr)
    , m_overflowMenuButton(nullptr)
    , m_titleBarCustomized(false)
    , m_saveVisibleSysMenu(true)
    , m_contextualTabsVisible(false)
    , m_minimizationEnabled(true)
    , m_simplifiedMode(false)
    , m_expandDirection(Qt::RightToLeft)
    , m_minimized(false)
    , m_keyTipsActive(false)
    , m_keyTipsReadyToActivate(false)
    , m_keyTipsComplement(true)
    , m_keyTipsEnabled(true)
    , m_titleGroupsVisible(true)
    , m_keyboardFocusWidget(nullptr)
    , m_quickAccessBarPosition(ARibbonBar::TopPosition)
    , m_tabBarPosition(ARibbonBar::TopPosition)
    , m_countKey(0)
    , m_hoveredSubControl(QStyle::SC_None)
    , m_activeSubControl(QStyle::SC_None)
    , m_customizeManager(nullptr)
    , m_customizeDialog(nullptr)
    , m_rowItemCount(3)
    , m_updateLayoutCount(0)
    , m_destoying(false)
{
    qApp->installEventFilter(this);
    m_activePageAnimation = new QPropertyAnimation();
    connect(m_activePageAnimation, SIGNAL(finished()), this, SLOT(activePageAnimationFinished()));
    connect(m_activePageAnimation, SIGNAL(valueChanged(const QVariant&)), this, SLOT(activePageAnimationValueChanged(const QVariant&)));
}

/*! \internal */
ARibbonBarPrivate::~ARibbonBarPrivate()
{
    qApp->removeEventFilter(this);
    deleteARibbonTitleBarWidget();
    Q_DELETE_AND_NULL(m_view);
    Q_DELETE_AND_NULL(m_activePageAnimation);
}

ARibbonBar* ARibbonBarPrivate::findARibbonBar(const QWidget* widget)
{
    if (widget == nullptr)
        return nullptr;
    QWidget* parent = widget->parentWidget();
    while (parent != nullptr)
    {
        if (ARibbonBar* ribbonBar = qobject_cast<ARibbonBar*>(parent))
        {
            return ribbonBar;
        }
        else if (ARibbonTitleBarWidget* titleBar = qobject_cast<ARibbonTitleBarWidget*>(parent))
        {
            if (ARibbonBar* ribbonBar = titleBar->ribbonBar())
                return ribbonBar;
        }
        parent = parent->parentWidget();
    }
    return nullptr;
}

void ARibbonBarPrivate::init()
{
    A_P(ARibbonBar);

    m_ribbonToolTipManager = new ARibbonToolTipManager(this);
    m_pagePopup = new ARibbonPagePopup(&p);
    m_view = new ARibbonBarView(&p);

    m_ribbonTabBar = new ARibbonTabBar(&p);
    QObject::connect(m_ribbonTabBar, SIGNAL(currentChanged(int)), this, SLOT(currentChanged(int)));
    QObject::connect(m_ribbonTabBar, SIGNAL(currentAboutToBeChanged(int, bool&)), this, SLOT(currentAboutToBeChanged(int, bool&)));
    m_banner = new ARibbonBarBanner(getPageParent());
    m_banner->setVisible(false);

    m_overflowMenuButton = new ARibbonOverflowMenuButton(getPageParent());
    m_overflowMenuButton->setPopupMode(QToolButton::InstantPopup);
    m_overflowMenuButton->setVisible(false);
    createARibbonTitleBarWidget();

    p.setNativeMenuBar(false);
    p.setAttribute(Qt::WA_Hover, true);
    p.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    m_SysBtnSplitLine = new QFrame(&p);
    m_SysBtnSplitLine->setFrameShape(QFrame::VLine);
    m_SysBtnSplitLine->setStyleSheet(tr("QFrame{color:#D8D9DA; background:#D8D9DA;}"));
    m_SysBtnSplitLine->show();

    m_QuckAcessSplitLine = new QFrame(&p);
    m_QuckAcessSplitLine->setFrameShape(QFrame::VLine);
    m_QuckAcessSplitLine->setStyleSheet(tr("QFrame{color:#D8D9DA; background:#D8D9DA;}"));
    m_QuckAcessSplitLine->show();
}

bool ARibbonBarPrivate::isTitleBarCustomizationSupported() const
{
#ifdef QTITAN_DESIGNER
    return false;
#endif
    A_P(const ARibbonBar);
    if (p.parentWidget() != nullptr && p.parentWidget()->layout() != nullptr &&
        p.parentWidget()->layout()->menuBar() == &p)
        return true;
    return false;
}

void ARibbonBarPrivate::changed(bool dirty)
{
    A_P(ARibbonBar);
    ARibbonBarAutoUpdater autoUpdater(&p);
    if (dirty)
    {
        setMetricsDirty();
        for (int i = 0; i < p.pageCount(); ++i)
        {
            for (int j = 0; j < p.page(i)->groupCount(); ++j)
            {
                for (int k = 0; k < p.page(i)->group(j)->controlCount(); ++k)
                    p.page(i)->group(j)->controlByIndex(k)->setDirty();
            }
        }
    }

    p.updateGeometry();
}

void ARibbonBarPrivate::updateMetrics()
{
    if (!m_metricsDirty)
        return;
    m_metricsDirty = false;
    A_P(ARibbonBar);
    QFont pageFont = getPageFont();
    QFont_setResolveMask(pageFont, QFont::AllPropertiesResolved);

    QFontMetrics fm(pageFont);
    int rowItemHeight = fm.height();

    QLineEdit edit(&p);
    edit.setAttribute(Qt::WA_MacSmallSize);
    edit.setFont(pageFont);
    QSize sizeHint = edit.sizeHint();
    rowItemHeight = qMax(sizeHint.height(), rowItemHeight);

    QComboBox box1(&p);
    box1.setAttribute(Qt::WA_MacSmallSize);
    box1.ensurePolished();
    sizeHint = box1.sizeHint();
    rowItemHeight = qMax(sizeHint.height(), rowItemHeight);

    QComboBox box2(&p);
    box2.setAttribute(Qt::WA_MacSmallSize);
    box2.setEditable(true); //Affects on MacOSX widget
    box2.setFont(pageFont);
    sizeHint = box2.sizeHint();
    rowItemHeight = qMax(sizeHint.height(), rowItemHeight);

    QSpinBox box3(&p);
    box3.setAttribute(Qt::WA_MacSmallSize);
    box3.setFont(pageFont);
    sizeHint = box3.sizeHint();
    rowItemHeight = qMax(sizeHint.height(), rowItemHeight);
    m_ribbonItemHeight = rowItemHeight;
    if (p.simplifiedMode())
    {
        m_ribbonItemHeight += qRound(fm.height() * 0.7);
        m_groupTitleHeight = 0;
    }
    else
    {
        m_ribbonItemHeight += fm.height() / 4;
        m_groupTitleHeight = calcGroupTitleHeight();
    }
}

void ARibbonBarPrivate::setMetricsDirty()
{
    m_metricsDirty = true;
    m_sizeHint = QSize();
}

/*! \internal */
int ARibbonBarPrivate::calcTabBarHeight() const
{
    return m_ribbonTabBar->sizeHint().height();
}

/*! \internal */
int ARibbonBarPrivate::calcGroupTitleHeight() const
{
    A_P(const ARibbonBar);
    int ret = 0;
    for (int i = 0, count = m_ribbonTabBar->count(); count > i; i++)
    {
        ARibbonPage* page = m_ribbonTabBar->getTab(i)->page();
        updateFont(page);

        for (int i = 0; i < page->groupCount(); ++i)
        {
            if (ARibbonGroup* group = page->group(i))
                ret = qMax(ret, ARibbonGroupPrivate::calcGroupTitleHeight(group->titleFont()));
        }
    }
    if (ret == 0)
        ret = ARibbonGroupPrivate::calcGroupTitleHeight(p.font());
    return ret;
}

void ARibbonBarPrivate::updateFont(ARibbonPage* page) const
{
    if (page != nullptr)
        page->setFont(getPageFont());
}

void ARibbonBarPrivate::updateLayoutRects()
{
    A_P(ARibbonBar);
    updateMetrics();

    WindowTitleBar* titleBar = findTitleBar();

    if (m_systemButton != nullptr)
    {
        Qt::ToolButtonStyle toolButtonStyle = Qt::ToolButtonTextOnly;
        if (p.tabBarPosition() == ARibbonBar::TopPosition)
            toolButtonStyle = Qt::ToolButtonStyle(p.style()->styleHint(QStyle::SH_ToolButtonStyle, nullptr, m_systemButton));
        m_systemButton->setToolButtonStyle(toolButtonStyle);
    }

    int titleBarHeight = p.style()->pixelMetric(QStyle::PM_TitleBarHeight, nullptr, p.window());
    int quickBarHeight = 0;
    int extendedHeight = 0;
    bool contextualTabsOnTitleBar = false;

    if (titleBar != nullptr && titleBar->isVisible())
    {
        if (titleBar->extendViewIntoTitleBar() && 
            (!p.isContextualTabsVisible() || p.tabBarPosition() == ARibbonBar::BottomPosition))
            extendedHeight = titleBarHeight;

        if (!titleBar->styledFrame() && p.tabBarPosition() == ARibbonBar::TopPosition)
            contextualTabsOnTitleBar = true;
    }
    
    if (p.tabBarPosition() == ARibbonBar::BottomPosition && p.quickAccessBarPosition() == ARibbonBar::TopPosition)
        quickBarHeight = titleBarHeight;
    if (p.tabBarPosition() == ARibbonBar::TopPosition && p.quickAccessBarPosition() == ARibbonBar::BottomPosition)
        quickBarHeight = titleBarHeight;

    int groupTitleHeight = 0;
    int pageHeight = 0;
    int contextualTabBarHeight = 0;

    int tabBarHeight = calcTabBarHeight();

    if (p.isContextualTabsVisible())
        contextualTabBarHeight = titleBarHeight;
    if (p.isTitleGroupsVisible())
        groupTitleHeight = m_groupTitleHeight;

    int visiblePageHeight = rowItemHeight() * rowItemCount() + groupTitleHeight +
        pageContentMargins().top() + pageContentMargins().bottom() + groupContentMargins().top() + groupContentMargins().bottom();
    pageHeight = p.isMinimized() ? 0 : visiblePageHeight;

    m_contextualTabBarRect = p.rect();
    m_contextualTabBarRect.setTop(p.tabBarPosition() == ARibbonBar::TopPosition ?
        extendedHeight : extendedHeight + quickBarHeight + pageHeight + tabBarHeight);
    m_contextualTabBarRect.setHeight(contextualTabBarHeight);
    if (contextualTabsOnTitleBar)
        contextualTabBarHeight = 0;

    m_tabBarRect = p.rect();
    m_tabBarRect.setTop(p.tabBarPosition() == ARibbonBar::TopPosition ? 
        extendedHeight + contextualTabBarHeight : extendedHeight + quickBarHeight + pageHeight);
    m_tabBarRect.setHeight(tabBarHeight);

    if (QWidget* rightWidget = p.cornerWidget(Qt::TopRightCorner))
    {
        if (rightWidget->isVisible())
        {
            const int hmargin = p.style()->pixelMetric(QStyle::PM_MenuBarHMargin, 0, &p);
            QSize sh = rightWidget->sizeHint();
            if (p.layoutDirection() == Qt::LeftToRight)
                m_tabBarRect.setWidth(m_tabBarRect.width() - sh.width() - hmargin);
            else
                m_tabBarRect.setLeft(m_tabBarRect.left() + (sh.width() - hmargin));
        }
    }

    m_sysButtonRect = QRect();
    if (m_systemButton != nullptr)
    {
        m_sysButtonRect = m_tabBarRect;
        QSize sh = QSize();
        if (m_systemButton->toolButtonStyle() == Qt::ToolButtonIconOnly)
        {
            const int size = m_tabBarRect.height() * 1.85;
            sh = QSize(size, size);

            if (p.tabBarPosition() == ARibbonBar::TopPosition)
            {
                m_sysButtonRect.setTop(m_tabBarRect.top() - size / 2);
            }
            else
            {
                m_sysButtonRect.setTop(m_contextualTabBarRect.top() - m_tabBarRect.height());
                m_sysButtonRect.setBottom(m_contextualTabBarRect.bottom());
            }
            m_sysButtonRect.setSize(sh);
            m_sysButtonRect = QStyle::alignedRect(Qt::LeftToRight, 
                Qt::Alignment(Qt::AlignLeft | Qt::AlignVCenter), sh, m_sysButtonRect);
        }
        else
        {
            sh = m_systemButton->sizeHint();
            m_sysButtonRect.setWidth(sh.width());
        }

        m_tabBarRect.setLeft(sh.width());
    }

    m_quickBarRect = QRect();
    if (m_quickAccessBar != nullptr)
    {
        QSize sh = m_quickAccessBar->sizeHint();
        if (quickBarHeight > 0)
        {
            m_quickBarRect = p.rect();
            m_quickBarRect.setTop(p.quickAccessBarPosition() == ARibbonBar::TopPosition ?
                extendedHeight : extendedHeight + contextualTabBarHeight + tabBarHeight + pageHeight);
            m_quickBarRect.setHeight(quickBarHeight);
            m_quickBarRect.setWidth(sh.width());
        }
        else if (!p.isContextualTabsVisible())
        {
            m_quickBarRect = m_tabBarRect.adjusted(10,3,10,0);
            m_quickBarRect.setSize(sh);
            m_tabBarRect.setLeft(m_tabBarRect.left() + sh.width() + 30);
        }
    }

    m_pageWindowRect = p.rect();
    m_pageWindowRect.setTop(p.isMinimized() ? 0 : p.tabBarPosition() == ARibbonBar::TopPosition ?
        extendedHeight + contextualTabBarHeight + tabBarHeight : extendedHeight + quickBarHeight);
    m_pageWindowRect.setHeight(visiblePageHeight);
    m_pageContentRect = m_pageWindowRect;

    m_bannerRect = QRect();
    if (m_banner != nullptr && !m_banner->isHidden())
    {
        m_bannerRect = m_pageContentRect;
        QSize sh = m_banner->sizeHint();
        if (m_banner->alignment() == Qt::AlignLeft)
        {
            m_bannerRect.setWidth(sh.width());
            m_pageContentRect.setLeft(sh.width());
        }
        else
        {
            m_bannerRect.setLeft(m_pageContentRect.right() - sh.width());
            m_pageContentRect.setWidth(m_pageContentRect.width() - sh.width());
        }
    }

    m_overflowButtonRect = QRect();
    if (p.simplifiedMode())
    {
        ARibbonPage* activePage = p.currentPage();
        if (activePage != nullptr)
        {
            m_overflowButtonRect = m_pageContentRect;
            QSize sh = QSize(m_pageWindowRect.height(), m_pageWindowRect.height());
#if 0
            if (m_overflowMenuButton->alignment() == Qt::AlignLeft)
            {
                m_overflowButtonRect.setWidth(sh.width());
                m_pageContentRect.setLeft(sh.width());
            }
            else
#endif
            {
                const int spacing = p.style()->pixelMetric(static_cast<QStyle::PixelMetric>(CommonStyle::PM_ARibbonLayoutHorizontalSpacing), nullptr, &p);
                m_overflowButtonRect.setSize(sh);
                m_overflowButtonRect.adjust(groupContentMargins().left(), groupContentMargins().top(),
                    -groupContentMargins().right(), -groupContentMargins().bottom());
                m_pageContentRect.setWidth(m_pageContentRect.width() - sh.width() - spacing);

                // TODO         m_overflowButtonRect.setLeft(m_pageContentRect.right() - sh.width());
                // TODO         m_pageContentRect.setWidth(m_pageContentRect.width() - sh.width());
            }
            m_overflowButtonRect.adjust(0, pageContentMargins().top(), 0, -pageContentMargins().bottom());
        }
    }

    if (m_bShowARibbonBarView)
    {
		m_sizeHint = QSize(aproch_DPIScaled(400, &p), extendedHeight + contextualTabBarHeight + tabBarHeight + pageHeight + quickBarHeight);
    }
	else
	{
		m_sizeHint = QSize(aproch_DPIScaled(400, &p),extendedHeight + contextualTabBarHeight + quickBarHeight);
	}

#ifdef QTITAN_DESIGNER
    p.setFixedHeight(m_sizeHint.height());
#endif
}

bool ARibbonBarPrivate::updateOverflowButton()
{
    bool result = false;
    if (m_destoying)
        return result;
    A_P(ARibbonBar);

    QRect overflowButtonRect = m_overflowButtonRect;
    ARibbonPage* activePage = p.currentPage();
    const bool hasMenu = activePage != nullptr && 
        !ARibbonPagePrivate::_get(activePage)->isOverflowMenuIsEmpty();
    m_overflowMenuButton->setVisible(!overflowButtonRect.isNull() && hasMenu);
    if (!m_overflowMenuButton->isHidden())
    {
        m_overflowMenuButton->setMenu(activePage->aproch_d().m_overflowMenu);
        const QRect rect = activePage->geometry();
        const int groupsLength = qMin(ARibbonPagePrivate::_get(activePage)->groupsLength(), rect.width());
        const int spacing = p.style()->pixelMetric(static_cast<QStyle::PixelMetric>(CommonStyle::PM_ARibbonLayoutHorizontalSpacing), nullptr, &p);
        overflowButtonRect.moveLeft(rect.left() + groupsLength + spacing);
        m_overflowMenuButton->setGeometry(overflowButtonRect);
        m_overflowMenuButton->raise();
    }
    return result;
}

void ARibbonBarPrivate::updateLayout()
{
    if (m_destoying)
        return;
    A_P(ARibbonBar);
    m_view->setGeometry(p.rect());

    /* In MacOSX the font has been selected in ARibbonStyle::polish(), so we have non valid reference to the tabbar. */
    Q_ASSERT(m_ribbonTabBar != nullptr);
    if (m_ribbonTabBar == nullptr)
        return;

    QStyleOption opt;
    opt.initFrom(&p);
    const int vmargin = p.style()->pixelMetric(QStyle::PM_MenuBarVMargin, &opt, &p);
    const int hmargin = p.style()->pixelMetric(QStyle::PM_MenuBarHMargin, &opt, &p);
    m_pageContentMargins = QMargins(hmargin, vmargin, hmargin, vmargin);

    const int margin = p.style()->pixelMetric(static_cast<QStyle::PixelMetric>(CommonStyle::PM_ARibbonGroupContentMargin), nullptr, &p);
    m_groupContentMargins = QMargins(margin, margin, margin, margin);
 
    updateTitleBarWidget();
    updateLayoutRects();
    m_ribbonTabBar->setGeometry(m_tabBarRect);
    layoutCorner();
    m_ribbonTabBar->updateLayout();

    if (m_systemButton != nullptr)
	{
        m_systemButton->setGeometry(m_sysButtonRect);
        m_systemButton->raise();
        if(m_SysBtnSplitLine!=nullptr)
            m_SysBtnSplitLine->setGeometry(QRect(m_sysButtonRect.x() + m_sysButtonRect.width(), m_sysButtonRect.y() + 3, 1, m_sysButtonRect.height() - 6));
    }

    if (m_quickAccessBar != nullptr && !m_quickBarRect.isNull())
    {
        m_quickAccessBar->setGeometry(m_quickBarRect);
        if (m_QuckAcessSplitLine != nullptr)
            m_QuckAcessSplitLine->setGeometry(QRect(m_quickBarRect.x() + m_quickBarRect.width() + 10, m_sysButtonRect.y() + 3, 1, m_sysButtonRect.height() - 6));
    }


    if (m_banner != nullptr && !m_bannerRect.isNull())
        m_banner->setGeometry(m_bannerRect);

    updateActivePageLayout(p.currentPage(), false);
    updateOverflowButton();

    if (m_ribbonTitleBarWidget != nullptr)
    {
        bool updateLayoutNeeded = true;
        if (m_ribbonTitleBarWidget->parentWidget() == material())
        {
            updateLayoutNeeded = m_ribbonTitleBarWidget->geometry() == m_contextualTabBarRect;
            m_ribbonTitleBarWidget->setGeometry(m_contextualTabBarRect);
        }
        if (updateLayoutNeeded)
            m_ribbonTitleBarWidget->updateLayout();
    }
}

/*! \internal */
void ARibbonBarPrivate::layoutCorner()
{
    //See QMenuBarPrivate::updateGeometries() for the details.
    A_P(ARibbonBar);
    if (QWidget* rightWidget = p.cornerWidget(Qt::TopRightCorner))
    {
        if (rightWidget && rightWidget->isVisible())
        {
            const int hmargin = p.style()->pixelMetric(QStyle::PM_MenuBarHMargin, 0, &p);
            QRect rcTab = m_ribbonTabBar->geometry();
            QRect rc = rightWidget->geometry();
            QPoint pos(p.width() - rc.width() - hmargin + 1, rcTab.top() + (rcTab.height() - rc.height()) / 2);
            QRect vRect = QStyle::visualRect(p.layoutDirection(), p.rect(), QRect(pos, rc.size()));
            rightWidget->setGeometry(vRect);
        }
    }

    if (QWidget* leftCorner = p.cornerWidget(Qt::TopLeftCorner))
    {
        leftCorner->setVisible(false);
    }
}

/*! \internal */
void ARibbonBarPrivate::createARibbonTitleBarWidget()
{
    A_P(ARibbonBar); 
    if (m_ribbonTitleBarWidget == nullptr)
    {
        m_ribbonTitleBarWidget = new ARibbonTitleBarWidget(&p);
        m_quickAccessBar = new ARibbonQuickAccessBar();
        m_quickAccessBar->setObjectName(QStringLiteral("aproch_ARibbon_Quick_Access_Bar"));
        m_ribbonTitleBarWidget->addWidget(m_quickAccessBar);
        m_quickAccessBar->setVisible(false);

        m_simplifiedAction = new QAction(ARibbonBar::tr_compatible(QtnARibbonSimplifiedARibbonActionString), &p);
        m_simplifiedAction->setToolTip(ARibbonBar::tr_compatible(QtnARibbonSimplifiedARibbonActionToolTipsString));
        m_simplifiedAction->setCheckable(true);
        ToggleSwitch* toggleSwitch = new ToggleSwitch();
        toggleSwitch->setTextAlignment(Qt::AlignRight);
        m_ribbonTitleBarWidget->addWidgetToRightSide(toggleSwitch);
        toggleSwitch->setDefaultAction(m_simplifiedAction);
        connect(m_simplifiedAction, SIGNAL(toggled(bool)), this, SLOT(toggledSimplifiedMode(bool)));
    }
}

/*! \internal */
void ARibbonBarPrivate::deleteARibbonTitleBarWidget()
{
    if (m_ribbonTitleBarWidget == nullptr)
        return;
    if (WindowTitleBar* titleBar = findTitleBar())
    {
        titleBar->setWidget(nullptr);
        titleBar->removeAndDelete();
    }
    m_ribbonTitleBarWidget->setParent(nullptr);
    Q_DELETE_AND_NULL(m_ribbonTitleBarWidget);
}

/*! \internal */
void ARibbonBarPrivate::setBackstageViewVisible(bool visible)
{
    A_P(ARibbonBar);
    if (static_cast<bool>(p.style()->styleHint((QStyle::StyleHint)CommonStyle::SH_ARibbonBackstageHideTabs)))
    {
        if (visible)
        {
            if (m_ribbonTabBar && !m_ribbonTabBar->isHidden())
            {
                m_visibleWidgets << m_ribbonTabBar;
                m_ribbonTabBar->setVisible(false);
            }

            if (m_quickAccessBar && !m_quickAccessBar->isHidden())
            {
                m_visibleWidgets << m_quickAccessBar;
                m_quickAccessBar->setVisible(false);
            }

            if (m_systemButton && m_systemButton->isHidden())
            {
                m_visibleWidgets << m_systemButton;
                m_systemButton->setVisible(false);
            }

            if (m_simplifiedAction->isVisible())
            {
                m_visibleActions << m_simplifiedAction;
                m_simplifiedAction->setVisible(false);
            }

            if (WindowTitleBar* titleBar = findTitleBar())
            {
                m_saveVisibleSysMenu = titleBar->isSysMenuButtonVisible();
                titleBar->setSysMenuButtonVisible(false);
            }
        }
        else
        {
            ARibbonBarAutoUpdater autoUpdater(&p);

            for (QVector<QWidget*>::ConstIterator it = m_visibleWidgets.begin(); it != m_visibleWidgets.end(); ++it)
                (*it)->setVisible(true);
            m_visibleWidgets.clear();

            for (QVector<QAction*>::ConstIterator it = m_visibleActions.begin(); it != m_visibleActions.end(); ++it)
                (*it)->setVisible(true);
            m_visibleActions.clear();

            if (WindowTitleBar* titleBar = findTitleBar())
                titleBar->setSysMenuButtonVisible(m_saveVisibleSysMenu);
        }
    }
}

/*! \internal */
QAction* ARibbonBarPrivate::createSystemButton(const QIcon& icon, const QString& text)
{
    A_P(ARibbonBar);
    QAction* action = new QAction(&p);
    action->setText(text);

    if (icon.isNull())
    {
        QIcon iconQtnLogo;
        QIcon qtitan(QStringLiteral(":/res/qtitan.png"));
        iconQtnLogo.addPixmap(qtitan.pixmap(QSize(64,56), QIcon::Disabled));
        QIcon icon32(QStringLiteral(":/res/qtitanlogo32x32.png"));
        iconQtnLogo.addPixmap(icon32.pixmap(QSize(32,32), QIcon::Disabled));
        action->setIcon(iconQtnLogo);
    }
    else
    {
        action->setIcon(icon);
    }

    m_systemButton = new ARibbonSystemButton(&p);
    m_systemButton->setAutoRaise(true);
    m_systemButton->setFocusPolicy(Qt::NoFocus);
    m_systemButton->setDefaultAction(action);
    m_systemButton->setBackgroundColor(Qt::white);

    m_systemButton->setPopupMode(QToolButton::InstantPopup);
    action->setIconText(action->text());

    return action;
}

/*! \internal */
void ARibbonBarPrivate::insertPage(int index, ARibbonPage* page)
{
    Q_ASSERT(page != nullptr);
    A_P(ARibbonBar);
    ARibbonPagePrivate::_get(page)->setARibbonBar(&p);
    m_ribbonTabBar->insertTab(index, page->title(), page);
}

/*! \internal */
void ARibbonBarPrivate::removePage(int index, bool deletePage)
{
    if (isIndexValid(index))
    {
        ARibbonPage* page = m_ribbonTabBar->getTab(index)->page();
        ARibbonPagePrivate* page_private = ARibbonPagePrivate::_get(page);
        page_private->setAssociativeTab(nullptr);
        page_private->setARibbonBar(nullptr);
        if (deletePage)
            delete page;

        if (m_ribbonTitleBarWidget != nullptr)
            m_ribbonTitleBarWidget->adjustSizeTitleBar();
        m_ribbonTabBar->removeTab(index);
    }
}

/*! \internal */
int ARibbonBarPrivate::backstageViewTop() const
{
    A_P(const ARibbonBar);
    int height = 0;
    if (p.isFrameThemeEnabled())
    {
        if (WindowTitleBar* titleBar = findTitleBar())
        {
            if (titleBar->extendViewIntoTitleBar())
            height = p.style()->pixelMetric(QStyle::PM_TitleBarHeight, nullptr, p.window());
        }
    }
    return height + tabBarHeight();
}

/*! \internal */
int ARibbonBarPrivate::pageIndex(ARibbonPage* page) const
{
    Q_ASSERT(page != nullptr);
    for (int i = 0, count = m_ribbonTabBar->count(); count > i; i++)
    {
        if (m_ribbonTabBar->getTab(i)->page() == page)
            return i;
    }
    return -1;
}

/*! \internal */
void ARibbonBarPrivate::showPagePopup(ARibbonPage* page)
{
    A_P(ARibbonBar);
    Q_UNUSED(page);
    int margin = p.style()->pixelMetric(QStyle::PM_MenuVMargin, nullptr, &p);
    QRect tabBarRect = m_tabBarRect;
    QRect pageRect = m_pageWindowRect;
    pageRect.moveTop(tabBarRect.bottom());
    pageRect = QRect(p.mapToGlobal(pageRect.topLeft()), QSize(pageRect.width(), pageRect.height() + margin * 2));
    m_pagePopup->popup(pageRect);
}

/*! \internal */
void ARibbonBarPrivate::hidePagePopup()
{
    m_pagePopup->hide();
}

/*! \internal */
bool ARibbonBarPrivate::isPagePopupVisible() const
{
    return m_pagePopup->isVisible();
}

/*! \internal */
QWidget* ARibbonBarPrivate::getPageParent()
{
    A_P(ARibbonBar)
    if (p.isMinimized())
        return m_pagePopup;
    else
        return material();
}

WindowTitleBar* ARibbonBarPrivate::findTitleBar() const
{
    A_P(const ARibbonBar)
    QWidget* parent = p.parentWidget();
    return parent != nullptr && parent->isWindow() ? WindowTitleBar::find(parent) : nullptr;
}

bool ARibbonBarPrivate::isUpdating() const
{
    Q_ASSERT(m_updateLayoutCount >= 0);
    return m_updateLayoutCount > 0;
}

QMargins ARibbonBarPrivate::pageContentMargins() const
{
    return m_pageContentMargins;
}

QMargins ARibbonBarPrivate::groupContentMargins() const
{
    return m_groupContentMargins;
}

MaterialWidget* ARibbonBarPrivate::material() const
{
    return m_view;
}

int ARibbonBarPrivate::groupTitleHeight() const
{
    return m_groupTitleHeight;
}

QFont ARibbonBarPrivate::getPageFont() const
{
    A_P(const ARibbonBar);
    QFont result = p.font();
    if (p.simplifiedMode())
        result.setPointSizeF(result.pointSizeF() * 1.2);
    return result;
}

int ARibbonBarPrivate::extendedTitleBarHeight() const
{
    A_P(const ARibbonBar)
    WindowTitleBar* titleBar = findTitleBar();
    if (titleBar != nullptr && titleBar->isVisible())
    {
        if (!titleBar->styledFrame())
            return 0;
    }
    else
    {
        if (!p.isContextualTabsVisible())
            return 0;
    }

    int height = p.style()->pixelMetric(QStyle::PM_TitleBarHeight, nullptr, p.window());
    return height;
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
void ARibbonBarPrivate::updateTitleBar()
{
    A_P(ARibbonBar);
    WindowTitleBar* titleBar = findTitleBar();
    if (titleBar == nullptr)
        return;

    if (p.isFrameThemeEnabled() || p.isAcrilycEnabled())
    {
        if (p.isFrameThemeEnabled())
        {
            const bool styledFrame = p.style()->styleHint((QStyle::StyleHint)CommonStyle::SH_ARibbonStyledFrame);
            titleBar->setExtendViewIntoTitleBar(styledFrame);
            titleBar->setStyledFrame(styledFrame);

            WindowTitleBar::SysButtonKind buttonKind = titleBar->sysButtonKind();
            bool visible = p.style()->styleHint((QStyle::StyleHint)CommonStyle::SH_ARibbonsSysMenuButtonVisible);
            if (visible)
                visible = buttonKind == WindowTitleBar::SysMenuButton || buttonKind == WindowTitleBar::BackButton;
            titleBar->setSysMenuButtonVisible(visible);
        }
        titleBar->setVisible(true);
        titleBar->update();
    }
}
#endif

void ARibbonBarPrivate::updateTitleBarWidget()
{
    A_P(ARibbonBar);
    WindowTitleBar* titleBar = findTitleBar();
    
    if (!p.isFrameThemeEnabled() || p.tabBarPosition() == ARibbonBar::BottomPosition)
    {
        if (titleBar != nullptr && titleBar->widget() == m_ribbonTitleBarWidget)
            titleBar->setWidget(nullptr);
        m_ribbonTitleBarWidget->setParent(material());
        m_ribbonTitleBarWidget->setVisible(p.isContextualTabsVisible());
        m_ribbonTitleBarWidget->setPalette(m_ribbonTabBar->palette());
        m_ribbonTitleBarWidget->setFont(p.font());
    }
    else
    {
        if (titleBar == nullptr)
            return;
        if (titleBar->widget() == nullptr)
        {
            m_ribbonTitleBarWidget->setParent(nullptr);
            titleBar->setWidget(m_ribbonTitleBarWidget, WindowTitleBar::AlignClient);
        }
        //m_ribbonTitleBarWidget->setPalette(titleBar->palette());
        m_ribbonTitleBarWidget->setPalette(m_ribbonTabBar->palette());
        m_ribbonTitleBarWidget->setFont(p.font());
    }

    if (p.isContextualTabsVisible() && ((p.quickAccessBarPosition() == ARibbonBar::TopPosition &&
        p.tabBarPosition() == ARibbonBar::TopPosition) || (p.quickAccessBarPosition() == ARibbonBar::BottomPosition &&
        p.tabBarPosition() == ARibbonBar::BottomPosition)))
    {
        if (m_quickAccessBar->parentWidget() != m_ribbonTitleBarWidget)
        {
            m_ribbonTitleBarWidget->insertWidget(1, m_quickAccessBar);
            m_quickAccessBar->setVisible(true);
        }
    }
    else
    {
        m_nQucikAccessBarIndex = m_ribbonTitleBarWidget->indexOf(m_quickAccessBar);
        m_ribbonTitleBarWidget->removeWidget(m_quickAccessBar);
        m_quickAccessBar->setParent(material());
        m_quickAccessBar->setVisible(true);
        m_quickAccessBar->setPalette(m_ribbonTabBar->palette());
    }
}

void ARibbonBarPrivate::preparePages()
{
    QWidget* parent = getPageParent();
    for (int i = 0, count = m_ribbonTabBar->count(); i < count; ++i)
    {
        ARibbonPage* page = m_ribbonTabBar->getTab(i)->page();
        if (page->parent() != parent)
        {
            ARibbonPageVisibleAutoUpdater autoUpdater(ARibbonPagePrivate::_get(page));
            page->setParent(parent);
            bool current = i == m_ribbonTabBar->currentIndex();
            aproch_set_page_visible(page, current);
        }
    }

    if (m_banner != nullptr)
    {
        bool visible = !m_banner->isHidden();
        m_banner->setParent(parent);
        m_banner->setVisible(visible);
    }

    if (m_overflowMenuButton != nullptr)
    {
        bool visible = !m_overflowMenuButton->isHidden();
        m_overflowMenuButton->setParent(parent);
        m_overflowMenuButton->setVisible(visible);
    }
}

/*! \internal */
void ARibbonBarPrivate::currentChanged(int index)
{
    A_P(ARibbonBar);
    if (m_destoying)
        return;

    ARibbonPage* activePage = nullptr;
    for (int i = 0, count = m_ribbonTabBar->count(); i < count; ++i)
    {
        ARibbonPage* page = m_ribbonTabBar->getTab(i)->page();
        if (i == index)
        {
            activePage = page;
            CommonStyle* commonStyle = CommonStyle::ensureStyle();
            const bool animation = !isUpdating() && commonStyle != nullptr ? commonStyle->isAnimationEnabled() : true;
            updateActivePageLayout(activePage, animation);
            updateOverflowButton();
        }
        aproch_set_page_visible(page, page == activePage);
    }

    QRect updateRect = getPageFrameRect();
    if (p.isMinimized())
        m_pagePopup->update(updateRect);
    else
        p.update(updateRect);
 
    emit p.currentPageIndexChanged(index);
    emit p.currentPageChanged(activePage);
}

void ARibbonBarPrivate::currentAboutToBeChanged(int index, bool& changed)
{
    A_P(ARibbonBar);
    ARibbonPage* page = p.page(index);
    emit p.pageAboutToBeChanged(page, changed);
}

void ARibbonBarPrivate::activePageAnimationFinished()
{
    ARibbonPage* animatedPage = static_cast<ARibbonPage *>(m_activePageAnimation->targetObject());
    if (animatedPage == nullptr)
        return;
    animatedPage->setGeometry(m_pageContentRect);
    animatedPage->setGraphicsEffect(nullptr);
}

void ARibbonBarPrivate::activePageAnimationValueChanged(const QVariant&)
{
    ARibbonPage* animatedPage = qobject_cast<ARibbonPage*>(m_activePageAnimation->targetObject());
    if (animatedPage == nullptr)
        return;

    QGraphicsOpacityEffect* opacityEffect = qobject_cast<QGraphicsOpacityEffect *>(animatedPage->graphicsEffect());
    if (opacityEffect != nullptr)
    {
        qreal progress = m_activePageAnimation->duration();
        qreal current_progress = m_activePageAnimation->currentTime();
        qreal opacity = current_progress / progress;
        opacityEffect->setOpacity(opacity);
    }
}

void ARibbonBarPrivate::updateActivePageLayout(ARibbonPage* activePage, bool animated)
{
    ARibbonPage* animatedPage = static_cast<ARibbonPage*>(m_activePageAnimation->targetObject());
    if (animatedPage != nullptr)
        animatedPage->setGraphicsEffect(nullptr);
    m_activePageAnimation->stop();
    m_activePageAnimation->setTargetObject(nullptr);

    if (activePage == nullptr || isUpdating())
        return;

    updateFont(activePage);

    if (!animated)
    {
        activePage->setGeometry(m_pageContentRect);
        activePage->updateLayout();
        return;
    }
    
    QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect();
    opacityEffect->setOpacity(0);
    activePage->setGraphicsEffect(opacityEffect);
    
    QRect startRect = m_pageContentRect.translated(0, m_pageContentRect.height());
    QRect endRect = m_pageContentRect;
    activePage->setGeometry(startRect);
    activePage->updateLayout();

    m_activePageAnimation->setTargetObject(activePage);
    m_activePageAnimation->setPropertyName("geometry");
    m_activePageAnimation->setStartValue(startRect);
    m_activePageAnimation->setEndValue(endRect);
    m_activePageAnimation->setDuration(150);
    m_activePageAnimation->setEasingCurve(QEasingCurve::OutExpo);
    m_activePageAnimation->start();
}

QMenu* ARibbonBarPrivate::createContextMenu()
{
    A_P(ARibbonBar)
    if (p.isBackstageVisible())
        return nullptr;

    QMenu* popup = new QMenu(&p);
    QAction* action = nullptr;
    ARibbonQuickAccessBar* quickAccessBar = p.quickAccessBar();
    if (quickAccessBar && !quickAccessBar->isHidden())
    {
        QList<QAction*> actions = quickAccessBar->actions();
        if (actions.size() > 0)
        {
            action = popup->addAction(ARibbonBar::tr_compatible(QtnARibbonCustomizeQuickAccessToolBarDotString));
            action->setObjectName(strCustomizeQAToolBar);
            connect(action, SIGNAL(triggered()), this, SLOT(toggledCustomizeBar()));

            action = popup->addAction(p.quickAccessBarPosition() == ARibbonBar::TopPosition ? ARibbonBar::tr_compatible(QtnARibbonShowQuickAccessToolBarBelowString)
                : ARibbonBar::tr_compatible(QtnARibbonShowQuickAccessToolBarAboveString));
            action->setObjectName(strCustomizeARibbonBar);
            connect(action, SIGNAL(triggered()), this, SLOT(toggledQuickAccessBarPos()));
        }
        popup->addSeparator();
    }

    action = popup->addAction(ARibbonBar::tr_compatible(QtnARibbonCustomizeActionString));
    action->setObjectName(strCustomizeARibbonBar);
    connect(action, SIGNAL(triggered()), this, SLOT(toggledCustomizeBar()));

    action = popup->addAction(ARibbonBar::tr_compatible(QtnARibbonMinimizeActionString));
    action->setCheckable(true);
    action->setChecked(p.isMinimized());
    action->setVisible(p.isMinimizationEnabled());
    connect(action, SIGNAL(triggered()), this, SLOT(toggledMinimized()));
    return popup;
}

/*! \internal */
static QString findAccel(const QString &text)
{
    if (text.isEmpty())
        return text;

    int fa = 0;
    QChar ac;
    while ((fa = text.indexOf(QLatin1Char('&'), fa)) != -1) 
    {
        ++fa;
        if (fa < text.length()) 
        {
            // ignore "&&"
            if (text.at(fa) == QLatin1Char('&')) 
            {
                ++fa;
                continue;
            } 
            else 
            {
                ac = text.at(fa);
                break;
            }
        }
    }
    if (ac.isNull())
        return QString();
    return (QString)ac.toUpper();
}

/*! \internal */
void ARibbonBarPrivate::setKeyTipsActive(bool active)
{
    A_P(ARibbonBar);
    if (m_keyTipsActive == active)
        return;
    m_keyTipsReadyToActivate = false;
    if (active && (
        !p.keyTipsEnabled() || !p.style()->styleHint(QStyle::SH_MenuBar_AltKeyNavigation, 0, &p)))
        return;

    m_keyTipsActive = active;

    if (m_keyTipsActive)
    {
        QWidget* fw = QApplication::focusWidget();
        if (fw != &p)
            m_keyboardFocusWidget = fw;
        p.setFocus(Qt::MenuBarFocusReason);

        if (m_levels.indexOf(&p) == -1)
        {
            QWidget* w = &p;
#if 0
            if (p.systemButton() != nullptr &&
                p.systemButton()->backstage() != nullptr &&
                p.systemButton()->backstage()->isVisible())
            {
                w = p.systemButton()->backstage();
            }
#endif
            showKeyTips(w);
        }
    }
    else
    {
        hideKeyTips();
        m_levels.clear();

        if (m_keyboardFocusWidget)
        {
            if (QApplication::focusWidget() == &p)
                m_keyboardFocusWidget->setFocus(Qt::MenuBarFocusReason);
            m_keyboardFocusWidget = nullptr;
        }
    }
    emit p.keyTipsShowed(active);
}

/*! \internal */
void ARibbonBarPrivate::showKeyTips(QWidget* w)
{
    A_P(ARibbonBar);

    hideKeyTips();

    if (qobject_cast<ARibbonBar*>(w))
        createKeyTips();
    else if (qobject_cast<ARibbonPage*>(w))
        createGroupKeyTips();
    else if (QMenu* menu = qobject_cast<QMenu*>(w))
        createPopupKeyTips(menu);

    int count = m_keyTips.count();
    if (count == 0)
        return;

    for (int i = count - 1; i >= 0; i--)
    {
        ARibbonKeyTip* keyTip = m_keyTips.at(i);
        keyTip->setExplicit(!keyTip->getStringTip().isEmpty());

        if (keyTip->isExplicit())
            continue;

        QString strCaption = keyTip->getCaption();
        if (strCaption.isEmpty())
        {
            m_keyTips.removeAt(i);
            keyTip->hideAndDestroy();
            continue;
        }

        strCaption = strCaption.toUpper();
        keyTip->setCaption(strCaption);

        if (keyTip->getPrefix().isEmpty())
        {
            QString key = ::findAccel(strCaption);
            if (key.isEmpty())
            {
                if (!strCaption.isEmpty())
                    keyTip->setStringTip(strCaption.at(0));
            }
            else
                keyTip->setStringTip(key);
        }
        else
        {
            keyTip->setStringTip(keyTip->getPrefix());
        }
    }

    count = m_keyTips.count();
    for (int i = 0; i < count; i++)
    {
        ARibbonKeyTip* keyTip = m_keyTips.at(i);

        QString strTip = keyTip->getStringTip();

        QList<ARibbonKeyTip*> list;
        list.append(keyTip);

        QString strUsed(QStringLiteral("& "));
        for ( int j = i + 1; j < count; j++)
        {
            ARibbonKeyTip* keyTipWidget = m_keyTips.at(j);;
            if (keyTipWidget->getStringTip().at(0) == strTip.at(0))
            {
                list.append(keyTipWidget);
                if (keyTipWidget->getStringTip().length() == 2)
                    strUsed += keyTipWidget->getStringTip().at(1);
            }
        }

        if (list.count() < 2)
            continue;

        int k = 1;
        for (int j = 0; j < list.count(); j++)
        {
            ARibbonKeyTip* keyTipWidget = list.at(j);
            if (keyTipWidget->getStringTip().length() > 1)
                continue;

            QChar chAccel(0);
            QString strCaption = keyTipWidget->getCaption();
            for (int n = 0; n < strCaption.length() - 1; n++)
            {
                if ((strCaption.at(n) == QLatin1Char(' ')) && (strUsed.indexOf(strCaption.at(n + 1)) == -1))
                {
                    chAccel = strCaption.at(n + 1);
                    strUsed += chAccel;
                    break;
                }
            }

            if (chAccel.isNull())
            {
                int start = strCaption.at(0) == QLatin1Char('&') ? 1 : 0;
                if (strCaption.at(start) == keyTipWidget->getStringTip().at(0))
                    start++;

                for (int n = start; n < strCaption.length(); n++)
                {
                    if (strUsed.indexOf(strCaption.at(n)) == -1)
                    {
                        chAccel = keyTipWidget->getCaption().at(n);
                        strUsed += chAccel;
                        break;
                    }
                }
            }
            if (chAccel.isNull())
            {
                QString str = QStringLiteral("%1").arg(k);
                chAccel = str.at(0);
                k++;
            }
            keyTipWidget->setStringTip(keyTipWidget->getStringTip() + chAccel);
        }
    }

    calcPositionKeyTips();
    p.setFocus();
}

/*! \internal */
void ARibbonBarPrivate::hideKeyTips()
{
    if (m_keyTips.count() == 0)
        return;
    QList<ARibbonKeyTip*> list = m_keyTips;
    m_keyTips.clear();

    for (int i = 0; i < list.count(); i++)
    {
        ARibbonKeyTip* keyTip = list.at(i);
        keyTip->hideAndDestroy();
    }
    m_countKey = 0;
}

/*! \internal */
void ARibbonBarPrivate::createKeyTips()
{
    A_P(ARibbonBar);
    aproch_updateTitleBarWidgetPosition(p.parentWidget());

    m_levels.push(&p);
    QList<QToolButton*> list = m_quickAccessBar->findChildren<QToolButton*>();
    for (int i = 0, count = list.count(); i < count; i++)
    {
        QToolButton* button = list.at(i);
        if (!button->isVisible() || button->objectName() == QStringLiteral("QtnARibbonQuickAccessButton"))
            continue;

        int k = 1;
        QString strCaption;
        if (QAction* act = button->defaultAction())
        {
            strCaption = act->property(_aproch_KeyTip).toString();
            if (strCaption.isEmpty())
            {
                QList<QAction*> listActs = m_quickAccessBar->actions();
                int index = listActs.indexOf(act);
                if (index != -1)
                    k = index;
            }
        }

        if (strCaption.isEmpty())
        {
            if (k < 10)
                strCaption = QStringLiteral("%1").arg(k);
            else if (k < 19)
                strCaption = QStringLiteral("%1").arg(k - 9);
            else 
                break;
        }

        m_keyTips.append(new ARibbonKeyTip(&p, button, strCaption, button->mapToGlobal(QPoint(button->rect().center().x(), button->rect().bottom() - 11)), 
            ARibbonKeyTip::AlignTipCenter | ARibbonKeyTip::AlignTipTop, button->isEnabled()));
    }

    if (m_ribbonTabBar && m_ribbonTabBar->isVisible())
    {
        for (int i = 0, count = m_ribbonTabBar->count(); i < count; i++)
        {
            ARibbonTab* tab = m_ribbonTabBar->getTab(i);

            bool propKeyTip = true;
            QString strCaption = tab->defaultAction()->property(_aproch_KeyTip).toString();

            if (strCaption.isEmpty())
            {
                strCaption = tab->text();
                propKeyTip = false;
            }

            if (!m_keyTipsComplement && !propKeyTip && ::findAccel(strCaption).isEmpty())
                strCaption = QString();

            if (!strCaption.isEmpty() && tab->isEnabled() && tab->isVisible())
            {
                QPoint pt(tab->rect().center().x(), tab->rect().bottom() - 9);
                ARibbonKeyTip* keyTip = new ARibbonKeyTip(&p, tab, strCaption, tab->mapToGlobal(pt), 
                    ARibbonKeyTip::AlignTipCenter | ARibbonKeyTip::AlignTipTop, true);
                m_keyTips.append(keyTip);
            }
        }
    }

    if (m_systemButton != nullptr)
    {
        QPoint pt(m_systemButton->rect().center().x(), m_systemButton->rect().center().y());

        QString strCaption = m_systemButton->defaultAction()->property(_aproch_KeyTip).toString();
        if (strCaption.isEmpty())
            strCaption = m_systemButton->text();

        m_keyTips.append(new ARibbonKeyTip(&p, m_systemButton, strCaption, 
            m_systemButton->mapToGlobal(pt), ARibbonKeyTip::AlignTipCenter | ARibbonKeyTip::AlignTipVCenter, m_systemButton->isEnabled()));
    }
}

/*! \internal */
void ARibbonBarPrivate::createGroupKeyTips()
{
    A_P(ARibbonBar);
    ARibbonPage* page = p.currentPage();
    if (page == nullptr)
        return;

    m_levels.push(page);

    for (int i = 0, count = page->groupCount(); i < count; i++)
    {
        ARibbonGroup* group = page->group(i);
        bool visible = group->isVisible();

        QList<QWidget*> list = group->findChildren<QWidget*>();
        for (int j = 0; j < list.count(); j++)
        {
            QWidget* widget = list.at(j);
            createWidgetKeyTips(group, widget, QString(), page->rect().translated(page->mapToGlobal(page->rect().topLeft())), visible);
        }
    }
}

/*! \internal */
static QString getTextAction(const ARibbonGroup& group, QWidget& widget, bool& propKeyTip)
{
    QString strCaption;
    foreach (QAction* action, group.actions())
    {
        if (QWidgetAction* widgetAction = qobject_cast<QWidgetAction*>(action))
        {
            if (&widget == widgetAction->defaultWidget())
            {
                strCaption = widgetAction->property(_aproch_KeyTip).toString();
                if (strCaption.isEmpty())
                {
                    strCaption = widgetAction->text();
                    propKeyTip = false;
                }
                break;
            }
        }
    }
    return strCaption;
}

/*! \internal */
void ARibbonBarPrivate::createWidgetKeyTips(ARibbonGroup* group, QWidget* widget, const QString& prefix, const QRect& rcGroups, bool visible)
{
    A_P(ARibbonBar);
    if (widget == nullptr)
        return;

    bool propKeyTip = true;
    QString strCaption;
    if (QAbstractButton* button = qobject_cast<QAbstractButton*>(widget))
    {
        strCaption = ::getTextAction(*group, *button, propKeyTip);
        if (strCaption.isEmpty())
        {
            if (QToolButton* toolButton = qobject_cast<QToolButton*>(button))
            {
                if (QAction* action = toolButton->defaultAction())
                {
                    strCaption = action->property(_aproch_KeyTip).toString();
                    if (strCaption.isEmpty())
                    {
                        strCaption = action->text();
                        propKeyTip = false;
                    }
                }

                if (strCaption.isEmpty())
                {
                    if (ARibbonGallery* ribGallery = qobject_cast<ARibbonGallery*>(toolButton->parentWidget()))
                    {
                        if (toolButton->property(_aproch_PopupButtonGallery).toBool())
                        {
                            if (OfficePopupMenu* menu = ribGallery->popupMenu())
                            {
                                if (QAction* act = menu->defaultAction())
                                    strCaption = act->text();
                            }
                            if (strCaption.isEmpty())
                                strCaption = ::getTextAction(*group, *toolButton->parentWidget(), propKeyTip);
                        }
                    }
                    else if (strCaption.isEmpty())
                        strCaption = group->title();
                }
            }
            else
            {
                strCaption = ::getTextAction(*group, *widget, propKeyTip);
                if (strCaption.isEmpty())
                    strCaption = button->text();
            }
        }
    }

    bool textWidget = true;
    if (ARibbonGallery* gallery = qobject_cast<ARibbonGallery*>(widget))
        textWidget = !gallery->popupMenu();

    if (strCaption.isEmpty() && textWidget)
        strCaption = ::getTextAction(*group, *widget, propKeyTip);

    if (!m_keyTipsComplement && !propKeyTip && ::findAccel(strCaption).isEmpty())
        strCaption = QString();

    if (strCaption.isEmpty())
        return;

    QRect rc = widget->rect();
    rc.translate(widget->mapToGlobal(rc.topLeft()));

    if (rc.isEmpty())
        return;

    QPoint pt(rc.left() + 21, rc.bottom() - 4);
    uint align = ARibbonKeyTip::AlignTipTop;

    QStyleOptionGroupBox opt;
    opt.initFrom(&p);
    opt.text = group->title();

    QRect rcGroup = group->rect();
    rcGroup.translate(group->mapToGlobal(rcGroup.topLeft()));

    const int heightCaptionGroup = groupTitleHeight();
    QRect rcCaption(QPoint(rcGroup.left(), rcGroup.bottom() - heightCaptionGroup + 2), QPoint(rcGroup.right(), rcGroup.bottom()));
    rcGroup.setBottom(rcCaption.top());

    QFontMetrics fm(p.font());
    const int h = fm.boundingRect(QLatin1Char('X')).height() / 2;

    if (rc.intersects(rcCaption))
    {
        pt = QPoint(rc.center().x(), rcGroups.bottom() - 6);
        align = ARibbonKeyTip::AlignTipTop | ARibbonKeyTip::AlignTipCenter;
    }
    else if ((Qt::ToolButtonStyle)widget->property("toolButtonStyle").toInt() == Qt::ToolButtonTextUnderIcon && rc.height() > rcGroup.height() * 0.5)
    {
        pt = QPoint(rc.center().x(), rc.bottom() + h);
        align = ARibbonKeyTip::AlignTipCenter | ARibbonKeyTip::AlignTipVCenter;
    }
    else if (qobject_cast<ARibbonGallery*>(widget))
    {
        pt = QPoint(rc.right(), rcGroup.bottom() - 2);
        align = ARibbonKeyTip::AlignTipCenter | ARibbonKeyTip::AlignTipVCenter;
    }
    else if (rc.center().y() < rcGroup.top() + rcGroup.height() * 1 / 3)
    {
        /// top
        pt = QPoint(rc.left() + 11, rcGroup.top());
        align = ARibbonKeyTip::AlignTipLeft | ARibbonKeyTip::AlignTipVCenter;
    }
    else if (rc.center().y() > rcGroup.top() + rcGroup.height() * 2 / 3)
    {
        pt = QPoint(rc.left() + 11, rcGroup.bottom());
        align = ARibbonKeyTip::AlignTipLeft | ARibbonKeyTip::AlignTipVCenter;
    }
    else
    {
        if (qobject_cast<ARibbonButton*>(widget) && qobject_cast<ARibbonToolBarControl*>(widget->parentWidget()))
            pt = QPoint(rc.left() + 11, rc.bottom() + h * 2);
        else
            pt = QPoint(rc.left() + 11, rcGroup.center().y() - 1);
        align = ARibbonKeyTip::AlignTipLeft | ARibbonKeyTip::AlignTipVCenter;
    }

    if (qobject_cast<QCheckBox*>(widget) || qobject_cast<QRadioButton*>(widget))
        pt.setX(rc.left() + 6);

    ARibbonKeyTip* keyTip = new ARibbonKeyTip(&p, widget, strCaption, pt, align, widget->isEnabled());

    if (!prefix.isEmpty())
        keyTip->setPrefix(prefix);

    QRect rcIntersect;
    rcIntersect = rcGroups.intersected(rc);
    keyTip->setVisibleTip(visible && (rcIntersect == rc) && (widget->isVisible()));
    m_keyTips.append(keyTip);
}

/*! \internal */
bool ARibbonBarPrivate::createPopupKeyTips(QMenu* menu)
{
    A_P(ARibbonBar);
    menu = menu != nullptr ? menu : qobject_cast<QMenu*>(QApplication::activePopupWidget());
    if (menu != nullptr)
    {
        m_levels.push(menu);

        QList<QAction*> list = menu->actions();
        for (int i = 0; i < list.count(); i++)
        {
            QAction* action = list.at(i);
            bool propKeyTip = true;
            QString strCaption = action->property(_aproch_KeyTip).toString();

            if (strCaption.isEmpty())
            {
                strCaption = action->text();
                propKeyTip = false;
            }

            if (!m_keyTipsComplement && !propKeyTip && ::findAccel(strCaption).isEmpty())
                strCaption = QString();

            if (!action->isVisible() || strCaption.isEmpty())
                continue;

            QRect rect = menu->actionGeometry(action);
            QPoint pt(rect.left() + 12, rect.bottom() - 11);
            ARibbonKeyTip* keyTip = new ARibbonKeyTip(&p, menu, strCaption, menu->mapToGlobal(pt), ARibbonKeyTip::AlignTipTop, action->isEnabled(), action);

            m_keyTips.append(keyTip);
        }

        QList<QWidget*> listWidget = menu->findChildren<QWidget*>();
        for (int i = 0; i < listWidget.count(); i++)
        {
            QWidget* widget = listWidget.at(i);

            bool propKeyTip = true;
            QString strCaption;

            if (QToolButton* toolButton = qobject_cast<QToolButton*>(widget))
            {
                if (QAction* action = toolButton->defaultAction())
                {
                    strCaption = action->property(_aproch_KeyTip).toString();
                    if (strCaption.isEmpty())
                    {
                        strCaption = action->text();
                        propKeyTip = false;
                    }
                }
            }

            if (!m_keyTipsComplement && !propKeyTip && ::findAccel(strCaption).isEmpty())
                strCaption = QString();

            if (!widget->isVisible() || strCaption.isEmpty())
                continue;

            QPoint pt(widget->rect().left() + 12, widget->rect().bottom() - 11);
            ARibbonKeyTip* keyTip = new ARibbonKeyTip(&p, widget, strCaption, widget->mapToGlobal(pt), 
                ARibbonKeyTip::AlignTipTop, widget->isEnabled());
            m_keyTips.append(keyTip);
        }
        return true;
    }
    return false;
}

/*! \internal */
void ARibbonBarPrivate::calcPositionKeyTips()
{
    A_P(ARibbonBar);
    for (int i = 0, count = m_keyTips.count(); i < count; i++)
    {
        ARibbonKeyTip* keyTip = m_keyTips.at(i);
        if (!keyTip->isVisibleTip())
            continue;

        QSize sz(0, 0);
        QFontMetrics fontMetrics(keyTip->font());
        QRect rect = fontMetrics.boundingRect(keyTip->getStringTip());
        sz = QSize(qMax(rect.width() + ACommonStylePrivate::dpiScaled(6, &p), 16),
            qMax(rect.height() + ACommonStylePrivate::dpiScaled(4, &p), 15));

        QPoint pt = keyTip->posTip();
        uint flagAlign = keyTip->getAlign();

        QRect rc(QPoint(pt.x() - ((flagAlign & ARibbonKeyTip::AlignTipCenter) ?  sz.width() * 0.5 : (flagAlign & ARibbonKeyTip::AlignTipRight) ? sz.width() : 0),
            pt.y() - (flagAlign & ARibbonKeyTip::AlignTipVCenter ?  sz.height() * 0.5 : flagAlign & ARibbonKeyTip::AlignTipBottom ? sz.height() : 0)), sz);

        keyTip->setGeometry(rc);
        keyTip->show();
    }
}

int ARibbonBarPrivate::rowItemHeight() const
{
    return m_ribbonItemHeight;
}

int ARibbonBarPrivate::rowItemCount() const
{
    A_P(const ARibbonBar);
    if (p.simplifiedMode())
        return 1;
    return m_rowItemCount;
}

void ARibbonBarPrivate::setRowItemCount(const int& nCount)
{
    m_rowItemCount = nCount;
}

bool ARibbonBarPrivate::pressTipCharEvent(const QKeyEvent* key)
{
    A_P(ARibbonBar);
    QString str = key->text().toUpper();
    if (str.isEmpty())
        return false;
    for (int i = 0; i < m_keyTips.count(); i++)
    {
        ARibbonKeyTip* keyTip = m_keyTips.at(i);
        int length = keyTip->getStringTip().length();
        if (length > m_countKey)
        {
            if (keyTip->getStringTip().at(m_countKey) == str.at(0))
            {
                if (length - 1 > m_countKey)
                {
                    m_countKey++;

                    QString str = keyTip->getStringTip().left(m_countKey);
                    for (int j = m_keyTips.count() - 1; j >= 0; j--)
                    {
                        keyTip = m_keyTips.at(j);
                        if (keyTip->getStringTip().left(m_countKey) != str)
                        {
                            m_keyTips.removeAt(j);
                            keyTip->hideAndDestroy();
                        }
                    }
                }
                else
                {
                    if (QAbstractButton* absButton = qobject_cast<QAbstractButton*>(keyTip->getOwner()))
                    {
                        if (keyTip->isEnabledTip() && keyTip->isVisibleTip())
                        {
                            bool showMenu = false;
                            if (QToolButton* button = qobject_cast<QToolButton*>(absButton))
                            {
                                if (button->popupMode() == QToolButton::InstantPopup)
                                {
                                }
                                else if (button->popupMode() == QToolButton::MenuButtonPopup)
                                {
                                    button->showMenu();
                                    showMenu = true;
                                }
                            }
                            if (!showMenu)
                                absButton->animateClick();

                            setKeyTipsActive(false);
                        }
                        return false;
                    }
                    else if (qobject_cast<QMenu*>(keyTip->getOwner()) && 
                        keyTip->isEnabledTip() && keyTip->isVisibleTip())
                    {
                        if (QAction* action = keyTip->getAction())
                        {
                            if (qobject_cast<ARibbonPageSystemPopup*>(action->menu()))
                            {
                                if (QMenu* activSystemMenu = qobject_cast<ARibbonSystemMenu*>(QApplication::activePopupWidget()))
                                    activSystemMenu->setActiveAction(action);
                                return false;
                            }
                            else
                            {
                                action->trigger();
                                QWidget* owner = keyTip->getOwner();
                                owner->close();

                                if (qobject_cast<ARibbonPageSystemPopup*>(owner))
                                {
                                    if (QWidget* activPopup = qobject_cast<ARibbonSystemMenu*>(QApplication::activePopupWidget()))
                                        activPopup->close();
                                }
                                setKeyTipsActive(false);
                                return false;
                            }
                        }
                    } 
                    else if (qobject_cast<ARibbonTab*>(keyTip->getOwner()))
                    {
                        if (ARibbonTab* tab = qobject_cast<ARibbonTab*>(keyTip->getOwner()))
                        {
                            int index = m_ribbonTabBar->tabIndex(tab);
                            if (index != -1)
                            {
                                {
                                    ARibbonBarAutoUpdater autoUpdater(&p);
                                    p.setCurrentPageIndex(index);
                                    if (p.isMinimized())
                                        showPagePopup(p.currentPage());
                                }
                                showKeyTips(p.page(index));
                            }
                        }
                        return false;
                    }
                    else if (keyTip->getOwner() && keyTip->isEnabledTip() && keyTip->isVisibleTip())
                    {
                        keyTip->getOwner()->setFocus();
                        setKeyTipsActive(false);
                    }
                }
                return false;
            }
        }
    }
    return false;
}

/*! \reimp */
bool ARibbonBarPrivate::eventFilter(QObject* obj, QEvent* event)
{
    A_P(ARibbonBar);
    switch (event->type())
    {
        case QEvent::WindowStateChange:
        {
            if (qobject_cast<QMdiSubWindow*>(obj) != nullptr)
            {
                QWindowStateChangeEvent* changeEvent = static_cast<QWindowStateChangeEvent*>(event);
                if (changeEvent->isOverride())
                    p.updateLayout();
            }
        }
        break;
        case QEvent::WindowActivate:
        case QEvent::WindowDeactivate:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseButtonDblClick:
        case QEvent::NonClientAreaMouseButtonPress:
        case QEvent::NonClientAreaMouseButtonRelease:
        case QEvent::NonClientAreaMouseButtonDblClick:
        {
            if (isKeyTipsActive())
                setKeyTipsActive(false);
        }
        break;
        case QEvent::Shortcut:
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            Q_UNUSED(keyEvent);
            m_keyTipsReadyToActivate = false;
        }
        break;
        case QEvent::KeyPress:
        {
            if (!isKeyTipsActive())
            {
                QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
                m_keyTipsReadyToActivate = keyEvent->key() == Qt::Key_Alt || keyEvent->key() == Qt::Key_Meta;
                break;
            }

            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() != Qt::Key_Escape)
            {
                if (qobject_cast<QMenu*>(obj))
                    return !pressTipCharEvent(keyEvent);
                else
                    return pressTipCharEvent(keyEvent);
            }
            else
            {
                if (m_levels.size() > 0)
                    m_levels.pop();
                if (m_levels.size() > 0)
                    showKeyTips(m_levels.pop());
                else
                    setKeyTipsActive(false);

                return true;
            }
        }
        break;
        case QEvent::KeyRelease:
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Alt || keyEvent->key() == Qt::Key_Meta)
            {
                if (m_keyTipsReadyToActivate && !isKeyTipsActive())
                {
                    setKeyTipsActive(true);
                    return true;
                }
                else if (isKeyTipsActive())
                {
                    setKeyTipsActive(false);
                    return true;
                }
            }
        }
        break;

        case QEvent::Show:
            {
                QMenu* menu = qobject_cast<QMenu*>(obj);
                if (m_levels.size() > 0 && menu != nullptr)
                {
                    if (m_levels.size() > 0 ||
                        ((qobject_cast<ARibbonPageSystemPopup*>(obj) || qobject_cast<ARibbonSystemMenu*>(obj))))
                    {
                        if (m_levels.indexOf(menu) == -1)
                        {
                            hideKeyTips();
                            showKeyTips(menu);
                            return true;
                        }
                    }
                }
            }
            break;
        case QEvent::Close:
            {
                QMenu* menu = qobject_cast<QMenu*>(obj);
                if (m_levels.indexOf(menu) > 0)
                {
                    m_levels.pop();
                    if (m_levels.count() > 0)
                        showKeyTips(m_levels.pop());
                    else
                        setKeyTipsActive(false);

                    return true;
                }
            }
            break;

        default:
            break;
    };
    return false;
}

QRect ARibbonBarPrivate::getPageFrameRect() const
{
    return m_pageWindowRect;
}

/*! \internal */
void ARibbonBarPrivate::createCustomizeDialog()
{
    A_P(ARibbonBar)
    if (m_customizeDialog != nullptr)
        return;

    m_customizeDialog = new ARibbonCustomizeDialog(p.parentWidget());
    // Create "Customize QAT" page:
    ARibbonQuickAccessBarCustomizePage* quickAccessBarCustomizePage = new ARibbonQuickAccessBarCustomizePage(&p);
    m_customizeDialog->addPage(quickAccessBarCustomizePage);
    // Create "Customize ARibbon" page:
    ARibbonBarCustomizePage* ribbonBarCustomizePage = new ARibbonBarCustomizePage(&p);
    m_customizeDialog->addPage(ribbonBarCustomizePage);
}

/*! \internal */
void ARibbonBarPrivate::toggledCustomizeBar()
{
    A_P(ARibbonBar);
    p.showCustomizeDialog();
}

/*! \internal */
void ARibbonBarPrivate::toggledQuickAccessBarPos()
{
    A_P(ARibbonBar);
    p.setQuickAccessBarPosition(p.quickAccessBarPosition() == ARibbonBar::TopPosition ? ARibbonBar::BottomPosition : ARibbonBar::TopPosition);
}

/*! \internal */
void ARibbonBarPrivate::toggledMinimized()
{
    A_P(ARibbonBar);
    p.setMinimized(!p.isMinimized());
}

/*! \internal */
void ARibbonBarPrivate::toggledSimplifiedMode(bool value)
{
    A_P(ARibbonBar);
    p.setSimplifiedMode(value);
}

/*!
\class ARibbonBar
\inmodule QtitanARibbon

\brief ARibbonBar class represents a component ARibbon UI used in Microsoft Office 2007/2010/2013.
Please note, after the ribbon bar has been added to the main window you need to setup the ARibbon's style (or MS-Office Style) to the application.
To do that please use the following code:
\code
QApplication app(...);
app.setStyle(new Qtitan::ARibbonStyle());
\endcode
or
\code
QApplication app(...);
app.setStyle("ribbonstyle");
\endcode

Note: To display ARibbonBar correctly the ARibbon style (or Office style) should be installed into your application by calling qApp.setStyle(new Qtitan::ARibbonStyle()); or app.setStyle("ribbonstyle");
ARibbonBar does not work with platform's specific or standard Qt styles.
*/

/*!
\fn void ARibbonBar::minimizationChanged(bool minimized);
The signal is used to get notification about changes of the minimization state of the ribbon bar.
The parameter \a minimized contains the current state of the minimization.
*/

/*!
\fn void ARibbonBar::currentPageIndexChanged(int index);
The signal is emitted when the active page at the ribbon bar was changed.
Parameter \a index - the index of the new active page.
*/

/*!
\fn void ARibbonBar::currentPageChanged(ARibbonPage * page);
The signal is emitted when the active page at the ribbon bar was changed.
Parameter \a page is a reference to the active ribbon page object.
*/

/*!
Constructs ARibbonBar object with the given \a parent.
*/
ARibbonBar::ARibbonBar(QWidget* parent)
    : QMenuBar(parent)
{
    initARibbonResource();
    A_INIT_PRIVATE(ARibbonBar);
    A_D(ARibbonBar);
    d.init();
    CommonStyle::ensureStyle();
    ensurePolished();
}

/*!
Destructor of the ARibbonBar object.
*/
ARibbonBar::~ARibbonBar()
{
    A_D(ARibbonBar);
    d.m_destoying = true;
    clearPages();
    A_FINI_PRIVATE();
}

/*!
Adds a new page on ARibbonBar. Parameter \a text is used to initialize the page title. 
The function returns a pointer to the new object ARibbonPage.
*/
ARibbonPage* ARibbonBar::addPage(const QString& text)
{
    return insertPage(-1, text);
}

/*!
Adds an existing \a page to the ARibbonBar. \a page - is a pointer to the ARibbonPage object.
*/
void ARibbonBar::addPage(ARibbonPage* page)
{
    insertPage(-1, page);
}

/*!
Creates a new page with the \a text title and insert it into the position \a index. 
The function returns a pointer to the new object ARibbonPage.
*/
ARibbonPage* ARibbonBar::insertPage(int index, const QString& text)
{
    ARibbonPage* page = new ARibbonPage();
    page->setTitle(text);
    insertPage(index, page);
    return page;
}

/*!
Inserts an existing \a page at the specified position \a index of the ARibbonBar. 
The \a index - is the position of the page, \a page - a pointer to the new object ARibbonPage.
*/
void ARibbonBar::insertPage(int index, ARibbonPage* page)
{
    Q_ASSERT(page != nullptr);
    A_D(ARibbonBar);
    if (d.pageIndex(page) != -1)
    {
        return;
    }
    ARibbonBarAutoUpdater autoUpdater(this);
    d.insertPage(index, page);
}

/*!
Moves the \a page from current position to the position \a newIndex.
*/
void ARibbonBar::movePage(ARibbonPage* page, int newIndex)
{
    A_D(ARibbonBar);
    movePage(d.pageIndex(page), newIndex);
}

/*!
Moves the page in the position \a index to the position \a newIndex.
*/
void ARibbonBar::movePage(int index, int newIndex)
{
    A_D(ARibbonBar);
    if (d.isIndexValid(index) && d.isIndexValid(newIndex))
    {
        ARibbonBarAutoUpdater autoUpdater(this);
        d.m_ribbonTabBar->moveTab(index, newIndex);
    }
}

/*!
Removes an existing \a page. Parameter \a page is a pointer to page that will be removed. 
The object that is referenced by \a page will be destroyed.
\sa detachPage
*/
void ARibbonBar::removePage(ARibbonPage* page)
{
    A_D(ARibbonBar);
    int index = d.pageIndex(page);
    if (index != -1)
        removePage(index);
}

/*!
Removes the page at the given \a index. 
An object that represents the page at position \a index will be destroyed.
\sa detachPage
*/
void ARibbonBar::removePage(int index)
{
    A_D(ARibbonBar);
    ARibbonBarAutoUpdater autoUpdater(this);
    d.removePage(index);
}

/*!
Removes page from the ribbon by the given \a page.
Important: The function does not destroy the page.
\sa removePage
*/
void ARibbonBar::detachPage(ARibbonPage* page)
{
    A_D(ARibbonBar);
    int index = d.pageIndex(page);
    if (index != -1)
        detachPage(index);
}

/*!
Removes page from the ribbon by the given position \a index.
Important: The function does not destroy the page at position \a index.
\sa removePage
*/
void ARibbonBar::detachPage(int index)
{
    A_D(ARibbonBar);
    ARibbonBarAutoUpdater autoUpdater(this);
    d.removePage(index, false);
}

/*!
Removes all pages from the ribbon bar.
*/
void ARibbonBar::clearPages()
{
    ARibbonBarAutoUpdater autoUpdater(this);
    while (pageCount() > 0)
        removePage(0);
}

/*!
Returns the current state of showing Key-Tips. If true then Key-Tips visible on screen at the moment.
*/ 
bool ARibbonBar::isKeyTipsShowing() const
{
    A_D(const ARibbonBar);
    return d.m_keyTipsActive;
}

/*!
Returns the flag is mechanizm of Key-Tips enabled or not.
*/ 
bool ARibbonBar::keyTipsEnabled() const
{
    A_D(const ARibbonBar);
    return d.m_keyTipsEnabled;
}

/*!
Enabled or disabled mechanizm of Key-Tips.
*/ 
void ARibbonBar::setKeyTipsEnable(bool enable)
{
    A_D(ARibbonBar);
    d.m_keyTipsEnabled = enable;
}


/*!
Returns the policy of assignment for KeyTips of the buttons and other controls. If return value
is true then the keytips assigned automatically, based on keyboard shortcuts <&File, &Open...>. 
If return value is false keytips will be assigned only if user set it by ARibbonBar::setKeyTip() function.
\sa setKeyTip()
*/ 
bool ARibbonBar::isKeyTipsComplement() const
{
    A_D(const ARibbonBar);
    return d.m_keyTipsComplement;
}

/*!
The function sets the policy of assignment for KeyTips of the buttons and other controls. If the parameter \a complement
is true then the keytips assigned automatically, based on keyboard shortcuts <&File, &Open...>. 
If parameter \a complement is false keytips will be assigned only if user set it by ARibbonBar::setKeyTip() function.
\sa setKeyTip()
*/ 
void ARibbonBar::setKeyTipsComplement(bool complement)
{
    A_D(ARibbonBar);
    d.m_keyTipsComplement = complement;
}

/*!
Sets the \a keyTip for the control associated with given \a action. Function is taken effect if ARibbonBar::isKeyTipsComplement() return true.
\sa setKeyTipsComplement()
*/
void ARibbonBar::setKeyTip(QAction* action, const QString& keyTip)
{
    if (!action)
    {
        Q_ASSERT(false);
        return;
    }
    action->setProperty(_aproch_KeyTip, keyTip);
}

/*!
This property holds whether the user can move the tabs within the ribbontabbar area.
\sa setMovableTabs(bool)
*/
bool ARibbonBar::isMovableTabs() const
{
    A_D(const ARibbonBar);
    return d.m_ribbonTabBar->isMovable();
}

void ARibbonBar::setMovableTabs(bool movable)
{
    A_D(ARibbonBar);
    d.m_ribbonTabBar->setMovable(movable);
}

Qt::TextElideMode ARibbonBar::tabsElideMode() const
{
    A_D(const ARibbonBar);
    return d.m_ribbonTabBar->elideMode();
}

void ARibbonBar::setTabsElideMode(Qt::TextElideMode mode)
{
    A_D(ARibbonBar);
    return d.m_ribbonTabBar->setElideMode(mode);
}

/*!
Sets custom logo \a pixmap for the ribbon bar. Logo can be aligned at the left or at the right of the ribbon bar. 
*/
void ARibbonBar::setLogoPixmap(const QPixmap& pixmap, Qt::AlignmentFlag alignment)
{
    A_D(ARibbonBar);
    d.m_banner->setVisible(!pixmap.isNull());
    d.m_banner->setPixmap(pixmap);
    d.m_banner->setAlignment(alignment);
    d.changed();
}

/*!
Returns the logo pixmap of the ribbon bar.
*/
QPixmap ARibbonBar::logoPixmap() const
{
    A_D(const ARibbonBar);
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    return d.m_banner->pixmap();
#else
    return *d.m_banner->pixmap();
#endif
}

/*!
Sets the background image for the title of the ribbon bar. 
*/
void ARibbonBar::setTitleBackground(const QPixmap& pixmap)
{
    A_D(ARibbonBar);
    d.m_titleBackground = pixmap;
    update();
}

/*!
Returns the current background image of the ribbon bar title. 
*/
const QPixmap& ARibbonBar::titleBackground() const
{
    A_D(const ARibbonBar);
    return d.m_titleBackground;
}

/*!
Returns pointer to the quick access bar. See description of class ARibbonQuickAccessBar for more details.
*/
ARibbonQuickAccessBar* ARibbonBar::quickAccessBar() const
{
    A_D(const ARibbonBar);
    return d.m_quickAccessBar;
}

/*!
Returns pointer to the search bar. See description of class ARibbonSearchBar for more details.
*/
ARibbonSearchBar* ARibbonBar::searchBar() const
{
    A_D(const ARibbonBar);
    return d.m_ribbonTitleBarWidget->searchBar();
}

/*!
Sets quick access bar \a position on the ribbon bar.
*/
void ARibbonBar::setQuickAccessBarPosition(ARibbonBar::BarPosition position)
{
    A_D(ARibbonBar);
    if (d.m_quickAccessBarPosition != position)
    {
        d.m_quickAccessBarPosition = position;
        d.changed();
    }
}

/*!
Retruns current position of quick access bar on the ribbon bar.
*/
ARibbonBar::BarPosition ARibbonBar::quickAccessBarPosition() const
{
    A_D(const ARibbonBar);
    return d.m_quickAccessBarPosition;
}

/*!
Sets tabs \a position on the ribbon bar.
*/
void ARibbonBar::setTabBarPosition(ARibbonBar::BarPosition position)
{
    A_D(ARibbonBar);
    if (d.m_tabBarPosition != position)
    {
        d.m_tabBarPosition = position;
        d.changed();
    }
}

/*!
Retruns current position of tabs on the ribbon bar.
*/
ARibbonBar::BarPosition ARibbonBar::tabBarPosition() const
{
    A_D(const ARibbonBar);
#if 0
    ARibbonBar::TabBarPosition pos = d.m_tabBarPosition;
    if (pos == ARibbonBar::TabBarBottomPosition && isMinimized())
        pos = ARibbonBar::TabBarTopPosition;
    return pos;
#endif
    return d.m_tabBarPosition;
}

void ARibbonBar::setSearchBarAppearance(ARibbonBar::SearchBarAppearance appearance)
{
    A_D(ARibbonBar);
    return d.m_ribbonTitleBarWidget->setSearchBarAppearance(appearance);
}

ARibbonBar::SearchBarAppearance ARibbonBar::searchBarAppearance() const
{
    A_D(const ARibbonBar);
    return d.m_ribbonTitleBarWidget->searchBarAppearance();
}


/*! \internal */
void ARibbonBar::setMinimizedFlag(bool flag)
{
    A_D(ARibbonBar);
    if (!isMinimizationEnabled())
        return;
    if (d.m_minimized != flag)
    {
        d.m_minimized = flag;
        d.hidePagePopup();

        d.preparePages();
        d.changed(true);
        emit minimizationChanged(d.m_minimized);
    }
}

/*!
Minimizes the ribbon bar if it was maximized before.
This is equivalent of setMinimized(true) or setMaximized(false) call.
*/
void ARibbonBar::minimize()
{
    setMinimizedFlag(true);
}

/*!
Returns true if ribbon bar is minimized. Otherwise returns false.
*/
bool ARibbonBar::isMinimized() const
{
    A_D(const ARibbonBar);
    return d.m_minimized;
}

/*!
Sets minimized \a flag for the ribbon bar.
*/
void ARibbonBar::setMinimized(bool flag)
{
    setMinimizedFlag(flag);
}

/*!
Maximizes the ribbon bar if it was minimized before.
This is equivalent of setMaximized(true) or setMinimized(false) call.
*/
void ARibbonBar::maximize()
{
    setMinimizedFlag(false);
}

/*!
Returns true if ribbon bar is maximized. Otherwise returns false.
*/
bool ARibbonBar::isMaximized() const
{
    return !isMinimized();
}

/*!
Sets maximized \a flag for the ribbon bar.
*/
void ARibbonBar::setMaximized(bool flag)
{
    setMinimizedFlag(!flag);
}

/*!
Sets the minimization feature of the ARibbon Bar to the \a enabled.
*/ 
void ARibbonBar::setMinimizationEnabled(bool enabled)
{
    A_D(ARibbonBar);
    if (!enabled)
        setMinimizedFlag(false);
    d.m_minimizationEnabled = enabled;
}

/*!
Returns the enable flag of the minimization feature of the ARibbon Bar.
*/ 
bool ARibbonBar::isMinimizationEnabled() const
{
    A_D(const ARibbonBar);
    return d.m_minimizationEnabled;
}

void ARibbonBar::visualARibbonBarView(bool bVisble) 
{ 
    A_D(ARibbonBar); 
    d.m_bShowARibbonBarView = bVisble;
	updateLayout();
	updateGeometry();
}

bool ARibbonBar::simplifiedMode() const
{
    A_D(const ARibbonBar);
    return d.m_simplifiedMode;
}

void ARibbonBar::setSimplifiedMode(bool flag)
{
    A_D(ARibbonBar);
    if (d.m_simplifiedMode == flag)
        return;
    d.m_simplifiedMode = flag;

    if (d.m_simplifiedAction != nullptr)
        d.m_simplifiedAction->setChecked(flag);

    if (isMinimized())
        maximize();
    else
        d.changed(true);

    emit simplifiedModeChanged(d.m_simplifiedMode);
}

bool ARibbonBar::simplifiedModeEnabled() const
{
    A_D(const ARibbonBar);
    if (d.m_simplifiedAction == nullptr)
        return false;
    return d.m_simplifiedAction->isVisible();
}

void ARibbonBar::setSimplifiedModeEnabled(bool enabled)
{
    A_D(ARibbonBar);
    if (d.m_simplifiedAction == nullptr)
        return;
    if (!enabled)
        setSimplifiedMode(false);
    d.m_simplifiedAction->setVisible(enabled);
    d.changed(true);
}

QAction* ARibbonBar::simplifiedAction() const
{
    A_D(const ARibbonBar);
    return d.m_simplifiedAction;
}

Qt::LayoutDirection ARibbonBar::expandDirection() const
{
    A_D(const ARibbonBar);
    return d.m_expandDirection;
}

void ARibbonBar::setExpandDirection(Qt::LayoutDirection direction)
{
    A_D(ARibbonBar);
    if (d.m_expandDirection == direction)
        return;
    d.m_expandDirection = direction;
    d.changed(true);
}

/*!
Returns customize manager. It is used to configure customization via a special dialog ARibbonCustomizeDialog.
\sa ARibbonCustomizeDialog
*/ 
ARibbonCustomizeManager* ARibbonBar::customizeManager()
{
    A_D(ARibbonBar);
    if (d.m_customizeManager == nullptr)
        d.m_customizeManager = new ARibbonCustomizeManager(this);
    return d.m_customizeManager;
}

/*!
Returns customize dialog. You can use the reference to add custom setting pages to the standard dialog. 
\sa ARibbonCustomizeManager, showCustomizeDialog()
*/
ARibbonCustomizeDialog* ARibbonBar::customizeDialog()
{
    A_D(ARibbonBar);
    d.createCustomizeDialog();
    return d.m_customizeDialog;
}

/*!
Shows customization dialog related to customizeDialog() function. Dialog is shown in modal mode to window that holds a ribbon bar.
\sa customizeDialog
*/
void ARibbonBar::showCustomizeDialog()
{
    A_D(ARibbonBar)
    ARibbonCustomizeDialog* dialog = customizeDialog();
    Q_ASSERT(dialog != nullptr);
    if (QAction* action = qobject_cast<QAction*>(d.sender()))
    {
        if (action->objectName() == strCustomizeQAToolBar)
        {
            ARibbonQuickAccessBarCustomizePage* page = nullptr;
            for (int i = 0; i < dialog->pageCount(); ++i)
            {
                page = qobject_cast<ARibbonQuickAccessBarCustomizePage *>(dialog->pageByIndex(i));
                if (page != nullptr)
                {
                    dialog->setCurrentPage(page);
                    break;
                }
            }
        }
        else if (action->objectName() == strCustomizeARibbonBar)
        {
            ARibbonBarCustomizePage* page = nullptr;
            for (int i = 0; i < dialog->pageCount(); ++i)
            {
                page = qobject_cast<ARibbonBarCustomizePage *>(dialog->pageByIndex(i));
                if (page != nullptr)
                {
                    dialog->setCurrentPage(page);
                    break;
                }
            }
        }
    }
    dialog->exec();
}

/*!
Make a page in the position specified by \a index, the current one.
*/ 
void ARibbonBar::setCurrentPageIndex(int index)
{
    A_D(ARibbonBar);
    d.m_ribbonTabBar->setCurrentIndex(index);
}

/*!
Returns the index of the current page.
*/ 
int ARibbonBar::currentPageIndex() const
{
    A_D(const ARibbonBar);
    return d.m_ribbonTabBar->currentIndex();
}

ARibbonPage* ARibbonBar::currentPage() const
{
    A_D(const ARibbonBar);
    return page(d.m_ribbonTabBar->currentIndex());
}

/*!
Returns the list of all pages in the ribbon bar.
*/
QList<ARibbonPage*> ARibbonBar::pages() const
{
    A_D(const ARibbonBar);
    QList<ARibbonPage*> pages;
    for (int i = 0, count = d.m_ribbonTabBar->count(); count > i; i++)
        pages << d.m_ribbonTabBar->getTab(i)->page();
    return pages;
}

int ARibbonBar::pageIndex(ARibbonPage* page) const
{
    A_D(const ARibbonBar);
    ARibbonPagePrivate* page_private = ARibbonPagePrivate::_get(page);
    if (page_private->associativeTab() == nullptr)
        return -1;
    return d.m_ribbonTabBar->tabIndex(page_private->associativeTab());
}

/*!
Returns the ribbon page by \a index. See description of class ARibbonPage for more details.
*/ 
ARibbonPage* ARibbonBar::page(int index) const
{
    A_D(const ARibbonBar);
    if (const ARibbonTab* tab = d.m_ribbonTabBar->getTab(index))
        return tab->page();
    return nullptr;
}

/*!
Returns the number of the pages inside the ribbon bar.
*/ 
int ARibbonBar::pageCount() const
{
    A_D(const ARibbonBar);
    return d.m_ribbonTabBar->count();
}

QAction* ARibbonBar::addTitleButton(const QIcon& icon, const QString& help)
{
    A_D(ARibbonBar);
    return d.m_ribbonTitleBarWidget->addTitleButton(icon, help);
}

void ARibbonBar::removeTitleButton(QAction* action)
{
    A_D(ARibbonBar);
    d.m_ribbonTitleBarWidget->removeTitleButton(action);
}

/*!
Adds the menu at the top right ribbon bar with a given \a text. Returns a pointer to the new object QMenu.
*/
QMenu* ARibbonBar::addMenu(const QString& text)
{
    A_D(ARibbonBar);
    return d.m_ribbonTabBar->addMenu(text);
}

/*!
Adds button in the upper part of ribbon bar. 
Parameter \a icon specifies the icon on the button.
Parameter \a text specifies the button text.
Parameter \a style specifies the button style and parameter \a menu specifies the pointer to QMenu object. 
Function returns a pointer to the new QAction object.
*/
QAction* ARibbonBar::addAction(const QIcon& icon, const QString& text, Qt::ToolButtonStyle style, QMenu* menu)
{
    A_D(ARibbonBar);
    QAction* action = d.m_ribbonTabBar->addAction(icon, text, style, menu);
    QMenuBar::addAction(action);
    return action;
}

/*!
Adds button to the upper part of the ribbon bar associated with an existing \a action.
Parameter \a style specifies the style of the button.
*/
QAction* ARibbonBar::addAction(QAction* action, Qt::ToolButtonStyle style)
{
    A_D(ARibbonBar);
    QMenuBar::addAction(action);
    return d.m_ribbonTabBar->addAction(action, style);
}

/*!
Adds system button to the upper left part of ribbon bar.
Parameter \a text specifies the text on the system button.
Function returns a pointer to the new QAction object.
*/
QAction* ARibbonBar::addSystemButton(const QString& text)
{
    return addSystemButton(QIcon(), text);
}

/*!
Adds system button to the upper left part of ribbon bar with \a icon.
Parameter \a text specifies the text on the system button.
Parameter icon specifies the icon. Function returns a pointer to the new QAction object.
*/
QAction* ARibbonBar::addSystemButton(const QIcon& icon, const QString& text)
{
    A_D(ARibbonBar);
    if (systemButton() == nullptr)
        d.createSystemButton(icon, text);

    ARibbonSystemMenu* systemMenu = new ARibbonSystemMenu(this);
    systemButton()->setSystemMenu(systemMenu);
    d.changed();
    return systemButton()->defaultAction();
}

/*!
Returns pointer to the ARibbonSystemButton object.
*/
ARibbonSystemButton* ARibbonBar::systemButton() const
{
    A_D(const ARibbonBar);
    return d.m_systemButton;
}

/*!
Returns the visibility of the BackStage view at the moment.
*/ 
bool ARibbonBar::isBackstageVisible() const
{
    if (systemButton() != nullptr)
    {
        if (ARibbonBackstageView* backstage = qobject_cast<ARibbonBackstageView*>(systemButton()->backstage()))
            return backstage->isVisible();
    }
    return false;
}

bool ARibbonBar::isAcrilycEnabled() const
{
    A_D(const ARibbonBar);
    return d.material()->blendType() != MaterialWidget::DisabledBlend;
}

void ARibbonBar::setAcrilycEnabled(bool enabled)
{
    A_D(ARibbonBar);
    d.material()->setBlendType(enabled ? MaterialWidget::BackgroundBlend : MaterialWidget::DisabledBlend);
#ifndef QTITAN_DESIGNER
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    Q_ASSERT(this->parentWidget());
    Q_ASSERT(this->parentWidget()->isWindow());
    if (enabled)
    {
        WindowTitleBar* titleBar = WindowTitleBar::get(this->parentWidget());
        titleBar->setBlurBehindWindowEnabled(true);
        d.updateTitleBar();
    }
    else
    {
        if (WindowTitleBar* titleBar = d.findTitleBar())
        {
            titleBar->setBlurBehindWindowEnabled(false);
        }
    }
    d.changed();
#else
    Q_UNUSED(enabled)
#endif // QT_VERSION_CHECK
#else
    Q_UNUSED(enabled)
#endif // QTITAN_DESIGNER
}

void ARibbonBar::setFrameThemeEnabled(bool enabled)
{
    A_D(ARibbonBar);
    if (d.m_titleBarCustomized == enabled)
        return;
    enabled = enabled && d.isTitleBarCustomizationSupported();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    d.m_titleBarCustomized = enabled;
    if (enabled)
    {
        Q_ASSERT(this->parentWidget());
        Q_ASSERT(this->parentWidget()->isWindow());

        //Creates window title bar, if needed
        WindowTitleBar* titleBar = WindowTitleBar::get(this->parentWidget());
        Q_UNUSED(titleBar);
        d.updateTitleBar();
    }
    else if (WindowTitleBar* titleBar = d.findTitleBar())
    {
        titleBar->setWidget(nullptr);
        titleBar->removeAndDelete();
    }
    d.changed();
    emit frameThemeChanged(isFrameThemeEnabled());
#else
    d.m_titleBarCustomized == false;
#endif // QT_VERSION_CHECK
}

/*!
\property ARibbonBar::frameThemeEnabled
    This property holds whether display the content for modern Office 2007, 2010, 2013, 2016 styles on the window title bar and 
    activate Windows Air support (applications under Windows 10, 8, 7, Vista) is enabled. 
    By default parameter is enabled. The property supports Windows, Linux and MacOSX.
\sa WindowTitleBar
\inmodule QtitanARibbon
*/ 
bool ARibbonBar::isFrameThemeEnabled() const
{
    A_D(const ARibbonBar);
    return d.m_titleBarCustomized;
}
//
//int ARibbonBar::tabBarHeight(bool isVisible) const
//{
//    A_D(const ARibbonBar);
//    return (d.m_ribbonTabBar && (d.m_ribbonTabBar->isVisible()||isVisible)) ? d.m_ribbonTabBar->height() : 2;
//}

/*!
Returns the visible state for the contextual tabs on the ribbon bar. By default it is true.
*/
bool ARibbonBar::isContextualTabsVisible() const
{
    A_D(const ARibbonBar);
    return d.m_contextualTabsVisible;
}

/*!
Sets the \a visible state for the contextual tabs defined by \a visible parameter.
*/
void ARibbonBar::setContextualTabsVisible(bool visible)
{
    A_D(ARibbonBar);
    if (d.m_contextualTabsVisible == visible)
        return;
    d.m_contextualTabsVisible = visible;
    d.changed();
}

/*!
\property ARibbonBar::titleGroupsVisible
Sets the visibility of title on all ribbon groups. 
\inmodule QtitanARibbon
*/
bool ARibbonBar::isTitleGroupsVisible() const
{
    A_D(const ARibbonBar);
    if (simplifiedMode())
        return false;
    return d.m_titleGroupsVisible;
}

void ARibbonBar::setTitleGroupsVisible(bool visible)
{
    A_D(ARibbonBar);
    if (d.m_titleGroupsVisible == visible)
        return;
    d.m_titleGroupsVisible = visible;
    d.changed(true);
}

int ARibbonBar::rowItemHeight() const
{
    A_D(const ARibbonBar);
    return d.rowItemHeight();
}

int ARibbonBar::rowItemCount() const
{
    A_D(const ARibbonBar);
    return d.rowItemCount();
}

/*!
Performs a full rebuild of the ribbon bar layout.
*/
void ARibbonBar::updateLayout()
{
    A_D(ARibbonBar);
    //Some optimization here. updateLayout() will be initiated in QEvent::Show handler.
    if (d.isUpdating() || !testAttribute(Qt::WA_WState_Visible))
        return;
    d.updateLayout();
}

/*!
Begin an updating the contents of the ribon bar. 
Use this method to prevent unnecessary calls to rebuild the lay-out at the time of ribbon bar initialization. 
Using the method allows to significantly increase the speed of creation and filling of the ribbon bar.
\sa endUpdate
*/
void ARibbonBar::beginUpdate()
{
    A_D(ARibbonBar);
    d.m_updateLayoutCount++;
    Q_ASSERT(d.m_updateLayoutCount > 0);
}

/*!
Ends the ribbon bar update. The method is always preceded by a call beginUpdate().
*/
void ARibbonBar::endUpdate()
{
    A_D(ARibbonBar);
    d.m_updateLayoutCount--;
    if (d.isUpdating())
        return;
    updateLayout();
}

/*! \reimp */
bool ARibbonBar::event(QEvent* event)
{
    A_D(ARibbonBar);
    if (event->type() == QEvent::Move || event->type() == QEvent::Resize)
        aproch_fixupQtMainLayout(this);

    bool result = QMenuBar::event(event);
    switch(event->type())
    {

    case QEvent::Show:
    {
        updateLayout();
    }
    break;

        case QEvent::ChildAdded:
            {
                QChildEvent* e = static_cast<QChildEvent *>(event);
                if (e->added())
                {
                    QWidget* widget = qobject_cast<QWidget *>(e->child());
                    if (widget != nullptr)
                    {
                    }
                }
            }
            break;
        case QEvent::StyleChange:
        case QEvent::FontChange:
            {
                if (event->type() == QEvent::StyleChange)
                {
                    CommonStyle::ensureStyle();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
                    d.updateTitleBar();
#endif
                }

                d.setMetricsDirty();
                if (isVisible())
                    d.changed(true);
            }
            break;
        case QEvent::LayoutDirectionChange:
        case QEvent::WindowStateChange: 
            d.changed();
            break;
        case QEvent::WindowTitleChange:
            d.changed();
            break;
        case QEvent::ActionRemoved:
        case QEvent::ActionChanged:
                QApplication::sendEvent(d.m_ribbonTabBar, event);
            break;
        default:
            break;
    }
    return result;
}

/*! \reimp */
bool ARibbonBar::eventFilter(QObject* object, QEvent* event)
{
    A_D(ARibbonBar);
    QWidget* leftWidget = cornerWidget(Qt::TopLeftCorner);
    QWidget* rightWidget = cornerWidget(Qt::TopRightCorner);
    if (leftWidget == object || rightWidget == object)
    {
        switch (event->type()) {
        case QEvent::ShowToParent:
        case QEvent::HideToParent:
            d.updateLayout();
            return false;
            break;
        default:
            break;
        }
    }
    bool result = QMenuBar::eventFilter(object, event);
    return result;
}

/*! \reimp */
void ARibbonBar::paintEvent(QPaintEvent* event)
{
    A_D(ARibbonBar);
#if 0
    QWidget* leftCorner = cornerWidget(Qt::TopLeftCorner);
    if(leftCorner && leftCorner->isVisible())
        leftCorner->setVisible(false);
#endif

    QPainter p(this);
    QRect rectEvent = event->rect();
    p.setClipRect(rectEvent);

    ARibbonStyleOption opt;
    opt.initFrom(this);
    opt.rectTabBar = d.m_ribbonTabBar->geometry();
    opt.titleBarHeight = d.extendedTitleBarHeight();
    opt.minimized = isMinimized();
    opt.tabBarPosition = tabBarPosition();
    opt.quickAccessBarPosition = quickAccessBarPosition();
    opt.isBackstageVisible = isBackstageVisible();

    if (ARibbonTab* tab = d.m_ribbonTabBar->getTab(d.m_ribbonTabBar->currentIndex()))
    {
        opt.rectCurrentTab = tab->rect();
        QPoint pntrb(tab->mapTo(this, opt.rectCurrentTab.topLeft()));
        opt.rectCurrentTab.translate(pntrb);
    }
    if (ARibbonPage* p = page(currentPageIndex()))
        opt.contextColor = p->contextColor();

    p.setBackgroundMode(Qt::TransparentMode);
    style()->drawPrimitive((QStyle::PrimitiveElement)CommonStyle::PE_ARibbonBarPanel, &opt, &p, this);

    if (!isMinimized())
    {
        opt.rect = d.getPageFrameRect();
        style()->drawPrimitive((QStyle::PrimitiveElement)CommonStyle::PE_ARibbonFrameGroups, &opt, &p, this);
    }
}

/*! \reimp */
void ARibbonBar::changeEvent(QEvent* event)
{
    return QMenuBar::changeEvent(event);
}

/*! \reimp */
void ARibbonBar::resizeEvent(QResizeEvent* event)
{
    QMenuBar::resizeEvent(event);
    updateLayout();
}

/*! \reimp */
void ARibbonBar::mouseDoubleClickEvent(QMouseEvent* event)
{
    QMenuBar::mouseDoubleClickEvent(event);

    if (event->button() != Qt::LeftButton)
        return;
}

/*! \reimp */
void ARibbonBar::mousePressEvent(QMouseEvent* event)
{
    // block the function call QMenuBar
    QWidget::mousePressEvent(event);
}

/*! \reimp */
void ARibbonBar::wheelEvent(QWheelEvent* event)
{
    A_D(ARibbonBar);
    if (QApplication::activePopupWidget() || !isMaximized())
    {
        event->ignore();
        return;
    }
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    QPoint pos = event->position().toPoint();
#else
    QPoint pos = event->pos();
#endif

    if (!d.m_ribbonTabBar->geometry().contains(pos))
    {
        d.m_ribbonTabBar->wheelEvent(event);
        event->ignore();
    }
}

void ARibbonBar::contextMenuEvent(QContextMenuEvent* event)
{
    A_D(ARibbonBar)
    if (QMenu* menu = d.createContextMenu()) 
    {
        menu->setAttribute(Qt::WA_DeleteOnClose);
        event->accept();
        emit showARibbonContextMenu(menu, event);
        if (event->isAccepted())
            menu->exec(event->globalPos());
    }
}

void ARibbonBar::setVisible(bool visible)
{
//    ARibbonBarAutoUpdater autoUpdater(this);
    QMenuBar::setVisible(visible);
}

int ARibbonBar::heightForWidth(int) const
{
    return sizeHint().height();
}

/*! \reimp */
QSize ARibbonBar::sizeHint() const
{
    A_D(const ARibbonBar);
    if (d.m_sizeHint.isEmpty())
        const_cast<ARibbonBarPrivate*>(&d)->updateLayoutRects();
    return d.m_sizeHint;
}

/*!
Installs translator to QApplication based on \a country variable. Format variable is de_de, ru_ru, fr_fr. If \a country is empty then loaded QTranslator based on current locale setting.
*/
bool ARibbonBar::loadTranslation(const QString& country)
{
    static bool loaded = false;
    if (loaded)
        return true;

    const QString directory = QStringLiteral(":/translations/ribbon");
    QTranslator* translator = new QTranslator(qApp);
    if (country.isEmpty())
    {
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
        foreach(const QString name, QLocale().uiLanguages())
        {
            const QLocale locale(name);
            if (translator->load(locale, QString(), QString(), directory))
            {
                qApp->installTranslator(translator);
                loaded = true;
                return true;
            }
        }
#endif
    }
    else
    {
        if (translator->load(country, directory))
        {
            qApp->installTranslator(translator);
            loaded = true;
            return true;
        }
    }

    delete translator;
    return false;
}

QString ARibbonBar::tr_compatible(const char *s, const char *c)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    return QCoreApplication::translate("Qtitan::ARibbonBar", s, c);
#else
    return QCoreApplication::translate("Qtitan::ARibbonBar", s, c, QCoreApplication::CodecForTr);
#endif
}

bool ARibbonBar::getNarrowMode() const
{
    A_D(const ARibbonBar);

    bool bNarrow = false;

    int nCount = d.rowItemCount();
    if (nCount == 2)
    {
        bNarrow = true;
    }
    else if (nCount == 3)
    {
        bNarrow = false;
    }

    return bNarrow;
}

void ARibbonBar::setNarrowMode(bool bNarrow /*= true*/)
{
	A_D(ARibbonBar);

    int nCount = 0;
    if (bNarrow)
        nCount = 2;
    else
    {
        nCount = 3;
    }

	if (d.rowItemCount() == nCount)
		return;

	beginUpdate();
	d.setRowItemCount(nCount);
    endUpdate();
}

bool ARibbonBar::getAutoReduceMode()
{
    A_D(const ARibbonBar);

    return d.m_bAutoReduce;
}

void ARibbonBar::setAutoReduceMode(bool bReduce)
{
    A_D(ARibbonBar);
    if(d.m_bAutoReduce == bReduce)
        return;

    beginUpdate();
    d.m_bAutoReduce = bReduce;
    endUpdate();
}

APROCH_NAMESPACE_END