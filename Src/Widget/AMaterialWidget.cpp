/****************************************************************************
 * @file    AMaterialWidget.cpp
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
#include "stdafx.h"
#include "AMaterialWidget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPaintEngine>
#include <QApplication>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QBackingStore>
#endif

#include "Platform/APlatform.h"
#include "AWindowTitleBar_p.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class AMaterialWidgetEffect : public QGraphicsEffect
{
public:
    AMaterialWidgetEffect(AMaterialWidget* widget);
    virtual ~AMaterialWidgetEffect();
    virtual QRectF boundingRectFor(const QRectF &sourceRect) const;
protected:
    virtual void draw(QPainter *painter);
    virtual void sourceChanged(ChangeFlags flags);
private:
    void forceRenderSiblings(QWidget* widget, QPainter* painter, const QRegion& clip);
    AMaterialWidget* m_widget;
#ifdef A_OPENGL_ENABLED
    OpenGLEffect* m_openglEffect;
#endif
    QImage m_blurredBuffer;
};

/*!
\enum AMaterialWidget::BlendType
Enumerator BlendTyle describes what's visible through the acrylic material widget.
\inmodule QtitanBase

\value DisabledBlend The blending mode is not used. The material widget is not transparent.
\value InAppBlend Through material widget the other widgets are visible within the application that are located under this widget.
\value BackgroundBlend Through the material widget the contents of the desktop are visible which is located under the 
top-level window. For proper operation of this mode, the platform should support blur effect for native top-level windows.
*/

/*!
\class AMaterialWidget
\inmodule QtitanBase
\brief Acrylic material is a special widget that emulates a type of brush that creates a partially transparent texture.

You can add acrylic widget to your Qt application window to add depth and and emphasize the hierarchy visually.
Note: Rendering acrylic material surfaces is GPU intensive, which can slow down the application, 
increase the power consumption on the devices on which the application is running.
Material widget adds Qt::AA_DontCreateNativeWidgetSiblings attribute to the application.
Therefore, this can affect on your application which use native windows - for example, widgets with OpenGL.
*/
AMaterialWidget::AMaterialWidget(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f), m_blend(DisabledBlend),
    m_tintColor(Qt::transparent),
    m_tintOpacity(0.7),
    m_noiseOpacity(0.04),
    m_buffer(0, 0, QImage::Format_ARGB32_Premultiplied)
{
    aproch_initializeDrawingResources();
    qApp->setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

#ifdef Q_OS_MAC
    if (aproch_blurBehindSupported())
        m_tintOpacity = 0.6;
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    AMaterialWidgetEffect* eff = new AMaterialWidgetEffect(this);
    setGraphicsEffect(eff);
#endif
    setAutoFillBackground(false);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setBackgroundRole(QPalette::Base);
    setBlendType(InAppBlend);
    m_acrylicBrush = createAcrylicBrush();
}

AMaterialWidget::~AMaterialWidget()
{
}

/*!
\property AMaterialWidget::blend
This property describes how the acrylic texture should be blended with the background under the current widget. 
\sa AcrylicBlend
*/
AMaterialWidget::BlendType AMaterialWidget::blendType() const
{
    return m_blend;
}

void AMaterialWidget::setBlendType(AMaterialWidget::BlendType blend)
{
    if (m_blend == blend)
        return;
    m_blend = blend;

    setAttribute(Qt::WA_OpaquePaintEvent, m_blend == BackgroundBlend);
    if (graphicsEffect() != nullptr)
        graphicsEffect()->setEnabled(m_blend == InAppBlend);
    update();
}

/*!
\property AMaterialWidget::tintColor
This property holds the color of the tint for acrylic texture.
\sa tintOpacity, noiseOpacity
*/
const QColor& AMaterialWidget::tintColor() const
{
    if (m_tintColor == Qt::transparent)
        return palette().color(backgroundRole());
    else
        return m_tintColor;
}

void AMaterialWidget::setTintColor(const QColor& color)
{
    m_tintColor = color;
    QPalette p = palette();
    QPalette_setColor(p, backgroundRole(), color);
    setPalette(p);
    update();
}

/*!
\property AMaterialWidget::tintOpacity
This property holds the opacity value of the tint for acrylic texture.
\sa tintColor, noiseOpacity
*/
double AMaterialWidget::tintOpacity() const
{
    return m_tintOpacity;
}

void AMaterialWidget::setTintOpacity(double opacity)
{
    if (m_tintOpacity == opacity)
        return;
    m_tintOpacity = opacity;
    m_acrylicBrush = createAcrylicBrush();
    update();
}

/*!
\property AMaterialWidget::noiseOpacity
This property holds the opacity value of the noise for acrylic texture.
\sa tintColor, tintOpacity
*/
double AMaterialWidget::noiseOpacity() const
{
    return m_noiseOpacity;
}

void AMaterialWidget::setNoiseOpacity(double opacity)
{
    if (m_noiseOpacity == opacity)
        return;
    m_noiseOpacity = opacity;
    m_acrylicBrush = createAcrylicBrush();
    update();
}

/*!
Creates acrylic material brush based on widget's settings.
*/
QBrush AMaterialWidget::createAcrylicBrush() const
{
    // TODO
    static QImage noiseTexture = QImage(QStringLiteral(":/res/acrylic_noise.png"));
    QImage acrylicTexture = QImage(QSize(64, 64), QImage::Format_ARGB32_Premultiplied);

    QColor fillColor = Qt::transparent;
#ifdef Q_OS_WIN
    if (!aproch_getWindows10AcrylicSupport())
    {
        //Adds soft light layer for the background.
        fillColor = Qt::white;
        fillColor.setAlpha(150);
    }
#endif
    acrylicTexture.fill(fillColor);
    {
        QPainter p(&acrylicTexture);
        p.setOpacity(tintOpacity());
        p.fillRect(QRect(0, 0, acrylicTexture.width(), acrylicTexture.height()), tintColor());
        p.setOpacity(noiseOpacity());
        p.fillRect(QRect(0, 0, acrylicTexture.width(), acrylicTexture.height()), QBrush(noiseTexture));
    }
    return QBrush(acrylicTexture);
}

/*! \internal */
const QImage& AMaterialWidget::grabBackstageToBuffer(const QRegion& clip, bool& hasChanges)
{
    const QImage::Format format = QImage::Format_ARGB32_Premultiplied;
    hasChanges = false;

    if (m_buffer.size() != aproch_toNativeDPI(size(), this, true))
        m_buffer = QImage(aproch_toNativeDPI(QSize(width(), height()), this, true), format);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    QBackingStore* bs = backingStore();
    QImage* windowBackstage = bs != nullptr && bs->paintDevice()->devType() == QInternal::Image ?
        static_cast<QImage *>(bs->paintDevice()) : nullptr;
#else
    QImage* windowBackstage = nullptr;
#endif
    if (windowBackstage != nullptr)
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        m_buffer.setDevicePixelRatio(windowBackstage->devicePixelRatioF());
#endif

        QPoint offset = mapTo(window(), QPoint(0, 0));
#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
        for (QRegion::const_iterator it = clip.begin(); it !=  clip.end(); ++it)
#else
        QVector<QRect> rcs = clip.rects();
        for (QVector<QRect>::const_iterator it = rcs.constBegin(); it !=  rcs.constEnd(); ++it)
#endif
        {
            const QRect& r = *it;
            QImage part = windowBackstage->copy(aproch_toNativeDPI(r.translated(offset), this, true));
            if (part.format() != format)
                part = part.convertToFormat(format);
            if (aproch_image_copy(m_buffer, aproch_toNativeDPI(r.topLeft(), this, true), part))
                hasChanges = true;
        }
    }
    return m_buffer;
}

/*! \internal */
void AMaterialWidget::setBackstageToWindow(const QImage& newBackstage, const QRegion& clip)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    QBackingStore* bs = backingStore();
    QImage* windowBackstage = bs != nullptr && bs->paintDevice()->devType() == QInternal::Image ?
        static_cast<QImage *>(bs->paintDevice()) : nullptr;
#else
    QImage* windowBackstage = nullptr;
#endif
    if (windowBackstage != nullptr)
    {
        const QImage::Format format = windowBackstage->format();
        QPoint offset = mapTo(window(), QPoint(0, 0));
        if (clip.boundingRect() == rect())
        {
            if (newBackstage.format() != format)
                aproch_image_copy(*windowBackstage, aproch_toNativeDPI(offset, this, true), newBackstage.convertToFormat(format));
            else
                aproch_image_copy(*windowBackstage, aproch_toNativeDPI(offset, this, true), newBackstage);
        }
        else
        {
            //Optimiztion....
#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
            for (QRegion::const_iterator it = clip.begin(); it != clip.end(); ++it)
#else
            QVector<QRect> rcs = clip.rects();
            for (QVector<QRect>::const_iterator it = rcs.constBegin(); it !=  rcs.constEnd(); ++it)
#endif
            {
                const QRect& r = *it;
                QImage part = newBackstage.copy(aproch_toNativeDPI(r, this, true));
                if (part.format() != format)
                    part = part.convertToFormat(format);
                aproch_image_copy(*windowBackstage, 
                    aproch_toNativeDPI(offset + r.topLeft(), this, true), part);
            }
        }
    }
}

/*! \internal */
QRect AMaterialWidget::clipRect() const
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 7, 0))
    QVariant v = QWidget::inputMethodQuery(Qt::ImInputItemClipRectangle);
    return v.toRect();
#else
    return rect();
#endif
}

/*! 
Paints acrylic material texture on \a painter that bounded by \a clip region.
*/
void AMaterialWidget::paintBackground(QPainter* painter, const QRegion& clip)
{
    const QRect rect = QRect(0, 0, width(), height());
    QRegion region = clip.isEmpty() ? rect : clip;
    painter->setClipRegion(region);

    if (blendType() == AMaterialWidget::BackgroundBlend)
    {
        AWindowTitleBar* titleBar = AWindowTitleBar::find(window());
        if (titleBar != nullptr)
            titleBar->m_p->paintMaterialBehindWindow(painter, this);
    }
    else if (blendType() == AMaterialWidget::InAppBlend)
    {
        //Do nothing
    }
    else
    {
        Q_ASSERT(blendType() == AMaterialWidget::DisabledBlend);
        return;
    }
    painter->save();
#if 0
    painter->setCompositionMode(QPainter::CompositionMode_SoftLight);
    painter->setOpacity(0.3);
    painter->fillRect(QRect(0, 0, rect.width(), rect.height()), Qt::white);
#endif
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter->setOpacity(1);
    painter->fillRect(rect, m_acrylicBrush);
    painter->restore();
}

/*! \reimp */
void AMaterialWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    paintBackground(&painter, event->region());
#if 0
    QRect rect = QRect(0, 0, width(), height());
    QPen pen(Qt::darkRed);
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawRect(rect.adjusted(0, 0, -1, -1));
#endif
}

/*! \reimp */
void AMaterialWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
}

/*! \reimp */
void AMaterialWidget::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
}

/*! \reimp */
void AMaterialWidget::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
}

/*! \reimp */
void AMaterialWidget::changeEvent(QEvent * event)
{
    if (event->type() == QEvent::PaletteChange)
        m_acrylicBrush = createAcrylicBrush();
    QWidget::changeEvent(event);
}

/* MaterialWidgetEfffect */
AMaterialWidgetEffect::AMaterialWidgetEffect(AMaterialWidget* widget)
    : QGraphicsEffect(), m_widget(widget)
{
#ifdef A_OPENGL_ENABLED
    m_openglEffect = new OpenGLGaussianBlurEffect;
#endif
}

AMaterialWidgetEffect::~AMaterialWidgetEffect()
{
    m_widget = nullptr;
#ifdef A_OPENGL_ENABLED
    A_DELETE_AND_NULL(m_openglEffect);
#endif
}

QRectF AMaterialWidgetEffect::boundingRectFor(const QRectF &sourceRect) const
{
    return sourceRect;
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
static QRect aproch_marginsRemoved(const QRect& rc, const QMargins& margins)
{
    return QRect(QPoint(rc.left() + margins.left(), rc.top() + margins.top()),
        QPoint(rc.right() - margins.right(), rc.bottom() - margins.bottom()));
}
#endif

void AMaterialWidgetEffect::forceRenderSiblings(QWidget* widget, QPainter* painter, const QRegion& clip)
{
    if (widget == nullptr || widget->parent() == nullptr)
        return;
#ifdef QTITAN_DESIGNER
    if (QLatin1String(widget->metaObject()->className()) == QStringLiteral("Qtitan::NavigationMainWindow"))
        return;
#endif
    QWidget* parent = widget->parentWidget();
    bool find = false;
    for (QObjectList::const_iterator it = parent->children().constBegin();
        it != parent->children().constEnd(); ++it)
    {
        QWidget *w = qobject_cast<QWidget *>(*it);
        if (w == nullptr || w->isWindow() || w->isHidden())
            continue;
        if (!find)
        {
            find = w == widget;
            continue;
        }
#if 0
        for (QRegion::const_iterator it = clip.begin(); it != clip.end(); ++it)
            painter->fillRect(*it, Qt::blue);
#endif
        QPoint offset = w->mapTo(w->window(), QPoint());
        QRegion dirty = clip.translated(-offset).intersected(m_widget->rect());
        if (dirty.isEmpty())
            continue;
        w->render(painter, dirty.boundingRect().topLeft() + offset,
            dirty, QWidget::RenderFlags(QWidget::DrawChildren));
    }
    forceRenderSiblings(parent, painter, clip);
}

void AMaterialWidgetEffect::draw(QPainter *painter)
{
    Q_UNUSED(painter);
    QRegion clip = aproch_fromNativeDPI(
        painter->paintEngine()->systemClip(), m_widget, true);

    QPoint offset = m_widget->mapTo(m_widget->window(), QPoint());
    clip = clip.translated(-offset).intersected(m_widget->rect());

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    QRect windowRect = m_widget->window()->rect().marginsRemoved(m_widget->window()->contentsMargins());
#else
    QRect windowRect = aproch_marginsRemoved(m_widget->window()->rect(), m_widget->window()->contentsMargins());
#endif

    QRect widgetRect = windowRect.translated(-offset).intersected(m_widget->clipRect());

    bool fullyUpdate = false;
    QImage buffer = m_widget->grabBackstageToBuffer(clip, fullyUpdate);
    if (fullyUpdate)
    {
#ifdef A_OPENGL_ENABLED
        m_openglEffect->applyEffectToImage(buffer);
        m_blurredBuffer = buffer;
#else
        if (m_blurredBuffer.size() != buffer.size() ||
            m_blurredBuffer.format() != buffer.format()
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            || m_blurredBuffer.devicePixelRatioF() != buffer.devicePixelRatioF()
#endif
            )
        {
            m_blurredBuffer = QImage(buffer.width(), buffer.height(), buffer.format());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            m_blurredBuffer.setDevicePixelRatio(buffer.devicePixelRatioF());
#endif
        }
        m_blurredBuffer.fill(Qt::transparent);
        {
            QPainter p(&m_blurredBuffer);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            QSize s = buffer.size();
            buffer = buffer.scaled(s / 4);
            qt_blurImage(buffer, 24, false);
            p.scale(4, 4);
            p.setRenderHint(QPainter::SmoothPixmapTransform);
            p.drawImage(QRect(QPoint(0, 0), buffer.size() / buffer.devicePixelRatioF()), buffer);
#else
            qt_blurImage(&p, buffer, 128, false, false);
#endif
        }
#endif
    }

    buffer = m_blurredBuffer;
    QRegion dirty = fullyUpdate ? widgetRect : clip;

    QRegion forcedUpdate = (fullyUpdate && clip != m_widget->rect()) ?
        QRegion(widgetRect) - clip : QRegion();

#if 1
    {
        QPainter p(&buffer);
        p.translate(-offset); //Translate to the window coordinates.
        m_widget->render(&p, dirty.boundingRect().topLeft() + offset,
            dirty.boundingRect(), QWidget::RenderFlags(QWidget::DrawChildren));
        if (!forcedUpdate.isEmpty())
            forceRenderSiblings(m_widget, &p, forcedUpdate.translated(offset));
    }
#endif
    m_widget->setBackstageToWindow(buffer, dirty);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    if (!forcedUpdate.isEmpty())
        m_widget->backingStore()->flush(forcedUpdate.translated(offset), m_widget->windowHandle(), QPoint(0, 0));
#endif
}

void AMaterialWidgetEffect::sourceChanged(ChangeFlags flags)
{
    QGraphicsEffect::sourceChanged(flags);
}

APROCH_NAMESPACE_END
