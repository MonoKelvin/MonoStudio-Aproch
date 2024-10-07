/****************************************************************************
 * @file    AMenu.h
 * @date    2024-10-04 
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
#include "AprochExportDefine.h"
#include <QMenu>
#include <QAction>

APROCH_NAMESPACE_BEGIN

class AMenuPrivate;

class APROCH_API AMenu : public QMenu
{
    Q_OBJECT;
    Q_PROPERTY(qreal shadowRadius READ getShadowRadius WRITE setShadowRadius);
    Q_PROPERTY(QSize shadowOffset READ getShadowOffset WRITE setShadowOffset);
    Q_PROPERTY(QColor shadowColor READ getShadowColor WRITE setShadowColor);
    Q_PROPERTY(QMargins shadowMargin READ getShadowMargin WRITE setShadowMargin);
public:
    explicit AMenu(QWidget* parent = nullptr);
    explicit AMenu(const QString& title, QWidget* parent = nullptr);
    ~AMenu();

    AMenu* addMenu();
    AMenu* addMenu(const QString& title);

    qreal getShadowRadius() const;
    void setShadowRadius(qreal r);

    QSize getShadowOffset() const;
    void setShadowOffset(const QSize& offset);

    QColor getShadowColor() const;
    void setShadowColor(const QColor& c);

    QMargins getShadowMargin() const;
    void setShadowMargin(const QMargins& m);

protected:
    virtual bool event(QEvent*) override;
    virtual bool eventFilter(QObject*, QEvent*) override;
    virtual void paintEvent(QPaintEvent*) override;

private:
    Q_DISABLE_COPY(AMenu);
    friend class AMenuPrivate;
    QSharedPointer<AMenuPrivate> d;
};


APROCH_NAMESPACE_END
