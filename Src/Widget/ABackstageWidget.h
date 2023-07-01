/****************************************************************************
 * @file    ABackstageWidget.h
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

#include <QPointer>
#include <QMargins>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QWindow>
#endif

APROCH_NAMESPACE_BEGIN

#ifdef A_PRIVATE_HEADERS
#define A_SHARED_BACKSTAGE_WINDOW
#endif

#ifdef A_SHARED_BACKSTAGE_WINDOW
class ABackstageSharedWindow;
class APROCH_API ABackstageWidgetBase : public QObject
{
    Q_OBJECT
public:
    ABackstageWidgetBase(QWidget* viewWidget, QObject* parent = nullptr);
    virtual ~ABackstageWidgetBase();
    QWidget* viewWidget() const;
    virtual QWidget* widget() const = 0;
Q_SIGNALS:
    void changed(const QList<QRectF> &region);
protected:
    QWidget* m_viewWidget;
    ABackstageSharedWindow* m_backstageWindow;
};
#else
class ABackstageWidgetBase : public QGraphicsScene
{
public:
    ABackstageWidgetBase(QWidget* viewWidget, QObject* parent)
        : QGraphicsScene(parent), m_viewWidget(viewWidget)
    {
    }
    QWidget* viewWidget() const
    {
        return m_viewWidget;
    }
private:
    QWidget* m_viewWidget;
};
#endif

class APROCH_API ABackstageWidget : public ABackstageWidgetBase
{
    Q_OBJECT
public:
    ABackstageWidget(QWidget* viewWidget, QObject* parent = nullptr);
    virtual ~ABackstageWidget();
    virtual QWidget* widget() const;
    void setWidget(QWidget* widget, const QRect& geometry = QRect());
    void activate();
    void deactivate();
    void setMargins(int left, int top, int right, int bottom);
    void setMargins(const QMargins& margins);
    const QMargins& margins() const;
    bool hasFocusItem();
    bool hasMouseGrabber();
    bool isInputMethodSupported() const;
    QVariant inputMethodQuery(Qt::InputMethodQuery query) const;
    void paintContent(QPainter& painter);

    /* mouse events */
    void send_mouseMoveEvent(QMouseEvent* e);
    void send_mousePressEvent(QMouseEvent* e);
    void send_mouseReleaseEvent(QMouseEvent* e);
    void send_mouseDoubleClickEvent(QMouseEvent* e);
    void send_mouseOverEvent(const QPoint& pos);
    void send_mouseOutEvent(const QPoint& pos);
    void send_wheelEvent(QWheelEvent *);
    /* keyboard events */
    void send_keyPressEvent(QKeyEvent* e, const QPoint& globalPosition);
    void send_keyReleaseEvent(QKeyEvent* e);
    void send_shortcutEvent(QKeyEvent* e);
    void send_shortcutOverrideEvent(QKeyEvent* e);
    /* input method */
    void send_inputMethodEvent(QInputMethodEvent* e);
    /* focus events */
    void send_focusInEvent(Qt::FocusReason reason);
    void send_focusOutEvent(Qt::FocusReason reason);
    void send_contextMenuEvent(QContextMenuEvent* event);
public:
    QCursor cursor() const;
    bool hasCursor() const;
    void setGeometry(const QRect& geometry);
    QRect geometry() const;
    void setGlobalPos(const QPoint& globalPos) const;
private:
    void syncToView();
    QGraphicsScene* scene() const;
    void connectWidget(const QRect& geometry);
    void disconnectWidget();
    void checkCursor(const QPoint& p);
    QWidget* findWidgetByPos(const QPoint& pos);
    bool tryShortcut(QKeyEvent* e);
protected Q_SLOTS:
    void widgetDestroyed();
private:
    void updateMousePressWidget(QWidget* w, Qt::MouseButtons buttons);
    QWidget* m_widget;
    QWidget* m_cursorWidget;
    QWidget* m_mousePressWidget;
    QGraphicsProxyWidget* m_proxyWidget;
    Qt::MouseButtons m_mousePressButtons;
    Qt::MouseButton m_mousePressButton;
    QMargins m_margins;
    QRect m_geometry;
#ifdef A_SHARED_BACKSTAGE_WINDOW
    QPointer<QWidget> m_overWidget;
    QRegion m_needsRepaint;
    friend class ABackstageSharedWindow;
#else
    QPointF m_mousePressPosition;
    QPoint m_mousePressScreenPosition;
    QPointF m_lastMouseMoveScenePosition;
    QPoint m_lastMouseMoveScreenPosition;
#endif
};

APROCH_NAMESPACE_END