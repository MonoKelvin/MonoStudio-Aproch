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

class QLabel;
class QMenuBar;
class QLayout;
class QAbstractButton;

APROCH_NAMESPACE_BEGIN

class ACaptionBarPrivate;

/**
 * @brief 标题栏
 * @note 标题栏内置的控件有：
 * - 图标I（QPushButton）。默认显示为应用图标
 * - 标题控件T（QLabel）。默认显示为应用名称
 * - 菜单栏M（QMenu）。默认创建
 * - 帮助按钮H（QPushButton）。默认不显示
 * - 最小化窗口按钮Mi。默认显示
 * - 最大化和还原窗口按钮Mx。默认显示
 * - 关闭窗口按钮C。默认显示
 * - 附加控件区域A。始终创建
 *
 * 其布局如下：
 *
 * [ I - M - T ----- A ----- H - Mi - Mx - C ]
 *
 */
class APROCH_API ACaptionBar : public QFrame
{
    Q_OBJECT
public:
    explicit ACaptionBar(QWidget *parent = nullptr);
    explicit ACaptionBar(const FWindowCaptionWidgets &captionWidgets, QWidget *parent = nullptr);
    ~ACaptionBar(void);

    /** @brief 重置标题栏控件 */
    void reset(const FWindowCaptionWidgets& widgets);

    QAbstractButton* getIcon() const;
    QMenuBar* getMenuBar() const;
    QLabel* getTitle() const;
    QLayout* getAppendixLayout() const;
    QAbstractButton* getHelpButton() const;
    QAbstractButton* getMinButton() const;
    QAbstractButton* getMaxButton() const;
    QAbstractButton* getCloseButton() const;

    void setIcon(QAbstractButton* icon);
    void setMenuBar(QMenuBar* menuBar);
    void setTitle(QLabel* label);
    void setAppendixLayout(QLayout* layout);
    void setHelpButton(QAbstractButton* btn);
    void setMinButton(QAbstractButton* btn);
    void setMaxButton(QAbstractButton* btn);
    void setCloseButton(QAbstractButton* btn);

    QAbstractButton* takeIcon();
    QMenuBar* takeMenuBar();
    QLabel* takeTitle();
    QLayout* takeAppendixLayout() const;
    QAbstractButton* takeHelpButton();
    QAbstractButton* takeMinButton();
    QAbstractButton* takeMaxButton();
    QAbstractButton* takeCloseButton();

    QWidget* getHostWidget() const;
    void setHostWidget(QWidget* w);

    void setTitleFollowHostWidget(bool on);
    bool isTitleFollowHostWidget() const;

    void setIconFollowHostWidget(bool on);
    bool isIconFollowWindow() const;

Q_SIGNALS:
    void helpRequested();
    void minimizeRequested();
    void maximizeRequested(bool max = false);
    void closeRequested();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

    virtual void titleChanged(const QString& text);
    virtual void iconChanged(const QIcon& icon);

protected:
    ACaptionBar(ACaptionBarPrivate* d, QWidget* parent = nullptr, const FWindowCaptionWidgets& captionWidgets = WindowWidgets);

    QScopedPointer<ACaptionBarPrivate> d_ptr;
};

APROCH_NAMESPACE_END
