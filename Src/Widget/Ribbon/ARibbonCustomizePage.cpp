/****************************************************************************
 * @file    ARibbonCustomizePage.cpp
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
#include "stdafx.h"
#include <QPainter>
#include <QBitmap>
#include <QRadioButton>
#include <QScrollBar>
#include <QLineEdit>
#include <QSpinBox>
#include <QPainterPath>
#include <QWidgetAction>
#include <QMessageBox>
#include <QApplication>

#include "Widget/Style/AStyleOption.h"
#include "Widget/Style/AStyleHelper.h"
#include "ARibbonCustomizePage.h"
#include "ARibbonCustomizeDialog_p.h"
#include "ARibbonCustomizeManager.h"
#include "ARibbonCustomizeManager_p.h"
#include "ARibbonQuickAccessBar.h"
#include "ARibbonBar.h"   
#include "ARibbonPage.h"
#include "ARibbonGroup.h"
#include "ARibbonDef.h"
#include "ui_UI_RibbonRenameDialog.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

/* ARibbonCustomizeStyledItemDelegate */
ARibbonCustomizeStyledItemDelegate::ARibbonCustomizeStyledItemDelegate(QListWidget* listWidget)
    : QStyledItemDelegate(listWidget)
    , m_listWidget(listWidget)
{
}

ARibbonCustomizeStyledItemDelegate::~ARibbonCustomizeStyledItemDelegate()
{
}

void ARibbonCustomizeStyledItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_ASSERT(index.isValid());

#if (QT_VERSION < QT_VERSION_CHECK(5, 7, 0))
    QStyleOptionViewItemV4 opt = option;
#else
    QStyleOptionViewItem opt = option;
#endif
    initStyleOption(&opt, index);

    const QWidget* widget = m_listWidget;
    QStyle* style = widget ? widget->style() : QApplication::style();

    painter->save();
    painter->setClipRect(opt.rect);

    QRect iconRect = style->subElementRect(QStyle::SE_ItemViewItemDecoration, &opt, widget);
    iconRect.setLeft(3);
    iconRect.setRight(19);
    QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &opt, widget);
    int offset = iconRect.width() + 3;
    textRect.translate(offset - textRect.x(), 0);

    // draw the background
    style->proxy()->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, widget);

    // draw the icon
    QIcon::Mode mode = QIcon::Normal;
    if (!(opt.state & QStyle::State_Enabled))
        mode = QIcon::Disabled;
    else if (opt.state & QStyle::State_Selected)
        mode = QIcon::Selected;
    QIcon::State state = opt.state & QStyle::State_Open ? QIcon::On : QIcon::Off;
    opt.icon.paint(painter, iconRect, opt.decorationAlignment, mode, state);

    // draw the text
    if (!opt.text.isEmpty()) 
    {
        QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled
            ? QPalette::Normal : QPalette::Disabled;
        if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
            cg = QPalette::Inactive;

        if (opt.state & QStyle::State_Selected)
            painter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
        else
            painter->setPen(opt.palette.color(cg, QPalette::Text));

        if (opt.state & QStyle::State_Editing) 
        {
            painter->setPen(opt.palette.color(cg, QPalette::Text));
            painter->drawRect(textRect.adjusted(0, 0, -1, -1));
        }

        const int textMargin = style->pixelMetric(QStyle::PM_FocusFrameHMargin, 0, widget) + 1;
        textRect = textRect.adjusted(textMargin, 0, -textMargin, 0); // remove width padding

        painter->setFont(opt.font);
        style->drawItemText(painter, textRect, QStyle::visualAlignment(opt.direction, opt.displayAlignment), 
            opt.palette, true/*enabled*/, opt.text/*, QPalette::ColorRole textRole = QPalette::NoRole*/);
    }

    // draw the focus rect
    if (opt.state & QStyle::State_HasFocus) 
    {
        QStyleOptionFocusRect o;
        o.QStyleOption::operator=(opt);
        o.rect = style->proxy()->subElementRect(QStyle::SE_ItemViewItemFocusRect, &opt, widget);
        o.state |= QStyle::State_KeyboardFocusChange;
        o.state |= QStyle::State_Item;
        QPalette::ColorGroup cg = (opt.state & QStyle::State_Enabled)
            ? QPalette::Normal : QPalette::Disabled;
        o.backgroundColor = opt.palette.color(cg, (opt.state & QStyle::State_Selected)
            ? QPalette::Highlight : QPalette::Window);
        style->proxy()->drawPrimitive(QStyle::PE_FrameFocusRect, &o, painter, widget);
    }

    painter->restore();
}


/* ARibbonQuickAccessBarCustomizePagePrivate */
ARibbonQuickAccessBarCustomizePagePrivate::ARibbonQuickAccessBarCustomizePagePrivate()
{
    m_ribbonBar = nullptr;
    m_separator = nullptr;
    m_currentAction = nullptr;
    m_separatorText = ARibbonBar::tr_compatible(RibbonSeparatorString);
    m_heightRowItem = 16;
    m_widthColIconItem = 20;
    m_widthColViewItem = 35;
    m_wasDisplayed = false;
}

ARibbonQuickAccessBarCustomizePagePrivate::~ARibbonQuickAccessBarCustomizePagePrivate()
{
    delete m_separator;
}

void ARibbonQuickAccessBarCustomizePagePrivate::init()
{
    A_P(ARibbonQuickAccessBarCustomizePage);
    Q_ASSERT(m_ribbonBar != nullptr);
    m_pageUI.setupUi(&p);

    QAbstractItemDelegate* itemDelegate = m_pageUI.commandsList->itemDelegate();
    m_pageUI.commandsList->setItemDelegate(new ARibbonCustomizeStyledItemDelegate(m_pageUI.commandsList));
    delete itemDelegate;

    QSize sz = QSize(24, 24);
    QPixmap m_imagesUpSrc = p.style()->standardPixmap(static_cast<QStyle::StandardPixmap>(ACommonStyle::SP_RibbonShadedArrowUp), nullptr);
    m_imagesUpSrc = m_imagesUpSrc.scaled(sz, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap m_imagesDownSrc = p.style()->standardPixmap(static_cast<QStyle::StandardPixmap>(ACommonStyle::SP_RibbonShadedArrowDown), nullptr);
    m_imagesDownSrc = m_imagesDownSrc.scaled(sz, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    m_pageUI.upButton->setText(QString());
    m_pageUI.upButton->setIcon(m_imagesUpSrc);
    m_pageUI.downButton->setText(QString());
    m_pageUI.downButton->setIcon(m_imagesDownSrc);

    m_separator = new QAction(m_separatorText, nullptr);
    m_separator->setSeparator(true);

    initListWidgetCommands();

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    connect(m_pageUI.comboBoxSourceCategory, SIGNAL(currentTextChanged(const QString&)), this, SLOT(setCurrentCategoryText(const QString&)));
#else
    connect(m_pageUI.comboBoxSourceCategory, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(setCurrentCategoryText(const QString&)));
#endif
    connect(m_pageUI.addButton,       SIGNAL(released()), this, SLOT(addClicked()));
    connect(m_pageUI.removeButton,    SIGNAL(released()), this, SLOT(removeClicked()));
    connect(m_pageUI.pushButtonReset, SIGNAL(released()), this, SLOT(resetClicked()));
    connect(m_pageUI.upButton,        SIGNAL(released()), this, SLOT(upClicked()));
    connect(m_pageUI.downButton,      SIGNAL(released()), this, SLOT(downClicked()));

    connect(m_pageUI.commandsList,       SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(currentCommandChanged(QListWidgetItem*)));
    connect(m_pageUI.quickAccessBarList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(currentQTCommandChanged(QListWidgetItem*)));

    m_pageUI.checkBoxQATOn->setChecked(m_ribbonBar->quickAccessBarPosition() != ARibbonBar::TopPosition);
}

void ARibbonQuickAccessBarCustomizePagePrivate::setupPage()
{
    Q_ASSERT(m_ribbonBar != nullptr);
    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    manager->setEditMode(true);
    m_pageUI.checkBoxQATOn->setChecked(m_ribbonBar->quickAccessBarPosition() != ARibbonBar::TopPosition);
    fillActions();
    fillStateCommands();
    setButtons();
}

static QIcon aproch_createIconStyleWidget(QWidget* widget)
{
    if (qobject_cast<QCheckBox*>(widget) || qobject_cast<QRadioButton*>(widget))
    {
        bool isRadio = qobject_cast<QRadioButton*>(widget);
        QStyleOptionButton opt;
        opt.initFrom(widget);
        int h = qApp->style()->proxy()->pixelMetric(QStyle::PM_IndicatorHeight, &opt, widget);
        opt.rect.setSize(QSize(h, h));

        QPixmap px(opt.rect.width(), opt.rect.height());
        px.fill(Qt::transparent);
        QPainter p(&px);

        opt.state &= ~QStyle::State_Off;
        opt.state &= ~QStyle::State_On;
        opt.state &= ~QStyle::State_NoChange;
        opt.state |= QStyle::State_On;

        qApp->style()->drawPrimitive(isRadio ? QStyle::PE_IndicatorRadioButton : QStyle::PE_IndicatorCheckBox, &opt, &p, 0);

        return QIcon(px);
    }
    return QIcon();
}

QIcon ARibbonQuickAccessBarCustomizePagePrivate::createIconStyleWidget(QWidget* widget)
{
    return aproch_createIconStyleWidget(widget);
}

class QtnHackToolButton : public QToolButton { public: void init(QStyleOptionToolButton* opt) { initStyleOption(opt); } };
#if (QT_VERSION < QT_VERSION_CHECK(5, 7, 0))
class QtnHackLineEdit : public QLineEdit { public: void init(QStyleOptionFrameV2* opt) { initStyleOption(opt); } };
#else
class QtnHackLineEdit : public QLineEdit { public: void init(QStyleOptionFrame* opt) { initStyleOption(opt); } };
#endif
class QtnHackComboBox : public QComboBox { public: void init(QStyleOptionComboBox* opt) { initStyleOption(opt); } };
class QtnHackSpinBox : public QSpinBox { public: QLineEdit* getlineEdit() { return lineEdit(); } };

static QPixmap aproch_createIconExStyleWidget(QWidget* widget, int width, int height, bool& iconView)
{
    if (widget == nullptr)
        return QPixmap();

    if (qobject_cast<QToolButton*>(widget))
    {
        iconView = true;
        QStyleOptionToolButton opt;
        ((QtnHackToolButton*)widget)->init(&opt);

        if (opt.features & QStyleOptionToolButton::HasMenu)
        {
            int mbi = qApp->style()->pixelMetric(QStyle::PM_MenuButtonIndicator, &opt, widget);
            opt.rect.setSize(QSize(mbi, mbi));

            QPixmap px(opt.rect.width(), opt.rect.height());
            px.fill(Qt::transparent);
            QPainter p(&px);
            qApp->style()->drawPrimitive(QStyle::PE_IndicatorArrowRight, &opt, &p, widget);
            return px;
        }
    }
    else if (qobject_cast<QLineEdit*>(widget))
    {
        iconView = false;
        QtStyleOptionFrame opt;
        ((QtnHackLineEdit*)widget)->init(&opt);
        if (opt.lineWidth <= 0)
            opt.lineWidth = 1;

        opt.rect = QRect(QPoint(0, 0), QSize(width, height));

        QPixmap px(opt.rect.width(), opt.rect.height());
        px.fill(Qt::transparent);
        QPainter p(&px);
        qApp->style()->drawPrimitive(QStyle::PE_PanelLineEdit, &opt, &p, widget);

        QRect rectCaret = opt.rect;
        rectCaret.adjust(3, 2, -3, -2);
        rectCaret.setRight(rectCaret.left() + 4);

        QPen oldPen = p.pen();
        p.setPen(Qt::black);

        QPainterPath path;
        path.moveTo(rectCaret.left(), rectCaret.top());
        path.lineTo(rectCaret.right(), rectCaret.top());

        path.moveTo(rectCaret.center().x(), rectCaret.top());
        path.lineTo(rectCaret.center().x(), rectCaret.bottom());

        path.moveTo(rectCaret.left(), rectCaret.bottom());
        path.lineTo(rectCaret.right(), rectCaret.bottom());

        p.drawPath(path);
        p.setPen(oldPen);

        return px;
    }
    else if (QSpinBox* wd = qobject_cast<QSpinBox*>(widget))
    {
        return aproch_createIconExStyleWidget(((QtnHackSpinBox*)wd)->getlineEdit(), width, height, iconView);
    }
    else if (QComboBox* cb = qobject_cast<QComboBox*>(widget))
    {
        iconView = false;
        QStyleOptionComboBox opt;
        ((QtnHackComboBox*)cb)->init(&opt);
        opt.rect = QRect(QPoint(0, 0), QSize(width, height));

        QPixmap px(opt.rect.width(), opt.rect.height());
        px.fill(Qt::transparent);
        QPainter p(&px);
        qApp->style()->drawComplexControl(QStyle::CC_ComboBox, &opt, &p, widget);
        return px;
    }
    return QPixmap();
}

QPixmap ARibbonQuickAccessBarCustomizePagePrivate::createIconExStyleWidget(QWidget* widget, bool& iconView)
{
    return aproch_createIconExStyleWidget(widget, m_widthColViewItem, m_heightRowItem, iconView);
}

void ARibbonQuickAccessBarCustomizePagePrivate::initListWidgetCommands()
{
    QStyleOption opt;
    opt.initFrom(m_pageUI.commandsList);

    QString str = QStringLiteral("XX");
    int width = opt.fontMetrics.boundingRect(str).width();
    m_heightRowItem = qMax(m_heightRowItem, width);
}

void ARibbonQuickAccessBarCustomizePagePrivate::fillActions()
{
    m_pageUI.comboBoxSourceCategory->clear();
    m_pageUI.comboBoxSourceCategory->addItems(m_sourceCategories);
}

void ARibbonQuickAccessBarCustomizePagePrivate::fillStateCommands()
{
    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != NULL);

    m_pageUI.quickAccessBarList->clear();

    setButtons();
    QListIterator<QAction *> itAction(manager->actions(m_ribbonBar->quickAccessBar()));
    QListWidgetItem* first = nullptr;

    while (itAction.hasNext()) 
    {
        QAction* action = itAction.next();

        QString actionName = m_separatorText;
        if (action)
            actionName = action->text();

        actionName.remove(QLatin1Char('&'));

        if (!actionName.isEmpty())
        {
            QListWidgetItem* item = new QListWidgetItem(actionName, m_pageUI.quickAccessBarList);
            if (action)
            {
                item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                QIcon icon = action->property("icon").value<QIcon>();
                item->setIcon(icon);
            }
            if (!first)
                first = item;
        }
    }
    if (first)
        m_pageUI.quickAccessBarList->setCurrentItem(first);

}

void ARibbonQuickAccessBarCustomizePagePrivate::fillListCategorieCommads(const QList<QAction*>& actions)
{
    m_pageUI.commandsList->clear();
    m_actionToItem.clear();
    m_itemToAction.clear();

    QListIterator<QAction *> itAction(actions);
    while (itAction.hasNext()) 
    {
        if (QAction* action = itAction.next())
        {
            QString actionName = action->text();
            actionName.remove(QLatin1Char('&'));

            QListWidgetItem* item = new QListWidgetItem(actionName, m_pageUI.commandsList);
            item->setIcon(action->icon());
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

            m_actionToItem.insert(action, item);
            m_itemToAction.insert(item, action);
        }
    }
    m_pageUI.commandsList->setCurrentItem(m_currentAction);
}

void ARibbonQuickAccessBarCustomizePagePrivate::setButtons()
{
    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != NULL);

    bool addEnabled = false;
    bool removeEnabled = false;

    QListWidgetItem* itemCommand = m_pageUI.commandsList->currentItem();
    QListWidgetItem* itemCommandQT = m_pageUI.quickAccessBarList->currentItem();
    if (itemCommand && (itemCommandQT || m_pageUI.quickAccessBarList->count() == 0))
    {
        if (itemCommand->text() != m_separatorText)
        {
            QAction* action = m_itemToAction.value(itemCommand);
            if (action && !manager->containsAction(m_ribbonBar->quickAccessBar(), action))
                addEnabled = true;
        }
        else
            addEnabled = true;
    }
    if (itemCommandQT)
    {
        removeEnabled = true;
    }

    m_pageUI.addButton->setEnabled(addEnabled);
    m_pageUI.removeButton->setEnabled(removeEnabled);

    int indexRow = m_pageUI.quickAccessBarList->currentRow();
    m_pageUI.upButton->setEnabled(indexRow != 0);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
    m_pageUI.downButton->setEnabled(indexRow < m_pageUI.quickAccessBarList->count()-1);
}

void ARibbonQuickAccessBarCustomizePagePrivate::applyClicked()
{
    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != NULL);
    manager->commit();

    if (m_ribbonBar->quickAccessBar() && !m_ribbonBar->quickAccessBar()->isHidden())
        m_ribbonBar->setQuickAccessBarPosition( m_pageUI.checkBoxQATOn->isChecked() ? ARibbonBar::BottomPosition : ARibbonBar::TopPosition);
}

void ARibbonQuickAccessBarCustomizePagePrivate::setCurrentCategoryText(const QString& strCategory)
{
    if (strCategory.isEmpty())
        return;

    if (m_strSeparator == strCategory)
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        m_pageUI.comboBoxSourceCategory->setCurrentText(m_currentSourceCategory);
#else
        int index = m_pageUI.comboBoxSourceCategory->findText(m_currentSourceCategory);
        if (index != -1)
            m_pageUI.comboBoxSourceCategory->setCurrentIndex(index);
#endif
        return;
    }
    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != nullptr);

    QList<QAction*> actions = manager->actionsByCategory(strCategory);
    fillListCategorieCommads(actions);
    m_currentSourceCategory = strCategory;
}

void ARibbonQuickAccessBarCustomizePagePrivate::currentCommandChanged(QListWidgetItem* current)
{
    if (m_itemToAction.contains(current))
        m_currentAction = current;
    else
        m_currentAction = nullptr;
    setButtons();
}

void ARibbonQuickAccessBarCustomizePagePrivate::currentQTCommandChanged(QListWidgetItem* current)
{
    Q_UNUSED(current);
    setButtons();
}

void ARibbonQuickAccessBarCustomizePagePrivate::addClicked()
{
    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != NULL);

    if (!m_currentAction)
        return;

    QListWidgetItem* currentToolBarAction = m_pageUI.quickAccessBarList->currentItem();

    QAction* action = m_itemToAction.value(m_currentAction);
    QListWidgetItem* item = nullptr;
    if (action) 
    {
        if (manager->containsAction(m_ribbonBar->quickAccessBar(), action)) 
        {
            item = m_actionToCurrentItem.value(action);

            if (item == currentToolBarAction)
                return;

            int row = m_pageUI.quickAccessBarList->row(item);
            m_pageUI.quickAccessBarList->takeItem(row);
            manager->removeActionAt(m_ribbonBar->quickAccessBar(), row);
        } 
        else 
        {
            QString actionName = action->text();
            actionName.remove(QLatin1Char('&'));
            item = new QListWidgetItem(actionName);
            item->setIcon(action->icon());
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            m_currentItemToAction.insert(item, action);
            m_actionToCurrentItem.insert(action, item);
        }
    } else {
        item = new QListWidgetItem(m_separatorText);
        m_currentItemToAction.insert(item, 0);
    }

    int row = m_pageUI.quickAccessBarList->count();
    if (currentToolBarAction)
        row = m_pageUI.quickAccessBarList->row(currentToolBarAction) + 1;


    m_pageUI.quickAccessBarList->insertItem(row, item);
    manager->insertAction(m_ribbonBar->quickAccessBar(), action, row);

    QListWidgetItem* itemCommand = m_pageUI.commandsList->currentItem();
    int index = m_pageUI.commandsList->row(itemCommand) + 1;
    m_pageUI.commandsList->setCurrentRow(index);

    m_pageUI.quickAccessBarList->setCurrentItem(item);

    setButtons();
}

void ARibbonQuickAccessBarCustomizePagePrivate::removeClicked()
{
    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != NULL);

    QListWidgetItem* currentToolBarAction = m_pageUI.quickAccessBarList->currentItem();
    if (!currentToolBarAction)
        return;

    int row = m_pageUI.quickAccessBarList->row(currentToolBarAction);
    manager->removeActionAt(m_ribbonBar->quickAccessBar(), row);

    QAction* action = m_currentItemToAction.value(currentToolBarAction);

    if (action)
        m_actionToCurrentItem.remove(action);

    m_currentItemToAction.remove(currentToolBarAction);
    delete currentToolBarAction;

    if (row == m_pageUI.quickAccessBarList->count())
        row--;

    if (row >= 0) 
    {
        QListWidgetItem *item = m_pageUI.quickAccessBarList->item(row);
        m_pageUI.quickAccessBarList->setCurrentItem(item);
    }
    setButtons();
}

void ARibbonQuickAccessBarCustomizePagePrivate::resetClicked()
{
    if (ARibbonQuickAccessBar* quickAccessBar = m_ribbonBar->quickAccessBar())
    {
        ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
        Q_ASSERT(manager != nullptr);
        manager->reset(quickAccessBar);
        fillStateCommands();
    }
}

void ARibbonQuickAccessBarCustomizePagePrivate::upClicked()
{
    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != nullptr);

    QListWidgetItem* currentToolBarAction = m_pageUI.quickAccessBarList->currentItem();
    if (!currentToolBarAction)
        return;
    int row = m_pageUI.quickAccessBarList->row(currentToolBarAction);
    if (row == 0)
        return;
    m_pageUI.quickAccessBarList->takeItem(row);
    int newRow = row - 1;
    m_pageUI.quickAccessBarList->insertItem(newRow, currentToolBarAction);

    QAction* action = manager->actions(m_ribbonBar->quickAccessBar()).at(row);
    manager->removeActionAt(m_ribbonBar->quickAccessBar(), row);
    manager->insertAction(m_ribbonBar->quickAccessBar(), action, newRow);
    m_pageUI.quickAccessBarList->setCurrentItem(currentToolBarAction);
    setButtons();
}

void ARibbonQuickAccessBarCustomizePagePrivate::downClicked()
{
    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != nullptr);

    QListWidgetItem *currentToolBarAction = m_pageUI.quickAccessBarList->currentItem();
    if (!currentToolBarAction)
        return;
    int row = m_pageUI.quickAccessBarList->row(currentToolBarAction);
    if (row == m_pageUI.quickAccessBarList->count() - 1)
        return;
    m_pageUI.quickAccessBarList->takeItem(row);
    int newRow = row + 1;
    m_pageUI.quickAccessBarList->insertItem(newRow, currentToolBarAction);
    QAction* action = manager->actions(m_ribbonBar->quickAccessBar()).at(row);
    manager->removeActionAt(m_ribbonBar->quickAccessBar(), row);
    manager->insertAction(m_ribbonBar->quickAccessBar(), action, newRow);
    m_pageUI.quickAccessBarList->setCurrentItem(currentToolBarAction);
    setButtons();
}


/*ARibbonQuickAccessBarCustomizePage*/
ARibbonQuickAccessBarCustomizePage::ARibbonQuickAccessBarCustomizePage(ARibbonBar* ribbonBar)
    : QWidget(nullptr)
{
    A_INIT_PRIVATE(ARibbonQuickAccessBarCustomizePage);
    A_D(ARibbonQuickAccessBarCustomizePage);
    d.m_ribbonBar = ribbonBar;
    d.init();
}

ARibbonQuickAccessBarCustomizePage::~ARibbonQuickAccessBarCustomizePage()
{
    A_DELETE_PRIVATE();
}

ARibbonBar* ARibbonQuickAccessBarCustomizePage::ribbonBar() const
{
    A_D(const ARibbonQuickAccessBarCustomizePage);
    return d.m_ribbonBar;
}

void ARibbonQuickAccessBarCustomizePage::addCustomCategory(const QString& strCategory)
{
    A_D(ARibbonQuickAccessBarCustomizePage);
    QString str = strCategory;
    str.remove(QStringLiteral("&"));
    d.m_sourceCategories.append(str);
}

void ARibbonQuickAccessBarCustomizePage::addSeparatorCategory(const QString& strCategory)
{
    A_D(ARibbonQuickAccessBarCustomizePage);

    QString separator = strCategory;
    if (separator.isEmpty())
        separator = QStringLiteral("----------");

    d.m_strSeparator = separator;
    d.m_sourceCategories.append(separator);
}

void ARibbonQuickAccessBarCustomizePage::accepted()
{
    A_D(ARibbonQuickAccessBarCustomizePage);
    if (d.m_wasDisplayed)
        d.applyClicked();
}

void ARibbonQuickAccessBarCustomizePage::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    A_D(ARibbonQuickAccessBarCustomizePage);
    d.m_wasDisplayed = true;
    d.setupPage();
}

///////////////////////////////////////////////////////////////////////////////
/* ARibbonBarCustomizePagePrivate */
ARibbonBarCustomizePagePrivate::ARibbonBarCustomizePagePrivate()
{
    m_ribbonBar = nullptr;
    m_currentSourceItemPage = nullptr;
    m_currentSourceItemGroup = nullptr;
    m_currentSourceAction = nullptr;
    m_currentItemPage = nullptr;
    m_currentItemGroup = nullptr;
    m_currentItemAction = nullptr;
    m_sufNameCustom = QStringLiteral(" (自定义)");
    m_wasDisplayed = false;
}

ARibbonBarCustomizePagePrivate::~ARibbonBarCustomizePagePrivate()
{
}

/* RibbonCustomizeRibbonStyledItemDelegate */
class RibbonCustomizeRibbonStyledItemDelegate : public QStyledItemDelegate
{
public:
    explicit RibbonCustomizeRibbonStyledItemDelegate(QObject* parent = nullptr)
        : QStyledItemDelegate(parent)
    {
    }
    virtual ~RibbonCustomizeRibbonStyledItemDelegate()
    {
    }
    void addCaption(const QString& strCategory)
    {
        Q_ASSERT(!strCategory.isEmpty());
        m_lstCaption.append(strCategory);
    }
    void clearCaptions()
    {
        m_lstCaption.clear();
    }
protected:
    virtual void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
    {
        QStyledItemDelegate::initStyleOption(option, index);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        if (m_lstCaption.contains(option->text))
            option->rect.setLeft(0);
#else
        if (const QStyleOptionViewItemV4* vopt = qstyleoption_cast<QStyleOptionViewItemV4*>(option))
            if (m_lstCaption.contains(vopt->text))
                option->rect.setLeft(0);
#endif // QT_VERSION_CHECK(4, 0, 0)
    }
protected:
    QList<QString> m_lstCaption;
};

void ARibbonBarCustomizePagePrivate::init()
{
    A_P(ARibbonBarCustomizePage);
    Q_ASSERT(m_ribbonBar != nullptr);
    m_pageUI.setupUi(&p);

    m_pageUI.treeCommands->setItemDelegate(new RibbonCustomizeRibbonStyledItemDelegate(m_pageUI.treeCommands));
    m_pageUI.treeRibbon->setItemDelegate(new RibbonCustomizeRibbonStyledItemDelegate(m_pageUI.treeRibbon));
    m_pageUI.switchTabsBox->setVisible(false);

    QSize sz = QSize(24, 24);
    QPixmap m_imagesUpSrc = p.style()->standardPixmap(static_cast<QStyle::StandardPixmap>(ACommonStyle::SP_RibbonShadedArrowUp), nullptr);
    m_imagesUpSrc = m_imagesUpSrc.scaled(sz, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap m_imagesDownSrc = p.style()->standardPixmap(static_cast<QStyle::StandardPixmap>(ACommonStyle::SP_RibbonShadedArrowDown), nullptr);
    m_imagesDownSrc = m_imagesDownSrc.scaled(sz, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    m_pageUI.upButton->setText(QString());
    m_pageUI.upButton->setIcon(m_imagesUpSrc);
    m_pageUI.downButton->setText(QString());
    m_pageUI.downButton->setIcon(m_imagesDownSrc);

    connect(m_pageUI.addButton,        SIGNAL(released()), this, SLOT(addClicked()));
    connect(m_pageUI.newTabButton,     SIGNAL(released()), this, SLOT(newTabClicked()));
    connect(m_pageUI.newGroupButton,   SIGNAL(released()), this, SLOT(newGroupClicked()));
    connect(m_pageUI.removeButton,     SIGNAL(released()), this, SLOT(removeClicked()));
    connect(m_pageUI.resetButton,      SIGNAL(released()), this, SLOT(resetClicked()));
    connect(m_pageUI.upButton,         SIGNAL(released()), this, SLOT(upClicked()));
    connect(m_pageUI.downButton,       SIGNAL(released()), this, SLOT(downClicked()));
    connect(m_pageUI.renameButton,     SIGNAL(released()), this, SLOT(renameClicked()));

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    connect(m_pageUI.comboBoxSourceCategory, SIGNAL(currentTextChanged(const QString&)), this, SLOT(setCurrentCategoryText(const QString&)));
#else
    connect(m_pageUI.comboBoxSourceCategory, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(setCurrentCategoryText(const QString&)));
#endif

    connect(m_pageUI.treeCommands,  SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(currentSourceChanged(QTreeWidgetItem*)));

    connect(m_pageUI.treeRibbon,   SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(currentPageChanged(QTreeWidgetItem*)));
    connect(m_pageUI.treeRibbon,   SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(itemPageChanged(QTreeWidgetItem*,int)) );
}

void ARibbonBarCustomizePagePrivate::setupPage()
{
    Q_ASSERT(m_ribbonBar != nullptr);

    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    if (!manager->isEditMode())
        manager->setEditMode();
    fillSourceCategories();
    setCurrentTabsText(QString());
    fillStateCategories();
}
#if 0
void ARibbonBarCustomizePagePrivate::setCategoryCaptionTree(QTreeWidget* tree, const QString& strCategory)
{
    A_P(ARibbonBarCustomizePage)
    if (!strCategory.isEmpty())
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(tree, QStringList(strCategory));
        item->setDisabled(true);
        QFont font = p.font(); 
        font.setBold(true);
        item->setFont(0, font);
        item->setBackground(0, QColor(238, 238, 238));
        RibbonCustomizeRibbonStyledItemDelegate* ribbonStyledItemDelegate = qobject_cast<RibbonCustomizeRibbonStyledItemDelegate*>(tree->itemDelegate());
        Q_ASSERT(ribbonStyledItemDelegate != nullptr);
        ribbonStyledItemDelegate->addCaption(strCategory);
    }
}
#endif

void ARibbonBarCustomizePagePrivate::clearCategoryCaptionTree(QTreeWidget* tree)
{
    RibbonCustomizeRibbonStyledItemDelegate* ribbonStyledItemDelegate = static_cast<RibbonCustomizeRibbonStyledItemDelegate*>(tree->itemDelegate());
    Q_ASSERT(ribbonStyledItemDelegate != nullptr);
    ribbonStyledItemDelegate->clearCaptions();
}

void ARibbonBarCustomizePagePrivate::fillSourceCategories()
{
    m_pageUI.comboBoxSourceCategory->clear();
    m_pageUI.comboBoxSourceCategory->addItems(m_sourceCategories);
    if (m_sourceCategories.size() > 0)
        m_pageUI.comboBoxSourceCategory->setCurrentIndex(0);
}

void ARibbonBarCustomizePagePrivate::fillSourceActions(QList<QAction*>& actions, QTreeWidgetItem* parentItem)
{
    QTreeWidgetItem* first = nullptr;
    QListIterator<QAction*> itAction(actions);
    while (itAction.hasNext()) 
    {
        QAction* action = itAction.next();
        if (action == nullptr)
            continue;

        QString actionName = action->text();
        actionName.remove(QStringLiteral("&"));

        if (!actionName.isEmpty())
        {
            QTreeWidgetItem* item = nullptr;
            if (parentItem)
                item = new QTreeWidgetItem(parentItem, QStringList(actionName));
            else
                item = new QTreeWidgetItem(m_pageUI.treeCommands, QStringList(actionName));

            item->setTextAlignment(0, Qt::AlignLeft | Qt::AlignVCenter );
            QIcon icon = action->property("icon").value<QIcon>();
            item->setIcon(0, icon);
            m_currentSourceItemToAction.insert(item, action);

            if (!first)
                first = item;
        }
    }

    if (first != nullptr)
        m_pageUI.treeCommands->setCurrentItem(first);
}

void ARibbonBarCustomizePagePrivate::fillSourcePages(QList<ARibbonPage*>& pages, const QString& strCategory)
{
    Q_UNUSED(strCategory);
    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != NULL);
    ARibbonBarCustomizeEngine* customizeEngine = manager->aproch_d().m_customizeEngine;

    QListIterator<ARibbonPage*> itPage(pages);
    while (itPage.hasNext()) 
    {
        ARibbonPage* page = itPage.next();

        if (!customizeEngine->defaultPageName(page).isEmpty())
        {
            QString title = customizeEngine->defaultPageName(page);
            title.remove(QStringLiteral("&"));

            QTreeWidgetItem* item = new QTreeWidgetItem(m_pageUI.treeCommands, QStringList(title));
            fillSourceGroups(page, item);

            m_currentSourceItemToPage.insert(item, page);
        }
    }
}

void ARibbonBarCustomizePagePrivate::fillSourceGroups(ARibbonPage* page, QTreeWidgetItem* parentItem)
{
    Q_ASSERT(page != nullptr);
    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != NULL);
    ARibbonBarCustomizeEngine* customizeEngine = manager->aproch_d().m_customizeEngine;

    QList< ARibbonGroup* > listGroup = customizeEngine->pageDefaultGroups(page);
    for (int i = 0, count = listGroup.count(); count > i; ++i)
    {
        QString groupTitle = customizeEngine->defaultGroupName(listGroup[i]);
        groupTitle.remove(QStringLiteral("&"));
        if (!groupTitle.isEmpty())
        {
            QTreeWidgetItem* newItem = new QTreeWidgetItem(parentItem, QStringList(groupTitle));
            fillSourceGroupActions(listGroup[i], newItem);
            m_currentSourceItemToGroup.insert(newItem, listGroup[i]);
        }
    }
}

void ARibbonBarCustomizePagePrivate::fillSourceGroupActions(ARibbonGroup* group, QTreeWidgetItem* parentItem)
{
    Q_ASSERT(group != nullptr);
    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != NULL);

    QListIterator<QAction *> itAction(manager->actionsGroup(group));
    while (itAction.hasNext()) 
    {
        QAction* action = itAction.next();

        QString actionName = action->text();
        actionName.remove(QStringLiteral("&"));

        if (!actionName.isEmpty())
        {
            QTreeWidgetItem* item = new QTreeWidgetItem(parentItem, QStringList(actionName));
            item->setTextAlignment(0, Qt::AlignLeft | Qt::AlignVCenter );
            QIcon icon = action->property("icon").value<QIcon>();
            item->setIcon(0, icon);
            m_currentSourceItemToAction.insert(item, action);
        }
    }
}

void ARibbonBarCustomizePagePrivate::fillStateCategories()
{
    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    QStringList listCommands = manager->categories();

    QListIterator<QString> itAction(listCommands);
    QStringList list;
    while (itAction.hasNext()) 
    {
        QString str = itAction.next();
        str.remove(QStringLiteral("&"));
        list << str;
    }

    m_pageUI.switchTabsBox->addItems(list);
    if (listCommands.size() > 0)
        m_pageUI.switchTabsBox->setCurrentIndex(0);
}

void ARibbonBarCustomizePagePrivate::fillStateRibbon(const QString& strCategory)
{
    Q_UNUSED(strCategory);
    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != nullptr);

    m_pageUI.treeRibbon->clear();
    m_pageToCurrentItem.clear();
    m_currentItemToPage.clear();
    m_currentItemToGroup.clear();

    QList<ARibbonPage*> pages = manager->pages();
    QListIterator<ARibbonPage*> itPages(pages);
    QTreeWidgetItem* first = nullptr;

    while (itPages.hasNext()) 
    {
        if (ARibbonPage* page = itPages.next())
        {
            QString pageTitle = !manager->pageName(page).isEmpty() ? manager->pageName(page) : page->title();
            pageTitle.remove(QStringLiteral("&"));

            if (!pageTitle.isEmpty())
            {
                if (page->property(__aproch_Widget_Custom).toString() == QStringLiteral("__aproch_Page_Custom"))
                    pageTitle += m_sufNameCustom;

                QStringList stringsPage;
                stringsPage.append(pageTitle);
                QTreeWidgetItem* item = new QTreeWidgetItem(m_pageUI.treeRibbon, stringsPage);

                if (!page->isVisible())
                    item->setCheckState(0, Qt::Unchecked);
                else
                    item->setCheckState(0, Qt::Checked);

                m_pageToCurrentItem.insert(page, item);
                m_currentItemToPage.insert(item, page);

                fillStateGroups(page, item);

                if (!first)
                    first = item;
            }
        }
    }

    enableVisualizePage(_bEnableVisualizePage);

    if (first)
    {
        m_pageUI.treeRibbon->setCurrentItem(first);
        m_pageUI.treeRibbon->expandItem(first);
    }
}

void ARibbonBarCustomizePagePrivate::fillStateGroups(ARibbonPage* page, QTreeWidgetItem* itemParent)
{
    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != nullptr);

    QList<ARibbonGroup*> groups = manager->pageGroups(page);
    QListIterator<ARibbonGroup*> itGroups(groups);

    while (itGroups.hasNext()) 
    {
        ARibbonGroup* group = itGroups.next();
        QString groupTitle = !manager->groupName(group).isEmpty() ? manager->groupName(group) : group->title();

        groupTitle.remove(QStringLiteral("&"));

        if (!groupTitle.isEmpty())
        {
            if (group->property(__aproch_Widget_Custom).toString() == QStringLiteral("__aproch_Group_Custom"))
                groupTitle += m_sufNameCustom;

            QStringList stringsGroup;
            stringsGroup.append(groupTitle);
            QTreeWidgetItem* item = new QTreeWidgetItem(itemParent, stringsGroup);
            m_currentItemToGroup.insert(item, group);
            fillStateActions(group, item, false);
        }
    }
}

void ARibbonBarCustomizePagePrivate::fillStateActions(ARibbonGroup* group, QTreeWidgetItem* itemParent, bool newGroup)
{
    Q_ASSERT(group != nullptr);
    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != nullptr);

    for (int i = itemParent->childCount() - 1; i >= 0; --i) 
    {
        QTreeWidgetItem* item = itemParent->takeChild(i);
        QAction* action = m_currentItemToAction.value(item);
        m_currentItemToAction.remove(item);
        m_actionToCurrentItem.remove(action);
        delete item;
    }

    bool isCustomGroup = group->property(__aproch_Widget_Custom).toString() == QStringLiteral("__aproch_Group_Custom");
    QListIterator<QAction *> itAction(newGroup ? group->actions() : manager->actionsGroup(group));
    while (itAction.hasNext()) 
    {
        QAction* action = itAction.next();

        QString actionName = action->text();
        actionName.remove(QStringLiteral("&"));

        if (!actionName.isEmpty())
        {
            QTreeWidgetItem* item = new QTreeWidgetItem(itemParent, QStringList(actionName));
            if (!isCustomGroup)
            {
            #if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
                item->setForeground(0, QColor(Qt::gray));
            #else
                item->setTextColor(0, QColor(Qt::gray));
            #endif
            }
            item->setTextAlignment(0, Qt::AlignLeft | Qt::AlignVCenter );
            QIcon icon = action->property("icon").value<QIcon>();
            QPixmap px = icon.pixmap(QSize(16, 16), isCustomGroup ? QIcon::Normal : QIcon::Disabled);
            item->setIcon(0, QIcon(px));

            if (isCustomGroup)
            {
                m_currentItemToAction.insert(item, action);
                m_actionToCurrentItem.insert(action, item);
            }
        }
    }
    if (isCustomGroup)
        manager->appendActions(group, manager->actionsGroup(group));
}

ARibbonPage* ARibbonBarCustomizePagePrivate::addPageCustom(ARibbonPage* srcPage)
{
    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != nullptr);

    QString strNewPage(ARibbonBar::tr_compatible(RibbonNewPageString));
    if (srcPage)
    {
        strNewPage = srcPage->title();
        strNewPage.remove(QLatin1Char('&'));
    }

    ARibbonPage* findPage = m_currentItemToPage.value(m_currentItemPage);
    int index = manager->pageIndex(findPage) + 1;
    ARibbonPage* newPage = manager->createPage(strNewPage, index);

    QTreeWidgetItem* item = new QTreeWidgetItem(m_pageUI.treeRibbon, m_currentItemPage);
    item->setCheckState(0, Qt::Checked);
    strNewPage += m_sufNameCustom;
    item->setText(0, strNewPage);

    m_pageToCurrentItem.insert(newPage, item);
    m_currentItemToPage.insert(item, newPage);
	enableVisualizePage(_bEnableVisualizePage);
    m_pageUI.treeRibbon->setCurrentItem(item);
    return newPage;
}

void ARibbonBarCustomizePagePrivate::addGroupCopy(ARibbonPage* srcPage, ARibbonPage* copyPage)
{
    if (!m_currentItemPage)
        return;

    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != nullptr);

    QList<ARibbonGroup*> srcGroups = srcPage->groups();
    for (int i = 0, count = srcGroups.count(); count > i; ++i)
    {
        ARibbonGroup* srcGroup = srcGroups.at(i);
        if (srcGroup->property(__aproch_Widget_Copy).toString() != QStringLiteral("__aproch_Group_Copy"))
        {
            QString titleGroup(srcGroup->title());
            ARibbonGroup* newGroup = manager->createGroup(copyPage, titleGroup, i);
            newGroup->setIcon(srcGroup->icon());
            newGroup->setProperty(__aproch_Widget_Copy, QStringLiteral("__aproch_Group_Copy"));

            titleGroup.remove(QStringLiteral("&"));
            QTreeWidgetItem* item = new QTreeWidgetItem(QStringList(titleGroup));
            m_currentItemPage->insertChild(i, item);

            m_currentItemToGroup.insert(item, newGroup);

            QListIterator<QAction*> itAction(srcGroup->actions());
            while (itAction.hasNext()) 
            {
                QAction* action = itAction.next();
                newGroup->addAction(action);
            }
            fillStateActions(newGroup, item, true);
        }
    }
}

void ARibbonBarCustomizePagePrivate::addActionGroupCustom(ARibbonGroup* group, QAction* action, int index)
{
    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != nullptr);

    QString actionName = action->text();
    actionName.remove(QStringLiteral("&"));

    if (!actionName.isEmpty())
    {
        QTreeWidgetItem* item = nullptr;
        if (index == -1)
            item = new QTreeWidgetItem(m_currentItemGroup, QStringList(actionName));
        else
        {
            item = new QTreeWidgetItem(QStringList(actionName));
            m_currentItemGroup->insertChild(index, item);
        }
        item->setTextAlignment(0, Qt::AlignLeft | Qt::AlignVCenter );
        QIcon icon = action->property("icon").value<QIcon>();
        QPixmap px = icon.pixmap(QSize(16, 16), QIcon::Normal);
        item->setIcon(0, QIcon(px));

        m_currentItemToAction.insert(item, action);
        m_actionToCurrentItem.insert(action, item);
        manager->insertAction(group, action, index);
    }
}

void ARibbonBarCustomizePagePrivate::setButtons()
{
    bool addEnabled = false;
    bool removeEnabled = false;
    bool renameEnabled = false;

    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != nullptr);

    if (m_currentSourceAction) 
    {
        if (ARibbonGroup* group = m_currentItemToGroup.value(m_currentItemGroup))
        {
            if (QAction* action = m_currentSourceItemToAction.value(m_currentSourceAction))
                addEnabled = !manager->containsAction(group, action);
        }
    }
    else if (!m_currentSourceItemGroup)
        addEnabled = true;
    else if (m_currentSourceItemGroup)
    {
        ARibbonPage* currentSourcePage = m_currentSourceItemToPage.value(m_currentSourceItemPage);
        ARibbonPage* currentPage = m_currentItemToPage.value(m_currentItemPage);
        if (currentSourcePage && currentPage)
        {
            if (currentSourcePage != currentPage)
            {
                ARibbonGroup* group = m_currentSourceItemToGroup.value(m_currentSourceItemGroup);
                QList<QAction*> listActions = manager->actionsGroup(group);//group->actions();

                QList<ARibbonGroup*> listGroup = manager->pageGroups(currentPage);
                QListIterator<ARibbonGroup*> itGroup(listGroup);
                bool find = false;
                while (itGroup.hasNext()) 
                {
                    ARibbonGroup* currentGroup = itGroup.next();
                    QList<QAction*> currentListActions = currentGroup->actions();

                    if (listActions == currentListActions)
                    {
                        find = true;
                        break;
                    }
                }
                if (!find)
                    addEnabled = true;
            }
        }
    }

    if (m_currentItemAction && m_currentItemGroup)
    {
        ARibbonGroup* group = m_currentItemToGroup.value(m_currentItemGroup);
        if (group && group->property(__aproch_Widget_Custom).toString() == QStringLiteral("__aproch_Group_Custom"))
            removeEnabled = true;
    }
    else if (m_currentItemGroup)
    {
        renameEnabled = true;
        removeEnabled = true;
    }
    else if (m_currentItemPage)
    {
        renameEnabled = _bEnableRenamePage;
        ARibbonPage* currentPage = m_currentItemToPage.value(m_currentItemPage);
        if (currentPage && currentPage->property(__aproch_Widget_Custom).toString() == QStringLiteral("__aproch_Page_Custom"))
            removeEnabled = true;
    }

    m_pageUI.addButton->setEnabled(addEnabled);
    m_pageUI.removeButton->setEnabled(removeEnabled);
    m_pageUI.renameButton->setEnabled(renameEnabled);

    bool upEnabled = false;
    bool downEnabled = false;

    if (m_currentItemPage && m_currentItemGroup)
    {
        ARibbonPage* currentPage = m_currentItemToPage.value(m_currentItemPage);
        Q_ASSERT(currentPage != nullptr);
        ARibbonGroup* currentGroup =  m_currentItemToGroup.value(m_currentItemGroup);
        Q_ASSERT(currentGroup != nullptr);

        QList<ARibbonGroup*> groups = manager->pageGroups(currentPage);

        int index = groups.indexOf(currentGroup);
        upEnabled = index != 0;
        downEnabled = index < (groups.size() - 1);

        if (m_currentItemAction)
        {
            if (QAction* action = m_currentItemToAction.value(m_currentItemAction))
            {
                QList<QAction*> actions = manager->actionsGroup(currentGroup);
                index = actions.indexOf(action);
                upEnabled = index != 0;
                downEnabled = index < (actions.size() - 1);
            }
        }
    }
    else if (m_currentItemPage && !m_currentItemGroup)
    {
        int index = m_pageUI.treeRibbon->indexOfTopLevelItem(m_pageUI.treeRibbon->currentItem());
     
        if (_bEnableMove)
        {
            upEnabled = index != 0;
            downEnabled = index < m_pageUI.treeRibbon->topLevelItemCount()-1;
        }
        else
        {
            upEnabled = false;
            downEnabled = false;
        }
    }

    m_pageUI.upButton->setEnabled(upEnabled);
    m_pageUI.downButton->setEnabled(downEnabled);
}

void ARibbonBarCustomizePagePrivate::applyClicked()
{
    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != NULL);
    manager->commit();
}

void ARibbonBarCustomizePagePrivate::cancelClicked()
{
    m_ribbonBar->customizeManager()->cancel();
}

void ARibbonBarCustomizePagePrivate::enableVisualizePage(bool bEnable)
{
    _bEnableVisualizePage = bEnable;

    auto items = m_currentItemToPage.keys();

    if (items.count() == 0)
        return;

    for (auto pItem : items)
    {
        if(pItem == nullptr)
            continue;

        if (bEnable)
        {
            pItem->setFlags(pItem->flags() | Qt::ItemIsUserCheckable);
            pItem->setTextColor(0, QColor(Qt::black));
        }
        else
        {
			pItem->setFlags(pItem->flags() & ~Qt::ItemIsUserCheckable);
			pItem->setTextColor(0, QColor(Qt::darkGray));
		}
    }
}

void ARibbonBarCustomizePagePrivate::enableAddPage(bool bEnable)
{
    if (m_pageUI.newTabButton == nullptr)
        return;

    m_pageUI.newTabButton->setEnabled(bEnable);
}

void ARibbonBarCustomizePagePrivate::enableRenamePage(bool bEnable)
{
    if (m_pageUI.renameButton == nullptr)
        return;

    _bEnableRenamePage = bEnable;
}

void ARibbonBarCustomizePagePrivate::enableMovePage(bool bEnable)
{
    _bEnableMove = bEnable;
}

void ARibbonBarCustomizePagePrivate::addClicked()
{
    A_P(ARibbonBarCustomizePage);

    if (m_currentSourceAction && m_currentItemGroup)
    {
        ARibbonGroup* group = m_currentItemToGroup.value(m_currentItemGroup);
        if (group && group->property(__aproch_Widget_Custom).toString() == QStringLiteral("__aproch_Group_Custom"))
        {
            if (QAction* action = m_currentSourceItemToAction.value(m_currentSourceAction))
            {
                int index = m_currentItemGroup->indexOfChild(m_currentItemAction);
                addActionGroupCustom(group, action, index + 1);

                m_currentItemAction = m_currentItemGroup->child(index + 1);

                if (m_currentItemAction)
                    m_pageUI.treeRibbon->setCurrentItem(m_currentItemAction);
                else
                {
                    QTreeWidgetItem* currentItem = m_currentItemGroup->child(m_currentItemGroup->childCount()-1);
                    m_pageUI.treeRibbon->setCurrentItem(currentItem);
                }

                if (m_currentSourceItemGroup && m_currentSourceAction)
                {
                    int indexNext = m_currentSourceItemGroup->indexOfChild(m_currentSourceAction) + 1;
                    QTreeWidgetItem* nextItem = m_currentSourceItemGroup->child(indexNext);
                    nextItem = nextItem ? nextItem : m_currentSourceAction;
                    m_pageUI.treeCommands->setCurrentItem(nextItem);
                }
                else if (m_currentSourceAction)
                {
                    int index = m_pageUI.treeCommands->indexOfTopLevelItem(m_currentSourceAction);
                    QTreeWidgetItem* item = m_pageUI.treeCommands->topLevelItem(index + 1);
                    item = item ? item : m_currentSourceAction;
                    m_pageUI.treeCommands->setCurrentItem(item);
                }
                m_pageUI.treeRibbon->expandItem(m_currentItemGroup);
            }
        }
        else
        {
            QMessageBox::warning(&p, p.windowTitle(), ARibbonBar::tr_compatible(RibbonAddCommandWarningString));
        }
    }
    else if (!m_currentSourceItemGroup)
    {
        if (ARibbonPage* page = m_currentSourceItemToPage.value(m_currentSourceItemPage))
        {
            int index = m_pageUI.treeCommands->indexOfTopLevelItem(m_currentSourceItemPage);
            QTreeWidgetItem* item = m_pageUI.treeCommands->topLevelItem(index + 1);
            item = item ? item : m_currentSourceItemPage;
            m_pageUI.treeCommands->setCurrentItem(item);

            ARibbonPage* newPage = addPageCustom(page);
            if (newPage)
                addGroupCopy(page, newPage);
        }
    }
    else if (m_currentSourceItemGroup)
    {
        ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
        Q_ASSERT(manager != nullptr);

        ARibbonGroup* group = m_currentSourceItemToGroup.value(m_currentSourceItemGroup);
        ARibbonPage* page = m_currentItemToPage.value(m_currentItemPage);

        if (group != nullptr && page != nullptr)
        {
            QTreeWidgetItem* itemParent = m_currentItemPage;
            int index = itemParent->childCount();
            QString titleGroup(group->title());
            ARibbonGroup* newGroup = manager->createGroup(page, titleGroup, index);
            newGroup->setIcon(group->icon());
            newGroup->setProperty(__aproch_Widget_Copy, QStringLiteral("__aproch_Group_Copy"));

            titleGroup.remove(QStringLiteral("&"));
            QTreeWidgetItem* item = new QTreeWidgetItem(QStringList(titleGroup));
            m_currentItemPage->insertChild(index, item);

            m_currentItemToGroup.insert(item, newGroup);

            QListIterator<QAction *> itAction(manager->actionsGroup(group));
            while (itAction.hasNext()) 
            {
                QAction* action = itAction.next();
                newGroup->addAction(action);
            }
            fillStateActions(newGroup, item, true);

            int indexNext = m_currentSourceItemPage->indexOfChild(m_currentSourceItemGroup) + 1;
            QTreeWidgetItem* nextItem = m_currentSourceItemPage->child(indexNext);
            nextItem = nextItem ? nextItem : m_currentSourceItemGroup;
            m_pageUI.treeCommands->setCurrentItem(nextItem);
            m_pageUI.treeRibbon->setCurrentItem(item);
        }
    }
    setButtons();
}

void ARibbonBarCustomizePagePrivate::newTabClicked()
{
    if (!m_currentItemPage)
        return;

    addPageCustom(nullptr);
    newGroupClicked();
    setButtons();
}

void ARibbonBarCustomizePagePrivate::newGroupClicked()
{
    if (!m_currentItemPage)
        return;

    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != nullptr);

    QString strNewGroup(ARibbonBar::tr_compatible(RibbonNewGroupString));
    ARibbonPage* currentPage = m_currentItemToPage.value(m_currentItemPage);
    Q_ASSERT(currentPage != nullptr);

    int index = 0;
    if (m_currentItemGroup)
    {
        ARibbonGroup* currentGroup = m_currentItemToGroup.value(m_currentItemGroup);
        Q_ASSERT(currentGroup != nullptr);

        QList<ARibbonGroup*> groups = manager->pageGroups(currentPage);
        index = groups.indexOf(currentGroup) + 1;
    }

    ARibbonGroup* newGroup = manager->createGroup(currentPage, strNewGroup, index);

    QStringList stringsGroup;
    strNewGroup += m_sufNameCustom;
    stringsGroup.append(strNewGroup);
    QTreeWidgetItem* item = new QTreeWidgetItem(stringsGroup);
    m_currentItemPage->insertChild(index, item);
    newGroup->setProperty(__aproch_Widget_Custom, QStringLiteral("__aproch_Group_Custom"));
    m_currentItemToGroup.insert(item, newGroup);
    m_pageUI.treeRibbon->setCurrentItem(item);
    setButtons();
}

void ARibbonBarCustomizePagePrivate::removeClicked()
{
    QTreeWidgetItem* currentItem = m_pageUI.treeRibbon->currentItem();
    if (!currentItem)
        return;

    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != nullptr);

    if (m_currentItemGroup && m_currentItemAction)
    {
        if (ARibbonGroup* group = m_currentItemToGroup.value(m_currentItemGroup))
        {
            if (group && group->property(__aproch_Widget_Custom).toString() == QStringLiteral("__aproch_Group_Custom"))
            {
                QTreeWidgetItem* currentParent = currentItem->parent();
                int index = currentParent->indexOfChild(currentItem);
                if (index == -1)
                    return;

                if (QAction* action = m_currentItemToAction.value(m_currentItemAction))
                {
                    m_actionToCurrentItem.remove(action);
                    m_currentItemToAction.remove(currentItem);
                    manager->removeActionAt(group, index);
                }
                delete currentItem;
            }
        }
    }
    else if (ARibbonGroup* group = m_currentItemToGroup.value(m_currentItemGroup))
    {
        QTreeWidgetItem* currentParent = currentItem->parent();
        int index = currentParent->indexOfChild(currentItem);
        if (index == -1)
            return;
        ARibbonPage* page = m_currentItemToPage.value(m_currentItemPage);
        Q_ASSERT(page != nullptr);

        manager->clearActions(group);
        manager->deleteGroup(page, index);
        m_currentItemToGroup.remove(currentItem);
        delete currentItem;
    }
    else if (ARibbonPage* page = m_currentItemToPage.value(m_currentItemPage))
    {
        int index = m_pageUI.treeRibbon->indexOfTopLevelItem(currentItem);
//        manager->removePage(page);
        manager->deletePage(page);
        m_pageToCurrentItem.remove(page);
        QListIterator<ARibbonGroup*> itGroup(manager->pageGroups(page));
        while (itGroup.hasNext()) 
            manager->clearActions(itGroup.next());

        m_currentItemToPage.remove(currentItem);
        delete currentItem;

        if (index == m_pageUI.treeRibbon->topLevelItemCount())
            index--;

        if (index >= 0) 
        {
            QTreeWidgetItem* item = m_pageUI.treeRibbon->topLevelItem(index);
            m_pageUI.treeRibbon->setCurrentItem(item);
        }
    }
    setButtons();
}

void ARibbonBarCustomizePagePrivate::resetClicked()
{
    if (m_ribbonBar)
    {
        ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
        Q_ASSERT(manager != NULL);
        manager->reset();
        fillStateRibbon(m_pageUI.switchTabsBox->currentText());
    }
}

template<class T>
QTreeWidgetItem* currentItem(const QMap<QTreeWidgetItem*, T*>& currentItemToObject, QTreeWidgetItem* current)
{
    if (current == nullptr)
        return nullptr;

    if (currentItemToObject.contains(current))
        return current;

    return currentItem(currentItemToObject, current->parent());
}

void ARibbonBarCustomizePagePrivate::upClicked()
{
    QTreeWidgetItem* currentItem = m_pageUI.treeRibbon->currentItem();
    if (!currentItem)
        return;

    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != NULL);

    if (m_currentItemGroup && m_currentItemAction)
    {
        QTreeWidgetItem* currentParent = currentItem->parent();
        int index = currentParent->indexOfChild(currentItem);
        if (!(index == -1 || index == 0))
        {
            currentParent->takeChild(index);
            int newIndex = index - 1;
            currentParent->insertChild(newIndex, currentItem);
            Q_ASSERT(m_currentItemPage != nullptr);
            QAction* action = m_currentItemToAction.value(currentItem);
            Q_ASSERT(action != nullptr);

            ARibbonGroup* group = m_currentItemToGroup.value(m_currentItemGroup);
            Q_ASSERT(group != nullptr);
            manager->removeActionAt(group, index);
            manager->insertAction(group, action, newIndex);

            m_pageUI.treeRibbon->setCurrentItem(currentItem);
        }
    }
    else if (m_currentItemToPage.contains(currentItem))
    {
        int index = m_pageUI.treeRibbon->indexOfTopLevelItem(currentItem);
        if (index != -1)
        {
            m_pageUI.treeRibbon->takeTopLevelItem(index);
            int newIndex = index - 1;
            m_pageUI.treeRibbon->insertTopLevelItem(newIndex, currentItem);
            manager->movePage(index, newIndex);
            m_pageUI.treeRibbon->setCurrentItem(currentItem);
        }
    }
    else if (m_currentItemToGroup.contains(currentItem))
    {
        QTreeWidgetItem* currentParent = currentItem->parent();
        int index = currentParent->indexOfChild(currentItem);
        if (!(index == -1 || index == 0))
        {
            currentParent->takeChild(index);
            int newIndex = index - 1;
            currentParent->insertChild(newIndex, currentItem);
            Q_ASSERT(m_currentItemPage != nullptr);
            ARibbonPage* page = m_currentItemToPage.value(m_currentItemPage);
            Q_ASSERT(page != nullptr);
            manager->moveGroup(page, index, newIndex);
            m_pageUI.treeRibbon->setCurrentItem(currentItem);
        }
    }

    setButtons();
}

void ARibbonBarCustomizePagePrivate::downClicked()
{
    QTreeWidgetItem* currentItem = m_pageUI.treeRibbon->currentItem();
    if (!currentItem)
        return;

    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != NULL);

    if (m_currentItemGroup && m_currentItemAction)
    {
        QTreeWidgetItem* currentParent = currentItem->parent();
        int index = currentParent->indexOfChild(currentItem);
        if (index != currentParent->childCount() - 1)
        {
            currentParent->takeChild(index);
            int newIndex = index + 1;
            currentParent->insertChild(newIndex, currentItem);
            Q_ASSERT(m_currentItemPage != nullptr);
            QAction* action = m_currentItemToAction.value(currentItem);
            Q_ASSERT(action != nullptr);

            ARibbonGroup* group = m_currentItemToGroup.value(m_currentItemGroup);
            Q_ASSERT(group != nullptr);
            manager->removeActionAt(group, index);
            manager->insertAction(group, action, newIndex);
        }
    }
    else if (m_currentItemToPage.contains(currentItem))
    {
        int index = m_pageUI.treeRibbon->indexOfTopLevelItem(currentItem);
        if (index != m_pageUI.treeRibbon->topLevelItemCount() - 1)
        {
            m_pageUI.treeRibbon->takeTopLevelItem(index);
            int newIndex = index + 1;
            m_pageUI.treeRibbon->insertTopLevelItem(newIndex, currentItem);
            manager->movePage(index, newIndex);
        }
    }
    else if (m_currentItemToGroup.contains(currentItem))
    {
        QTreeWidgetItem* currentParent = currentItem->parent();
        int index = currentParent->indexOfChild(currentItem);
        if (index != currentParent->childCount() - 1)
        {
            currentParent->takeChild(index);
            int newIndex = index + 1;
            currentParent->insertChild(newIndex, currentItem);
            Q_ASSERT(m_currentItemPage != nullptr);
            ARibbonPage* page = m_currentItemToPage.value(m_currentItemPage);
            Q_ASSERT(page != nullptr);
            manager->moveGroup(page, index, newIndex);
        }
    }
    m_pageUI.treeRibbon->setCurrentItem(currentItem);
    setButtons();
}

void ARibbonBarCustomizePagePrivate::renameClicked()
{
    A_P(ARibbonBarCustomizePage);
    class RenameDialog : public QDialog
    {
    public:
        RenameDialog(QWidget* parent, const QString& currentName) : QDialog(parent) 
        {
            setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
            m_renameDialogUI.setupUi(this);
            m_renameDialogUI.lineEditName->setText(currentName);
        }
        QString currentName() const
        {
            return m_renameDialogUI.lineEditName->text();
        }
    protected:
        Ui::RibbonRenameDialog m_renameDialogUI;
    };

    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != nullptr);

    QTreeWidgetItem* currentItem = m_pageUI.treeRibbon->currentItem();
    if (!currentItem)
        return;

    QString currentName;
    ARibbonPage* page = m_currentItemToPage.value(currentItem);
    ARibbonGroup* group = m_currentItemToGroup.value(currentItem);
    if (page)
    {
        if (!manager->pageName(page).isEmpty())
            currentName = manager->pageName(page);
        else
            currentName = page->title();
    }
    else if (group)
    {
        if (!manager->groupName(group).isEmpty())
            currentName = manager->groupName(group);
        else
            currentName = group->title();
    }

    if (!currentName.isEmpty())
    {
        RenameDialog renameDialog(&p, currentName);
        if (renameDialog.exec() == QDialog::Accepted)
        {
            QString currentText = renameDialog.currentName();
            if (page != nullptr )
            {
                manager->setPageName(page, currentText);
                currentText.remove(QStringLiteral("&"));
                if (page->property(__aproch_Widget_Custom).toString() == QStringLiteral("__aproch_Page_Custom"))
                    currentText += m_sufNameCustom;
                currentItem->setText(0, currentText);
            }
            else if (group != nullptr)
            {
                manager->setGroupName(group, currentText);
                currentText.remove(QStringLiteral("&"));
                if (group->property(__aproch_Widget_Custom).toString() == QStringLiteral("__aproch_Group_Custom"))
                    currentText += m_sufNameCustom;
                currentItem->setText(0, currentText);
            }
        }
    }
}

void ARibbonBarCustomizePagePrivate::setCurrentCategoryText(const QString& strCategory)
{
    if (strCategory.isEmpty())
        return;

    if (m_strSeparator == strCategory)
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        m_pageUI.comboBoxSourceCategory->setCurrentText(m_currentSourceCategory);
#else
        int index = m_pageUI.comboBoxSourceCategory->findText(m_currentSourceCategory);
        if (index != -1)
            m_pageUI.comboBoxSourceCategory->setCurrentIndex(index);
#endif
        return;
    }

    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != NULL);

    m_currentSourceItemToPage.clear();
    m_currentSourceItemToGroup.clear();
    m_currentSourceItemToAction.clear();

    m_pageUI.treeCommands->clear();
    clearCategoryCaptionTree(m_pageUI.treeCommands);

    QList<QAction*> actions = manager->actionsByCategory(strCategory);
    if (actions.size() > 0)
    {
        m_pageUI.treeCommands->setRootIsDecorated(false);
        fillSourceActions(actions);
    }
    else
    {
        QList<ARibbonPage*> pages = manager->pagesByCategory(strCategory);
        if (pages.size() > 0)
        {
            m_pageUI.treeCommands->setRootIsDecorated(true);
            fillSourcePages(pages, strCategory);
        }
    }
    m_currentSourceCategory = strCategory;
    setButtons();
}

void ARibbonBarCustomizePagePrivate::setCurrentTabsText(const QString& strCategory)
{
//    if (strCategory.isEmpty())
//        return;
    fillStateRibbon(strCategory);
}

void ARibbonBarCustomizePagePrivate::currentSourceChanged(QTreeWidgetItem* current)
{
    m_currentSourceItemPage = ::currentItem(m_currentSourceItemToPage, current);
    m_currentSourceItemGroup = ::currentItem(m_currentSourceItemToGroup, current);
    m_currentSourceAction = ::currentItem(m_currentSourceItemToAction, current);
    setButtons();
}

void ARibbonBarCustomizePagePrivate::currentPageChanged(QTreeWidgetItem* current)
{
    m_currentItemPage = ::currentItem(m_currentItemToPage, current);
    m_currentItemGroup = ::currentItem(m_currentItemToGroup, current);
    m_currentItemAction = (m_currentItemGroup && m_currentItemGroup != current) ? current : nullptr;
    setButtons();
}

void ARibbonBarCustomizePagePrivate::itemPageChanged(QTreeWidgetItem* itemPage, int column)
{
    if (m_currentItemToPage.empty())
        return;

    ARibbonCustomizeManager* manager = m_ribbonBar->customizeManager();
    Q_ASSERT(manager != nullptr);

    ARibbonPage* page = m_currentItemToPage.value(itemPage);
    if (page == nullptr)
        return;

    if (itemPage->checkState(column) == Qt::Checked)
    {
        if (manager->isPageHidden(page))
            m_ribbonBar->customizeManager()->setPageHidden(page, false);
    }
    else if (itemPage->checkState(column) == Qt::Unchecked)
    {
        if (!manager->isPageHidden(page))
            m_ribbonBar->customizeManager()->setPageHidden(page, true);
    }
}

/* ARibbonBarCustomizePage */
ARibbonBarCustomizePage::ARibbonBarCustomizePage(ARibbonBar* ribbonBar)
    : QWidget(nullptr)
{
    A_INIT_PRIVATE(ARibbonBarCustomizePage);
    A_D(ARibbonBarCustomizePage);
    d.m_ribbonBar = ribbonBar;
    d.init();
}

ARibbonBarCustomizePage::~ARibbonBarCustomizePage()
{
    A_DELETE_PRIVATE();
}

ARibbonBar* ARibbonBarCustomizePage::ribbonBar() const
{
    A_D(const ARibbonBarCustomizePage);
    return d.m_ribbonBar;
}

void ARibbonBarCustomizePage::addCustomCategory( const QString& strCategory )
{
    A_D(ARibbonBarCustomizePage);
    d.m_sourceCategories.append(strCategory);
}

void ARibbonBarCustomizePage::addSeparatorCategory(const QString& strCategory)
{
    A_D(ARibbonBarCustomizePage);
    QString separator = strCategory;
    if (separator.isEmpty())
        separator = QStringLiteral("----------");
    d.m_strSeparator = separator;
    d.m_sourceCategories.append(separator);
}

void ARibbonBarCustomizePage::enableModifyPage(bool bEnable)
{
	A_D(ARibbonBarCustomizePage);
	d.enableVisualizePage(bEnable);
	d.enableAddPage(bEnable);
	d.enableRenamePage(bEnable);
	d.enableMovePage(bEnable);
}

void ARibbonBarCustomizePage::accepted()
{
    A_D(ARibbonBarCustomizePage);
    if (d.m_wasDisplayed)
        d.applyClicked();
}

void ARibbonBarCustomizePage::rejected()
{
    A_D(ARibbonBarCustomizePage);
    d.cancelClicked();
}

void ARibbonBarCustomizePage::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    A_D(ARibbonBarCustomizePage);
    d.m_wasDisplayed = true;
    d.setupPage();
}

void ARibbonBarCustomizePage::hideEvent(QHideEvent* event)
{
    QWidget::hideEvent(event);
}

APROCH_NAMESPACE_END