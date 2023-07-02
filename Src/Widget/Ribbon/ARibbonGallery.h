/****************************************************************************
 * @file    ARibbonGallery.h
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
#include <QIcon>
#include <QWidget>

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

class QStyleOptionSlider;
class QMenu;

APROCH_NAMESPACE_BEGIN

class ARibbonGallery;
class ARibbonGalleryGroup;
class ARibbonGalleryItemStyleOption;
class ARibbonGalleryItemPrivate;

/* ARibbonGalleryItem */
class APROCH_API ARibbonGalleryItem
{
public:
    ARibbonGalleryItem();
    virtual ~ARibbonGalleryItem();
public:
    virtual QSize sizeHint() const;
    void setSizeHint(const QSize& size);
    QIcon icon() const;
    void setIcon(const QIcon& icon);
    QString caption() const;
    void setCaption(const QString& caption);
    QString toolTip() const;
    void setToolTip(const QString& toolTip);
    QString statusTip() const;
    void setStatusTip(const QString& statusTip);
    int getIndex() const;
    bool isSeparator() const;
    void setSeparator(bool b);
    void setEnabled(bool enabled);
    bool isEnabled() const;
    void setVisible(bool visible);
    bool isVisible() const;
    virtual QVariant data(int role) const;
    virtual void setData(int role, const QVariant& value);
    virtual void draw(QPainter* p, ARibbonGallery* gallery, QRect rectItem,
    bool enabled, bool selected, bool pressed, bool checked);
private:
    A_DECLARE_PRIVATE(ARibbonGalleryItem)
    Q_DISABLE_COPY(ARibbonGalleryItem)
    friend class ARibbonGalleryGroup;
    friend class ARibbonGalleryGroupPrivate;
};

class ARibbonGalleryGroupPrivate;
class AOfficePopupMenu;
/* ARibbonGalleryGroup */
class APROCH_API ARibbonGalleryGroup : public QObject
{
    Q_OBJECT
public:
    ARibbonGalleryGroup(QObject* parent = nullptr);
    virtual ~ARibbonGalleryGroup();
public:
    ARibbonGalleryItem* addItem(const QString& caption, const QPixmap& pixmap = QPixmap(), const QColor& transparentColor = QColor());
    ARibbonGalleryItem* addItemFromMap(const QString& caption, int mapIndex, const QPixmap& map, const QSize& mapSizeImage, const QColor& transparentColor = QColor());

    void appendItem(ARibbonGalleryItem* item);
    void insertItem(int index, ARibbonGalleryItem* item);

    ARibbonGalleryItem* addSeparator(const QString& caption);

    void clear();
    void remove(int index);

    int itemCount() const;
    ARibbonGalleryItem* item(int index) const;
    ARibbonGalleryItem* takeItem(int index); 

    QSize size() const;
    void setSize(const QSize& size);
public:
    void setClipItems(bool clipItems);
private:
    friend class ARibbonGalleryItem;
    friend class ARibbonGallery;
    friend class ARibbonGalleryPrivate;
    A_DECLARE_PRIVATE(ARibbonGalleryGroup)
    Q_DISABLE_COPY(ARibbonGalleryGroup)
};

class ARibbonGalleryPrivate;
/* ARibbonGallery */
class APROCH_API ARibbonGallery : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool isBorderVisible READ isBorderVisible WRITE setBorderVisible)
    Q_PROPERTY(bool isTransparent READ isTransparent WRITE setLabelsVisible)
    Q_PROPERTY(int minimumColumnCount READ minimumColumnCount WRITE setMinimumColumnCount)
    Q_PROPERTY(int maximumColumnCount READ maximumColumnCount WRITE setMaximumColumnCount)
    Q_PROPERTY(Qt::ScrollBarPolicy scrollBarPolicy READ scrollBarPolicy WRITE setScrollBarPolicy)
    Q_PROPERTY(bool isResizable READ isResizable)
    Q_PROPERTY(bool autoWidth READ autoWidth WRITE setAutoWidth)

public:
    ARibbonGallery(QWidget* parent = nullptr);
    virtual ~ARibbonGallery();
public:
    void setGalleryGroup(ARibbonGalleryGroup* items);
    ARibbonGalleryGroup* galleryGroup() const;

    bool isBorderVisible() const;
    void setBorderVisible(bool visible);

    bool isTransparent() const;
    void setTransparent(bool transparent = true);

    bool isLabelsVisible() const;
    void setLabelsVisible(bool showLabels);

    void setScrollBarPolicy(Qt::ScrollBarPolicy policy);
    Qt::ScrollBarPolicy scrollBarPolicy() const;

    void ensureVisible(int index);

    QAction* setPopupMenu(AOfficePopupMenu* popupMenu);
    AOfficePopupMenu* popupMenu() const;

    void setMinimumColumnCount(int count);
    int minimumColumnCount() const;

    void setMaximumColumnCount(int count);
    int maximumColumnCount() const;

    void setColumnCount(int count);
    int columnCount() const;

    void setRowCount(int row);
    int rowCount() const;
public:
    int itemCount() const;

    ARibbonGalleryItem* item(int index) const;
    void setSelectedItem(int index);
    int selectedItem() const;

    void setCheckedIndex(int index);
    int checkedIndex() const;

    void setCheckedItem(const ARibbonGalleryItem* item);
    ARibbonGalleryItem* checkedItem() const;

    bool isShowAsButton() const;
    bool isItemSelected() const;
public:
    virtual QRect borders() const;
    QRect getDrawItemRect(int index);
    bool isResizable() const;

    bool autoWidth() const;
    void setAutoWidth(bool width);

    int hitTestItem(QPoint point, QRect* rect = nullptr) const;

    QRect getItemsRect() const;
    void hideSelection();
    void updatelayout();
    void bestFit();
Q_SIGNALS:
    void itemPressed(ARibbonGalleryItem* item);
    void itemClicked(ARibbonGalleryItem* item);
    void itemClicking(ARibbonGalleryItem* item, bool& handled);
    void currentItemChanged(ARibbonGalleryItem* current, ARibbonGalleryItem* previous);
    void itemSelectionChanged();
public:
    virtual void selectedItemChanged();
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
protected:
    virtual bool event(QEvent* event);
    virtual void paintEvent(QPaintEvent*);
    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void wheelEvent(QWheelEvent* event);
    virtual void leaveEvent(QEvent*);
    virtual void focusOutEvent(QFocusEvent*);
    virtual void resizeEvent(QResizeEvent*);
private:
    friend class ARibbonGalleryGroupPrivate;
    A_DECLARE_PRIVATE(ARibbonGallery)
    Q_DISABLE_COPY(ARibbonGallery)
};

APROCH_NAMESPACE_END