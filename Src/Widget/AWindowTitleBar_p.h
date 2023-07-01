/****************************************************************************
 * @file    AAWindowTitleBar_p.h
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
#include <QScreen>
#include <QWidget>
#include <QStyleOptionTitleBar>
#include <QAction>

#include "Platform/APlatform.h"
#include "AWindowTitleBar.h"
#include "ABackstageWidget.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
inline QMargins operator+(const QMargins& m1, const QMargins& m2) Q_DECL_NOTHROW
{
    return QMargins(m1.left() + m2.left(), m1.top() + m2.top(),
                    m1.right() + m2.right(), m1.bottom() + m2.bottom());
}

inline QMargins operator-(const QMargins& m1, const QMargins& m2) Q_DECL_NOTHROW
{
    return QMargins(m1.left() - m2.left(), m1.top() - m2.top(),
                    m1.right() - m2.right(), m1.bottom() - m2.bottom());
}
#endif

class AWindowStyleHelperAbstract
{
public:
    AWindowStyleHelperAbstract()
        : m_window(nullptr), m_destroyed(false)
    {
    }
    virtual ~AWindowStyleHelperAbstract()
    {
    }
public:
    const QMargins& frameMargins() const
    {
        return m_frameMargins;
    }
    void setFrameMargins(const QMargins& margins)
    {
        m_frameMargins = margins;
    }
    QWidget* window() const
    {
        return m_window;
    };
    virtual void setWindow(QWidget* window)
    {
        m_window = window;
    }
    const QSize& windowSize() const
    {
        return m_windowSize;
    };
    void setWindowSize(const QSize& size)
    {
        m_windowSize = size;
    }
    virtual void update(const QRect& rect) = 0;
    virtual void update(const QRegion& region) = 0;
    virtual bool isActive() const = 0;
    virtual bool underMouse() const = 0;
    virtual void showSystemMenu(const QPoint& pos) = 0;
    bool isDestroyed() const
    {
        return m_destroyed;
    }
    void setDestroyed()
    {
        m_destroyed = true;
    }
protected:
    virtual void nonClientActivateEvent(bool active) = 0;
    virtual void paintNonClientEvent(QPainter* painter, Qt::WindowFrameSection section = Qt::NoSection) = 0;
    virtual void mousePressNonClientEvent(QMouseEvent* event) = 0;
    virtual void mouseReleaseNonClientEvent(QMouseEvent* event) = 0;
    virtual void mouseDoubleClickNonClientEvent(QMouseEvent* event) = 0;
    virtual void mouseMoveNonClientEvent(QMouseEvent* event) = 0;
    virtual void mouseNonClientOver(const QPoint& pos) = 0;
    virtual void mouseNonClientOut(const QPoint& pos) = 0;
    virtual void mouseContextNonClientEvent(QContextMenuEvent* event) = 0;
    virtual void resizeNonClientEvent(const QSize& windowSize) = 0;
private:
    QWidget* m_window;
    QMargins m_frameMargins;
    QSize m_windowSize;
    bool m_destroyed;
};

class AFrameStyleOption;

class AWindowTitleBarPrivate : public QObject, public AWindowStyleHelperAbstract
{
    Q_OBJECT
public:
    friend class AExendedView;
    friend class AWindowTitleBar;
    AWindowTitleBarPrivate();
    virtual ~AWindowTitleBarPrivate();
public:
    bool isInitialized() const
    {
        return m_initialized;
    }
    virtual void setWindow(QWidget* window);
    void setVisible(bool visible);
    bool isVisible() const;
    void setResizible(bool resizible);
    bool isResizible() const;
    virtual bool underMouse() const;
    void setTitleBarWidget(QWidget* widget);
    QWidget* titleBarWidget() const;
    QString titleBarText() const;
    AWindowTitleBar::WidgetAlign titleBarWidgetAlign() const;
    void setTitleBarWidgetAlign(AWindowTitleBar::WidgetAlign align);
    bool hasFocus() const;
    QRect getClientRect() const;
    QRegion getNonClientRegion() const;
    QRect getTitleBarRect();
    QStyle::SubControl hitTest(const QPoint& pos, Qt::WindowFrameSection& frameSection);
    bool styledFrame() const;
    void setStyledFrame(bool styled);
    bool blurBehindWindowEnabled() const;
    void setBlurBehindWindowEnabled(bool enabled);
    bool extendViewIntoTitleBar() const;
    void setExtendViewIntoTitleBar(bool extend);
    AWindowTitleBar::SysButtonKind sysButtonKind() const;
    void setSysButtonKind(AWindowTitleBar::SysButtonKind kind);
    const QColor& backgroundColor() const;
    void setBackgroundColor(const QColor& color);
    const QColor& textColor() const;
    void setTextColor(const QColor& color);
    QColor frameColor() const;
    const QMargins& systemFrameMargins() const;
    int borderThickness() const;
    int titleHeight() const;
    bool isMaximized() const;
    bool isFullScreen() const;
    void updateLayout();
    virtual void updateFrame(bool forceLayoutUpdate);
    virtual void update(const QRect& rect);
    virtual void update(const QRegion& region);
    void setCursor(const QCursor& cursor);
    void unsetCursor();
    virtual QVariant inputMethodQuery(Qt::InputMethodQuery query) const;
    void paintMaterialBehindWindow(QPainter* painter, QWidget* w) const;
    void updateWidgetPosition();
    static AWindowTitleBarPrivate* get(AWindowTitleBar* titleBar);
protected:
    virtual void inputMethodEvent(QInputMethodEvent* event);
    virtual bool eventFilter(QObject* object, QEvent* event);
    virtual bool installWindow();
    virtual void resetWindow();
    virtual void nonClientActivateEvent(bool active);
    virtual void paintNonClientEvent(QPainter* painter, Qt::WindowFrameSection section = Qt::NoSection);
    virtual void mousePressNonClientEvent(QMouseEvent* event);
    virtual void mouseReleaseNonClientEvent(QMouseEvent* event);
    virtual void mouseDoubleClickNonClientEvent(QMouseEvent* event);
    virtual void mouseMoveNonClientEvent(QMouseEvent* event);
    virtual void mouseNonClientOver(const QPoint& pos);
    virtual void mouseNonClientOut(const QPoint& pos);
    virtual void mouseContextNonClientEvent(QContextMenuEvent* event);
    virtual void resizeNonClientEvent(const QSize& windowSize);
    virtual bool isGlowEnabled() const;
protected:
    virtual QRect getSubControlRect(QStyle::SubControl sc);
    virtual QStyle::SubControl getHoverSubControl(const QPoint& pos);
    virtual void styleChanged();
    virtual void updateSysButtonsRect();
    virtual QStyle::SubControl getHoverNativeSubControl(const QPoint& pos);
    virtual void paintTitleBarSysButton(QPainter* painter, const QStyleOptionTitleBar& option);
    virtual void paintTitleBarText(QPainter* painter, const QStyleOptionTitleBar& option);
    virtual void nativeCommand(QMouseEvent* event, Qt::WindowFrameSection frameSection) = 0;
    virtual QPoint mapFromGlobal(const QPoint& pos) const = 0;
    virtual QPoint mapToGlobal(const QPoint& pos) const = 0;
    virtual QPoint tranlateToNonClient(const QPoint& clientPos) = 0;
protected:
    virtual bool mouseEventFilter(QObject* object, QEvent* event);
    void updateInputMethod();
    void checkUnderMouse(const QPoint& pos);
    bool installWindowInternal();
    bool useStyleToPaintIconAndText() const;
    void processExtendViewEvent(QEvent* event);
    Qt::WindowFrameSection checkFrameSection(const QPoint& pos);
    void initTitleBarOption(QStyleOptionTitleBar& option);
    void initFrameOption(AFrameStyleOption& frameOption, Qt::WindowFrameSection section = Qt::NoSection);
    void updateSubControl(const QPoint& pos);
    void setSubControlPressed(QStyle::SubControl sc);
    void subControlClicked(QMouseEvent* event);
    virtual void processContextMenuEvent(QMouseEvent* event);
    bool defaultResizeHandler();
    void updateTitleBarWidgetGeometry();
    bool containsPoint(const QPoint& pos);
    void updateExtendedViewGeometry();
    void updateCursor();
    void updateMaterialWidgets();
    QCursor getCursorForFrameSection(Qt::WindowFrameSection frameSection);
    int getSysMenuButtonWidth() const;
    bool hasCustomBackground() const;
    bool hasCustomForeground() const;
    const QPixmap& behindWindowBackground() const;
    void syncContentsMargins();
    bool m_initialized;
    bool m_visible;
    bool m_resizible;
    Qt::WindowFlags m_oldFlags;
    ABackstageWidget* m_titleBarWidget;
    AWindowTitleBar::WidgetAlign m_titleBarWidgetAlign;
    QRect m_textRect;
    Qt::WindowFrameSection m_hoveredFrameSection;
    QStyle::SubControl m_hovered;
    QStyle::SubControl m_pressed;
    bool m_nonClientUnderMouse;
    bool m_styledFrame;
    bool m_blurBehindWindowEnabled;
    bool m_extendViewIntoTitleBar;
    QMargins m_systemFrameMargins;
    QRect m_systemButtonsRect;
    int m_borderThickness;
    int m_titleHeight;
    int m_borderThicknessRequested;
    int m_titleHeightRequested;
    QIcon m_icon;
    bool m_maximized;
    bool m_fullScreen;
    AWindowTitleBar::SysButtonKind m_sysButtonKind;
    QColor m_backgroundColor;
    QColor m_textColor;
    QPalette m_titleBarPalette;
    QMargins m_frameWindows10;
    QWidget* m_extendedView;
    QMargins m_origContentsMargins;
    bool m_hasCursor;
    QPixmap m_bluredDesktop;
    int m_eventsCount;
    bool m_QWizardSupports;
private Q_SLOTS:
    void  backstageWidgetUpdate(const QList<QRectF>& region);
    void screenChanged(QScreen* screen);
    friend class AWindowTitleBarAutoUpdater;
};

class AWindowTitleBarPrivateQt : public AWindowTitleBarPrivate
{
public:
    AWindowTitleBarPrivateQt();
    virtual ~AWindowTitleBarPrivateQt();
    virtual bool isActive() const;
    virtual void showSystemMenu(const QPoint& pos);
protected:
    virtual bool installWindow();
    virtual void resetWindow();
    virtual bool eventFilter(QObject* object, QEvent* event);
    virtual void processContextMenuEvent(QMouseEvent* event);
    virtual void resizeNonClientEvent(const QSize& windowSize);
    virtual void nativeCommand(QMouseEvent* event, Qt::WindowFrameSection frameSection);
    virtual QPoint mapFromGlobal(const QPoint& pos) const;
    virtual QPoint mapToGlobal(const QPoint& pos) const;
    virtual QPoint tranlateToNonClient(const QPoint& clientPos);
    virtual bool mouseEventFilter(QObject* object, QEvent* event);
private:
    bool isDragMode() const;
    void startDrag(const QPoint& pos, Qt::WindowFrameSection frameSection);
    void stopDrag();
    void dragMove(const QPoint& pos);
    virtual void updateFrame(bool forceLayoutUpdate);
private:
    Qt::WindowFrameSection m_frameSection;
    QPoint m_startDragPos;
    QRect m_grabGeometry;
};

class AExendedView : public QWidget
{
public:
    AExendedView(AWindowTitleBarPrivate* bar);
    virtual ~AExendedView();
protected:
    virtual void paintEvent(QPaintEvent* event);
private:
    AWindowTitleBarPrivate* m_bar;
};

class AWindowTitleBarAutoUpdater
{
public:
    AWindowTitleBarAutoUpdater(AWindowTitleBarPrivate* p)
        : m_p(p)
    {
        m_p->m_eventsCount++;
    }

    ~AWindowTitleBarAutoUpdater()
    {
        m_p->m_eventsCount--;
    }
private:
    AWindowTitleBarPrivate* m_p;
};

APROCH_NAMESPACE_END
