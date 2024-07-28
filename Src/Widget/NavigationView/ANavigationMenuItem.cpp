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

ANavigationViewItemBase::ANavigationViewItemBase(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground);
}

QSize ANavigationViewItemBase::sizeHint() const
{
    return QSize(180, 44);
}


////////////////////////////////////////////////////////////////////////////////////////////////////


class ANavigationMenuItemPrivate
{
public:
    QLabel* iconLabel = nullptr;
    QLabel* textLabel = nullptr;
    QLabel* expandLabel = nullptr;
    ANavigationMenuItem::EExpandState expandState = ANavigationMenuItem::NoExpandState;
};

ANavigationMenuItem::ANavigationMenuItem(QWidget* parent)
    : ANavigationMenuItem(QString(), QIcon(), parent)
{
}

ANavigationMenuItem::ANavigationMenuItem(const QString& text, const QIcon& icon, QWidget* parent)
    : ANavigationViewItemBase(parent)
    , d_ptr(new ANavigationMenuItemPrivate())
{
    QBoxLayout* theLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);

    d_ptr->iconLabel = new QLabel(this);
    d_ptr->iconLabel->setAlignment(Qt::AlignCenter);
    d_ptr->iconLabel->setPixmap(icon.pixmap(AFontIcon::DefaultIconSize));
    d_ptr->iconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    d_ptr->iconLabel->setMinimumSize(AFontIcon::DefaultIconSize);
    d_ptr->iconLabel->setObjectName("aproch-nav-menuitem-icon");

    d_ptr->textLabel = new QLabel(text, this);
    d_ptr->textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    d_ptr->textLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    d_ptr->textLabel->setObjectName("aproch-nav-menuitem-text");

    theLayout->addWidget(d_ptr->iconLabel, 0, Qt::AlignLeft);
    theLayout->addWidget(d_ptr->textLabel, 1, Qt::AlignLeft);
    theLayout->setSpacing(0);
    theLayout->setContentsMargins(QMargins(0, 0, 0, 0));

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void ANavigationMenuItem::setIcon(const QIcon& icon)
{
    d_ptr->iconLabel->setPixmap(icon.pixmap(AFontIcon::DefaultIconSize));
}

QIcon ANavigationMenuItem::getIcon() const
{
    return d_ptr->iconLabel->pixmap();
}

void ANavigationMenuItem::setText(const QString& text)
{
    d_ptr->textLabel->setText(text);
}

QString ANavigationMenuItem::getText() const
{
    return d_ptr->textLabel->text();
}

void ANavigationMenuItem::setExpandState(EExpandState state)
{
    if (d_ptr->expandState == state)
        return;

    switch (state)
    {
    case NoExpandState:
    {
        if (d_ptr->expandLabel)
        {
            d_ptr->expandLabel->deleteLater();
            d_ptr->expandLabel = nullptr;
        }
    }
        break;
    case Expanded:
    case UnExpanded:
    {
        if (!d_ptr->expandLabel)
        {
            d_ptr->expandLabel = new QLabel(this);
            d_ptr->expandLabel->setAlignment(Qt::AlignCenter);
            d_ptr->expandLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
            d_ptr->expandLabel->setMinimumSize(AFontIcon::DefaultIconSize);
            d_ptr->expandLabel->setObjectName("aproch-nav-menuitem-expanded");

            auto theLayout = qobject_cast<QBoxLayout*>(layout());
            Q_ASSERT(theLayout);
            theLayout->addWidget(d_ptr->expandLabel, 0, Qt::AlignRight);
        }

        QIcon icon;
        if (state == Expanded)
            icon = AFontIcon::icon("\uE70D");
        else
            icon = AFontIcon::icon("\uE70E");
        d_ptr->expandLabel->setPixmap(icon.pixmap(AFontIcon::DefaultIconSize));

    }
        break;
    default:
        break;
    }

    d_ptr->expandState = state;
    emit expandStateChanged();
}

ANavigationMenuItem::EExpandState ANavigationMenuItem::getExpandState() const
{
    return d_ptr->expandState;
}

AInfoBadge* ANavigationMenuItem::getInfoBadge() const
{
    throw std::exception("no implement");
    return nullptr;
}

void ANavigationMenuItem::showEvent(QShowEvent* evt)
{
    if (d_ptr->iconLabel)
    {
        const auto& lm = layout()->contentsMargins();
        setMinimumWidth(d_ptr->iconLabel->width() + lm.left() + lm.right());
    }

    QWidget::showEvent(evt);
}


////////////////////////////////////////////////////////////////////////////////////////////////////


class ANavigationMenuItemGroupPrivate
{
public:
    QLabel* textLabel = nullptr;
};

ANavigationMenuItemGroup::ANavigationMenuItemGroup(QWidget* parent)
    : ANavigationMenuItemGroup(QString(), parent)
{
}

ANavigationMenuItemGroup::ANavigationMenuItemGroup(const QString& text, QWidget* parent)
    : ANavigationViewItemBase(parent)
    , d_ptr(new ANavigationMenuItemGroupPrivate())
{
    QBoxLayout* theLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);

    d_ptr->textLabel = new QLabel(text, this);
    d_ptr->textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    d_ptr->textLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    d_ptr->textLabel->setObjectName("aproch-nav-menuitem-text");

    theLayout->addWidget(d_ptr->textLabel, 1, Qt::AlignLeft);
    theLayout->setSpacing(0);
    theLayout->setContentsMargins(QMargins(0, 0, 0, 0));

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void ANavigationMenuItemGroup::setText(const QString& text)
{
    d_ptr->textLabel->setText(text);
}

QString ANavigationMenuItemGroup::getText() const
{
    return d_ptr->textLabel->text();
}


////////////////////////////////////////////////////////////////////////////////////////////////////

class ANavigationViewItemSeparatorPrivate
{
public:
    Qt::Orientation ori = Qt::Horizontal;
    int thickness = 1;
};

ANavigationViewItemSeparator::ANavigationViewItemSeparator(QWidget* parent)
    : ANavigationViewItemSeparator(Qt::Horizontal, parent)
{
}

ANavigationViewItemSeparator::ANavigationViewItemSeparator(Qt::Orientation ori, QWidget* parent)
    : ANavigationViewItemBase(parent)
    , d_ptr(new ANavigationViewItemSeparatorPrivate)
{
    setOrientation(ori);
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void ANavigationViewItemSeparator::setOrientation(Qt::Orientation ori)
{
    if (d_ptr->ori == ori)
        return;

    d_ptr->ori = ori;

    if (d_ptr->ori == Qt::Horizontal)
        resize(width(), d_ptr->thickness);
    else
        resize(d_ptr->thickness, height());
}

Qt::Orientation ANavigationViewItemSeparator::getOrientation() const
{
    return d_ptr->ori;
}

void ANavigationViewItemSeparator::setThickness(int n)
{
    if (d_ptr->thickness == n)
        return;

    d_ptr->thickness = n;

    if (d_ptr->ori == Qt::Horizontal)
        resize(width(), d_ptr->thickness);
    else
        resize(d_ptr->thickness, height());
}

int ANavigationViewItemSeparator::getThickness() const
{
    return d_ptr->thickness;
}

QSize ANavigationViewItemSeparator::sizeHint() const
{
    const QSize defSizeHint = ANavigationViewItemBase::sizeHint();
    return d_ptr->ori == Qt::Horizontal ? QSize(defSizeHint.width(), 1) : QSize(1, defSizeHint.height());
}


////////////////////////////////////////////////////////////////////////////////////////////////////


ANavigationBackButton::ANavigationBackButton(QWidget* parent)
    : QPushButton(parent)
{
    setIcon(AFontIcon::icon("\uE72B"));
    setToolTip(tr("Back"));
    setAttribute(Qt::WA_StyledBackground);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}


////////////////////////////////////////////////////////////////////////////////////////////////////


ANavigationCompactButton::ANavigationCompactButton(const QString& text, QWidget* parent)
    : QPushButton(text, parent)
{
    setCheckable(true);
    setIcon(AFontIcon::icon("\uE700"));
    setToolTip(tr("Close Navigation"));
    setAttribute(Qt::WA_StyledBackground);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(this, &ANavigationCompactButton::toggled, this, [=](bool expand) {
        setToolTip(expand ? tr("Close Navigation") : tr("Open Navigation"));
    });
}


////////////////////////////////////////////////////////////////////////////////////////////////////


ANavigationSettingsButton::ANavigationSettingsButton(QWidget* parent)
    : QPushButton(parent)
{
    setIcon(AFontIcon::icon("\uE713"));
    setText(tr("Settings"));
    setToolTip(text());
    setAttribute(Qt::WA_StyledBackground);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}


APROCH_NAMESPACE_END
