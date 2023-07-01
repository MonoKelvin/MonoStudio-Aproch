/****************************************************************************
 * @file    AWindowTitleBar_win.h
 * @date    2023-07-01
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
#include "AWindowTitleBar_p.h"

#ifdef A_PRIVATE_HEADERS
#include <qpa/qplatformwindow.h>
#include <qpa/qplatformnativeinterface.h>
#endif

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class AWindowTitleBarGlowManager;
class AWindowsStyleMouseHandlerWin;

/* AWindowTitleBarPrivateWin */
class AWindowTitleBarPrivateWin : public AWindowTitleBarPrivate, public AWindowsHook
{
public:
    AWindowTitleBarPrivateWin();
    virtual ~AWindowTitleBarPrivateWin();
public:
    virtual void update(const QRect& rect);
    virtual void update(const QRegion& region);
    virtual bool isActive() const;
    virtual void showSystemMenu(const QPoint& pos);
public:
    bool isAeroGlassEnabled() const;
    void setAeroGlassEnabled(bool enabled);
    void setTransitionsEffectEnabled(bool enabled);

    virtual void nativeCommand(QMouseEvent* event, Qt::WindowFrameSection frameSection);
    void execNativeCommand(const QPoint& pos);
protected:
    bool isCompositionEnabled() const;
    bool isCompositionSupported() const;
    virtual LRESULT winProc(HWND hWnd, uint nMessage, WPARAM wParam, LPARAM lParam);
    virtual void winEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);
    void initializeWindowUI();
    void finalizeWindowUI();
    void forceNonClientSizeUpdate();
    bool updateDwmSettings(bool enabled);
    void updateWindowSize();
    virtual void styleChanged();
    virtual void updateSysButtonsRect();
    virtual QStyle::SubControl getHoverNativeSubControl(const QPoint& pos);

    bool handleMessage(MSG* msg, LRESULT& result);
    bool handleNonClientCreate();
    bool handleNonClientDestroy();
    bool handleNonClientPaint(HRGN updateRegion);
    bool handleNonClientCalcSize(NCCALCSIZE_PARAMS* sizeParams, bool calcRects);
    bool handlePaint();
    bool handleNonClientMouseEvent(MSG* msg, LRESULT& outResult);
    int handleNonClientHitTest(LPPOINT ptHit);

    void setRedraw(bool redraw);
    bool isIconic() const;
    void enforceMenuBackground();
    void paintNonClientAreaPart(HDC hdc, const QRegion& clipRegion, const QMargins& margins, Qt::WindowFrameSection section);
    void paintNonClientArea(HDC hdc);
protected:
    virtual bool installWindow();
    virtual void resetWindow();
    virtual bool eventFilter(QObject* object, QEvent* event);
    virtual void mouseMoveNonClientEvent(QMouseEvent* event);
    virtual void mouseContextNonClientEvent(QContextMenuEvent* event);
    virtual void resizeNonClientEvent(const QSize& windowSize);
    virtual QPoint mapFromGlobal(const QPoint& pos) const;
    virtual QPoint mapToGlobal(const QPoint& pos) const;
    virtual QPoint tranlateToNonClient(const QPoint& clientPos);
    virtual bool isGlowEnabled() const;
private:
    void checkMaximized();
    void fixupQtInternals(bool forceUpdate = true);
    void setTrackMouseEnabled(bool enabled);
    bool hasMaximizedStyle() const;
    bool hasFullScreenStyle() const;
    bool hasSysMenuStyle() const;
    void setSysMenuStyle(bool enabled);
    QMargins extraFrameMargins() const;
    QMargins invisibleNonClientMaximizeMargins() const;
    QMargins calcSystemFrameMargins() const;
    QMargins calcWidndows10FrameMargins() const;
    bool nativeCommandNeeded() const;
private:
    bool m_isActive;
    bool m_isFinalized;
    bool m_aeroGlassEnabled;
    bool m_lockStyleChange;
    bool m_fixupQtNeeded;
    bool m_mouseTracking;
    bool m_fillClientArea;

    AWindowTitleBarGlowManager* m_glowManager;
    LRESULT m_hitTest;
    bool m_ignoreSysCommand;
    bool m_sysCommandStatus;
    int m_sysCommand;
};

APROCH_NAMESPACE_END