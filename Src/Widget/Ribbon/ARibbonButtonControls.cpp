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
#include "ARibbonButtonControls.h"
#include "ARibbonGroup.h"
#include "ARibbonControlsPrivate.h"
#ifdef A_MEMORY_DEBUG
#include "QtitanMSVSDebug.h"
#endif

QTITAN_USE_NAMESPACE

static Qt::ToolButtonStyle defineToolButtonStyle = Qt::ToolButtonTextUnderIcon;

RibbonButtonControlPrivate::RibbonButtonControlPrivate()
    : m_toolButton(nullptr)
{
}

RibbonButtonControlPrivate::~RibbonButtonControlPrivate()
{
}

void RibbonButtonControlPrivate::init()
{
    A_P(RibbonButtonControl);
    p.sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setStretchable(false);
    p.sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setStretchable(false);
    p.sizeDefinition(ARibbonControlSizeDefinition::GroupSmall)->setStretchable(false);
    p.sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setStretchable(false);

    m_toolButton = new RibbonButton(&p);
    p.setContentWidget(m_toolButton);
}

/*!
\class RibbonButtonControl
\inmodule QtitanRibbon
*/
RibbonButtonControl::RibbonButtonControl(ARibbonGroup* parentGroup)
    : RibbonWidgetControl(parentGroup, true)
{
    A_INIT_PRIVATE(RibbonButtonControl);
    A_D(RibbonButtonControl);
    d.init();
}

RibbonButtonControl::~RibbonButtonControl()
{
    A_FINI_PRIVATE();
}

const QIcon& RibbonButtonControl::largeIcon() const
{
    A_D(const RibbonButtonControl);
    return d.m_largeIcon;
}

void RibbonButtonControl::setLargeIcon(const QIcon& icon)
{
    A_D(RibbonButtonControl);
    d.m_largeIcon = icon;
}

const QIcon& RibbonButtonControl::smallIcon() const
{
    A_D(const RibbonButtonControl);
    return d.m_smallIcon;
}

void RibbonButtonControl::setSmallIcon(const QIcon& icon)
{
    A_D(RibbonButtonControl);
    d.m_smallIcon = icon;
}

const QString& RibbonButtonControl::label() const
{
    A_D(const RibbonButtonControl);
    return d.m_label;
}

void RibbonButtonControl::setLabel(const QString& label)
{
    A_D(RibbonButtonControl);
    d.m_label = label;
}

void RibbonButtonControl::setMenu(QMenu* menu)
{
    A_D(RibbonButtonControl);

     if (menu && d.m_toolButton->actions().contains(menu->menuAction()))
        d.m_toolButton->removeAction(menu->menuAction());

    d.m_toolButton->setMenu(menu);
}

QMenu* RibbonButtonControl::menu() const
{
    A_D(const RibbonButtonControl);
    return d.m_toolButton->menu();
}

void RibbonButtonControl::setPopupMode(QToolButton::ToolButtonPopupMode mode)
{
    A_D(RibbonButtonControl);
    d.m_toolButton->setPopupMode(mode);
}

QToolButton::ToolButtonPopupMode RibbonButtonControl::popupMode() const
{
    A_D(const RibbonButtonControl);
    return d.m_toolButton->popupMode();
}

void RibbonButtonControl::setToolButtonStyle(Qt::ToolButtonStyle style)
{
    A_D(RibbonButtonControl);
    if (style == Qt::ToolButtonFollowStyle)
        style = defineToolButtonStyle;

    d.m_toolButton->setToolButtonStyle(style);
    if (style == Qt::ToolButtonIconOnly)
    {
        sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
        sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setLabelVisible(false);
        sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
        sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setLabelVisible(false);
        sizeDefinition(ARibbonControlSizeDefinition::GroupSmall)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
        sizeDefinition(ARibbonControlSizeDefinition::GroupSmall)->setLabelVisible(false);
        sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
        sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setLabelVisible(false);
    }
    else if (style == Qt::ToolButtonTextOnly)
    {
        sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setImageSize(ARibbonControlSizeDefinition::ImageNone);
        sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setLabelVisible(true);
        sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setImageSize(ARibbonControlSizeDefinition::ImageNone);
        sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setLabelVisible(true);
        sizeDefinition(ARibbonControlSizeDefinition::GroupSmall)->setImageSize(ARibbonControlSizeDefinition::ImageNone);
        sizeDefinition(ARibbonControlSizeDefinition::GroupSmall)->setLabelVisible(true);
        sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setImageSize(ARibbonControlSizeDefinition::ImageNone);
        sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setLabelVisible(true);
    }
    else if (style == Qt::ToolButtonTextBesideIcon)
    {
        sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
        sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
        sizeDefinition(ARibbonControlSizeDefinition::GroupSmall)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
        sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
    }
    else if (style == Qt::ToolButtonTextUnderIcon)
    {
        sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setImageSize(ARibbonControlSizeDefinition::ImageLarge);
        sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setLabelVisible(true);
        sizeDefinition(ARibbonControlSizeDefinition::GroupSmall)->setLabelVisible(true);
        sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setLabelVisible(true);
    }
}

Qt::ToolButtonStyle RibbonButtonControl::toolButtonStyle() const
{
    A_D(const RibbonButtonControl);
    return d.m_toolButton->toolButtonStyle();
}

QToolButton* RibbonButtonControl::widget() const
{
    A_D(const RibbonButtonControl);
    return d.m_toolButton;
}

QSize RibbonButtonControl::sizeHint() const
{
    A_D(const RibbonButtonControl);
    QSize sz = RibbonWidgetControl::sizeHint();

    // 菜单类型预留箭头宽度
	if (d.m_toolButton && menu())
	{
        Qt::ToolButtonStyle style = toolButtonStyle();
        if (style == Qt::ToolButtonTextUnderIcon)
        {
		    sz.setWidth(sz.width() + 12);
        }
	}

    if (d.m_toolButton && parentGroup() != nullptr)
    {
        if (ARibbonBar* ribbonBar = parentGroup()->ribbonBar())
        {
            Qt::ToolButtonStyle style = toolButtonStyle();
            if (d.m_toolButton->isLargeIcon() && (style == Qt::ToolButtonIconOnly || style == Qt::ToolButtonTextUnderIcon))
                sz.setHeight(ribbonBar->rowItemHeight() * ribbonBar->rowItemCount());
            else if (style == Qt::ToolButtonTextBesideIcon || style == Qt::ToolButtonIconOnly || style == Qt::ToolButtonTextOnly)
                sz.setHeight(ribbonBar->rowItemHeight());
        }
    }

    return sz;
}

void RibbonButtonControl::sizeChanged(ARibbonControlSizeDefinition::GroupSize size)
{
    A_D(RibbonButtonControl);
    RibbonControl::sizeChanged(size);

    d.m_toolButton->setDefaultAction(defaultAction());

    if (sizeDefinition(size)->imageSize() == ARibbonControlSizeDefinition::ImageLarge)
    {
        d.m_toolButton->setLargeIcon(true);
        d.m_toolButton->setIcon(largeIcon());
        if (sizeDefinition(size)->isLabelVisible())
        {
            d.m_toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            d.m_toolButton->setText(label());
        }
        else
            d.m_toolButton->setToolButtonStyle(Qt::ToolButtonIconOnly);

        d.m_toolButton->setWordWrap(sizeDefinition(size)->wordWrap());
    }
    else if (sizeDefinition(size)->imageSize() == ARibbonControlSizeDefinition::ImageSmall)
    {
        d.m_toolButton->setLargeIcon(false);
        d.m_toolButton->setIcon(smallIcon());
        if (sizeDefinition(size)->isLabelVisible())
        {
            d.m_toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            d.m_toolButton->setText(label());
        }
        else
            d.m_toolButton->setToolButtonStyle(Qt::ToolButtonIconOnly);

        d.m_toolButton->setWordWrap(false);
    }
    else if (sizeDefinition(size)->imageSize() == ARibbonControlSizeDefinition::ImageNone)
    {
        d.m_toolButton->setLargeIcon(false);
        d.m_toolButton->setIcon(QIcon());
        d.m_toolButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
        d.m_toolButton->setText(label());
        d.m_toolButton->setWordWrap(false);
    }
}

void RibbonButtonControl::actionChanged()
{
    if (defaultAction() != nullptr)
    {
        if (!defaultAction()->iconText().isEmpty())
            setLabel(defaultAction()->iconText());
        else 
            setLabel(defaultAction()->text());

        setLargeIcon(defaultAction()->icon());
        setSmallIcon(defaultAction()->icon());
    }

    RibbonControl::actionChanged();
}

/*! \reimp */
bool RibbonButtonControl::event(QEvent* event)
{
    bool result = RibbonWidgetControl::event(event);

    QEvent::Type type = event->type();
    if (type == QEvent::FontChange || type == QEvent::StyleChange)
    {
        sizeChanged(currentSize());
    }
    return result;
}


/* RibbonCheckBoxControl */
RibbonCheckBoxControl::RibbonCheckBoxControl(const QString& text, ARibbonGroup* parentGroup)
    : RibbonWidgetControl(parentGroup, true/*ignoreActionSettings*/)
{
    QCheckBox* checkBox = new QCheckBox(text, this);
    setContentWidget(checkBox);
}

RibbonCheckBoxControl::~RibbonCheckBoxControl()
{
}

QCheckBox* RibbonCheckBoxControl::widget() const
{
    return qobject_cast<QCheckBox*>(contentWidget());
}

/* RibbonCheckBoxControl */
RibbonRadioButtonControl::RibbonRadioButtonControl(const QString& text, ARibbonGroup* parentGroup)
    : RibbonWidgetControl(parentGroup, true/*ignoreActionSettings*/)
{
    QRadioButton* radioButton = new QRadioButton(text, this);
    setContentWidget(radioButton);
}

RibbonRadioButtonControl::~RibbonRadioButtonControl()
{
}

QRadioButton* RibbonRadioButtonControl::widget() const
{
    return qobject_cast<QRadioButton*>(contentWidget());
}
