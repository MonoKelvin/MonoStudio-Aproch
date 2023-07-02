/****************************************************************************
 * @file    ARibbonControls.h
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

#include <QWidget>
#include <QResizeEvent>
#include <QStyleOption>
#include <QLabel>
#include <QToolButton>
#include <QWidgetAction>
#include <QMenu>

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class ARibbonControl;
class ARibbonControlSizeDefinitionPrivate;
class APROCH_API ARibbonControlSizeDefinition : public QObject
{
    Q_OBJECT
public:
    enum GroupSize
    {
        GroupLarge  = 0,
        GroupMedium = 1,
        GroupSmall  = 2,
        GroupPopup  = 3,
        GroupSimplified = 4,
        GroupSimplifiedOverflow = 5
    };

    enum ControlImageSize 
    {
        ImageNone,
        ImageLarge,
        ImageSmall
    };

public:
    explicit ARibbonControlSizeDefinition(ARibbonControl* parent, GroupSize size);
    virtual ~ARibbonControlSizeDefinition();
public:
    ControlImageSize imageSize() const;
    void setImageSize(ControlImageSize size);

    bool isLabelVisible() const;
    void setLabelVisible(bool visible);

    bool isPopup() const;
    void setPopup(bool popup);

    int visualIndex() const;
    void setVisualIndex(int index);

    bool wordWrap() const;
    void setWordWrap(bool on);

    bool showSeparator() const;
    void setShowSeparator(bool showSeparator);

    bool isVisible() const;
    void setVisible(bool visible);

    bool isEnabled() const;
    void setEnabled(bool enabled);

    bool isStretchable() const;
    void setStretchable(bool stretchable);

    int minimumItemCount() const;
    void setMinimumItemCount(int count);

    int maximumItemCount() const;
    void setMaximumItemCount(int count);

private:
    A_DECLARE_PRIVATE(ARibbonControlSizeDefinition)
    Q_DISABLE_COPY(ARibbonControlSizeDefinition)
};

class ARibbonGroup;
class ARibbonControlPrivate;
/* ARibbonControl */
class APROCH_API ARibbonControl : public QWidget
{
    Q_OBJECT
public:
    explicit ARibbonControl(ARibbonGroup* parent = nullptr);
    virtual ~ARibbonControl();
public:
    void setParentGroup(ARibbonGroup* parentGroup);
    ARibbonGroup* parentGroup() const;
    QAction* defaultAction() const;
    void setDefaultAction(QAction* action);

    virtual bool adjustCurrentSize(bool expand);
    virtual void sizeChanged(ARibbonControlSizeDefinition::GroupSize size);

    virtual void actionChanged();
    virtual void updateLayout();

    ARibbonControlSizeDefinition::GroupSize currentSize() const;
    ARibbonControlSizeDefinition* sizeDefinition(ARibbonControlSizeDefinition::GroupSize size) const;
protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void setDirty();
public:
    static QSize getBoundedSizeHint(const QWidget* widget);
private:
    friend class ARibbonGroup;
    friend class ARibbonBarPrivate;
    A_DECLARE_PRIVATE(ARibbonControl)
    Q_DISABLE_COPY(ARibbonControl)
};

class ARibbonWidgetControlPrivate;
/* ARibbonWidgetControl */
class APROCH_API ARibbonWidgetControl : public ARibbonControl
{
    Q_OBJECT
public:
    explicit ARibbonWidgetControl(ARibbonGroup* parent, bool ignoreActionSettings);
    virtual ~ARibbonWidgetControl();
public:
    int margin() const;
    void setMargin(int margin);
public:
    virtual void sizeChanged(ARibbonControlSizeDefinition::GroupSize size) override;
    virtual QSize sizeHint() const override;
protected:
    QWidget* contentWidget() const;
    void setContentWidget(QWidget* widget);
protected:
    virtual void childEvent(QChildEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
private:
    friend class ARibbonGroup;
    friend class ARibbonGroupPrivate;
    A_DECLARE_PRIVATE(ARibbonWidgetControl)
    Q_DISABLE_COPY(ARibbonWidgetControl)
};

class ARibbonColumnBreakControlPrivate;
class APROCH_API ARibbonColumnBreakControl : public ARibbonWidgetControl
{
    Q_OBJECT
public:
    explicit ARibbonColumnBreakControl(ARibbonGroup* parent = nullptr);
    virtual ~ARibbonColumnBreakControl();
public:
    Qt::Alignment alignmentText() const;
    void setAlignmentText(Qt::Alignment alignment);
public:
    virtual void sizeChanged(ARibbonControlSizeDefinition::GroupSize size) override;
    virtual QSize sizeHint() const override;
protected:
    virtual void paintEvent(QPaintEvent* event) override;
private:
    A_DECLARE_PRIVATE(ARibbonColumnBreakControl)
    Q_DISABLE_COPY(ARibbonColumnBreakControl)
};

class APROCH_API ARibbonLabelControl : public ARibbonWidgetControl
{
public: 
    explicit ARibbonLabelControl(ARibbonGroup* parent = nullptr);
    explicit ARibbonLabelControl(const QString& text, ARibbonGroup* parent = nullptr);
    virtual ~ARibbonLabelControl();
public:
    QLabel* widget() const;
private:
    Q_DISABLE_COPY(ARibbonLabelControl)
};

class ARibbonToolBarControlPrivate;
/* ARibbonToolBarControl */
class APROCH_API ARibbonToolBarControl : public ARibbonControl
{
    Q_OBJECT
public:
    explicit ARibbonToolBarControl(ARibbonGroup* parent = nullptr);
    virtual ~ARibbonToolBarControl();
public:
    void clear();
    QAction* addWidget(QWidget* widget);
    QAction* addWidget(const QIcon& icon, const QString& text, QWidget* widget);
    QAction* addMenu(const QIcon& icon, const QString& text, QMenu* menu, QToolButton::ToolButtonPopupMode mode = QToolButton::MenuButtonPopup);
    QAction* addAction(QAction* action, Qt::ToolButtonStyle style, QMenu* menu = nullptr, QToolButton::ToolButtonPopupMode mode = QToolButton::MenuButtonPopup);
    QAction* insertAction(QAction* before, QAction* action, Qt::ToolButtonStyle style, QMenu* menu = nullptr, QToolButton::ToolButtonPopupMode mode = QToolButton::MenuButtonPopup);
    QAction* addAction(const QString& text);
    QAction* addAction(const QIcon& icon, const QString& text);
    QAction* addAction(const QString& text, const QObject* receiver, const char* member);
    QAction* addAction(const QIcon& icon, const QString& text, const QObject* receiver, const char * member);
    QAction* addAction(const QIcon& icon, const QString& text, Qt::ToolButtonStyle style, QMenu* menu = nullptr, QToolButton::ToolButtonPopupMode mode = QToolButton::MenuButtonPopup);
    QAction* addSeparator();
    QMenu* overflowMenu();
    bool isOverflowMenuIsEmpty() const;
    int margin() const;
    void setMargin(int margin);
    QWidget* widgetByAction(QAction* action);
    using QWidget::addAction;
    using QWidget::insertAction;
public:
    virtual void sizeChanged(ARibbonControlSizeDefinition::GroupSize size) override;
    virtual bool adjustCurrentSize(bool expand) override;
    virtual void updateLayout() override;
    virtual QSize sizeHint() const override;
protected:
    int rowsCount();
    void setRowsCount(int count);
    virtual void setDirty() override;
protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void actionEvent(QActionEvent* event) override;
    virtual void changeEvent(QEvent* event) override;
private:
    A_DECLARE_PRIVATE(ARibbonToolBarControl)
    Q_DISABLE_COPY(ARibbonToolBarControl)
};

APROCH_NAMESPACE_END