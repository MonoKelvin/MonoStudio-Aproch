/****************************************************************************
 * @file    ADropDownAnimation.cpp
 * @date    2025-01-01 
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
#include "ADropDownAnimation.h"

#include <QApplication>
#include <QElapsedtimer>
#include <QEvent>
#include <QImage>
#include <QPainter>
#include <QScreen>
#include <QPixmap>
#include <QPointer>
#include <QTimer>
#include <QWindow>

APROCH_NAMESPACE_BEGIN

class ADropDownAnimation : public QWidget
{
    Q_OBJECT
public:
    ADropDownAnimation(QWidget* w, Qt::WindowFlags f, EDropDownDirectionFlags orient);
    void run(int time);

protected:
    void paintEvent(QPaintEvent*) override;
    void closeEvent(QCloseEvent*) override;

private slots:
    void scroll();

private:
    QPointer<QWidget> widget;

    int currentHeight;
    int currentWidth;
    int totalHeight;
    int totalWidth;

    int duration;
    int elapsed;
    bool done;
    bool showWidget;
    EDropDownDirectionFlags orientation;

    QTimer anim;
    QElapsedTimer checkTime;

    QPixmap pm;
};

static ADropDownAnimation* q_roll = nullptr;

ADropDownAnimation::ADropDownAnimation(QWidget* w, Qt::WindowFlags f, EDropDownDirectionFlags orient)
    : QWidget(nullptr, f)
    , orientation(orient)
{
    setScreen(w->screen());
    //QWidgetPrivate::get(this)->setScreen(w->screen());
#ifndef Q_OS_WIN
    setEnabled(false);
#endif

    widget = w;
    Q_ASSERT(widget);

    setAttribute(Qt::WA_NoSystemBackground, true);

    if (widget->testAttribute(Qt::WA_Resized))
    {
        totalWidth = widget->width();
        totalHeight = widget->height();
    }
    else
    {
        totalWidth = widget->sizeHint().width();
        totalHeight = widget->sizeHint().height();
    }

    currentHeight = totalHeight;
    currentWidth = totalWidth;

    if (orientation & (EDropDownDirection::Right | EDropDownDirection::Left))
        currentWidth = 0;
    if (orientation & (EDropDownDirection::Down | EDropDownDirection::Up))
        currentHeight = 0;

    pm = widget->grab();
}

void ADropDownAnimation::paintEvent(QPaintEvent*)
{
    int x = orientation & EDropDownDirection::Right ? qMin(0, currentWidth - totalWidth) : 0;
    int y = orientation & EDropDownDirection::Down ? qMin(0, currentHeight - totalHeight) : 0;

    QPainter p(this);
    p.drawPixmap(x, y, pm);
}

void ADropDownAnimation::closeEvent(QCloseEvent* e)
{
    e->accept();
    if (done)
        return;

    showWidget = false;
    done = true;
    scroll();

    QWidget::closeEvent(e);
}

void ADropDownAnimation::run(int time)
{
    if (!widget)
        return;

    duration = time;
    elapsed = 0;

    if (duration < 0)
    {
        int dist = 0;
        if (orientation & (EDropDownDirection::Right | EDropDownDirection::Left))
            dist += totalWidth - currentWidth;
        if (orientation & (EDropDownDirection::Down | EDropDownDirection::Up))
            dist += totalHeight - currentHeight;
        duration = qMin(qMax(dist / 3, 50), 120);
    }

    connect(&anim, SIGNAL(timeout()), this, SLOT(scroll()));

    move(widget->geometry().x(), widget->geometry().y());
    resize(qMin(currentWidth, totalWidth), qMin(currentHeight, totalHeight));

    //This is roughly equivalent to calling setVisible(true) without actually showing the widget
    widget->setAttribute(Qt::WA_WState_ExplicitShowHide, true);
    widget->setAttribute(Qt::WA_WState_Hidden, false);

    show();
    setEnabled(false);

    showWidget = true;
    done = false;
    anim.start(1);
    checkTime.start();
}

void ADropDownAnimation::scroll()
{
    if (!done && widget)
    {
        int tempel = checkTime.elapsed();
        if (elapsed >= tempel)
            elapsed++;
        else
            elapsed = tempel;

        if (currentWidth != totalWidth)
        {
            currentWidth = totalWidth * (elapsed / duration)
                + (2 * totalWidth * (elapsed % duration) + duration)
                / (2 * duration);
            // equiv. to int((totalWidth*elapsed) / duration + 0.5)
        }
        if (currentHeight != totalHeight)
        {
            currentHeight = totalHeight * (elapsed / duration)
                + (2 * totalHeight * (elapsed % duration) + duration)
                / (2 * duration);
            currentHeight *= qTan(currentHeight);
            // equiv. to int((totalHeight*elapsed) / duration + 0.5)
        }
        done = (currentHeight >= totalHeight) &&
            (currentWidth >= totalWidth);

        int w = totalWidth;
        int h = totalHeight;
        int x = widget->geometry().x();
        int y = widget->geometry().y();

        if (orientation & EDropDownDirection::Right || orientation & EDropDownDirection::Left)
            w = qMin(currentWidth, totalWidth);
        if (orientation & EDropDownDirection::Down || orientation & EDropDownDirection::Up)
            h = qMin(currentHeight, totalHeight);

        setUpdatesEnabled(false);
        if (orientation & EDropDownDirection::Up)
            y = widget->geometry().y() + qMax(0, totalHeight - currentHeight);
        if (orientation & EDropDownDirection::Left)
            x = widget->geometry().x() + qMax(0, totalWidth - currentWidth);
        if (orientation & EDropDownDirection::Up || orientation & EDropDownDirection::Left)
            move(x, y);

        resize(w, h);
        setUpdatesEnabled(true);
        repaint();
    }
    if (done || !widget)
    {
        anim.stop();
        if (widget)
        {
            if (!showWidget)
            {
#ifdef Q_OS_WIN
                setEnabled(true);
                setFocus();
#endif
                widget->hide();
            }
            else
            {
                //Since we are faking the visibility of the widget
                //we need to unset the hidden state on it before calling show
                widget->setAttribute(Qt::WA_WState_Hidden, true);
                widget->show();
                lower();
            }
        }
        q_roll = nullptr;
        deleteLater();
    }
}

void aDropDownEffect(QWidget* w, EDropDownDirectionFlags orient, int time)
{
    if (q_roll)
    {
        q_roll->deleteLater();
        q_roll = nullptr;
    }

    if (!w)
        return;

    QCoreApplication::sendPostedEvents(w, QEvent::Move);
    QCoreApplication::sendPostedEvents(w, QEvent::Resize);
    Qt::WindowFlags flags = Qt::ToolTip;

    // those can be popups - they would steal the focus, but are disabled
    q_roll = new ADropDownAnimation(w, flags, orient);
    q_roll->run(time);
}

APROCH_NAMESPACE_END

#include "ADropDownAnimation.moc"
