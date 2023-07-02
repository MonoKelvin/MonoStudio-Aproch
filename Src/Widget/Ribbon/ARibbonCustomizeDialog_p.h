/****************************************************************************
 * @file    ARibbonCustomizeDialog_p.h
 * @date    2023-07-02 
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
#include <QListWidget>
#include <QLayout>
#include <QDialogButtonBox>
#include <QStyledItemDelegate>

#include "ARibbonCustomizePage.h"
#include "ui_UI_RibbonQuickAccessBarCustomizePage.h"
#include "ui_UI_RibbonBarCustomizePage.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class ARibbonCustomizePageWidget;
/* ARibbonCustomizePageItem */
class ARibbonCustomizePageItem : public QListWidgetItem
{
public:
    explicit ARibbonCustomizePageItem(const QString& text, ARibbonCustomizePageWidget* page) 
        : QListWidgetItem(text)
        , m_page(page)
    {}
public:
    ARibbonCustomizePageWidget* m_page;
};

/* ARibbonCustomizeListWidget */
class ARibbonCustomizeListWidget : public QListWidget
{
public:
    explicit ARibbonCustomizeListWidget(QWidget* parent = nullptr);
    virtual ~ARibbonCustomizeListWidget();
public:
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
};

/* ARibbonCustomizeDialogPrivate */
class ARibbonCustomizeDialogPrivate : public QObject
{
    Q_OBJECT
public:
    A_DECLARE_PUBLIC(ARibbonCustomizeDialog)
public:
    explicit ARibbonCustomizeDialogPrivate();
    virtual ~ARibbonCustomizeDialogPrivate();
public:
    void init();
    void setCustomizeMode(bool edit);
    void addPage(ARibbonCustomizePageWidget* page);
    void insertPage(int index, ARibbonCustomizePageWidget* page);
public Q_SLOTS:
    void switchToPage(int currentRow);
public:
    QList<QWidget*> m_listWidget;
    QVBoxLayout* m_verticalLayout;
    QHBoxLayout* m_horizontalLayout;
    QListWidget* m_listPage;
    QFrame* m_pageFrame;
    QDialogButtonBox *m_buttonBox;
    int m_currentPage;
    QVBoxLayout* m_pageVBoxLayout;
private:
    Q_DISABLE_COPY(ARibbonCustomizeDialogPrivate)
};

/* ARibbonCustomizeStyledItemDelegate */
class ARibbonCustomizeStyledItemDelegate : public QStyledItemDelegate
{
public:
    explicit ARibbonCustomizeStyledItemDelegate(QListWidget* listWidget);
    virtual ~ARibbonCustomizeStyledItemDelegate();
public:
    virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
protected:
    QListWidget* m_listWidget;
private:
    Q_DISABLE_COPY(ARibbonCustomizeStyledItemDelegate)
};

class ARibbonBar;
class ARibbonPage;
/* ARibbonQuickAccessBarCustomizePagePrivate */
class ARibbonQuickAccessBarCustomizePagePrivate : public QObject
{
    Q_OBJECT
public:
    A_DECLARE_PUBLIC(ARibbonQuickAccessBarCustomizePage)
    enum ColumnCommand
    {
        nIconCommand = 0,
        nNameCommand,
        nViewCommand
    };
public:
    explicit ARibbonQuickAccessBarCustomizePagePrivate();
    virtual ~ARibbonQuickAccessBarCustomizePagePrivate();
public:
    void init();
    void setupPage();
    void initListWidgetCommands();
    QIcon createIconStyleWidget(QWidget* widget);
    QPixmap createIconExStyleWidget(QWidget* widget, bool& iconView);

    void fillActions();
    void fillStateCommands();
    void fillListCategorieCommads(const QList<QAction*>& actions);

    void setButtons();
    void applyClicked();
public Q_SLOTS:
    void setCurrentCategoryText(const QString&);
    void currentCommandChanged(QListWidgetItem* current);
    void currentQTCommandChanged(QListWidgetItem* current);

    void addClicked();
    void removeClicked();
    void resetClicked();
    void upClicked();
    void downClicked();
public:
    Ui::ARibbonQuickAccessBarCustomizePage m_pageUI;
    ARibbonBar* m_ribbonBar;

    QStringList m_sourceCategories;
    QAction* m_separator;
    QString m_separatorText;
    QString m_strSeparator;
    QString m_currentSourceCategory;

    QListWidgetItem* m_currentAction;
    QMap<QAction*, QListWidgetItem*> m_actionToItem;
    QMap<QListWidgetItem*, QAction*> m_itemToAction;

    QMap<QAction*, QListWidgetItem*> m_actionToCurrentItem;
    QMap<QListWidgetItem*, QAction*> m_currentItemToAction;

    int m_heightRowItem;
    int m_widthColIconItem;
    int m_widthColViewItem;
    bool m_wasDisplayed;
private:
    Q_DISABLE_COPY(ARibbonQuickAccessBarCustomizePagePrivate)
};

class ARibbonGroup;
/* ARibbonBarCustomizePagePrivate */
class ARibbonBarCustomizePagePrivate : public QObject
{
    Q_OBJECT
public:
    A_DECLARE_PUBLIC(ARibbonBarCustomizePage)
public:
    explicit ARibbonBarCustomizePagePrivate();
    virtual ~ARibbonBarCustomizePagePrivate();
public:
    void init();
    void setupPage();
    void clearCategoryCaptionTree(QTreeWidget* tree);
    void fillSourceCategories();
    void fillSourceActions(QList<QAction*>& actions, QTreeWidgetItem* itemParent = nullptr);
    void fillSourcePages(QList<ARibbonPage*>& pages, const QString& strCategory);
    void fillSourceGroups(ARibbonPage* page, QTreeWidgetItem* itemParent);
    void fillSourceGroupActions(ARibbonGroup* group, QTreeWidgetItem* itemParent);

    void fillStateCategories();
    void fillStateRibbon(const QString& strCategory);
    void fillStateGroups(ARibbonPage* page, QTreeWidgetItem* itemParent);
    void fillStateActions(ARibbonGroup* group, QTreeWidgetItem* itemParent, bool newGroup);

    ARibbonPage* addPageCustom(ARibbonPage* srcPage);
    void addGroupCopy(ARibbonPage* srcPage, ARibbonPage* copyPage);
    void addActionGroupCustom(ARibbonGroup* group, QAction* action, int index);
    void setButtons();

    void applyClicked();
    void cancelClicked();

    /*
    * @brief 
    * @param [in] bEnable
    */
    void enableVisualizePage(bool bEnable = true);

    /*
    * @brief 
	* @param [in] bEnable
	*/
    void enableAddPage(bool bEnable = true);

    /*
    * @brief 
	* @param [in] bEnable
	*/
    void enableRenamePage(bool bEnable = true);

    /*
	 * @brief 
	 * @param [in] bEnable
	 */
    void enableMovePage(bool bEnable = true);

public Q_SLOTS:
    void addClicked();
    void newTabClicked();
    void newGroupClicked();
    void removeClicked();
    void resetClicked();
    void upClicked();
    void downClicked();
    void renameClicked();
    void setCurrentCategoryText(const QString&);
    void setCurrentTabsText(const QString&);

    void currentSourceChanged(QTreeWidgetItem* itemPage);

    void currentPageChanged(QTreeWidgetItem* itemPage);
    void itemPageChanged(QTreeWidgetItem* itemPage, int);

public:
    ARibbonBar* m_ribbonBar;
    Ui::RibbonBarCustomizePage m_pageUI;

    QString m_strSeparator;
    QString m_currentSourceCategory;

    QStringList m_sourceCategories;
    QTreeWidgetItem* m_currentSourceItemPage;
    QTreeWidgetItem* m_currentSourceItemGroup;
    QTreeWidgetItem* m_currentSourceAction;
    QMap< QTreeWidgetItem*, ARibbonPage* > m_currentSourceItemToPage;
    QMap< QTreeWidgetItem*, ARibbonGroup* > m_currentSourceItemToGroup;
    QMap< QTreeWidgetItem*, QAction* > m_currentSourceItemToAction;

    QTreeWidgetItem* m_currentItemPage;
    QTreeWidgetItem* m_currentItemGroup;
    QTreeWidgetItem* m_currentItemAction;
    QMap<ARibbonPage*, QTreeWidgetItem*> m_pageToCurrentItem;
    QMap<QTreeWidgetItem*, ARibbonPage*> m_currentItemToPage;

    QMap< QAction*, QTreeWidgetItem* > m_actionToCurrentItem;
    QMap< QTreeWidgetItem*, QAction* > m_currentItemToAction;
    QMap< QTreeWidgetItem*, ARibbonGroup* > m_currentItemToGroup;

    QString m_sufNameCustom;
    bool m_wasDisplayed;
    bool _bEnableVisualizePage = true;     
    bool _bEnableRenamePage = true;        
    bool _bEnableMove = true;

private:
    Q_DISABLE_COPY(ARibbonBarCustomizePagePrivate)
};

APROCH_NAMESPACE_END