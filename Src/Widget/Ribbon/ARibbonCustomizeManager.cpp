/****************************************************************************
 * @file    ARibbonCustomizeManager.cpp
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
#include "stdafx.h"
#include <QBuffer>

#include "ARibbonCustomizeManager.h"
#include "ARibbonCustomizeManager_p.h"
#include "ARibbonBar_p.h"
#include "ARibbonPage.h"
#include "ARibbonPage_p.h"
#include "ARibbonGroup.h"
#include "ARibbonQuickAccessBar.h"
#include "ARibbonDef.h"

APROCH_NAMESPACE_BEGIN

#define A_XML_TAGNAME_RIBBON_STORAGE   QStringLiteral("RibbonStorage")
#define A_XML_TAGNAME_RIBBON_QABAR     QStringLiteral("QuickAccessBar")
#define A_XML_TAGNAME_RIBBON_QABARITEM QStringLiteral("QuickAccessBarItem")
#define A_XML_TAGNAME_RIBBON_BAR       QStringLiteral("ARibbonBar")
#define A_XML_TAGNAME_RIBBON_PAGE      QStringLiteral("ARibbonPage")
#define A_XML_TAGNAME_RIBBON_GROUP     QStringLiteral("ARibbonGroup")
#define A_XML_TAGNAME_RIBBON_ACTION    QStringLiteral("RibbonAction")

#define A_XML_ATTRNAME_RIBBON_ID       QStringLiteral("nameId")
#define A_XML_ATTRNAME_RIBBON_TITLE    QStringLiteral("title")
#define A_XML_ATTRNAME_RIBBON_VISIBLE  QStringLiteral("visible")
#define A_XML_ATTRNAME_RIBBON_TYPE     QStringLiteral("type")

/* ACustomizeEngineAbstract */
ACustomizeEngineAbstract::ACustomizeEngineAbstract(QObject* parent)
    : QObject(parent)
{
}

ACustomizeEngineAbstract::~ACustomizeEngineAbstract()
{
}

QString ACustomizeEngineAbstract::categoryAllActions() const
{
    return m_allCategoryName;
}

QList<QAction*> ACustomizeEngineAbstract::actionsAll() const
{
    return m_allNameToActions.values();
}

QList<QAction*> ACustomizeEngineAbstract::actionsByCategory(const QString& category) const
{
    QMap< QString, QList<QAction*> >::ConstIterator it = m_categoryToActions.find(category);
    if (it != m_categoryToActions.constEnd())
        return it.value();
    return QList<QAction*>();
}

QStringList ACustomizeEngineAbstract::categories() const
{
    return m_lstCategory;
}

QString ACustomizeEngineAbstract::actionId(QAction* action) const
{
    return m_allActionsToName.value(action);
}

void ACustomizeEngineAbstract::setActionId(QAction* action, const QString& id)
{
    QString cmdName = id;

    if (action == nullptr || action->isSeparator())
        return;

    if (cmdName.isEmpty())
    {
        //        qWarning("ACustomizeEngineAbstract::setActionId(): 'commandName' not set for QAction "
        //            "%p '%s', using 'text' instead", action, action->text().toLocal8Bit().constData());
        cmdName = action->text();

        if (!cmdName.isEmpty())
            cmdName += QLatin1Char('_');

        cmdName += ACustomizeEngineAbstract::generateUniqueNameIdentifier();
    }

    if (cmdName.isEmpty())
    {
        Q_ASSERT(false);
        return;
    }

    if (m_allNameToActions.contains(cmdName))
        return;

    if (m_allActionsToName.contains(action))
        return;

    m_allNameToActions.insert(cmdName, action);
    m_allActionsToName.insert(action, cmdName);

    if (!m_allCategoryName.isEmpty())
    {
        m_categoryToActions[m_allCategoryName].append(action);
        m_actionToCategory[action] = m_allCategoryName;
    }
}

void ACustomizeEngineAbstract::unsetActionId(QAction* action)
{
    if (action == nullptr || action->isSeparator())
        return;

    if (!m_allActionsToName.contains(action))
        return;

    QString cmdName = m_allActionsToName[action];
    if (!cmdName.isEmpty())
        m_allNameToActions.remove(cmdName);
    m_allActionsToName.remove(action);

    if (!m_allCategoryName.isEmpty())
    {
        m_categoryToActions[m_allCategoryName].removeOne(action);
        m_actionToCategory.remove(action);
    }
}

void ACustomizeEngineAbstract::addAllActionsCategory(const QString& category)
{
    if (!category.isEmpty() && !m_lstCategory.contains(category))
    {
        m_allCategoryName = category;
        m_lstCategory.append(category);
    }
}

void ACustomizeEngineAbstract::addToCategory(const QString& category, QAction* action)
{
    if (category.isEmpty() || action == nullptr || action->isSeparator())
        return;

    m_categoryToActions[category].append(action);
    m_actionToCategory[action] = category;

    if (!m_lstCategory.contains(category))
        m_lstCategory.append(category);

    setActionId(action);
}

void ACustomizeEngineAbstract::removeFromCategory(const QString& category, QAction* action)
{
    if (category.isEmpty() || action == nullptr || action->isSeparator())
        return;

    if (m_lstCategory.contains(category))
    {
        m_categoryToActions[category].removeOne(action);
        m_actionToCategory.remove(action);
    }
    unsetActionId(action);
}

QString ACustomizeEngineAbstract::generateUniqueNameIdentifier(int hint)
{
    static QString prefix = QStringLiteral("__aproch_Action_%1");
    if (hint >= UserNameID && hint <= MaxUserNameID && !m_setUniqueIdentifier.contains(hint))
    {
        m_setUniqueIdentifier.insert(hint);
        return prefix.arg(hint);
    }

    int id = MaxUserNameID;
    while (m_setUniqueIdentifier.contains(id) && id >= UserNameID)
        --id;

    if (id >= UserNameID)
    {
        m_setUniqueIdentifier.insert(id);
        return prefix.arg(id);
    }
    return QString();
}

QString ACustomizeEngineAbstract::generateUniqueNameIdentifierGroup(int hint/* = -1*/)
{
    static QString prefix = QStringLiteral("__aproch_Group_%1");
    if (hint >= UserNameID && hint <= MaxUserNameID && !m_setUniqueIdentifierGroup.contains(hint))
    {
        m_setUniqueIdentifierGroup.insert(hint);
        return prefix.arg(hint);
    }

    int id = MaxUserNameID;
    while (m_setUniqueIdentifierGroup.contains(id) && id >= UserNameID)
        --id;

    if (id >= UserNameID)
    {
        m_setUniqueIdentifierGroup.insert(id);
        return prefix.arg(id);
    }
    return QString();
}

QString ACustomizeEngineAbstract::generateUniqueNameIdentifierPage(int hint/* = -1*/)
{
    static QString prefix = QStringLiteral("__aproch_Page_%1");
    if (hint >= UserNameID && hint <= MaxUserNameID && !m_setUniqueIdentifierPage.contains(hint))
    {
        m_setUniqueIdentifierPage.insert(hint);
        return prefix.arg(hint);
    }

    int id = MaxUserNameID;
    while (m_setUniqueIdentifierPage.contains(id) && id >= UserNameID)
        --id;

    if (id >= UserNameID)
    {
        m_setUniqueIdentifierPage.insert(id);
        return prefix.arg(id);
    }
    return QString();
}


/* AToolBarCustomizeEngine */
AToolBarCustomizeEngine::AToolBarCustomizeEngine(QObject* parent)
    : ACustomizeEngineAbstract(parent)
{
}

AToolBarCustomizeEngine::~AToolBarCustomizeEngine()
{
}

QList<QAction*> AToolBarCustomizeEngine::actions(QToolBar* toolBar) const
{
    if (m_toolBars.contains(toolBar))
        return m_toolBars.value(toolBar);
    return QList<QAction*>();
}

QToolBar* AToolBarCustomizeEngine::toolBarWidgetAction(QAction* action) const
{
    if (m_widgetActions.contains(action))
        return m_widgetActions.value(action);
    return nullptr;
}

QToolBar* AToolBarCustomizeEngine::findDefaultToolBar(const QString& objectName) const
{
    QMap<QToolBar*, QList<QAction*> >::ConstIterator itToolBar = m_defaultToolBars.constBegin();
    while (itToolBar != m_defaultToolBars.constEnd())
    {
        QToolBar* tb = itToolBar.key();
        if (tb->objectName() == objectName)
            return tb;

        ++itToolBar;
    }

    qWarning("AToolBarCustomizeEngine::findDefaultToolBar(): cannot find a QToolBar named "
             "'%s', trying to match using 'windowTitle' instead.",
             objectName.toLocal8Bit().constData());

    itToolBar = m_defaultToolBars.constBegin();
    while (itToolBar != m_defaultToolBars.constEnd())
    {
        QToolBar* tb = itToolBar.key();
        if (tb->windowTitle() == objectName)
            return tb;
        ++itToolBar;
    }
    qWarning("AToolBarCustomizeEngine::findDefaultToolBar(): cannot find a QToolBar with "
             "matching 'windowTitle' (looking for '%s').",
             objectName.toLocal8Bit().constData());

    return nullptr;
}

QMap< QToolBar*, QList<QAction*> > AToolBarCustomizeEngine::defaultToolBars() const
{
    return m_defaultToolBars;
}

void AToolBarCustomizeEngine::removeWidgetActions(const QMap< QToolBar*, QList<QAction*> >& actions)
{
    QMap< QToolBar*, QList<QAction*> >::ConstIterator itWidget = actions.constBegin();
    while (itWidget != actions.constEnd())
    {
        QToolBar* toolBar = itWidget.key();
        QList< QAction* > newActions = m_toolBars.value(toolBar);
        QList< QAction* > newActionsWithSeparators = m_toolBarsWithSeparators.value(toolBar);

        QList< QAction* > removedActions;
        QList< QAction* > actionList = itWidget.value();
        QListIterator< QAction* > itAction(actionList);
        while (itAction.hasNext())
        {
            QAction* action = itAction.next();
            if (newActions.contains(action) && toolBarWidgetAction(action) == toolBar)
            {
                newActions.removeAll(action);
                newActionsWithSeparators.removeAll(action);
                removedActions.append(action);
            }
        }

        m_toolBars.insert(toolBar, newActions);
        m_toolBarsWithSeparators.insert(toolBar, newActionsWithSeparators);
        QListIterator< QAction* > itRemovedAction(removedActions);
        while (itRemovedAction.hasNext())
        {
            QAction* oldAction = itRemovedAction.next();
            m_widgetActions.insert(oldAction, 0);
            m_actionToToolBars[oldAction].removeAll(toolBar);
        }
        ++itWidget;
    }
}

void AToolBarCustomizeEngine::addDefaultToolBar(QToolBar* toolBar)
{
    if (!toolBar)
        return;

    if (m_toolBars.contains(toolBar))
        return;

    QList<QAction*> newActionsWithSeparators;
    QList<QAction*> newActions;
    QList<QAction*> actions = toolBar->actions();
    QListIterator<QAction*> itAction(actions);
    while (itAction.hasNext())
    {
        QAction* action = itAction.next();
        bool visible = action->isVisible();
        QString name = action->text();
        QString prop = action->property(__aproch_Action_Invisible).toString();
        if (visible && action->property(__aproch_Action_Invisible).toString() != QLatin1String(__aproch_Action_Invisible) &&
            action->property(__aproch_Quick_Access_Button).toString() != QLatin1String(__aproch_Quick_Access_Button))
        {
            setActionId(action);

            if (m_widgetActions.contains(action))
                m_widgetActions.insert(action, toolBar);

            newActionsWithSeparators.append(action);
            if (action->isSeparator())
                action = 0;
            else
                m_actionToToolBars[action].append(toolBar);
            newActions.append(action);
        }
    }

    m_defaultToolBars.insert(toolBar, newActions);
    m_toolBars.insert(toolBar, newActions);
    m_toolBarsWithSeparators.insert(toolBar, newActionsWithSeparators);
}

QList< QToolBar*> AToolBarCustomizeEngine::toolBarsByCategory(const QString& category) const
{
    QList< QToolBar*> result;

    for (QMap< QToolBar*, QString>::const_iterator it = m_barToCategory.constBegin(); it != m_barToCategory.constEnd(); ++it)
    {
        if (it.value() == category)
            result.append(it.key());
    }
    return result;
}

void AToolBarCustomizeEngine::addToCategory(const QString& category, QToolBar* bar)
{
    if (category.isEmpty() || bar == nullptr)
        return;

    if (m_barToCategory.contains(bar))
        return;

    m_barToCategory[bar] = category;

    if (!m_lstCategory.contains(category))
        m_lstCategory.append(category);
}

void AToolBarCustomizeEngine::setToolBar(QToolBar* toolBar, const QList< QAction* >& actions)
{
    if (!toolBar)
        return;
    if (!m_toolBars.contains(toolBar))
        return;
    if (actions == m_toolBars[toolBar])
        return;

    QMap< QToolBar*, QList<QAction*> > toRemove;
    QList< QAction* > newActions;
    QListIterator< QAction* > itAction(actions);
    while (itAction.hasNext())
    {
        QAction* action = itAction.next();
        if (!action || (!newActions.contains(action) && m_allNameToActions.values().contains(action)))
            newActions.append(action);

        QToolBar* oldToolBar = toolBarWidgetAction(action);
        if (oldToolBar && oldToolBar != toolBar)
            toRemove[oldToolBar].append(action);
    }

    removeWidgetActions(toRemove);

    QList< QAction* > oldActions = m_toolBarsWithSeparators.value(toolBar);
    QListIterator< QAction* > itOldAction(oldActions);
    while (itOldAction.hasNext())
    {
        QAction* action = itOldAction.next();
        if (action == nullptr)
            continue;

        if (toolBarWidgetAction(action) == toolBar)
            m_widgetActions.insert(action, 0);
        toolBar->removeAction(action);
        if (action->isSeparator())
            delete action;
        else
            m_actionToToolBars[action].removeAll(toolBar);
    }

    QList< QAction* > newActionsWithSeparators;
    QListIterator< QAction* > itNewActions(newActions);
    while (itNewActions.hasNext())
    {
        QAction* action = itNewActions.next();
        QAction* newAction = nullptr;
        if (!action)
            newAction = toolBar->insertSeparator(0);

        if (m_allNameToActions.values().contains(action))
        {
            toolBar->insertAction(0, action);
            newAction = action;
            m_actionToToolBars[action].append(toolBar);
        }
        newActionsWithSeparators.append(newAction);
    }
    m_toolBars.insert(toolBar, newActions);
    m_toolBarsWithSeparators.insert(toolBar, newActionsWithSeparators);
}

void AToolBarCustomizeEngine::saveStateQuickAccessBar(IXmlStreamWriter* xmlwriter)
{
    xmlwriter->writeStartElement(A_XML_TAGNAME_RIBBON_QABAR);
    QMap< QToolBar*, QList<QAction*> >::ConstIterator itToolBar = m_defaultToolBars.constBegin();
    while (itToolBar != m_defaultToolBars.constEnd())
    {
        xmlwriter->writeStartElement(A_XML_TAGNAME_RIBBON_QABARITEM);

        QToolBar* tb = itToolBar.key();
        QString title = tb->objectName();
        if (tb->objectName().isEmpty())
        {
            qWarning("AToolBarCustomizeEngine::saveStateQuickAccessBar(): 'objectName' not set for QToolBar "
                     "%p '%s', using 'windowTitle' instead",
                     tb, tb->windowTitle().toLocal8Bit().constData());
            title = tb->windowTitle();
        }

        xmlwriter->writeAttribute(A_XML_ATTRNAME_RIBBON_TITLE, title);

        QListIterator<QAction*> itAction(m_toolBars[tb]);
        while (itAction.hasNext())
        {
            QString id;
            if (QAction* action = itAction.next())
            {
                xmlwriter->writeStartElement(A_XML_TAGNAME_RIBBON_ACTION);
                emit ActionToNameId(action, id);
                if (id.isEmpty())
                {
                    id = m_allActionsToName.value(action);
                }

                if (id.isEmpty())
                {
                    qWarning("AToolBarCustomizeEngine::saveStateQuickAccessBar(): 'objectName' not set for QAction "
                             "%p '%s', using 'text' instead",
                             action, action->text().toLocal8Bit().constData());
                    id = action->text();
                }
                xmlwriter->writeAttribute(A_XML_ATTRNAME_RIBBON_ID, id);
                xmlwriter->writeEndElement();
            }
            else
            {
                xmlwriter->writeStartElement(A_XML_TAGNAME_RIBBON_ACTION);
                xmlwriter->writeAttribute(A_XML_ATTRNAME_RIBBON_ID, id);
                xmlwriter->writeEndElement();
            }
        }
        ++itToolBar;
        xmlwriter->writeEndElement();
    }
    xmlwriter->writeEndElement();
}

bool AToolBarCustomizeEngine::restoreStateQuickAccessBar(IXmlStreamReader* xmlreader)
{
    if (!xmlreader->readNextStartElement())
        return false;

    if (xmlreader->name() != A_XML_TAGNAME_RIBBON_QABAR)
        return false;

    while (xmlreader->readNextStartElement())
    {
        if (xmlreader->name() != A_XML_TAGNAME_RIBBON_QABARITEM)
            return false;

        QXmlStreamAttributes attrs = xmlreader->attributes();
        if (!attrs.hasAttribute(A_XML_ATTRNAME_RIBBON_TITLE))
            return false;
        QString title = attrs.value(A_XML_ATTRNAME_RIBBON_TITLE).toString();

        QList<QAction*> actions;
        while (xmlreader->readNextStartElement())
        {
            if (xmlreader->name() != A_XML_TAGNAME_RIBBON_ACTION)
                return false;

            QXmlStreamAttributes attrs = xmlreader->attributes();

            if (!attrs.hasAttribute(A_XML_ATTRNAME_RIBBON_ID))
                return false;

            QString id = attrs.value(A_XML_ATTRNAME_RIBBON_ID).toString();

            if (id.isEmpty())
                actions.append(0);

            QAction* pAction = nullptr;
            emit NameIdToAction(id, &pAction);

            if (pAction == nullptr)
                pAction = m_allNameToActions.value(id);
            if (pAction != nullptr)
                actions.append(pAction);

            xmlreader->skipCurrentElement();
        }

        if (QToolBar* toolBar = findDefaultToolBar(title))
            setToolBar(toolBar, actions);
    }
    return true;
}

void AToolBarCustomizeEngine::saveState(IXmlStreamWriter* xmlwriter)
{
    saveStateQuickAccessBar(xmlwriter);
}

bool AToolBarCustomizeEngine::restoreState(IXmlStreamReader* xmlreader)
{
    return restoreStateQuickAccessBar(xmlreader);
}

/* ARibbonBarCustomizeEngine */
ARibbonBarCustomizeEngine::ARibbonBarCustomizeEngine(QObject* parent, ARibbonBar* ribbonBar)
    : AToolBarCustomizeEngine(parent)
    , m_ribbonBar(ribbonBar)
{
}

ARibbonBarCustomizeEngine::~ARibbonBarCustomizeEngine()
{
}

QString ARibbonBarCustomizeEngine::pageId(ARibbonPage* page) const
{
    for (QMap< QString, ARibbonPage* >::const_iterator it = m_allNameIdToPages.constBegin(); it != m_allNameIdToPages.constEnd(); ++it)
    {
        if (it.value() == page)
            return it.key();
    }
    return QString();
}

void ARibbonBarCustomizeEngine::setPageId(ARibbonPage* page, const QString& id)
{
    QString strPageName = id;

    if (page == nullptr)
        return;

    if (strPageName.isEmpty())
    {
        //        qWarning("ARibbonBarCustomizeEngine::setPageId(): 'pageName' not set for ARibbonPage "
        //            "%p '%s', using 'text' instead", page, page->title().toLocal8Bit().constData());

        strPageName = page->title();

        if (!strPageName.isEmpty())
            strPageName += QLatin1Char('_');
        strPageName += ACustomizeEngineAbstract::generateUniqueNameIdentifierPage();
    }

    if (strPageName.isEmpty())
    {
        Q_ASSERT(false);
        return;
    }

    if (m_allNameIdToPages.contains(strPageName))
        return;
    m_allNameIdToPages.insert(strPageName, page);
}

QString ARibbonBarCustomizeEngine::groupId(ARibbonGroup* group) const
{
    for (QMap< QString, ARibbonGroup* >::const_iterator it = m_allNameIdToGroups.constBegin(); it != m_allNameIdToGroups.constEnd(); ++it)
    {
        if (it.value() == group)
            return it.key();
    }
    return QString();
}

void ARibbonBarCustomizeEngine::setGroupId(ARibbonPage* page, ARibbonGroup* group, const QString& id)
{
    QString strGroupName = id;

    if (page == nullptr || group == nullptr)
        return;

    if (strGroupName.isEmpty())
    {
        strGroupName = page->title() + QStringLiteral(":") + group->title();
    }

    if (strGroupName.isEmpty())
    {
        Q_ASSERT(false);
        return;
    }

    if (m_allNameIdToGroups.contains(strGroupName))
        return;

    m_allNameIdToGroups.insert(strGroupName, group);
}

void ARibbonBarCustomizeEngine::addToCategory(const QString& category, QAction* act)
{
    ACustomizeEngineAbstract::addToCategory(category, act);
}

void ARibbonBarCustomizeEngine::removeFromCategory(const QString& category, QAction* action)
{
    ACustomizeEngineAbstract::removeFromCategory(category, action);
}

QList<ARibbonPage*> ARibbonBarCustomizeEngine::pagesByCategory(const QString& category) const
{
    QList< ARibbonPage*> result;
    for (QMap< ARibbonPage*, QString>::const_iterator it = m_pageToCategory.constBegin(); it != m_pageToCategory.constEnd(); ++it)
    {
        if (it.value() == category)
            result.append(it.key());
    }
    return result;
}

void ARibbonBarCustomizeEngine::addToCategory(const QString& category, ARibbonPage* page)
{
    if (category.isEmpty() || page == nullptr)
        return;

    if (m_pageToCategory.contains(page))
        return;

    m_pageToCategory[page] = category;

    if (!m_lstCategory.contains(category))
        m_lstCategory.append(category);
}

QList< ARibbonGroup* > ARibbonBarCustomizeEngine::groupsByCategory(const QString& category) const
{
    QList< ARibbonGroup*> result;
    for (QMap< ARibbonGroup*, QString>::const_iterator it = m_groupToCategory.constBegin(); it != m_groupToCategory.constEnd(); ++it)
    {
        if (it.value() == category)
            result.append(it.key());
    }
    return result;
}

void ARibbonBarCustomizeEngine::addToCategory(const QString& category, ARibbonGroup* group)
{
    if (category.isEmpty() || group == nullptr)
        return;

    QString strCategory = category;
    strCategory.remove(QLatin1Char('&'));

    if (m_groupToCategory.contains(group))
        return;

    m_groupToCategory[group] = strCategory;

    QList<QAction*> actions = group->actions();
    QListIterator<QAction*> itActions(actions);
    while (itActions.hasNext())
    {
        QAction* action = itActions.next();
        if (!action->text().isEmpty())
        {
            setActionId(action);
            addToCategory(strCategory, action);
        }
    }

    for (int index = 0, count = group->controlCount(); count > index; ++index)
    {
        RibbonControl* control = group->controlByIndex(index);
        Q_ASSERT(control != nullptr);
        if (RibbonToolBarControl* toolBarControl = qobject_cast<RibbonToolBarControl*>(control))
        {
            QList<QAction*> actions = toolBarControl->actions();
            QListIterator<QAction*> itActions(actions);
            while (itActions.hasNext())
            {
                QAction* action = itActions.next();
                if (!action->text().isEmpty())
                {
                    setActionId(action);
                    addToCategory(strCategory, action);
                }
            }
        }
    }
}

QList<ARibbonGroup*> ARibbonBarCustomizeEngine::pageDefaultGroups(ARibbonPage* page) const
{
    return m_defaultPageGroups.value(page);
}

const QMap<ARibbonPage*, QString>& ARibbonBarCustomizeEngine::defaultPageNames() const
{
    return m_defaultPagesName;
}

QString ARibbonBarCustomizeEngine::defaultPageName(ARibbonPage* page) const
{
    return m_defaultPagesName.value(page);
}

QList<ARibbonPage*> ARibbonBarCustomizeEngine::defaultHiddenPages() const
{
    return m_defaultHiddenPages;
}

QList< ARibbonPage* > ARibbonBarCustomizeEngine::editHiddenPages() const
{
    return m_regularHiddenPages;
}

QList<ARibbonGroup*> ARibbonBarCustomizeEngine::regularGroups(ARibbonPage* page) const
{
    if (m_regularPageGroups.contains(page))
        return m_regularPageGroups.value(page);
    return QList<ARibbonGroup*>();
}

const QMap< ARibbonPage*, QList<ARibbonGroup*> >& ARibbonBarCustomizeEngine::defaultPageGroups() const
{
    return m_defaultPageGroups;
}

const QList<ARibbonPage*>& ARibbonBarCustomizeEngine::defaultPages() const
{
    return m_defaultPages;
}

const QMap<ARibbonGroup*, QString>& ARibbonBarCustomizeEngine::defaultGroupNames() const
{
    return m_defaultNameGroups;
}

QString ARibbonBarCustomizeEngine::defaultGroupName(ARibbonGroup* group) const
{
    return m_defaultNameGroups.value(group);
}

void ARibbonBarCustomizeEngine::addDefaultPages()
{
    QList< ARibbonPage*> newPages;
    QList< ARibbonPage*> pages = m_ribbonBar->pages();
    QListIterator<ARibbonPage*> itPages(pages);

    while (itPages.hasNext())
    {
        ARibbonPage* page = itPages.next();
        //        if (page->objectName() != QStringLiteral("__aproch_Page_Invisible"))
        {
            setPageId(page);
            newPages.append(page);

            if (ARibbonPagePrivate::_get(page)->m_associativeTab->isHidden())
                m_defaultHiddenPages.append(page);
        }
        m_defaultPagesName.insert(page, page->title());
        addDefaultGroups(page);
    }
    m_defaultPages.append(newPages);
}

void ARibbonBarCustomizeEngine::addDefaultGroups(ARibbonPage* page)
{
    if (!page)
        return;

    if (m_defaultPageGroups.contains(page))
        return;

    QList< ARibbonGroup* > newGroups;
    QList< ARibbonGroup* > groups = page->groups();
    QListIterator<ARibbonGroup*> itGroups(groups);

    while (itGroups.hasNext())
    {
        ARibbonGroup* group = itGroups.next();
        //        if (group->objectName() != QStringLiteral("__aproch_Group_Invisible") )
        {
            setGroupId(page, group);
            newGroups.append(group);
        }
        m_defaultNameGroups.insert(group, group->title());
    }
    m_defaultPageGroups.insert(page, newGroups);
}

void ARibbonBarCustomizeEngine::setQuickAccessBar()
{
    if (RibbonQuickAccessBar* quickAccessBar = m_ribbonBar->quickAccessBar())
        setToolBar(quickAccessBar, m_regularToolBars.value(quickAccessBar));
}

void ARibbonBarCustomizeEngine::setRibbonBar()
{
    if (!m_ribbonBar)
        return;

    const int curIndex = m_ribbonBar->currentPageIndex();
    QList<ARibbonPage*> currentListPage = m_ribbonBar->pages();
    if (m_regularPages.isEmpty())
    {
        QListIterator<ARibbonPage*> itRegularPage(currentListPage);
        while (itRegularPage.hasNext())
        {
            ARibbonPage* currentPage = itRegularPage.next();
            if (m_defaultPagesName.contains(currentPage))
            {
                QString strTitle = m_defaultPagesName.value(currentPage);
                if (!strTitle.isEmpty())
                    currentPage->setTitle(strTitle);
            }
            currentPage->setVisible(!m_defaultHiddenPages.contains(currentPage));
            if (!m_regularPages.contains(currentPage))
            {
                m_ribbonBar->detachPage(currentPage);
                delete currentPage;
            }
        }
    }
    else
    {
        QListIterator<ARibbonPage*> itRegularPage(currentListPage);
        while (itRegularPage.hasNext())
        {
            ARibbonPage* currentPage = itRegularPage.next();
            if (!m_regularPages.contains(currentPage))
            {
                if (currentPage->property(__aproch_Widget_Custom).toString() == QStringLiteral("__aproch_Page_Custom"))
                    m_ribbonBar->removePage(currentPage);
            }
        }
    }

    QList<ARibbonPage*> newListPage;
    QListIterator<ARibbonPage*> itRegularPage(m_regularPages);
    while (itRegularPage.hasNext())
    {
        ARibbonPage* regularPage = itRegularPage.next();
        newListPage.append(regularPage);
        if (m_regularPagesName.contains(regularPage))
        {
            QString strTitle = m_regularPagesName.value(regularPage);
            if (!strTitle.isEmpty())
                regularPage->setTitle(strTitle);
        }
        m_ribbonBar->detachPage(regularPage);
    }

    QListIterator<ARibbonPage*> itNewPages(newListPage);
    while (itNewPages.hasNext())
    {
        ARibbonPage* page = itNewPages.next();
        m_ribbonBar->addPage(page);
        if (m_regularHiddenPages.contains(page))
            page->setVisible(false);
        else
            page->setVisible(true);
    }

    ARibbonPage* currentPage = nullptr;
    if (curIndex != -1)
        currentPage = m_ribbonBar->page(curIndex);

    if (currentPage != nullptr && currentPage->isVisible())
        m_ribbonBar->setCurrentPageIndex(curIndex);
}

void ARibbonBarCustomizeEngine::setRibbonGroups()
{
    for (QMap< ARibbonPage*, QList<ARibbonGroup*> >::ConstIterator itCurPages = m_regularPageGroups.begin(); itCurPages != m_regularPageGroups.end(); ++itCurPages)
    {
        ARibbonPage* page = itCurPages.key();
        Q_ASSERT(page != nullptr);

        QList< ARibbonGroup* > groupsPage = itCurPages.value();
        QList< ARibbonGroup* > realGroups = page->groups();
        if (groupsPage != realGroups || m_regularNameGroups.size() > 0 || m_regularGroupsActions.size() > 0)
        {
            QList< ARibbonGroup* > newGroups;
            QListIterator< ARibbonGroup* > itGroups(groupsPage);
            while (itGroups.hasNext())
            {
                ARibbonGroup* group = itGroups.next();

                if (group && (!newGroups.contains(group)))
                    newGroups.append(group);

                if (group)
                {
                    //                    if (!realGroups.contains(group) && !m_allGroupsToNameId.keys().contains(group) )
                    //                        setGroupId(group);

                    QListIterator<ARibbonGroup*> itRegularGroups(realGroups);
                    while (itRegularGroups.hasNext())
                    {
                        ARibbonGroup* regularGroup = itRegularGroups.next();
                        if (!groupsPage.contains(regularGroup) && (regularGroup->property(__aproch_Widget_Custom).toString() == QStringLiteral("__aproch_Group_Custom") ||
                            regularGroup->property(__aproch_Widget_Copy).toString() == QStringLiteral("__aproch_Group_Copy")))
                        {
                            realGroups.removeOne(regularGroup);
                            page->removeGroup(regularGroup);
                        }
                    }
                }
            }

            QListIterator< ARibbonGroup* > itOldGroup(realGroups);
            while (itOldGroup.hasNext())
                page->detachGroup(itOldGroup.next());

            QListIterator< ARibbonGroup* > itNewGroups(newGroups);
            while (itNewGroups.hasNext())
            {
                ARibbonGroup* group = itNewGroups.next();
                page->addGroup(group);
                group->setVisible(true);

                if (m_regularNameGroups.contains(group))
                    group->setTitle(m_regularNameGroups.value(group));

                if (group->property(__aproch_Widget_Custom).toString() == QStringLiteral("__aproch_Group_Custom"))
                {
                    group->clear();
                    QList< QAction* > actions = m_regularGroupsActions[group];
                    QListIterator< QAction* > itActions(actions);
                    while (itActions.hasNext())
                        group->addAction(itActions.next());
                }
            }
        }
    }
}

void ARibbonBarCustomizeEngine::updateRibbonBar()
{
    int currentIndex = m_ribbonBar->currentPageIndex();
    if (currentIndex != -1)
    {
        RibbonBarAutoUpdater autoUpdater(m_ribbonBar);
        if (ARibbonPage* page = m_ribbonBar->page(currentIndex))
        {
#if 0
            if (page->isVisible())
            {
                page->updateLayout();
                return;
            }
#endif
            RibbonBarAutoUpdater autoUpdater(m_ribbonBar);
            if (!m_regularHiddenPages.contains(page))
                return;
        }
    }

    int index = 0;
    const QList<ARibbonPage*>& listPage = m_ribbonBar->pages();
    for (QList<ARibbonPage*>::const_iterator it = listPage.constBegin(); it != listPage.constBegin(); ++it)
    {
        ARibbonPage* page = (*it);
        if (page->isVisible() || (!m_regularHiddenPages.isEmpty() && !m_regularHiddenPages.contains(page)))
        {
            m_ribbonBar->setCurrentPageIndex(index);
            break;
        }
        index++;
    }
}

void ARibbonBarCustomizeEngine::deleteFreeGroups()
{
    for (QMap< ARibbonPage*, QList<ARibbonGroup*> >::ConstIterator itPages = m_regularPageGroups.begin(); itPages != m_regularPageGroups.end(); ++itPages)
    {
        ARibbonPage* page = itPages.key();
        QList<ARibbonGroup*> listGroup = m_regularPageGroups.value(page);
        for (int i = listGroup.count() - 1; i >= 0; --i)
        {
            ARibbonGroup* group = listGroup.at(i);
            if (group && group->parent() == nullptr)
            {
                m_regularPageGroups[page].removeOne(group);
                delete group;
            }
        }
    }
}

void ARibbonBarCustomizeEngine::deleteFreePages()
{
    QListIterator< ARibbonPage* > itPages(m_regularPages);
    while (itPages.hasNext())
    {
        ARibbonPage* page = itPages.next();
        if (page && page->parent() == nullptr)
            delete page;
    }
}

void ARibbonBarCustomizeEngine::saveStateGroup(IXmlStreamWriter* xmlwriter, ARibbonPage* page)
{
    Q_ASSERT(page != NULL);
    if (page == NULL)
        return;

    int nNewGroupId = 0;

    QList< ARibbonGroup* > groups = page->groups();
    QListIterator< ARibbonGroup* > itGroup(groups);
    while (itGroup.hasNext())
    {
        if (ARibbonGroup* group = itGroup.next())
        {
            xmlwriter->writeStartElement(A_XML_TAGNAME_RIBBON_GROUP);

            bool custom = group->property(__aproch_Widget_Custom).toString() == QStringLiteral("__aproch_Group_Custom");
            bool copy = group->property(__aproch_Widget_Copy).toString() == QStringLiteral("__aproch_Group_Copy");

            QString nameId = groupId(group);

            if (nameId.isEmpty())
            {
                if (!(custom || copy))
                {
                    qWarning("ARibbonBarCustomizeEngine::saveStateGroup(): 'objectName' not set for ARibbonGroup "
                             "%p '%s', using 'text' instead", group, group->title().toLocal8Bit().constData());
                }
                nameId = QStringLiteral("新建：") + page->title() + QStringLiteral(":") + QString::number(nNewGroupId);
                nNewGroupId++;
            }

            xmlwriter->writeAttribute(A_XML_ATTRNAME_RIBBON_ID, nameId);
            xmlwriter->writeAttribute(A_XML_ATTRNAME_RIBBON_TITLE, group->title());

            if (custom)
                xmlwriter->writeAttribute(A_XML_ATTRNAME_RIBBON_TYPE, QStringLiteral("CustomGroup"));
            else if (copy)
                xmlwriter->writeAttribute(A_XML_ATTRNAME_RIBBON_TYPE, QStringLiteral("CopyGroup"));

            if (custom || copy)
            {
                QList< QAction* > actions = group->actions();
                QListIterator< QAction* > itActions(actions);
                while (itActions.hasNext())
                {
                    QAction* pAction = itActions.next();
                    QString nameAction;
                    emit ActionToNameId(pAction, nameAction);
                    if (nameAction.isEmpty())
                    {
                        nameAction = m_allActionsToName.value(pAction);
                    }

                    if (!nameAction.isEmpty())
                    {
                        xmlwriter->writeStartElement(A_XML_TAGNAME_RIBBON_ACTION);
                        xmlwriter->writeAttribute(A_XML_ATTRNAME_RIBBON_ID, nameAction);
                        xmlwriter->writeEndElement();
                    }
                }
            }
            xmlwriter->writeEndElement();
        }
    }
}

bool ARibbonBarCustomizeEngine::restoreStateGroup(IXmlStreamReader* xmlreader, ARibbonPage* page)
{

    QList< ARibbonGroup* > groups;
    QMap< ARibbonGroup*, QList<QAction*> > groupActions;

    while (xmlreader->readNextStartElement())
    {
        if (xmlreader->name() != A_XML_TAGNAME_RIBBON_GROUP)
            return false;

        QXmlStreamAttributes attrs = xmlreader->attributes();
        if (!attrs.hasAttribute(A_XML_ATTRNAME_RIBBON_ID))
            return false;

        QString nameId = attrs.value(A_XML_ATTRNAME_RIBBON_ID).toString();

        if (!attrs.hasAttribute(A_XML_ATTRNAME_RIBBON_TITLE))
            return false;

        QString title = attrs.value(A_XML_ATTRNAME_RIBBON_TITLE).toString();

        bool customGroup = false;
        bool copyGroup = false;
        QString strType = attrs.value(A_XML_ATTRNAME_RIBBON_TYPE).toString();
        if (!strType.isEmpty())
        {
            if (strType == QStringLiteral("CustomGroup"))
                customGroup = true;
            else if (strType == QStringLiteral("CopyGroup"))
                copyGroup = true;
        }

        if (nameId.isEmpty())
        {
            groups.append(0);
            xmlreader->readNextStartElement();
        }
        else if (!(customGroup || copyGroup))
        {
            ARibbonGroup* group = m_allNameIdToGroups.value(nameId);

            QList<ARibbonGroup*> listGroup = m_allNameIdToGroups.values();
            for (QList< ARibbonGroup* >::iterator it = listGroup.begin(); it != listGroup.end() && group == nullptr; ++it)
            {
                if ((*it)->title() == nameId || (*it)->title() == title)
                    group = (*it);
            }

            if (group)
            {
                groups.append(group);
                group->setTitle(title);
            }
            xmlreader->readNextStartElement();
        }
        else
        {
            ARibbonGroup* newGroup = new ARibbonGroup(NULL);
            //            newGroup->setObjectName(nameId);
            newGroup->setTitle(title);
            groups.append(newGroup);

            if (customGroup)
                newGroup->setProperty(__aproch_Widget_Custom, QStringLiteral("__aproch_Group_Custom"));
            else
                newGroup->setProperty(__aproch_Widget_Copy, QStringLiteral("__aproch_Group_Copy"));

            QList<QAction*> actions;

            while (xmlreader->readNextStartElement())
            {
                if (xmlreader->name() != A_XML_TAGNAME_RIBBON_ACTION)
                    return false;
                QXmlStreamAttributes attrsAction = xmlreader->attributes();
                QString idName = attrsAction.value(A_XML_ATTRNAME_RIBBON_ID).toString();
                if (!idName.isEmpty())
                {
                    QAction* pAction = nullptr;
                    emit NameIdToAction(idName, &pAction);

                    if (pAction == nullptr)
                    {
                        pAction = m_allNameToActions.value(idName);
                    }

                    if (pAction != nullptr)
                    {
                        newGroup->addAction(pAction);
                        actions.append(pAction);
                    }
                }
                xmlreader->readNextStartElement();
            }
            groupActions.insert(newGroup, actions);
        }
    }

    if (m_ribbonBar != nullptr)
    {
        m_regularPageGroups.insert(page, groups);
        m_regularGroupsActions = groupActions;
        setRibbonGroups();
        m_regularPageGroups.clear();
        m_regularGroupsActions.clear();
    }
    return true;
}

void ARibbonBarCustomizeEngine::saveStatePage(IXmlStreamWriter* xmlwriter)
{
    QListIterator< ARibbonPage* > itPage(m_ribbonBar->pages());
    while (itPage.hasNext())
    {
        if (ARibbonPage* page = itPage.next())
        {
            xmlwriter->writeStartElement(A_XML_TAGNAME_RIBBON_PAGE);

            QString nameId = pageId(page);

            if (nameId.isEmpty())
            {
                if (page->property(__aproch_Widget_Custom).toString() != QStringLiteral("__aproch_Page_Custom"))
                {
                    qWarning("ARibbonBarCustomizeEngine::saveState(): 'pageId' not set for ARibbonPage "
                             "%p '%s', using 'text' instead",
                             page, page->title().toLocal8Bit().constData());
                }
                nameId = page->title();
            }

            xmlwriter->writeAttribute(A_XML_ATTRNAME_RIBBON_ID, nameId);
            xmlwriter->writeAttribute(A_XML_ATTRNAME_RIBBON_TITLE, page->title());
            xmlwriter->writeAttribute(A_XML_ATTRNAME_RIBBON_VISIBLE, QVariant(page->isVisible()).toString());

            if (page->property(__aproch_Widget_Custom).toString() == QStringLiteral("__aproch_Page_Custom"))
                xmlwriter->writeAttribute(A_XML_ATTRNAME_RIBBON_TYPE, QStringLiteral("CustomPage"));

            saveStateGroup(xmlwriter, page);

            xmlwriter->writeEndElement();
        }
    }
}

bool ARibbonBarCustomizeEngine::restoreStatePage(IXmlStreamReader* xmlreader)
{
    RibbonBarAutoUpdater autoUpdater(m_ribbonBar);
    bool ok = true;
    m_regularHiddenPages.clear();

    while (xmlreader->readNextStartElement())
    {
        if (xmlreader->name() != A_XML_TAGNAME_RIBBON_PAGE)
            return false;

        QXmlStreamAttributes attrs = xmlreader->attributes();
        if (!attrs.hasAttribute(A_XML_ATTRNAME_RIBBON_ID))
            return false;

        QString nameId = attrs.value(A_XML_ATTRNAME_RIBBON_ID).toString();

        if (!attrs.hasAttribute(A_XML_ATTRNAME_RIBBON_TITLE))
            return false;

        QString title = attrs.value(A_XML_ATTRNAME_RIBBON_TITLE).toString();

        if (!attrs.hasAttribute(A_XML_ATTRNAME_RIBBON_VISIBLE))
            return false;

        bool visible = QVariant(attrs.value(A_XML_ATTRNAME_RIBBON_VISIBLE).toString()).toBool();

        QString strType = attrs.value(A_XML_ATTRNAME_RIBBON_TYPE).toString();
        bool customPage = !strType.isEmpty() && strType == QStringLiteral("CustomPage");


        if (nameId.isEmpty())
        {
            m_regularPages.append(0);
            xmlreader->readNextStartElement();
        }
        else if (!customPage)
        {
            ARibbonPage* page = m_allNameIdToPages.value(nameId);

            QList<ARibbonPage*> listPage = m_allNameIdToPages.values();
            for (QList< ARibbonPage* >::iterator it = listPage.begin(); it != listPage.end() && page == nullptr; ++it)
            {
                if ((*it)->title() == nameId || (*it)->title() == title)
                    page = (*it);
            }

            if (page != nullptr)
            {
                m_regularPages.append(page);

                if (!visible)
                    m_regularHiddenPages.append(page);
                page->setTitle(title);
                restoreStateGroup(xmlreader, page);
            }
            else
                xmlreader->readNextStartElement();
        }
        else if (customPage)
        {
            ARibbonPage* newPage = new ARibbonPage();
            newPage->setTitle(title);
            newPage->setObjectName(nameId);
            m_regularPages.append(newPage);
            newPage->setProperty(__aproch_Widget_Custom, QStringLiteral("__aproch_Page_Custom"));

            if (!visible)
                m_regularHiddenPages.append(newPage);
            restoreStateGroup(xmlreader, newPage);
        }
    }


    ((ARibbonBarCustomizeEngine*)this)->setRibbonBar();
    updateRibbonBar();
    m_regularPages.clear();
    m_regularHiddenPages.clear();
    return ok;
}

void ARibbonBarCustomizeEngine::saveStateRibbonBar(IXmlStreamWriter* xmlwriter)
{
    xmlwriter->writeStartElement(A_XML_TAGNAME_RIBBON_BAR);
    saveStatePage(xmlwriter);
    xmlwriter->writeEndElement();
}

bool ARibbonBarCustomizeEngine::restoreStateRibbonBar(IXmlStreamReader* xmlreader)
{
    if (!xmlreader->readNextStartElement())
        return false;

    if (xmlreader->name() != A_XML_TAGNAME_RIBBON_BAR)
        return false;

    bool ok = restoreStatePage(xmlreader);
    Q_ASSERT(ok);
    return ok;
}

void ARibbonBarCustomizeEngine::saveState(IXmlStreamWriter* xmlwriter)
{
    AToolBarCustomizeEngine::saveState(xmlwriter);
    saveStateRibbonBar(xmlwriter);
}

bool ARibbonBarCustomizeEngine::restoreState(IXmlStreamReader* xmlreader)
{

    RibbonBarAutoUpdater autoUpdater(m_ribbonBar);
    if (!AToolBarCustomizeEngine::restoreState(xmlreader))
    {
        Q_ASSERT(false);
        return false;
    }
    if (!restoreStateRibbonBar(xmlreader))
    {
        Q_ASSERT(false);
        return false;
    }
    return true;
}


/* ARibbonCustomizeManagerPrivate */
ARibbonCustomizeManagerPrivate::ARibbonCustomizeManagerPrivate()
{
    m_editMode = false;
    m_customizeEngine = nullptr;
    m_ribbonBar = nullptr;
}

ARibbonCustomizeManagerPrivate::~ARibbonCustomizeManagerPrivate()
{
}

void ARibbonCustomizeManagerPrivate::init(ARibbonBar* ribbonBar)
{
    A_P(ARibbonCustomizeManager)
        m_ribbonBar = ribbonBar;
    m_customizeEngine = new ARibbonBarCustomizeEngine(&p, ribbonBar);
}

void ARibbonCustomizeManagerPrivate::enableRestoreVisualizePage(bool bEnable)
{
    _bRestoreVisualizePage = bEnable;
}

void ARibbonCustomizeManagerPrivate::storePageVisual()
{
    if (m_ribbonBar == nullptr)
        return;

    QList<ARibbonPage*> pages = m_ribbonBar->pages();
    for (auto pPage : pages)
    {
        if (pPage == nullptr)
            continue;

        _pagesVisual[pPage->title()] = pPage->isVisible();
    }
}

void ARibbonCustomizeManagerPrivate::restorePageVisual()
{
    if (m_ribbonBar == nullptr)
        return;

    QList<ARibbonPage*> pages = m_ribbonBar->pages();
    for (auto pPage : pages)
    {
        if (pPage == nullptr)
            continue;

        auto iter = _pagesVisual.find(pPage->title());

        if (iter == _pagesVisual.end())
        {
            pPage->setVisible(true);
            continue;
        }

        pPage->setVisible(iter.value());
    }
}

/*!
\class ARibbonCustomizeManager
\inmodule QtitanRibbon
\brief ARibbonCustomizeManager is a base class to customize actions on the ARibbonBar.
*/
ARibbonCustomizeManager::ARibbonCustomizeManager(ARibbonBar* ribbonBar)
    : QObject(ribbonBar)
{
    A_INIT_PRIVATE(ARibbonCustomizeManager);
    A_D(ARibbonCustomizeManager);
    d.init(ribbonBar);
    connect(d.m_customizeEngine, SIGNAL(ActionToNameId(const QAction*, QString&)), this, SIGNAL(ActionToNameId(const QAction*, QString&)));
    connect(d.m_customizeEngine, SIGNAL(NameIdToAction(const QString&, QAction**)), this, SIGNAL(NameIdToAction(const QString&, QAction**)));
}

/*!
Destructor.
*/
ARibbonCustomizeManager::~ARibbonCustomizeManager()
{
    A_DELETE_PRIVATE();
}

/*!
Returns list of all categories.
*/
QStringList ARibbonCustomizeManager::categories() const
{
    A_D(const ARibbonCustomizeManager);
    return d.m_customizeEngine->categories();
}

/*!
Returns a list of all actions which are subject to customize.
*/
QList<QAction*> ARibbonCustomizeManager::actionsAll() const
{
    A_D(const ARibbonCustomizeManager);
    return d.m_customizeEngine->actionsAll();
}

/*!
Adds QToolBar \a toolBar to customize manager.
*/
void ARibbonCustomizeManager::addToolBar(QToolBar* toolBar)
{
    A_D(ARibbonCustomizeManager);
    d.m_customizeEngine->addDefaultToolBar(toolBar);
}

/*!
Adds new \a category to customize manager.
*/
void ARibbonCustomizeManager::addAllActionsCategory(const QString& category)
{
    A_D(ARibbonCustomizeManager);
    d.m_customizeEngine->addAllActionsCategory(category);
}

/*!
Returns all actions from the specific \a category.
*/
QList<QAction*> ARibbonCustomizeManager::actionsByCategory(const QString& category) const
{
    A_D(const ARibbonCustomizeManager);
    return d.m_customizeEngine->actionsByCategory(category);
}

/*!
Adds action to the specific \a category.
*/
void ARibbonCustomizeManager::addToCategory(const QString& category, QAction* action)
{
    A_D(ARibbonCustomizeManager);
    d.m_customizeEngine->addToCategory(category, action);
}

/*!
Romoves action from the specific \a category.
*/
void ARibbonCustomizeManager::removeFromCategory(const QString& category, QAction* action)
{
    A_D(ARibbonCustomizeManager);
    d.m_customizeEngine->removeFromCategory(category, action);
}

/*!
Returns all toolbars from the specific \a category.
*/
QList<QToolBar*> ARibbonCustomizeManager::toolBarsByCategory(const QString& category) const
{
    A_D(const ARibbonCustomizeManager);
    return d.m_customizeEngine->toolBarsByCategory(category);
}

/*!
Adds \a toolBar to the specific \a category.
*/
void ARibbonCustomizeManager::addToCategory(const QString& category, QToolBar* toolBar)
{
    A_D(ARibbonCustomizeManager);
    d.m_customizeEngine->AToolBarCustomizeEngine::addToCategory(category, toolBar);
}

/*!
Returns all ribbon pages from the specific \a category.
*/
QList< ARibbonPage* > ARibbonCustomizeManager::pagesByCategory(const QString& category) const
{
    A_D(const ARibbonCustomizeManager);
    return d.m_customizeEngine->pagesByCategory(category);
}

/*!
Adds ribbon \a page to the specific \a category.
*/
void ARibbonCustomizeManager::addToCategory(const QString& category, ARibbonPage* page)
{
    A_D(ARibbonCustomizeManager);
    return d.m_customizeEngine->addToCategory(category, page);
}

/*!
Returns all ribbon groups from the specific \a category.
*/
QList< ARibbonGroup* > ARibbonCustomizeManager::groupsByCategory(const QString& category) const
{
    A_D(const ARibbonCustomizeManager);
    return d.m_customizeEngine->groupsByCategory(category);
}

/*!
Adds ribbon \a group to the specific \a category.
*/
void ARibbonCustomizeManager::addToCategory(const QString& category, ARibbonGroup* group)
{
    A_D(ARibbonCustomizeManager);
    d.m_customizeEngine->addToCategory(category, group);
}

/*!
Returns unique identifier for \a action.
*/
QString ARibbonCustomizeManager::actionId(QAction* action) const
{
    A_D(const ARibbonCustomizeManager);
    return d.m_customizeEngine->actionId(action);
}

/*!
Assigns the unique identifier \a id for \a action.
*/
void ARibbonCustomizeManager::setActionId(QAction* action, const QString& id)
{
    A_D(ARibbonCustomizeManager);
    d.m_customizeEngine->setActionId(action, id);
}

/*!
Returns unique identifier for \a page.
*/
QString ARibbonCustomizeManager::pageId(ARibbonPage* page) const
{
    A_D(const ARibbonCustomizeManager);
    return d.m_customizeEngine->pageId(page);
}

/*!
Assign the unique identifier \a id for \a page.
*/
void ARibbonCustomizeManager::setPageId(ARibbonPage* page, const QString& id)
{
    A_D(ARibbonCustomizeManager);
    d.m_customizeEngine->setPageId(page, id);
}

/*!
Returns unique identifier for \a group.
*/
QString ARibbonCustomizeManager::groupId(ARibbonGroup* group) const
{
    A_D(const ARibbonCustomizeManager);
    return d.m_customizeEngine->groupId(group);
}

/*!
Assign the unique identifier \a id for \a group.
*/
void ARibbonCustomizeManager::setGroupId(ARibbonPage* page, ARibbonGroup* group, const QString& id)
{
    A_D(ARibbonCustomizeManager);
    d.m_customizeEngine->setGroupId(page, group, id);
}

/*!
Sets customize manager to the edit mode.
*/
void ARibbonCustomizeManager::setEditMode(bool set)
{
    A_D(ARibbonCustomizeManager);
    if (d.m_editMode == set)
        return;

    d.m_editMode = set;
    if (d.m_editMode)
    {
        d.m_customizeEngine->m_regularToolBars.clear();
        d.m_customizeEngine->m_regularHiddenPages.clear();
        d.m_customizeEngine->m_regularPagesName.clear();
        d.m_customizeEngine->m_regularPages.clear();
        d.m_customizeEngine->m_regularPageGroups.clear();
        d.m_customizeEngine->m_regularGroupsActions.clear();

        if (RibbonQuickAccessBar* toolBar = d.m_customizeEngine->m_ribbonBar->quickAccessBar())
        {
            QList<QAction*> listAction = toolBar->actions();
            for (QList< QAction* >::iterator it = listAction.begin(); it != listAction.end(); ++it)
            {
                if (d.m_customizeEngine->m_allActionsToName.contains(*it))
                    d.m_customizeEngine->m_regularToolBars[toolBar].append(*it);
                else if ((*it)->isSeparator())
                    d.m_customizeEngine->m_regularToolBars[toolBar].append(0);
            }
        }

        d.m_customizeEngine->m_regularPages = d.m_customizeEngine->m_ribbonBar->pages();
        for (QList< ARibbonPage* >::iterator it = d.m_customizeEngine->m_regularPages.begin(); it != d.m_customizeEngine->m_regularPages.end(); ++it)
        {
            ARibbonPage* page = *it;
            setPageName(page, page->title());
            setPageHidden(page, !page->isVisible());

            QList<ARibbonGroup*> listGrops = page->groups();
            d.m_customizeEngine->m_regularPageGroups.insert(page, listGrops);

            for (QList< ARibbonGroup* >::iterator iGroup = listGrops.begin(); iGroup != listGrops.end(); ++iGroup)
            {
                ARibbonGroup* group = *iGroup;
                QList<QAction*> actions = group->actions();
                if (!actions.isEmpty())
                    d.m_customizeEngine->m_regularGroupsActions.insert(group, actions);

                for (int index = 0, count = group->controlCount(); count > index; ++index)
                {
                    RibbonControl* control = group->controlByIndex(index);
                    Q_ASSERT(control != nullptr);
                    if (RibbonToolBarControl* toolBarControl = qobject_cast<RibbonToolBarControl*>(control))
                    {
                        QList<QAction*> actions = toolBarControl->actions();
                        d.m_customizeEngine->m_regularGroupsActions.insert(group, actions);
                    }
                }
            }
        }
    }
    else
        cancel();
}

/*!
Returns true if customize manager in edit mode. Otherwise, returns false.
*/
bool ARibbonCustomizeManager::isEditMode() const
{
    A_D(const ARibbonCustomizeManager);
    return d.m_editMode;
}

/*!
Reset all changes to the default for additional \a toolBar. If \a toolBar is NULL then reset changes for ARibbonBar.
*/
void ARibbonCustomizeManager::reset(QToolBar* toolBar)
{
    A_D(ARibbonCustomizeManager);
    if (toolBar == nullptr)
    {
        d.m_customizeEngine->m_regularHiddenPages = d.m_customizeEngine->defaultHiddenPages();
        d.m_customizeEngine->m_regularPagesName = d.m_customizeEngine->defaultPageNames();
        d.m_customizeEngine->m_regularNameGroups = d.m_customizeEngine->defaultGroupNames();

        d.m_customizeEngine->m_regularPages = d.m_customizeEngine->defaultPages();
        d.m_customizeEngine->m_regularPageGroups = d.m_customizeEngine->defaultPageGroups();

        d.m_customizeEngine->m_regularGroupsActions.clear();

        for (QMap< ARibbonPage*, QList<ARibbonGroup*> >::iterator it = d.m_customizeEngine->m_regularPageGroups.begin(); it != d.m_customizeEngine->m_regularPageGroups.end(); ++it)
        {
            QList< ARibbonGroup* > listGrops = it.value();
            for (QList< ARibbonGroup* >::iterator iGroup = listGrops.begin(); iGroup != listGrops.end(); ++iGroup)
            {
                ARibbonGroup* group = *iGroup;
                QList<QAction*> actions = group->actions();
                if (!actions.isEmpty())
                    d.m_customizeEngine->m_regularGroupsActions.insert(group, actions);

                for (int index = 0, count = group->controlCount(); count > index; ++index)
                {
                    RibbonControl* control = group->controlByIndex(index);
                    Q_ASSERT(control != nullptr);
                    if (RibbonToolBarControl* toolBarControl = qobject_cast<RibbonToolBarControl*>(control))
                    {
                        QList<QAction*> actions = toolBarControl->actions();
                        d.m_customizeEngine->m_regularGroupsActions.insert(group, actions);
                    }
                }
            }
        }
    }
    else
    {
        d.m_customizeEngine->m_regularToolBars.clear();
        QList<QAction*> listAction = d.m_customizeEngine->defaultToolBars().value(toolBar);
        for (QList< QAction* >::iterator it = listAction.begin(); it != listAction.end(); ++it)
        {
            if (d.m_customizeEngine->m_allActionsToName.contains(*it))
                d.m_customizeEngine->m_regularToolBars[toolBar].append(*it);
            else if ((*it)->isSeparator())
                d.m_customizeEngine->m_regularToolBars[toolBar].append(0);
        }
    }
}


/*!
Cancel all changes that were made.
*/
void ARibbonCustomizeManager::cancel()
{
    A_D(ARibbonCustomizeManager);
    d.m_customizeEngine->m_regularToolBars.clear();
    d.m_customizeEngine->m_regularHiddenPages.clear();
    d.m_customizeEngine->m_regularPagesName.clear();
    d.m_customizeEngine->m_regularNameGroups.clear();
    d.m_customizeEngine->deleteFreePages();
    d.m_customizeEngine->m_regularPages.clear();
    d.m_customizeEngine->deleteFreeGroups();
    d.m_customizeEngine->m_regularPageGroups.clear();
    d.m_customizeEngine->m_regularGroupsActions.clear();
}

/*!
Apply all changes to the toolBars.
*/
void ARibbonCustomizeManager::commit()
{
    A_D(ARibbonCustomizeManager);
    if (isEditMode())
    {
        if (d._bRestoreVisualizePage)
            d.storePageVisual();

        RibbonBarAutoUpdater autoUpdater(d.m_ribbonBar);
        d.m_customizeEngine->setQuickAccessBar();
        d.m_customizeEngine->setRibbonBar();
        d.m_customizeEngine->setRibbonGroups();
        d.m_customizeEngine->updateRibbonBar();

        if (d._bRestoreVisualizePage)
            d.restorePageVisual();
    }
}

/*!
Returns all actions from \a toolBar.
*/
QList< QAction* > ARibbonCustomizeManager::actions(QToolBar* toolBar) const
{
    A_D(const ARibbonCustomizeManager);
    if (qobject_cast<RibbonQuickAccessBar*>(toolBar))
        return d.m_customizeEngine->m_regularToolBars.value(toolBar);
    else
        return d.m_customizeEngine->actions(toolBar);
}

/*!
Insert \a action to \a toolBar in the position \a index.
*/
void ARibbonCustomizeManager::insertAction(QToolBar* toolBar, QAction* action, int index)
{
    A_D(ARibbonCustomizeManager);
    if (!isEditMode())
        setEditMode(true);

    d.m_customizeEngine->m_regularToolBars[toolBar].insert(index, action);
}

/*!
Remove an action from \a toolBar in the position \a index.
*/
void ARibbonCustomizeManager::removeActionAt(QToolBar* toolBar, int index)
{
    A_D(ARibbonCustomizeManager);
    if (!isEditMode())
        setEditMode(true);

    d.m_customizeEngine->m_regularToolBars[toolBar].removeAt(index);
}

/*!
Returns true if \a toolBar contains the \a action.
*/
bool ARibbonCustomizeManager::containsAction(QToolBar* toolBar, QAction* action) const
{
    A_D(const ARibbonCustomizeManager);
    return d.m_customizeEngine->m_regularToolBars[toolBar].contains(action);
}

/*!
Returns all pages that were added to customize manager.
*/
QList<ARibbonPage*> ARibbonCustomizeManager::pages() const
{
    A_D(const ARibbonCustomizeManager);
    return d.m_customizeEngine->m_regularPages;
}

/*!
Creates page with name \a pageName and moves it to the position \a index. If \a index is -1 then page will be added to the last position.
*/
ARibbonPage* ARibbonCustomizeManager::createPage(const QString& pageName, int index)
{
    A_D(ARibbonCustomizeManager);

    if (!isEditMode())
        setEditMode(true);

    ARibbonPage* newPage = new ARibbonPage();
    newPage->setTitle(pageName);
    newPage->setProperty(__aproch_Widget_Custom, QStringLiteral("__aproch_Page_Custom"));

    if (index == -1)
        d.m_customizeEngine->m_regularPages.append(newPage);
    else
        d.m_customizeEngine->m_regularPages.insert(index, newPage);
    return newPage;
}

/*!
Deletes the \a page.
*/
void ARibbonCustomizeManager::deletePage(ARibbonPage* page)
{
    A_D(ARibbonCustomizeManager);
    if (!isEditMode())
        setEditMode(true);

    d.m_customizeEngine->deleteFreeGroups();
    d.m_customizeEngine->m_regularPages.removeOne(page);
    d.m_customizeEngine->m_regularPageGroups.remove(page);

    if (page->parentWidget() == nullptr)
        delete page;
}

/*!
Returns an index of the \a page.
*/
int ARibbonCustomizeManager::pageIndex(ARibbonPage* page) const
{
    A_D(const ARibbonCustomizeManager);
    return d.m_customizeEngine->m_regularPages.indexOf(page);
}

/*!
Moves page from position \a oldIndex to \a newIndex.
*/
void ARibbonCustomizeManager::movePage(int oldIndex, int newIndex)
{
    A_D(ARibbonCustomizeManager);

    ARibbonPage* page = d.m_customizeEngine->m_regularPages.at(oldIndex);
    d.m_customizeEngine->m_regularPages.removeAt(oldIndex);
    d.m_customizeEngine->m_regularPages.insert(newIndex, page);
}

/*!
Returns all groups in \a page.
*/
QList<ARibbonGroup*> ARibbonCustomizeManager::pageGroups(ARibbonPage* page) const
{
    A_D(const ARibbonCustomizeManager);
    return d.m_customizeEngine->m_regularPageGroups.value(page);
}

/*!
Creates a new group in \a page with name \a groupName in the position \a index. If \a index is -1 then group will be added to the last position.
*/
ARibbonGroup* ARibbonCustomizeManager::createGroup(ARibbonPage* page, const QString& groupName, int index)
{
    A_D(ARibbonCustomizeManager);
    if (!isEditMode())
        setEditMode(true);

    ARibbonGroup* newGroup = new ARibbonGroup();
    newGroup->setTitle(groupName);

    if (index == -1)
        d.m_customizeEngine->m_regularPageGroups[page].append(newGroup);
    else
        d.m_customizeEngine->m_regularPageGroups[page].insert(index, newGroup);
    return newGroup;
}

/*!
Deletes group by \a index from \a page.
*/
void ARibbonCustomizeManager::deleteGroup(ARibbonPage* page, int index)
{
    A_D(ARibbonCustomizeManager);
    if (!isEditMode())
        setEditMode(true);

    ARibbonGroup* group = d.m_customizeEngine->m_regularPageGroups[page].takeAt(index);
    Q_ASSERT(group != nullptr);

    QList<ARibbonGroup*> lstGroups = d.m_customizeEngine->defaultPageGroups().value(page);
    const bool isExist = lstGroups.contains(group);

    if (!isExist && group->parent() == nullptr)
        delete group;
}

/*!
Moves group from position \a oldIndex to \a newIndex in \a page.
*/
void ARibbonCustomizeManager::moveGroup(ARibbonPage* page, int oldIndex, int newIndex)
{
    A_D(ARibbonCustomizeManager);
    if (!isEditMode())
        setEditMode(true);

    ARibbonGroup* group = d.m_customizeEngine->m_regularPageGroups[page].at(oldIndex);
    d.m_customizeEngine->m_regularPageGroups[page].removeAt(oldIndex);
    d.m_customizeEngine->m_regularPageGroups[page].insert(newIndex, group);
}

/*!
Inserts \a action to the \a group in the position \a index.
*/
void ARibbonCustomizeManager::insertAction(ARibbonGroup* group, QAction* action, int index)
{
    A_D(ARibbonCustomizeManager);
    if (!isEditMode())
        setEditMode(true);
    d.m_customizeEngine->m_regularGroupsActions[group].insert(index, action);
}

/*!
Inserts a list of \a actions to the \a group in last postion.
*/
void ARibbonCustomizeManager::appendActions(ARibbonGroup* group, const QList<QAction*>& actions)
{
    A_D(ARibbonCustomizeManager);
    if (!isEditMode())
        setEditMode(true);
    d.m_customizeEngine->m_regularGroupsActions[group] = actions;
}

/*!
Clear all actions in \a group.
*/
void ARibbonCustomizeManager::clearActions(ARibbonGroup* group)
{
    A_D(ARibbonCustomizeManager);
    if (!isEditMode())
        setEditMode(true);
    d.m_customizeEngine->m_regularGroupsActions.remove(group);
}

/*!
Removes an action from \a group in the position \a index.
*/
void ARibbonCustomizeManager::removeActionAt(ARibbonGroup* group, int index)
{
    A_D(ARibbonCustomizeManager);
    if (!isEditMode())
        setEditMode(true);
    d.m_customizeEngine->m_regularGroupsActions[group].removeAt(index);
}

/*!
Returns true if \a group contains the \a action.
*/
bool ARibbonCustomizeManager::containsAction(ARibbonGroup* group, QAction* action) const
{
    A_D(const ARibbonCustomizeManager);
    return d.m_customizeEngine->m_regularGroupsActions.value(group).contains(action);
}

/*!
Returns all actions from \a group.
*/
QList<QAction*> ARibbonCustomizeManager::actionsGroup(ARibbonGroup* group) const
{
    A_D(const ARibbonCustomizeManager);
    return d.m_customizeEngine->m_regularGroupsActions.value(group);
}

/*!
Returns true if page is hidden.
*/
bool ARibbonCustomizeManager::isPageHidden(ARibbonPage* page) const
{
    A_D(const ARibbonCustomizeManager);
    return d.m_customizeEngine->m_regularHiddenPages.contains(page);
}

/*!
Assigns \a page hiddenness to \a hide.
*/
void ARibbonCustomizeManager::setPageHidden(ARibbonPage* page, bool hide)
{
    if (!isEditMode())
        setEditMode(true);

    A_D(ARibbonCustomizeManager);
    if (hide)
        d.m_customizeEngine->m_regularHiddenPages.append(page);
    else
        d.m_customizeEngine->m_regularHiddenPages.removeOne(page);
}

/*!
Returns true if page is visible.
\sa isPageHidden()
*/
bool ARibbonCustomizeManager::isPageVisible(ARibbonPage* page) const
{
    return !isPageHidden(page);
}

/*!
Assigns \a page visibility to \a visible.
*/
void ARibbonCustomizeManager::setPageVisible(ARibbonPage* page, bool visible)
{
    setPageHidden(page, !visible);
}

/*!
Returns the name of the \a page.
*/
QString ARibbonCustomizeManager::pageName(ARibbonPage* page) const
{
    A_D(const ARibbonCustomizeManager);
    return d.m_customizeEngine->m_regularPagesName.value(page);
}

/*!
Assigns the name \a pageName for the \a page.
*/
void ARibbonCustomizeManager::setPageName(ARibbonPage* page, const QString& pageName)
{
    A_D(const ARibbonCustomizeManager);
    d.m_customizeEngine->m_regularPagesName.insert(page, pageName);
}

/*!
Returns the name of the \a group.
*/
QString ARibbonCustomizeManager::groupName(ARibbonGroup* group) const
{
    A_D(const ARibbonCustomizeManager);
    return d.m_customizeEngine->m_regularNameGroups.value(group);
}

/*!
Assigns the name \a groupName for the \a group.
*/
void ARibbonCustomizeManager::setGroupName(ARibbonGroup* group, const QString& groupName)
{
    A_D(const ARibbonCustomizeManager);
    d.m_customizeEngine->m_regularNameGroups.insert(group, groupName);
}

/*!
Sets the default state of the ribbon bar.
The method must be called immediately after the ribbon bar is initialized in order to fix the state in the customization manager, which state should be used by default.
\sa saveStateToXML, saveStateToDevice, loadStateFromDevice, loadStateFromXML
*/
void ARibbonCustomizeManager::setDefaultState()
{
    A_D(ARibbonCustomizeManager);
    d.m_customizeEngine->addDefaultPages();
    if (RibbonQuickAccessBar* quickAccessBar = d.m_ribbonBar->quickAccessBar())
        d.m_customizeEngine->addDefaultToolBar(quickAccessBar);
}

void ARibbonCustomizeManager::enableModifyPage(bool bEnable)
{
    A_D(ARibbonCustomizeManager);
    if (d.m_ribbonBar == nullptr)
        return;
    RibbonCustomizeDialog* pDialog = d.m_ribbonBar->customizeDialog();
    if (pDialog == nullptr)
        return;

    if (pDialog->pageCount() <= 1)
        return;

    for (int i = 0; i < pDialog->pageCount(); i++)
    {
        auto pRibbonBarCustomizePage = dynamic_cast<RibbonBarCustomizePage*>(pDialog->pageByIndex(i));
        if (pRibbonBarCustomizePage == nullptr)
            continue;

        pRibbonBarCustomizePage->enableModifyPage(bEnable);
    }

    d.enableRestoreVisualizePage(!bEnable);
}

/*!
Save the ribbon bar state to the \a device with given \a autoFormatting.
\sa saveStateToXML, setDefaultState
*/
bool ARibbonCustomizeManager::saveStateToDevice(QIODevice* device, bool autoFormatting)
{
    XmlStreamWriter xmlwriter(device);
    xmlwriter.setAutoFormatting(autoFormatting);
    return saveStateToXML(&xmlwriter);
}

/*!
Save the ribbon bar state to the xml/fastinfoset stream \a xmlwriter.
\sa saveStateToXML, setDefaultState
*/
bool ARibbonCustomizeManager::saveStateToXML(IXmlStreamWriter* xmlwriter)
{
    A_D(const ARibbonCustomizeManager);

    connect(d.m_customizeEngine, SIGNAL(ActionToNameId(const QAction*, QString&)), this, SIGNAL(ActionToNameId(const QAction*, QString&)));

    xmlwriter->writeStartDocument(QStringLiteral("1.0"));
    xmlwriter->writeNamespace(QStringLiteral("https://www.devmachines.com/qt/qtitan"), QStringLiteral("Qtitan"));
    xmlwriter->writeStartElement(A_XML_TAGNAME_RIBBON_STORAGE);
    xmlwriter->writeAttribute(QStringLiteral("type"), QStringLiteral("storage"));
    xmlwriter->writeAttribute(QStringLiteral("version"), QStringLiteral("2.0"));

    d.m_customizeEngine->saveState(xmlwriter);
    xmlwriter->writeEndDocument();

    return true;

}

/*!
Restore the ribbon bar state from the \a device.
\sa loadStateFromXML, setDefaultState
*/
bool ARibbonCustomizeManager::loadStateFromDevice(QIODevice* device)
{
    AXmlStreamReader xmlreader(device);
    return loadStateFromXML(&xmlreader);
}

/*!
Restore the ribbon bar state from the xml/fastinfoset stream \a xmlreader.
\sa loadStateFromDevice, setDefaultState
*/
bool ARibbonCustomizeManager::loadStateFromXML(IXmlStreamReader* xmlreader)
{
    A_D(ARibbonCustomizeManager);

    connect(d.m_customizeEngine, SIGNAL(NameIdToAction(const QString&, QAction**)), 
            this, SIGNAL(NameIdToAction(const QString&, QAction**)));


    if (!xmlreader->readNextStartElement())
        return false;

    if (xmlreader->name() != A_XML_TAGNAME_RIBBON_STORAGE)
        return false;

    QXmlStreamAttributes attrs = xmlreader->attributes();
    if (!attrs.hasAttribute(QStringLiteral("version")))
        return false;
    QString version = attrs.value(QStringLiteral("version")).toString();
    if (version != QStringLiteral("2.0"))
        return false;

    if (d._bRestoreVisualizePage)
        d.storePageVisual();

    d.m_customizeEngine->restoreState(xmlreader);

    if (d._bRestoreVisualizePage)
        d.restorePageVisual();


    if (xmlreader->readNext() != QXmlStreamReader::EndDocument)
        return false;

    return true;
}

APROCH_NAMESPACE_END