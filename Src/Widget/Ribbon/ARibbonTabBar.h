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
#ifndef QTN_RIBBONTABBAR_H
#define QTN_RIBBONTABBAR_H

#include <QWidget>
#include <QMenu>

#include "QtitanDef.h"
#include "QtnCommonStyle.h"
#include "QtnAbstractTabBar.h"
#include "QtnRibbonPage.h"

QTITAN_BEGIN_NAMESPACE

class RibbonTabBar;
class RibbonTabPrivate;
class RibbonTab;

class ContextualTab
{
public:
    ContextualTab(RibbonBar* bar, RibbonTab* tab);
    virtual ~ContextualTab();
    const QColor& color() const;
    const QString& title() const;
    const QString& groupName() const;
    QRect rect() const;
    QFont font() const;
public:
    RibbonBar* ribbonBar;
    RibbonTab* firstTab;
    RibbonTab* lastTab;
};

class RibbonTab : public AbstractTab
{
    Q_OBJECT
public:
    explicit RibbonTab(RibbonTabBar* parent, const QString& text);
    ~RibbonTab() override;
public:
    RibbonTabBar* tabBar() const;
    RibbonPage* page() const;
    Qt::Alignment alignment() const;
    void setAlignment(Qt::Alignment);
    void setText(const QString& text) override;
    const QString& text() const;
    const QString& contextTitle() const;
    const QString& contextGroupName() const;
    const QColor& contextColor() const;
    ContextualTab* contextualTab() const;
    void setContextualTab(ContextualTab* tab);
    QAction* defaultAction() const;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
public:
    void clicked() override;
    void dblClicked() override;
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    friend class RibbonPage;
    friend class RibbonTabBarPrivate;
    QTN_DECLARE_PRIVATE(RibbonTab)
    Q_DISABLE_COPY(RibbonTab)
};

/* RibbonTabBar */
class RibbonTabBar : public AbstractTabBar
{
    Q_OBJECT
public:
    explicit RibbonTabBar(RibbonBar* ribbonBar);
    ~RibbonTabBar() override;
public:
    RibbonBar* ribbonBar() const;
    RibbonTab* getTab(int index);
    const RibbonTab* getTab(int index) const;
protected:
    QAction* addAction(const QIcon& icon, const QString& text, Qt::ToolButtonStyle style, QMenu* menu = Q_NULL);
    QAction* addAction(QAction* action, Qt::ToolButtonStyle style);
    QMenu* addMenu(const QString& title);
protected:
    void paintTab(QPainter* painter, AbstractTab* tab) const override;
    void paintTabBarFrame(QPainter* painter, QStyleOptionTabBarBase* optTabBase) override;
public:
    AbstractTab* createTab(const QIcon& icon, const QString& text, QObject* object) override;
    QSize sizeHint() const override;
protected:
    bool event(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
private:
    RibbonBar* m_ribbonBar;
private:
    friend class RibbonTab;
    friend class RibbonBar;
    Q_DISABLE_COPY(RibbonTabBar)
};

QTITAN_END_NAMESPACE

#endif // QTN_RIBBONTABBAR_H
