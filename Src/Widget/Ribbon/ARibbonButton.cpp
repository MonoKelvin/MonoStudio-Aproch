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

#include "QtnRibbonButtonPrivate.h"
#include "QtnRibbonBarPrivate.h"
#include "QtnCommonStyle.h"
#include "QtnCommonStylePrivate.h"
#include "QtnRibbonGroup.h"
#include "QtnStyleHelperPrivate.h"

#ifdef QTN_MEMORY_DEBUG
#include "QtitanMSVSDebug.h"
#endif

QTITAN_USE_NAMESPACE


RibbonButtonPrivate::RibbonButtonPrivate()
    : QObject()
    , m_wordWrap(true)
    , m_largeIcon(false)
    , m_simplifiedMode(false)
    , m_wrapIndex(-1)
    , m_saveButtonStyle(Qt::ToolButtonIconOnly)
    , m_simplifiedButtonStyle(Qt::ToolButtonIconOnly)
    , m_saveArrowType(Qt::NoArrow)
{
}

void RibbonButtonPrivate::resetWordWrap()
{
    QTN_P(RibbonButton);
    m_wordIndexes.clear();

    int start = 0;
    QString str = p.text();

    for (int offset = start;;)
    {
        int index = str.indexOf(QLatin1Char(' '), offset);
        if (index >= 0)
        {
            Q_ASSERT(index != 0);
            m_wordIndexes.append(index);
            offset = index + 1;
        }
        else
            break;
    }
}

QSize RibbonButtonPrivate::sizeWordWrap()
{
    QTN_P(RibbonButton);
    QStyleOptionToolButton opt;
    p.initStyleOption(&opt);

    QString strText = opt.text;
    QSize sizeText = opt.fontMetrics.size(Qt::TextShowMnemonic, strText);
    const int space = qtn_horizontalAdvanceFont(opt.fontMetrics, QLatin1Char(' ')) * 2;
    const int textLineHeight = sizeText.height();
    const bool indicatorCenter = m_wordIndexes.size() == 0 && opt.features & QStyleOptionToolButton::HasMenu;
    sizeText.setWidth(sizeText.width() + space);

    const int menuArrowWidth = !indicatorCenter ? p.style()->pixelMetric(QStyle::PM_MenuButtonIndicator, &opt, &p) + 2 : 0;

    m_wrapIndex = -1;
    int textWidth = 0;

    if (m_wordIndexes.size() == 0 || !m_wordWrap)
    {
        textWidth = sizeText.width() + space;
    }
    else
    {
        textWidth = 32767;
        for (int i = 0; i < m_wordIndexes.size(); i++)
        {
            int index = m_wordIndexes[i];
            QString strLineOne = strText.left(index);
            const int cxLineOne = opt.fontMetrics.size(Qt::TextShowMnemonic, strLineOne).width() + space;

            QString strLineTwo = strText.mid(index + 1);
            const int cxLineTwo = opt.fontMetrics.size(Qt::TextShowMnemonic, strLineTwo).width() + menuArrowWidth + space;
            int width = qMax(cxLineOne, cxLineTwo);

            if (width < textWidth)
            {
                textWidth = width;
                m_wrapIndex = index;
            }
        }
    }

    if (textWidth % 2)
        textWidth--;

    return QSize(textWidth, textLineHeight * 2);
}

/*!
\class RibbonButton
\inmodule QtitanRibbon
\brief RibbonButton class implements tool button that used in ribbon ui. Button appearance and behavior adapted for display inside the RibbonBar. 

*/
RibbonButton::RibbonButton(QWidget* parent)
    : QToolButton(parent)
{
    QTN_INIT_PRIVATE(RibbonButton);
    setAutoRaise(true);
}

RibbonButton::~RibbonButton()
{
    QTN_FINI_PRIVATE();
}

bool RibbonButton::wordWrap() const
{ 
    QTN_D(const RibbonButton);
    return d.m_wordWrap; 
}

void RibbonButton::setWordWrap(bool on)
{
    QTN_D(RibbonButton);
    if (d.m_wordWrap != on)
    {
        d.m_wordWrap = on;
        changed();
    }
}

bool RibbonButton::isLargeIcon() const
{ 
    QTN_D(const RibbonButton);
    return d.m_largeIcon; 
}

void RibbonButton::setLargeIcon(bool large)
{ 
    QTN_D(RibbonButton);
    d.m_largeIcon = large; 
}

bool RibbonButton::simplifiedMode() const
{
    QTN_D(const RibbonButton);
    return d.m_simplifiedMode;
}

void RibbonButton::setSimplifiedMode(bool enabled)
{
    QTN_D(RibbonButton);
    // this function is valid if the button is on RibbonToolBarControl
    if (d.m_simplifiedMode == enabled)
        return;
    d.m_simplifiedMode = enabled;
    changed();

    if (d.m_simplifiedMode)
    {
        if (toolButtonStyle() == Qt::ToolButtonTextUnderIcon)
        {
            d.m_simplifiedButtonStyle = toolButtonStyle();
            setToolButtonStyle(Qt::ToolButtonIconOnly);
            setLargeIcon(!(toolButtonStyle() == Qt::ToolButtonIconOnly));
        }
    }
    else if (d.m_simplifiedButtonStyle == Qt::ToolButtonTextUnderIcon && 
             toolButtonStyle() != Qt::ToolButtonTextUnderIcon)
    {
        setToolButtonStyle(d.m_simplifiedButtonStyle);
        setLargeIcon(toolButtonStyle() == Qt::ToolButtonTextUnderIcon);
    }
}

/*! \reimp */
QSize RibbonButton::sizeHint() const
{
    QTN_D(const RibbonButton);
    RibbonButtonPrivate& pd = const_cast<RibbonButtonPrivate&>(d);

    if (pd.m_saveButtonStyle != toolButtonStyle() || pd.m_saveArrowType != arrowType())
        const_cast<RibbonButton *>(this)->changed();

    if (pd.m_sizeHint.isValid())
        return pd.m_sizeHint;

    pd.m_saveButtonStyle = toolButtonStyle();
    pd.m_saveArrowType = arrowType();

    QSize sz = QToolButton::sizeHint();
    if (RibbonControl* control = qobject_cast<RibbonControl *>(parentWidget()))
    {
        if (RibbonBar* ribbonBar = control->parentGroup()->ribbonBar())
            sz.setHeight(ribbonBar->rowItemHeight());
    }

    int w = 0, h = 0;
    QStyleOptionToolButton opt;
    initStyleOption(&opt);

    QString textButton = text();
    if (!textButton.isEmpty() && (bool)style()->styleHint((QStyle::StyleHint)CommonStyle::SH_RibbonItemUpperCase, 0, this))
    {
        if (qobject_cast<RibbonTabBar*>(parentWidget()) != Q_NULL)
            textButton = textButton.toUpper();
    }

    if (opt.toolButtonStyle != Qt::ToolButtonTextOnly)
    {
        w = opt.iconSize.width();
        h = opt.iconSize.height();
    }

    bool allowIndicator = false;
    QToolButton::ToolButtonPopupMode mode = QToolButton::popupMode();
    if (opt.toolButtonStyle != Qt::ToolButtonIconOnly)
    {
        if (opt.toolButtonStyle == Qt::ToolButtonTextUnderIcon)
        {
            pd.resetWordWrap();
            pd.m_sizeWordWrap = pd.sizeWordWrap();

            h = sz.height();

            if (RibbonControl* control = qobject_cast<RibbonControl *>(parentWidget()))
            {
                if (RibbonBar* ribbonBar = control->parentGroup()->ribbonBar())
                    h = ribbonBar->rowItemHeight() * ribbonBar->rowItemCount();
            }
            // if the text is more than icon
            if (pd.m_sizeWordWrap.width() > w)
                w = pd.m_sizeWordWrap.width();
        }
        else
        {
            allowIndicator = true;
            QSize textSize = opt.fontMetrics.size(Qt::TextShowMnemonic, textButton);
            textSize.setWidth(textSize.width() + qtn_horizontalAdvanceFont(opt.fontMetrics, QLatin1Char(' ')) * 2);
            h = sz.height();

            if (opt.toolButtonStyle == Qt::ToolButtonTextBesideIcon)
            {
                w = h;
                w += textSize.width() + CommonStylePrivate::dpiScaled(2, this);

                if (textSize.height() > h)
                    h = textSize.height();
            }
            else
                w = textSize.width() + CommonStylePrivate::dpiScaled(4, this);
        }
    }
    else
    {
        allowIndicator = true;

        h = sz.height();
        QSize textSize = opt.fontMetrics.size(Qt::TextShowMnemonic, textButton.isEmpty() ? QStringLiteral("XXX") : textButton);
        if (textSize.height() > h)
            h = textSize.height();
        w = h;

        if ((opt.features & QStyleOptionToolButton::HasMenu) || (opt.subControls & QStyle::SC_ToolButtonMenu))
            w += CommonStylePrivate::dpiScaled(1, this);

        if (isLargeIcon() && opt.toolButtonStyle == Qt::ToolButtonIconOnly)
        {
            if (RibbonControl* control = qobject_cast<RibbonControl *>(parentWidget()))
            {
                if (RibbonBar* ribbonBar = control->parentGroup()->ribbonBar())
                {
                    h = ribbonBar->rowItemHeight() * ribbonBar->rowItemCount();
                    w = opt.iconSize.width() + CommonStylePrivate::dpiScaled(4, this);
                }
            }
        }
    }

    opt.rect.setSize(QSize(w, h)); // PM_MenuButtonIndicator depends on the height

    if (isLargeIcon() && opt.toolButtonStyle == Qt::ToolButtonIconOnly)
        allowIndicator = false;

    if ((mode == MenuButtonPopup || mode == QToolButton::InstantPopup) && allowIndicator)
        w += style()->pixelMetric(QStyle::PM_MenuButtonIndicator, &opt, this);

    pd.m_sizeHint = QSize(w, h);
    return pd.m_sizeHint;
}

void RibbonButton::changed()
{
    QTN_D(RibbonButton);
    d.m_sizeHint = QSize();
}

void RibbonButton::updateIconSize()
{
    const int sz = style()->pixelMetric(isLargeIcon() ? QStyle::PM_LargeIconSize : 
        QStyle::PM_SmallIconSize, 0, this);
    QSize size(sz, sz);
    if (iconSize() != size)
        setIconSize(size);
}

/*! \reimp */
bool RibbonButton::event(QEvent* event)
{
    QEvent::Type type = event->type();
    if (type == QEvent::LayoutRequest || type == QEvent::FontChange || type == QEvent::StyleChange)
        changed();
    updateIconSize();
    return QToolButton::event(event);
}

/*! \reimp */
void RibbonButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QTN_D(const RibbonButton);
    QPainter p(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);

    if (property("wordWrap").toBool())
    {
        QToolButton::ToolButtonPopupMode mode = QToolButton::popupMode();
        if (mode == MenuButtonPopup || mode == QToolButton::InstantPopup)
        {
            const bool indicatorCenter = d.m_wordIndexes.size() == 0 && opt.features & QStyleOptionToolButton::HasMenu;
            const int menuArrowWidth = !indicatorCenter ? style()->pixelMetric(QStyle::PM_MenuButtonIndicator, &opt, this) : 0;
            if (menuArrowWidth > 0)
            {
                opt.text += QLatin1Char(' ');
                opt.text += QLatin1Char(' ');
            }
        }
    }
    style()->drawComplexControl((QStyle::ComplexControl)CommonStyle::CC_RibbonButton, &opt, &p, this);
}

/*! \reimp */
void RibbonButton::actionEvent(QActionEvent* event)
{
    QToolButton::actionEvent(event);
    switch (event->type()) 
    {
        case QEvent::ActionChanged:
        case QEvent::ActionAdded:
        case QEvent::ActionRemoved:
            changed();
            break;
        default:
            break;
    }
}

/*! \reimp */
void RibbonButton::changeEvent(QEvent* event)
{
    QToolButton::changeEvent(event);
}
