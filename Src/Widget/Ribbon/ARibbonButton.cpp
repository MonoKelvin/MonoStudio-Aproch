/****************************************************************************
 * @file    ARibbonButton.cpp
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
#include <QPainter>

#include "ARibbonButton_p.h"
#include "ARibbonBar_p.h"
#include "Widget/Style/ACommonStyle.h"
#include "Widget/Style/ACommonStyle_p.h"
#include "ARibbonGroup.h"
#include "Widget/Style/AStyleHelper.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

ARibbonButtonPrivate::ARibbonButtonPrivate()
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

void ARibbonButtonPrivate::resetWordWrap()
{
    A_P(ARibbonButton);
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

QSize ARibbonButtonPrivate::sizeWordWrap()
{
    A_P(ARibbonButton);
    QStyleOptionToolButton opt;
    p.initStyleOption(&opt);

    QString strText = opt.text;
    QSize sizeText = opt.fontMetrics.size(Qt::TextShowMnemonic, strText);
    const int space = aproch_horizontalAdvanceFont(opt.fontMetrics, QLatin1Char(' ')) * 2;
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
\class ARibbonButton
\inmodule QtitanRibbon
\brief ARibbonButton class implements tool button that used in ribbon ui. Button appearance and behavior adapted for display inside the ARibbonBar. 

*/
ARibbonButton::ARibbonButton(QWidget* parent)
    : QToolButton(parent)
{
    A_INIT_PRIVATE(ARibbonButton);
    setAutoRaise(true);
}

ARibbonButton::~ARibbonButton()
{
    A_DELETE_PRIVATE();
}

bool ARibbonButton::wordWrap() const
{ 
    A_D(const ARibbonButton);
    return d.m_wordWrap; 
}

void ARibbonButton::setWordWrap(bool on)
{
    A_D(ARibbonButton);
    if (d.m_wordWrap != on)
    {
        d.m_wordWrap = on;
        changed();
    }
}

bool ARibbonButton::isLargeIcon() const
{ 
    A_D(const ARibbonButton);
    return d.m_largeIcon; 
}

void ARibbonButton::setLargeIcon(bool large)
{ 
    A_D(ARibbonButton);
    d.m_largeIcon = large; 
}

bool ARibbonButton::simplifiedMode() const
{
    A_D(const ARibbonButton);
    return d.m_simplifiedMode;
}

void ARibbonButton::setSimplifiedMode(bool enabled)
{
    A_D(ARibbonButton);
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
QSize ARibbonButton::sizeHint() const
{
    A_D(const ARibbonButton);
    ARibbonButtonPrivate& pd = const_cast<ARibbonButtonPrivate&>(d);

    if (pd.m_saveButtonStyle != toolButtonStyle() || pd.m_saveArrowType != arrowType())
        const_cast<ARibbonButton *>(this)->changed();

    if (pd.m_sizeHint.isValid())
        return pd.m_sizeHint;

    pd.m_saveButtonStyle = toolButtonStyle();
    pd.m_saveArrowType = arrowType();

    QSize sz = QToolButton::sizeHint();
    if (ARibbonControl* control = qobject_cast<ARibbonControl *>(parentWidget()))
    {
        if (ARibbonBar* ribbonBar = control->parentGroup()->ribbonBar())
            sz.setHeight(ribbonBar->rowItemHeight());
    }

    int w = 0, h = 0;
    QStyleOptionToolButton opt;
    initStyleOption(&opt);

    QString textButton = text();
    if (!textButton.isEmpty() && (bool)style()->styleHint((QStyle::StyleHint)ACommonStyle::SH_RibbonItemUpperCase, 0, this))
    {
        if (qobject_cast<ARibbonTabBar*>(parentWidget()) != nullptr)
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

            if (ARibbonControl* control = qobject_cast<ARibbonControl *>(parentWidget()))
            {
                if (ARibbonBar* ribbonBar = control->parentGroup()->ribbonBar())
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
            textSize.setWidth(textSize.width() + aproch_horizontalAdvanceFont(opt.fontMetrics, QLatin1Char(' ')) * 2);
            h = sz.height();

            if (opt.toolButtonStyle == Qt::ToolButtonTextBesideIcon)
            {
                w = h;
                w += textSize.width() + ACommonStylePrivate::dpiScaled(2, this);

                if (textSize.height() > h)
                    h = textSize.height();
            }
            else
                w = textSize.width() + ACommonStylePrivate::dpiScaled(4, this);
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
            w += ACommonStylePrivate::dpiScaled(1, this);

        if (isLargeIcon() && opt.toolButtonStyle == Qt::ToolButtonIconOnly)
        {
            if (ARibbonControl* control = qobject_cast<ARibbonControl *>(parentWidget()))
            {
                if (ARibbonBar* ribbonBar = control->parentGroup()->ribbonBar())
                {
                    h = ribbonBar->rowItemHeight() * ribbonBar->rowItemCount();
                    w = opt.iconSize.width() + ACommonStylePrivate::dpiScaled(4, this);
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

void ARibbonButton::changed()
{
    A_D(ARibbonButton);
    d.m_sizeHint = QSize();
}

void ARibbonButton::updateIconSize()
{
    const int sz = style()->pixelMetric(isLargeIcon() ? QStyle::PM_LargeIconSize : 
        QStyle::PM_SmallIconSize, 0, this);
    QSize size(sz, sz);
    if (iconSize() != size)
        setIconSize(size);
}

/*! \reimp */
bool ARibbonButton::event(QEvent* event)
{
    QEvent::Type type = event->type();
    if (type == QEvent::LayoutRequest || type == QEvent::FontChange || type == QEvent::StyleChange)
        changed();
    updateIconSize();
    return QToolButton::event(event);
}

/*! \reimp */
void ARibbonButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    A_D(const ARibbonButton);
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
    style()->drawComplexControl((QStyle::ComplexControl)ACommonStyle::CC_RibbonButton, &opt, &p, this);
}

/*! \reimp */
void ARibbonButton::actionEvent(QActionEvent* event)
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
void ARibbonButton::changeEvent(QEvent* event)
{
    QToolButton::changeEvent(event);
}

APROCH_NAMESPACE_END