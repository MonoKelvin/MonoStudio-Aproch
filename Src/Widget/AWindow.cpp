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

#ifdef Q_OS_WIN
// #include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi")
#endif

APROCH_NAMESPACE_BEGIN

AWindow::AWindow(QWidget *parent, Qt::WindowType type)
    : QWidget(parent, type), mCentralWidget(nullptr), mEventLoop(nullptr), mIsResizable(true), mIsMovable(true), mIsEnableMaximized(true)
{
    setMaximumSize(QGuiApplication::primaryScreen()->size());
    // setMouseTracking(true);

#ifdef Q_OS_WIN
    const MARGINS shadow = {0, 0, 0, 1};
    ::DwmExtendFrameIntoClientArea((HWND)winId(), &shadow);
#endif

    // 主布局
    mMainLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    mMainLayout->setSpacing(0);
    mMainLayout->setContentsMargins(0, 0, 0, 0);

    // 标题栏
    mCaptionBar = new ACaptionBar(this);
    mMainLayout->setMenuBar(mCaptionBar);

    // 调整显示层次
    mCaptionBar->raise();

    // 默认的窗口初始化数据
    mWindowStyle.ResizerThickness = 8;
    appendDragRegionWidget(mCaptionBar);
    setResizable(mIsResizable);

    // 窗口控制按钮
    connect(mCaptionBar->getMinimizeButton(), &QPushButton::clicked, this, &AWindow::showMinimized);
    connect(mCaptionBar->getMaxRestoreButton(), &QPushButton::clicked, this, &AWindow::switchMaxOrOriginalSize);
    connect(mCaptionBar->getCloseButton(), &QPushButton::clicked, this, &AWindow::close);

    connect(this, &AWindow::windowIconChanged, this, &AWindow::setCaptionIcon);
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
    mEventLoop = new QEventLoop(this);
    const int result = mEventLoop->exec(QEventLoop::DialogExec);
    if (guard.isNull())
        return -1;

    if (deleteOnClose)
        delete this;
    return result;
}

void AWindow::setResizable(bool resizable) noexcept
{
    if (!resizable)
    {
        if (nullptr != mCaptionBar->getMaxRestoreButton())
            mCaptionBar->getMaxRestoreButton()->setEnabled(false);

#ifdef Q_OS_WIN
        const HWND theHwnd = (HWND)winId();
        const LONG style = GetWindowLong(theHwnd, GWL_STYLE);
        SetWindowLong(theHwnd, GWL_STYLE, style & ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX);
    }
    else
    {

        const HWND theHwnd = (HWND)winId();
        const LONG style = GetWindowLong(theHwnd, GWL_STYLE);
        SetWindowLong(theHwnd, GWL_STYLE, style | WS_THICKFRAME | WS_BORDER | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
#endif
    }

    mIsResizable = resizable;
}

void AWindow::setEnableMaximized(bool enable) noexcept
{
    mIsEnableMaximized = enable;

#ifdef Q_OS_WIN
    LONG styleValue = ::GetWindowLong(HWND(winId()), GWL_STYLE);
    if (mIsEnableMaximized)
    {
        styleValue |= WS_MAXIMIZEBOX;
        ::SetWindowLong(HWND(winId()), GWL_STYLE, styleValue);
    }
    else
    {
        styleValue &= ~WS_MAXIMIZEBOX;
        ::SetWindowLong(HWND(winId()), GWL_STYLE, styleValue);
    }
#endif

    if (nullptr != mCaptionBar)
    {
        mCaptionBar->setWidgetsVisibility(WindowMaximizeButton,
                                          mIsEnableMaximized ? WVS_Show : WVS_Invisible);
    }
}

void AWindow::setWindowStyle(const SWindowStyle &windowStyleOp)
{
    mWindowStyle = windowStyleOp;
}

void AWindow::setCentralWidget(QWidget *widget)
{
    if (mCentralWidget == widget)
    {
        return;
    }

    if (nullptr != mCentralWidget)
    {
        mCentralWidget->deleteLater();
        mCentralWidget = nullptr;
    }

    if (nullptr != widget)
    {
        mCentralWidget = widget;
        if (mCentralWidget->parentWidget() != this)
        {
            mCentralWidget->setParent(this);
        }

        mMainLayout->insertWidget(0, mCentralWidget);

        mCentralWidget->installEventFilter(this);
        mCentralWidget->lower();
    }
}

void AWindow::switchMaxOrOriginalSize()
{
    if (!mIsResizable)
        return;

    if (isFullScreen() || isMinimized())
    {
        return;
    }

    if (isMaximized())
    {
        showNormal();
    }
    else if (mIsEnableMaximized)
    {
        showMaximized();
    }
}

void AWindow::appendDragRegionWidget(QWidget *regionWidget)
{
    if (!mDragRegionWidgets.contains(regionWidget))
        mDragRegionWidgets.append(regionWidget);
}

bool AWindow::isPointInDragRegion(const QPoint &point) const
{
    for (const auto &regionWidget : mDragRegionWidgets)
    {
        if (childAt(point) == regionWidget)
            return true;
    }

    return false;
}

void AWindow::setCaptionIcon(const QIcon &icon)
{
    if (nullptr == mCaptionBar || nullptr == mCaptionBar->getTitle())
        return;

    mCaptionBar->getTitle()->setPixmap(icon.pixmap(0));
}

bool AWindow::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
    if (nullptr == message || nullptr == result)
        return false;

#ifdef Q_OS_WIN
    if (eventType != "windows_generic_MSG")
        return false;

    const MSG *msg = static_cast<MSG *>(message);
    QWidget *theWidget = QWidget::find(reinterpret_cast<WId>(msg->hwnd));
    if (!theWidget)
        return false;

    const int nX = GET_X_LPARAM(msg->lParam) - geometry().x();
    const int nY = GET_Y_LPARAM(msg->lParam) - geometry().y();

    switch (msg->message)
    {
    case WM_NCLBUTTONDBLCLK:
    {
        if ((!mIsEnableMaximized || !mIsResizable) && msg->wParam == HTCAPTION)
        {
            *result = 0;
            return false;
        }
        break;
    }
    case WM_GETMINMAXINFO:
    {
        if (::IsZoomed(msg->hwnd))
        {
            RECT frame = {0, 0, 0, 0};
            AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);
            frame.left = abs(frame.left);
            frame.top = abs(frame.bottom);
            theWidget->setContentsMargins(frame.left, frame.top, frame.right, frame.bottom);
        }
        else
        {
            theWidget->setContentsMargins(0, 0, 0, 0);
        }

        *result = ::DefWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam);
        return true;
    }
    case WM_NCCALCSIZE:
    {
        *result = 0; // 去除标题栏
        return true;
    }
    case WM_NCHITTEST:
    {
        if (mIsResizable)
        {
            // 鼠标区域位于窗体边框，进行缩放
            if ((nX > 0) && (nX < mWindowStyle.ResizerThickness))
                *result = HTLEFT;
            if ((nX > this->width() - mWindowStyle.ResizerThickness) && (nX < this->width()))
                *result = HTRIGHT;
            if ((nY > 0) && (nY < mWindowStyle.ResizerThickness))
                *result = HTTOP;
            if ((nY > this->height() - mWindowStyle.ResizerThickness) && (nY < this->height()))
                *result = HTBOTTOM;
            if ((nX > 0) && (nX < mWindowStyle.ResizerThickness) && (nY > 0) && (nY < mWindowStyle.ResizerThickness))
                *result = HTTOPLEFT;
            if ((nX > this->width() - mWindowStyle.ResizerThickness) && (nX < this->width()) && (nY > 0) && (nY < mWindowStyle.ResizerThickness))
                *result = HTTOPRIGHT;
            if ((nX > 0) && (nX < mWindowStyle.ResizerThickness) && (nY > this->height() - mWindowStyle.ResizerThickness) && (nY < this->height()))
                *result = HTBOTTOMLEFT;
            if ((nX > this->width() - mWindowStyle.ResizerThickness) && (nX < this->width()) && (nY > this->height() - mWindowStyle.ResizerThickness) && (nY < this->height()))
                *result = HTBOTTOMRIGHT;
        }

        // 如果正在缩放，则直接返回
        if (*result != 0)
            return true;

        if (mIsMovable && isPointInDragRegion(QPoint(nX, nY)))
        {
            *result = HTCAPTION;
            return true;
        }

        break;
    }
    default:
        break;
    }

#endif // Q_OS_WIN

    return QWidget::nativeEvent(eventType, message, result);
}

void AWindow::paintEvent(QPaintEvent *ev)
{
    APROCH_USE_STYLE_SHEET();

    QPainter p(this);
    p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
}

void AWindow::closeEvent(QCloseEvent *ev)
{
    if (mEventLoop != nullptr)
    {
        mEventLoop->exit();
        mEventLoop = nullptr;
    }
    ev->accept();
}

APROCH_NAMESPACE_END
