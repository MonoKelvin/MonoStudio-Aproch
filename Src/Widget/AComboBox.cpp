/****************************************************************************
 * @file    AComboBox.cpp
 * @date    2024-12-31 
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
#include "stdafx.h"
#include "AComboBox.h"
#include "Animation/ADropDownAnimation.h"

#include <QListView>

APROCH_NAMESPACE_BEGIN

class AComboBoxPrivate
{
public:
    void updateContentMargins(QWidget* cb);

};

void AComboBoxPrivate::updateContentMargins(QWidget* v)
{
    auto ge = qobject_cast<QGraphicsDropShadowEffect*>(v->graphicsEffect());
    if (!ge)
        return;

    const int l = ge->blurRadius() - ge->offset().x();
    const int r = ge->blurRadius() + ge->offset().x();
    const int t = ge->blurRadius() - ge->offset().y();
    const int b = ge->blurRadius() + ge->offset().y();
    const int h_2 = (l + r) / 2;
    const int v_2 = (t + b) / 2;
    v->setContentsMargins(l - h_2, t - v_2, r - h_2, b - v_2);
}

AComboBox::AComboBox(QWidget* parent)
    : QComboBox(parent)
    , d(new AComboBoxPrivate())
{
    setAttribute(Qt::WA_StyledBackground);

    QListView* listView = new QListView(this);
    listView->horizontalScrollBar()->setProperty(APropValue_Single, true);
    listView->verticalScrollBar()->setProperty(APropValue_Single, true);
    setView(listView);

    auto viewFrame = listView->window();
    viewFrame->installEventFilter(this);
    viewFrame->setWindowFlag(Qt::FramelessWindowHint, true);
    viewFrame->setWindowFlag(Qt::NoDropShadowWindowHint, true);
    viewFrame->setAttribute(Qt::WA_TranslucentBackground, true);
    viewFrame->setAutoFillBackground(false);

    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect();
    listView->setGraphicsEffect(shadowEffect);
}

AComboBox::~AComboBox()
{
}

qreal AComboBox::getShadowRadius() const
{
    auto ge = qobject_cast<QGraphicsDropShadowEffect*>(view()->graphicsEffect());
    if (!ge)
        return 0.0;

    return ge->blurRadius();
}

void AComboBox::setShadowRadius(qreal r)
{
    auto ge = qobject_cast<QGraphicsDropShadowEffect*>(view()->graphicsEffect());
    if (!ge)
        return;

    ge->setBlurRadius(r);
    d->updateContentMargins(view());
}

QSize AComboBox::getShadowOffset() const
{
    auto ge = qobject_cast<QGraphicsDropShadowEffect*>(view()->graphicsEffect());
    if (!ge)
        return QSize();

    const QPointF offset = ge->offset();
    return QSize(offset.x(), offset.y());
}

void AComboBox::setShadowOffset(const QSize& offset)
{
    auto ge = qobject_cast<QGraphicsDropShadowEffect*>(view()->graphicsEffect());
    if (!ge)
        return;

    ge->setOffset(offset.width(), offset.height());
    d->updateContentMargins(view());
}

QColor AComboBox::getShadowColor() const
{
    auto ge = qobject_cast<QGraphicsDropShadowEffect*>(view()->graphicsEffect());
    if (!ge)
        return Qt::transparent;

    return ge->color();
}

void AComboBox::setShadowColor(const QColor& c)
{
    auto ge = qobject_cast<QGraphicsDropShadowEffect*>(view()->graphicsEffect());
    if (!ge)
        return;
    ge->setColor(c);
}

bool AComboBox::eventFilter(QObject* watched, QEvent* evt)
{
    if (watched == view()->window())
    {
        QWidget* w = view()->window();
        switch (evt->type())
        {
        case QEvent::Show:
        {
            auto ge = qobject_cast<QGraphicsDropShadowEffect*>(view()->graphicsEffect());
            if (!ge)
                return false;

            QPoint gp = mapToGlobal(QPoint(0, height()));  // 控件左下角映射到全局位置
            int newX = w->x() - (ge->blurRadius() - ge->offset().x()) + 1;
            int newY = gp.y() - (ge->blurRadius() - ge->offset().y());
            bool down = true;
            if (w->y() < gp.y() - height() / 2)
            {
                newY = w->y() + (ge->blurRadius() + ge->offset().y()) - 1;
                down = false;
            }
            w->setGeometry(newX, newY, w->width() + 2 * ge->blurRadius(), w->height());

            aDropDownEffect(view(), down ? EDropDownDirection::Down : EDropDownDirection::Up, 150);
        }
        break;
        default:
            break;
        }
    }

    return QComboBox::eventFilter(watched, evt);
}

APROCH_NAMESPACE_END