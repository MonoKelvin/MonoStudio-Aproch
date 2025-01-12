/****************************************************************************
 * @file    AFontIcon.cpp
 * @date    2021-10-27
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
#include "AFontIcon.h"
#include "AFontDatabase.h"

#include <QApplication>
#include <QScreen>

APROCH_NAMESPACE_BEGIN

QSize AFontIcon::DefaultIconSize = QSize(22, 22);

class AFontIconPrivate
{
public:
    void init(AFontIcon* q, const QString& content, const QFont& font)
    {
        glyph = content;

        q->setFont(font);
        q->setAttribute(Qt::WA_StyledBackground);
        q->resize(38, 38);
    }

    static QColor getColorForTheme()
    {
        EThemeType type = ATheme::getTheme();
        if (type == EThemeType::System)
            type = ATheme::getSystemTheme();
        else if (type == EThemeType::Custom)
            type = EThemeType::Light;

        QColor color;
        if (type == EThemeType::Light)
            color = QApplication::palette().color(QPalette::WindowText);
        else
            color = Qt::white;
        return color;
    }

    QString glyph;
};

AFontIcon::AFontIcon(const QString& glyph, QWidget* parent)
    : AFontIcon(glyph, AFontDatabase::SegoeFluentIcons, parent)
{
}

AFontIcon::AFontIcon(const QString& glyph, AFontDatabase::EAppFontType type, QWidget* parent)
    : QWidget(parent)
    , d_ptr(new AFontIconPrivate())
{
    QFont font = AFontDatabase::getFont(type);
    font.setPixelSize(16);
    d_ptr->init(this, glyph, font);
}

AFontIcon::AFontIcon(const QString& glyph, const QFont& font, QWidget* parent)
    : QWidget(parent)
    , d_ptr(new AFontIconPrivate())
{
    d_ptr->init(this, glyph, font);
}

QString AFontIcon::getGlyph() const
{
    return d_ptr->glyph;
}

void AFontIcon::setGlyph(const QString& glyph)
{
    if (d_ptr->glyph == glyph)
        return;

    d_ptr->glyph = glyph;
    emit glyphChanged();
}

QIcon AFontIcon::icon(const QString& glyph, const QSize& size)
{
    return icon(glyph, AFontDatabase::getDefaultIconFont(), AFontIconPrivate::getColorForTheme(), size);
}

QIcon AFontIcon::icon(const QString& glyph, AFontDatabase::EAppFontType type, const QSize& size)
{
    return icon(glyph, type, AFontIconPrivate::getColorForTheme(), size);
}

QIcon AFontIcon::icon(const QString& glyph, AFontDatabase::EAppFontType type, const QColor& color, const QSize& size)
{
    return icon(glyph, AFontDatabase::getFont(type), color, size);
}

QIcon AFontIcon::icon(const QString& glyph, const QFont& font, const QSize& size)
{
    return icon(glyph, font, AFontIconPrivate::getColorForTheme(), size);
}

QIcon AFontIcon::icon(const QString& glyph, const QFont& font, const QColor& color, const QSize& size)
{
    qreal screenRatio = QApplication::primaryScreen()->devicePixelRatio();
    QImage iconImg(size * screenRatio, QImage::Format_ARGB32);
    iconImg.fill(Qt::transparent);
    iconImg.setDevicePixelRatio(screenRatio);

    QFont realFont = font;
    realFont.setPixelSize(16);

    QTextOption textOp;
    textOp.setAlignment(Qt::AlignCenter);

   /* bool less = false;
    bool greater = false;
    do
    {
        QFontMetrics fm(realFont);
        int s = fm.horizontalAdvance(glyph, textOp);
        if (s < size.width())
        {
            realFont.setPixelSize(realFont.pixelSize() + 1);
            less = true;
        }
        else if (s > size.width())
        {
            realFont.setPixelSize(realFont.pixelSize() - 1);
            greater = true;
        }
        else
            break;

        if (less && greater)
            break;

    } while (true);*/

    QPainter painter(&iconImg);
    painter.setFont(realFont);
    painter.setPen(color);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    painter.setRenderHint(QPainter::VerticalSubpixelPositioning);
#endif
    painter.setRenderHint(QPainter::LosslessImageRendering);
    painter.drawText(QRect(0, 0, size.width(), size.height()), glyph, textOp);

    QIcon icon(QPixmap::fromImage(iconImg));
    return icon;
}

QSize AFontIcon::sizeHint() const
{
    return QSize(38, 38);
}

void AFontIcon::paintEvent(QPaintEvent* evt)
{
    QWidget::paintEvent(evt);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QTextOption textOp;
    textOp.setAlignment(Qt::AlignCenter);

    QRect iconRect = rect().marginsRemoved(QMargins(2, 2, 2, 2));

    painter.drawText(iconRect, getGlyph(), textOp);
}

APROCH_NAMESPACE_END
