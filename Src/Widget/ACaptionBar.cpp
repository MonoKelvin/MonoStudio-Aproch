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
#include "Private/ACaptionBar_p.h"

namespace {
    QAbstractButton* createDefaultIcon()
    {
        auto iconBtn = new QPushButton();
        iconBtn->setObjectName(AStr("aproch-captionbar-icon"));
        iconBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        return iconBtn;
    }

    QLabel* createDefaultTitle()
    {
        auto titleLabel = new QLabel();
        titleLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setObjectName(AStr("aproch-captionbar-title"));
        return titleLabel;
    }

    QMenuBar* createDefaultMenuBar()
    {
        QMenuBar* menuBar = new QMenuBar();
        menuBar->setObjectName("aproch-captionbar-menubar");
        menuBar->setAttribute(Qt::WA_TranslucentBackground, true);
        menuBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        return menuBar;
    }

    QPushButton* createDefaultWinBtn(EWindowCaptionWidget type)
    {
        QString objName;
        QIcon icon;
        QPushButton* winBtn = new QPushButton();

        HWND hWnd = GetShellWindow();  // 获取Shell窗口句柄

        switch (type)
        {
        case EWindowCaptionWidget::WindowHelpButton:
            objName = AStr("aproch-captionbar-help");
            break;
        case EWindowCaptionWidget::WindowMinimizeButton:
            icon = QIcon(":/icon/windowbtn_minimize");
            objName = AStr("aproch-captionbar-min");
            break;
        case EWindowCaptionWidget::WindowMaximizeButton:
            icon = QIcon(":/icon/windowbtn_maximize");
            winBtn->setCheckable(true);
            objName = AStr("aproch-captionbar-max");
            break;
        case EWindowCaptionWidget::WindowCloseButton:
            icon = QIcon(":/icon/windowbtn_close");
            objName = AStr("aproch-captionbar-close");
            break;
        default:
            break;
        }

        winBtn->setIcon(icon);
        winBtn->setProperty("aproch-system-button", true);
        winBtn->setObjectName(objName.toLatin1());
        winBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

        return winBtn;
    }

}

APROCH_NAMESPACE_BEGIN

void ACaptionBarPrivate::init(const FWindowCaptionWidgets& widgets)
{
    mainLayout = new QHBoxLayout();
    if (QLocale::system().textDirection() == Qt::RightToLeft)
        mainLayout->setDirection(QBoxLayout::RightToLeft);

    mainLayout->setContentsMargins(QMargins());
    mainLayout->setSpacing(0);

    // 初始化占位符
    for (int i = _widget2Index(EWindowCaptionWidget::WindowIcon); 
         i <= _widget2Index(EWindowCaptionWidget::WindowCloseButton);
         ++i)
        insertDefaultSpace(i);

    // init widgets
    {
        if (widgets & EWindowCaptionWidget::WindowIcon)
            q_ptr->setIcon(createDefaultIcon());
        if (widgets & EWindowCaptionWidget::WindowMenu)
            q_ptr->setMenuBar(createDefaultMenuBar());
        if (widgets & EWindowCaptionWidget::WindowTitle)
            q_ptr->setTitle(createDefaultTitle());

        if (widgets & EWindowCaptionWidget::WindowAppendixLayout)
        {
            auto AppendixLayout = new QBoxLayout(QBoxLayout::LeftToRight);
            AppendixLayout->setContentsMargins(QMargins());
            AppendixLayout->setSpacing(0);
            q_ptr->setAppendixLayout(AppendixLayout);
        }

        // 控制按钮
        if (widgets & EWindowCaptionWidget::WindowHelpButton)
            q_ptr->setHelpButton(createDefaultWinBtn(EWindowCaptionWidget::WindowHelpButton));
        if (widgets & EWindowCaptionWidget::WindowMinimizeButton)
            q_ptr->setMinButton(createDefaultWinBtn(EWindowCaptionWidget::WindowMinimizeButton));
        if (widgets & EWindowCaptionWidget::WindowMaximizeButton)
            q_ptr->setMaxButton(createDefaultWinBtn(EWindowCaptionWidget::WindowMaximizeButton));
        if (widgets & EWindowCaptionWidget::WindowCloseButton)
            q_ptr->setCloseButton(createDefaultWinBtn(EWindowCaptionWidget::WindowCloseButton));
    }

    q_ptr->setLayout(mainLayout);
}

void ACaptionBarPrivate::setWidgetAt(EWindowCaptionWidget type, QWidget* widget)
{
    const int index = _widget2Index(type);
    auto item = mainLayout->takeAt(index);
    auto orgWidget = item->widget();
    if (orgWidget)
        orgWidget->deleteLater();

    delete item;
    if (!widget)
    {
        insertDefaultSpace(index);
    }
    else
    {
        mainLayout->insertWidget(index, widget);
    }
}

QWidget* ACaptionBarPrivate::takeWidgetAt(EWindowCaptionWidget type)
{
    const int index = _widget2Index(type);
    auto item = mainLayout->itemAt(index);
    auto orgWidget = item->widget();
    if (orgWidget)
    {
        item = mainLayout->takeAt(index);
        delete item;
        insertDefaultSpace(index);
    }
    return orgWidget;
}

void ACaptionBarPrivate::setAppendixLayout(QLayout* layout)
{
    const int index = _widget2Index(EWindowCaptionWidget::WindowAppendixLayout);
    auto item = mainLayout->takeAt(index);
    auto orgLayout = item->layout();
    if (orgLayout)
        orgLayout->deleteLater();

    delete item;
    if (!layout)
    {
        insertDefaultSpace(index);
    }
    else
    {
        // 确保将布局脱离标题栏控件，否则insertLayout无法正确插入
        if (layout->parentWidget() == q_ptr)
            layout->setParent(nullptr);
        mainLayout->insertLayout(index, layout);
    }
}

void ACaptionBarPrivate::takeAppendixLayout()
{
    const int index = _widget2Index(EWindowCaptionWidget::WindowAppendixLayout);
    auto item = mainLayout->itemAt(index);
    auto orgLayout = item->layout();
    if (orgLayout)
    {
        item = mainLayout->takeAt(index);
        delete item;
        insertDefaultSpace(index);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ACaptionBar::ACaptionBar(QWidget* parent)
    : ACaptionBar(new ACaptionBarPrivate(), parent)
{
}

ACaptionBar::ACaptionBar(const FWindowCaptionWidgets& captionWidgets, QWidget* parent)
    : ACaptionBar(new ACaptionBarPrivate(), parent, captionWidgets)
{
}

ACaptionBar::ACaptionBar(ACaptionBarPrivate* d, QWidget* parent, const FWindowCaptionWidgets& captionWidgets)
    : QFrame(parent), d_ptr(d)
{
    setObjectName(AStr("aproch-captionbar"));

    d_ptr->q_ptr = this;
    d_ptr->init(captionWidgets);
}

ACaptionBar::~ACaptionBar(void)
{
}

void ACaptionBar::reset(const FWindowCaptionWidgets& widgets)
{
    if (widgets & EWindowCaptionWidget::WindowIcon)
        setIcon(createDefaultIcon());
    else
        setIcon(nullptr);

    if (widgets & EWindowCaptionWidget::WindowMenu)
        setMenuBar(createDefaultMenuBar());
    else
        setMenuBar(nullptr);

    if (widgets & EWindowCaptionWidget::WindowTitle)
        setTitle(createDefaultTitle());
    else
        setMenuBar(nullptr);

    //if (widgets & EWindowCaptionWidget::WindowAppendixLayout)
    {
        auto AppendixLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        AppendixLayout->setContentsMargins(QMargins());
        AppendixLayout->setSpacing(0);
        setAppendixLayout(AppendixLayout);
    }

    // 控制按钮
    if (widgets & EWindowCaptionWidget::WindowHelpButton)
        setHelpButton(createDefaultWinBtn(EWindowCaptionWidget::WindowHelpButton));
    else
        setHelpButton(nullptr);
    if (widgets & EWindowCaptionWidget::WindowMinimizeButton)
        setMinButton(createDefaultWinBtn(EWindowCaptionWidget::WindowMinimizeButton));
    else
        setMinButton(nullptr);
    if (widgets & EWindowCaptionWidget::WindowMaximizeButton)
        setMaxButton(createDefaultWinBtn(EWindowCaptionWidget::WindowMaximizeButton));
    else
        setMaxButton(nullptr);
    if (widgets & EWindowCaptionWidget::WindowCloseButton)
        setCloseButton(createDefaultWinBtn(EWindowCaptionWidget::WindowCloseButton));
    else
        setCloseButton(nullptr);
}

QAbstractButton* ACaptionBar::getIcon() const
{
    return qobject_cast<QAbstractButton*>(d_ptr->widgetAt(EWindowCaptionWidget::WindowIcon));
}

QLabel* ACaptionBar::getTitle() const
{
    return qobject_cast<QLabel*>(d_ptr->widgetAt(EWindowCaptionWidget::WindowTitle));
}

QMenuBar* ACaptionBar::getMenuBar() const
{
    return qobject_cast<QMenuBar*>(d_ptr->widgetAt(EWindowCaptionWidget::WindowMenu));
}

QLayout* ACaptionBar::getAppendixLayout() const
{
    return d_ptr->appendixLayout();
}

QAbstractButton* ACaptionBar::getHelpButton() const
{
    return qobject_cast<QAbstractButton*>(d_ptr->widgetAt(EWindowCaptionWidget::WindowHelpButton));
}

QAbstractButton* ACaptionBar::getMinButton() const
{
    return qobject_cast<QAbstractButton*>(d_ptr->widgetAt(EWindowCaptionWidget::WindowMinimizeButton));
}

QAbstractButton* ACaptionBar::getMaxButton() const
{
    return qobject_cast<QAbstractButton*>(d_ptr->widgetAt(EWindowCaptionWidget::WindowMaximizeButton));
}

QAbstractButton* ACaptionBar::getCloseButton() const
{
    return qobject_cast<QAbstractButton*>(d_ptr->widgetAt(EWindowCaptionWidget::WindowCloseButton));
}

void ACaptionBar::setIcon(QAbstractButton* icon)
{
    auto org = takeIcon();
    if (org)
        org->deleteLater();
    if (!icon)
        return;
    d_ptr->setWidgetAt(EWindowCaptionWidget::WindowIcon, icon);
    if (d_ptr->isAutoIcon && d_ptr->hostWidget)
        icon->setIcon(d_ptr->hostWidget->windowIcon());
    icon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
}

void ACaptionBar::setTitle(QLabel* label)
{
    auto org = takeTitle();
    if (org)
        org->deleteLater();
    if (!label)
        return;
    d_ptr->setWidgetAt(EWindowCaptionWidget::WindowTitle, label);
    if (d_ptr->isAutoTitle && d_ptr->hostWidget)
        label->setText(d_ptr->hostWidget->windowTitle());
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
}

void ACaptionBar::setMenuBar(QMenuBar* menuBar)
{
    auto org = takeMenuBar();
    if (org)
        org->deleteLater();
    if (!menuBar)
        return;
    d_ptr->setWidgetAt(EWindowCaptionWidget::WindowMenu, menuBar);
    menuBar->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
}

void ACaptionBar::setAppendixLayout(QLayout* layout)
{
    takeAppendixLayout();
    if (!layout)
        return;
    d_ptr->setAppendixLayout(layout);
}

void ACaptionBar::setHelpButton(QAbstractButton* btn)
{
    auto org = takeCloseButton();
    if (org)
        org->deleteLater();
    if (!btn)
        return;
    d_ptr->setWidgetAt(EWindowCaptionWidget::WindowHelpButton, btn);
    connect(btn, &QAbstractButton::clicked, this, &ACaptionBar::helpRequested);
}

void ACaptionBar::setMinButton(QAbstractButton* btn)
{
    auto org = takeMinButton();
    if (org)
        org->deleteLater();
    if (!btn)
        return;
    d_ptr->setWidgetAt(EWindowCaptionWidget::WindowMinimizeButton, btn);
    connect(btn, &QAbstractButton::clicked, this, &ACaptionBar::minimizeRequested);
}

void ACaptionBar::setMaxButton(QAbstractButton* btn)
{
    auto org = takeMaxButton();
    if (org)
        org->deleteLater();
    if (!btn)
        return;
    d_ptr->setWidgetAt(EWindowCaptionWidget::WindowMaximizeButton, btn);
    connect(btn, &QAbstractButton::clicked, this, &ACaptionBar::maximizeRequested);
}

void ACaptionBar::setCloseButton(QAbstractButton* btn)
{
    auto org = takeCloseButton();
    if (org)
        org->deleteLater();
    if (!btn)
        return;
    d_ptr->setWidgetAt(EWindowCaptionWidget::WindowCloseButton, btn);
    connect(btn, &QAbstractButton::clicked, this, &ACaptionBar::closeRequested);
}

QAbstractButton* ACaptionBar::takeIcon()
{
    return static_cast<QAbstractButton*>(d_ptr->takeWidgetAt(EWindowCaptionWidget::WindowIcon));
}

QLabel* ACaptionBar::takeTitle()
{
    return static_cast<QLabel*>(d_ptr->takeWidgetAt(EWindowCaptionWidget::WindowTitle));
}

QMenuBar* ACaptionBar::takeMenuBar()
{
    return static_cast<QMenuBar*>(d_ptr->takeWidgetAt(EWindowCaptionWidget::WindowMenu));
}

void ACaptionBar::takeAppendixLayout()
{
    d_ptr->takeAppendixLayout();
}

QAbstractButton* ACaptionBar::takeHelpButton()
{
    auto btn = static_cast<QAbstractButton*>(d_ptr->takeWidgetAt(EWindowCaptionWidget::WindowHelpButton));
    if (!btn)
    {
        return nullptr;
    }
    disconnect(btn, &QAbstractButton::clicked, this, &ACaptionBar::helpRequested);
    return btn;
}

QAbstractButton* ACaptionBar::takeMinButton()
{
    auto btn = static_cast<QAbstractButton*>(d_ptr->takeWidgetAt(EWindowCaptionWidget::WindowMinimizeButton));
    if (!btn)
    {
        return nullptr;
    }
    disconnect(btn, &QAbstractButton::clicked, this, &ACaptionBar::minimizeRequested);
    return btn;
}

QAbstractButton* ACaptionBar::takeMaxButton()
{
    auto btn = static_cast<QAbstractButton*>(d_ptr->takeWidgetAt(EWindowCaptionWidget::WindowMaximizeButton));
    if (!btn)
    {
        return nullptr;
    }
    disconnect(btn, &QAbstractButton::clicked, this, &ACaptionBar::maximizeRequested);
    return btn;
}

QAbstractButton* ACaptionBar::takeCloseButton()
{
    auto btn = static_cast<QAbstractButton*>(d_ptr->takeWidgetAt(EWindowCaptionWidget::WindowCloseButton));
    if (!btn)
    {
        return nullptr;
    }
    disconnect(btn, &QAbstractButton::clicked, this, &ACaptionBar::closeRequested);
    return btn;
}

QWidget* ACaptionBar::getHostWidget() const
{
    return d_ptr->hostWidget;
}

void ACaptionBar::setHostWidget(QWidget* w)
{
    QWidget* org = d_ptr->hostWidget;
    if (org)
    {
        org->removeEventFilter(this);
    }
    d_ptr->hostWidget = w;
    if (w)
    {
        w->installEventFilter(this);
    }
}

void ACaptionBar::setTitleFollowHostWidget(bool on)
{
    d_ptr->isAutoTitle = on;

    if (on && getTitle() && d_ptr->hostWidget)
        getTitle()->setText(d_ptr->hostWidget->windowTitle());
}

bool ACaptionBar::isTitleFollowHostWidget() const
{
    return d_ptr->isAutoTitle;
}

void ACaptionBar::setIconFollowHostWidget(bool on)
{
    d_ptr->isAutoIcon = on;

    if (on && getIcon() && d_ptr->hostWidget)
        getIcon()->setIcon(d_ptr->hostWidget->windowIcon());
}

bool ACaptionBar::isIconFollowWindow() const
{
    return d_ptr->isAutoIcon;
}

bool ACaptionBar::eventFilter(QObject* obj, QEvent* event)
{
    auto w = d_ptr->hostWidget;
    if (obj == w)
    {
        QAbstractButton* iconBtn = getIcon();
        QLabel* label = getTitle();
        QAbstractButton* maxBtn = getMaxButton();
        switch (event->type())
        {
        case QEvent::WindowIconChange: {
            if (d_ptr->isAutoIcon && iconBtn)
            {
                iconBtn->setIcon(w->windowIcon());
                iconChanged(w->windowIcon());
            }
            break;
        }
        case QEvent::WindowTitleChange: {
            if (d_ptr->isAutoTitle && label)
            {
                label->setText(w->windowTitle());
                titleChanged(w->windowTitle());
            }
            break;
        }
        case QEvent::WindowStateChange: {
            if (maxBtn)
            {
                maxBtn->setChecked(w->isMaximized());
            }
            break;
        }
        default:
            break;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void ACaptionBar::titleChanged(const QString& text)
{
    Q_UNUSED(text)
}

void ACaptionBar::iconChanged(const QIcon& icon)
{
    Q_UNUSED(icon)
}

APROCH_NAMESPACE_END
