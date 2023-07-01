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
#include <QScreen>
#include <QPainter>
#include <QLineEdit>
#include <QComboBox>
#include <QToolBar>
#include <QAbstractItemView>

#include "ARibbonGroup.h"
#include "ARibbonPage.h"
#include "ARibbonPagePrivate.h"
#include "ARibbonBarPrivate.h"
#include "ARibbonGallery.h"
#include "ARibbonPopup.h"
#include "QtnOfficePopupMenu.h"
#include "ARibbonGroupPrivate.h"
#include "ARibbonControlsPrivate.h"
#include "QtnStyleHelperPrivate.h"
#include "QtnOfficePopupColorButton.h"
#include "ARibbonButtonControls.h"
#include "ARibbonGalleryControls.h"
#include "ARibbonInputControls.h"
#include "QtnCommonStylePrivate.h"
#ifdef A_MEMORY_DEBUG
#include "QtitanMSVSDebug.h"
#endif

QTITAN_USE_NAMESPACE

/*!
\property ARibbonGroup::reduced
Returns true if the group is in the reduced mode.
*/

/*!
\property ARibbonGroup::title
Holds the title of the group.
*/

/*!
\property ARibbonGroup::titleFont
Holds the font that used to render the group title.
*/

/*!
\property ARibbonGroup::titleColor
Holds the color that used to render the group title.
*/

/*!
\property ARibbonGroup::icon
Holds the icon for the group title.
*/

/*!
\property ARibbonGroup::spacing
Holds the spacing between the items inside the group in pixels.
*/

/*!
\property ARibbonGroup::optionButtonVisible
Specifies the visibility of the option button for the group. The button is visible by default.
*/

/*!
\property ARibbonGroup::contentAlignment
Specifies the alignment of the group content items.
*/

/*!
\property ARibbonGroup::titleElideMode
Specifies is how to shorten the group title text if it does not fit in width.
*/

static void setSizeDefinitionByStyle(RibbonButtonControl* control, Qt::ToolButtonStyle style)
{
    if (Qt::ToolButtonTextUnderIcon == style)
    {
        control->sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setImageSize(ARibbonControlSizeDefinition::ImageLarge);
        control->sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setLabelVisible(true);
    }
    else if (Qt::ToolButtonTextBesideIcon == style)
    {
        control->sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
        control->sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
    }
    else if (Qt::ToolButtonIconOnly == style)
    {
        control->sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
        control->sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
        control->sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setLabelVisible(false);
        control->sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setLabelVisible(false);
        control->sizeDefinition(ARibbonControlSizeDefinition::GroupSmall)->setLabelVisible(false);
        control->sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setLabelVisible(false);
    }
}


/* RibbonButtonAction */
class RibbonButtonAction : public QWidgetAction
{
public:
    explicit RibbonButtonAction(QObject* parent, const QIcon& icon, const QString& text, Qt::ToolButtonStyle style, 
        QMenu* menu, QToolButton::ToolButtonPopupMode mode = QToolButton::DelayedPopup);
    virtual ~RibbonButtonAction();
protected:
    virtual QWidget* createWidget(QWidget* parent);
private:
    Qt::ToolButtonStyle m_style;
    QToolButton::ToolButtonPopupMode m_mode;
};

RibbonButtonAction::RibbonButtonAction(QObject* parent, const QIcon& icon, const QString& text, Qt::ToolButtonStyle style, 
                                       QMenu* menu, QToolButton::ToolButtonPopupMode mode)
    : QWidgetAction(parent)
    , m_style(style)
    , m_mode(mode)
{
    setIcon(icon);
    setText(text);
    setMenu(menu);
}

RibbonButtonAction::~RibbonButtonAction()
{
}

QWidget* RibbonButtonAction::createWidget(QWidget* parent)
{
    if (QToolBar* toolBar = qobject_cast<QToolBar*>(parent))
    {
        RibbonButton* button = new RibbonButton(parent);
        button->setAutoRaise(true);
        button->setFocusPolicy(Qt::NoFocus);
        button->setIconSize(toolBar->iconSize());
        button->setToolButtonStyle(toolBar->toolButtonStyle());
        QObject::connect(toolBar, SIGNAL(iconSizeChanged(QSize)), button, SLOT(setIconSize(QSize)));
        QObject::connect(toolBar, SIGNAL(toolButtonStyleChanged(Qt::ToolButtonStyle)), button, SLOT(setToolButtonStyle(Qt::ToolButtonStyle)));
        button->setDefaultAction(this); 
        QObject::connect(button, SIGNAL(triggered(QAction*)), toolBar, SIGNAL(actionTriggered(QAction*)));
        return button;
    }
    else if (ARibbonGroup* ribbonGroup = qobject_cast<ARibbonGroup*>(parent))
    {
        RibbonButtonControl* control = new RibbonButtonControl(ribbonGroup);
        ::setSizeDefinitionByStyle(control, m_style);
        control->setDefaultAction(this);
        control->setToolButtonStyle(m_style);
        if (menu())
        {
            control->setPopupMode(m_mode);
            control->setMenu(menu());
            menu()->setDefaultAction(this);
        }
        return control;
    }
    return nullptr;
}


/* RibbonGroupPrivate */
RibbonGroupPrivate::RibbonGroupPrivate()
    : m_optionButton(nullptr)
    , m_buttonScrollLeft(nullptr)
    , m_buttonScrollRight(nullptr)
    , m_groupPopup(nullptr)
    , m_toolBar(nullptr)
    , m_currentSize(ARibbonControlSizeDefinition::GroupLarge)
    , m_contentAlignment(Qt::AlignTop)
    , m_controlsAlignment(Qt::AlignVCenter)
    , m_spacing(0)
    , m_butOptionVisible(false)
    , m_hasOverflowContent(false)
    , m_actOption(nullptr)
    , m_pageScrollPos(0)
    , m_groupScrollPos(0)  // Scroll position
    , m_titleElideMode(Qt::ElideRight)
{
}

RibbonGroupPrivate::~RibbonGroupPrivate()
{
    Q_DELETE_AND_NULL(m_groupPopup);
}

void RibbonGroupPrivate::init()
{
    A_P(ARibbonGroup);
    p.setMouseTracking(true);
    m_optionButton = new RibbonGroupOption(&p);
    m_actOption = new QAction(&p);
    m_actOption->setObjectName(QStringLiteral("__aproch_Action_Option"));
    m_optionButton->setDefaultAction(m_actOption);
    QObject::connect(m_optionButton, SIGNAL(triggered(QAction*)), &p, SIGNAL(actionTriggered(QAction*)));
    m_optionButton->hide();
}

void RibbonGroupPrivate::clear(bool deleteControls)
{
    A_P(ARibbonGroup);
    if (p.isControlsGrouping())
    {
        m_toolBar->clear();
        return;
    }

    QList<QAction*> actions = p.actions();
    for (int i = 0; i < actions.size(); ++i)
        p.removeAction(actions.at(i));

    while (m_controls.size() > 0)
    {
        RibbonControl* control = m_controls[0];
        p.removeControl(control);

        if (deleteControls)
            delete control;
    }
}

ARibbonPage* RibbonGroupPrivate::parentPage() const
{
    A_P(const ARibbonGroup);
    return qobject_cast<ARibbonPage*>(p.parentWidget());
}

void RibbonGroupPrivate::enableGroupScroll(bool scrollLeft, bool scrollRight)
{
    A_P(ARibbonGroup);

    if (m_buttonScrollLeft == nullptr)
    {
        m_buttonScrollLeft = new ARibbonGroupScrollButton(&p, true);
        m_buttonScrollLeft->setVisible(false);
        QObject::connect(m_buttonScrollLeft, SIGNAL(pressed()), this, SLOT(pressLeftScrollButton()));
    }

    if (m_buttonScrollRight == nullptr)
    {
        m_buttonScrollRight = new ARibbonGroupScrollButton(&p, false);
        m_buttonScrollRight->setVisible(false);
        QObject::connect(m_buttonScrollRight, SIGNAL(pressed()), this, SLOT(pressRightScrollButton()));
    }

    const ARibbonBar* rb = p.ribbonBar();
    Q_ASSERT(rb != NULL);

    const int heightGroup = rb->rowItemHeight() * rb->rowItemCount();

    QRect rcGroup(p.geometry());
    rcGroup.setHeight(heightGroup + 4);

    if (scrollLeft)
    {
        m_buttonScrollLeft->raise();
        m_buttonScrollLeft->setVisible(true);
        m_buttonScrollLeft->setGeometry(QRect(QPoint(rcGroup.left() - 1, rcGroup.top()), QPoint(rcGroup.left() + 12, rcGroup.bottom())));
    }
    else
        m_buttonScrollLeft->setVisible(false);

    if (scrollRight)
    {
        m_buttonScrollRight->raise();
        m_buttonScrollRight->setVisible(true);
        m_buttonScrollRight->setGeometry(QRect(QPoint(rcGroup.right() - 12, rcGroup.top()), QPoint(rcGroup.right() + 1, rcGroup.bottom())));
    }
    else
        m_buttonScrollRight->setVisible(false);
}

void RibbonGroupPrivate::showGroupScroll()
{
    A_P(ARibbonGroup);
    if (p.isReduced() && p.isVisible())
    {
        QRect screen = aproch_availableGeometry(&p);
        int totalWidth = p.layout()->minimumSize().width();
        int groupLength = screen.width();
        int scrollPos = m_groupScrollPos;

        if (totalWidth > groupLength)
        {
            if (scrollPos > totalWidth - groupLength)
                scrollPos = totalWidth - groupLength;
        }
        else
            scrollPos = 0;

        if (scrollPos < 0) 
            scrollPos = 0;

        m_groupScrollPos = scrollPos;
        enableGroupScroll(scrollPos > 0, totalWidth - groupLength - scrollPos > 0);
    }
}

void RibbonGroupPrivate::initStyleOption(RibbonGroupStyleOption& opt) const
{
    A_P(const ARibbonGroup)
    opt.initFrom(&p);
    if (ARibbonPage* page = parentPage())
        opt.contextColor = page->contextColor();

    opt.heightCaption = 0;
    opt.fontMetrics = QFontMetrics(p.titleFont());
    opt.widthOptButton = m_optionButton->isVisible() ? m_optionButton->width() : 0;
    opt.textAlignment = Qt::AlignCenter;
    opt.textElideMode = m_titleElideMode;
    int sz = p.style()->pixelMetric(QStyle::PM_SmallIconSize, &opt, &p);
    opt.iconSize = QSize(sz, sz);

    if (m_titleColor.isValid())
        QPalette_setColor(opt.palette, QPalette::WindowText, m_titleColor);

    if (p.ribbonBar() != nullptr && p.ribbonBar()->isTitleGroupsVisible())
    {
        opt.text = m_title;
        opt.heightCaption = RibbonGroupPrivate::calcGroupTitleHeight(p.titleFont());
    }
}

int RibbonGroupPrivate::calcMinimumWidth(ARibbonControlSizeDefinition::GroupSize size) const
{
    A_P(const ARibbonGroup)
    RibbonGroupStyleOption opt;
    initStyleOption(opt);

    int width = p.style()->pixelMetric(static_cast<QStyle::PixelMetric>(CommonStyle::PM_RibbonGroupReducedWidth), &opt, &p);

    if (size == ARibbonControlSizeDefinition::GroupSmall)
        width += 1;
    else if (size == ARibbonControlSizeDefinition::GroupMedium)
        width += 2;
    else if (size == ARibbonControlSizeDefinition::GroupLarge)
        width += 3;
    return width;
}

int RibbonGroupPrivate::arrangeRowContent(QList<RibbonControl*>& row, int leftOffset, int topOffset, 
    int rowItemHeight, int rowItemCount, QWidget* parent) const
{
    A_P(const ARibbonGroup)
    int max = 0;
    int rowHeight = rowItemHeight;
    int top = 0;
    if (p.contentAlignment() & Qt::AlignVCenter)
        top = (rowItemHeight * (rowItemCount - row.size())) * 0.5;
    else if (p.contentAlignment() & Qt::AlignBottom)
        top = rowItemHeight * (rowItemCount - row.size());

    for (QList<RibbonControl*>::const_iterator it = row.constBegin(); it != row.constEnd(); ++it)
        max = qMax(RibbonControl::getBoundedSizeHint(*it).width(), max);

    if (parent == nullptr)
    {
        row.clear();
        return max;
    }

    topOffset += top;
    //leftOffset += hmargin;

    for (QList<RibbonControl*>::const_iterator it = row.constBegin(); it != row.constEnd(); ++it)
    {
        QSize size = RibbonControl::getBoundedSizeHint(*it);
        size.rheight() = qMin(rowHeight, size.height());
        const bool stretchable = (*it)->sizeDefinition((*it)->currentSize())->isStretchable();
        QRect r = QRect(leftOffset, topOffset, max, rowHeight);
        size.rwidth() = stretchable ? max : size.width();

        QRect controlRect = QStyle::alignedRect(p.layoutDirection(), p.controlsAlignment(), size, r);
        (*it)->setGeometry(controlRect);
        topOffset += rowHeight;
    }
    row.clear();
    return max;
}

void RibbonGroupPrivate::reposition(const QRect& rect)
{
    A_P(ARibbonGroup)
#ifdef QTITAN_DESIGNER
    p.setFixedSize(rect.size());
#endif
    p.setGeometry(rect);
}

void RibbonGroupPrivate::updateOptionButtonLayout(QWidget* parent)
{
    A_P(ARibbonGroup);
    if (parent != nullptr)
        m_optionButton->setParent(parent);

    m_optionButton->setVisible(p.isOptionButtonVisible());
    if (!m_optionButton->isHidden())
    {
        RibbonGroupStyleOption opt;
        initStyleOption(opt);
        if (parent && !qobject_cast<ARibbonGroup*>(parent))
            opt.rect.setSize(parent->sizeHint());

        QRect rc = opt.rect;
        QSize sz = m_optionButton->sizeHint();
        const int hSpace = p.style()->pixelMetric(QStyle::PM_LayoutHorizontalSpacing, nullptr, &p) / 2;
        const int vSpace = p.style()->pixelMetric(QStyle::PM_LayoutVerticalSpacing, nullptr, &p) / 2;

        if (p.currentSize() != ARibbonControlSizeDefinition::GroupSimplified)
            rc.setTop(rc.bottom() - opt.heightCaption);
        else
            rc.setTop(rc.bottom() - (sz.height() + vSpace * 2));

        QRect rectButOption(QPoint(rc.right() - sz.width() - hSpace, rc.top() + (rc.height() - sz.height()) / 2), sz);
        m_optionButton->setGeometry(QStyle::visualRect(p.layoutDirection(), rc, rectButOption));
    }
}

QSize RibbonGroupPrivate::updateControlsLayout(QWidget* parent, int leftOffset, int topOffset)
{
    A_P(const ARibbonGroup);
    m_hasOverflowContent = false;
    const ARibbonBar* rb = p.ribbonBar();
    if (rb == nullptr)
        return QSize();

    //static int margin = 4;
    const int maxRows = rb->rowItemCount(); // Get max row count from ARibbonBar here. 
    const int rowItemHeight = rb->rowItemHeight();

    QList<RibbonControl*> rowControls;
    QList<RibbonControl*> controls = controlsOrdered();

    RibbonControl* lastControl = controls.size() > 0 ? controls.last() : nullptr;
    for (QList<RibbonControl *>::const_iterator it = controls.constBegin(); it != controls.constEnd(); ++it)
    {
        RibbonControl* ribbonControl = *it;
        RibbonColumnBreakControl* sepatator = qobject_cast<RibbonColumnBreakControl *>(ribbonControl);

        if (ribbonControl->currentSize() == ARibbonControlSizeDefinition::GroupSimplifiedOverflow)
            m_hasOverflowContent = true;

        if (!m_hasOverflowContent && ribbonControl->currentSize() == ARibbonControlSizeDefinition::GroupSimplified)
        {
            if (RibbonToolBarControl* toolBarControl = qobject_cast<RibbonToolBarControl*>(ribbonControl))
                m_hasOverflowContent = !toolBarControl->isOverflowMenuIsEmpty();

            if (!m_hasOverflowContent && !ribbonControl->sizeDefinition(ARibbonControlSizeDefinition::GroupSimplified)->isVisible())
            {
                if (ribbonControl->sizeDefinition(ARibbonControlSizeDefinition::GroupSimplifiedOverflow)->isVisible())
                    m_hasOverflowContent = true;
            }
        }

        bool visible = ribbonControl->sizeDefinition(ribbonControl->currentSize())->isVisible();
        if (ribbonControl->defaultAction() != nullptr)
            visible = visible && ribbonControl->defaultAction()->isVisible();

        if (ribbonControl->currentSize() == ARibbonControlSizeDefinition::GroupSimplifiedOverflow)
        {
            m_hasOverflowContent = true;
            visible = false;
        }

        if (parent != nullptr)
        {
            if (ribbonControl->currentSize() != ARibbonControlSizeDefinition::GroupSimplifiedOverflow)
                ribbonControl->setParent(parent);

            if (sepatator == nullptr)
                ribbonControl->setVisible(visible);
            else
                ribbonControl->setVisible(visible && sepatator->sizeDefinition(sepatator->currentSize())->showSeparator());
        }

        if (!visible)
            continue;

        ribbonControl->updateLayout();
        QSize sizeHint = RibbonControl::getBoundedSizeHint(ribbonControl);
        if (sepatator != nullptr || sizeHint.height() > rowItemHeight)
        {
            if (rowControls.size() > 0)
                leftOffset += arrangeRowContent(rowControls, leftOffset, topOffset, rowItemHeight, maxRows, parent);

            if (sepatator != nullptr)
            {
                if (!sepatator->sizeDefinition(sepatator->currentSize())->showSeparator())
                    sizeHint = QSize(0, 0);
            }

            if (parent != nullptr)
            {
                int maxHeight = sizeHint.height();
                if (maxHeight > rowItemHeight * maxRows)
                    maxHeight = rowItemHeight * maxRows;
                else
                    maxHeight = qMax(rowItemHeight, sizeHint.height());

                ribbonControl->setGeometry(QRect(leftOffset, topOffset, sizeHint.width(), maxHeight));
            }
            leftOffset += sizeHint.width();
            if (lastControl != ribbonControl)
                leftOffset += m_spacing;
        }
        else 
        {
            rowControls.append(ribbonControl);
            if (rowControls.size() >= maxRows)
            {
                leftOffset += arrangeRowContent(rowControls, leftOffset, topOffset, rowItemHeight, maxRows, parent);
                if (lastControl != ribbonControl)
                    leftOffset += m_spacing;
            }
        }
    }

    if (rowControls.size() > 0)
        leftOffset += arrangeRowContent(rowControls, leftOffset, topOffset, rowItemHeight, maxRows, parent);

    return QSize(leftOffset, rowItemHeight * maxRows);
}

bool RibbonGroupPrivate::adjustCurrentSize(bool expand)
{
    A_P(ARibbonGroup);
    bool normDir = p.ribbonBar() == nullptr || p.ribbonBar()->expandDirection() == Qt::RightToLeft;
    if (expand)
        normDir = !normDir;

    QListIterator<RibbonControl*> iterator(m_controls);
    if (normDir)
        iterator.toBack();

    while (normDir ? iterator.hasPrevious() : iterator.hasNext())
    {
        RibbonControl* control = normDir ? iterator.previous() : iterator.next();
        if (control->adjustCurrentSize(expand))
            return true;
    }
    return false;
}

void RibbonGroupPrivate::updateLayoutParent()
{
    A_P(ARibbonGroup);
    RibbonBarAutoUpdater autoUpdater(p.ribbonBar());
    //if (ribbonBar != nullptr)
    //    ribbonBar->updateLayout();
}

/*! \reimp */
QSize RibbonGroupPrivate::sizeHint() const
{
    A_P(const ARibbonGroup);
    const ARibbonBar* ribbonBar = p.ribbonBar();
    if (ribbonBar == nullptr)
        return QSize(100, 100);

    QMargins margins = ARibbonBarPrivate::_get(ribbonBar)->groupContentMargins();

    int groupTitleHeight = ARibbonBarPrivate::_get(ribbonBar)->groupTitleHeight();
    const int minWidth = calcMinimumWidth(p.currentSize());
    const int groupHeight = ribbonBar->rowItemHeight() * ribbonBar->rowItemCount() + margins.top() + margins.bottom();

    RibbonGroupPrivate* group_private = const_cast<RibbonGroupPrivate*>(this);
    int topOffset = 0, leftOffset = 0;
    if (p.isReduced())
    {
        topOffset = margins.top();
        leftOffset = margins.left() + margins.right();
    }
    QSize size = group_private->updateControlsLayout(group_private->parentForControls(), leftOffset, topOffset);
    group_private->m_contentSizeHint = size;
    if (ribbonBar->simplifiedMode() && size.width() == 0)
        return size;

    size.rwidth() += margins.left() + margins.right();
    size.rwidth() = qMax(size.width(), minWidth);

    QSize titleSize;
    if (ribbonBar->isTitleGroupsVisible())
    {
        QFontMetrics fm(p.titleFont());
        titleSize = fm.size(Qt::TextShowMnemonic, p.title());
        titleSize.rwidth() += aproch_horizontalAdvanceFont(fm, QLatin1Char(' ')) * 4;

        int buttonWidth = 0;
        if (p.isOptionButtonVisible())
            buttonWidth = m_optionButton->width();
        titleSize.rwidth() += buttonWidth;
    } 
    else if (p.currentSize() == ARibbonControlSizeDefinition::GroupSimplified)
    {
        if (p.isOptionButtonVisible())
        {
            const int hSpace = p.style()->pixelMetric(QStyle::PM_LayoutHorizontalSpacing, nullptr, &p) / 2;
            size.rwidth() += (m_optionButton->width() + hSpace);
        }
    }

    if (p.minimumWidth() == 0)
        size.rwidth() = p.titleElideMode() == Qt::ElideNone ? qMax(titleSize.width(), size.width()) : size.width();
    else
        size.rwidth() = qMax(p.minimumWidth(), size.width());

    if (p.isMinimized() && !p.isHidden())
    {
        QRect screen = aproch_availableGeometry(&p);
        size.rwidth() = qMin(screen.width(), size.width());
    }

    size.setHeight(groupHeight + groupTitleHeight);
    return size;
}

QSize RibbonGroupPrivate::contentSizeHint() const
{
    return m_contentSizeHint;
}

void RibbonGroupPrivate::setReductionSize(ARibbonControlSizeDefinition::GroupSize size)
{
    if (m_currentSize == size)
        return;
    
    m_currentSize = size;

    for (QList<RibbonControl*>::const_iterator it = m_controls.constBegin(); it != m_controls.constEnd(); ++it)
    {
        if (size != (*it)->currentSize())
            (*it)->sizeChanged(size);
    }
}

void RibbonGroupPrivate::reduce()
{
    if (ARibbonPage* page = parentPage())
    {
        if (!ARibbonPagePrivate::_get(page)->canReduce(m_currentSize))
        {
            Q_ASSERT(false);
            return;
        }
        setReductionSize(static_cast<ARibbonControlSizeDefinition::GroupSize>(m_currentSize + 1));
    }
}

void RibbonGroupPrivate::expand()
{
    if (ARibbonPage* page = parentPage())
    {
        if (!ARibbonPagePrivate::_get(page)->canExpand(m_currentSize))
        {
            Q_ASSERT(false);
            return;
        }
        setReductionSize(static_cast<ARibbonControlSizeDefinition::GroupSize>(m_currentSize - 1));
    }
}

RibbonGroupPopup* RibbonGroupPrivate::groupPopup()
{
    A_P(ARibbonGroup);
    if (m_groupPopup == nullptr)
        m_groupPopup = new RibbonGroupPopup(&p);
    return m_groupPopup;
}

QWidget* RibbonGroupPrivate::parentForControls()
{
    A_P(ARibbonGroup);
    if (p.isReduced())
        return groupPopup();
    return &p;
}

QList<RibbonControl*> RibbonGroupPrivate::controlsOrdered() const
{
    QList<RibbonControl*> ret = m_controls;
    std::sort(ret.begin(), ret.end(), RibbonGroupPrivate::visualIndexLessThan);
    return ret;
}

bool RibbonGroupPrivate::hasOverflowContent() const
{
    return m_hasOverflowContent;
}

static void listPageWidth(int totalWidth, int realWidth, QList<int>& pagesWidth) 
{
    if (totalWidth > realWidth)
    {
        pagesWidth << realWidth;
        listPageWidth(totalWidth - realWidth, realWidth, pagesWidth);
    }
    else
        pagesWidth << totalWidth;
    return;
}

void RibbonGroupPrivate::pressLeftScrollButton()
{
    A_P(ARibbonGroup)
    QList<int> pagesWidth;
    listPageWidth(p.layout()->sizeHint().width(), p.geometry().width(), pagesWidth);

    if (0 < m_pageScrollPos)
        m_pageScrollPos--; 

    m_groupScrollPos -= pagesWidth[m_pageScrollPos];
    showGroupScroll();
    p.layout()->update();
}

void RibbonGroupPrivate::pressRightScrollButton()
{
    A_P(ARibbonGroup)
    QList<int> pagesWidth;
    listPageWidth(p.layout()->sizeHint().width(), p.geometry().width(), pagesWidth);

    m_groupScrollPos += pagesWidth[m_pageScrollPos];
    showGroupScroll();
    p.layout()->update();

    if (pagesWidth.size() - 1 > m_pageScrollPos)
        m_pageScrollPos++; 
}

bool RibbonGroupPrivate::visualIndexLessThan(RibbonControl* first, RibbonControl* second)
{
    int v1 = first->sizeDefinition(first->currentSize())->visualIndex();
    if (v1 == -1)
    {
        ARibbonGroup* group = first->parentGroup();
        v1 = group->aproch_d().m_controls.indexOf(first);
    }

    int v2 = second->sizeDefinition(second->currentSize())->visualIndex();
    if (v2 == -1)
    {
        ARibbonGroup* group = second->parentGroup();
        v2 = group->aproch_d().m_controls.indexOf(second);
    }
    return v1 < v2;
}

int RibbonGroupPrivate::calcGroupTitleHeight(const QFont& titleFont)
{
    QFontMetrics fm(titleFont);
    return fm.height() * 1.2;
}

void RibbonGroupPrivate::showPopupMenu()
{
    A_P(ARibbonGroup);
    QPoint pos = p.mapToGlobal(p.rect().bottomLeft());
    groupPopup()->popup(pos);
}

ARibbonBar* ARibbonGroup::ribbonBar() const
{
    A_D(const ARibbonGroup);
    if (ARibbonPage* page = d.parentPage())
        return page->ribbonBar();
    return nullptr;
}

/*!
\class ARibbonGroup
\inmodule QtitanRibbon
\brief ARibbonGroup class implements a group of items that is hosted within ARibbonPage.
*/

/*!
Constructs ARibbonGroup object with the given \a page and \a title.
*/
ARibbonGroup::ARibbonGroup(ARibbonPage* page, const QString& title)
    : QWidget(page)
{
    A_INIT_PRIVATE(ARibbonGroup);
    A_D(ARibbonGroup);
    d.init();
    setTitle(title);
}

/*!
Constructs ARibbonGroup object with the given \a parent.
*/
ARibbonGroup::ARibbonGroup(QWidget* parent)
    : QWidget(parent)
{
    A_INIT_PRIVATE(ARibbonGroup);
    A_D(ARibbonGroup);
    d.init();
}

/*!
Destructor of the ARibbonGroup object.
*/
ARibbonGroup::~ARibbonGroup()
{
    A_D(ARibbonGroup);
    d.clear(true);
    A_FINI_PRIVATE();
}

/*!
Returns true if the group is reduced.
*/ 
bool ARibbonGroup::isReduced() const
{
    A_D(const ARibbonGroup);
    if (d.m_controls.size() == 1)
    {
        if (!d.m_controls.at(0)->sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->isStretchable())
            return false;
    }
    return d.m_currentSize == ARibbonControlSizeDefinition::GroupPopup;
}

/*!
Returns the title of the group.
*/ 
const QString& ARibbonGroup::title() const
{
    A_D(const ARibbonGroup);
    return d.m_title;
}

/*!
Sets the \a title for the group.
 */
void ARibbonGroup::setTitle(const QString& title)
{
    A_D(ARibbonGroup);
    if (d.m_title == title)
        return;
    d.m_title = title;
    d.updateLayoutParent();
    emit titleChanged(d.m_title);
}

/*!
Returns the title font of the group.
*/ 
const QFont& ARibbonGroup::titleFont() const
{
    A_D(const ARibbonGroup);
    if (d.m_titleFont == QFont())
        return font();
    return d.m_titleFont;
}

/*!
Sets the \a title font for the group.
 */
void ARibbonGroup::setTitleFont(const QFont& font)
{
    A_D(ARibbonGroup);
    if (d.m_titleFont == font)
        return;
    d.m_titleFont = QFont(font, this);
    d.updateLayoutParent();
    emit titleFontChanged(d.m_titleFont);
}

/*!
Returns the title color of the group.
*/ 
const QColor& ARibbonGroup::titleColor() const
{
    A_D(const ARibbonGroup);
    return d.m_titleColor;
}

/*!
Sets the \a title color for the group.
 */
void ARibbonGroup::setTitleColor(const QColor& color)
{
    A_D(ARibbonGroup);
    if (d.m_titleColor == color)
        return;
    d.m_titleColor = color;
    update();
}

/*!
Returns the icon of the group.
*/ 
const QIcon& ARibbonGroup::icon() const
{
    A_D(const ARibbonGroup);
    return d.m_icon;
}

/*!
Sets the \a icon for the group.
 */
void ARibbonGroup::setIcon(const QIcon& icon)
{
    A_D(ARibbonGroup);
    d.m_icon = icon;
    d.updateLayoutParent();
}

/*!
Returns the visible state for the option button used in the caption of the group.
*/
bool ARibbonGroup::isOptionButtonVisible() const
{
    A_D(const ARibbonGroup);
    return d.m_butOptionVisible && (currentSize() == ARibbonControlSizeDefinition::GroupSimplified || 
        (ribbonBar() == nullptr || ribbonBar()->isTitleGroupsVisible()));
}

/*!
Sets the visible state for the option button used in the group caption.
Notes: Option button is usually used to start the dialogue associated with the group.
*/
void ARibbonGroup::setOptionButtonVisible(bool visible)
{
    A_D(ARibbonGroup);
    if (d.m_butOptionVisible == visible)
        return;
    d.m_butOptionVisible = visible;
    d.updateLayoutParent();
}


/*!
Returns the pointer to action associated with the option button. 
Notes: Use this action if you want to initialize tooltips, the text in the status bar (StatusTip), icons or for to define custom events.
*/ 
QAction* ARibbonGroup::optionButtonAction() const
{
    A_D(const ARibbonGroup);
    return d.m_actOption;
}

/*!
  Sets the given \a action for the option button of the group. Option button is a small button on the bottom of the group. If action is not set then the option button is not visible.
*/ 
void ARibbonGroup::setOptionButtonAction(QAction* action)
{
    A_D(ARibbonGroup);
    if ( d.m_actOption != nullptr )
        delete d.m_actOption;
    d.m_actOption = action;
    d.m_optionButton->setDefaultAction(d.m_actOption);
}

/*!
Returns the enum that describes the ribbon control alignment for the control's column within the given group. 
*/
Qt::Alignment ARibbonGroup::contentAlignment() const
{
    A_D(const ARibbonGroup);
    return d.m_contentAlignment;
}

/*!
Sets the enum that describes the ribbon control alignment for the control's column within the given group.
*/
void ARibbonGroup::setContentAlignment(Qt::Alignment alignment)
{
    A_D(ARibbonGroup);
    if (d.m_contentAlignment == alignment)
        return;
    d.m_contentAlignment = alignment;
    d.updateLayoutParent();
}

/*!
Returns the alignment of block ribbon controls if width of the all controls is more than minmum width of the group.
*/
Qt::Alignment ARibbonGroup::controlsAlignment() const
{
    A_D(const ARibbonGroup);
    return d.m_controlsAlignment;
}

/*!
Sets the alignment of the block ribbon controls.
*/
void ARibbonGroup::setControlsAlignment(Qt::Alignment alignment)
{
    A_D(ARibbonGroup);
    if (d.m_controlsAlignment == alignment)
        return;
    d.m_controlsAlignment = alignment;
    d.updateLayoutParent();
}

/*!
Returns the spacing between the items inside the group.
*/
int ARibbonGroup::spacing() const
{
    A_D(const ARibbonGroup);
    return d.m_spacing;
}

/*!
Sets the spacing between the items inside the group.
*/
void ARibbonGroup::setSpacing(int spacing)
{
    A_D(ARibbonGroup);
    if (d.m_spacing == spacing)
        return;
    d.m_spacing = spacing;
    d.updateLayoutParent();
}

/*!
Returns the count of ribbon controls within the group.
\sa controlByWidget, controlByIndex, controlByAction
*/
int ARibbonGroup::controlCount() const
{
    A_D(const ARibbonGroup);
    return d.m_controls.size();
}

/*!
Sets the feature controls centering to \a enabled.

\sa setContentAlignment()
*/
void ARibbonGroup::setControlsCentering(bool enabled)
{
    A_D(ARibbonGroup);
    if (enabled)
        d.m_contentAlignment = Qt::AlignVCenter;
    else
        d.m_contentAlignment = Qt::Alignment();
    d.updateLayoutParent();
}

/*!
Returns the value of the feature controls centering.
*/
bool ARibbonGroup::isControlsCentering() const
{
    A_D(const ARibbonGroup);
    return d.m_contentAlignment.testFlag(Qt::AlignVCenter);
}

/*!
Sets the feature grouping controls to \a enabled.
*/
void ARibbonGroup::setControlsGrouping(bool enabled)
{
    A_D(ARibbonGroup);
    if (enabled)
    {
        if (isControlsGrouping())
            return;
        d.m_toolBar = new RibbonToolBarControl(this);
        addControl(d.m_toolBar);
    }
    else
    {
        if (d.m_toolBar != nullptr)
            removeControl(d.m_toolBar);
        Q_DELETE_AND_NULL(d.m_toolBar);
    }
}

/*!
Returns the value of the feature grouping controls.
*/
bool ARibbonGroup::isControlsGrouping() const
{
    A_D(const ARibbonGroup);
    return d.m_toolBar != nullptr;
}

/*!
Returns the ribbon control by \a index.
\sa controlByWidget, controlByIndex, controlByAction
*/
RibbonControl* ARibbonGroup::controlByIndex(int index) const
{
    A_D(const ARibbonGroup);
    return d.m_controls[index];
}

/*!
Returns the ribbon control by \a action.
\sa controlCount, controlByIndex, controlByWidget
*/
RibbonControl* ARibbonGroup::controlByAction(QAction* action) const
{
    A_D(const ARibbonGroup);
    Q_ASSERT(action != nullptr);
    for (QList<RibbonControl *>::const_iterator it = d.m_controls.constBegin(); it != d.m_controls.constEnd(); ++it)
    {
        if ((*it)->defaultAction() == action)
            return *it;
    }
    return nullptr;
}

/*!
Returns the widget ribbon control by \a widget.
\sa controlCount, controlByIndex, controlByAction
*/
RibbonWidgetControl* ARibbonGroup::controlByWidget(QWidget* widget) const
{
    A_D(const ARibbonGroup);
    Q_ASSERT(widget != nullptr);
    for (QList<RibbonControl *>::const_iterator it = d.m_controls.constBegin(); it != d.m_controls.constEnd(); ++it)
    {
        if (RibbonWidgetControl* widgetControl = qobject_cast<RibbonWidgetControl*>((*it)))
        {
            if (widgetControl->contentWidget() == widget)
                return widgetControl;
        }
    }
    return nullptr;
}

ARibbonControlSizeDefinition::GroupSize ARibbonGroup::currentSize() const
{
    A_D(const ARibbonGroup);
    return aproch_get_groupsize(const_cast<RibbonGroupPrivate*>(&d)->m_currentSize, this);
}

/*!
Adds the ribbon \a control to the group. In most cases you have to use addAction to create and add ribbon control. 
*/
void ARibbonGroup::addControl(RibbonControl* control)
{
    A_D(ARibbonGroup);
    if (d.m_controls.indexOf(control) != -1)
        return; // Already was added. 

    control->aproch_d().m_parentGroup = this;
    d.m_controls.append(control);
    control->sizeChanged(currentSize());
    d.updateLayoutParent();
}

/*!
Removes the ribbon \a control from the group.
*/
void ARibbonGroup::removeControl(RibbonControl* control)
{
    A_D(ARibbonGroup);
    if (d.m_controls.removeAll(control) > 0)
    {
        control->aproch_d().m_parentGroup = nullptr;
        control->setParent(nullptr);
        d.updateLayoutParent();
    }
}

/*!
  Creates and adds the action to the group with the given \a icon, \a text and \a style. If you specify the parameter \a menu, then will be constructed a special split button which will allow to cause this menu. Parameter \a mode defines the mode of the popup emerging.
*/ 
QAction* ARibbonGroup::addAction(const QIcon& icon, const QString& text, Qt::ToolButtonStyle style, 
                                QMenu* menu, QToolButton::ToolButtonPopupMode mode)
{
    A_D(const ARibbonGroup);

    if (isControlsGrouping())
        return d.m_toolBar->addAction(icon, text, style, menu, mode);

    RibbonButtonAction* action = new RibbonButtonAction(this, icon, text, style, menu, mode);
    addAction(action);
    return action;
}

/*!
  Adds the \a action to the group with the given parameters \a style, \a menu and \a mode.
*/ 
QAction* ARibbonGroup::addAction(QAction* action, Qt::ToolButtonStyle style, QMenu* menu, QToolButton::ToolButtonPopupMode mode)
{
    A_D(ARibbonGroup);

    if (isControlsGrouping())
        return d.m_toolBar->addAction(action, style, menu, mode);

    addAction(action);

    RibbonButtonControl* control = qobject_cast<RibbonButtonControl*>(controlByAction(action));
    Q_ASSERT(control != nullptr);

    control->setToolButtonStyle(style);
    if (menu || action->menu())
        control->setPopupMode(mode);
    if (menu)
        control->setMenu(menu);

    return action;
}

/*!
  Adds \a action before other action that specified in \a before argument. In result action's widget will be placed before widget that is associated with action \a before. 
*/ 
QAction* ARibbonGroup::insertAction(QAction* before, QAction* action, Qt::ToolButtonStyle style, QMenu* menu, QToolButton::ToolButtonPopupMode mode)
{
    A_D(ARibbonGroup);

    if (isControlsGrouping())
        return d.m_toolBar->insertAction(before, action, style, menu, mode);

    insertAction(before, action);

    RibbonButtonControl* control = qobject_cast<RibbonButtonControl*>(controlByAction(action));
    Q_ASSERT(control != nullptr);

    control->setToolButtonStyle(style);
    if (menu || action->menu())
        control->setPopupMode(mode);
    if (menu)
        control->setMenu(menu);

    return action;
}

/*!
Adds the \a widget to the group. As alternative can be created QWidgetAction and added it via addAction method.  
*/
QAction* ARibbonGroup::addWidget(QWidget* widget)
{
    A_D(ARibbonGroup);
    if (isControlsGrouping())
        return d.m_toolBar->addWidget(widget);
    return addWidget(QIcon(), QString(), widget);
}

/*!
Overloaded method. Adds the \a widget to the group with given \a icon and \a text. As alternative can be created QWidgetAction and added it via addAction method.
*/
QAction* ARibbonGroup::addWidget(const QIcon& icon, const QString& text, QWidget* widget)
{
    A_D(ARibbonGroup);

    if (isControlsGrouping())
        return d.m_toolBar->addWidget(icon, text, widget);

    QWidgetAction* action = new QWidgetAction(this);
    action->setIcon(icon);
    action->setText(text);

    RibbonWidgetControl* control = new RibbonWidgetControl(this, false);
    control->setContentWidget(widget);
    control->setDefaultAction(action);
    action->setDefaultWidget(control);
    addAction(action);

    return action;
}

/*!
Overloaded method. Adds the \a widget to the group with given \a icon and \a text.
Parameter \a stretch specifies a the ability to adjust the ribbon control which corresponds to the widget
on the control's column width within the group.
*/
QAction* ARibbonGroup::addWidget(const QIcon& icon, const QString& text, bool stretch, QWidget* widget)
{
    A_D(ARibbonGroup);

    if (isControlsGrouping())
        return d.m_toolBar->addWidget(icon, text, widget);

    QAction* action = addWidget(icon, text, widget);
    if (stretch)
    {
        RibbonControl* control = controlByAction(action);
        Q_ASSERT(control != nullptr);
        control->sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setStretchable(true);
        control->sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setStretchable(true);
        control->sizeDefinition(ARibbonControlSizeDefinition::GroupSmall)->setStretchable(true);
        control->sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setStretchable(true);
    }
    return action;
}

/*!
Adds the ribbon button with the given \a icon and \a text that holds the QMenu.
Note, parameter \a style can be any value of type Qt::ToolButtonStyle except Qt::ToolButtonFollowStyle. 
In the case of Qt::ToolButtonFollowStyle will be used Qt::ToolButtonTextUnderIcon instead.
*/
QMenu* ARibbonGroup::addMenu(const QIcon& icon, const QString& text, Qt::ToolButtonStyle style)
{
    A_D(ARibbonGroup);

    QMenu* menu = new QMenu(text, this);
    if (isControlsGrouping())
    {
        d.m_toolBar->addMenu(icon, text, menu, QToolButton::InstantPopup);
    }
    else
    {
        RibbonButtonAction* action = new RibbonButtonAction(this, icon, text, style, menu, QToolButton::InstantPopup);
        addAction(action);
    }
    return menu;
}

/*!
Adds a separator action to the given group.
*/
QAction* ARibbonGroup::addSeparator()
{
    A_D(ARibbonGroup);

    if (isControlsGrouping())
        return d.m_toolBar->addSeparator();

    QAction*action = new QAction(this);
    action->setSeparator(true);
    addAction(action);
    return action;
}

/*!
Removes ribbon control from the group which corresponds to the \a widget.
*/
void ARibbonGroup::remove(QWidget* widget)
{
    if (isControlsGrouping())
    {
        Q_ASSERT(false);
        //d.m_toolBar->removeWidget(widget);
        return;
    }

    if (!widget)
        return;


    QList<QAction*> actions = this->actions();
    for (int i = 0; i < actions.size(); i++)
    {
        if (QWidgetAction* widgetAction = qobject_cast<QWidgetAction*>(actions.at(i)))
        {
            if (widgetAction->defaultWidget() == widget)
            {
                removeAction(actions.at(i));
                return;
            }
        }
    }
}

/*!
Removes all ribbon controls from the given group.
*/
void ARibbonGroup::clear()
{
    A_D(ARibbonGroup);
    d.clear(false);
}

/*!
Returns the enum where ellipsis should be added for group title.
\sa setTitleElideMode
*/
Qt::TextElideMode ARibbonGroup::titleElideMode() const
{
    A_D(const ARibbonGroup);
    return d.m_titleElideMode;
}

/*!
Sets the enum where ellipsis should be added for group title.
The \a mode parameter affects the calculation of the width of the group and can take the following values:
\table
\row
\li Qt::ElideLeft
\li The text of the group name has an ellipsis on the left. The width of the group will be equal to the width of the content of the group depending on ARibbonControlSizeDefinition::GroupSize.
\row
\li Qt::ElideRight
\li The text of the group name has an ellipsis on the right. The width of the group will be equal to the width of the content of the group depending on ARibbonControlSizeDefinition::GroupSize.
\row
\li Qt::ElideMiddle
\li The text of the group name has an ellipsis on the middle. The width of the group will be equal to the width of the content of the group depending on ARibbonControlSizeDefinition::GroupSize.
\row
\li Qt::ElideNone
\li The text of the group name occupies the entire width of the group and does not have ellipses. The group width will be equal to the maximum width of the group content depending on the ARibbonControlSizeDefinition::GroupSize or text length.
\endtable
*/
void ARibbonGroup::setTitleElideMode(Qt::TextElideMode mode)
{
    A_D(ARibbonGroup);
    if (d.m_titleElideMode != mode)
    {
        d.m_titleElideMode = mode;
        d.updateLayoutParent();
    }
}

/*!
Returns the recommended size for ARibbonGroup.
\reimp
*/
QSize ARibbonGroup::sizeHint() const
{
    A_D(const ARibbonGroup);
    QSize result = d.sizeHint();
    if (isReduced())
    {
        RibbonGroupStyleOption opt;
        d.initStyleOption(opt);
		opt.text = d.m_title;
		opt.heightCaption = RibbonGroupPrivate::calcGroupTitleHeight(titleFont());
        // 组类型预留箭头宽度
        result = QSize(style()->pixelMetric(static_cast<QStyle::PixelMetric>(CommonStyle::PM_RibbonGroupReducedWidth),
                                          &opt, this)+20, result.height());
    }
    return result;
}

void ARibbonGroup::currentIndexChanged(int index)
{
    Q_UNUSED(index);
    if (isVisible())
    {
        bool doReleased = true;
        QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
        if (comboBox)
        {
            QAbstractItemView* itemView = comboBox->view();
            if (itemView && !itemView->isVisible())
                doReleased = false;
        }
        if (doReleased)
            emit released();
    }
}

/*! \reimp */
bool ARibbonGroup::event(QEvent* event)
{
    A_D(ARibbonGroup);

    switch (event->type())
    {
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
        default:
            break;
    }

    if (isMinimized())
    {
        if (event->type() == QEvent::Show)
            d.showGroupScroll();
        else if (event->type() == QEvent::Hide)
        {
            d.m_groupScrollPos = 0;
            d.m_pageScrollPos = 0;
            layout()->update();
            if (windowFlags() & Qt::Popup)
                emit hidePopup();
        }
    }
    else
    {
        switch(event->type())
        {
            case QEvent::KeyPress :
                {
                    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
                    if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
                    {
                        if (QWidget* focus = focusWidget())
                        {
                            if (qobject_cast<QComboBox*>(focus) || qobject_cast<QLineEdit*>(focus))
                                emit released();
                        }
                    }
                }
                break;
            case QEvent::Show:
            case QEvent::Hide:
                break;
            case QEvent::ParentChange:
                break;
            default:
                break;
        }
    }
    return QWidget::event(event);
}

/*! \reimp */
void ARibbonGroup::paintEvent(QPaintEvent* event)
{
    A_D(ARibbonGroup);
    QPainter p(this);
    if (isReduced())
    {
        RibbonGroupStyleOption opt;
        opt.initFrom(this);
        opt.text = d.m_title;
        opt.icon = d.m_icon;
        opt.features = QStyleOptionToolButton::None;
        int sz = style()->pixelMetric(QStyle::PM_SmallIconSize, &opt, this);
        opt.iconSize = QSize(sz, sz);
        opt.subControls = QStyle::SC_ToolButton;

        if (ARibbonPage* page = d.parentPage())
            opt.contextColor = page->contextColor();

        if (!d.groupPopup()->isHidden())
        {
            opt.state |= QStyle::State_Sunken;
            opt.features |= QStyleOptionToolButton::HasMenu;
        }

        QRect rect = opt.rect;
        rect.setBottom(rect.bottom() - 2);
        opt.rect = rect;
        style()->drawControl(static_cast<QStyle::ControlElement>(CommonStyle::CE_RibbonReducedGroup), &opt, &p, this);
    }
    else
    {
        QRect rectEvent = event->rect();
        p.setClipRect(rectEvent);
        RibbonGroupStyleOption opt;
        d.initStyleOption(opt);
        style()->drawPrimitive(static_cast<QStyle::PrimitiveElement>(CommonStyle::PE_RibbonFrameGroup), &opt, &p, this);

        if (opt.heightCaption > 0)
        {
            QFont saveFont = p.font();
            // Set the font to draw the group name
            p.setFont(titleFont());
            style()->drawControl(static_cast<QStyle::ControlElement>(CommonStyle::CE_RibbonGroupLabel), &opt, &p, this);
            p.setFont(saveFont);
        }
    }
}

/*! \reimp */
void ARibbonGroup::actionEvent(QActionEvent* event)
{
    QAction* action = event->action();
    RibbonControl* control = controlByAction(action);

    switch (event->type()) 
    {
        case QEvent::ActionAdded: 
        {
            if (control == nullptr)
            {
                if (QWidgetAction* widgetAction = qobject_cast<QWidgetAction*>(action)) 
                {
                    QWidget* widget = widgetAction->requestWidget(this);
                    if (widget != nullptr) 
                    {
                        control = qobject_cast<RibbonControl*>(widget);
                        if (control == nullptr)
                        {
                            RibbonWidgetControl* widgetControl = new RibbonWidgetControl(this, false);
                            widgetControl->setContentWidget(widget);
                            control = widgetControl;
                            control->setDefaultAction(action);
                            widgetAction->setDefaultWidget(control);
                        }
                    }
                    else
                    {
                        Q_ASSERT(false);
                        return;
                    }
                }
                else if (action->isSeparator())
                    control = new RibbonColumnBreakControl(this);

                if (control == nullptr)
                    control = new RibbonButtonControl(this);

                control->setDefaultAction(action);
                addControl(control);
            }
        }
        break;
        case QEvent::ActionRemoved:
        {
            if (control != nullptr)
            {
                removeControl(control);
                if (QWidgetAction* widgetAction = qobject_cast<QWidgetAction*>(action))
                {
                    bool forceDelete = false;
                    if (widgetAction->defaultWidget() == control)
                        widgetAction->releaseWidget(control);
                    else if (RibbonWidgetControl* widgetControl = qobject_cast<RibbonWidgetControl*>(control))
                    {
                        if (QWidget* contentWidget = widgetControl->contentWidget())
                        {
                            widgetAction->releaseWidget(contentWidget);
                            if (widgetAction->defaultWidget() == contentWidget)
                                forceDelete = true;
                        }
                    }

                    if (forceDelete || widgetAction->defaultWidget() == nullptr)
                        delete control;
                }
                else
                {
                    control->hide();
                    delete control;
                }
            }
        }
        break;
        case QEvent::ActionChanged:
            {
                Q_ASSERT(control != nullptr);
                if (control != nullptr)
                    control->actionChanged();
                break;
            }
        default:
            Q_ASSERT_X(false, "ARibbonGroup::actionEvent", "internal error");
    }
}

/*! \reimp */
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
void ARibbonGroup::enterEvent(QEnterEvent* event)
#else
void ARibbonGroup::enterEvent(QEvent* event)
#endif
{
    Q_UNUSED(event);
    update();
}

/*! \reimp */
void ARibbonGroup::leaveEvent(QEvent* event)
{
    Q_UNUSED(event);
    update();
}

/*! \reimp */
void ARibbonGroup::mousePressEvent(QMouseEvent* event)
{
    A_D(ARibbonGroup);
    if (isReduced())
    {
        if (event->button() == Qt::LeftButton)
        {
            d.showPopupMenu();
            event->accept();
            update();
            return;
        }
    }
    QWidget::mousePressEvent(event);
}

