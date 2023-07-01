/****************************************************************************
**
** Qtitan Library by Developer Machines (Microsoft-Ribbon implementation for Qt.C++)
** 
** Copyright (c) 2009-2022 Developer Machines (https://www.devmachines.com)
**           ALL RIGHTS RESERVED
** 
**  The entire contents of this file is protected by copyright law and
**  international treaties. Unauthorized reproduction, reverse-engineering
**  and distribution of all or any portion of the code contained in this
**  file is strictly prohibited and may result in severe civil and 
**  criminal penalties and will be prosecuted to the maximum extent 
**  possible under the law.
**
**  RESTRICTIONS
**
**  THE SOURCE CODE CONTAINED WITHIN THIS FILE AND ALL RELATED
**  FILES OR ANY PORTION OF ITS CONTENTS SHALL AT NO TIME BE
**  COPIED, TRANSFERRED, SOLD, DISTRIBUTED, OR OTHERWISE MADE
**  AVAILABLE TO OTHER INDIVIDUALS WITHOUT WRITTEN CONSENT
**  AND PERMISSION FROM DEVELOPER MACHINES
**
**  CONSULT THE END USER LICENSE AGREEMENT FOR INFORMATION ON
**  ADDITIONAL RESTRICTIONS.
**
****************************************************************************/
#ifndef QTN_RIBBONGROUPMENU_H
#define QTN_RIBBONGROUPMENU_H

#include <QEvent>
#include <QMenu>
#include <QToolButton>
#include <QStyleOption>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidgetAction>

#include "QtnRibbonButton.h"
#include "QtnPopupMenu.h"

QTITAN_BEGIN_NAMESPACE

class RibbonGroup;
class RibbonBar;

class RibbonPagePopup : public QWidget
{
    Q_OBJECT
public:
    RibbonPagePopup(RibbonBar* ribbonBar);
    RibbonBar* ribbonBar() const;
    void popup(const QRect& rect);
protected:
    void paintEvent(QPaintEvent* event) override;
    bool event(QEvent* event) override;
private:
    RibbonBar* m_ribbonBar;
};

class RibbonGroupPopup : public QWidget
{
public:
    explicit RibbonGroupPopup(RibbonGroup* group);
    virtual ~RibbonGroupPopup();
    void popup(const QPoint& pos);
public:
    virtual QSize sizeHint() const override;
protected:
    void hideEvent(QHideEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private:
    RibbonGroup* m_group;
private:
    Q_DISABLE_COPY(RibbonGroupPopup)
};

class RibbonGroupOption : public QToolButton
{
    Q_OBJECT
public:
    explicit RibbonGroupOption(QWidget* parent);
    virtual ~RibbonGroupOption();
public:
    QString text() const;
public:
    virtual QSize sizeHint() const override;
protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void actionEvent(QActionEvent* event) override;
private:
    Q_DISABLE_COPY(RibbonGroupOption)
};

class RibbonGroupScrollButton : public QToolButton
{
    Q_OBJECT
public:
    explicit RibbonGroupScrollButton(QWidget* parent, bool scrollLeft);
    virtual ~RibbonGroupScrollButton();
Q_SIGNALS:
    void stopScrollTimer();
protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
protected:
    bool m_scrollLeft;
private:
    Q_DISABLE_COPY(RibbonGroupScrollButton)
};

class RibbonOverflowMenuButton : public RibbonButton
{
public:
    RibbonOverflowMenuButton(QWidget* parent);
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private:
    Q_DISABLE_COPY(RibbonOverflowMenuButton)
};

class RibbonOverflowAction : public QWidgetAction
{
public:
    RibbonOverflowAction(QWidget* control, QWidget* parent);
    virtual ~RibbonOverflowAction();
public:
    virtual QWidget* createWidget(QWidget* parent) override;
    virtual void deleteWidget(QWidget* widget) override;
private:
    QWidget* m_control;
};

class RibbonOverflowMenu : public PopupMenu
{
    Q_OBJECT
public:
    RibbonOverflowMenu(QWidget* parent);
    virtual ~RibbonOverflowMenu();
public:
    void clearGroups();
    void addGroup(RibbonGroup* group);
    void fillMenu();
    bool isEmpty() const;
protected:
    virtual void paintMenuItem(QPainter* painter, QStyleOptionMenuItem* option) override;
private:
    QList<RibbonGroup*> m_groups;
    Q_DISABLE_COPY(RibbonOverflowMenu)
};

QTITAN_END_NAMESPACE

#endif //QTN_RIBBONGROUPMENU_H




