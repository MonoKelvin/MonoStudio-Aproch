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
#ifndef A_RIBBONTITLEBAR_H
#define A_RIBBONTITLEBAR_H

#include <QStyle>
#include <QWidget>
#include <QBoxLayout>
#include <QToolButton>
#include <QLineEdit>
#include "ARibbonTabBar.h"
#include "ARibbonSearchBar.h"

QTITAN_BEGIN_NAMESPACE

class ARibbonBar;
class RibbonButton;
class WindowTitleBar;
class TitleBarStyleOption;

class RibbonTitleBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RibbonTitleBarWidget(ARibbonBar* ribbonBar);
    ~RibbonTitleBarWidget() override;
public:
public:
    ARibbonBar* ribbonBar() const;
    RibbonTabBar* ribbonTabBar() const;
    RibbonSearchBar* searchBar() const;
    void addWidget(QWidget* widget, int stretch = 0);
    void insertWidget(int index, QWidget* widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
    int  indexOf(QWidget* widget);
    void addWidgetToRightSide(QWidget *widget, int stretch = 0);
    void removeWidget(QWidget* widget);
    void adjustSizeTitleBar();
    void updateLayout();
    void setVisible(bool visible) override;
    QRect calcContextualAreaRect() const;
    WindowTitleBar* getWindowTitleBar() const;
    QAction* addTitleButton(const QIcon& icon, const QString& help);
    void removeTitleButton(QAction* action);
    void setSearchBarAppearance(ARibbonBar::SearchBarAppearance appearance);
    ARibbonBar::SearchBarAppearance searchBarAppearance() const;
public Q_SLOTS:
    void windowTitleChanged(const QString&);
protected:
    QWidget* topWidget() const;
    QWidget* titleBarWindow() const;
    void updateTextTitle();
    QFont titleFont() const;
    void initTitleBarOption(TitleBarStyleOption* opt) const;
    ContextualTab* hitContextHeaders(const QPoint& point) const;
    void updateContextualTabs();
    void removeContextualTabs();
    QSize calcMinSize(QWidget* widget) const;
    QRect calcTextRect() const;
    int lastLeftWidgetIndex() const;
protected:
    bool event(QEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private:
    ARibbonBar* m_ribbonBar;
    QBoxLayout* m_layout;
    QSpacerItem* m_leftItem;
    QSpacerItem* m_rightItem;
    ARibbonBar::SearchBarAppearance m_searchBarAppearance;
    RibbonSearchBar* m_searchBar;
    QVector<ContextualTab *> m_contextualTabs;
    QString m_strTitle;
    bool m_dirtyTextTitle;
    QWidget* _pTopWidget = nullptr;
private:
    Q_DISABLE_COPY(RibbonTitleBarWidget)
};

QTITAN_END_NAMESPACE

#endif // A_RIBBONTITLEBAR_H
