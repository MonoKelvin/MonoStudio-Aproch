/****************************************************************************
 * @file    AAvatar.h
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
#pragma once

namespace aproch
{
    /**
     * @brief 用户头像控件
     */
    class APROCH_API AAvatar : public QWidget
    {
        Q_OBJECT

        Q_PROPERTY(QPixmap mAvatar READ getAvatar WRITE setAvatar)
        Q_PROPERTY(bool mStateEnable READ isStateEnable WRITE setStateEnable)
        Q_PROPERTY(QColor mStateColor READ getStateColor WRITE setStateColor)
        Q_PROPERTY(int mStateRadius READ getStateRadius WRITE setStateRadius)
        Q_PROPERTY(QString mName READ getName WRITE setName)
    public:
        explicit AAvatar(QWidget *parent = nullptr);
        explicit AAvatar(const QPixmap &pixmap, QWidget *parent = nullptr);
        ~AAvatar() override;

        /**
         * @brief 设置状态是否开启
         * @param enable 状态是否开启
         */
        void setStateEnable(bool enable);

        /**
         * @brief 是否状态开启
         * @return
         */
        inline bool isStateEnable() const noexcept
        {
            return mStateEnable;
        }

        /**
         * @brief 设置状态颜色
         * @param color 状态颜色
         */
        void setStateColor(const QColor &color);

        /**
         * @brief 获取状态颜色
         * @return 状态颜色
         */
        inline QColor getStateColor() const noexcept
        {
            return mStateColor;
        }

        /**
         * @brief 设置状态半径
         * @param radius 状态半径
         */
        void setStateRadius(int radius);

        /**
         * @brief 获取状态半径
         * @return 状态半径
         */
        inline int getStateRadius() const noexcept
        {
            return mStateRadius;
        }

        /**
         * @brief 设置头像图片
         * @param pixmap 图片
         */
        void setAvatar(const QPixmap &pixmap);

        /**
         * @brief 获取头像
         * @return 图片
         */
        inline const QPixmap &getAvatar() const noexcept
        {
            return mAvatar;
        }

        /**
         * @brief 设置名称
         * @param name 名称
         */
        void setName(const QString &name);

        /**
         * @brief 获取名称
         * @return 名称
         */
        inline QString getName() const noexcept
        {
            return mName;
        }

        /**
         * @brief 设置没有图片时可显示名称的最大个数
         * @param count 个数。不允许超过名称 @see getName 中的字符数量
         * @return
         */
        void setNameShowCount(unsigned int count);

    Q_SIGNALS:
        void onClick(void);

    protected:
        void mousePressEvent(QMouseEvent *event) override;
        void paintEvent(QPaintEvent *event) override;

        /**
         * @brief 绘制头像
         * @param painter 画笔
         * @param rect 举行
         * @param pixmap 图片
         * @param defalultText 默认文本
         */
        void drawAvatar(QPainter *painter, const QRect &rect, const QPixmap &pixmap, const QString &defalultText);

    private:
        /**
         * @brief 初始化
         * @param pixmap 图像
         */
        void _init(const QPixmap &pixmap);

    private:
        /** @brief 头像 */
        QPixmap mAvatar;

        /** @brief 用户名称 */
        QString mName;

        /** @brief 状态颜色 */
        QColor mStateColor;

        /** @brief 状态半径 */
        int mStateRadius;

        /** @brief 没有图片时显示名称的最大个数 */
        unsigned int mNameShowCount;

        /** @brief 状态是否开启 */
        bool mStateEnable;
    };
}
