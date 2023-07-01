/****************************************************************************
 * @file    ARibbonGroup.h
 * @date    2023-07-01 
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

#include <QToolButton>
#include <QMenu>

#include "ARibbonControls.h"
#include "ARibbonPage.h"
#include "QtitanDef.h"

APROCH_NAMESPACE_BEGIN

class RibbonGroupPrivate;
class APROCH_API ARibbonGroup : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool reduced READ isReduced)
    Q_PROPERTY(const QString& title READ title WRITE setTitle)
    Q_PROPERTY(const QFont& titleFont READ titleFont WRITE setTitleFont)
    Q_PROPERTY(const QColor& titleColor READ titleColor WRITE setTitleColor)
    Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing)
    Q_PROPERTY(bool optionButtonVisible READ isOptionButtonVisible WRITE setOptionButtonVisible DESIGNABLE false)
    Q_PROPERTY(Qt::Alignment contentAlignment READ contentAlignment WRITE setContentAlignment)
    Q_PROPERTY(Qt::TextElideMode titleElideMode READ titleElideMode WRITE setTitleElideMode)
public:
    explicit ARibbonGroup(QWidget* parent = nullptr);
    explicit ARibbonGroup(ARibbonPage* page, const QString& title);
    ~ARibbonGroup() override;
public:
    ARibbonBar* ribbonBar() const;

    bool isReduced() const;

    const QString& title() const;
    void setTitle(const QString& title);

    const QFont& titleFont() const;
    void setTitleFont(const QFont& font);

    const QColor& titleColor() const;
    void setTitleColor(const QColor& color);

    const QIcon& icon() const;
    void setIcon(const QIcon& icon);

    bool isOptionButtonVisible() const;
    void setOptionButtonVisible(bool visible = true);

    QAction* optionButtonAction() const;
    void setOptionButtonAction(QAction* action);

    Qt::Alignment contentAlignment() const;
    void setContentAlignment(Qt::Alignment alignment);

    Qt::Alignment controlsAlignment() const;
    void setControlsAlignment(Qt::Alignment alignment);

    int spacing() const;
    void setSpacing(int);

    int controlCount() const;
    RibbonControl* controlByIndex(int index) const;
    RibbonControl* controlByAction(QAction* action) const;
    RibbonWidgetControl* controlByWidget(QWidget* widget) const;
    ARibbonControlSizeDefinition::GroupSize currentSize() const;

    void addControl(RibbonControl* control);
    void removeControl(RibbonControl* control);

    QAction* addAction(const QIcon& icon, const QString& text, Qt::ToolButtonStyle style, 
        QMenu* menu = nullptr, QToolButton::ToolButtonPopupMode mode = QToolButton::MenuButtonPopup);
    QAction* addAction(QAction* action, Qt::ToolButtonStyle style, QMenu* menu = nullptr, 
        QToolButton::ToolButtonPopupMode mode = QToolButton::MenuButtonPopup);
    QAction* insertAction(QAction* before, QAction* action, Qt::ToolButtonStyle style, QMenu* menu = nullptr, 
        QToolButton::ToolButtonPopupMode mode = QToolButton::MenuButtonPopup);

    QAction* addWidget(QWidget* widget);
    QAction* addWidget(const QIcon& icon, const QString& text, QWidget* widget);
    QAction* addWidget(const QIcon& icon, const QString& text, bool stretch, QWidget* widget);
    QMenu* addMenu(const QIcon& icon, const QString& text, Qt::ToolButtonStyle style = Qt::ToolButtonFollowStyle);
    QAction* addSeparator();

    void remove(QWidget* widget);
    void clear();

    Qt::TextElideMode titleElideMode() const;
    void setTitleElideMode(Qt::TextElideMode mode);
public:
    void setControlsCentering(bool enabled = true); //Deprecated, user setContentAlignment instead
    bool isControlsCentering() const; //Deprecated
    void setControlsGrouping(bool enabled = true); //Deprecated, user RibbonToolBarControl instead
    bool isControlsGrouping() const; //Deprecated
public:
    QSize sizeHint() const override;
public:
    using QWidget::addAction;
    using QWidget::insertAction;
Q_SIGNALS:
    /*! \internal */
    void released();
    /*! \internal */
    void actionTriggered(QAction*);
    /*! \internal */
    void hidePopup();
Q_SIGNALS:
    void titleChanged(const QString& title);
    void titleFontChanged(const QFont& font);
protected Q_SLOTS:
    /*! \internal */
    void currentIndexChanged(int index);
protected:
    bool event(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void actionEvent(QActionEvent* event) override;
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    void enterEvent(QEnterEvent* event) override;
#else
    void enterEvent(QEvent* event) override;
#endif
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private:
    A_DECLARE_PRIVATE(ARibbonGroup)
    Q_DISABLE_COPY(ARibbonGroup)
};

QTITAN_END_NAMESPACE

#endif // A_RIBBONGROUP_H
