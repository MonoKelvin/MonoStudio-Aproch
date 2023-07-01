/****************************************************************************
 * @file    AWindowTitleBar.h
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
#include <QStyleOptionTitleBar>

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class AWindowTitleBarPrivate;

class APROCH_API AWindowTitleBar : public QObject
{
    Q_OBJECT
protected:
    AWindowTitleBar(QWidget* window);

public:
    enum SysButtonKind
    {
        SysButtonHidden,
        SysMenuButton,
        BackButton
    };

    enum WidgetAlign
    {
        AlignLeft,
        AlignRight,
        AlignClient
    };

    static AWindowTitleBar* get(QWidget* window);
    static AWindowTitleBar* find(const QWidget* window);
    virtual ~AWindowTitleBar();

public:
    void setVisible(bool visible);
    bool isVisible() const;
    void setResizible(bool resizible);
    bool isResizible() const;
    void show();
    void hide();
    void update();
    void setWidget(QWidget* widget, WidgetAlign align = AlignLeft);
    QWidget* widget() const;
    bool styledFrame() const;
    void setStyledFrame(bool styled);
    bool blurBehindWindowEnabled() const;
    void setBlurBehindWindowEnabled(bool enabled);
    bool extendViewIntoTitleBar() const;
    void setExtendViewIntoTitleBar(bool extend);
    void setTitleHeight(int height);
    int titleHeight() const;
    void setBorderThickness(int thickness);
    int borderThickness() const;
    bool isSysMenuButtonVisible() const;
    void setSysMenuButtonVisible(bool visible);
    SysButtonKind sysButtonKind() const;
    void setSysButtonKind(SysButtonKind kind);
    const QColor& backgroundColor() const;
    void setBackgroundColor(const QColor& color);
    const QColor& textColor() const;
    void setTextColor(const QColor& color);
    QColor frameColor() const;
    QPalette palette() const;
    void initStyleOption(QStyleOptionTitleBar* option) const;
    bool isQWizardSupports() const;
    void setQWizardSupports(bool supports) const;
    void removeAndDelete();
    QRect systemButtonsRect() const;
    static void startDrag(QWidget* window, const QPoint& pos);

Q_SIGNALS:
    void backRequested();
    void showHelp();

protected Q_SLOTS:
    void windowDestroyed();

private:
    friend class AWindowTitleBarPrivate;
    friend class AMaterialWidget;
    AWindowTitleBarPrivate* m_p;
};

APROCH_NAMESPACE_END
