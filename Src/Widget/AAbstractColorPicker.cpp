/****************************************************************************
 * @file    AAbstractColorPicker.cpp
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
#include "AAbstractColorPicker.h"

namespace aproch
{
    AAbstractColorPicker::AAbstractColorPicker(const QColor &initColor, QWidget *parent)
#ifndef QT_NO_OPENGL
        : QOpenGLWidget(parent), QOpenGLFunctions(), mShaderProgram(nullptr)
#else
        : QWidget(parent)
#endif
            ,
            mIndicator(nullptr), mDefaultColor(initColor), mCurrentColor(initColor), mPaddings(0, 0, 0, 0), mIsMousePressed(false), mIsSetColorForIndicator(false)
    {
        setAttribute(Qt::WA_TranslucentBackground);

#ifndef QT_NO_OPENGL
        QSurfaceFormat surfaceFormat;
        surfaceFormat.setSamples(4);
        setFormat(surfaceFormat);
#endif
    }

    AAbstractColorPicker::~AAbstractColorPicker()
    {
    }

    void AAbstractColorPicker::isSetColorForIndicator(bool enabled)
    {
        mIsSetColorForIndicator = enabled;
        if (mIsSetColorForIndicator)
        {
            mIndicator->updateTooltip(mCurrentColor);
            mIndicator->setColor(mCurrentColor);
        }
    }

    void AAbstractColorPicker::setColor(const QColor &color)
    {
        if (mCurrentColor == color)
            return;

        mCurrentColor = color;

        if (nullptr != mIndicator)
        {
            const QPoint p = getColorPixel(mCurrentColor);
            updateIndicator(p.x(), p.y());
        }

        update();
        emit colorChanged();
    }

    void AAbstractColorPicker::setIndicatorPos(int x, int y)
    {
        if (nullptr != mIndicator)
        {
            mCurrentColor = getPixelColor(x, y);
            updateIndicator(x, y);
        }
    }

    QPoint AAbstractColorPicker::getIndicatorPos() const
    {
        if (nullptr != mIndicator)
            return mIndicator->geometry().center();
        return rect().center();
    }

    bool AAbstractColorPicker::event(QEvent *ev)
    {
        switch (ev->type())
        {
        case QEvent::MouseButtonDblClick:
            mIsMousePressed = true;
            break;
        case QEvent::MouseButtonRelease:
            mIsMousePressed = false;
            break;
        case QEvent::MouseButtonPress:
            mIsMousePressed = true;
        case QEvent::MouseMove:
        {
            if (mIsMousePressed)
            {
                const auto &position = ((QMouseEvent *)ev)->pos();
                const auto &oldPos = getIndicatorPos();
                if (oldPos != position)
                {
                    setIndicatorPos(position.x(), position.y());
                    emit indicatorPosChanged();
                    return true;
                }
            }
        }
        break;
        default:
            break;
        }
        return QWidget::event(ev);
    }

    void AAbstractColorPicker::updateIndicator(int x, int y, bool isStopAniForced)
    {
        mIndicator->moveTo(x, y, isStopAniForced);
        mIndicator->updateTooltip(mCurrentColor);
        if (mIsSetColorForIndicator)
            mIndicator->setColor(mCurrentColor);
    }

#ifndef QT_NO_OPENGL
    void AAbstractColorPicker::initializeGL()
    {
        initializeOpenGLFunctions();
    }

    void AAbstractColorPicker::resizeGL(int x, int y)
    {
        const QPoint p = getColorPixel(mCurrentColor);
        updateIndicator(p.x(), p.y(), true);
    }

    bool AAbstractColorPicker::setShader(const QString &vtxFileName, const QString &fragFileName)
    {
        // 创建顶点着色器
        QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
        if (!vshader->compileSourceFile(vtxFileName))
        {
            vshader->deleteLater();
            return false;
        }

        // 创建片段着色器
        QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
        if (!fshader->compileSourceFile(fragFileName))
        {
            fshader->deleteLater();
            return false;
        }

        if (nullptr != mShaderProgram)
        {
            mShaderProgram->deleteLater();
            mShaderProgram = nullptr;
        }

        mShaderProgram = new QOpenGLShaderProgram(this);
        mShaderProgram->addShader(vshader);
        mShaderProgram->addShader(fshader);
        mShaderProgram->link();
        mShaderProgram->bind();

        return true;
    }
#endif

    void AAbstractColorPicker::resetColor()
    {
        setColor(mDefaultColor);
    }
}
