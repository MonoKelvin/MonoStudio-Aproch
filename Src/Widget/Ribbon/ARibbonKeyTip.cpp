/****************************************************************************
 * @file    ARibbonKeyTip.cpp
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
#include "ARibbonKeyTip.h"
#include "Widget/Style/ACommonStyle_p.h"
#include "Widget/Style/AStyleHelper.h"

#include <QBitmap>

APROCH_NAMESPACE_BEGIN

/* ARibbonKeyTipPrivate */
class ARibbonKeyTipPrivate : public QObject
{
    A_DECLARE_PUBLIC(ARibbonKeyTip)
public:
    explicit ARibbonKeyTipPrivate();
public:
    QString m_strTip;
    QString m_strCaption;
    QString m_strPrefix;
    QWidget* m_owner;
    QAction* m_action;
    QWidget* m_bars;
    QPoint m_pnt;
    bool m_enabled;
    bool m_visible;
    uint m_align;
    bool m_explicit;
};

/* ARibbonKeyTipPrivate */
ARibbonKeyTipPrivate::ARibbonKeyTipPrivate()
    : m_owner(nullptr)
    , m_action(nullptr)
    , m_bars(nullptr)
    , m_enabled(false)
    , m_visible(true)
    , m_align(0)
    , m_explicit(false)
{
}

/*!
    \class ARibbonKeyTip
    \internal
*/
ARibbonKeyTip::ARibbonKeyTip(QWidget* bars, QWidget* owner, const QString& caption,
                             const QPoint& pt, uint align, bool enabled, QAction* action)
    : QFrame(bars,
             Qt::WindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::BypassGraphicsProxyWidget
#ifndef Q_OS_WIN
             | Qt::X11BypassWindowManagerHint
#endif
    ))
{
    A_INIT_PRIVATE(ARibbonKeyTip);
    A_D(ARibbonKeyTip);
    setFocusPolicy(Qt::NoFocus);
    d.m_action = action;
    d.m_owner = owner;
    d.m_strCaption = caption;
    d.m_bars = bars;
    d.m_pnt = pt;
    d.m_enabled = enabled;
    d.m_align = align;

    setEnabled(enabled);

    if (!enabled)
        setWindowOpacity(0.5);
}

ARibbonKeyTip::~ARibbonKeyTip()
{
    A_DELETE_PRIVATE();
}

QPoint ARibbonKeyTip::posTip() const
{
    A_D(const ARibbonKeyTip);
    return d.m_pnt;
}

void ARibbonKeyTip::setVisibleTip(bool visible)
{
    A_D(ARibbonKeyTip);
    d.m_visible = visible;
}

bool ARibbonKeyTip::isVisibleTip() const
{
    A_D(const ARibbonKeyTip);
    return d.m_visible;
}

bool ARibbonKeyTip::isEnabledTip() const
{
    A_D(const ARibbonKeyTip);
    return d.m_enabled;
}

QWidget* ARibbonKeyTip::getBars() const
{
    A_D(const ARibbonKeyTip);
    return d.m_bars;
}

uint ARibbonKeyTip::getAlign() const
{
    A_D(const ARibbonKeyTip);
    return d.m_align;
}

void ARibbonKeyTip::setExplicit(bool exp)
{
    A_D(ARibbonKeyTip);
    d.m_explicit = exp;
}

bool ARibbonKeyTip::isExplicit() const
{
    A_D(const ARibbonKeyTip);
    return d.m_explicit;
}

QString ARibbonKeyTip::getStringTip() const
{
    A_D(const ARibbonKeyTip);
    return d.m_strTip;
}

void ARibbonKeyTip::setStringTip(const QString& str)
{
    A_D(ARibbonKeyTip);
    d.m_strTip = str;
}

QString ARibbonKeyTip::getCaption() const
{
    A_D(const ARibbonKeyTip);
    return d.m_strCaption;
}

void ARibbonKeyTip::setCaption(const QString& str)
{
    A_D(ARibbonKeyTip);
    d.m_strCaption = str;
}

QString ARibbonKeyTip::getPrefix() const
{
    A_D(const ARibbonKeyTip);
    return d.m_strPrefix;
}

void ARibbonKeyTip::setPrefix(const QString& pref)
{
    A_D(ARibbonKeyTip);
    d.m_strPrefix = pref;
}

QWidget* ARibbonKeyTip::getOwner() const
{
    A_D(const ARibbonKeyTip);
    return d.m_owner;
}

QAction* ARibbonKeyTip::getAction() const
{
    A_D(const ARibbonKeyTip);
    return d.m_action;
}

void ARibbonKeyTip::hideAndDestroy()
{
#ifdef Q_OS_MAC //Tries to hide the pop window on Mac quickly.
    setGeometry(QRect(-1, -1, 1, 1));
    setWindowFlags(Qt::Window);
#endif
    hide();
    deleteLater();
}

void ARibbonKeyTip::setVisible(bool visible)
{
    QFrame::setVisible(visible);
}

/*! \reimp */
void ARibbonKeyTip::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    A_D(ARibbonKeyTip);
    QPainter p(this);
    QStyleOption opt;
    opt.initFrom(this);
    if (d.m_enabled)
        opt.state |= QStyle::State_Enabled;
    else
        opt.state &= ~QStyle::State_Enabled;
    style()->drawPrimitive(static_cast<QStyle::PrimitiveElement>(ACommonStyle::PE_RibbonKeyTip), &opt, &p, this);
    style()->drawItemText(&p, opt.rect, Qt::AlignVCenter | Qt::AlignCenter | Qt::TextSingleLine | Qt::TextWordWrap,
                          opt.palette, d.m_enabled, d.m_strTip, QPalette::ToolTipText);
}

/*! \reimp */
void ARibbonKeyTip::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    return QFrame::resizeEvent(event);

    QBitmap maskBitmap(width(), height());
    maskBitmap.clear();
    QPainter p(&maskBitmap);
    p.setBrush(Qt::black);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    p.drawRoundedRect(0, 0, width(), height(), 2, 2, Qt::AbsoluteSize);
#else
    p.drawRoundedRect(0, 0, width() - 1, height() - 1, 2, 2, Qt::AbsoluteSize);
#endif
    p.end();
    setMask(maskBitmap);
}

APROCH_NAMESPACE_END