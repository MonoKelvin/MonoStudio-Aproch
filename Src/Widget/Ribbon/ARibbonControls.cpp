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
#include <QAction>
#include <QStyleOption>
#include <QPainter>
#include <QToolBar>
#include <QScreen>

#include "QtnPlatform.h"
#include "ARibbonControls.h"
#include "ARibbonControlsPrivate.h"
#include "ARibbonGroupPrivate.h"
#include "ARibbonBarPrivate.h"
#include "QtnCommonStylePrivate.h"
#ifdef A_MEMORY_DEBUG
#include "QtitanMSVSDebug.h"
#endif

QTITAN_USE_NAMESPACE

RibbonControlSizeDefinitionPrivate::RibbonControlSizeDefinitionPrivate()
    : m_parentControl(nullptr)
{
}

void RibbonControlSizeDefinitionPrivate::init(RibbonControl* parentControl, ARibbonControlSizeDefinition::GroupSize size)
{
    m_parentControl = parentControl;

    m_imageSize = ARibbonControlSizeDefinition::ImageSmall;
    m_labelVisible = true;
    m_popup = size == ARibbonControlSizeDefinition::GroupPopup;
    m_showSeparator = true;
    m_visible = true;
    m_enabled = true;
    m_stretchable = true;
    m_wordWrap = true;
    m_visualIndex = -1;
    m_minItemCount = -1;
    m_maxItemCount = -1;

    if (size == ARibbonControlSizeDefinition::GroupSimplified || 
        size == ARibbonControlSizeDefinition::GroupSimplifiedOverflow)
    {
    }
    else if (size == ARibbonControlSizeDefinition::GroupLarge || size == ARibbonControlSizeDefinition::GroupPopup)
    {
        m_imageSize = ARibbonControlSizeDefinition::ImageLarge;
        m_labelVisible = true;
    }
    else
    {
        m_imageSize = ARibbonControlSizeDefinition::ImageSmall;
        if (size == ARibbonControlSizeDefinition::GroupMedium)
            m_labelVisible = true;
        else 
            m_labelVisible = false;
    }
}

void RibbonControlSizeDefinitionPrivate::updateSizeDefinition()
{
    m_parentControl->sizeChanged(m_parentControl->currentSize());
}

RibbonControlSizeDefinitionPrivate::~RibbonControlSizeDefinitionPrivate()
{
}

/*!
\class ARibbonControlSizeDefinition
\inmodule QtitanRibbon
*/
ARibbonControlSizeDefinition::ARibbonControlSizeDefinition(RibbonControl* parentControl, ARibbonControlSizeDefinition::GroupSize size)
    : QObject()
{
    A_INIT_PRIVATE(ARibbonControlSizeDefinition);
    A_D(ARibbonControlSizeDefinition);
    d.init(parentControl, size);
}

ARibbonControlSizeDefinition::~ARibbonControlSizeDefinition()
{
    A_FINI_PRIVATE();
}

ARibbonControlSizeDefinition::ControlImageSize ARibbonControlSizeDefinition::imageSize() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_imageSize;
}

void ARibbonControlSizeDefinition::setImageSize(ARibbonControlSizeDefinition::ControlImageSize size)
{
    A_D(ARibbonControlSizeDefinition);
    if (d.m_imageSize != size)
    {
        d.m_imageSize = size;
        d.updateSizeDefinition();
    }
}

bool ARibbonControlSizeDefinition::isLabelVisible() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_labelVisible;
}

void ARibbonControlSizeDefinition::setLabelVisible(bool visible)
{
    A_D(ARibbonControlSizeDefinition);
    if (d.m_labelVisible != visible)
    {
        d.m_labelVisible = visible;
        d.updateSizeDefinition();
    }
}

bool ARibbonControlSizeDefinition::isPopup() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_popup;
}

void ARibbonControlSizeDefinition::setPopup(bool popup)
{
    A_D(ARibbonControlSizeDefinition);
    d.m_popup = popup;
}

int ARibbonControlSizeDefinition::visualIndex() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_visualIndex;
}

void ARibbonControlSizeDefinition::setVisualIndex(int index)
{
    A_D(ARibbonControlSizeDefinition);
    d.m_visualIndex = index;
}

bool ARibbonControlSizeDefinition::showSeparator() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_showSeparator;
}

bool ARibbonControlSizeDefinition::wordWrap() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_wordWrap;
}

void ARibbonControlSizeDefinition::setWordWrap(bool on)
{
    A_D(ARibbonControlSizeDefinition);
    if (d.m_wordWrap != on)
    {
        d.m_wordWrap = on;
        d.updateSizeDefinition();
    }
}

void ARibbonControlSizeDefinition::setShowSeparator(bool showSeparator)
{
    A_D(ARibbonControlSizeDefinition);
    d.m_showSeparator = showSeparator;
}

bool ARibbonControlSizeDefinition::isVisible() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_visible;
}

void ARibbonControlSizeDefinition::setVisible(bool visible)
{
    A_D(ARibbonControlSizeDefinition);
    d.m_visible = visible;
}

bool ARibbonControlSizeDefinition::isEnabled() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_enabled;
}

void ARibbonControlSizeDefinition::setEnabled(bool enabled)
{
    A_D(ARibbonControlSizeDefinition);
    d.m_enabled = enabled;
}

bool ARibbonControlSizeDefinition::isStretchable() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_stretchable;
}

void ARibbonControlSizeDefinition::setStretchable(bool stretchable)
{
    A_D(ARibbonControlSizeDefinition);
    d.m_stretchable = stretchable;
}

int ARibbonControlSizeDefinition::minimumItemCount() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_minItemCount;
}

void ARibbonControlSizeDefinition::setMinimumItemCount(int count)
{
    A_D(ARibbonControlSizeDefinition);
    d.m_minItemCount = count;
}

int ARibbonControlSizeDefinition::maximumItemCount() const
{
    A_D(const ARibbonControlSizeDefinition);
    return d.m_maxItemCount;
}

void ARibbonControlSizeDefinition::setMaximumItemCount(int count)
{
    A_D(ARibbonControlSizeDefinition);
    d.m_maxItemCount = count;
}

/* RibbonControlPrivate */
RibbonControlPrivate::RibbonControlPrivate()
    : m_parentGroup(nullptr)
    , m_size(ARibbonControlSizeDefinition::GroupLarge)
    , m_action(nullptr)
{
}

RibbonControlPrivate::~RibbonControlPrivate()
{
    delete m_allSizes[0];
    delete m_allSizes[1];
    delete m_allSizes[2];
    delete m_allSizes[3];
    delete m_allSizes[4];
    delete m_allSizes[5];
    m_allSizes.clear();
}

void RibbonControlPrivate::init(ARibbonGroup* parentGroup)
{
    A_P(RibbonControl);
    if (m_parentGroup != nullptr)
    {
        m_parentGroup->removeControl(&p);

		for (auto size : m_allSizes)
		{
			delete size;
		}
		m_allSizes.clear();
    }

    m_parentGroup = parentGroup;
    m_allSizes.append(new ARibbonControlSizeDefinition(&p, ARibbonControlSizeDefinition::GroupLarge));  //Large
    m_allSizes.append(new ARibbonControlSizeDefinition(&p, ARibbonControlSizeDefinition::GroupMedium)); //Medium
    m_allSizes.append(new ARibbonControlSizeDefinition(&p, ARibbonControlSizeDefinition::GroupSmall));  //Small
    m_allSizes.append(new ARibbonControlSizeDefinition(&p, ARibbonControlSizeDefinition::GroupPopup));  //Popup
    m_allSizes.append(new ARibbonControlSizeDefinition(&p, ARibbonControlSizeDefinition::GroupSimplified));  //Simplified
    m_allSizes.append(new ARibbonControlSizeDefinition(&p, ARibbonControlSizeDefinition::GroupSimplifiedOverflow));  //Simplified Overflow
}

void RibbonControlPrivate::changed()
{
}

/*!
\class RibbonControl
\inmodule QtitanRibbon
*/
RibbonControl::RibbonControl(ARibbonGroup* parentGroup)
    : QWidget(nullptr)
{
    A_INIT_PRIVATE(RibbonControl)
    A_D(RibbonControl);
    d.init(parentGroup);
}

RibbonControl::~RibbonControl()
{
    if (parentGroup())
        parentGroup()->removeControl(this);
    A_FINI_PRIVATE()
}

void RibbonControl::setParentGroup(ARibbonGroup* parentGroup)
{
    A_D(RibbonControl);
	d.init(parentGroup);
}

ARibbonGroup* RibbonControl::parentGroup() const
{
    A_D(const RibbonControl);
    return d.m_parentGroup;
}

QAction* RibbonControl::defaultAction() const
{
    A_D(const RibbonControl);
    return d.m_action;
}

void RibbonControl::setDefaultAction(QAction* action)
{
    A_D(RibbonControl);
    if (d.m_action == action)
        return;
    d.m_action = action;
    actionChanged();
}

void RibbonControl::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
}

void RibbonControl::setDirty()
{
    //Do nothing.
}

QSize RibbonControl::getBoundedSizeHint(const QWidget* widget)
{
    QSize ret = widget->sizeHint();
    ret.rwidth() = qMin(widget->maximumWidth(), ret.width());
    ret.rheight() = qMin(widget->maximumHeight(), ret.height());
    ret.rwidth() = qMax(widget->minimumWidth(), ret.width());
    ret.rheight() = qMax(widget->minimumHeight(), ret.height());
    return ret;
}

ARibbonControlSizeDefinition* RibbonControl::sizeDefinition(ARibbonControlSizeDefinition::GroupSize size) const
{
    A_D(const RibbonControl);
    if ((int)size < 0 || (int)size >= d.m_allSizes.count())
    {
        Q_ASSERT(false);
        return d.m_allSizes[0];
    }
    return d.m_allSizes[(int)size];
}

bool RibbonControl::adjustCurrentSize(bool expand)
{
    ARibbonControlSizeDefinition::GroupSize size = currentSize();
    if (size < ARibbonControlSizeDefinition::GroupSimplified)
        return false;

    if (expand)
    {
        if (size == ARibbonControlSizeDefinition::GroupSimplifiedOverflow)
            sizeChanged(ARibbonControlSizeDefinition::GroupSimplified);
    }
    else
    {
        if (size == ARibbonControlSizeDefinition::GroupSimplified)
            sizeChanged(ARibbonControlSizeDefinition::GroupSimplifiedOverflow);
    }
    return size != currentSize();
}

void RibbonControl::sizeChanged(ARibbonControlSizeDefinition::GroupSize size)
{
    A_D(RibbonControl);
    d.m_size = size;
}

void RibbonControl::actionChanged()
{
    A_D(RibbonControl);
    d.changed();
}

void RibbonControl::updateLayout()
{
}

ARibbonControlSizeDefinition::GroupSize RibbonControl::currentSize() const
{
    A_D(const RibbonControl);
    ARibbonControlSizeDefinition::GroupSize old = d.m_size;
    aproch_get_groupsize(const_cast<RibbonControlPrivate*>(&d)->m_size, parentGroup());
    if (old != d.m_size)
        const_cast<RibbonControl*>(this)->sizeChanged(d.m_size);
    return d.m_size;
}

/* RibbonWidgetControlPrivate */
RibbonWidgetControlPrivate::RibbonWidgetControlPrivate()
    : m_widget(nullptr)
    , m_visibleText(false)
    , m_ignoreActionSettings(false)
    , m_margin(4)
{
}

RibbonWidgetControlPrivate::~RibbonWidgetControlPrivate()
{
}

void RibbonWidgetControlPrivate::init(bool ignoreActionSettings)
{
    A_P(RibbonWidgetControl);
    m_ignoreActionSettings = ignoreActionSettings;
    p.sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setLabelVisible(true);
    p.sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setLabelVisible(false);
    p.sizeDefinition(ARibbonControlSizeDefinition::GroupSmall)->setLabelVisible(false);
    p.sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setLabelVisible(true);

    if (!ignoreActionSettings)
        p.setAttribute(Qt::WA_Hover);
}

void RibbonWidgetControlPrivate::initStyleOption(QStyleOptionButton* option) const
{
    A_P(const RibbonWidgetControl);
    option->initFrom(&p);
    QAction* action = p.defaultAction();
    if (action && !m_ignoreActionSettings)
    {
        option->icon = action->icon();
        option->text = m_visibleText ? action->text() : QString();
        QSize szIcon = option->icon.actualSize(option->rect.size());
        option->iconSize = szIcon;
    }
}

void RibbonWidgetControlPrivate::updateFont()
{
    if (m_widget != nullptr)
        m_widget->setFont(QFont());
}

/*!
\class RibbonWidgetControl
\inmodule QtitanRibbon
*/
RibbonWidgetControl::RibbonWidgetControl(ARibbonGroup* parentGroup, bool ignoreActionSettings)
    : RibbonControl(parentGroup)
{
    A_INIT_PRIVATE(RibbonWidgetControl);
    A_D(RibbonWidgetControl);
    d.init(ignoreActionSettings);
}

RibbonWidgetControl::~RibbonWidgetControl()
{
    A_FINI_PRIVATE();
}

int RibbonWidgetControl::margin() const
{
    A_D(const RibbonWidgetControl);
    return d.m_margin;
}

void RibbonWidgetControl::setMargin(int margin)
{
    A_D(RibbonWidgetControl);
    d.m_margin = margin;
}

QWidget* RibbonWidgetControl::contentWidget() const
{
    A_D(const RibbonWidgetControl);
    return d.m_widget;
}

void RibbonWidgetControl::setContentWidget(QWidget* widget)
{
    A_D(RibbonWidgetControl);
    if (d.m_widget != nullptr)
        delete d.m_widget;

    d.m_widget = widget;
    d.updateFont();

    if (d.m_widget != nullptr)
    {
        d.m_widget->setParent(this);
        d.m_widget->setVisible(true);
        d.m_widget->setAttribute(Qt::WA_MacSmallSize);
    }
}

void RibbonWidgetControl::sizeChanged(ARibbonControlSizeDefinition::GroupSize size)
{
    A_D(RibbonWidgetControl);
    RibbonControl::sizeChanged(size);
    d.m_visibleText = sizeDefinition(size)->isLabelVisible();
}

QSize RibbonWidgetControl::sizeHint() const
{
    A_D(const RibbonWidgetControl);
    if (d.m_widget != nullptr)
    {
        QSize sz = getBoundedSizeHint(d.m_widget);

        QStyleOptionButton opt;
        d.initStyleOption(&opt);

        if (!opt.icon.isNull())
            sz.setWidth(sz.width() + opt.iconSize.width() + d.m_margin);

        if (!opt.text.isEmpty())
        {
            QFontMetrics fm = opt.fontMetrics;
            QSize szText = fm.boundingRect(opt.text).size();
            sz.setWidth(sz.width() + szText.width() + d.m_margin);
        }
        return sz;
    }
    return RibbonControl::sizeHint();
}

void RibbonWidgetControl::childEvent(QChildEvent* event)
{
    A_D(RibbonWidgetControl);
    RibbonControl::childEvent(event);

    if (event->type() == QEvent::StyleChange)
        d.updateFont();

    if (event->type() == QEvent::ChildRemoved)
    {
        if (ARibbonGroup* group = parentGroup())
        {
            if (QAction* action = defaultAction())
            {
                QChildEvent* childEvent = (QChildEvent*)event;
                if (childEvent->child()->isWidgetType())
                {
                    if (childEvent->child() == contentWidget())
                    {
                        d.m_widget = nullptr;
                        group->removeAction(action);
                    }
                }
            }
        }
    }
}

void RibbonWidgetControl::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    A_D(RibbonWidgetControl);
    if (!d.m_ignoreActionSettings && d.m_widget != nullptr)
    {
        QPainter painter(this);

        QStyleOptionButton opt;
        d.initStyleOption(&opt);

        bool enabled = d.m_widget->isEnabled();
        int posText = 0;

        if (!opt.icon.isNull())
        {
            QRect rcIcon = opt.rect;
            rcIcon.setWidth(opt.iconSize.width());
            opt.icon.paint(&painter, rcIcon, Qt::Alignment(Qt::AlignLeft|Qt::AlignVCenter), enabled ? QIcon::Normal : QIcon::Disabled);
            posText = rcIcon.width() + d.m_margin;
        }

        if (!opt.text.isEmpty())
        {
            QRect rectText = opt.rect;
            rectText.setLeft(posText);

            QFontMetrics fm = opt.fontMetrics;
            QSize szText = fm.boundingRect(opt.text).size();
            rectText.setWidth(szText.width());

            int flags = Qt::TextHideMnemonic | Qt::AlignVCenter;
            style()->drawItemText(&painter, rectText, flags, opt.palette, opt.state & QStyle::State_Enabled, opt.text, QPalette::WindowText);
        }
    }
}

void RibbonWidgetControl::resizeEvent(QResizeEvent* event)
{
    A_D(RibbonWidgetControl);
    if (d.m_widget != nullptr)
    {
        int x = 0;
        int y = 0;

        QSize sz = event->size();

        QStyleOptionButton opt;
        d.initStyleOption(&opt);

        if (!opt.icon.isNull())
        {
            x = opt.iconSize.width();
            x += d.m_margin;
        }
        if (!opt.text.isEmpty())
        {
            QFontMetrics fm = opt.fontMetrics;
            QSize szText = fm.boundingRect(opt.text).size();
            x += szText.width();
            x += d.m_margin;
        }

        sz.rwidth() -= x;
        QRect r = QRect(x, y, sz.width(), sz.height());
        sz.rwidth() = qMin(sz.width(), d.m_widget->maximumWidth());
        QRect controlRect = QStyle::alignedRect(layoutDirection(), Qt::AlignRight, sz, r);
        d.m_widget->setGeometry(controlRect);
    }

    RibbonControl::resizeEvent(event);
}


/* RibbonColumnBreakControlPrivate */
RibbonColumnBreakControlPrivate::RibbonColumnBreakControlPrivate()
    : m_textAlignment(Qt::AlignVCenter)
{
}

RibbonColumnBreakControlPrivate::~RibbonColumnBreakControlPrivate()
{
}

void RibbonColumnBreakControlPrivate::init()
{
    A_P(RibbonColumnBreakControl);
    QFont fnt = QApplication::font("QMenuBar");
    qreal pointSize = 0.8 * fnt.pointSizeF();
    fnt.setPointSizeF(pointSize);
    p.setFont(fnt);
}

void RibbonColumnBreakControlPrivate::initStyleOption(QStyleOptionGroupBox* option) const
{
    A_P(const RibbonColumnBreakControl);
    option->initFrom(&p);
    option->state |= QStyle::State_Horizontal;
    option->textAlignment = m_textAlignment;
    if (QAction* action = p.defaultAction())
        option->text = action->text();
}


/*!
\class RibbonColumnBreakControl
\inmodule QtitanRibbon
*/
RibbonColumnBreakControl::RibbonColumnBreakControl(ARibbonGroup* parentGroup)
    : RibbonWidgetControl(parentGroup, true)
{
    A_INIT_PRIVATE(RibbonColumnBreakControl);
    A_D(RibbonColumnBreakControl);
    d.init();
}

RibbonColumnBreakControl::~RibbonColumnBreakControl()
{
    A_FINI_PRIVATE()
}

Qt::Alignment RibbonColumnBreakControl::alignmentText() const
{
    A_D(const RibbonColumnBreakControl);
    return d.m_textAlignment;
}

void RibbonColumnBreakControl::setAlignmentText(Qt::Alignment alignment)
{
    A_D(RibbonColumnBreakControl);
    if (d.m_textAlignment != alignment)
    {
        switch (alignment)
        {
            case Qt::AlignTop:
                    d.m_textAlignment = alignment;
                break;
            case Qt::AlignBottom:
                    d.m_textAlignment = alignment;
                break;
            case Qt::AlignVCenter:
                    d.m_textAlignment = alignment;
                break;
            default:
                Q_ASSERT(false);
               break;
        }
        update();
    }
}

void RibbonColumnBreakControl::sizeChanged(ARibbonControlSizeDefinition::GroupSize size)
{
    RibbonControl::sizeChanged(size);
    if (sizeDefinition(size)->showSeparator())
    {
    }
    else
    {
    }
}

QSize RibbonColumnBreakControl::sizeHint() const
{
    A_D(const RibbonColumnBreakControl);
    if (!parentGroup() || !parentGroup()->ribbonBar())
        return QWidget::sizeHint();

    QStyleOptionGroupBox opt;
    d.initStyleOption(&opt);
    const int height = parentGroup()->ribbonBar()->rowItemHeight() * parentGroup()->ribbonBar()->rowItemCount();
    int extent = style()->pixelMetric(QStyle::PM_ToolBarSeparatorExtent, &opt, parentGroup());

    if (!opt.text.isEmpty())
    {
        QFontMetrics fm = opt.fontMetrics;
        extent += fm.height();
    }

    return QSize(extent, height);
}

void RibbonColumnBreakControl::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    A_D(RibbonColumnBreakControl);
    QPainter p(this);
    QStyleOptionGroupBox opt;
    d.initStyleOption(&opt);
    style()->drawPrimitive(QStyle::PE_IndicatorToolBarSeparator, &opt, &p, parentGroup());
}

/*!
\class RibbonLabelControl
\inmodule QtitanRibbon
*/
RibbonLabelControl::RibbonLabelControl(ARibbonGroup* parentGroup)
    : RibbonWidgetControl(parentGroup, true/*ignoreActionSettings*/)
{
    setContentWidget(new QLabel(this));
}

RibbonLabelControl::RibbonLabelControl(const QString& text, ARibbonGroup* parentGroup)
    : RibbonWidgetControl(parentGroup, true/*ignoreActionSettings*/)
{
    setContentWidget(new QLabel(text, this));
}

RibbonLabelControl::~RibbonLabelControl()
{
}

QLabel* RibbonLabelControl::widget() const
{
    return qobject_cast<QLabel*>(contentWidget());
}


/* RibbonToolBarSeparator */
RibbonToolBarSeparator::RibbonToolBarSeparator(RibbonToolBarControl* parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

QSize RibbonToolBarSeparator::sizeHint() const
{
    QSize sz = QWidget::sizeHint();
    QStyleOption opt;
    opt.initFrom(this);
    opt.state |= QStyle::State_Horizontal;
    const int extent = style()->pixelMetric(QStyle::PM_ToolBarSeparatorExtent, &opt, parentWidget());
    return QSize(extent, sz.width());
}

void RibbonToolBarSeparator::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    QStyleOption opt;
    opt.initFrom(this);
    opt.state |= QStyle::State_Horizontal;
    style()->drawPrimitive(QStyle::PE_IndicatorToolBarSeparator, &opt, &p, parentWidget());
}

/* RibbonToolBarOverflowMenu */
RibbonToolBarOverflowMenu::RibbonToolBarOverflowMenu(RibbonToolBarControl* parent)
    : PopupMenu(parent)
    , m_parent(parent)
{
}

RibbonToolBarOverflowMenu::~RibbonToolBarOverflowMenu()
{
}

void RibbonToolBarOverflowMenu::fillMenu()
{
    QAction* action = nullptr;
    RibbonToolBarControlPrivate* priv = RibbonToolBarControlPrivate::_get(m_parent);
    RibbonToolBarControlPrivate::RibbonToolBarActionGroupList::iterator it = priv->m_groups.begin();
    for (; it != priv->m_groups.end(); ++it)
    {
        if (it->row != -1)
            continue;
        RibbonToolBarControlPrivate::RibbonToolBarActionGroupItems::iterator jt = it->items.begin();
        for (; jt != it->items.end(); ++jt)
        {
            action = jt->action;
            if (action != nullptr && action->isVisible())
            {
                if (action->isSeparator())
                    addAction(action);
                else if (qobject_cast<QToolButton*>(jt->widget))
                    addAction(action);
                else if (jt->widget != nullptr)
                {
                    jt->rect = jt->widget->geometry();
                    action = new RibbonOverflowAction(jt->widget, this);
                    addAction(action);
                }
            }
        }
    }
}

void RibbonToolBarOverflowMenu::clearMenu()
{
    clear();
    RibbonToolBarControlPrivate* priv = RibbonToolBarControlPrivate::_get(m_parent);
    RibbonToolBarControlPrivate::RibbonToolBarActionGroupList::const_iterator it = priv->m_groups.constBegin();
    for (; it != priv->m_groups.constEnd(); ++it)
    {
        RibbonToolBarControlPrivate::RibbonToolBarActionGroupItems::const_iterator jt = it->items.constBegin();
        for (; jt != it->items.constEnd(); ++jt)
        {
            if (jt->action != nullptr && jt->widget != nullptr && jt->widget->parent() == nullptr)
            {
                jt->widget->setParent(m_parent);
                jt->widget->setGeometry(jt->rect);
                jt->widget->setVisible(true);
            }
        }
    }
}

bool RibbonToolBarOverflowMenu::event(QEvent* event)
{
    if (event->type() == QEvent::Show)
        fillMenu();
    else if (event->type() == QEvent::Hide)
        clearMenu();
    return PopupMenu::event(event);
}
/* RibbonColumnBreakControlPrivate */
RibbonToolBarControlPrivate::RibbonToolBarControlPrivate()
    : m_overflowMenu(nullptr)
    , m_margin(0)
    , m_rowsCount(2)
    , m_dirty(true)
    , m_calcRows(-1)
    , m_currColumnIndex(0)
    , m_fullWidth(0)
    , m_separatorWidth(0)
{
}

RibbonToolBarControlPrivate::~RibbonToolBarControlPrivate()
{
}

void RibbonToolBarControlPrivate::init()
{
    A_P(RibbonToolBarControl);
    m_overflowMenu = new RibbonToolBarOverflowMenu(&p);
}

void RibbonToolBarControlPrivate::recalcWidths(int height)
{
    A_P(RibbonToolBarControl);
    ARibbonGroup* parentGroup = p.parentGroup();
    if (parentGroup == nullptr || parentGroup->isHidden())
        return;
    ARibbonBar* ribbonBar = parentGroup->ribbonBar();
    if (ribbonBar == nullptr)
        return;

    m_vecWidths.clear();
    m_mapNonOptWidths.clear();
    RibbonGroupStyleOption opt;
    RibbonGroupPrivate::_get(parentGroup)->initStyleOption(opt);
    m_separatorWidth = parentGroup->style()->pixelMetric(QStyle::PM_ToolBarSeparatorExtent, &opt, &p);
    
    int rowHeight = ribbonBar->rowItemHeight();
    int maxWidth = 0;
    const int minWidth = 0;
    m_currColumnIndex = 0;
    const int maxRows = 3;
    int lastWidth = -1;
    int dx = 16;

    //Recalculation default sizes for groups.
    for (RibbonToolBarActionGroupList::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
    {
        if (maxWidth > 0)
            maxWidth += (m_separatorWidth + p.margin());
        RibbonToolBarActionsGroup& actionGroup = *it;
        QSize groupSize = calcGroupSizeHint(actionGroup.items);
        groupSize.setHeight(groupSize.height() < rowHeight ? rowHeight : groupSize.height());
        actionGroup.defaultSize = groupSize;
        maxWidth += groupSize.width();
    }

    if (p.currentSize() == ARibbonControlSizeDefinition::GroupSimplified)
    {
        QRect rect(0, 0, maxWidth + dx, height);
        do
        {
            updateGroupsGeometry(rect, false);
            if (lastWidth == -1 || m_fullWidth < lastWidth)
            {
                lastWidth = m_fullWidth;
                if (lastWidth <= minWidth || (lastWidth <= minWidth && m_groups.size() == 1))
                {
                    if (m_vecWidths.size() == 0)
                    {
                        m_vecWidths.append(lastWidth);
                        m_mapNonOptWidths.insert(m_fullWidth + p.margin(), rect.width());
                        return;
                    }
                    break;
                }
                m_vecWidths.append(lastWidth);
                m_mapNonOptWidths.insert(m_fullWidth + p.margin(), rect.width());
                rect.setRight(lastWidth - dx);
            }
            else
                rect.setRight(rect.right() - dx);
        }
        while (rect.width() > 0);
    }
    else
    {
        for (int rows = 1; rows <= maxRows; rows++)
        {
            QRect rect(QPoint(0, 0), QSize(minWidth, height));

            for (;; rect.setRight(rect.right() + dx))
            {
                if (rect.width() >= maxWidth + dx)
                    break;

                updateGroupsGeometry(rect, false);

                if (lastWidth != -1 && m_fullWidth > lastWidth)
                    break;

                if (m_calcRows == rows && m_fullWidth > 0)
                {
                    if (m_calcRows == maxRows - 1)
                        m_vecWidths.clear();

                    m_vecWidths.append(m_fullWidth);
                    m_mapNonOptWidths.insert(m_fullWidth  + p.margin(), rect.width());
                    lastWidth = m_fullWidth;
                    break;
                }
            }
        }
    }

    if (m_vecWidths.count() == 0)
        m_vecWidths.append(minWidth);
    if (m_vecWidths.count() == 1)
        m_vecWidths.append(m_vecWidths[0]);

    m_vecWidths.append(minWidth);
}

void RibbonToolBarControlPrivate::showDefaultButton()
{
    A_P(RibbonToolBarControl);
    ARibbonGroup* parentGroup = p.parentGroup();
    if (parentGroup == nullptr || parentGroup->isHidden())
        return;
#if 0 
    RibbonGroupStyleOption opt;
    RibbonGroupPrivate::_get(parentGroup)->initStyleOption(opt);
    const int widthDefaultButton = p.parentGroup()->style()->pixelMetric((QStyle::PixelMetric)CommonStyle::PM_RibbonGroupReducedWidth, &opt, parentGroup);
    currentRect.setRight(m_rect.left() + widthDefaultButton);
#endif
    m_calcRows = 0;
}

void RibbonToolBarControlPrivate::addActionToGroup(QAction* action, QWidget* widget)
{
    bool endgroup = m_groups.size() == 0 || (m_groups.last().items.size() > 0 && m_groups.last().items.last().action->isSeparator());
    if (endgroup)
        m_groups.append(RibbonToolBarActionsGroup());
    RibbonToolBarActionGroupItems& lastGroup = m_groups.last().items;
    RibbonToolBarActionGroupItem item(action, widget);
    lastGroup.append(item);
}

static int indexOfAction(const RibbonToolBarControlPrivate::RibbonToolBarActionGroupItems& group, QAction* action)
{
    int index = 0;
    for (RibbonToolBarControlPrivate::RibbonToolBarActionGroupItems::const_iterator it = group.constBegin(); it != group.constEnd(); ++it)
    {
        if (it->action == action)
            return index;
        index++;
    }
    return -1;
}

void RibbonToolBarControlPrivate::removeActionFromGroup(QAction* action)
{
    for (RibbonToolBarActionGroupList::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
    {
        RibbonToolBarActionsGroup group = *it;
        RibbonToolBarActionGroupItems& items = group.items;
        int index = indexOfAction(items, action);
        if (index != -1)
        {
            delete items.at(index).widget;
            items.removeAt(index);
            if (items.size() == 0)
            {
                m_groups.erase(it);
            }
            else if (action->isSeparator() && (it + 1) != m_groups.end())
            {
                RibbonToolBarActionsGroup group2 = *(it + 1);
                items += group2.items;//*(it + 1);
//                updateGroupProperties(items);
                m_groups.erase(it + 1);
            }
            return;
        }
    }
}

#define MAX_ROW 1000

void RibbonToolBarControlPrivate::updateGroupsGeometry(const QRect& rect, bool updateWidgets)
{
    A_P(RibbonToolBarControl);
    ARibbonGroup* parentGroup = p.parentGroup();
    if (parentGroup == nullptr || parentGroup->isHidden())
        return;

    ARibbonBar* ribbonBar = parentGroup->ribbonBar();
    if (ribbonBar == nullptr)
        return;

    QRect currentRect = rect;
    m_fullWidth = 0;
    m_calcRows = 0;

    QFontMetrics fm(parentGroup->titleFont());
    const QSize sizeCaption = fm.boundingRect(parentGroup->title()).size();

    bool m_trancateCaption = false;
    if (!m_trancateCaption)
        currentRect.setRight(currentRect.left() + qMax(rect.width(), sizeCaption.width()));

    QSize size = rect.size();

    int rowHeight = ribbonBar->rowItemHeight();

    int x = 0, y = 0;
    int startX = x;
    bool isFullHeight = currentRect.bottom() == 32767;
    int maxWidth = size.width();

    if (updateWidgets && m_mapNonOptWidths.size() > 0 && m_mapNonOptWidths.contains(maxWidth))
        maxWidth = m_mapNonOptWidths.value(maxWidth); //TODO Remove me

    for (RibbonToolBarActionGroupList::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
    {
        RibbonToolBarActionsGroup& actionGroup = *it;
        actionGroup.row = -1;
        QSize groupSize = actionGroup.defaultSize;
        const bool isLargeButton = groupSize.height() > rowHeight;
        if (isLargeButton)
        {
            QRect groupRect(QPoint(currentRect.left() + x, currentRect.top() /*+ m_margin / 2*/),
                QSize(groupSize.width(), isFullHeight ? groupSize.height() : size.height()));
            actionGroup.rect = groupRect;
            actionGroup.row = MAX_ROW;
            x += groupSize.width();
            m_fullWidth = qMax(m_fullWidth, x);
            x += (p.margin() + m_separatorWidth);
            startX = x;
        }
    }

    int bottom = 0;
    QVector<int> vecRowWidths;
    for (RibbonToolBarActionGroupList::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
    {
        RibbonToolBarActionsGroup& actionGroup = *it;
        if (actionGroup.row != -1)
            continue;
        QSize groupSize = actionGroup.defaultSize;
        groupSize.setHeight(groupSize.height() < rowHeight ? rowHeight : groupSize.height());

        if (x + groupSize.width() > maxWidth)
        {
            if (x == startX)
            {
                showDefaultButton();
                return;
            }

            y += rowHeight;
            vecRowWidths.append(x);

            m_calcRows++;
            x = startX;
            rowHeight = 0;
        }

        if (y + groupSize.height() > size.height())
        {
            showDefaultButton();
            return;
        }

        if (x > startX)
            x += (p.margin() + m_separatorWidth);

        QRect groupRect(QPoint(currentRect.left() + x /*+ m_margin*/, currentRect.top() + y /*+ m_margin / 2*/), groupSize);

        actionGroup.rect = groupRect;
        actionGroup.row = m_calcRows;

        rowHeight = qMax(rowHeight, groupSize.height());

        x += groupSize.width();
        m_fullWidth = qMax(m_fullWidth, x);

        bottom = qMax(bottom, groupRect.bottom());
    }

    vecRowWidths.append(x);
    m_calcRows++;

    if (isFullHeight)
    {
        //TODO: remove me
        currentRect.setBottom(bottom + sizeCaption.height() + p.margin() / 2);
        size.setHeight(currentRect.height() - sizeCaption.height() - p.margin() / 2);
    }

    int i = 0;
    if (m_calcRows > 1)
    {
        bool recalcFullWidth = false;
        while (true)
        {
            int maxRowWidth = 0, maxRowIndex = -1;
            for (i = 0; i < vecRowWidths.size(); i++)
            {
                if (vecRowWidths[i] > maxRowWidth)
                {
                    maxRowWidth = vecRowWidths[i];
                    maxRowIndex = i;
                }
            }

            if (maxRowIndex < 0)
                break;

            int minWidth = 9999;
            RibbonToolBarActionsGroup* minElem = nullptr;

            for (RibbonToolBarActionGroupList::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
            {
                RibbonToolBarActionsGroup& actionGroup = *it;
                if (actionGroup.row == maxRowIndex)
                {
                    QRect groupRect = actionGroup.rect;
                    if (!groupRect.isEmpty() && groupRect.width() < minWidth)
                    {
                        minWidth = groupRect.width();
                        minElem = &actionGroup;
                    }
                }
            }

            if (minElem == nullptr)
                break;

            bool moved = false;
            for (i = maxRowIndex + 1; i < vecRowWidths.size(); i++)
            {
                if (vecRowWidths[i] + minWidth < maxRowWidth)
                {
                    int x = 0, y = 0;
                    for (RibbonToolBarActionGroupList::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
                    {
                        RibbonToolBarActionsGroup& actionGroup = *it;

                        if (actionGroup.row == i)
                        {
                            x = qMax(actionGroup.rect.right() + (p.margin() + m_separatorWidth), x);
                            y = actionGroup.rect.top();
                        }
                        else if (actionGroup.row == maxRowIndex)
                        {
                            QRect groupRect = actionGroup.rect;

                            if (groupRect.left() > minElem->rect.left())
                            {
                                int dx = -(minWidth + (p.margin() + m_separatorWidth));
                                groupRect.adjust(dx, 0, dx, 0);
                                actionGroup.rect = groupRect;
                            }
                        }
                    }

                    minElem->rect = QRect(QPoint(x, y), minElem->rect.size());
                    minElem->row = i;

                    vecRowWidths[i] += minWidth;
                    vecRowWidths[maxRowIndex] -= minWidth;

                    recalcFullWidth = true;
                    moved = true;
                    break;
                }
            }
            if (!moved)
                break;
        }

        if (recalcFullWidth)
        {
            m_fullWidth = 0;
            for (RibbonToolBarActionGroupList::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
                m_fullWidth = qMax(m_fullWidth, (*it).rect.right() + 1);
            //m_fullWidth -= currentRect.left() /*+ m_margin*/;
        }
    }

    if (!isFullHeight && m_calcRows > 1)
    {
        int yOffset = (size.height() - m_calcRows * rowHeight) / (m_calcRows + 1);
        if (yOffset > 0)
        {
            for (RibbonToolBarActionGroupList::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
            {
                RibbonToolBarActionsGroup& actionGroup = *it;

                int row = actionGroup.row;
                QRect groupRect = actionGroup.rect;

                if (row != MAX_ROW && !groupRect.isEmpty())
                {
                    int dy = (yOffset * (row + 1) - row);
                    groupRect.adjust(0, dy - 1, 0, dy);
                    actionGroup.rect = groupRect;
                }
            }
        }
    }

    if (updateWidgets)
        updateGeometryWidget();
}

void RibbonToolBarControlPrivate::updateGeometryWidget()
{
    A_P(RibbonToolBarControl);
    ARibbonGroup* parentGroup = p.parentGroup();
    if (parentGroup == nullptr || parentGroup->isHidden())
        return;
    ARibbonBar* ribbonBar = parentGroup->ribbonBar();
    if (ribbonBar == nullptr)
        return;

//    QRect groupRect = parentGroup->rect();

    int rowHeight = ribbonBar->rowItemHeight();
    QList<QWidget*> showWidgets, hideWidgets;

    int currentRow = 0;
    QWidget* lastWidget = nullptr;
    RibbonToolBarSeparator* currentSeparator = nullptr;
    for (RibbonToolBarActionGroupList::iterator it = m_groups.begin(); it != m_groups.end(); ++it)
    {
        RibbonToolBarActionsGroup& actionGroup = *it;
        if (currentRow != actionGroup.row && actionGroup.row != MAX_ROW)
        {
            currentRow = actionGroup.row;
            if (currentSeparator)
                hideWidgets << currentSeparator;
        }

        QRect rect = actionGroup.rect;
        int x = rect.left();
        QList<RibbonToolBarActionGroupItem> items = actionGroup.items;
        for (QList<RibbonToolBarActionGroupItem>::iterator itItem = items.begin(); itItem != items.end(); ++itItem)
        {
            RibbonToolBarActionGroupItem& item = *itItem;
            if (item.widget != nullptr)
            {
                QSize sizeElem = RibbonControl::getBoundedSizeHint(item.widget);
                if (lastWidget)
                {
                    if (RibbonToolBarSeparator* separatorControl = qobject_cast<RibbonToolBarSeparator*>(item.widget))
                    {
                        QSize sz = lastWidget->sizeHint();
                        sizeElem.setHeight(sz.height());
                        currentSeparator = separatorControl;
                        showWidgets << separatorControl;
                    }
                    else
                        sizeElem.setHeight(sizeElem.height() < rowHeight ? rowHeight : sizeElem.height());
                }
                else
                    sizeElem.setHeight(sizeElem.height() < rowHeight ? rowHeight : sizeElem.height());

                if (item.widget != nullptr)
                    lastWidget = item.widget;

                if (item.isEmpty())
                    hideWidgets << item.widget;
                else
                {
                    item.widget->setGeometry(QRect(QPoint(x, rect.top()), sizeElem));
                    showWidgets << item.widget;
                    x += sizeElem.width();
                }
            }
        }
    }

    if (currentSeparator != nullptr && currentSeparator == lastWidget)
        hideWidgets << currentSeparator;

    // we have to do the show/hide here, because it triggers more calls to setGeometry
    for (int i = 0, count = showWidgets.count(); i < count; ++i)
        showWidgets.at(i)->show();

    for (int i = 0, count = hideWidgets.count(); i < count; ++i)
        hideWidgets.at(i)->hide();
}

void RibbonToolBarControlPrivate::updateGroupProperties(RibbonToolBarActionGroupItems& group)
{
    QWidget* firstWidget = nullptr;
    QWidget* lastWidget = nullptr;
    for (RibbonToolBarActionGroupItems::iterator it = group.begin(); it != group.end(); ++it)
    {
        if (it->isEmpty() || it->widget == nullptr || it->action->isSeparator())
            continue;

        if (firstWidget == nullptr)
            firstWidget = it->widget;

        lastWidget = it->widget;
        lastWidget->setProperty(_aproch_Begin_Group, firstWidget == it->widget);
        lastWidget->setProperty(_aproch_Group, true);
        lastWidget->setProperty(_aproch_End_Group, false);
    }

    if (lastWidget != nullptr)
        lastWidget->setProperty(_aproch_End_Group, true);
}

QSize RibbonToolBarControlPrivate::calcGroupSizeHint(const RibbonToolBarActionGroupItems& group) const
{
    A_P(const RibbonToolBarControl);
    ARibbonGroup* parentGroup = p.parentGroup();
    if (parentGroup == nullptr || parentGroup->isHidden())
        return QSize();
    ARibbonBar* ribbonBar = parentGroup->ribbonBar();
    Q_ASSERT(ribbonBar != nullptr);

    QSize ret = QSize(0, 0);
    for (RibbonToolBarControlPrivate::RibbonToolBarActionGroupItems::const_iterator it = group.constBegin(); it != group.constEnd(); ++it)
    {
        if (it->action->isSeparator() && it->action->isVisible())
            continue;

        Q_ASSERT(it->widget != nullptr);
        if (it->widget == nullptr)
            continue;

        if (!it->isEmpty())
        {
            QSize s = RibbonControl::getBoundedSizeHint(it->widget);
            s.setHeight(s.height() < ribbonBar->rowItemHeight() ? ribbonBar->rowItemHeight() : s.height());
            ret.rwidth() += s.width();
            ret.rheight() = qMax(ret.height(), s.height());
        }
    }
    return ret;
}

void RibbonToolBarControlPrivate::updateGroupItems()
{
    A_P(RibbonToolBarControl);
    ARibbonGroup* parentGroup = p.parentGroup();
    if (parentGroup == nullptr)
        return;
    ARibbonBar* ribbonBar = parentGroup->ribbonBar();
    if (ribbonBar == nullptr)
        return;

    recalcWidths(ribbonBar->rowItemHeight() * ribbonBar->rowItemCount());
    for (RibbonToolBarActionGroupList::iterator itGroup = m_groups.begin(); itGroup != m_groups.end(); ++itGroup)
        updateGroupProperties((*itGroup).items);
}

void RibbonToolBarControlPrivate::updateFont()
{
    for (RibbonToolBarActionGroupList::const_iterator it = m_groups.begin(); it != m_groups.end(); ++it)
    {
        const RibbonToolBarActionsGroup& actionGroup = *it;
        for (RibbonToolBarControlPrivate::RibbonToolBarActionGroupItems::const_iterator jt = actionGroup.items.constBegin(); 
            jt != actionGroup.items.constEnd(); ++jt)
        {
            const RibbonToolBarActionGroupItem& item = *jt;
            item.widget->setFont(QFont());
        }
    }
}

/*!
\class RibbonToolBarControl
\inmodule QtitanRibbon
*/
RibbonToolBarControl::RibbonToolBarControl(ARibbonGroup* parentGroup)
    : RibbonControl(parentGroup)
{
    A_INIT_PRIVATE(RibbonToolBarControl);
    A_D(RibbonToolBarControl);
    d.init();
}

RibbonToolBarControl::~RibbonToolBarControl()
{
    A_FINI_PRIVATE();
}

int RibbonToolBarControl::rowsCount()
{
    A_D(RibbonToolBarControl);
    return d.m_rowsCount;
}

void RibbonToolBarControl::setRowsCount(int count)
{
    A_D(RibbonToolBarControl);
    d.m_rowsCount = count;
}

void RibbonToolBarControl::setDirty()
{
    A_D(RibbonToolBarControl);
    d.m_dirty = true;
    for (RibbonToolBarControlPrivate::RibbonToolBarActionGroupList::iterator it = d.m_groups.begin(); it != d.m_groups.end(); ++it)
        for (RibbonToolBarControlPrivate::RibbonToolBarActionGroupItems::iterator jt = it->items.begin(); jt != it->items.end(); ++jt)
            if (RibbonButton* button = qobject_cast<RibbonButton*>(jt->widget))
                button->changed();
}

void RibbonToolBarControl::clear()
{
    QList<QAction *> actions = this->actions();
    for (int i = 0; i < actions.size(); i++)
        removeAction(actions.at(i));
}

QAction* RibbonToolBarControl::addWidget(QWidget* widget)
{
    QWidgetAction* action = new QWidgetAction(this);
    action->setDefaultWidget(widget);
    RibbonControl::addAction(action);
    return action;
}

QAction* RibbonToolBarControl::addWidget(const QIcon& icon, const QString& text, QWidget* widget)
{
    QWidgetAction* action = new QWidgetAction(this);
    action->setIcon(icon);
    action->setText(text);
    action->setDefaultWidget(widget);
    RibbonControl::addAction(action);
    return action;
}

QAction* RibbonToolBarControl::addMenu(const QIcon& icon, const QString& text, QMenu* menu, QToolButton::ToolButtonPopupMode mode)
{
    Q_ASSERT(menu != nullptr);
    Q_ASSERT_X(mode != QToolButton::DelayedPopup, "RibbonToolBarControl::addMenu", "The use of mode is not valid");

    if (mode == QToolButton::DelayedPopup)
        return nullptr;

    QAction* action = menu->menuAction();
    action->setIcon(icon);
    menu->setTitle(text);

    RibbonControl::addAction(action);

    RibbonButton* button = qobject_cast<RibbonButton *>(widgetByAction(action));
    if (button != nullptr)
    {
        button->setPopupMode(mode);
        button->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
    return action;
}

QAction* RibbonToolBarControl::addAction(QAction* action, Qt::ToolButtonStyle style, QMenu* menu, QToolButton::ToolButtonPopupMode mode)
{
    addAction(action);
    RibbonButton* button = qobject_cast<RibbonButton *>(widgetByAction(action));
    Q_ASSERT(button != nullptr);
    button->setToolButtonStyle(style);
    if (menu || action->menu())
        button->setPopupMode(mode);
    if (menu)
        button->setMenu(menu);
    return action;
}

QAction* RibbonToolBarControl::insertAction(QAction* before, QAction* action, Qt::ToolButtonStyle style, QMenu* menu, QToolButton::ToolButtonPopupMode mode)
{
    insertAction(before, action);
    RibbonButton* button = qobject_cast<RibbonButton *>(widgetByAction(action));
    Q_ASSERT(button != nullptr);
    button->setToolButtonStyle(style);
    if (menu || action->menu())
        button->setPopupMode(mode);
    if (menu)
        button->setMenu(menu);
    return action;
}

QAction* RibbonToolBarControl::addAction(const QString& text)
{
    QAction *action = new QAction(text, this);
    RibbonControl::addAction(action);
    return action;
}

QAction* RibbonToolBarControl::addAction(const QIcon& icon, const QString& text)
{
    QAction *action = new QAction(icon, text, this);
    RibbonControl::addAction(action);
    return action;
}

QAction* RibbonToolBarControl::addAction(const QString& text, const QObject* receiver, const char* member)
{
    QAction *action = new QAction(text, this);
    QObject::connect(action, SIGNAL(triggered(bool)), receiver, member);
    RibbonControl::addAction(action);
    return action;
}

QAction* RibbonToolBarControl::addAction(const QIcon& icon, const QString& text, const QObject* receiver, const char* member)
{
    QAction* action = new QAction(icon, text, this);
    QObject::connect(action, SIGNAL(triggered(bool)), receiver, member);
    RibbonControl::addAction(action);
    return action;
}

QAction* RibbonToolBarControl::addAction(const QIcon& icon, const QString& text, Qt::ToolButtonStyle style, QMenu* menu, QToolButton::ToolButtonPopupMode mode)
{
    QAction* action = nullptr;
    if (mode == QToolButton::InstantPopup && menu)
    {
        action = menu->menuAction();
        action->setIcon(icon);
        menu->setTitle(text);
    }
    else
        action = new QAction(icon, text, this);

    RibbonControl::addAction(action);

    if (RibbonButton* button = qobject_cast<RibbonButton *>(widgetByAction(action)))
    {
        button->setToolButtonStyle(style);
        button->setLargeIcon(style == Qt::ToolButtonTextUnderIcon);
    }
    return action;
}

QAction* RibbonToolBarControl::addSeparator()
{
    QAction* action = new QAction(this);
    action->setSeparator(true);
    RibbonControl::addAction(action);
    return action;
}

QMenu* RibbonToolBarControl::overflowMenu()
{
    A_D(RibbonToolBarControl);
    return d.m_overflowMenu;
}

bool RibbonToolBarControl::isOverflowMenuIsEmpty() const
{
    A_D(const RibbonToolBarControl);
    RibbonToolBarControlPrivate::RibbonToolBarActionGroupList::const_iterator it = d.m_groups.constBegin();
    for (; it != d.m_groups.constEnd(); ++it)
    {
        if (it->row == -1)
            return false;
    }
    return true;
}

QWidget* RibbonToolBarControl::widgetByAction(QAction* action)
{
    A_D(RibbonToolBarControl);
    for (RibbonToolBarControlPrivate::RibbonToolBarActionGroupList::iterator it = d.m_groups.begin(); it != d.m_groups.end(); ++it)
    {
        RibbonToolBarControlPrivate::RibbonToolBarActionsGroup& group = *it;
        RibbonToolBarControlPrivate::RibbonToolBarActionGroupItems& items = group.items;
        int index = indexOfAction(items, action);
        if (index != -1)
            return items[index].widget;
    }
    return nullptr;
}

void RibbonToolBarControl::sizeChanged(ARibbonControlSizeDefinition::GroupSize size)
{
    RibbonControl::sizeChanged(size);
    A_D(RibbonToolBarControl);
    if (d.m_vecWidths.isEmpty())
        return;

    switch (size)
    {
        case ARibbonControlSizeDefinition::GroupLarge:
        case ARibbonControlSizeDefinition::GroupMedium:
            setRowsCount(2);
            break;
        case ARibbonControlSizeDefinition::GroupPopup:
            setRowsCount(2);
            break;
        case ARibbonControlSizeDefinition::GroupSmall:
            setRowsCount(3);
            break;
        case ARibbonControlSizeDefinition::GroupSimplified:
            setRowsCount(1);
            break;
        default:
            break;
    }

    for (RibbonToolBarControlPrivate::RibbonToolBarActionGroupList::iterator it = d.m_groups.begin(); it != d.m_groups.end(); ++it)
        for (RibbonToolBarControlPrivate::RibbonToolBarActionGroupItems::iterator jt = it->items.begin(); jt != it->items.end(); ++jt)
            if (RibbonButton* button = qobject_cast<RibbonButton*>(jt->widget))
                button->setSimplifiedMode(size == ARibbonControlSizeDefinition::GroupSimplified || size == ARibbonControlSizeDefinition::GroupSimplifiedOverflow);
}

bool RibbonToolBarControl::adjustCurrentSize(bool expand)
{
    A_D(RibbonToolBarControl);
    if (currentSize() == ARibbonControlSizeDefinition::GroupSimplified)
    {
        int current = d.m_currColumnIndex;
        const int old = current;

        if (expand)
            current--;
        else
            current++;

        if (current >= 0 && current < d.m_vecWidths.count())
            d.m_currColumnIndex = current;

        if (old != d.m_currColumnIndex)
            return true;
    }
    return RibbonControl::adjustCurrentSize(expand);
}

void RibbonToolBarControl::updateLayout()
{
    A_D(RibbonToolBarControl);
    if (parentGroup() == nullptr)
        return;

    if (d.m_dirty)
    {
        d.m_dirty = false;
        d.updateGroupItems();
    }

    if (d.m_vecWidths.isEmpty())
        return;

    if (ARibbonBar* ribbonBar = parentGroup()->ribbonBar())
    {
        QSize sz;
        int currWidthIndex = rowsCount() == 2 ? 0 : 1;
        if (currentSize() == ARibbonControlSizeDefinition::GroupSimplified)
            currWidthIndex = d.m_currColumnIndex;

        const int currPanelWidth = d.m_vecWidths[currWidthIndex] /*+ 2 * margin()*/;
        sz.setWidth(currPanelWidth);
        const int vertMargin = CommonStylePrivate::dpiScaled(3, this);
        const int height = (ribbonBar->rowItemHeight() * ribbonBar->rowItemCount());// /*+ 2 * (margin() / 2)*/ +vertMargin;
        sz.setHeight(height);
        d.updateGroupsGeometry(QRect(QPoint(margin(), 0/*margin() / 2*/), sz), true);
        //d.updateGeometryWidget();
        d.m_sizeHint = sz;
        d.m_sizeHint.setWidth(sz.width() + margin() * 2);
    }
}

/*! \reimp */
QSize RibbonToolBarControl::sizeHint() const
{
    A_D(const RibbonToolBarControl);
    return d.m_sizeHint;
}

/*! \reimp */
void RibbonToolBarControl::paintEvent(QPaintEvent* event)
{
    RibbonControl::paintEvent(event);
}

/*! \reimp */
void RibbonToolBarControl::actionEvent(QActionEvent* event)
{
    A_D(RibbonToolBarControl);
    QAction* action = event->action();
    switch (event->type())
    {
        case QEvent::ActionAdded:
            {
                QWidget* widget = nullptr;
                if (!action->isSeparator())
                {
                    QWidgetAction* widgetAction = qobject_cast<QWidgetAction *>(action);
                    if (widgetAction)
                        widget = widgetAction->requestWidget(this);

                    if (widget == nullptr)
                    {
                        RibbonButton* button = new RibbonButton(this);
                        button->setAutoRaise(true);
                        button->setFocusPolicy(Qt::NoFocus);
                        button->setDefaultAction(action);
                        button->setToolButtonStyle(Qt::ToolButtonIconOnly);
                        widget = button;
                    }
                    widget->hide();
                }
                else
                {
                    RibbonToolBarSeparator* separator = new RibbonToolBarSeparator(this);
                    widget = separator;
                }

                d.addActionToGroup(action, widget);
                widget->setFont(QFont());
                setDirty();
                break;
            }
        case QEvent::ActionChanged:
            {
                setDirty();
            }
            break;
        case QEvent::ActionRemoved:
            {
                d.removeActionFromGroup(action);
                setDirty();
            }
            break;
        default:
            break;
    }
}

/*! \reimp */
void RibbonToolBarControl::changeEvent(QEvent* event)
{
    A_D(RibbonToolBarControl);
    RibbonControl::changeEvent(event);

    switch (event->type())
    {
        case QEvent::FontChange:
        case QEvent::StyleChange:
            {
                if (event->type() == QEvent::StyleChange)
                    d.updateFont();
                setDirty();
            }
            break;
        default:
            break;
    }
}

int RibbonToolBarControl::margin() const
{
    A_D(const RibbonToolBarControl);
    return d.m_margin;
}

void RibbonToolBarControl::setMargin(int margin)
{
    A_D(RibbonToolBarControl);
    d.m_margin = margin;
}
