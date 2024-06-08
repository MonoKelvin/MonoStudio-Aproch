/****************************************************************************
 * @file    ABorderlessWidget.h
 * @date    2022-05-04
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
#pragma once
#include <QDialog>

#ifdef Q_OS_WIN
#include <windowsx.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi")
#endif

APROCH_NAMESPACE_BEGIN

/**
 * @brief 无边框窗口控件
 */
template <class BaseType>
class ABorderlessWidgetT : public QWidget
{
public:
    explicit ABorderlessWidgetT(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags())
        : BaseType(parent, flags), mIsResizable(true)
    {
#ifdef Q_OS_WIN
        const MARGINS shadow = {0, 0, 0, 1};
        ::DwmExtendFrameIntoClientArea((HWND)winId(), &shadow);
#endif
    }

    /**
     * @brief 设置是否可以缩放缩放窗口
     * @param resizable 是否可以缩放窗口
     */
    void setResizable(bool resizable)
    {
#ifdef Q_OS_WIN
        const HWND theHwnd = (HWND)winId();
        const LONG style = GetWindowLong(theHwnd, GWL_STYLE);
        if (resizable)
            SetWindowLong(theHwnd, GWL_STYLE, style | WS_THICKFRAME | WS_BORDER | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
        else
            SetWindowLong(theHwnd, GWL_STYLE, style & ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX);
#endif
        mIsResizable = resizable;
    }

    /**
     * @brief 获得窗口是否可以缩放
     * @return 窗口是否可以缩放
     */
    inline bool isResizable(void) const noexcept
    {
        return mIsResizable;
    }

    /**
     * @brief 添加可拖拽窗口的区域控件
     * @param regionWidget 区域控件
     */
    void appendDragRegionWidget(QWidget *regionWidget)
    {
        if (!mDragRegionWidgets.contains(regionWidget))
            mDragRegionWidgets.append(regionWidget);
    }

    /**
     * @brief 是否点在可托拽的窗口区域控件内
     * @param point 点
     * @return 在窗口可托拽区域控件返回true，否则返回false
     */
    bool isPointInDragRegion(const QPoint &point)
    {
        if (mDragRegionWidgets.contains(this))
        {
            if (childAt(point) == nullptr)
                return true;
        }

        for (const auto &regionWidget : mDragRegionWidgets)
        {
            if (childAt(point) == regionWidget)
                return true;
        }
        return false;
    }

protected:
    virtual void paintEvent(QPaintEvent *ev) override
    {
        APROCH_USE_STYLE_SHEET();
        BaseType::paintEvent(ev);
    }

    virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result) override
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
            if (!mIsResizable && msg->wParam == HTCAPTION)
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
                if ((nX > 0) && (nX < 8))
                    *result = HTLEFT;
                if ((nX > width() - 8) && (nX < width()))
                    *result = HTRIGHT;
                if ((nY > 0) && (nY < 8))
                    *result = HTTOP;
                if ((nY > height() - 8) && (nY < height()))
                    *result = HTBOTTOM;
                if ((nX > 0) && (nX < 8) && (nY > 0) && (nY < 8))
                    *result = HTTOPLEFT;
                if ((nX > width() - 8) && (nX < width()) && (nY > 0) && (nY < 8))
                    *result = HTTOPRIGHT;
                if ((nX > 0) && (nX < 8) && (nY > height() - 8) && (nY < height()))
                    *result = HTBOTTOMLEFT;
                if ((nX > width() - 8) && (nX < width()) && (nY > height() - 8) && (nY < height()))
                    *result = HTBOTTOMRIGHT;
            }

            // 如果正在缩放，则直接返回
            if (*result != 0)
                return true;

            if (isPointInDragRegion(QPoint(nX, nY)))
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

private:
    /** @brief 可拖动窗口的区域 */
    QWidgetList mDragRegionWidgets;

    /** @brief 是否可以缩放尺寸 */
    bool mIsResizable;

private:
    Q_DISABLE_COPY_MOVE(ABorderlessWidgetT)
};

using ABorderlessWidget = ABorderlessWidgetT<QWidget>; // 无边框控件
using ABorderlessDialog = ABorderlessWidgetT<QDialog>; // 无边框对话框

APROCH_NAMESPACE_END
