/****************************************************************************
 * @file    AAToggleSwitch.h
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
#include <QAbstractButton>

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class AToggleSwitchPrivate;
class APROCH_API AToggleSwitch : public QAbstractButton
{
    Q_OBJECT;
    Q_PROPERTY(bool on READ isOn WRITE setOn);
public:
    enum Shape
    {
        Rectangle,
        RectangleSmallGroove,
        Rounded,
        RoundedSmallGroove,
        Circle,
        CircleSmallGroove,
    };

public:
    explicit AToggleSwitch(QAction* action, QWidget* parent = nullptr);
    explicit AToggleSwitch(QWidget* parent = nullptr);
    virtual ~AToggleSwitch();

public:
    bool isOn() const;

    Qt::Alignment textAlignment() const;
    void setTextAlignment(Qt::Alignment alignment);

    QString labelOff() const;
    void setLabelOff(const QString& off);

    QString labelOn() const;
    void setLabelOn(const QString& on);

    bool isAnimated() const;
    void setAnimated(bool animated);

    bool isVisibleLabels() const;
    void setVisibleLabels(bool visible = true);

    Shape shape() const;
    void setShape(Shape style);

    QAction* defaultAction() const;
    void setDefaultAction(QAction* action);

public Q_SLOTS:
    void setOn(bool set = true);

public:
    virtual QSize sizeHint() const override;
    virtual QSize minimumSizeHint() const override;

protected:
    virtual bool event(QEvent* event) override;
    virtual void changeEvent(QEvent* event) override;
    virtual void actionEvent(QActionEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
private:
    A_DECLARE_PRIVATE(AToggleSwitch);
    Q_DISABLE_COPY(AToggleSwitch);
};

APROCH_NAMESPACE_END