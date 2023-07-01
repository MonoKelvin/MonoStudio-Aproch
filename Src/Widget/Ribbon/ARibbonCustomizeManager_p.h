/****************************************************************************
 * @file    ARibbonCustomizeManager_p.h
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
#include <QMap>
#include <QSet>
#include <QList>
#include <QAction>
#include <QToolBar>
#include "ARibbonCustomizeManager.h"

APROCH_NAMESPACE_BEGIN

#define UserNameID 1000     // first user name id
#define MaxUserNameID 65535 // last user name id

#define __aproch_Widget_Custom "WidgetCustom"
#define __aproch_Widget_Copy "WidgetCopy"
#define __aproch_Action_Invisible "__aproch_Action_Invisible"
#define __aproch_Quick_Access_Button "__aproch_Quick_Access_Button"

/* ACustomizeEngineAbstract */
class ACustomizeEngineAbstract : public QObject
{
    Q_OBJECT;

public:
    explicit ACustomizeEngineAbstract(QObject *parent);
    virtual ~ACustomizeEngineAbstract();

public:
    QString categoryAllActions() const;
    QList<QAction *> actionsAll() const;

    QList<QAction *> actionsByCategory(const QString &category) const;
    QStringList categories() const;

    QString actionId(QAction *action) const;
    void setActionId(QAction *action, const QString &id = QString());
    void unsetActionId(QAction *action);

    void addToCategory(const QString &category, QAction *action);
    void removeFromCategory(const QString &category, QAction *action);
    void addAllActionsCategory(const QString &category);

    virtual void saveState(IXmlStreamWriter *xmlwriter) = 0;
    virtual bool restoreState(IXmlStreamReader *xmlreader) = 0;

protected:
    QString generateUniqueNameIdentifier(int hint = -1);
    QString generateUniqueNameIdentifierGroup(int hint = -1);
    QString generateUniqueNameIdentifierPage(int hint = -1);

protected:
    QSet<int> m_setUniqueIdentifier;
    QSet<int> m_setUniqueIdentifierGroup;
    QSet<int> m_setUniqueIdentifierPage;

    QString m_allCategoryName;
    QList<QString> m_lstCategory;

    QMap<QString, QAction *> m_allNameToActions;
    QMap<QAction *, QString> m_allActionsToName;

    QMap<QString, QList<QAction *>> m_categoryToActions;
    QMap<QAction *, QString> m_actionToCategory;

private:
    Q_DISABLE_COPY(ACustomizeEngineAbstract);
};

/* AToolBarCustomizeEngine */
class AToolBarCustomizeEngine : public ACustomizeEngineAbstract
{
    Q_OBJECT
public:
    explicit AToolBarCustomizeEngine(QObject *parent);
    virtual ~AToolBarCustomizeEngine();

public:
    QList<QAction *> actions(QToolBar *toolBar) const;
    QToolBar *toolBarWidgetAction(QAction *action) const;
    QToolBar *findDefaultToolBar(const QString &objectName) const;
    QMap<QToolBar *, QList<QAction *>> defaultToolBars() const;

    void removeWidgetActions(const QMap<QToolBar *, QList<QAction *>> &actions);
    void addDefaultToolBar(QToolBar *toolBar);

    QList<QToolBar *> toolBarsByCategory(const QString &category) const;
    void addToCategory(const QString &category, QToolBar *bar);

    void setToolBar(QToolBar *toolBar, const QList<QAction *> &actions);

    void saveStateQuickAccessBar(IXmlStreamWriter *xmlwriter);
    bool restoreStateQuickAccessBar(IXmlStreamReader *xmlreader);

    void saveState(IXmlStreamWriter *xmlwriter);
    bool restoreState(IXmlStreamReader *xmlreader);

protected:
    QMap<QAction *, QToolBar *> m_widgetActions;
    QMap<QAction *, QList<QToolBar *>> m_actionToToolBars;

    QMap<QToolBar *, QString> m_barToCategory;

    QMap<QToolBar *, QList<QAction *>> m_defaultToolBars;
    QMap<QToolBar *, QList<QAction *>> m_toolBarsWithSeparators;
    QMap<QToolBar *, QList<QAction *>> m_toolBars;
    QMap<QToolBar *, QList<QAction *>> m_regularToolBars;

protected:
signals:
    /**
     * @brief 动作转换为名称ID
     * @param [in] pAction 动作
     * @param [out] strNameId 名称ID
     */
    void ActionToNameId(const QAction *pAction, QString &strNameID);

    /**
     * @brief 名称ID转动作
     * @param [in] strNameID 名称ID
     * @param [out] ppAction 动作
     */
    void NameIdToAction(const QString &strNameID, QAction **ppAction);

private:
    Q_DISABLE_COPY(AToolBarCustomizeEngine)
};

class ARibbonBar;
class ARibbonPage;
class ARibbonGroup;
class ARibbonBarCustomizeEngine : public AToolBarCustomizeEngine
{
    Q_OBJECT
public:
    explicit ARibbonBarCustomizeEngine(QObject *parent, ARibbonBar *ribbonBar);
    virtual ~ARibbonBarCustomizeEngine();

public:
    QString pageId(ARibbonPage *page) const;
    void setPageId(ARibbonPage *page, const QString &id = QString());

    QString groupId(ARibbonGroup *group) const;
    void setGroupId(ARibbonPage *page, ARibbonGroup *group, const QString &id = QString());

    void addToCategory(const QString &category, QAction *act);
    void removeFromCategory(const QString &category, QAction *action);

    QList<ARibbonPage *> pagesByCategory(const QString &category) const;
    void addToCategory(const QString &category, ARibbonPage *page);

    QList<ARibbonGroup *> groupsByCategory(const QString &category) const;
    void addToCategory(const QString &category, ARibbonGroup *group);

    QList<ARibbonGroup *> pageDefaultGroups(ARibbonPage *page) const;
    QList<ARibbonPage *> defaultHiddenPages() const;
    QList<ARibbonPage *> editHiddenPages() const;

    const QMap<ARibbonPage *, QString> &defaultPageNames() const;
    QString defaultPageName(ARibbonPage *page) const;

    QList<ARibbonGroup *> regularGroups(ARibbonPage *page) const;
    const QMap<ARibbonPage *, QList<ARibbonGroup *>> &defaultPageGroups() const;
    const QMap<ARibbonPage *, QList<ARibbonGroup *>> &regularPageGroups() const;
    const QMap<ARibbonGroup *, QString> &defaultGroupNames() const;
    QString defaultGroupName(ARibbonGroup *group) const;

    const QList<ARibbonPage *> &defaultPages() const;

    void addDefaultPages();
    void addDefaultGroups(ARibbonPage *page);
    void addNonContextCategories();

    void setQuickAccessBar();
    void setRibbonBar();
    void setRibbonGroups();
    void updateRibbonBar();
    void deleteFreeGroups();
    void deleteFreePages();

    void saveStateGroup(IXmlStreamWriter *xmlwriter, ARibbonPage *page);
    bool restoreStateGroup(IXmlStreamReader *xmlreader, ARibbonPage *page);

    void saveStatePage(IXmlStreamWriter *xmlwriter);
    bool restoreStatePage(IXmlStreamReader *xmlreader);

    void saveStateRibbonBar(IXmlStreamWriter *xmlwriter);
    bool restoreStateRibbonBar(IXmlStreamReader *xmlreader);

    void saveState(IXmlStreamWriter *xmlwriter);
    bool restoreState(IXmlStreamReader *xmlreader);

protected:
    ARibbonBar *m_ribbonBar;

    QMap<ARibbonPage *, QString> m_pageToCategory;
    QMap<ARibbonGroup *, QString> m_groupToCategory;

    QList<ARibbonPage *> m_defaultPages;
    QList<ARibbonPage *> m_defaultHiddenPages;
    QMap<ARibbonPage *, QString> m_defaultPagesName;
    QMap<ARibbonPage *, QList<ARibbonGroup *>> m_defaultPageGroups;
    QMap<ARibbonGroup *, QString> m_defaultNameGroups;

    QList<ARibbonPage *> m_regularPages;
    QMap<ARibbonPage *, QString> m_regularPagesName;
    QList<ARibbonPage *> m_regularHiddenPages;
    QMap<ARibbonPage *, QList<ARibbonGroup *>> m_regularPageGroups;
    QMap<ARibbonGroup *, QString> m_regularNameGroups;

    QMap<QString, ARibbonPage *> m_allNameIdToPages;

    QMap<QString, ARibbonGroup *> m_allNameIdToGroups;

    QMap<ARibbonGroup *, QList<QAction *>> m_regularGroupsActions;

private:
    friend class ARibbonCustomizeManager;
    Q_DISABLE_COPY(ARibbonBarCustomizeEngine)
};

class ARibbonCustomizeManagerPrivate : public QObject
{
    Q_OBJECT
public:
    A_DECLARE_PUBLIC(ARibbonCustomizeManager)
public:
    explicit ARibbonCustomizeManagerPrivate();
    virtual ~ARibbonCustomizeManagerPrivate();

public:
    void init(ARibbonBar *ribbonBar);

    /*
     * @brief 允许还原页面可见性
     */
    void enableRestoreVisualizePage(bool bEnable = true);

    /*
     * @brief 记录页签可见性
     */
    void storePageVisual();

    /*
     * @brief 还原页签可见性
     */
    void restorePageVisual();

public:
    QMap<QString, bool> _pagesVisual;
    bool _bRestoreVisualizePage = false;
    bool m_editMode;
    ARibbonBar *m_ribbonBar;
    ARibbonBarCustomizeEngine *m_customizeEngine;

private:
    Q_DISABLE_COPY(ARibbonCustomizeManagerPrivate)
};

APROCH_NAMESPACE_END
