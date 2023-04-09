/****************************************************************************
 * @file    AAvatar.cpp
 * @date    2022-06-18
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
#include "AAvatar.h"

#include <QPainterPath>

namespace aproch
{
    AAvatar::AAvatar(QWidget* parent)
        : QWidget(parent)
    {
        _init(QPixmap());
    }

    AAvatar::AAvatar(const QPixmap& pixmap, QWidget* parent)
        : QWidget(parent)
    {
        _init(pixmap);
    }

    AAvatar::~AAvatar()
    {
    }

    void AAvatar::setStateEnable(bool enable)
    {
        if (enable == mStateEnable)
            return;

        mStateEnable = enable;
        update();
    }

    void AAvatar::setStateColor(const QColor& color)
    {
        if (color == mStateColor)
            return;

        mStateColor = color;
        update();
    }

    void AAvatar::setStateRadius(int radius)
    {
        if (radius == mStateRadius && radius >= 0)
            return;

        mStateRadius = radius;
        update();
    }

    inline void AAvatar::setAvatar(const QPixmap& pixmap)
    {
        mAvatar = pixmap;
        update();
    }

    void AAvatar::setName(const QString& name)
    {
        if (name == mName)
            return;

        mName = name;
        update();
    }

    void AAvatar::setNameShowCount(unsigned int count)
    {
        if (mNameShowCount == count)
            return;

        mNameShowCount = count;
        update();
    }

    void AAvatar::drawAvatar(QPainter* painter, const QRect& rect, const QPixmap& pixmap, const QString& defalultText)
    {
        QPainterPath path;
        path.addEllipse(rect);

        painter->setRenderHints(QPainter::Antialiasing, true);
        painter->setPen(Qt::NoPen);

        painter->save();
        painter->setBrush(QBrush(palette().background().color()));
        painter->setClipPath(path);

        // 当没有图片时绘制背景色
        if (pixmap.isNull())
        {
            painter->drawRect(rect);

            QString trimmedStr = defalultText.trimmed();
            if (!trimmedStr.isEmpty() && mNameShowCount != 0)
            {
                if (mNameShowCount < trimmedStr.size())
                    trimmedStr = trimmedStr.left(mNameShowCount);

                painter->setFont(font());
                painter->setPen(palette().text().color());
                painter->drawText(rect, Qt::AlignCenter, trimmedStr);
            }
        }
        else
        {
            painter->drawPixmap(rect, pixmap);
        }

        painter->restore();

        if (mStateEnable)
        {
            const qreal offset = 0.8535 * rect.width() - mStateRadius;
            const QRectF rt(offset, offset, 2 * mStateRadius, 2 * mStateRadius);

            painter->setPen(Qt::NoPen);
            painter->setBrush(QBrush(mStateColor));
            painter->drawEllipse(rt);
        }
    }

    void AAvatar::_init(const QPixmap& pixmap)
    {
        mStateColor = QColor("#1EE76E");
        mStateRadius = 6;
        mStateEnable = false;
        mNameShowCount = 1;

        setAutoFillBackground(true);
        setAvatar(pixmap);
        setCursor(Qt::PointingHandCursor);
    }

    void AAvatar::mousePressEvent(QMouseEvent*)
    {
        emit onClick();
    }

    void AAvatar::paintEvent(QPaintEvent* event)
    {
        APROCH_USE_STYLE_SHEET();

        QPainter painter(this);
        drawAvatar(&painter, rect(), mAvatar, mName);
        return QWidget::paintEvent(event);
    }
}
