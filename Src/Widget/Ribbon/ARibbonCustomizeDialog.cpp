/****************************************************************************
 * @file    ARibbonCustomizeDialog.cpp
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
#include <QApplication>
#include <QScreen>
#include <QPainter>
#include <QVBoxLayout>

#include "ARibbonCustomizeDialog.h"
#include "ARibbonCustomizeDialog_p.h"
#include "ARibbonCustomizeManager.h"
#include "ARibbonCustomizePage.h"
#include "Platform/APlatform.h"
#include "ARibbonDef.h"

// 
// The most of the following code is copied from Qtitan.
// 
// Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
// Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
// 

APROCH_NAMESPACE_BEGIN

static QWidget* aproch_WantTheFocus(QWidget* ancestor)
{
  const int MaxIterations = 100;

  QWidget* candidate = ancestor;
  for (int i = 0; i < MaxIterations; ++i) 
  {
    candidate = candidate->nextInFocusChain();
    if (!candidate)
      break;

    if (candidate->focusPolicy() & Qt::TabFocus) 
    {
      if (candidate != ancestor && ancestor->isAncestorOf(candidate))
        return candidate;
    }
  }
  return 0;
}

// TODO

/* ARibbonCustomizePageWidget */
class ARibbonCustomizePageWidget : public QWidget
{
public:
    explicit ARibbonCustomizePageWidget(QWidget* parent);
    virtual ~ARibbonCustomizePageWidget();
public:
    QWidget* content() const;

    /*
    * @brief 设置自定义页签控件(支持加入图标和文本·)
    * @param [in] content 内容
    */
    void setContent(QWidget* content);

    /*
    * @brief 设置自定义页签控件(支持加入图标和文本·)
    * @param [in] content 内容
    */
    void setContent(QWidget* content,bool bText,bool bIcon);
protected:
    QHBoxLayout* m_hBoxLayout;
    QVBoxLayout* m_vBoxLayout;
    QLabel m_labelIcon;
    QLabel m_labelTitle;
    QWidget* m_content;
private:
    friend class ARibbonCustomizeDialog;
    Q_DISABLE_COPY(ARibbonCustomizePageWidget)
};

/* ARibbonCustomizePageWidget */
ARibbonCustomizePageWidget::ARibbonCustomizePageWidget(QWidget* parent)
    : QWidget(parent)
{
    m_content = nullptr;
    QFont font = m_labelTitle.font();
    font.setBold(true);
    m_labelTitle.setFont(font);
    m_hBoxLayout = new QHBoxLayout();
    m_hBoxLayout->addWidget(&m_labelIcon);
    m_hBoxLayout->addWidget(&m_labelTitle,1);
    m_vBoxLayout = new QVBoxLayout(this);
    m_vBoxLayout->addLayout(m_hBoxLayout);
}

ARibbonCustomizePageWidget::~ARibbonCustomizePageWidget()
{
}

QWidget* ARibbonCustomizePageWidget::content() const
{
    return m_content;
}

void ARibbonCustomizePageWidget::setContent(QWidget* content)
{
	if (content == nullptr)
	{
		Q_ASSERT(false);
		return;
	}
	QIcon icon = content->windowIcon();
	m_labelIcon.setPixmap(icon.isNull() ? QPixmap(QStringLiteral(":/res/qtitanlogo32x32.png")) : icon.pixmap(QSize(32, 32)));
    m_labelIcon.setVisible(true);
	m_labelTitle.setText(content->statusTip());
	m_labelTitle.setVisible(true);
	m_vBoxLayout->addWidget(content);
	m_content = content;
}

void ARibbonCustomizePageWidget::setContent(QWidget* content, bool bText, bool bIcon)
{
    if (content == nullptr)
    {
        Q_ASSERT(false);
        return;
    }

    if (bIcon)
    {
        QIcon icon = content->windowIcon();
        m_labelIcon.setPixmap(icon.isNull() ? QPixmap(QStringLiteral(":/res/qtitanlogo32x32.png")) : icon.pixmap(QSize(32, 32)));
    }
    else
    {
        m_labelIcon.setVisible(false);
    }
    if (bText)
    {
        m_labelTitle.setText(content->statusTip());
		m_labelTitle.setVisible(true);
    }
    else
    {
        m_labelTitle.setVisible(false);
    }
    m_vBoxLayout->addWidget(content);
    m_content = content;
}

/* ARibbonCustomizeListWidget */
ARibbonCustomizeListWidget::ARibbonCustomizeListWidget(QWidget* parent)
    : QListWidget(parent)
{
}

ARibbonCustomizeListWidget::~ARibbonCustomizeListWidget()
{
}

QSize ARibbonCustomizeListWidget::sizeHint() const
{
    QSize sz = QListWidget::sizeHint();
    QStyleOption opt;
    opt.initFrom(this);

    int maxWidth = 0;
    for (int row = 0, countRow = count(); countRow > row; row++)
    {
        if (QListWidgetItem* rbItem = item(row))
        {
            QString str = rbItem->text() + QStringLiteral("XX");
            int width = opt.fontMetrics.boundingRect(str).width();
            maxWidth = qMax(maxWidth, width);
        }
    }
    return QSize(maxWidth, sz.height());
}

QSize ARibbonCustomizeListWidget::minimumSizeHint() const
{
    QSize sz = QListWidget::minimumSizeHint();
    sz.setWidth( sizeHint().width() );
    return sz;
}

int m_currentPage;
QVBoxLayout* m_pageVBoxLayout;


/* ARibbonCustomizeDialogPrivate */
ARibbonCustomizeDialogPrivate::ARibbonCustomizeDialogPrivate()
    : m_verticalLayout(nullptr)
    , m_horizontalLayout(nullptr)
    , m_listPage(nullptr)
    , m_pageFrame(nullptr)
    , m_buttonBox(nullptr)
    , m_currentPage(0)
    , m_pageVBoxLayout(nullptr)
{
}

ARibbonCustomizeDialogPrivate::~ARibbonCustomizeDialogPrivate()
{
}

void ARibbonCustomizeDialogPrivate::init()
{
    A_P(ARibbonCustomizeDialog);
    p.setWindowTitle(ARibbonBar::tr_compatible(RibbonCustomizeDialogOptionsString));

    QRect geom = aproch_availableGeometry(&p);
    p.resize(2 * geom.width() / 5, 2 * geom.height() / 5);

    m_verticalLayout = new QVBoxLayout(&p);
    m_horizontalLayout = new QHBoxLayout();
    m_horizontalLayout->setSpacing(1);
    m_listPage = new ARibbonCustomizeListWidget(&p);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(m_listPage->sizePolicy().hasHeightForWidth());
    m_listPage->setSizePolicy(sizePolicy);
    m_listPage->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_horizontalLayout->addWidget(m_listPage, 0, Qt::AlignLeft);

    m_pageFrame = new QFrame(&p);
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(m_pageFrame->sizePolicy().hasHeightForWidth());
    m_pageFrame->setSizePolicy(sizePolicy1);
    m_pageFrame->setAutoFillBackground(false);
    m_pageFrame->setFrameShape(QFrame::StyledPanel);
    m_pageFrame->setFrameShadow(QFrame::Sunken);

    m_horizontalLayout->addWidget(m_pageFrame);

    m_horizontalLayout->setStretch(1, 1);

    m_verticalLayout->addLayout(m_horizontalLayout);

    m_buttonBox = new QDialogButtonBox(&p);
    m_buttonBox->setOrientation(Qt::Horizontal);
    m_buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    m_verticalLayout->addWidget(m_buttonBox);

    QObject::connect(m_buttonBox, SIGNAL(accepted()), &p, SLOT(accept()), Qt::DirectConnection);
    QObject::connect(m_buttonBox, SIGNAL(rejected()), &p, SLOT(reject()), Qt::DirectConnection);

    m_verticalLayout->setContentsMargins(2, 2, 2, 6);

    m_pageVBoxLayout = new QVBoxLayout(m_pageFrame);
    m_pageVBoxLayout->setSpacing(0);
    m_pageVBoxLayout->addSpacing(0);
    QSpacerItem *spacerItem = new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding);
    m_pageVBoxLayout->addItem(spacerItem);
    connect(m_listPage, SIGNAL(currentRowChanged(int)), this, SLOT(switchToPage(int)));
}

void ARibbonCustomizeDialogPrivate::setCustomizeMode(bool edit)
{
    for (int index = 0, count = m_listWidget.count(); count > index; ++index)
    {
        if (ARibbonBarCustomizePage* page = qobject_cast<ARibbonBarCustomizePage*>(m_listWidget.at(index)))
        {
            page->ribbonBar()->customizeManager()->setEditMode(edit);
            break;
        }
        else if (ARibbonQuickAccessBarCustomizePage* page = qobject_cast<ARibbonQuickAccessBarCustomizePage*>(m_listWidget.at(index)))
        {
            page->ribbonBar()->customizeManager()->setEditMode(edit);
            break;
        }
    }
}

void ARibbonCustomizeDialogPrivate::addPage(ARibbonCustomizePageWidget* page)
{
    if (!page) 
    {
        qWarning("ARibbonCustomizeDialogPrivate::addPage: Cannot insert null page");
        return;
    }
    page->setParent(m_pageFrame);

    int n = m_pageVBoxLayout->count();
    // disable layout to prevent layout updates while adding
    bool pageVBoxLayoutEnabled = m_pageVBoxLayout->isEnabled();
    m_pageVBoxLayout->setEnabled(false);

    m_pageVBoxLayout->insertWidget(n - 1, page);

    // hide new page and reset layout to old status
    page->hide();
    m_pageVBoxLayout->setEnabled(pageVBoxLayoutEnabled);
    ARibbonCustomizePageItem* item = new ARibbonCustomizePageItem(page->windowTitle(), page);
    m_listPage->addItem(item);
}

void ARibbonCustomizeDialogPrivate::insertPage(int index, ARibbonCustomizePageWidget* page)
{
    if (!page) 
    {
        qWarning("ARibbonCustomizeDialogPrivate::insertPage: Cannot insert null page");
        return;
    }

    ARibbonCustomizePageItem* posItem = static_cast<ARibbonCustomizePageItem*>(m_listPage->item(index));
    int indexItem = m_listPage->row(posItem);
    if (indexItem == -1)
        return;

    page->setParent(m_pageFrame);

    int n = m_pageVBoxLayout->count();
    // disable layout to prevent layout updates while adding
    bool pageVBoxLayoutEnabled = m_pageVBoxLayout->isEnabled();
    m_pageVBoxLayout->setEnabled(false);

    m_pageVBoxLayout->insertWidget(n - 1, page);

    // hide new page and reset layout to old status
    page->hide();
    m_pageVBoxLayout->setEnabled(pageVBoxLayoutEnabled);

    ARibbonCustomizePageItem* item = new ARibbonCustomizePageItem(page->windowTitle(), page);
    m_listPage->insertItem(indexItem, item);
}

void ARibbonCustomizeDialogPrivate::switchToPage(int currentRow)
{
    if (m_currentPage != currentRow)
    {
        if (ARibbonCustomizePageItem* item = static_cast<ARibbonCustomizePageItem*>(m_listPage->item(m_currentPage)))
        {
            ARibbonCustomizePageWidget* page = item->m_page;
            page->hide();
            page->parentWidget()->update();
        }
    }

    m_currentPage = currentRow;

    if (ARibbonCustomizePageItem* item = static_cast<ARibbonCustomizePageItem*>(m_listPage->item(m_currentPage)))
    {
        ARibbonCustomizePageWidget* page = item->m_page;
        page->show();

        if (QWidget* candidate = aproch_WantTheFocus(page))
          candidate->setFocus();

        bool expandPage = !page->layout();
        if (!expandPage) 
        {
          const QLayoutItem *pageItem = m_pageVBoxLayout->itemAt(m_pageVBoxLayout->indexOf(page));
          expandPage = pageItem->expandingDirections() & Qt::Vertical;
        }
        QSpacerItem *bottomSpacer = m_pageVBoxLayout->itemAt(m_pageVBoxLayout->count() - 1)->spacerItem();
        Q_ASSERT(bottomSpacer);
        bottomSpacer->changeSize(0, 0, QSizePolicy::Ignored, expandPage ? QSizePolicy::Ignored : QSizePolicy::MinimumExpanding);
        m_pageVBoxLayout->invalidate();
        page->parentWidget()->repaint();
    }
}

/*!
\class ARibbonCustomizeDialog
\inmodule QtitanRibbon
\brief ARibbonCustomizeDialog implements dialog to configure actions, pages, groups that are located at ARibbonBar.
\sa ARibbonBar::customizeDialog(), ARibbonBar::showCustomizeDialog()
*/
ARibbonCustomizeDialog::ARibbonCustomizeDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    A_INIT_PRIVATE(ARibbonCustomizeDialog);
    A_D(ARibbonCustomizeDialog);
    d.init();
}

ARibbonCustomizeDialog::~ARibbonCustomizeDialog()
{
    A_DELETE_PRIVATE();
}

/*!
Adds a widget to the dialogue as a setting \a page. 
*/ 
void ARibbonCustomizeDialog::addPage(QWidget* page)
{
    A_D(ARibbonCustomizeDialog);
    Q_ASSERT(page != nullptr);
    ARibbonCustomizePageWidget* pageWidget = new ARibbonCustomizePageWidget(this);
    pageWidget->setContent(page,false,false);
    pageWidget->setWindowTitle(page->windowTitle());

    if (const QMetaObject* mo = page->metaObject())
    {
        if (mo->indexOfSlot("accepted()") != -1)
            connect(this, SIGNAL(accepted()), page, SLOT(accepted()), Qt::DirectConnection);
        if (mo->indexOfSlot("rejected()") != -1)
            connect(this, SIGNAL(rejected()), page, SLOT(rejected()), Qt::DirectConnection);
    }

    d.m_listWidget.append(page);
    d.addPage(pageWidget);
}

/*!
Inserts a widget to the dialogue in a position \a index as a setting \a page.
*/
void ARibbonCustomizeDialog::insertPage(int index, QWidget* page)
{
    A_D(ARibbonCustomizeDialog);
    Q_ASSERT(page != nullptr);
    ARibbonCustomizePageWidget* pageWidget = new ARibbonCustomizePageWidget(this);
    pageWidget->setContent(page);
    pageWidget->setWindowTitle(page->windowTitle());
    connect(this, SIGNAL(accepted()), page, SLOT(accepted()));

    d.m_listWidget.insert(index, page);
    d.insertPage(index, pageWidget);
}

/*!
Returns index of the \a page.
*/
int ARibbonCustomizeDialog::indexOf(QWidget* page) const
{
    A_D(const ARibbonCustomizeDialog);
    return d.m_listWidget.indexOf(page);
}

/*!
Returns a \a page that is active at the moment.
*/
QWidget* ARibbonCustomizeDialog::currentPage() const
{
    A_D(const ARibbonCustomizeDialog);
    return d.m_listWidget.at(d.m_currentPage);
}

/*!
Makes the \a page an active.
*/
void ARibbonCustomizeDialog::setCurrentPage(QWidget* page)
{
    int index = indexOf(page);
    if (index != -1)
        setCurrentPageIndex(index);
}

/*!
Returns an index of the page that is active at the moment.
*/
int ARibbonCustomizeDialog::currentPageIndex() const
{
    A_D(const ARibbonCustomizeDialog);
    return d.m_currentPage;
}

/*!
Makes the page with the \a index an active.
*/
void ARibbonCustomizeDialog::setCurrentPageIndex(int index)
{
    A_D(ARibbonCustomizeDialog);

    if (index == -1)
    {
        Q_ASSERT(false);
        return;
    }
    d.m_currentPage = index;
    d.m_listPage->setCurrentRow(index);

    if (ARibbonCustomizePageItem* posItem = static_cast<ARibbonCustomizePageItem*>(d.m_listPage->item(index)))
    {
        if (posItem->m_page)
            posItem->m_page->setVisible(true);
    }
}

/*!
Returns a count of the pages in the dialogue.
*/
int ARibbonCustomizeDialog::pageCount() const
{
    A_D(const ARibbonCustomizeDialog);
    return d.m_listWidget.count();
}

/*!
Returns a page by its \a index.
*/
QWidget* ARibbonCustomizeDialog::pageByIndex(int index) const
{
    A_D(const ARibbonCustomizeDialog);
    return d.m_listWidget[index];
}

/*! \reimp */
void ARibbonCustomizeDialog::accept()
{
    A_D(ARibbonCustomizeDialog);
    QDialog::accept();
    d.setCustomizeMode(false);
}

/*! \reimp */
void ARibbonCustomizeDialog::reject()
{
    A_D(ARibbonCustomizeDialog);
    QDialog::reject();
    d.setCustomizeMode(false);
}

/*! \reimp */
void ARibbonCustomizeDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    A_D(ARibbonCustomizeDialog);
    d.setCustomizeMode(true);
    if (d.m_currentPage != -1)
        setCurrentPageIndex(d.m_currentPage);
}

/*! \reimp */
void ARibbonCustomizeDialog::hideEvent(QHideEvent* event)
{
    A_D(ARibbonCustomizeDialog);
    QDialog::hideEvent(event);
    for (int index = 0, count = d.m_listPage->count(); count > index; ++index)
    {
        if (ARibbonCustomizePageItem* posItem = static_cast<ARibbonCustomizePageItem*>(d.m_listPage->item(index)))
        {
            if (posItem->m_page)
                posItem->m_page->setVisible(false);
        }
    }
}

APROCH_NAMESPACE_END