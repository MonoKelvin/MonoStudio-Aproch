/****************************************************************************
 * @file    APlatform_win.cpp
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
#include "stdafx.h"
#include "APlatform_win.h"
#include <QWidget>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QWindow>
#endif
#include <QScreen>
#include <QLibrary>
#include <QIcon>
#include <QPaintEngine>
#include <QApplication>
#include <QStyle>
#include <QSysInfo>
#include <QSettings>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
#include <QOperatingSystemVersion>
#endif

//
// The most of the following code is copied from Qtitan.
//
// Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
// Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
//

#ifdef A_PRIVATE_HEADERS
#include <qpa/qplatformnativeinterface.h>
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
#include <private/qwindowsfontdatabasebase_p.h>
#endif
#endif
#include "Widget/Style/ACommonStyle.h"
#include "APlatform.h"
#include <tchar.h>

#ifdef A_MEMORY_DEBUG
#include "QtitanMSVSDebug.h"
#endif

#define FALSE_EXIT 2

PtrDwmDefWindowProc APROCH_PRE_NAMESPACE(pDwmDefWindowProc) = nullptr;
PtrDwmIsCompositionEnabled APROCH_PRE_NAMESPACE(pDwmIsCompositionEnabled) = nullptr;
PtrDwmGetWindowAttribute APROCH_PRE_NAMESPACE(pDwmGetWindowAttribute) = nullptr;
PtrDwmSetWindowAttribute APROCH_PRE_NAMESPACE(pDwmSetWindowAttribute) = nullptr;
PtrDwmExtendFrameIntoClientArea APROCH_PRE_NAMESPACE(pDwmExtendFrameIntoClientArea) = nullptr;
PtrDwmEnableBlurBehindWindow APROCH_PRE_NAMESPACE(pDwmEnableBlurBehindWindow) = nullptr;
PtrDwmGetColorizationColor APROCH_PRE_NAMESPACE(pDwmGetColorizationColor) = nullptr;
PtrDwmGetColorizationParameters APROCH_PRE_NAMESPACE(pDwmGetColorizationParameters) = nullptr;
PtrDwmSetColorizationParameters APROCH_PRE_NAMESPACE(pDwmSetColorizationParameters) = nullptr;
PtrDwmFlush APROCH_PRE_NAMESPACE(pDwmFlush) = nullptr;
PtrAdjustWindowRectExForDpi APROCH_PRE_NAMESPACE(pAdjustWindowRectExForDpi) = nullptr;
PtrGetDpiForWindow APROCH_PRE_NAMESPACE(pGetDpiForWindow) = nullptr;
PtrGetDpiForSystem APROCH_PRE_NAMESPACE(pGetDpiForSystem) = nullptr;
PtrGetSystemDpiForProcess APROCH_PRE_NAMESPACE(pGetSystemDpiForProcess) = nullptr;

PtrIsAppThemed APROCH_PRE_NAMESPACE(pIsAppThemed) = nullptr;
PtrOpenThemeData APROCH_PRE_NAMESPACE(pOpenThemeData) = nullptr;
PtrCloseThemeData APROCH_PRE_NAMESPACE(pCloseThemeData) = nullptr;
PtrGetThemeRect APROCH_PRE_NAMESPACE(pGetThemeRect) = nullptr;
PtrGetThemeSysSize APROCH_PRE_NAMESPACE(pGetThemeSysSize) = nullptr;
PtrGetThemeSysFont APROCH_PRE_NAMESPACE(pGetThemeSysFont) = nullptr;
PtrDrawThemeTextEx APROCH_PRE_NAMESPACE(pDrawThemeTextEx) = nullptr;
PtrDrawThemeIcon APROCH_PRE_NAMESPACE(pDrawThemeIcon) = nullptr;
PtrSetWindowTheme APROCH_PRE_NAMESPACE(pSetWindowTheme) = nullptr;
PtrGetThemeColor APROCH_PRE_NAMESPACE(pGetThemeColor) = nullptr;
PtrGetThemeSysColor APROCH_PRE_NAMESPACE(pGetThemeSysColor) = nullptr;
PtrSetWindowThemeAttribute APROCH_PRE_NAMESPACE(pSetWindowThemeAttribute) = nullptr;
PtrSetWindowCompositionAttribute APROCH_PRE_NAMESPACE(pSetWindowCompositionAttribute) = nullptr;

void APROCH_PRE_NAMESPACE(aproch_initializeDrawingResources)()
{
    static bool tried = false;
    if (!tried)
    {
        tried = true;

        QLibrary user32Lib(QString::fromLatin1("user32"));
        pSetWindowCompositionAttribute = (PtrSetWindowCompositionAttribute)user32Lib.resolve("SetWindowCompositionAttribute");
        pAdjustWindowRectExForDpi = (PtrAdjustWindowRectExForDpi)user32Lib.resolve("AdjustWindowRectExForDpi");
        pGetDpiForWindow = (PtrGetDpiForWindow)user32Lib.resolve("GetDpiForWindow");
        pGetDpiForSystem = (PtrGetDpiForSystem)user32Lib.resolve("GetDpiForSystem");
        pGetSystemDpiForProcess = (PtrGetSystemDpiForProcess)user32Lib.resolve("GetSystemDpiForProcess");

        QLibrary dwmLib(QString::fromLatin1("dwmapi"));
        pDwmIsCompositionEnabled = (PtrDwmIsCompositionEnabled)dwmLib.resolve("DwmIsCompositionEnabled");
        pDwmGetWindowAttribute = (PtrDwmGetWindowAttribute)dwmLib.resolve("DwmGetWindowAttribute");
        pDwmSetWindowAttribute = (PtrDwmSetWindowAttribute)dwmLib.resolve("DwmSetWindowAttribute");
        if (pDwmIsCompositionEnabled && pDwmSetWindowAttribute)
        {
            pDwmDefWindowProc = (PtrDwmDefWindowProc)dwmLib.resolve("DwmDefWindowProc");
            pDwmExtendFrameIntoClientArea = (PtrDwmExtendFrameIntoClientArea)dwmLib.resolve("DwmExtendFrameIntoClientArea");
            pDwmEnableBlurBehindWindow = (PtrDwmEnableBlurBehindWindow)dwmLib.resolve("DwmEnableBlurBehindWindow");
            pDwmGetColorizationColor = (PtrDwmGetColorizationColor)dwmLib.resolve("DwmGetColorizationColor");
            pDwmGetColorizationParameters = (PtrDwmGetColorizationParameters)dwmLib.resolve((LPCSTR)127);
            pDwmSetColorizationParameters = (PtrDwmSetColorizationParameters)dwmLib.resolve((LPCSTR)131);
            pDwmFlush = (PtrDwmFlush)dwmLib.resolve("DwmFlush");
        }

        QLibrary themeLib(QString::fromLatin1("uxtheme"));
        pIsAppThemed = (PtrIsAppThemed)themeLib.resolve("IsAppThemed");

        if (pIsAppThemed)
        {
            pOpenThemeData = (PtrOpenThemeData)themeLib.resolve("OpenThemeData");
            pCloseThemeData = (PtrCloseThemeData)themeLib.resolve("CloseThemeData");
            pGetThemeRect = (PtrGetThemeRect)themeLib.resolve("GetThemeRect");
            pGetThemeSysSize = (PtrGetThemeSysSize)themeLib.resolve("GetThemeSysSize");
            pGetThemeSysFont = (PtrGetThemeSysFont)themeLib.resolve("GetThemeSysFont");
            pDrawThemeTextEx = (PtrDrawThemeTextEx)themeLib.resolve("DrawThemeTextEx");
            pDrawThemeIcon = (PtrDrawThemeIcon)themeLib.resolve("DrawThemeIcon");
            pSetWindowTheme = (PtrSetWindowTheme)themeLib.resolve("SetWindowTheme");
            pGetThemeColor = (PtrGetThemeColor)themeLib.resolve("GetThemeColor");
            pGetThemeSysColor = (PtrGetThemeSysColor)themeLib.resolve("GetThemeSysColor");
            pSetWindowThemeAttribute = (PtrSetWindowThemeAttribute)themeLib.resolve("SetWindowThemeAttribute");
        }
    }
}

static bool aproch_isWindows8();

bool APROCH_PRE_NAMESPACE(aproch_isCompositionAvailable)()
{
    static bool tried = pDwmIsCompositionEnabled == nullptr;
    static BOOL dwmEnabled = FALSE;
    if (!tried)
        tried = SUCCEEDED(pDwmIsCompositionEnabled(&dwmEnabled));
    return dwmEnabled;
}

HDC APROCH_PRE_NAMESPACE(aproch_createDC)()
{
    HDC display_dc = GetDC(0);
    HDC hdc = CreateCompatibleDC(display_dc);
    ReleaseDC(0, display_dc);
    Q_ASSERT(hdc);
    return hdc;
}

HBITMAP APROCH_PRE_NAMESPACE(aproch_createDIB)(HDC hdc, int width, int height, QImage::Format format, uchar **bitsIn)
{
    BITMAPINFO_MASK bmi;
    memset(&bmi, 0, sizeof(bmi));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // top-down.
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biSizeImage = 0;

    if (format == QImage::Format_RGB16)
    {
        bmi.bmiHeader.biBitCount = 16;
        bmi.bmiHeader.biCompression = BI_BITFIELDS;
        bmi.redMask = 0xF800;
        bmi.greenMask = 0x07E0;
        bmi.blueMask = 0x001F;
    }
    else
    {
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;
        bmi.redMask = 0;
        bmi.greenMask = 0;
        bmi.blueMask = 0;
    }

    uchar *bits = nullptr;
    HBITMAP bitmap = CreateDIBSection(hdc, reinterpret_cast<BITMAPINFO *>(&bmi),
                                      DIB_RGB_COLORS, reinterpret_cast<void **>(&bits), 0, 0);
    if (Q_UNLIKELY(!bitmap || !bits))
        qFatal("%s: CreateDIBSection failed.", __FUNCTION__);

    *bitsIn = bits;
    return bitmap;
}

int APROCH_PRE_NAMESPACE(aproch_get_sys_command_win)(QWidget *window, QMouseEvent *event,
                                                     Qt::WindowFrameSection frameSection)
{
    if (window == nullptr)
        return -1;

    int command = -1;

    switch (frameSection)
    {
    case Qt::LeftSection:
        command = SC_SIZE | 0x0001;
        break;
    case Qt::RightSection:
        command = SC_SIZE | 0x0002;
        break;
    case Qt::BottomSection:
        command = SC_SIZE | 0x0006;
        break;
    case Qt::TopSection:
        command = SC_SIZE | 0x0003;
        break;
    case Qt::TopLeftSection:
        command = SC_SIZE | 0x0004;
        break;
    case Qt::BottomRightSection:
        command = SC_SIZE | 0x0008;
        break;
    case Qt::TopRightSection:
        command = SC_SIZE | 0x0005;
        break;
    case Qt::BottomLeftSection:
        command = SC_SIZE | 0x0007;
        break;
    case Qt::TitleBarArea:
        if (event != nullptr && event->type() == QEvent::MouseButtonDblClick)
        {
            if (event->button() == Qt::LeftButton)
            {
                if (window->isMaximized())
                    command = SC_RESTORE;
                else
                    command = SC_MAXIMIZE;
            }
        }
        else
            command = SC_MOVE | 0x0002;

        break;
    default:
        Q_ASSERT(false);
        break;
    }
    return command;
}

bool APROCH_PRE_NAMESPACE(aproch_sys_command_win)(QWidget *window, int command, const QPoint &pos, bool synchronize)
{
    if (window == nullptr)
        return false;
    HWND winId = (HWND)window->effectiveWinId();
    QApplication::processEvents();
    ReleaseCapture();
    if (!IsWindow(winId))
        return false;

    if (synchronize)
    {
        RECT r;
        GetWindowRect(winId, &r);
        QPoint offset = QCursor::pos() - pos;
        MoveWindow(winId, r.left + offset.x(), r.top + offset.y(), RectWidth(r), RectHeight(r), TRUE);
    }
    int ret = SendMessageW(winId, WM_SYSCOMMAND, (WPARAM)command, MAKELPARAM(pos.x(), pos.y()));
    return ret == 0;
}

/* AImageHDC */
AImageHDC::AImageHDC(int width, int height, QImage::Format format)
    : m_hdc(aproch_createDC()), m_bitmap(nullptr), m_null_bitmap(nullptr)
{
    if (width != 0 && height != 0)
    {
        uchar *bits;
        m_bitmap = aproch_createDIB(m_hdc, width, height, format, &bits);
        m_null_bitmap = static_cast<HBITMAP>(SelectObject(m_hdc, m_bitmap));
        m_image = QImage(bits, width, height, format);
    }
    else
    {
        m_image = QImage(width, height, format);
    }

    GdiFlush();
}

AImageHDC::~AImageHDC()
{
    if (m_hdc)
    {
        if (m_bitmap)
        {
            if (m_null_bitmap)
                SelectObject(m_hdc, m_null_bitmap);
            DeleteObject(m_bitmap);
        }
        DeleteDC(m_hdc);
    }
}

QImage::Format AImageHDC::systemFormat()
{
    static int depth = -1;
    if (depth == -1)
    {
        if (HDC defaultDC = GetDC(0))
        {
            depth = GetDeviceCaps(defaultDC, BITSPIXEL);
            ReleaseDC(0, defaultDC);
        }
        else
        {
            depth = 32;
        }
    }
    return depth == 16 ? QImage::Format_RGB16 : QImage::Format_ARGB32_Premultiplied;
}

/* WindowsHook */
AWindowsHook::AWindowsHook()
    : m_oldProc(nullptr), m_winId(nullptr), m_winEventHook(nullptr)
{
    QLibrary user32Lib(QString::fromLatin1("user32"));

    m_setWinEventProc = (LPFNSETWINEVENTHOOK)user32Lib.resolve("SetWinEventHook");
    m_unhookWinEventProc = (LPFNUNHOOKWINEVENT)user32Lib.resolve("UnhookWinEvent");
    Q_ASSERT(m_setWinEventProc != nullptr && m_unhookWinEventProc != nullptr);
}
AWindowsHook::~AWindowsHook()
{
    setWinId(0);
}

HWND AWindowsHook::winId() const
{
    return m_winId;
}

#define kParentWndProc L"Egl::SurfaceParentWndProc"

void AWindowsHook::setWinId(HWND winId)
{
    LONG_PTR hookWindowProc = reinterpret_cast<LONG_PTR>(AWindowsHookManager::HookWindowProc);

    if (m_winId != 0)
    {
        LONG_PTR eglWndFunc = reinterpret_cast<LONG_PTR>(GetPropW(m_winId, kParentWndProc));
        if (hookWindowProc == eglWndFunc)
        {
            // See SurfaceD3D::unsubclassWindow() for the details.
            SetPropW(m_winId, kParentWndProc, reinterpret_cast<HANDLE>(m_oldProc));
        }
        else
        {
            if (IsWindowUnicode(m_winId))
                SetWindowLongPtrW(m_winId, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_oldProc));
            else
                SetWindowLongPtrA(m_winId, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_oldProc));
        }

        m_oldProc = nullptr;

        if (m_winEventHook != nullptr)
        {
            m_unhookWinEventProc(m_winEventHook);
            m_winEventHook = nullptr;
        }
    }

    m_winId = winId;
    if (m_winId == 0)
        return;

    if (IsWindowUnicode(m_winId))
    {
        m_oldProc = (WNDPROC)GetWindowLongPtrW(m_winId, GWLP_WNDPROC);
        SetWindowLongPtrW(m_winId, GWLP_WNDPROC, hookWindowProc);
    }
    else
    {
        m_oldProc = (WNDPROC)GetWindowLongPtrA(m_winId, GWLP_WNDPROC);
        SetWindowLongPtrA(m_winId, GWLP_WNDPROC, hookWindowProc);
    }

    Q_ASSERT(m_winEventHook == nullptr);
#if 0
    if (m_winEventHook == nullptr)
        m_winEventHook = m_setWinEventProc(EVENT_OBJECT_REORDER, EVENT_OBJECT_REORDER, nullptr, &WindowsHookManager::HookWindowEventProc, GetCurrentProcessId(), 0, 0);
#endif
}

LRESULT AWindowsHook::OldWindowProc(HWND hWnd, uint nMessage, WPARAM wParam, LPARAM lParam)
{
    return ::CallWindowProc(m_oldProc, hWnd, nMessage, wParam, lParam);
}

LRESULT AWindowsHook::winProc(HWND hWnd, uint nMessage, WPARAM wParam, LPARAM lParam)
{
    Q_ASSERT(winId() == hWnd);
    return OldWindowProc(hWnd, nMessage, wParam, lParam);
}

void AWindowsHook::winEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
    Q_ASSERT(winId() == hWnd);
    Q_UNUSED(hWinEventHook);
    Q_UNUSED(event);
    Q_UNUSED(hWnd);
    Q_UNUSED(idObject);
    Q_UNUSED(idChild);
    Q_UNUSED(dwEventThread);
    Q_UNUSED(dwmsEventTime);
}

/* WindowsHookManager */
AWindowsHookManager::AWindowsHookManager()
{
}

AWindowsHookManager::~AWindowsHookManager()
{
    Q_ASSERT(m_hookHash.size() == 0);
    // removeAll();
}

AWindowsHookManager *AWindowsHookManager::instance()
{
    static AWindowsHookManager manager;
    return &manager;
}

LRESULT CALLBACK AWindowsHookManager::HookWindowProc(HWND hWnd, uint message, WPARAM wParam, LPARAM lParam)
{
    AWindowsHook *hook = AWindowsHookManager::instance()->hookByWinId(hWnd);
    if (hook != nullptr)
        return hook->winProc(hWnd, message, wParam, lParam);
    return S_FALSE;
}

void CALLBACK AWindowsHookManager::HookWindowEventProc(HWINEVENTHOOK hWinEventHook,
                                                      DWORD event, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
    AWindowsHook *hook = AWindowsHookManager::instance()->hookByWinId(hWnd);
    if (hook != nullptr)
        hook->winEventProc(hWinEventHook, event, hWnd, idObject, idChild, dwEventThread, dwmsEventTime);
}

void AWindowsHookManager::addHook(HWND hWnd, AWindowsHook *hook)
{
    if (m_hookHash.contains(hWnd))
    {
        Q_ASSERT(false); // Can't add hook twice.
        return;
    }
    hook->setWinId(hWnd);
    m_hookHash.insert(hWnd, hook);
}

void AWindowsHookManager::removeHook(AWindowsHook *hook)
{
    if (hook == nullptr || !m_hookHash.contains(hook->winId()))
    {
        Q_ASSERT(false); ////Can't remove hook twice.
        return;
    }
    m_hookHash.remove(hook->winId());
    hook->setWinId(0);
}

AWindowsHook *AWindowsHookManager::hookByWinId(HWND hWnd)
{
    if (!m_hookHash.contains(hWnd))
        return nullptr;
    return m_hookHash[hWnd];
}

QPixmap APROCH_PRE_NAMESPACE(aproch_createPixmap)(HBITMAP bitmap, int hbitmapFormat)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    return qt_pixmapFromWinHBITMAP(bitmap, hbitmapFormat);
#else
    return QPixmap::fromWinHBITMAP(bitmap, (QPixmap::HBitmapFormat)hbitmapFormat);
#endif
}

HBITMAP APROCH_PRE_NAMESPACE(aproch_createHBITMAP)(const QPixmap &pixmap, int hbitmapFormat)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    return qt_pixmapToWinHBITMAP(pixmap, hbitmapFormat);
#else
    return pixmap.toWinHBITMAP((QPixmap::HBitmapFormat)hbitmapFormat);
#endif
}

HICON APROCH_PRE_NAMESPACE(aproch_createHICON)(const QIcon &icon, QSize &inOutSize)
{
    QPixmap pixmap = icon.pixmap(inOutSize);
    inOutSize = pixmap.size();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    return qt_pixmapToWinHICON(pixmap);
#else
    return pixmap.toWinHICON();
#endif
}

HDC APROCH_PRE_NAMESPACE(aproch_getDC)(QPainter *painter)
{
    HDC hdc = 0;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#ifdef A_PRIVATE_HEADERS
    QPaintDevice *paintDevice = painter->device();
    if (paintDevice->devType() != QInternal::Widget)
        return nullptr;
    QWidget *widget = static_cast<QWidget *>(paintDevice);
    QPlatformNativeInterface *nativeInterface = QGuiApplication::platformNativeInterface();
    QBackingStore *backingStore = widget->backingStore();
    hdc = static_cast<HDC>(nativeInterface->nativeResourceForBackingStore(QByteArrayLiteral("getDC"), backingStore));
#else
    Q_UNUSED(painter);
#endif
#else
    QPaintEngine *engine = painter->paintEngine();
    hdc = engine != nullptr ? engine->getDC() : 0;
#endif
    return hdc;
}

void APROCH_PRE_NAMESPACE(aproch_releaseDC)(QPainter *painter, HDC dc)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    Q_UNUSED(painter);
    Q_UNUSED(dc);
#else
    QPaintEngine *engine = painter->paintEngine();
    engine != nullptr ? engine->releaseDC(dc) : 0;
#endif
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
static LOGFONTW qfont_to_LOGFONT(const QFont &f)
{
    LOGFONTW lf;
    memset(&lf, 0, sizeof(LOGFONTW));

    lf.lfHeight = -qRound((double)f.pixelSize());
    lf.lfWidth = 0;
    lf.lfEscapement = 0;
    lf.lfOrientation = 0;
    if (f.weight() == 50)
        lf.lfWeight = FW_DONTCARE;
    else
        lf.lfWeight = (f.weight() * 900) / 99;
    lf.lfItalic = f.style() != QFont::StyleNormal;
    lf.lfCharSet = DEFAULT_CHARSET;

    int strat = OUT_DEFAULT_PRECIS;
    if (f.styleStrategy() & QFont::PreferBitmap)
    {
        strat = OUT_RASTER_PRECIS;
#ifndef Q_OS_WINCE
    }
    else if (f.styleStrategy() & QFont::PreferDevice)
    {
        strat = OUT_DEVICE_PRECIS;
    }
    else if (f.styleStrategy() & QFont::PreferOutline)
    {
        strat = OUT_OUTLINE_PRECIS;
    }
    else if (f.styleStrategy() & QFont::ForceOutline)
    {
        strat = OUT_TT_ONLY_PRECIS;
#endif
    }

    lf.lfOutPrecision = strat;

    int qual = DEFAULT_QUALITY;

    if (f.styleStrategy() & QFont::PreferMatch)
        qual = DRAFT_QUALITY;
#ifndef Q_OS_WINCE
    else if (f.styleStrategy() & QFont::PreferQuality)
        qual = PROOF_QUALITY;
#endif

    if (f.styleStrategy() & QFont::PreferAntialias)
    {
        if (!aproch_isWindowsXP() /* TODO */)
        {
            qual = CLEARTYPE_QUALITY;
        }
        else
        {
            qual = ANTIALIASED_QUALITY;
        }
    }
    else if (f.styleStrategy() & QFont::NoAntialias)
    {
        qual = NONANTIALIASED_QUALITY;
    }

    lf.lfQuality = qual;

    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;

    int hint = FF_DONTCARE;
    switch (f.styleHint())
    {
    case QFont::Helvetica:
        hint = FF_SWISS;
        break;
    case QFont::Times:
        hint = FF_ROMAN;
        break;
    case QFont::Courier:
        hint = FF_MODERN;
        break;
    case QFont::OldEnglish:
        hint = FF_DECORATIVE;
        break;
    case QFont::System:
        hint = FF_MODERN;
        break;
    default:
        break;
    }

    lf.lfPitchAndFamily = DEFAULT_PITCH | hint;

    QString fam = f.family();

    if (fam.isEmpty())
        fam = QStringLiteral("MS Sans Serif");

    if ((fam == QStringLiteral("MS Sans Serif")) && (f.style() == QFont::StyleItalic || (-lf.lfHeight > 18 && -lf.lfHeight != 24)))
    {
        fam = QStringLiteral("Arial"); // MS Sans Serif has bearing problems in italic, and does not scale
    }
    if (fam == QStringLiteral("Courier") && !(f.styleStrategy() & QFont::PreferBitmap))
        fam = QStringLiteral("Courier New");

    memcpy(lf.lfFaceName, fam.utf16(), sizeof(wchar_t) * qMin(fam.length() + 1, 32)); // 32 = Windows hard-coded

    return lf;
}
#endif

HFONT APROCH_PRE_NAMESPACE(aproch_get_qfont_handle)(const QFont &f)
{
    Q_ASSERT(false); // Does not work under Qt5 :((
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    LOGFONTW lf = qfont_to_LOGFONT(f);
    return CreateFontIndirectW(&lf);
#else
    return f.handle();
#endif
}

void APROCH_PRE_NAMESPACE(aproch_release_qfont_handle)(HFONT hfont)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    DeleteObject(hfont);
#else
    Q_UNUSED(hfont);
#endif
}

void APROCH_PRE_NAMESPACE(aproch_setWidgetPostion)(QWidget *widget, const QPoint &position)
{
    MoveWindow((HWND)widget->internalWinId(), position.x(), position.y(), widget->width(), widget->height(), false);
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
int APROCH_PRE_NAMESPACE(aproch_getNativeWindowDPI)(const QWidget *window)
{
    UINT dpi = 96;
    if (QCoreApplication::instance()->testAttribute(Qt::AA_Use96Dpi))
        return dpi;

    if (pAdjustWindowRectExForDpi != nullptr)
    {
        QWidget *wd = (QWidget *)window;
        if (wd != nullptr)
        {
            if (QWidget *topLevel = window->window())
                wd = topLevel;
        }

        HWND winId = wd != nullptr ? (HWND)wd->internalWinId() : 0;
        if (winId != 0 && IsWindow(winId))
            dpi = pGetDpiForWindow(winId);
        else if (pGetSystemDpiForProcess != nullptr)
            dpi = pGetSystemDpiForProcess(GetCurrentProcess());
        else
            dpi = pGetDpiForSystem();
    }
    else
    {
        QScreen *screen = nullptr;
        if (window)
        {
            if (QWidget *topLevel = window->window())
            {
                if (QWindow *topLevelWindow = topLevel->windowHandle())
                    screen = topLevelWindow->screen();
            }
        }
        if (!screen && QGuiApplication::primaryScreen())
            screen = QGuiApplication::primaryScreen();
        dpi = qRound(screen->logicalDotsPerInchX());
    }
    return dpi;
}
#else
static double logPixel = 96.0;
int APROCH_PRE_NAMESPACE(aproch_getNativeWindowDPI)(const QWidget *window)
{
    Q_UNUSED(window);
#ifdef Q_OS_WIN
    HDC displayDC = GetDC(0);
    int currentlogPixel = GetDeviceCaps(displayDC, LOGPIXELSY);
    ReleaseDC(0, displayDC);
    return currentlogPixel;
#endif /* Q_OS_WIN*/
    // PI has not been initialised, or it is being initialised. Give a default dpi
    return static_cast<int>(logPixel);
}
#endif

QMargins APROCH_PRE_NAMESPACE(aproch_getWindowFrameMargings)(const QWidget *window)
{
    RECT rect = {0, 0, 0, 0};
    DWORD style = WS_CAPTION | WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
    DWORD exStyle = WS_EX_WINDOWEDGE;

    if (pAdjustWindowRectExForDpi != nullptr)
    {
        UINT dpi = aproch_getNativeWindowDPI(window);
        if (!pAdjustWindowRectExForDpi(&rect, style, FALSE, exStyle, dpi))
        {
            const DWORD errorCode = GetLastError();
            if (errorCode != NO_ERROR)
                qErrnoWarning(int(errorCode), "%s: pAdjustWindowRectExForDpi() failed", __FUNCTION__);
            Q_ASSERT(false);
        }
    }
    else
    {
        if (!AdjustWindowRectEx(&rect, style, FALSE, exStyle))
        {
            const DWORD errorCode = GetLastError();
            if (errorCode != NO_ERROR)
                qErrnoWarning(int(errorCode), "%s: AdjustWindowRectEx() failed", __FUNCTION__);
            Q_ASSERT(false);
        }
    }

    return QMargins(qAbs(rect.left), qAbs(rect.top), qAbs(rect.right), qAbs(rect.bottom));
}

bool APROCH_PRE_NAMESPACE(aproch_getWindows10AcrylicSupport)()
{
#if 0
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    QOperatingSystemVersion version = QOperatingSystemVersion::current();
    if (version < QOperatingSystemVersion::Windows10)
        return false;
    if (version > QOperatingSystemVersion::Windows10 ||
        (version.microVersion() >= 16190 && version.microVersion() < 17134))
        return true;
#endif
#endif
    return false;
}

bool APROCH_PRE_NAMESPACE(aproch_setBlurBehindWindow)(QWidget *widget, bool enabled)
{
    QWidget *window = widget->isWindow() ? widget : widget->nativeParentWidget();
    if (window == nullptr)
        return false;

    window->setProperty("aproch_blurBehind", enabled);

    if (!aproch_blurBehindSupported())
        return false;

    bool result = false;

    if (pSetWindowCompositionAttribute != nullptr)
    {
        DWORD color_value = 0x01FFFFFF;
        static bool _acrylic = aproch_getWindows10AcrylicSupport();
        if (!_acrylic)
        {
            QColor color = aproch_getColorizationColor();
            color_value = RGB(
                qRound(color.red() * ((double)color.alpha() / 255) + 255 - color.alpha()),
                qRound(color.green() * ((double)color.alpha() / 255) + 255 - color.alpha()),
                qRound(color.blue() * ((double)color.alpha() / 255) + 255 - color.alpha()));
        }

        ACCENTPOLICY policy = {ACCENT_DISABLE, 0, color_value, 0};
        if (enabled)
            policy.nAccentState = _acrylic ? ACCENT_ENABLE_ACRYLIC_BLURBEHIND : ACCENT_ENABLE_BLURBEHIND;
        WINCOMPATTRDATA data = {A_WCA_ACCENT_POLICY, &policy, sizeof(ACCENTPOLICY)};
        result = pSetWindowCompositionAttribute((HWND)window->winId(), &data);
    }
    else if (pDwmEnableBlurBehindWindow != nullptr)
    {
        DWM_BLURBEHIND bb;
        ZeroMemory(&bb, sizeof(DWM_BLURBEHIND));

        bb.dwFlags = DWM_BB_ENABLE;
        bb.fEnable = true;
        bb.hRgnBlur = NULL;
        result = SUCCEEDED(pDwmEnableBlurBehindWindow((HWND)window->winId(), &bb));
    }

    return result;
}

bool APROCH_PRE_NAMESPACE(aproch_setWindowColorization)(QWidget *widget, const QColor &color)
{
    QWidget *window = widget->isWindow() ? widget : widget->nativeParentWidget();
    if (window == nullptr)
        return false;

    bool result = false;

    if (pSetWindowCompositionAttribute != nullptr)
    {
        DWORD color_value = RGB(
            qRound(color.red() * ((double)color.alpha() / 255) + 255 - color.alpha()),
            qRound(color.green() * ((double)color.alpha() / 255) + 255 - color.alpha()),
            qRound(color.blue() * ((double)color.alpha() / 255) + 255 - color.alpha()));

        AccentTypes accent = ACCENT_ENABLE_GRADIENT; // ACCENT_ENABLE_TRANSPARENTGRADIENT;
        ACCENTPOLICY policy = {accent, 2, color_value, 0};
        WINCOMPATTRDATA data = {A_WCA_ACCENT_POLICY, &policy, sizeof(ACCENTPOLICY)};
        result = pSetWindowCompositionAttribute((HWND)window->winId(), &data);
    }
    return result;
}

bool APROCH_PRE_NAMESPACE(aproch_getDesktopLightTheme)()
{
    // TODO
    // NSString *osxMode = [[NSUserDefaults standardUserDefaults] stringForKey:@"AppleInterfaceStyle"];
    // f osxMode is nil then it isn't in dark mode, but if osxMode is @"Dark" then it is in dark mode.

    QSettings settings(QStringLiteral("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows"), QSettings::NativeFormat);
    if (settings.contains(QStringLiteral("CurrentVersion/Themes/Personalize/AppsUseLightTheme")))
    {
        DWORD theme = settings.value(QStringLiteral("CurrentVersion/Themes/Personalize/AppsUseLightTheme")).toInt();
        if (theme == 1)
            return true;
    }
    return false;
}

static QColor aproch_colorToQtColor(DWORD color, DWORD balance)
{
    if (balance > 100)
        balance = 100;

    BYTE ALPHA = 255 * balance / 100; // Convert from 0-100 to 0-255

    int red = qRed(color) * ALPHA / 255 * 1.135;
    red = qMin(red, 255);

    int green = qGreen(color) * ALPHA / 255 * 1.135;
    green = qMin(green, 255);

    int blue = qBlue(color) * ALPHA / 255 * 1.135;
    blue = qMin(blue, 255);
#if 0
    BYTE RATIO = 0xD9;
    red = ((red * ALPHA) + (RATIO * (255 - ALPHA))) / 255;
    green = ((green * ALPHA) + (RATIO * (255 - ALPHA))) / 255;
    blue = ((blue * ALPHA) + (RATIO * (255 - ALPHA))) / 255;
#endif
    return QColor(red, green, blue);
}

static QColor aproch_dwm_getColorizationColor()
{
    aproch_initializeDrawingResources();
    if (!pDwmGetColorizationColor)
        return QColor(Qt::darkGray);

    DWORD colorizationColor;
    BOOL opaqueBlend = FALSE;
    pDwmGetColorizationColor(&colorizationColor, &opaqueBlend);
    return QColor(qRed(colorizationColor), qGreen(colorizationColor), qBlue(colorizationColor), qAlpha(colorizationColor));
}

static QColor aproch_dwm_getColorizationParameters()
{
    aproch_initializeDrawingResources();
    if (!pDwmGetColorizationParameters)
        return QColor(Qt::darkGray);

    DWMCOLORIZATIONPARAMS dwmcolor;
    pDwmGetColorizationParameters(&dwmcolor);
    return QColor(qRed(dwmcolor.dwColor), qGreen(dwmcolor.dwColor), qBlue(dwmcolor.dwColor), qAlpha(dwmcolor.dwColor));
}

void APROCH_PRE_NAMESPACE(aproch_initTitleBarPalette)(QPalette &palette)
{
    QSettings settings(QStringLiteral("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows"), QSettings::NativeFormat);
    const bool hasColorPrevalenceSetting = settings.contains(QStringLiteral("DWM/ColorPrevalence"));
    const bool hasColorizationSetting = settings.contains(QStringLiteral("DWM/ColorizationColor")) /* &&
         settings.value(QStringLiteral("DWM/EnableWindowColorization"), true).toBool()*/
        ;

    // Title bar has colorization background. Setting was added in Windows 10.
    const bool colorizationBackground = hasColorPrevalenceSetting && settings.value(QStringLiteral("DWM/ColorPrevalence"), true).toBool();
    const bool isWindows7 = aproch_isWindows7();
    const bool isWindows8 = aproch_isWindows8();
    QColor bgcolor = Qt::gray;
    QColor fgcolor = Qt::white;
    QColor frameColor = Qt::darkGray;
    if (hasColorizationSetting)
    {
        if (isWindows7 && aproch_isCompositionAvailable())
        {
            bgcolor = aproch_dwm_getColorizationParameters();
            frameColor = bgcolor;
            fgcolor = Qt::white;
        }
        else if (isWindows7)
        {
            COLORREF clrActiveCaption = ::GetSysColor(COLOR_ACTIVECAPTION);
            bgcolor = QColor(GetRValue(clrActiveCaption), GetGValue(clrActiveCaption), GetBValue(clrActiveCaption));
            frameColor = bgcolor;
            fgcolor = Qt::white;
        }
        else if (!hasColorPrevalenceSetting && isWindows8)
        {
            bgcolor = palette.color(QPalette::Active, QPalette::Window);
            bgcolor.setAlpha(0);
            fgcolor = palette.color(QPalette::Active, QPalette::WindowText);
            frameColor = bgcolor;
        }
        else if (colorizationBackground)
        {
            DWORD color = settings.value(QStringLiteral("DWM/ColorizationColor")).toInt();
            DWORD balance = settings.value(QStringLiteral("DWM/ColorizationColorBalance"), 100).toInt();
            bgcolor = aproch_colorToQtColor(color, balance);
            frameColor = bgcolor;
            if (aproch_isColorLight(bgcolor))
                fgcolor = Qt::black;
            else
                fgcolor = Qt::white;
        }
        else
        {
            bgcolor = Qt::white;
            fgcolor = Qt::black;
            frameColor = Qt::darkGray;
        }
    }
    palette.setColor(QPalette::Active, QPalette::Window, bgcolor);
    palette.setColor(QPalette::Active, QPalette::WindowText, fgcolor);
    palette.setColor(QPalette::Active, QPalette::ButtonText, fgcolor);
    palette.setColor(QPalette::Active, QPalette::Shadow, frameColor);

    bgcolor = Qt::white;
    fgcolor = Qt::gray;
    frameColor = Qt::darkGray;

    if (hasColorizationSetting)
    {
        if (aproch_isCompositionAvailable() && aproch_isWindows7())
        {
            bgcolor = aproch_dwm_getColorizationColor();
            frameColor = bgcolor;
            fgcolor = Qt::white;
        }
        else if (isWindows7)
        {
            COLORREF clrInactiveCaption = ::GetSysColor(COLOR_INACTIVECAPTION);
            bgcolor = QColor(GetRValue(clrInactiveCaption), GetGValue(clrInactiveCaption), GetBValue(clrInactiveCaption));
            frameColor = bgcolor;
            fgcolor = Qt::white;
        }
        else if (!hasColorPrevalenceSetting && isWindows8)
        {
            bgcolor = palette.color(QPalette::Inactive, QPalette::Window);
            bgcolor.setAlpha(0);
            fgcolor = palette.color(QPalette::Inactive, QPalette::WindowText);
            frameColor = bgcolor;
        }
        else if (false /*colorizationBackground*/)
        {
            DWORD color = settings.value(QStringLiteral("DWM/ColorizationAfterglow")).toInt();
            DWORD balance = settings.value(QStringLiteral("DWM/ColorizationAfterglowBalance"), 100).toInt();
            bgcolor = aproch_colorToQtColor(color, balance);
            frameColor = bgcolor;
            if (aproch_isColorLight(bgcolor))
                fgcolor = Qt::black;
            else
                fgcolor = Qt::white;
        }
        else
        {
            bgcolor = Qt::white;
            fgcolor = Qt::lightGray;
            frameColor = Qt::gray;
        }
    }
    palette.setColor(QPalette::Inactive, QPalette::Window, bgcolor);
    palette.setColor(QPalette::Inactive, QPalette::WindowText, fgcolor);
    palette.setColor(QPalette::Inactive, QPalette::ButtonText, fgcolor);
    palette.setColor(QPalette::Inactive, QPalette::Shadow, frameColor);
}

QColor APROCH_PRE_NAMESPACE(aproch_getColorizationColor)()
{
    QSettings settings(QStringLiteral("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows"), QSettings::NativeFormat);
    if (settings.contains(QStringLiteral("DWM/ColorizationColor")))
    {
        if (settings.contains(QStringLiteral("DWM/ColorPrevalence")))
        {
            // Setting was added in Windows 10
            if (!settings.value(QStringLiteral("DWM/ColorPrevalence")).toBool())
                return Qt::transparent;
        }
        if (!settings.value(QStringLiteral("DWM/EnableWindowColorization")).toBool())
            return Qt::transparent;

        DWORD colorizationColor = settings.value(QStringLiteral("DWM/ColorizationColor")).toInt();
        DWORD colorizationColorBalance = settings.value(QStringLiteral("DWM/ColorizationColorBalance")).toInt();
        if (colorizationColorBalance > 100)
            colorizationColorBalance = 100;
        BYTE ALPHA = 255 * colorizationColorBalance / 100; // Convert from 0-100 to 0-255

        int r = ((qRed(colorizationColor) * ALPHA) + (0xD9 * (255 - ALPHA))) / 255;
        int g = ((qGreen(colorizationColor) * ALPHA) + (0xD9 * (255 - ALPHA))) / 255;
        int b = ((qBlue(colorizationColor) * ALPHA) + (0xD9 * (255 - ALPHA))) / 255;
        return QColor(r, g, b);
    }

    return aproch_dwm_getColorizationColor();
}

bool APROCH_PRE_NAMESPACE(aproch_isWindowsXP())
{
    static const bool windowsXP =
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
        QOperatingSystemVersion::current() < QOperatingSystemVersion::Windows7;
#else
#if defined(Q_OS_WIN)
        QSysInfo::windowsVersion() <= QSysInfo::WV_XP;
#else
        false;
#endif
#endif
    return windowsXP;
}

bool APROCH_PRE_NAMESPACE(aproch_isWindows7())
{
    static const bool windows7 =
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
        QOperatingSystemVersion::current() <= QOperatingSystemVersion::Windows7;
#else
#if defined(Q_OS_WIN)
        QSysInfo::windowsVersion() <= QSysInfo::WV_WINDOWS7;
#else
        false;
#endif
#endif
    return windows7;
}

static bool aproch_isWindows8()
{
    static const bool windows8 =
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
        QOperatingSystemVersion::current() >= QOperatingSystemVersion::Windows8 &&
        QOperatingSystemVersion::current() <= QOperatingSystemVersion::Windows8_1;
#else
#if defined(Q_OS_WIN)
        QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS8 &&
        QSysInfo::windowsVersion() <= QSysInfo::WV_WINDOWS8_1;
#else
        false;
#endif
#endif
    return windows8;
}

bool APROCH_PRE_NAMESPACE(aproch_isWindows10())
{
    static const bool windows10 =
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
        QOperatingSystemVersion::current() >= QOperatingSystemVersion::Windows10;
#else
#if defined(Q_OS_WIN)
        QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS10;
#else
        false;
#endif
#endif
    return windows10;
}

void APROCH_PRE_NAMESPACE(aproch_paintAirEffect)(QPainter *painter, const QRect &rect, const QRegion &clip)
{
    QPaintDevice *paintDevice = painter->device();
    if (!aproch_isCompositionAvailable() || paintDevice->devType() != QInternal::Widget)
        return;

    QWidget *widget = static_cast<QWidget *>(paintDevice);
    QWidget *window = widget->isWindow() ? widget : widget->nativeParentWidget();
    if (window == nullptr)
        return;
    HDC hdc = aproch_getDC(painter);
    if (hdc == nullptr)
        return;

    QPoint offset = widget->mapTo(window, QPoint(static_cast<int>(painter->worldTransform().dx()),
                                                 static_cast<int>(painter->worldTransform().dy())));

#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
    for (QRegion::const_iterator it = clip.begin(); it != clip.end(); ++it)
#else
    QVector<QRect> rects = clip.rects();
    for (QVector<QRect>::const_iterator it = rects.constBegin(); it != rects.constEnd(); ++it)
#endif
    {
        QRect r = (*it).intersected(rect);
        if (r.isEmpty())
            continue;
        r.translate(offset);
        RECT rc;
        rc.left = r.left();
        rc.top = r.top();
        rc.right = r.right() + 1;
        rc.bottom = r.bottom() + 1;
        COLORREF old = ::SetBkColor(hdc, RGB(0, 0, 0));
        ::ExtTextOutW(hdc, 0, 0, ETO_OPAQUE, &rc, nullptr, 0, nullptr);
        ::SetBkColor(hdc, old);
    }

    aproch_releaseDC(painter, hdc);
}

static bool aproch_getNonClientMetrics(NONCLIENTMETRICSW &ncm)
{
    ZeroMemory(&ncm, sizeof(NONCLIENTMETRICSW));
    ncm.cbSize = sizeof(NONCLIENTMETRICSW);
    return ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICSW), &ncm, 0);
}

static LOGFONTW aproch_getCaptionLogFont(HANDLE hTheme)
{
    LOGFONTW lf = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}};
    HRESULT result = S_FALSE;
    if (hTheme)
        result = pGetThemeSysFont(hTheme, HLP_TMT_CAPTIONFONT, &lf);

    if (!SUCCEEDED(result))
    {
        NONCLIENTMETRICSW ncm;
        aproch_getNonClientMetrics(ncm);
        lf = ncm.lfCaptionFont;
    }
    return lf;
}

static HFONT aproch_createDwmCaptionFont(HANDLE hTheme)
{
    LOGFONTW lf = aproch_getCaptionLogFont(hTheme);
    return ::CreateFontIndirectW(&lf);
}

void APROCH_PRE_NAMESPACE(aproch_paintTitleBarText)(QPainter *painter, const QString &text, const QRect &rect, bool active, const QColor &color)
{
    if (!rect.isValid())
        return;

    HANDLE hTheme = pOpenThemeData(GetDesktopWindow(), L"WINDOW");
    if (!hTheme)
        return;

    AImageHDC imagehdc(rect.width(), rect.height(), AImageHDC::systemFormat());

    HFONT hCaptionFont = aproch_createDwmCaptionFont(hTheme);
    HFONT hOldFont = (HFONT)SelectObject(imagehdc.hdc(), (HGDIOBJ)hCaptionFont);

    const UINT uFormat = DT_VCENTER | DT_CENTER | DT_END_ELLIPSIS | DT_SINGLELINE | DT_NOPREFIX;
    RECT rctext = {0, 0, rect.width(), rect.height()};

    HLP_DTTOPTS dto;
    memset(&dto, 0, sizeof(HLP_DTTOPTS));
    dto.dwSize = sizeof(HLP_DTTOPTS);
    dto.dwFlags = HLP_DTT_COMPOSITED | HLP_DTT_GLOWSIZE;
    dto.iGlowSize = 5; // glowSize();

    if (color.isValid())
    {
        dto.iGlowSize = 0;
        dto.dwFlags |= HLP_DTT_TEXTCOLOR;
        dto.crText = RGB(color.red(), color.green(), color.blue());
    }

    HRESULT result = pDrawThemeTextEx(hTheme, imagehdc.hdc(),
                                      false ? HLP_WP_MAXCAPTION : HLP_WP_CAPTION, active ? HLP_FS_ACTIVE : HLP_FS_INACTIVE,
                                      (LPCWSTR)text.utf16(), -1, uFormat, &rctext, &dto);
    Q_UNUSED(result)

    SelectObject(imagehdc.hdc(), (HGDIOBJ)hOldFont);
    DeleteObject(hCaptionFont);

    painter->drawImage(rect, imagehdc.image());
}

QPixmap APROCH_PRE_NAMESPACE(aproch_getTitleBarIcon)(QWidget *widget)
{
    HICON hIcon = (HICON)(DWORD_PTR)::SendMessageW((HWND)widget->winId(), WM_GETICON, ICON_SMALL, 0);
    if (!hIcon)
        hIcon = (HICON)(DWORD_PTR)::GetClassLongPtrW((HWND)widget->winId(), GCLP_HICONSM);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    return qt_pixmapFromWinHICON(hIcon);
#else
    return QPixmap::fromWinHICON(hIcon);
#endif
}

void APROCH_PRE_NAMESPACE(aproch_paintTitleBarIcon)(QPainter *painter, const QIcon &icon, const QRect &rect)
{
    if (icon.isNull() || !rect.isValid())
        return;

    QSize size = rect.size();
    HICON hIcon = aproch_createHICON(icon, size);
    if (hIcon == nullptr)
        return;
    QRect r = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size, rect);
    AImageHDC imagehdc(rect.width(), rect.height(), AImageHDC::systemFormat());
    DrawIconEx(imagehdc.hdc(), r.left() - rect.left(), r.top() - rect.top(), hIcon, 0, 0, 0, nullptr, DI_NORMAL | DI_COMPAT);
    painter->drawImage(rect, imagehdc.image());
    DestroyIcon(hIcon);
}

static void aproch_add_rect(HRGN &hrgn, const QRect &r)
{
    HRGN temp = CreateRectRgn(r.left(), r.top(), r.x() + r.width(), r.y() + r.height());
    if (temp)
    {
        HRGN dest = CreateRectRgn(0, 0, 0, 0);
        int result = CombineRgn(dest, hrgn, temp, RGN_OR);
        if (result)
        {
            DeleteObject(hrgn);
            hrgn = dest;
        }
        DeleteObject(temp);
    }
}

HRGN APROCH_PRE_NAMESPACE(aproch_create_region)(const QRegion &region)
{
    HRGN rgn = CreateRectRgn(0, 0, 0, 0);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
    for (QRegion::const_iterator it = region.begin(); it != region.end(); ++it)
#else
    QVector<QRect> rcs = region.rects();
    for (QVector<QRect>::const_iterator it = rcs.constBegin(); it != rcs.constEnd(); ++it)
#endif
    {
        QRect r = (*it);
        aproch_add_rect(rgn, r);
    }
    return rgn;
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0)) || !defined(A_PRIVATE_HEADERS)
static QFont::Weight aproch_weightFromInteger(int weight)
{
    if (weight < 400)
        return QFont::Light;
    else if (weight < 600)
        return QFont::Normal;
    else if (weight < 700)
        return QFont::DemiBold;
    else if (weight < 800)
        return QFont::Bold;
    else
        return QFont::Black;
}
#endif

static QFont aproch_LOGFONTtoQFont(const LOGFONTW &lf, int dpi)
{
    QFont result;
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0)) || !defined(A_PRIVATE_HEADERS)
    Q_UNUSED(dpi);
    QString family = QString::fromWCharArray(lf.lfFaceName);
    result = QFont(family);
    result.setItalic(lf.lfItalic);
    if (lf.lfWeight != FW_DONTCARE)
        result.setWeight(aproch_weightFromInteger(lf.lfWeight));
    int lfh = qAbs(lf.lfHeight);
    const HDC displayDC = GetDC(0);
    result.setPointSizeF(lfh * 72.0 / GetDeviceCaps(displayDC, LOGPIXELSY));
    ReleaseDC(0, displayDC);
    result.setUnderline(false);
    result.setOverline(false);
    result.setStrikeOut(false);
#else

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    result = QWindowsFontDatabaseBase::LOGFONT_to_QFont(lf, dpi);
#else
    QPlatformNativeInterface *nativeInterface = QGuiApplication::platformNativeInterface();
    Q_ASSERT(nativeInterface != nullptr);
    bool ret = QMetaObject::invokeMethod(nativeInterface, "logFontToQFont", Qt::DirectConnection,
                                         Q_RETURN_ARG(QFont, result),
                                         Q_ARG(const void *, &lf),
                                         Q_ARG(int, dpi));
    Q_UNUSED(ret);
    Q_ASSERT(ret);
#endif
#endif
    return result;
}

QFont APROCH_PRE_NAMESPACE(aproch_getWindowCaptionFont)(QWidget *window, int dpi)
{
    if (dpi == -1)
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        dpi = window->devicePixelRatio() * window->logicalDpiY();
#else
        dpi = 0;
#endif
    }

    HWND hWnd = nullptr;
    if (!pOpenThemeData)
        return QFont(QApplication::font("QMdiSubWindowTitleBar"), window);

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0)) || !defined(A_PRIVATE_HEADERS)
    hWnd = (HWND)window->winId();
#else
    if (window->windowHandle())
        hWnd = static_cast<HWND>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow(QByteArrayLiteral("handle"), window->windowHandle()));
#endif
    const HANDLE hTheme = hWnd ? pOpenThemeData(hWnd, L"WINDOW") : nullptr;
    if (!hTheme)
        return QFont(QApplication::font("QMdiSubWindowTitleBar"), window);
    const LOGFONTW logFont = aproch_getCaptionLogFont(hTheme);
    return aproch_LOGFONTtoQFont(logFont, dpi);
}

QImage APROCH_PRE_NAMESPACE(aproch_getDesktopImage)(int screen)
{
    Q_UNUSED(screen);
    wchar_t imagePath[MAX_PATH];

    if (!SystemParametersInfoW(SPI_GETDESKWALLPAPER, MAX_PATH, imagePath, 0))
        return QImage();
    QImage ret(QString((const QChar *)&imagePath[0]));
    return ret;
}

DesktopImageAspectStyle APROCH_PRE_NAMESPACE(aproch_getDesktopAspectStyle)(int screen)
{
    Q_UNUSED(screen);
    DesktopImageAspectStyle ret = AspectStyleCentral;
    QSettings settings(QStringLiteral("HKEY_CURRENT_USER\\Control Panel\\Desktop"), QSettings::NativeFormat);
    DWORD wallpaperStyle = settings.contains(QStringLiteral("WallpaperStyle")) ? settings.value(QStringLiteral("WallpaperStyle")).toInt() : 0;
    if (wallpaperStyle == 0)
    {
        if (settings.contains(QStringLiteral("TileWallpaper")) &&
            settings.value(QStringLiteral("TileWallpaper")).toBool())
            ret = AspectStyleTiled;
    }
    else if (wallpaperStyle == 2)
        ret = AspectStyleIgnoreRatio;
    else if (wallpaperStyle == 6)
        ret = AspectStyleKeepRatio;
    else
        ret = AspectStyleKeepRatioByExpanding;
    return ret;
}
