/****************************************************************************
 * @file    AWheelColorPicker.cpp
 * @date    2021-10-29
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
#include "AWheelColorPicker.h"

APROCH_NAMESPACE_BEGIN

AWheelColorPicker::AWheelColorPicker(const QColor &initColor, QWidget *parent)
    : AAbstractColorPicker(initColor, parent), mBrightness(1.0)
{
    mIndicator = new AColorPickerIndicator(this);
    const int s = qMax(mIndicator->width(), mIndicator->height());
    setAttribute(Qt::WA_StyledBackground);
}

void AWheelColorPicker::setIndicatorPos(int x, int y)
{
    const float hW = float(width()) * 0.5, hH = float(height()) * 0.5;
    const float radius = qMin(hW, hH);
    const QPoint c = rect().center();
    const QVector2D c2p(c.x() - x, c.y() - y);

    // 超出圆盘的边缘时，就将指示器位置设置为边缘位置
    const float l = c2p.length();
    if (l > radius)
    {
        const float d = float(radius) / l;
        x = d * (x - hW) + hW;
        y = hH - d * (hH - y);
    }

    return AAbstractColorPicker::setIndicatorPos(x, y);
}

QPoint AWheelColorPicker::getColorPixel(const QColor &color) const
{
    float h, s, v;
    color.getHsvF(&h, &s, &v);

    // 极坐标转笛卡尔坐标
    float X = 0.0, Y = s;
    if (qFuzzyCompare(h, 0.25f))
    {
        Y = -s;
    }
    else if (!qFuzzyCompare(h, 0.75f))
    {
        const qreal K = tan(h * M_2PI);
        X = s / sqrt(1 + K * K);
        Y = X * K;
        if (0.25 < h && h < 0.75)
            X = -X;
        if ((0.75 < h && h < 1.0) || (0 < h && h < 0.25))
            Y = -Y;
    }

    const qreal Kx = width() * 0.5;
    const qreal Ky = height() * 0.5;
    const qreal R = qMin(Kx, Ky);
    return QPoint(X * R + Kx, Y * R + Ky);
}

QColor AWheelColorPicker::getPixelColor(int x, int y) const
{
    const qreal R = qMin(width(), height()) * 0.5;
    const qreal Kx = width() * 0.5 - R;
    const qreal Ky = height() * 0.5 - R;
    const qreal X = (x - Kx) / R - 1.0;
    const qreal Y = (y - Ky) / R - 1.0;

    // 饱和度
    qreal radius = sqrt(X * X + Y * Y);
    if (radius > 1.0)
        radius = 1.0;

    // 色相
    const qreal angle = atan2(Y, -X) + M_PI;

    return QColor::fromHsvF(angle / M_2PI, radius, mBrightness);
}

void AWheelColorPicker::setBrightness(float brightness)
{
    brightness = qBound(0.0f, brightness, 1.0f);
    mBrightness = brightness;
    update();
}

#ifndef QT_NO_OPENGL
void AWheelColorPicker::initializeGL()
{
    AAbstractColorPicker::initializeGL();

    setShader(":/shader/wheel_color_picker.vert", ":/shader/wheel_color_picker.frag");
}

void AWheelColorPicker::paintGL()
{
    if (nullptr == mShaderProgram)
        return;

    const int w = width();
    const int h = height();
    const int side = qMin(w, h);
    glViewport((w - side) / 2, (h - side) / 2, side, side);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    constexpr int vertexCount = 4;
    GLfloat vertices[] = {
        -1.0, -1.0,
        1.0, -1.0,
        1.0, 1.0,
        -1.0, 1.0};

    // 顶点
    /*const float delta = 2.0 * M_PI / vertexCount;
    GLfloat* vertices = new GLfloat[vertexCount * 2];
    for (int i = 0; i < vertexCount * 2; i += 2)
    {
        vertices[i] = cos(delta * i);
        vertices[i + 1] = sin(delta * i);
    }*/

    mVBO.create();
    mVBO.bind();
    mVBO.allocate(vertices, vertexCount * 2 * sizeof(GLfloat));

    const GLuint vPosition = mShaderProgram->attributeLocation("vPosition");
    mShaderProgram->setAttributeBuffer(vPosition, GL_FLOAT, 0, 2);
    glEnableVertexAttribArray(vPosition);

    mShaderProgram->setUniformValue("uResolution", QVector2D(w, h));
    mShaderProgram->setUniformValue("fBrightness", GLfloat(mBrightness));
    // mShaderProgram->setUniformValue("v4Paddings", QVector4D(mPaddings.left(), mPaddings.top(), mPaddings.right(), mPaddings.bottom()));

    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

    // delete[] vertices;
    // vertices = nullptr;

    QRegion region((w - side) / 2 + 1, (h - side) / 2 + 1, side - 2, side - 2, QRegion::Ellipse);
    setMask(region);
}
#else
void AWheelColorPicker::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    const QVector2D center(rect().center());
    int radius = qMin(width() / 2, height() / 2);

    painter.translate(width() * 0.5, height() * 0.5);
    for (int x = -radius; x < radius; ++x)
    {
        for (int y = -radius; y < radius; ++y)
        {
            const float k = float(qMax(abs(x), abs(y))) / sqrt(float(x * x + y * y));

            painter.setPen(getPixelColor(x + radius, y + radius));
            painter.drawPoint(k * x, k * y);
        }
    }
}
#endif

APROCH_NAMESPACE_END
