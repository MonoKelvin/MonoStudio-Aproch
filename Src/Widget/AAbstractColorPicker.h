/****************************************************************************
 * @file    AAbstractColorPicker.h
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
#pragma once

#ifndef QT_NO_OPENGL
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QopenGLShaderProgram>
#include <QOpenGLBuffer>
#endif

namespace aproch
{
    class AColorPickerIndicator;

    /**
     * @brief 颜色取色器抽象基类
     */
    class APROCH_API AAbstractColorPicker
#ifndef QT_NO_OPENGL
        : public QOpenGLWidget,
            protected QOpenGLFunctions
#else
        : public QWidget
#endif
    {
        Q_OBJECT
        Q_DISABLE_COPY_MOVE(AAbstractColorPicker)
        Q_PROPERTY(QColor color READ getColor WRITE setColor NOTIFY colorChanged)
        Q_PROPERTY(QColor defaultColor READ getDefaultColor WRITE setDefaultColor)
    public:
        explicit AAbstractColorPicker(const QColor &initColor = Qt::white, QWidget *parent = nullptr);
        virtual ~AAbstractColorPicker();

        /**
         * @brief 设置是否同步改变指示器的颜色
         * @param enabled 是否开启
         */
        void isSetColorForIndicator(bool enabled);

        /**
         * @brief 获取默认颜色
         * @return 颜色
         */
        inline QColor getDefaultColor() const noexcept
        {
            return mDefaultColor;
        }

        /**
         * @brief 设置默认颜色
         * @return 颜色
         */
        inline void setDefaultColor(const QColor &color)
        {
            mDefaultColor = color;
        }

        /**
         * @brief 获取像素位置的颜色
         * @param x X坐标
         * @param y Y坐标
         * @return 颜色
         */
        virtual QColor getPixelColor(int x, int y) const = 0;

        /**
         * @brief 获取颜色所在的像素位置
         * @param color 颜色
         * @return 坐标
         */
        virtual QPoint getColorPixel(const QColor &color) const = 0;

        /**
         * @brief 设置指示器位置
         * @param x x坐标
         * @param y y坐标
         */
        virtual void setIndicatorPos(int x, int y);

        /**
         * @brief 获取指示器位置
         * @return 坐标
         */
        virtual QPoint getIndicatorPos() const;

        /**
         * @brief 获取当前颜色
         * @return 当前颜色
         */
        inline QColor getColor() const
        {
            return mCurrentColor;
        }

        /**
         * @brief 设置当前颜色
         * @param color 颜色
         */
        void setColor(const QColor &color);

        /**
         * @brief 获取指示器控件
         * @return 指示器控件
         */
        inline AColorPickerIndicator *getIndicator() const noexcept
        {
            return mIndicator;
        }

        /**
         * @brief 设置内边距
         * @param paddings 内边距
         */
        inline void setPaddings(const QMargins &paddings)
        {
            mPaddings = paddings;
        }

    Q_SIGNALS:
        /**
         * @brief 信号：当前颜色改变了
         */
        void colorChanged();

        /**
         * @brief 信号：指示器位置改变了
         */
        void indicatorPosChanged();

    public Q_SLOTS:
        /**
         * @brief 还原颜色到默认值
         */
        void resetColor();

    protected:
        virtual bool event(QEvent *ev) override;

#ifndef QT_NO_OPENGL
        virtual void initializeGL() override;
        virtual void resizeGL(int x, int y) override;

        /**
         * @brief 设置着色器
         * @param vtxFileName 顶点着色器文件路径
         * @param fragFileName 片元着色器文件路径
         */
        bool setShader(const QString &vtxFileName, const QString &fragFileName);

    protected:
        /** @brief 着色器程序 */
        QOpenGLShaderProgram *mShaderProgram;

        /** @brief 顶点缓冲对象 */
        QOpenGLBuffer mVBO;
#else
        /**
         * @brief 绘制色盘
         * @param ev 绘图事件
         */
        virtual void paintEvent(QPaintEvent *ev) override = 0;
#endif
        /**
         * @brief 更新指示器
         * @param x X坐标
         * @param y Y坐标
         * @param isStopAniForced 是否强制停止动画
         */
        void updateIndicator(int x, int y, bool isStopAniForced = false);

    protected:
        /** @brief 指示器控件 */
        AColorPickerIndicator *mIndicator;

        /** @brief 默认颜色 */
        QColor mDefaultColor;

        /** @brief 当前颜色 */
        QColor mCurrentColor;

        /** @brief 内边距 */
        QMargins mPaddings;

        /** @brief 鼠标是否按下 */
        bool mIsMousePressed;

        /** @brief 是否给指示器同步颜色 */
        bool mIsSetColorForIndicator;
    };
}
