/****************************************************************************
 * @file    AARibbonPage.h
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
#include "Widget/Style/ACommonStyle.h"
#include "ARibbonBar.h"

APROCH_NAMESPACE_BEGIN

class ARibbonGroup;
class ARibbonTab;
class ARibbonPagePrivate;

class APROCH_API ARibbonPage : public QWidget
{
    Q_OBJECT;
    Q_ENUMS(ContextColor);
    Q_PROPERTY(const QString& title READ title WRITE setTitle);
    Q_PROPERTY(const QString& contextTitle READ contextTitle WRITE setContextTitle);
    Q_PROPERTY(const QString& contextGroupName READ contextGroupName WRITE setContextGroupName);
    Q_PROPERTY(const QColor& contextColor READ contextColor WRITE setContextColor);
    Q_PROPERTY(int groupCount READ groupCount);
public:
    ARibbonPage(QWidget* parent = nullptr);
#if 0
    ARibbonPage(ARibbonBar* ribbonBar, const QString& title);
#endif
    virtual ~ARibbonPage();
public:
    ARibbonBar* ribbonBar() const;
    bool isVisible() const;
    virtual void setVisible(bool visible);
    void addGroup(ARibbonGroup* group);
    ARibbonGroup* addGroup(const QString& title);
    ARibbonGroup* addGroup(const QIcon& icon, const QString& title);
    void insertGroup(int index, ARibbonGroup* group);
    ARibbonGroup* insertGroup(int index, const QString& title);
    ARibbonGroup* insertGroup(int index, const QIcon& icon, const QString& title);
    void removeGroup(ARibbonGroup* group);
    void removeGroupByIndex(int index);
    void detachGroup(ARibbonGroup* group);
    void detachGroupByIndex(int index);
    void clearGroups();
    QAction* defaultAction() const;
    int groupCount() const;
    ARibbonGroup* group(int index) const;
    int groupIndex(ARibbonGroup* group) const;
    QList<ARibbonGroup*> groups() const;
    const QString& title() const;
    const QColor& contextColor() const;
    const QString& contextTitle() const;
    const QString& contextGroupName() const;
    void setContextColor(EContextColor color);
    void updateLayout();

public Q_SLOTS:
    void setTitle(const QString& title);
    void setContextColor(const QColor& color);
    void setContextTitle(const QString& title);
    void setContextGroupName(const QString& groupName);

Q_SIGNALS:
    void titleChanged(const QString& title);

protected Q_SLOTS:
    void released();
    void actionTriggered(QAction* action);

public:
    virtual QSize sizeHint() const;

protected:
    virtual bool event(QEvent* event);
    virtual void paintEvent(QPaintEvent* event);
    virtual void changeEvent(QEvent* event);
    virtual void resizeEvent(QResizeEvent* event);

private:
    friend class ARibbonBarPrivate;
    A_DECLARE_PRIVATE(ARibbonPage);
    Q_DISABLE_COPY(ARibbonPage);
};

APROCH_NAMESPACE_END