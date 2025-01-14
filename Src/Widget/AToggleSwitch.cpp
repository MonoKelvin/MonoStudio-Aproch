/****************************************************************************
 * @file    AToggleSwitch.cpp
 * @date    2025-01-12 
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
#include "AToggleSwitch.h"
#include "Widget/Private/AToggleSwitch_p.h"

#include <QListView>
#include <QApplication>

APROCH_NAMESPACE_BEGIN

const int INDICATOR_PADDING = 4;

void AToggleSwitchIndicator::onHoverEnter()
{
}

void AToggleSwitchIndicator::onHoverLeave()
{
}

void AToggleSwitchIndicator::onPressed()
{
}

void AToggleSwitchIndicator::onRelease()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void AToggleSwitchPrivate::updateIndicatorPosition(bool toggled)
{
    QRect rct = indicator->geometry();
    int newy = (container->height() - indicator->height()) / 2;

    indicatorAnim->stop();
    if (toggled)
    {
        indicatorAnim->setStartValue(rct);
        rct.moveTo(container->width() - indicator->width() - INDICATOR_PADDING, newy);
        indicatorAnim->setEndValue(rct);
        indicatorAnim->start();
    }
    else
    {
        QRect rct = indicator->geometry();
        indicatorAnim->setStartValue(rct);
        rct.moveTo(INDICATOR_PADDING, newy);
        indicatorAnim->setEndValue(rct);
        indicatorAnim->start();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

AToggleSwitch::AToggleSwitch(QWidget* parent)
    :AToggleSwitch(QString(), parent)
{
}

AToggleSwitch::AToggleSwitch(const QString& text, QWidget* parent)
    : QRadioButton(text, parent)
    , d(new AToggleSwitchPrivate(this))
{
    setAttribute(Qt::WA_StyledBackground);
    setAutoExclusive(false);
    setMouseTracking(true);

    d->offText = this->text();
    d->onText = this->text();

    QBoxLayout* layout = new QBoxLayout(isRightToLeft() ? QBoxLayout::RightToLeft :
                                        QBoxLayout::LeftToRight, this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    d->container = new AToggleSwitchIndicatorContainer(this);
    d->container->installEventFilter(this);
    d->container->show();
    layout->addWidget(d->container, 0, Qt::AlignLeft | Qt::AlignHCenter);

    d->indicator = new AToggleSwitchIndicator(d->container);
    d->indicator->installEventFilter(this);
    d->indicator->show();
    d->indicator->raise();

    d->indicatorAnim = new QPropertyAnimation(d->indicator, "geometry", d->indicator);
    d->indicatorAnim->setEasingCurve(QEasingCurve::OutCubic);
    d->indicatorAnim->setDuration(180);

    connect(this, &AToggleSwitch::toggled, this, &AToggleSwitch::onToggled);
}

AToggleSwitch::~AToggleSwitch()
{
    delete d;
}

QString AToggleSwitch::getOnText() const
{
    return d->onText;
}

QString AToggleSwitch::getOffText() const
{
    return d->offText;
}

void AToggleSwitch::setOffText(const QString& offText)
{
    d->offText = offText;

    if (!isChecked())
        setText(d->offText);
}

void AToggleSwitch::setOnText(const QString& onText)
{
    d->onText = onText;

    if (isChecked())
        setText(d->onText);
}

QSize AToggleSwitch::sizeHint() const
{
    return QRadioButton::sizeHint();
}

void AToggleSwitch::paintEvent(QPaintEvent* e)
{
    QStylePainter p(this);
    QStyleOptionButton opt;
    initStyleOption(&opt);

    QStyle* theStyle = style();
    QStyleOptionButton subopt = opt;

    //subopt.rect = theStyle->subElementRect(QStyle::SE_RadioButtonIndicator, &opt, this);
    //theStyle->drawPrimitive(QStyle::PE_IndicatorRadioButton, &subopt, &p, this);

    subopt.rect = theStyle->subElementRect(QStyle::SE_RadioButtonContents, &opt, this);
    theStyle->drawControl(QStyle::CE_RadioButtonLabel, &subopt, &p, this);

    if (opt.state & QStyle::State_HasFocus)
    {
        QStyleOptionFocusRect fropt;
        fropt.QStyleOption::operator=(opt);
        fropt.rect = theStyle->subElementRect(QStyle::SE_RadioButtonFocusRect, &opt, this);
        theStyle->drawPrimitive(QStyle::PE_FrameFocusRect, &fropt, &p, this);
    }
}

bool AToggleSwitch::event(QEvent* e)
{
    switch (e->type())
    {
    case QEvent::MouseButtonPress:
    {
        if (!isEnabled())
            break;
        QMouseEvent* mevt = static_cast<QMouseEvent*>(e);
        d->mousePressedPoint = mevt->pos();
        d->indicatorStartPoint = d->indicator->pos();
        d->isMousePressed = true;
    }
    break;
    case QEvent::MouseButtonRelease:
    {
        if (!isEnabled())
            break;
        d->isMousePressed = false;
        if (!d->isPressedMoving)
            break;
        d->isPressedMoving = false;
        const int cx = d->indicator->geometry().center().x();
        if (isChecked())
        {
            if (cx >= d->container->rect().center().x())
                d->updateIndicatorPosition(true);
            else
                setChecked(false);
        }
        else
        {
            if (cx <= d->container->rect().center().x())
                d->updateIndicatorPosition(false);
            else
                setChecked(true);
        }
        e->accept();
        return true;
    }
    break;
    case QEvent::MouseMove:
    {
        if (!isEnabled())
            break;
        if (!d->isMousePressed)
            break;
        QMouseEvent* mevt = static_cast<QMouseEvent*>(e);
        const int dx = mevt->pos().x() - d->mousePressedPoint.x();
        int newX = qMax(INDICATOR_PADDING, qMin(d->indicatorStartPoint.x() + dx,
                    d->container->width() - d->indicator->width() - INDICATOR_PADDING));
        d->indicator->move(newX, d->indicator->y());
        d->isPressedMoving = true;
    }
    break;
    case QEvent::EnabledChange:
    {
        d->container->setEnabled(this->isEnabled());
    }
    break;
    default:
        break;
    }
    return QRadioButton::event(e);
}

bool AToggleSwitch::eventFilter(QObject* watched, QEvent* e)
{
    if (watched == d->container)
    {
        switch (e->type())
        {
        case QEvent::Resize:
        case QEvent::Move:
        case QEvent::Show:
        {
            d->updateIndicatorPosition(isChecked());
            if (e->type() == QEvent::Show)
                d->container->setFixedSize(d->container->sizeHint());
        }
            break;
        default:
            break;
        }
    }
    return false;
}

void AToggleSwitch::onToggled(bool toggled)
{
    d->updateIndicatorPosition(toggled);
    if (toggled)
    {
        if (text() != d->onText)
        {
            setText(d->onText);
            AGraphicsToolkit::updateWidgetLayout(this);
        }
    }
    else
    {
        if (text() != d->offText)
        {
            setText(d->offText);
            AGraphicsToolkit::updateWidgetLayout(this);
        }
    }

    d->indicator->setProperty("toggled", toggled);
    d->container->setProperty("toggled", toggled);
    d->indicator->style()->unpolish(d->indicator);
    d->indicator->style()->polish(d->indicator);
    d->container->style()->unpolish(d->container);
    d->container->style()->polish(d->container);
}

APROCH_NAMESPACE_END
