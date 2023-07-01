/****************************************************************************
 * @file    ARibbonCustomizeManager.h
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
#include <QList>
#include <QAction>
#include <QString>
#include <QToolBar>

#include "ARibbonBar.h"
#include "ARibbonPage.h"
#include "ARibbonGroup.h"
#include "Common/AXmlAccess.h"

APROCH_NAMESPACE_BEGIN

class ARibbonCustomizeManagerPrivate;
class APROCH_API ARibbonCustomizeManager : public QObject
{
    Q_OBJECT
public:
    explicit ARibbonCustomizeManager(ARibbonBar* ribbonBar);
    virtual ~ARibbonCustomizeManager();
public:
    QStringList categories() const;
    QList<QAction*> actionsAll() const;

    void addToolBar(QToolBar* toolBar);

    void addAllActionsCategory(const QString& category);

    QList<QAction*> actionsByCategory(const QString& category) const;
    void addToCategory(const QString& category, QAction* act);
    void removeFromCategory(const QString& category, QAction* act);

    QList<QToolBar*> toolBarsByCategory(const QString& category) const;
    void addToCategory(const QString& category, QToolBar* toolBar);

    QList<ARibbonPage* > pagesByCategory(const QString& category) const;
    void addToCategory(const QString& category, ARibbonPage* page);

    QList<ARibbonGroup* > groupsByCategory(const QString& category) const;
    void addToCategory(const QString& category, ARibbonGroup* group);

    QString actionId(QAction* action) const;
    void setActionId(QAction* action, const QString& id);

    QString pageId(ARibbonPage* page) const;
    void setPageId(ARibbonPage* page, const QString& id);

    QString groupId(ARibbonGroup* group) const;
    void setGroupId(ARibbonPage* page, ARibbonGroup* group, const QString& id);

    void setEditMode(bool set = true);
    bool isEditMode() const;

    void reset(QToolBar* toolBar = nullptr);
    void commit();
    void cancel();

    QList< QAction* > actions(QToolBar* toolBar) const;
    void insertAction(QToolBar* toolBar, QAction* action, int index);
    void removeActionAt(QToolBar* toolBar, int index);
    bool containsAction(QToolBar* toolBar, QAction* action) const;

    QList<ARibbonPage*> pages() const;
    ARibbonPage* createPage(const QString& pageName, int index = -1);
    void deletePage(ARibbonPage* page);
    int pageIndex(ARibbonPage* page) const;
    void movePage(int oldIndex, int newIndex);

    QList<ARibbonGroup*> pageGroups(ARibbonPage* page) const;
    ARibbonGroup* createGroup(ARibbonPage* page, const QString& groupName, int index = -1);
    void deleteGroup(ARibbonPage* page, int index);
    void moveGroup(ARibbonPage* page, int oldIndex, int newIndex);

    void insertAction(ARibbonGroup* group, QAction* action, int index);
    void appendActions(ARibbonGroup* group, const QList<QAction*>& actions);
    void clearActions(ARibbonGroup* group);
    void removeActionAt(ARibbonGroup* group, int index);
    bool containsAction(ARibbonGroup* group, QAction* action) const;
    QList<QAction*> actionsGroup(ARibbonGroup* group) const;

    bool isPageHidden(ARibbonPage* page) const;
    void setPageHidden(ARibbonPage* page, bool hide);

    bool isPageVisible(ARibbonPage* page) const;
    void setPageVisible(ARibbonPage* page, bool visible);

    QString pageName(ARibbonPage* page) const;
    void setPageName(ARibbonPage* page, const QString& pageName);

    QString groupName(ARibbonGroup* group) const;
    void setGroupName(ARibbonGroup* group, const QString& groupName);

    bool saveStateToDevice(QIODevice* device, bool autoFormatting = true);
    bool saveStateToXML(IXmlStreamWriter* xmlwriter);
    bool loadStateFromDevice(QIODevice* device);
    bool loadStateFromXML(IXmlStreamReader* xmlreader);
    void setDefaultState();

    /*
     * @brief 是否允许修改页面
     */
    void enableModifyPage(bool bEnable = true);

    // TODO
Q_SIGNALS:
    /**
    * @brief 动作转换为名称ID
    * @param [in] pAction 动作
    * @param [out] strNameId 名称ID
    */
    void ActionToNameId(const QAction* pAction, QString& strNameID);

    /**
    * @brief 名称ID转动作
    * @param [in] strNameID 名称ID
    * @param [out] ppAction 动作
    */
    void NameIdToAction(const QString& strNameID, QAction** ppAction);

private:
    friend class ARibbonQuickAccessBar;
    friend class ARibbonBarCustomizePagePrivate;
    Q_DECLARE_PRIVATE(ARibbonCustomizeManager);
    Q_DISABLE_COPY(ARibbonCustomizeManager);
};

APROCH_NAMESPACE_END