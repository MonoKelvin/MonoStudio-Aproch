/****************************************************************************
 * @file    AWindow.cpp
 * @date    2021-1-24
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
#include "AWindow.h"

#include <QGuiApplication>
#include <QScreen>

#include <QWindowKit/QWKWidgets/widgetwindowagent.h>

APROCH_NAMESPACE_BEGIN

class AWindowPrivate
{
public:
    QEventLoop* eventLoop = nullptr;
};

static inline void emulateLeaveEvent(QWidget* widget)
{
    Q_ASSERT(widget);
    if (!widget)
        return;
    
    QTimer::singleShot(0, widget, [widget]() {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        const QScreen* screen = widget->screen();
#else
        const QScreen* screen = widget->windowHandle()->screen();
#endif
        const QPoint globalPos = QCursor::pos(screen);
        if (!QRect(widget->mapToGlobal(QPoint{ 0, 0 }), widget->size()).contains(globalPos))
        {
            QCoreApplication::postEvent(widget, new QEvent(QEvent::Leave));
            if (widget->testAttribute(Qt::WA_Hover))
            {
                const QPoint localPos = widget->mapFromGlobal(globalPos);
                const QPoint scenePos = widget->window()->mapFromGlobal(globalPos);
                static constexpr const auto oldPos = QPoint{};
                const Qt::KeyboardModifiers modifiers = QGuiApplication::keyboardModifiers();
#if (QT_VERSION >= QT_VERSION_CHECK(6, 4, 0))
                const auto event =
                    new QHoverEvent(QEvent::HoverLeave, scenePos, globalPos, oldPos, modifiers);
                Q_UNUSED(localPos);
#elif (QT_VERSION >= QT_VERSION_CHECK(6, 3, 0))
                const auto event = new QHoverEvent(QEvent::HoverLeave, localPos, globalPos, oldPos, modifiers);
                Q_UNUSED(scenePos);
#else
                const auto event = new QHoverEvent(QEvent::HoverLeave, localPos, oldPos, modifiers);
                Q_UNUSED(scenePos);
#endif
                QCoreApplication::postEvent(widget, event);
            }
        }
    });
}

AWindow::AWindow(QWidget* parent)
    : AWindow(EWindowCaptionWidget::WindowWidgets, parent)
{
}

AWindow::AWindow(const FWindowCaptionWidgets& captionWidgets, QWidget *parent)
    : QMainWindow(parent)
    , d_ptr(new AWindowPrivate())
{
    initStyle(this);
    setAttribute(Qt::WA_StyledBackground);

    // 标题栏
    auto captionBar = new ACaptionBar(captionWidgets, this);
    captionBar->setHostWidget(this);

    setMenuWidget(captionBar);
    updateCaptionBar();

#ifdef Q_OS_MAC
    mWinAgent->setSystemButtonAreaCallback([](const QSize& size) {
        static constexpr const int width = 75;
        return QRect(QPoint(size.width() - width, 0), QSize(width, size.height()));
    });
#endif

#ifndef Q_OS_MAC
    connect(captionBar, &ACaptionBar::minimizeRequested, this, &QWidget::showMinimized);
    connect(captionBar, &ACaptionBar::maximizeRequested, this, [this, captionBar](bool max) {
        // 固定大小不可缩放
        if (sizePolicy() == QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed) ||
            (minimumWidth() == maximumWidth() && minimumHeight() == maximumHeight()))
            return;

        if (max)
            showMaximized();
        else
            showNormal();

        // It's a Qt issue that if a QAbstractButton::clicked triggers a window's maximization,
        // the button remains to be hovered until the mouse move. As a result, we need to
        // manually send leave events to the button.
        emulateLeaveEvent(captionBar->getMaxButton());
    });
    connect(captionBar, &ACaptionBar::closeRequested, this, &QWidget::close);
#endif
}

AWindow::~AWindow()
{
}

int AWindow::showModality()
{
    setAttribute(Qt::WA_ShowModal, true);
    setWindowModality(Qt::ApplicationModal);

    const bool deleteOnClose = testAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_DeleteOnClose, false);

    show();

    QPointer<QWidget> guard = this;
    d_ptr->eventLoop = new QEventLoop(this);
    const int result = d_ptr->eventLoop->exec(QEventLoop::DialogExec);
    if (guard.isNull())
        return -1;

    if (deleteOnClose)
        delete this;
    return result;
}

ACaptionBar* AWindow::getCaptionBar(void) const
{
    return qobject_cast<ACaptionBar*>(menuWidget());
}

void AWindow::updateCaptionBar()
{
    Q_ASSERT(mWinAgent);

    ACaptionBar* captionBar = getCaptionBar();
    if (!captionBar)
        return;

#ifndef Q_OS_MAC
    mWinAgent->setSystemButton(QWK::WindowAgentBase::WindowIcon, captionBar->getIcon());
    mWinAgent->setSystemButton(QWK::WindowAgentBase::Help, captionBar->getHelpButton());
    mWinAgent->setSystemButton(QWK::WindowAgentBase::Minimize, captionBar->getMinButton());
    mWinAgent->setSystemButton(QWK::WindowAgentBase::Maximize, captionBar->getMaxButton());
    mWinAgent->setSystemButton(QWK::WindowAgentBase::Close, captionBar->getCloseButton());
#endif
    mWinAgent->setTitleBar(captionBar);

    if (captionBar->getMenuBar())
        mWinAgent->setHitTestVisible(captionBar->getMenuBar(), true);
}

bool AWindow::event(QEvent* evt)
{
    switch (evt->type())
    {
    case QEvent::WindowActivate: {
        auto captionBar = menuWidget();
        if (captionBar)
        {
            captionBar->setProperty("active", true);
            style()->polish(captionBar);
        }
        break;
    }

    case QEvent::WindowDeactivate: {
        auto captionBar = menuWidget();
        if (captionBar)
        {
            captionBar->setProperty("active", false);
            style()->polish(captionBar);
        }
        break;
    }

    default:
        break;
    }
    return QMainWindow::event(evt);
}

void AWindow::closeEvent(QCloseEvent *ev)
{
    if (d_ptr->eventLoop != nullptr)
    {
        d_ptr->eventLoop->exit();
        d_ptr->eventLoop = nullptr;
    }

    ev->accept();
}

void AWindow::showEvent(QShowEvent* ev)
{
    QMainWindow::showEvent(ev);

    if (isWinUIEnabled())
    {
        setWinUITheme(getWinUITheme());
        setWinUIMaterial(getWinUIMaterial());
    }
}

APROCH_NAMESPACE_END
