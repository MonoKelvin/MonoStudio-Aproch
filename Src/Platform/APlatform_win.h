/****************************************************************************
 * @file    APlatform_win.h
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

#include <qt_windows.h>
#include <vsstyle.h>
#include <vssym32.h>

#include <QHash>
#include <QPixmap>
#include <QPainter>
#include <qevent.h>

//
// The most of the following code is copied from Qtitan.
//
// Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
// Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
//

QT_BEGIN_NAMESPACE
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
Q_GUI_EXPORT QPixmap qt_pixmapFromWinHBITMAP(HBITMAP, int hbitmapFormat = 0);
Q_GUI_EXPORT HBITMAP qt_pixmapToWinHBITMAP(const QPixmap &p, int hbitmapFormat = 0);
Q_GUI_EXPORT HICON qt_pixmapToWinHICON(const QPixmap &p);
Q_GUI_EXPORT QPixmap qt_pixmapFromWinHICON(HICON icon);
#endif
QT_END_NAMESPACE

#if (_MSC_VER <= 1200) && !defined(_WIN64)
#define GetWindowLongPtrW GetWindowLongW
#define GetWindowLongPtrA GetWindowLongA
#define SetWindowLongPtrW SetWindowLongW
#define SetWindowLongPtrA SetWindowLongA
#endif
#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0
#endif

inline void NormalizeRect(RECT &r)
{
    int nTemp;
    if (r.left > r.right)
    {
        nTemp = r.left;
        r.left = r.right;
        r.right = nTemp;
    }
    if (r.top > r.bottom)
    {
        nTemp = r.top;
        r.top = r.bottom;
        r.bottom = nTemp;
    }
}

inline int RectWidth(const RECT &rIn)
{
    RECT r;
    CopyRect(&r, &rIn);
    NormalizeRect(r);
    return r.right - r.left;
}

inline int RectHeight(const RECT &rIn)
{
    RECT r;
    CopyRect(&r, &rIn);
    NormalizeRect(r);
    return r.bottom - r.top;
}

typedef struct
{
    BITMAPINFOHEADER bmiHeader;
    DWORD redMask;
    DWORD greenMask;
    DWORD blueMask;
} BITMAPINFO_MASK;

// DWM related
typedef struct // MARGINS
{
    int cxLeftWidth;    // width of left border that retains its size
    int cxRightWidth;   // width of right border that retains its size
    int cyTopHeight;    // height of top border that retains its size
    int cyBottomHeight; // height of bottom border that retains its size
} HLP_MARGINS;

#ifndef HIMAGELIST
struct _IMAGELIST;
typedef struct _IMAGELIST *HIMAGELIST;
#endif

typedef struct // DTTOPTS
{
    DWORD dwSize;
    DWORD dwFlags;
    COLORREF crText;
    COLORREF crBorder;
    COLORREF crShadow;
    int eTextShadowType;
    POINT ptShadowOffset;
    int iBorderSize;
    int iFontPropId;
    int iColorPropId;
    int iStateId;
    BOOL fApplyOverlay;
    int iGlowSize;
} HLP_DTTOPTS;

typedef struct
{
    DWORD dwFlags;
    DWORD dwMask;
} HLP_WTA_OPTIONS;

#define HLP_WM_THEMECHANGED 0x031A
#define HLP_WM_DWMCOMPOSITIONCHANGED 0x031E

enum HLP_WINDOWTHEMEATTRIBUTETYPE
{
    HLP_WTA_NONCLIENT = 1
};

#define WTNCA_NODRAWCAPTION 0x00000001
#define WTNCA_NODRAWICON 0x00000002
#define WTNCA_NOSYSMENU 0x00000004
#define WTNCA_NOMIRRORHELP 0x00000008
#define WTNCA_VALIDBITS (WTNCA_NODRAWCAPTION | \
                         WTNCA_NODRAWICON |    \
                         WTNCA_NOSYSMENU |     \
                         WTNCA_NOMIRRORHELP)

#define HLP_DT_CENTER 0x00000001 // DT_CENTER
#define HLP_DT_VCENTER 0x00000004
#define HLP_DT_SINGLELINE 0x00000020
#define HLP_DT_NOPREFIX 0x00000800

enum HLP_NAVIGATIONPARTS // NAVIGATIONPARTS
{
    HLP_NAV_BACKBUTTON = 1,
    HLP_NAV_FORWARDBUTTON = 2,
    HLP_NAV_MENUBUTTON = 3,
};

enum HLP_NAV_BACKBUTTONSTATES // NAV_BACKBUTTONSTATES
{
    HLP_NAV_BB_NORMAL = 1,
    HLP_NAV_BB_HOT = 2,
    HLP_NAV_BB_PRESSED = 3,
    HLP_NAV_BB_DISABLED = 4,
};

#define HLP_AUTOHIDE_LEFT 0x0001
#define HLP_AUTOHIDE_RIGHT 0x0002
#define HLP_AUTOHIDE_TOP 0x0004
#define HLP_AUTOHIDE_BOTTOM 0x0008

#define HLP_TMT_CAPTIONFONT (801) // TMT_CAPTIONFONT

//---- bits used in dwFlags of DTTOPTS ----
#define HLP_DTT_TEXTCOLOR (1UL << 0)     // crText has been specified
#define HLP_DTT_BORDERCOLOR (1UL << 1)   // crBorder has been specified
#define HLP_DTT_SHADOWCOLOR (1UL << 2)   // crShadow has been specified
#define HLP_DTT_SHADOWTYPE (1UL << 3)    // iTextShadowType has been specified
#define HLP_DTT_SHADOWOFFSET (1UL << 4)  // ptShadowOffset has been specified
#define HLP_DTT_BORDERSIZE (1UL << 5)    // iBorderSize has been specified
#define HLP_DTT_FONTPROP (1UL << 6)      // iFontPropId has been specified
#define HLP_DTT_COLORPROP (1UL << 7)     // iColorPropId has been specified
#define HLP_DTT_STATEID (1UL << 8)       // IStateId has been specified
#define HLP_DTT_CALCRECT (1UL << 9)      // Use pRect as and in/out parameter
#define HLP_DTT_APPLYOVERLAY (1UL << 10) // fApplyOverlay has been specified
#define HLP_DTT_GLOWSIZE (1UL << 11)     // iGlowSize has been specified
#define HLP_DTT_CALLBACK (1UL << 12)     // pfnDrawTextCallback has been specified
#define HLP_DTT_COMPOSITED (1UL << 13)   // Draws text with antialiased alpha (needs a DIB section)

#define HLP_WM_NCMOUSELEAVE 674 // WM_NCMOUSELEAVE

#define HLP_WP_CAPTION 1    // WP_CAPTION
#define HLP_WP_MAXCAPTION 5 // WP_MAXCAPTION
#define HLP_CS_ACTIVE 1     // CS_ACTIVE
#define HLP_FS_ACTIVE 1
#define HLP_FS_INACTIVE 2
#define HLP_TMT_FILLCOLORHINT 3821   // TMT_FILLCOLORHINT
#define HLP_TMT_BORDERCOLORHINT 3822 // TMT_BORDERCOLORHINT

#ifndef DCX_USESTYLE
#define DCX_USESTYLE 0x00010000
#endif
#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif

#ifndef HTHEME
typedef HANDLE HTHEME;
#endif

//typedef enum _DWMWINDOWATTRIBUTE
//{
//    DWMWA_NCRENDERING_ENABLED = 1,
//    DWMWA_NCRENDERING_POLICY,
//    DWMWA_TRANSITIONS_FORCEDISABLED,
//    DWMWA_ALLOW_NCPAINT,
//    DWMWA_CAPTION_BUTTON_BOUNDS,
//    DWMWA_NONCLIENT_RTL_LAYOUT,
//    DWMWA_FORCE_ICONIC_REPRESENTATION,
//    DWMWA_FLIP3D_POLICY,
//    DWMWA_EXTENDED_FRAME_BOUNDS,
//    DWMWA_HAS_ICONIC_BITMAP,
//    DWMWA_DISALLOW_PEEK,
//    DWMWA_EXCLUDED_FROM_PEEK,
//    DWMWA_CLOAK,
//    DWMWA_CLOAKED,
//    DWMWA_FREEZE_REPRESENTATION,
//    DWMWA_LAST
//} DWMWINDOWATTRIBUTE;
enum DWMWINDOWATTRIBUTE;

#ifndef WM_DWMCOMPOSITIONCHANGED
#define WM_DWMCOMPOSITIONCHANGED 0x031E
#define WM_DWMNCRENDERINGCHANGED 0x031F
#define WM_DWMCOLORIZATIONCOLORCHANGED 0x0320
#define WM_DWMWINDOWMAXIMIZEDCHANGE 0x0321
#endif

//typedef enum _DWMNCRENDERINGPOLICY
//{
//    DWMNCRP_USEWINDOWSTYLE,
//    DWMNCRP_DISABLED,
//    DWMNCRP_ENABLED,
//    DWMNCRP_LAST
//} DWMNCRENDERINGPOLICY;
enum DWMNCRENDERINGPOLICY;

//typedef struct
//{
//    DWORD dwFlags;
//    BOOL fEnable;
//    HRGN hRgnBlur;
//    BOOL fTransitionOnMaximized;
//} DWM_BLURBEHIND;
struct _DWM_BLURBEHIND;
typedef _DWM_BLURBEHIND DWM_BLURBEHIND;

// A value for the fEnable member has been specified.
#define DWM_BB_ENABLE 0x00000001
// A value for the hRgnBlur member has been specified.
#define DWM_BB_BLURREGION 0x00000002
// A value for the fTransitionOnMaximized member has been specified.
#define DWM_BB_TRANSITIONONMAXIMIZED 0x00000004

enum WIS_UX_BUFFERFORMAT
{
    WIS_BPBF_COMPATIBLEBITMAP, // Compatible bitmap
    WIS_BPBF_DIB,              // Device-independent bitmap
    WIS_BPBF_TOPDOWNDIB,       // Top-down device-independent bitmap
    WIS_BPBF_TOPDOWNMONODIB    // Top-down monochrome device-independent bitmap
};

// BP_PAINTPARAMS
struct WIS_UX_PAINTPARAMS
{
    DWORD cbSize;
    DWORD dwFlags; // BPPF_ flags
    const RECT *prcExclude;
    const BLENDFUNCTION *pBlendFunction;
};

#if 1
#define ABGR(a, b, g, r) ((a & 0xffu) << 24) | ((b & 0xffu) << 16) | ((g & 0xffu) << 8) | (r & 0xffu)

struct ACCENTPOLICY
{
    int nAccentState;
    DWORD nFlags;
    COLORREF nColor;
    DWORD nAnimationId;
};

struct WINCOMPATTRDATA
{
    DWORD nAttribute;
    PVOID pData;
    ULONG ulDataSize;
};

enum AccentTypes
{
    ACCENT_DISABLE = 0,
    ACCENT_ENABLE_GRADIENT = 1,
    ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
    ACCENT_ENABLE_BLURBEHIND = 3,
    ACCENT_ENABLE_ACRYLIC_BLURBEHIND = 4
};

enum AccentFlags
{
    ACCENT_LeftBorder = 0x20,
    ACCENT_TopBorder = 0x40,
    ACCENT_RightBorder = 0x80,
    ACCENT_BottomBorder = 0x100,
    ACCENT_AllBorders = (ACCENT_LeftBorder | ACCENT_TopBorder | ACCENT_RightBorder | ACCENT_BottomBorder)
};

#define A_WCA_ACCENT_POLICY 19

struct DWMCOLORIZATIONPARAMS
{
    DWORD dwColor;
    DWORD dwAfterglow;
    DWORD dwColorBalance;
    DWORD dwAfterglowBalance;
    DWORD dwBlurBalance;
    DWORD dwGlassReflectionIntensity;
    DWORD dwOpaqueBlend;
};

#endif

typedef VOID(WINAPI *PtrDwmDefWindowProc)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *plResult);
typedef HRESULT(WINAPI *PtrDwmIsCompositionEnabled)(BOOL *pfEnabled);

typedef HRESULT(WINAPI *PtrDwmGetWindowAttribute)(HWND hwnd, DWORD dwAttribute, PVOID pvAttribute, DWORD cbAttribute);
typedef HRESULT(WINAPI *PtrDwmSetWindowAttribute)(HWND, DWORD, LPCVOID, DWORD);
typedef HRESULT(WINAPI *PtrDwmExtendFrameIntoClientArea)(HWND hWnd, const HLP_MARGINS *pMarInset);
typedef HRESULT(WINAPI *PtrDwmEnableBlurBehindWindow)(HWND hWnd, const DWM_BLURBEHIND *pBlurBehind);
typedef BOOL(WINAPI *PtrSetWindowCompositionAttribute)(HWND, WINCOMPATTRDATA *);
typedef HRESULT(WINAPI *PtrDwmGetColorizationColor)(DWORD *pcrColorization, BOOL *pfOpaqueBlend);
typedef HRESULT(WINAPI *PtrDwmGetColorizationParameters)(DWMCOLORIZATIONPARAMS *color);
typedef HRESULT(WINAPI *PtrDwmSetColorizationParameters)(DWMCOLORIZATIONPARAMS *color, UINT unknown);
typedef BOOL(WINAPI *PtrAdjustWindowRectExForDpi)(LPRECT lpRect, DWORD dwStyle,
                                                  BOOL bMenu, DWORD dwExStyle, UINT dpi);
typedef UINT(WINAPI *PtrGetDpiForWindow)(HWND hwnd);
typedef UINT(WINAPI *PtrGetDpiForSystem)(VOID);
typedef UINT(WINAPI *PtrGetSystemDpiForProcess)(HANDLE hProcess);
typedef HRESULT(WINAPI *PtrDwmFlush)(void);

// Theme related
typedef bool(WINAPI *PtrIsAppThemed)();
typedef HANDLE(WINAPI *PtrOpenThemeData)(HWND hwnd, LPCWSTR pszClassList);
typedef HRESULT(WINAPI *PtrCloseThemeData)(HTHEME hTheme);
typedef HRESULT(WINAPI *PtrGetThemeRect)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, LPRECT pRect);
typedef int(WINAPI *PtrGetThemeSysSize)(HTHEME hTheme, int iSizeID);

typedef HRESULT(WINAPI *PtrGetThemeSysFont)(HANDLE hTheme, int iFontId, LOGFONTW *plf);
typedef HRESULT(WINAPI *PtrDrawThemeTextEx)(HANDLE hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int cchText, DWORD dwTextFlags, LPRECT pRect, const HLP_DTTOPTS *pOptions);
typedef HRESULT(WINAPI *PtrDrawThemeIcon)(HANDLE hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, HIMAGELIST himl, int iImageIndex);
typedef HRESULT(WINAPI *PtrSetWindowThemeAttribute)(HWND hwnd, enum HLP_WINDOWTHEMEATTRIBUTETYPE eAttribute, PVOID pvAttribute, DWORD cbAttribute);
typedef HRESULT(WINAPI *PtrSetWindowTheme)(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList);
typedef HRESULT(WINAPI *PtrGetThemeColor)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, COLORREF *pColor);
typedef COLORREF(WINAPI *PtrGetThemeSysColor)(HTHEME hTheme, int iColorID);

APROCH_NAMESPACE_BEGIN

class APROCH_API AImageHDC
{
public:
    AImageHDC(int width, int height, QImage::Format format);
    virtual ~AImageHDC();
    int width() const { return m_image.width(); }
    int height() const { return m_image.height(); }
    QSize size() const { return m_image.size(); }
    QImage &image() { return m_image; }
    const QImage &image() const { return m_image; }
    HDC hdc() const { return m_hdc; }
    static QImage::Format systemFormat();

private:
    const HDC m_hdc;
    QImage m_image;
    HBITMAP m_bitmap;
    HBITMAP m_null_bitmap;
};

typedef void(CALLBACK *WINEVENTPROC)(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD idEventThread, DWORD dwmsEventTime);
typedef HWINEVENTHOOK(WINAPI *LPFNSETWINEVENTHOOK)(UINT eventMin, UINT eventMax, HMODULE hmodWinEventProc,
                                                   WINEVENTPROC lpfnWinEventProc, DWORD idProcess, DWORD idThread, UINT dwflags);
typedef BOOL(WINAPI *LPFNUNHOOKWINEVENT)(HWINEVENTHOOK hWinEventHook);

class AWindowsHook
{
public:
    friend class AWindowsHookManager;
    AWindowsHook();
    virtual ~AWindowsHook();
    HWND winId() const;

protected:
    virtual LRESULT winProc(HWND hWnd, uint nMessage, WPARAM wParam, LPARAM lParam);
    virtual void winEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);
    LRESULT OldWindowProc(HWND hWnd, uint nMessage, WPARAM wParam, LPARAM lParam);

private:
    void setWinId(HWND winId);
    WNDPROC m_oldProc;
    HWND m_winId;
    HWINEVENTHOOK m_winEventHook;
    LPFNSETWINEVENTHOOK m_setWinEventProc;
    LPFNUNHOOKWINEVENT m_unhookWinEventProc;
};

class AWindowsHookManager
{
public:
    friend class AWindowsHook;

protected:
    AWindowsHookManager();

public:
    ~AWindowsHookManager();
    static AWindowsHookManager *instance();

public:
    void addHook(HWND winId, AWindowsHook *hook);
    void removeHook(AWindowsHook *hook);
    AWindowsHook *hookByWinId(HWND hWnd);

private:
    static LRESULT CALLBACK HookWindowProc(HWND hWnd, uint message, WPARAM wParam, LPARAM lParam);
    static void CALLBACK HookWindowEventProc(HWINEVENTHOOK hWinEventHook,
                                             DWORD event, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);

private:
    QHash<HWND, AWindowsHook *> m_hookHash;

private:
    Q_DISABLE_COPY(AWindowsHookManager)
};

extern PtrDwmDefWindowProc pDwmDefWindowProc;
extern PtrDwmIsCompositionEnabled pDwmIsCompositionEnabled;
extern PtrDwmGetWindowAttribute pDwmGetWindowAttribute;
extern PtrDwmSetWindowAttribute pDwmSetWindowAttribute;
extern PtrDwmExtendFrameIntoClientArea pDwmExtendFrameIntoClientArea;
extern PtrDwmEnableBlurBehindWindow pDwmEnableBlurBehindWindow;
extern PtrDwmGetColorizationColor pDwmGetColorizationColor;
extern PtrDwmGetColorizationParameters pDwmGetColorizationParameters;
extern PtrDwmSetColorizationParameters pDwmSetColorizationParameters;
extern PtrDwmFlush pDwmFlush;
extern PtrAdjustWindowRectExForDpi pAdjustWindowRectExForDpi;
extern PtrGetDpiForWindow pGetDpiForWindow;
extern PtrGetDpiForSystem pGetDpiForSystem;
extern PtrGetSystemDpiForProcess pGetSystemDpiForProcess;

extern PtrIsAppThemed pIsAppThemed;
extern PtrOpenThemeData pOpenThemeData;
extern PtrCloseThemeData pCloseThemeData;
extern PtrGetThemeRect pGetThemeRect;
extern PtrGetThemeSysSize pGetThemeSysSize;

extern PtrGetThemeSysFont pGetThemeSysFont;
extern PtrDrawThemeTextEx pDrawThemeTextEx;
extern PtrDrawThemeIcon pDrawThemeIcon;
extern PtrSetWindowTheme pSetWindowTheme;
extern PtrGetThemeColor pGetThemeColor;
extern PtrGetThemeSysColor pGetThemeSysColor;
extern PtrSetWindowThemeAttribute pSetWindowThemeAttribute;
extern PtrSetWindowCompositionAttribute pSetWindowCompositionAttribute;

__inline HRESULT _SetWindowThemeNonClientAttributes(HWND hwnd, DWORD dwMask, DWORD dwAttributes)
{
    HLP_WTA_OPTIONS wta;
    wta.dwFlags = dwAttributes;
    wta.dwMask = dwMask;
    return pSetWindowThemeAttribute(hwnd, HLP_WTA_NONCLIENT, (void *)&(wta), sizeof(wta));
}

APROCH_API bool aproch_isCompositionAvailable();
APROCH_API QPixmap aproch_createPixmap(HBITMAP bitmap, int hbitmapFormat = 1 /* PremultipliedAlpha */);
APROCH_API HBITMAP aproch_createHBITMAP(const QPixmap &pixmap, int hbitmapFormat = 1 /* PremultipliedAlpha */);
APROCH_API HICON aproch_createHICON(const QIcon &icon, QSize &inOutSize);
APROCH_API HDC aproch_getDC(QPainter *painter);
APROCH_API void aproch_releaseDC(QPainter *painter, HDC dc);
APROCH_API HDC aproch_createDC();
APROCH_API bool aproch_setWindowColorization(QWidget *widget, const QColor &color);

APROCH_API HFONT aproch_get_qfont_handle(const QFont &f);
APROCH_API void aproch_release_qfont_handle(HFONT hf);
HBITMAP aproch_createDIB(HDC hdc, int width, int height, QImage::Format format, uchar **bitsIn);
HRGN aproch_create_region(const QRegion &region);
APROCH_API QFont aproch_getWindowCaptionFont(QWidget* window, int dpi = -1);

APROCH_API QMargins aproch_getWindowFrameMargings(const QWidget* window);
APROCH_API int aproch_getNativeWindowDPI(const QWidget *window);

APROCH_API bool aproch_getWindows10AcrylicSupport();
APROCH_API bool aproch_isWindowsXP();
APROCH_API bool aproch_isWindows7();
APROCH_API bool aproch_isWindows10();
APROCH_API int aproch_get_sys_command_win(QWidget *window, QMouseEvent *event, Qt::WindowFrameSection frameSection);
APROCH_API bool aproch_sys_command_win(QWidget *window, int command, const QPoint &pos, bool synchronize = false);

APROCH_NAMESPACE_END
