/****************************************************************************
 * @file    ACaptionBar.h
 * @date    2021-1-9
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

#define AOBJNAME_WINDOWS_TITLE AStr("aproch_window_title")
#define AOBJNAME_WINDOWS_MENUBAR AStr("aproch_window_menu_bar")
#define AOBJNAME_BTN_CONTROLLER AStr("window_controller")
#define AOBJNAME_BTN_HELP AStr("window_help")

class QLabel;
class QPushButton;

namespace aproch
{
    class AFlowLayout;

    /**
     * @brief 标题栏
     * @note 标题栏内置的控件有：
     * 1. 标题控件T（QLabel）。默认显示为应用图标
     * 2. 菜单栏M（QMenu）。默认创建
     * 3. 帮助按钮H（QPushButton）。默认不显示
     * 4. 最小化窗口按钮Mi。默认显示
     * 5. 最大化和还原窗口按钮Mx。默认显示
     * 6. 关闭窗口按钮C。默认显示
     * 7. 附加控件区域A。始终创建
     *
     * 其布局如下：
     *
     * [ T - M ----- A ----- H - Mi - Mx - C ]
     *
     * 通过@see setWidgetsVisibility 可以控制这些控件的显示、隐藏。 如果某个控件已经被销毁，则当使用
     * @see EWidgetVisiblityState::WVS_Show 显示控件时，将创建默认对应的控件后在显示；而 @see EWidgetVisiblityState::WVS_Hide
     * 或者@see EWidgetVisiblityState::WVS_Invisible 不会重新创建控件。
     *
     * WVS_Invisible 和 WVS_Hide 的区别：前者隐藏控件，但不会取消其所占用的矩形区域，而后者隐藏后其矩形区域也会消失
     *
     */
    class APROCH_API ACaptionBar : public QWidget
    {
        Q_OBJECT
    public:
        explicit ACaptionBar(QWidget *parent = nullptr);
        explicit ACaptionBar(const FWindowCaptionWidgets &widgets, QWidget *parent = nullptr);
        ~ACaptionBar(void);

        /**
         * @brief 获取附件区域的水平布局容器
         * @note 附件区域的布局是标题栏预留的控件区域，开发者可以向里面添加自定义的各种控件
         * @return 附件区域的布局
         */
        inline QBoxLayout *getAppendixLayout() const noexcept
        {
            return mDefaultWidgets.AppendixLayout;
        }

        /**
         * @brief 设置标题栏控件显隐状态
         * @param widgets 控件标识
         * @param type 状态类型
         */
        void setWidgetsVisibility(const FWindowCaptionWidgets &widgets, EWidgetVisiblityState type);

        /**
         * @brief 获取标题控件
         * @return 标题控件
         */
        inline QLabel *getTitle() const noexcept
        {
            return mDefaultWidgets.Title;
        }

        /**
         * @brief 获取菜单栏控件
         * @return 菜单栏控件
         */
        inline QMenuBar *getMenuBar() const noexcept
        {
            return mDefaultWidgets.MenuBar;
        }

        /**
         * @brief 获取帮助按钮控件
         * @return 帮助按钮控件
         */
        inline QPushButton *getHelpButton() const noexcept
        {
            return mDefaultWidgets.BtnHelp;
        }

        /**
         * @brief 获取窗口最小化按钮控件
         * @return 窗口最小化按钮控件
         */
        inline QPushButton *getMinimizeButton() const noexcept
        {
            return mDefaultWidgets.BtnMinimize;
        }

        /**
         * @brief 获取窗口最大还原按钮控件
         * @return 窗口最大还原按钮控件
         */
        inline QPushButton *getMaxRestoreButton() const noexcept
        {
            return mDefaultWidgets.BtnMaxRestore;
        }

        /**
         * @brief 获取窗口关闭按钮控件
         * @return 窗口关闭按钮控件
         */
        inline QPushButton *getCloseButton() const noexcept
        {
            return mDefaultWidgets.BtnClose;
        }

    protected:
        virtual void paintEvent(QPaintEvent *event) override;

    private:
        /**
         * @brief 初始化
         * @param widgets 要添加的控件
         */
        void init(const FWindowCaptionWidgets &widgets);

        /**
         * @brief 设置控件状态
         * @param widget 控件
         * @param type 操作状态的类型
         * @return 返回true，则该控件为不存在，并且需要创建该控件
         */
        bool setWidgetState(QWidget *widget, EWidgetVisiblityState type);

        /**
         * @brief 创建默认标题控件
         * @return 标题控件
         */
        QLabel *createDefaultTitle();

        /**
         * @brief 创建默认菜单栏
         * @return 菜单栏
         */
        QMenuBar *createDefaultMenuBar();

        /**
         * @brief 创建默认窗口按钮
         * @param nOp 选项。0: help, 1: min, 2: max, 3: close
         * @return 按钮
         */
        QPushButton *createDefaultWinBtn(int nOp);

    protected:
        /**
         * @brief 默认控件
         */
        struct SDefaultWidgets
        {
            /** @brief 标题 */
            QLabel *Title;

            /** @brief 菜单栏 */
            QMenuBar *MenuBar;

            /** @brief 附加控件布局 */
            QBoxLayout *AppendixLayout;

            /** @brief 窗口按钮布局 */
            QBoxLayout *CtrlBtnLayout;

            /** @brief 帮助按钮 */
            QPushButton *BtnHelp;

            /** @brief 最小化窗口按钮 */
            QPushButton *BtnMinimize;

            /** @brief 最大还原窗口按钮 */
            QPushButton *BtnMaxRestore;

            /** @brief 关闭窗口按钮 */
            QPushButton *BtnClose;

            SDefaultWidgets() noexcept
                : Title(nullptr), MenuBar(nullptr), AppendixLayout(nullptr), CtrlBtnLayout(nullptr), BtnHelp(nullptr), BtnMinimize(nullptr), BtnMaxRestore(nullptr), BtnClose(nullptr)
            {
            }
        };

        /** @brief 默认控件 */
        SDefaultWidgets mDefaultWidgets;
    };
}
