#include "WinUIWindow.h"
#include "AprochAPI.h"

#include <QRandomGenerator>
#include <QSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include <QComboBox>

#include <QApplication>
#include <QClipboard>

WinUIWindow::WinUIWindow(QWidget *parent)
    : ARibbonMainWindow(parent)
{
    ui.setupUi(this);

    createRibbonBar();
    //createStatusBar();
    //createDockWindows();
}

WinUIWindow::~WinUIWindow()
{
}

void WinUIWindow::createRibbonBar()
{
    //  We strongly recommend escaping the ribbon bar initialization using beginUpdate()/endUpdate() methods. 
    //  This significantly increase the speed of the application startup.
    ribbonBar()->beginUpdate();

    /*connect(ribbonBar(), SIGNAL(showRibbonContextMenu(QMenu*, QContextMenuEvent*)),
            this, SLOT(showRibbonContextMenu(QMenu*, QContextMenuEvent*)));*/
    ribbonBar()->setFrameThemeEnabled();

    ARibbonPage* m_pageHome = ribbonBar()->addPage(tr("&Home"));
    createGroupClipboard(m_pageHome);

    ARibbonPage* m_pageHome2 = ribbonBar()->addPage(tr("&View"));
    createGroupClipboard(m_pageHome2);

    ribbonBar()->endUpdate();
}

void WinUIWindow::createGroupClipboard(ARibbonPage* page)
{
    ARibbonGroup* groupClipboard = page->addGroup(tr("Clipboard"));
    groupClipboard->setOptionButtonVisible();
    QAction* act = groupClipboard->optionButtonAction();
    act->setText(tr("Clipboard"));
    act->setIcon(QIcon(QStringLiteral(":/res/clipboardToolTip.png")));
    act->setToolTip(tr("Show the Office clipboard Task Pane"));
    act->setStatusTip(tr("Show the Office clipboard Task Pane"));
    connect(act, SIGNAL(triggered()), this, SLOT(optionClipboard()));

    QMenu* editPaste = new QMenu(ribbonBar());
    QAction* m_actionPaste1 = editPaste->addAction(tr("&Edit"));
    m_actionPaste1->setPriority(QAction::LowPriority);
    m_actionPaste1->setShortcut(QKeySequence::Paste);

    editPaste->addAction(tr("Paste Special"));

    QAction* m_actionPaste2 = groupClipboard->addAction(QIcon(), tr("&Paste"), Qt::ToolButtonTextUnderIcon, editPaste);
    ARibbonControl* control = groupClipboard->controlByAction(m_actionPaste2);
    control->sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setImageSize(ARibbonControlSizeDefinition::ImageLarge);
    control->sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setImageSize(ARibbonControlSizeDefinition::ImageLarge);
    control->sizeDefinition(ARibbonControlSizeDefinition::GroupSmall)->setImageSize(ARibbonControlSizeDefinition::ImageLarge);
    control->sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setWordWrap(false);

    m_actionPaste2->setPriority(QAction::LowPriority);
    m_actionPaste2->setShortcut(QKeySequence::Paste);
    m_actionPaste2->setToolTip(tr("Insert Clipboard contents"));
    m_actionPaste1->setEnabled(!QApplication::clipboard()->text().isEmpty());
    m_actionPaste2->setEnabled(!QApplication::clipboard()->text().isEmpty());

    QAction* m_actionCut = groupClipboard->addAction(QIcon(QStringLiteral(":/res/smallcut.png")), tr("&Cut"), Qt::ToolButtonTextBesideIcon);
    control = groupClipboard->controlByAction(m_actionCut);
    control->sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
    control->sizeDefinition(ARibbonControlSizeDefinition::GroupSimplified)->setVisible(false);
    control->sizeDefinition(ARibbonControlSizeDefinition::GroupSimplifiedOverflow)->setVisible(true);
    m_actionCut->setShortcut(QKeySequence::Cut);
    m_actionCut->setToolTip(tr("Cut the selection and put it on the Clipboard"));

    QAction* m_actionCopy = groupClipboard->addAction(QIcon(QStringLiteral(":/res/smallcopy.png")), tr("&Copy"), Qt::ToolButtonTextBesideIcon);
    control = groupClipboard->controlByAction(m_actionCopy);
    control->sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
    control->sizeDefinition(ARibbonControlSizeDefinition::GroupSimplified)->setVisible(false);
    control->sizeDefinition(ARibbonControlSizeDefinition::GroupSimplifiedOverflow)->setVisible(true);
    m_actionCopy->setShortcut(QKeySequence::Copy);

    QAction* m_actionFormatPointerAction = groupClipboard->addAction(QIcon(QStringLiteral(":/res/smallformatpainter.png")), tr("F&ormat Pointer"), Qt::ToolButtonTextBesideIcon);
    control = groupClipboard->controlByAction(m_actionFormatPointerAction);
    control->sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
    control->sizeDefinition(ARibbonControlSizeDefinition::GroupSimplified)->setImageSize(ARibbonControlSizeDefinition::ImageSmall);
    control->sizeDefinition(ARibbonControlSizeDefinition::GroupSimplified)->setLabelVisible(false);
}