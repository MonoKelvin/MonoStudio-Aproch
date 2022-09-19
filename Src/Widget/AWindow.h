/****************************************************************************
 * @file    AWindow.h
 * @date    2021-1-24
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

#include <QWidget>

class QEventLoop;

namespace aproch
{
    class ACaptionBar;

    /**
     * @brief Aproch窗口风格选项
     */
    struct SWindowStyle
    {
        /** @brief 缩放窗口大小的最大边界厚度 */
        int ResizerThickness;

        /** @brief 背景阴影参数 */
        SShadowParam BackgroundShadow;
    };

    /**
     * @brief 自定义窗口组件
     */
    class APROCH_API AWindow : public QWidget
    {
        Q_OBJECT
    public:
        explicit AWindow(QWidget *parent = nullptr, Qt::WindowType type = Qt::Window);
        virtual ~AWindow();

        /**
         * @brief 以模态窗口的方式显示
         * @note 关闭后会删除该控件
         */
        int showModality();

        /**
         * @brief 设置是否可以缩放缩放窗口
         * @param resizable 是否可以缩放窗口
         */
        void setResizable(bool resizable) noexcept;

        /**
         * @brief 获得窗口是否可以缩放
         * @return 窗口是否可以缩放
         */
        inline bool isResizable(void) const noexcept
        {
            return mIsResizable;
        }

        /**
         * @brief 设置窗口移动属性
         * @param movable 是否可以移动窗口
         *  - true: 如果窗口没有标题栏则设置为true也无效
         *  - false: 让窗口无法移动
         */
        inline void setMovable(bool movable) noexcept
        {
            mIsMovable = movable;
        }

        /**
         * @brief 获取窗口是否可以移动
         * @return 窗口是否可以移动
         */
        inline bool isMovable(void) const noexcept
        {
            return mIsMovable;
        }

        /**
         * @brief 设置是否可以最大化或全屏
         * @param enable 是否可以最大化或全屏
         */
        void setEnableMaximized(bool enable) noexcept;

        /**
         * @brief 窗口是否可以最大化显示
         * @return 是否可以最大化显示
         */
        inline bool isEnableMaximized() const noexcept
        {
            return mIsEnableMaximized;
        }

        /**
         * @brief 设置窗口风格选项
         * @param shadowParam 窗口风格选项
         */
        void setWindowStyle(const SWindowStyle &windowStyleOp);

        /**
         * @brief 获得窗口风格选项
         * @return 窗口风格选项
         */
        inline const SWindowStyle getWindowStyle(void) const noexcept
        {
            return mWindowStyle;
        }

        /**
         * @brief 设置中心控件。可以为nullptr
         * @param widget 中心控件
         */
        void setCentralWidget(QWidget *widget);

        /**
         * @brief 获得中心控件
         * @return 中心控件
         */
        inline QWidget *getCentralWidget(void) const noexcept
        {
            return mCentralWidget;
        }

        /**
         * 获取标题栏
         * @return AMenuBar* 标题栏控件
         */
        inline ACaptionBar *getCaptionBar(void) const noexcept
        {
            return mCaptionBar;
        }

        /**
         * @brief 添加可拖拽窗口的区域控件
         * @param regionWidget 区域控件
         */
        void appendDragRegionWidget(QWidget *regionWidget);

        /**
         * @brief 是否点在可托拽的窗口区域控件内
         * @param point 点
         * @return 在窗口可托拽区域控件返回true，否则返回false
         */
        bool isPointInDragRegion(const QPoint &point) const;

    public Q_SLOTS:
        /**
         * @brief 切换最大化和还原窗口大小（全屏状态或者最小状态不起作用）
         */
        void switchMaxOrOriginalSize();

        /**
         * @brief 设置标题栏的图标
         * @param [in] icon 图标
         */
        void setCaptionIcon(const QIcon &icon);

    protected:
        virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
        virtual void paintEvent(QPaintEvent *ev) override;
        virtual void closeEvent(QCloseEvent *ev) override;

    protected:
        /** @brief 窗口风格 */
        SWindowStyle mWindowStyle;

        /** @brief 可拖动窗口的区域 */
        QWidgetList mDragRegionWidgets;

        // 标题栏
        ACaptionBar *mCaptionBar;

        // 中心控件
        QWidget *mCentralWidget;

        // 主要布局
        QBoxLayout *mMainLayout;

        // 阻塞事件循环，用于实现模态窗口
        QEventLoop *mEventLoop;

        // 是否可以缩放尺寸
        bool mIsResizable;

        // 是否可以移动
        bool mIsMovable;

        /** @brief 是否可以最大化（或全屏） */
        bool mIsEnableMaximized;
    };
}
