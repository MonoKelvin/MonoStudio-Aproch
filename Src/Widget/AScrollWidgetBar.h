/****************************************************************************
 * @file    AScrollWidgetBar.h
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

#include <QWidget>
#include <QAbstractButton>
#include <QPropertyAnimation>

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class AScrollWidgetBarButton;

class APROCH_API AAbstractScrollWidgetBar : public QWidget
{
    Q_OBJECT
public:
    enum MoveType
    {
        MoveByPage,
        MoveByItem
    };
    AAbstractScrollWidgetBar(QWidget* parent, Qt::Orientation orientation = Qt::Horizontal);
    ~AAbstractScrollWidgetBar() override;
    void setMoveType(MoveType type)
    {
        m_moveType = type;
    }
public:
    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);
    bool scrollButtonsBeside() const;
    void setScrollButtonsBeside(bool beside);
    QWidget* viewWidget() const;
    int pageIndent() const;
    void setPageIndent(int indent);
    const QMargins& margins() const;
    void setMargins(const QMargins& margins);
    int itemIndent() const;
    void setItemIndent(int indent);
    void setAmimationSettings(int duration, const QEasingCurve& curve);
    int maxViewItemCount() const;
    void setMaxViewItemCount(int count);
    QSize sizeHint() const override;

    virtual int getColumnCount() const = 0;
    virtual QSize getColumnSize(int column) const = 0;
    void ensureVisible(int column);
    virtual void updateLayout();
public Q_SLOTS:
    void movePrev();
    void moveNext();
    void moveTo(int column);
protected:
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
protected:
    virtual QWidget* createViewWidget();
    void setTogetherButtonsVisible(bool visible, bool prevEnabled, bool nextEnabled);
    void setPrevButtonVisible(bool visible, bool enabled);
    void setNextButtonVisible(bool visible, bool enabled);
    virtual void updateItemsLayout() = 0;
    int getColumnLength(int column) const;
    virtual QColor getButtonColor() const;
    int calcColumnsLength(int start, int end) const;
    bool isColumnFirst(int column) const;
    bool isColumnLast(int column) const;
    int nextPageColumn(int column, bool back) const;
    int nextColumn(int column, bool back) const;
    QPoint getViewOffset() const;
    void startScrollAnimation();
protected Q_SLOTS:
    void scrollAnimationFinished();
protected:
    friend class AScrollWidgetBarButton;
    AScrollWidgetBarButton* m_prevButton;
    AScrollWidgetBarButton* m_nextButton;
    Qt::Orientation m_orientation;
    MoveType m_moveType;
    int m_columnNum;
    int m_columnFirst;
    int m_columnLast;
    QWidget* m_viewWidget;
    int m_pageIndent;
    QMargins m_margins;
    int m_itemIndent;
    QPropertyAnimation* m_animation;
    int m_maxViewItemCount;
    bool m_scrollButtonsBeside;
    Q_DISABLE_COPY(AAbstractScrollWidgetBar)
};

class APROCH_API AScrollWidgetBar : public AAbstractScrollWidgetBar
{
    Q_OBJECT
public:
    AScrollWidgetBar(QWidget* parent, Qt::Orientation orientation = Qt::Horizontal);
    virtual ~AScrollWidgetBar();
public:
    void addWidget(QWidget* widget);
    void removeWidget(QWidget* widget);
    void beginUpdate();
    void endUpdate();
    bool isUpdating() const;
    QWidget* getWidget(int index) const;
    int getWidgetCount() const;
    virtual int getColumnCount() const;
    virtual QSize getColumnSize(int column) const;
    virtual int getColumnIndex(QWidget* widget) const;
protected:
    virtual void updateItemsLayout();
    virtual bool focusNextPrevChild(bool next);
    QVector<QWidget*> m_widgets;
    int m_updateCount;
private:
    Q_DISABLE_COPY(AScrollWidgetBar)
};

class APROCH_API AScrollWidgetBarButton : public QAbstractButton
{
public:
    explicit AScrollWidgetBarButton(AAbstractScrollWidgetBar* bar);
    ~AScrollWidgetBarButton() override;
    QSize sizeHint() const override;
protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    void enterEvent(QEnterEvent* event) override;
#else
    void enterEvent(QEvent* event) override;
#endif
    void leaveEvent(QEvent* event) override;
private:
private:
    Q_DISABLE_COPY(AScrollWidgetBarButton)
};

APROCH_NAMESPACE_END