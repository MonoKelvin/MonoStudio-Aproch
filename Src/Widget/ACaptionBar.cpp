/****************************************************************************
 * @file    ACaptionBar.cpp
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
#include "stdafx.h"
#include "ACaptionBar.h"

namespace aproch
{
    ACaptionBar::ACaptionBar(QWidget* parent /*= nullptr*/)
        : QWidget(parent)
    {
        init(WindowTitle | WindowSystemMenu | WindowControllerButtons);
    }

    ACaptionBar::ACaptionBar(const FWindowCaptionWidgets& widgets, QWidget* parent /*= nullptr*/)
        : QWidget(parent)
    {
        init(widgets);
    }

    ACaptionBar::~ACaptionBar(void)
    {

    }

    void ACaptionBar::setWidgetsVisibility(const FWindowCaptionWidgets& widgets, EWidgetVisiblityState type)
    {
        QBoxLayout* theLayout = qobject_cast<QBoxLayout*>(layout());

        if (widgets & WindowTitle)
        {
            if (setWidgetState(mDefaultWidgets.Title, type))
            {
                mDefaultWidgets.Title = createDefaultTitle();
                theLayout->insertWidget(0, mDefaultWidgets.Title);
            }
        }

        if (widgets & WindowSystemMenu)
        {
            if (setWidgetState(mDefaultWidgets.MenuBar, type))
            {
                mDefaultWidgets.MenuBar = createDefaultMenuBar();
                theLayout->insertWidget((nullptr == mDefaultWidgets.Title) ? 0 : 1, mDefaultWidgets.MenuBar);
            }
        }

        if (widgets & WindowContextHelpButton)
        {
            if (setWidgetState(mDefaultWidgets.BtnHelp, type))
            {
                mDefaultWidgets.BtnHelp = createDefaultWinBtn(0);
                if (nullptr != mDefaultWidgets.MenuBar)
                    theLayout->insertWidget(theLayout->indexOf(mDefaultWidgets.MenuBar) + 1, mDefaultWidgets.BtnHelp);
                else if (nullptr != mDefaultWidgets.Title)
                    theLayout->insertWidget(1, mDefaultWidgets.BtnHelp);
                else
                    theLayout->insertWidget(0, mDefaultWidgets.BtnHelp);
            }
        }

        if (widgets & WindowMinimizeButton)
        {
            if (setWidgetState(mDefaultWidgets.BtnMinimize, type))
            {
                mDefaultWidgets.BtnMinimize = createDefaultWinBtn(1);
                if (nullptr != mDefaultWidgets.BtnMaxRestore)
                    theLayout->insertWidget(theLayout->indexOf(mDefaultWidgets.BtnMaxRestore), mDefaultWidgets.BtnMinimize);
                else if (nullptr != mDefaultWidgets.BtnClose)
                    theLayout->insertWidget(theLayout->indexOf(mDefaultWidgets.BtnClose), mDefaultWidgets.BtnMinimize);
                else
                    theLayout->addWidget(mDefaultWidgets.BtnMinimize);
            }
        }

        if (widgets & WindowMaximizeButton)
        {
            if (setWidgetState(mDefaultWidgets.BtnMaxRestore, type))
            {
                mDefaultWidgets.BtnMinimize = createDefaultWinBtn(2);
                if (nullptr != mDefaultWidgets.BtnClose)
                    theLayout->insertWidget(theLayout->indexOf(mDefaultWidgets.BtnClose), mDefaultWidgets.BtnMinimize);
                else
                    theLayout->addWidget(mDefaultWidgets.BtnMinimize);
            }
        }

        if (widgets & WindowCloseButton)
        {
            if (setWidgetState(mDefaultWidgets.BtnClose, type))
            {
                mDefaultWidgets.BtnClose = createDefaultWinBtn(3);
                theLayout->addWidget(mDefaultWidgets.BtnMinimize);
            }
        }
    }

    void ACaptionBar::paintEvent(QPaintEvent* event)
    {
        APROCH_USE_STYLE_SHEET();

        return __super::paintEvent(event);
    }

    void ACaptionBar::init(const FWindowCaptionWidgets& widgets)
    {
        setMouseTracking(true);

        // 主要的水平布局
        QBoxLayout* mainHLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
        mainHLayout->setSpacing(0);

        // 标题
        mDefaultWidgets.Title = createDefaultTitle();
        mainHLayout->addWidget(mDefaultWidgets.Title);

        // 菜单栏
        mDefaultWidgets.MenuBar = createDefaultMenuBar();
        mainHLayout->addWidget(mDefaultWidgets.MenuBar);

        QSpacerItem* spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding);
        mainHLayout->addSpacerItem(spacerItem);

        // 空闲布局
        mDefaultWidgets.AppendixLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        mDefaultWidgets.AppendixLayout->setMargin(0);
        //mDefaultWidgets.AppendixLayout->setSpacing(AppUIStyle.HorizontalSpacing);
        mainHLayout->addLayout(mDefaultWidgets.AppendixLayout);

        // 控制按钮
        mDefaultWidgets.BtnHelp = createDefaultWinBtn(0);
        mDefaultWidgets.BtnMinimize = createDefaultWinBtn(1);
        mDefaultWidgets.BtnMaxRestore = createDefaultWinBtn(2);
        mDefaultWidgets.BtnClose = createDefaultWinBtn(3);

        // 控制按钮的布局
        mDefaultWidgets.CtrlBtnLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        mDefaultWidgets.CtrlBtnLayout->setMargin(0);
        mDefaultWidgets.CtrlBtnLayout->setSpacing(0);
        mDefaultWidgets.CtrlBtnLayout->addWidget(mDefaultWidgets.BtnHelp);
        mDefaultWidgets.CtrlBtnLayout->addWidget(mDefaultWidgets.BtnMinimize);
        mDefaultWidgets.CtrlBtnLayout->addWidget(mDefaultWidgets.BtnMaxRestore);
        mDefaultWidgets.CtrlBtnLayout->addWidget(mDefaultWidgets.BtnClose);

        mainHLayout->addLayout(mDefaultWidgets.CtrlBtnLayout);
        mainHLayout->setMargin(0);

        // 设置显示隐藏
        setWidgetsVisibility(~widgets, WVS_Hide);
    }

    QLabel* ACaptionBar::createDefaultTitle()
    {
        if (nullptr != mDefaultWidgets.Title)
        {
            mDefaultWidgets.Title->deleteLater();
            mDefaultWidgets.Title = nullptr;
        }

        QLabel* lbTitle = new QLabel(this);
        lbTitle->setAttribute(Qt::WA_TransparentForMouseEvents);
        lbTitle->setPixmap(QPixmap(":/Img/AppIcon/png/aproch_24x24.png"));
        lbTitle->setContentsMargins(10, 4, 10, 4);
        lbTitle->setObjectName(AOBJNAME_WINDOWS_TITLE);
        lbTitle->setAlignment(Qt::AlignCenter);
        return lbTitle;
    }

    QMenuBar* ACaptionBar::createDefaultMenuBar()
    {
        if (nullptr != mDefaultWidgets.MenuBar)
        {
            mDefaultWidgets.MenuBar->deleteLater();
            mDefaultWidgets.MenuBar = nullptr;
        }

        QMenuBar* menuBar = new QMenuBar(this);
        menuBar->setObjectName(AOBJNAME_WINDOWS_MENUBAR);
        menuBar->setAttribute(Qt::WA_TranslucentBackground, true);
        menuBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        return menuBar;
    }

    QPushButton* ACaptionBar::createDefaultWinBtn(int nOp)
    {
        QString propValue = AOBJNAME_BTN_CONTROLLER;
        QPixmap iconPixmap;

        switch (nOp)
        {
        case 0:
            iconPixmap = style()->standardPixmap(QStyle::SP_TitleBarContextHelpButton);
            propValue = AOBJNAME_BTN_HELP;
            if (nullptr != mDefaultWidgets.BtnHelp)
            {
                mDefaultWidgets.BtnHelp->deleteLater();
                mDefaultWidgets.BtnHelp = nullptr;
            }
            break;
        case 1:
            iconPixmap = QPixmap(":/icon/windowbtn_minimize");
            if (nullptr != mDefaultWidgets.BtnMinimize)
            {
                mDefaultWidgets.BtnMinimize->deleteLater();
                mDefaultWidgets.BtnMinimize = nullptr;
            }
            break;
        case 2:
            iconPixmap = QPixmap(":/icon/windowbtn_maximize");
            if (nullptr != mDefaultWidgets.BtnMaxRestore)
            {
                mDefaultWidgets.BtnMaxRestore->deleteLater();
                mDefaultWidgets.BtnMaxRestore = nullptr;
            }
            break;
        case 3:
            iconPixmap = QPixmap(":/icon/windowbtn_close");
            if (nullptr != mDefaultWidgets.BtnClose)
            {
                mDefaultWidgets.BtnClose->deleteLater();
                mDefaultWidgets.BtnClose = nullptr;
            }
            break;
        default:
            break;
        }

        // 控制按钮
        QPushButton* winBtn = new QPushButton(this);
        winBtn->setIcon(iconPixmap);
        //winBtn->setFixedSize(AppUIStyle.WindowControllerSize);
        winBtn->setProperty(APropName_BtnType, propValue);

        return winBtn;
    }

    bool ACaptionBar::setWidgetState(QWidget* widget, EWidgetVisiblityState type)
    {
        if (nullptr == widget)
        {
            if (type == EWidgetVisiblityState::WVS_Show)
                return true;
            return false;
        }

        switch (type)
        {
        case aproch::WVS_Show:
            widget->setHidden(false);
            widget->show();
            break;
        case aproch::WVS_Hide:
            widget->setHidden(true);
            break;
        case aproch::WVS_Invisible:
            // TODO
            widget->setHidden(true);
            break;
        default:
            break;
        }

        widget->updateGeometry();
        return false;
    }
}
