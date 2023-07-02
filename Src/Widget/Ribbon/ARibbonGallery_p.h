/****************************************************************************
 * @file    ARibbonGallery_p.h
 * @date    2023-07-02 
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
#include <QRect>
#include <QVector>
#include <QVariant>
#include <QBasicTimer>
#include <QToolButton>
#include <QIcon>

#include "ARibbonGallery.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

class QPainter;
class QScrollBar;
class QAction;

class QMenu;
class QToolButton;

APROCH_NAMESPACE_BEGIN

class AWidgetItemData
{
public:
    inline AWidgetItemData() : role(-1) {}
    inline AWidgetItemData(int r, QVariant v) : role(r), value(v) {}
    int role;
    QVariant value;
    inline bool operator==(const AWidgetItemData &other) { return role == other.role && value == other.value; }
};

class ARibbonGalleryItem;
class ARibbonGalleryItemPrivate: public QObject
{
public:
    A_DECLARE_PUBLIC(ARibbonGalleryItem);
public:
    explicit ARibbonGalleryItemPrivate();
public:
    void init();
public:
    int m_index;
    QString m_descriptionText;
    ARibbonGalleryGroup* m_items;
    bool m_separator : 1;
    bool m_enabled   : 1;
    bool m_visible   : 1;

    QVector<AWidgetItemData> m_values;
};

class ARibbonGalleryGroup;
/* ARibbonGalleryGroupPrivate */
class ARibbonGalleryGroupPrivate : public QObject
{
public:
    A_DECLARE_PUBLIC(ARibbonGalleryGroup)
public:
    explicit ARibbonGalleryGroupPrivate();
    virtual ~ARibbonGalleryGroupPrivate();
public:
    void init();
    void updateIndexes(int start /*= 0*/);
    void itemsChanged();
    void redrawWidget();
    void clear();
public:
    bool m_clipItems : 1;
    QSize m_sizeItem;
    QVector<ARibbonGalleryItem*> m_arrItems;
    QList<QWidget*> m_viewWidgets;
};


class ARibbonGallery;

struct SGalleryItemRect
{
    ARibbonGalleryItem* item;
    QRect rect;
    bool beginRow;
};

/* ARibbonGalleryButton */
class ARibbonGalleryButton : public QToolButton
{
    Q_OBJECT
public:
    ARibbonGalleryButton(QWidget* parent) : QToolButton(parent) {}
protected:
    virtual void paintEvent(QPaintEvent* event);
private:
    Q_DISABLE_COPY(ARibbonGalleryButton)
};


class AOfficePopupMenu;
/* ARibbonGalleryPrivate */
class ARibbonGalleryPrivate : public QObject
{
public:
    Q_OBJECT
    A_DECLARE_PUBLIC(ARibbonGallery)
public:
    explicit ARibbonGalleryPrivate();
    virtual ~ARibbonGalleryPrivate();
public:
    void init();
    void layoutItems();
    void layoutScrollBar();
    void setScrollBarValue();
    void setScrollBarPolicy(Qt::ScrollBarPolicy policy);
    Qt::ScrollBarPolicy scrollBarPolicy() const;
    QAction* setPopupMenu(AOfficePopupMenu* popup);

    void drawGalleryItems(QPainter* painter);
    void repaintItems(QRect* pRect = nullptr, bool bAnimate = true);

    bool isItemChecked(ARibbonGalleryItem* pItem) const;
    bool isScrollButtonEnabled(bool buttonUp);

    int scrollWidth() const;

    void startAnimation(int scrollPos);
    void startAnimate();
public Q_SLOTS:
    void setScrollPos(int nScrollPos);
    void actionTriggered(int action);
    void pressedScrollUp();
    void pressedScrollDown();
protected:
    virtual bool event(QEvent* event);
    virtual bool eventFilter(QObject* object, QEvent* event);
public:
    QScrollBar* m_scrollBar; 
    QToolButton* m_buttonScrollUp;
    QToolButton* m_buttonScrollDown;
    QToolButton* m_buttonPopup;
    AOfficePopupMenu* m_menuBar;
    QVector<SGalleryItemRect> m_arrRects;

    bool m_showBorders       : 1;
    bool m_showLabels        : 1;
    bool m_hideSelection     : 1;
    bool m_transparent       : 1; 
    bool m_keyboardSelected  : 1;
    bool m_preview           : 1;
    bool m_pressed           : 1;
    bool m_animation         : 1;
    bool m_autoWidth         : 1;

    int m_totalHeight;
    int m_scrollPos;
    int m_scrollPosTarget;
    qreal m_animationStep;
    int m_timerElapse;

    int m_selected;
    ARibbonGalleryItem* m_checkedItem;
    ARibbonGalleryGroup* m_items;

    QPoint m_ptPressed;
    QBasicTimer m_scrollTimer;

    int m_minimumColumnCount;
    int m_maximumColumnCount;
    int m_currentColumnCount;
    int m_currentRowCount;
private:
    Q_DISABLE_COPY(ARibbonGalleryPrivate)
};

APROCH_NAMESPACE_END