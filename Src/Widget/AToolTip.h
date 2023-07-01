/****************************************************************************
 * @file    AToolTip.h
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

#include <QFrame>
#include <QTimer>
#include <QIcon>
#include <QEvent>

APROCH_NAMESPACE_BEGIN

class AToolTipPrivate;
class APROCH_API AToolTip : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
    Q_PROPERTY(int margin READ margin WRITE setMargin)
public:
    AToolTip(const QString& title, const QString& text, const QIcon& icon = QIcon(), QWidget* parent = nullptr);
    virtual ~AToolTip();
    static AToolTip* instance();
public:
    static void showToolTip(const QPoint& pos, const QString& title, const QString& text, const QIcon& icon = QIcon(), QWidget* parent = nullptr);
    static void hideToolTip();
    static bool isToolTipVisible();

    enum WrapMode
    {
        NoWrap = 0, 
        NativeWrap = NoWrap,  
        AutoWrap,
    };

    static void setWrapMode(WrapMode mode);
    static WrapMode wordWrap();

    static void setIconVisible(bool visible);
    static bool isIconVisible();
public:
    int margin() const;
    void setMargin(int);

    const QString& title() const;
    const QString& text() const;

    void setIcon(const QIcon &icon);
    const QIcon& icon() const;
public:
    void reuseTip(const QString& textTitle, const QString& text);
    void hideTip();
    void closeToolTip();
    void setTipRect(QWidget* w, const QRect& r);
    void restartExpireTimer();
    bool tipChanged(const QPoint& pos, const QString& text, const QString& textTitle, QObject* o);
    void placeTip(const QPoint& pos, QWidget* w);
public Q_SLOTS:
    void setTitle(const QString&);
    void setText(const QString&);
public:
    virtual QSize sizeHint() const;
protected:
    virtual bool event(QEvent* event);
    virtual bool eventFilter(QObject*, QEvent* event);
    virtual void timerEvent(QTimerEvent* event);
    virtual void paintEvent(QPaintEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
public:
    bool fadingOut() const;
private:
    A_DECLARE_PRIVATE(AToolTip)
    Q_DISABLE_COPY(AToolTip)
};

APROCH_NAMESPACE_END