/****************************************************************************
 * @file    ANavigationMenuItem.cpp
 * @date    2024-07-13 
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
#include "ANavigationMenuItem.h"

APROCH_NAMESPACE_BEGIN

class ANavigationViewItemPrivate
{
public:
    QLabel* iconLabel = nullptr;
    QLabel* textLabel = nullptr;
};

ANavigationMenuItem::ANavigationMenuItem(QWidget* parent)
    : ANavigationMenuItem(QString(), QIcon(), parent)
{
}

ANavigationMenuItem::ANavigationMenuItem(const QString& text, const QIcon& icon, QWidget* parent)
    : QWidget(parent)
    , d_ptr(new ANavigationViewItemPrivate())
{
    QBoxLayout* theLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);

    //AFontIcon* iconLabel = new AFontIcon("", this);
    d_ptr->iconLabel = new QLabel(this);
    d_ptr->iconLabel->setPixmap(icon.pixmap(AFontIcon::DefaultIconSize));
    d_ptr->iconLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    d_ptr->textLabel = new QLabel(text, this);
    d_ptr->textLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    theLayout->addWidget(d_ptr->iconLabel);
    theLayout->addWidget(d_ptr->textLabel);
    theLayout->setSpacing(12);
    theLayout->setContentsMargins(QMargins(10, 0, 10, 0));

    setAttribute(Qt::WA_StyledBackground);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

QSize ANavigationMenuItem::sizeHint() const
{
    return QSize(180, 44);
}

AInfoBadge* ANavigationMenuItem::getInfoBadge() const
{
    throw std::exception("no implement");
    return nullptr;
}


////////////////////////////////////////////////////////////////////////////////////////////////////


ANavigationMenuItemGroup::ANavigationMenuItemGroup(QWidget* parent)
    : ANavigationMenuItemGroup(QString(), parent)
{
}

ANavigationMenuItemGroup::ANavigationMenuItemGroup(const QString& text, QWidget* parent)
    : ANavigationMenuItem(text, QIcon(), parent)
{
    d_ptr->iconLabel->deleteLater();
    d_ptr->iconLabel = nullptr;

    setAttribute(Qt::WA_TransparentForMouseEvents);
}

APROCH_NAMESPACE_END