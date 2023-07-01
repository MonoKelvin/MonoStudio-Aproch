/****************************************************************************
 * @file    APopupMenu_p.h
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
#include "APopupMenu.h"
#include "Style/AStyleOption.h"
#include "AScrollWidgetBar.h"

APROCH_NAMESPACE_BEGIN

class APopupMenuBar;

class APopupMenuView : public QWidget
{
    Q_OBJECT
public:
    explicit APopupMenuView(QWidget* parent, APopupMenu* menu);
public:
    APopupMenuBar* scrollMenuBar() const;
protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    virtual void enterEvent(QEnterEvent* event);
#else
    virtual void enterEvent(QEvent* event);
#endif
    virtual void leaveEvent(QEvent* event);
protected:
    APopupMenu* m_menu;
private:
    friend class APopupMenuBar;
    Q_DISABLE_COPY(APopupMenuView);
};

class APopupMenuBar : public AAbstractScrollWidgetBar
{
    Q_OBJECT
public:
    explicit APopupMenuBar(APopupMenu* parent);
public:
    APopupMenuView* menuViewWidget() const;
    AScrollWidgetBarButton* downButton() const
    {
        return m_nextButton;
    }
    QPoint viewOffset() const
    {
        return getViewOffset();
    }
    int scrollOffset() const
    {
        return m_scrollOffset;
    }
    void updateLayout() override;
protected:
    QWidget* createViewWidget() override;
    void updateItemsLayout() override;
    int getColumnCount() const override;
    QSize getColumnSize(int column) const override;
protected:
    void keyPressEvent(QKeyEvent* event) override;
protected:
    APopupMenu* m_menu;
    int m_scrollOffset;
private:
    friend class APopupMenu;
    Q_DISABLE_COPY(APopupMenuBar);
};

/* APopupMenuPrivate */
class APROCH_API APopupMenuPrivate : public QObject
{
public:
    A_DECLARE_PUBLIC(APopupMenu)
public:
    explicit APopupMenuPrivate();
public:
    static inline APopupMenuPrivate* _get(APopupMenu* menu)
    {
        return &menu->aproch_d();
    }
    static inline const APopupMenuPrivate* _get(const APopupMenu* menu)
    {
        return &menu->aproch_d();
    }
    APopupMenuBar* scrollMenuBar() const
    {
        return m_scrollMenuBar;
    }
public:
    void styleChanged();
    const QMargins& margins() const;
    void setMargins(const QMargins& margins);
protected:
    void updateActionRects(bool force = false);
    QMargins m_margins;
    QWidget* m_preview;
    APopupMenuBar* m_scrollMenuBar;
    QVector<QSize> m_columns;
    bool m_ignoreResize;
    friend class APopupMenuBar;
};

APROCH_NAMESPACE_END