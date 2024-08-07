/****************************************************************************
 * @file    ANavigationMenuItem.h
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
#pragma once
#include "AprochExportDefine.h"
#include <QPushButton>

class QLabel;

APROCH_NAMESPACE_BEGIN

class AInfoBadge;
class AIndicatorBar;
class ANavigationMenuItemPrivate;
class ANavigationMenuItemGroupPrivate;
class ANavigationViewItemSeparatorPrivate;

class APROCH_API ANavigationViewItemBase : public QWidget
{
    Q_OBJECT;
public:
    explicit ANavigationViewItemBase(QWidget* parent = nullptr);
    virtual QSize sizeHint() const override;
};

class APROCH_API ANavigationMenuItem : public ANavigationViewItemBase
{
    Q_OBJECT;
    Q_PROPERTY(QString text READ getText WRITE setText);
    Q_PROPERTY(QIcon icon READ getIcon WRITE setIcon);
    Q_PROPERTY(EExpandState expandState READ getExpandState WRITE setExpandState NOTIFY expandStateChanged);
public:
    explicit ANavigationMenuItem(QWidget* parent = nullptr);
    ANavigationMenuItem(const QString& text, const QIcon& icon, QWidget* parent = nullptr);

    enum EExpandState
    {
        NoExpandState,
        Expanded,
        Collapsed,
    };
    Q_ENUM(EExpandState);

    void setIcon(const QIcon& icon);
    QIcon getIcon() const;

    void setText(const QString& text);
    QString getText() const;

    void setExpandState(EExpandState state);
    EExpandState getExpandState() const;

    void setExpandButtonVisible(bool visible);
    bool getExpandButtonVisible() const;

    bool isSelected() const;

    AInfoBadge* getInfoBadge() const;
    AIndicatorBar* getIndicatorBar() const;

Q_SIGNALS:
    void expandStateChanged();

public Q_SLOTS:
    void setSelected(bool select);

protected:
    virtual void showEvent(QShowEvent* evt) override;

protected Q_SLOTS:
    void rotateExpandedIcon(const QVariant& rot);

protected:
    Q_DISABLE_COPY_MOVE(ANavigationMenuItem);
    QSharedPointer<ANavigationMenuItemPrivate> d_ptr;
};

class APROCH_API ANavigationMenuItemGroup : public ANavigationViewItemBase
{
    Q_OBJECT;
    Q_PROPERTY(QString text READ getText WRITE setText);
public:
    explicit ANavigationMenuItemGroup(QWidget* parent = nullptr);
    explicit ANavigationMenuItemGroup(const QString& text, QWidget* parent = nullptr);

    void setText(const QString& text);
    QString getText() const;

private:
    Q_DISABLE_COPY_MOVE(ANavigationMenuItemGroup); 
    QSharedPointer<ANavigationMenuItemGroupPrivate> d_ptr;
};

class APROCH_API ANavigationViewItemSeparator : public ANavigationViewItemBase
{
    Q_OBJECT;
    Q_PROPERTY(Qt::Orientation orientation READ getOrientation WRITE setOrientation);
    Q_PROPERTY(int thickness READ getThickness WRITE setThickness);
public:
    explicit ANavigationViewItemSeparator(QWidget* parent = nullptr);
    explicit ANavigationViewItemSeparator(Qt::Orientation ori, QWidget* parent = nullptr);

    void setOrientation(Qt::Orientation ori);
    Qt::Orientation getOrientation() const;

    void setThickness(int n);
    int getThickness() const;

    virtual QSize sizeHint() const override;

private:
    friend class ANavigationViewItemSeparator;
    Q_DISABLE_COPY_MOVE(ANavigationViewItemSeparator);
    QSharedPointer<ANavigationViewItemSeparatorPrivate> d_ptr;
};

class APROCH_API ANavigationBackButton : public QPushButton     // TODO: AButton supports animation
{
    Q_OBJECT;
public:
    explicit ANavigationBackButton(QWidget* parent = nullptr);

protected:
    Q_DISABLE_COPY_MOVE(ANavigationBackButton);
};

class APROCH_API ANavigationCompactButton : public QPushButton
{
    Q_OBJECT;
public:
    explicit ANavigationCompactButton(const QString& text = QString(), QWidget* parent = nullptr);

protected:
    Q_DISABLE_COPY_MOVE(ANavigationCompactButton);
};

class APROCH_API ANavigationSettingsButton : public QPushButton
{
    Q_OBJECT;
public:
    explicit ANavigationSettingsButton(QWidget* parent = nullptr);

protected:
    Q_DISABLE_COPY_MOVE(ANavigationSettingsButton);
};

APROCH_NAMESPACE_END
