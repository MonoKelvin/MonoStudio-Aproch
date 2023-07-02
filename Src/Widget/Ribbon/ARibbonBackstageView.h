/****************************************************************************
 * @file    ARibbonBackstageView.h
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
#include <QFrame>
#include <QToolButton>

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class ARibbonBar;

class APROCH_API ARibbonBackstageSeparator : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)
public:
    ARibbonBackstageSeparator(QWidget* parent);
    virtual ~ARibbonBackstageSeparator();
public:
    void setOrientation(Qt::Orientation orient);
    Qt::Orientation orientation() const;
private:
    Q_DISABLE_COPY(ARibbonBackstageSeparator)
};

class ARibbonBackstageButtonPrivate;
class APROCH_API ARibbonBackstageButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(bool tabStyle READ tabStyle WRITE setTabStyle)
public:
    ARibbonBackstageButton(QWidget* parent);
    virtual ~ARibbonBackstageButton();
public:
    bool tabStyle() const;
    void setTabStyle(bool style);
public:
    virtual QSize minimumSizeHint() const override;
    virtual QSize sizeHint() const override;
protected:
    virtual bool event(QEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
private:
    A_DECLARE_PRIVATE(ARibbonBackstageButton)
    Q_DISABLE_COPY(ARibbonBackstageButton)
};

class APROCH_API ARibbonBackstagePage : public QWidget
{
    Q_OBJECT
public:
    explicit ARibbonBackstagePage(QWidget* parent);
    virtual ~ARibbonBackstagePage();
protected:
    virtual void paintEvent(QPaintEvent* event) override;
private:
    Q_DISABLE_COPY(ARibbonBackstagePage)
};


class ARibbonBackstageViewPrivate;
/* ARibbonBackstageView */
class APROCH_API ARibbonBackstageView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int closePrevented READ isClosePrevented WRITE setClosePrevented)
public:
    explicit ARibbonBackstageView(QWidget* parent = nullptr);
    virtual ~ARibbonBackstageView();
public:
    bool isClosePrevented() const;
    void setClosePrevented(bool prevent);

    QAction* addAction(const QIcon& icon, const QString& text);
    QAction* addSeparator();
    QAction* addPage(QWidget* widget);

    QWidget* activePage() const;
    void setActivePage(QWidget* widget);

    QRect actionGeometry(QAction* action) const;
    QRect menuGeometry() const;

    using QWidget::addAction;
public Q_SLOTS:
    void open();
Q_SIGNALS:
    void aboutToShow();
    void aboutToHide();
protected:
    virtual bool event(QEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual bool eventFilter(QObject* object, QEvent* event) override;
    virtual void actionEvent(QActionEvent* event) override;
    virtual void showEvent(QShowEvent* event) override;
    virtual void hideEvent(QHideEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
private:
    friend class ARibbonBackstageViewMenu;
    friend class ARibbonBar;
    A_DECLARE_PRIVATE(ARibbonBackstageView)
    Q_DISABLE_COPY(ARibbonBackstageView)
};

APROCH_NAMESPACE_END