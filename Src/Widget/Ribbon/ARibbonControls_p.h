/****************************************************************************
 * @file    ARibbonControls_p.h
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
#include <QObject>

#include "Widget/APopupMenu.h"
#include "ARibbonControls.h"
#include "ARibbonButtonControls.h"
#include "ARibbonButton.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

/* ARibbonControlSizeDefinitionPrivate */   
class ARibbonControlSizeDefinitionPrivate : public QObject
{
    Q_OBJECT
public:
    A_DECLARE_PUBLIC(ARibbonControlSizeDefinition)
public:
    explicit ARibbonControlSizeDefinitionPrivate();
    virtual ~ARibbonControlSizeDefinitionPrivate();
    void init(ARibbonControl* parent, ARibbonControlSizeDefinition::GroupSize size);
    void updateSizeDefinition();
public:
    ARibbonControl* m_parentControl;
    ARibbonControlSizeDefinition::ControlImageSize m_imageSize;
    bool m_labelVisible : 1;
    bool m_popup : 1;
    bool m_showSeparator : 1;
    bool m_visible : 1;
    bool m_enabled : 1;
    bool m_stretchable : 1;
    bool m_wordWrap : 1;
    int m_visualIndex;
    int m_minItemCount;
    int m_maxItemCount;
private:
    Q_DISABLE_COPY(ARibbonControlSizeDefinitionPrivate)
};

/* ARibbonControlPrivate */   
class ARibbonControlPrivate : public QObject
{
    Q_OBJECT
public:
    A_DECLARE_PUBLIC(ARibbonControl)
public:
    explicit ARibbonControlPrivate();
    virtual ~ARibbonControlPrivate();
public:
    void init(ARibbonGroup* parentGroup);
    void changed();
public:
    ARibbonGroup* m_parentGroup;
    QList<ARibbonControlSizeDefinition*> m_allSizes;
    ARibbonControlSizeDefinition::GroupSize m_size;
    QAction* m_action;
private:
    Q_DISABLE_COPY(ARibbonControlPrivate)
};

/* ARibbonWidgetControlPrivate */   
class ARibbonWidgetControlPrivate : public QObject
{
    Q_OBJECT
public:
    A_DECLARE_PUBLIC(ARibbonWidgetControl)
public:
    explicit ARibbonWidgetControlPrivate();
    virtual ~ARibbonWidgetControlPrivate();
public:
    void init(bool ignoreActionSettings);
    void initStyleOption(QStyleOptionButton* option) const;
    void updateFont();
public:
    QWidget* m_widget;
    bool m_visibleText;
    bool m_ignoreActionSettings;
    int m_margin;
private:
    Q_DISABLE_COPY(ARibbonWidgetControlPrivate)
};


/* ARibbonColumnBreakControlPrivate */   
class ARibbonColumnBreakControlPrivate : public QObject
{
    Q_OBJECT
public:
    A_DECLARE_PUBLIC(ARibbonColumnBreakControl)
public:
    explicit ARibbonColumnBreakControlPrivate();
    virtual ~ARibbonColumnBreakControlPrivate();
public:
    void init();
    void initStyleOption(QStyleOptionGroupBox* option) const;
public:
    Qt::Alignment m_textAlignment;
private:
    Q_DISABLE_COPY(ARibbonColumnBreakControlPrivate)
};

/* ARibbonButtonControlPrivate */   
class ARibbonButtonControlPrivate : public QObject
{
    Q_OBJECT
public:
    A_DECLARE_PUBLIC(ARibbonButtonControl)
public:
    explicit ARibbonButtonControlPrivate();
    virtual ~ARibbonButtonControlPrivate();
    void init();
public:
    ARibbonButton* m_toolButton;
    QIcon m_largeIcon;
    QIcon m_smallIcon;
    QString m_label;
private:
    Q_DISABLE_COPY(ARibbonButtonControlPrivate)
};

class ARibbonToolBarSeparator : public QWidget
{
    Q_OBJECT
public:
    explicit ARibbonToolBarSeparator(ARibbonToolBarControl* parent);
public:
    virtual QSize sizeHint() const override;
protected:
    virtual void paintEvent(QPaintEvent* event) override;
private:
    Q_DISABLE_COPY(ARibbonToolBarSeparator)
};

/* ARibbonToolBarOverflowMenu */
class ARibbonToolBarOverflowMenu : public APopupMenu
{
    Q_OBJECT
public:
    ARibbonToolBarOverflowMenu(ARibbonToolBarControl* parent);
    virtual ~ARibbonToolBarOverflowMenu();
public:
    void fillMenu();
    void clearMenu();
protected:
    virtual bool event(QEvent* event) override;
    virtual void paintMenuItem(QPainter* painter, QStyleOptionMenuItem* option) override
    { APopupMenu::paintMenuItem(painter, option); }
private:
    ARibbonToolBarControl* m_parent;
    Q_DISABLE_COPY(ARibbonToolBarOverflowMenu);
};

/* ARibbonToolBarControlPrivate */   
class ARibbonToolBarControlPrivate : public QObject
{
    Q_OBJECT
public:
    A_DECLARE_PUBLIC(ARibbonToolBarControl)
    struct ARibbonToolBarActionGroupItem
    {
        QRect rect;
        QAction* action;
        QWidget* widget;
        ARibbonToolBarActionGroupItem(QAction* act, QWidget* w) : action(act), widget(w) {}
        bool isEmpty() const { return action == nullptr || !action->isVisible();  }
    };
    typedef QList<ARibbonToolBarActionGroupItem> ARibbonToolBarActionGroupItems;

    struct ARibbonToolBarActionsGroup
    {
        int row;
        QRect rect;
        QSize defaultSize;
        ARibbonToolBarActionGroupItems items;
        ARibbonToolBarActionsGroup() : row(0) {}
    };
    typedef QList<ARibbonToolBarActionsGroup> ARibbonToolBarActionGroupList;
public:
    explicit ARibbonToolBarControlPrivate();
    virtual ~ARibbonToolBarControlPrivate();
    inline static ARibbonToolBarControlPrivate* _get(ARibbonToolBarControl* toolBar)
    { return &toolBar->aproch_d(); }
    void init();
public:
    void recalcWidths(int height);
    void showDefaultButton();
    void addActionToGroup(QAction* action, QWidget* widget);
    void removeActionFromGroup(QAction* action);
    void updateGroupsGeometry(const QRect& rect, bool updateWidgets);
    void updateGroupProperties(ARibbonToolBarActionGroupItems& group);
    void updateGeometryWidget();
    void updateGroupItems();
    QSize calcGroupSizeHint(const ARibbonToolBarActionGroupItems& group) const;
    void updateFont();
public:
    ARibbonToolBarOverflowMenu* m_overflowMenu;
    ARibbonToolBarActionGroupList m_groups;
    int m_margin;
    int m_rowsCount;
    QSize m_sizeHint;

    QVector<int> m_vecWidths;
    QMap<int, int> m_mapNonOptWidths;
    bool m_dirty;
    int m_calcRows;
    int m_currColumnIndex;
    int m_fullWidth;
    int m_separatorWidth;
private:
    Q_DISABLE_COPY(ARibbonToolBarControlPrivate)
};

APROCH_NAMESPACE_END