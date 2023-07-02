/****************************************************************************
 * @file    ARibbonSystemMenu.h
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

#include "Widget/APopupMenu.h"
#include <QToolButton>

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class ARibbonBar;
class ASystemToolButtonStyleOption;
class ARibbonBackstageView;
class ARibbonSystemButtonPrivate;
class ARibbonSystemMenu;

class APROCH_API ARibbonSystemButton : public QToolButton
{
    Q_OBJECT
public:
    ARibbonSystemButton(ARibbonBar* ribbonBar);
    ARibbonSystemButton(QWidget* parent);
    ~ARibbonSystemButton() override;
    ARibbonBar* ribbonBar() const;
    ARibbonBackstageView* backstage() const;
    void setBackstage(ARibbonBackstageView* backstage);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor& color);

    ARibbonSystemMenu* systemMenu() const;
    void setSystemMenu(ARibbonSystemMenu* menu);

    void initButtonStyleOption(ASystemToolButtonStyleOption* option) const;
    QSize sizeHint() const override;
protected:
    bool event(QEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private:
    friend class ARibbonSystemMenu;
    Q_DISABLE_COPY(ARibbonSystemButton)
    A_DECLARE_PRIVATE(ARibbonSystemButton)
};

class ARibbonPageSystemRecentFileList;
class ARibbonPageSystemPopup;
class ARibbonFrameMenuStyleOption;
class ARibbonSystemMenuPrivate;

class APROCH_API ARibbonSystemMenu : public APopupMenu
{
    Q_OBJECT
public:
    explicit ARibbonSystemMenu(ARibbonBar* ribbonBar);
    ~ARibbonSystemMenu() override;
    ARibbonBar* ribbonBar() const;
    QAction* addPopupBarAction(const QString& text);
    void addPopupBarAction(QAction* action, Qt::ToolButtonStyle style = Qt::ToolButtonTextOnly);
    ARibbonPageSystemRecentFileList* addPageRecentFile(const QString& caption);
    ARibbonPageSystemPopup* addPageSystemPopup(const QString& caption, QAction* defaultAction, bool splitAction);
    void setVisible(bool visible) override;
    QSize sizeHint() const override;
    void updateLayout() override;
protected:
    void initFrameMenuStyleOption(ARibbonFrameMenuStyleOption* option) const;

    void paintEvent(QPaintEvent* event) override;
    void paintMenuItem(QPainter* painter, QStyleOptionMenuItem* option) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void changeEvent(QEvent* event) override;
private:
    friend class ARibbonPageSystemPopup;
    friend class ARibbonPageSystemPopupPrivate;
    friend class ARibbonPageSystemRecentFileList;
    Q_DISABLE_COPY(ARibbonSystemMenu);
    A_DECLARE_EX_PRIVATE(ARibbonSystemMenu);
};

class ARibbonPageSystemRecentFileListPrivate;
class APROCH_API ARibbonPageSystemRecentFileList : public QWidget
{
    Q_OBJECT
public:
    ARibbonPageSystemRecentFileList(const QString& caption);
    ~ARibbonPageSystemRecentFileList() override;
public:
    QSize sizeHint() const override;
    void setSize(int size);
    int getSize() const; 
    QAction* getCurrentAction() const;
public Q_SLOTS:
    void updateRecentFileActions(const QStringList& files);
Q_SIGNALS:
    void openRecentFile(const QString& file);
protected:
    void paintEvent(QPaintEvent* event) override;
    void paintItems(QPainter* painter);
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    void enterEvent(QEnterEvent* event) override;
#else
    void enterEvent(QEvent* event) override;
#endif
    void leaveEvent(QEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
private:
    friend class ARibbonSystemMenu;
    Q_DISABLE_COPY(ARibbonPageSystemRecentFileList)
    A_DECLARE_PRIVATE(ARibbonPageSystemRecentFileList)
};

class ARibbonPageSystemPopupPrivate;
class APROCH_API ARibbonPageSystemPopup : public APopupMenu
{
    Q_OBJECT
public:
    ARibbonPageSystemPopup(const QString& caption, QWidget* parent = nullptr);
    ~ARibbonPageSystemPopup() override;
public:
    QSize sizeHint() const override;
protected:
    bool event(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void paintMenuItem(QPainter* painter, QStyleOptionMenuItem* option) override;
private:
    Q_DISABLE_COPY(ARibbonPageSystemPopup);
    A_DECLARE_EX_PRIVATE(ARibbonPageSystemPopup);
};

APROCH_NAMESPACE_END